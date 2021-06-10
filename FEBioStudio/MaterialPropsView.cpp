/*This file is part of the FEBio Studio source code and is licensed under the MIT license
listed below.

See Copyright-FEBio-Studio.txt for details.

Copyright (c) 2020 University of Utah, The Trustees of Columbia University in 
the City of New York, and others.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

#include "stdafx.h"
#include "MaterialPropsView.h"
#include <MeshTools/GMaterial.h>
#include <FEMLib/FEMaterial.h>
#include <FEMLib/FEMultiMaterial.h>
#include <QPainter>
#include <QLineEdit>
#include <QComboBox>
#include "units.h"
#include "PropertyList.h"
#include <MeshTools/FEModel.h>


QStringList GetEnumValues(FEModel* fem, const char* ch)
{
	QStringList ops;
	char sz[256] = { 0 };
	if (ch[0] == '$')
	{
		if (fem)
		{
			fem->GetVariableNames(ch, sz);
			ch = sz;
		}
		else ch = 0;
	}

	while (ch && (*ch))
	{
		ops << QString(ch);
		ch = ch + strlen(ch) + 1;
	}

	return ops;
}

//-----------------------------------------------------------------------------
CEditVariableParam::CEditVariableParam(QWidget* parent) : QComboBox(parent)
{
	addItem("<constant>");
	addItem("<math>");
	addItem("<map>");

	setEditable(true);
	setInsertPolicy(QComboBox::NoInsert);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	m_param = nullptr;

	QObject::connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(onCurrentIndexChanged(int)));
}

void CEditVariableParam::setParam(Param* p)
{
	m_param = p;
	if (p == nullptr) return;

	blockSignals(true);
	if (p->GetParamType() == Param_Type::Param_FLOAT)
	{
		setCurrentIndex(0);
		setEditText(QString("%1").arg(p->GetFloatValue()));
	}
	else if (p->GetParamType() == Param_Type::Param_MATH)
	{
		setCurrentIndex(1);
		setEditText(QString::fromStdString(p->GetMathString()));
	}
	else if (p->GetParamType() == Param_Type::Param_STRING)
	{
		setCurrentIndex(2);
		setEditText(QString::fromStdString(p->GetStringValue()));
	}
	else
	{
		assert(false);
	}
	blockSignals(false);
}

void CEditVariableParam::onCurrentIndexChanged(int index)
{
	if (m_param == nullptr) return;

	if (index == 0) m_param->SetParamType(Param_FLOAT);
	if (index == 1) m_param->SetParamType(Param_MATH);
	if (index == 2) m_param->SetParamType(Param_STRING);

	setParam(m_param);

	emit typeChanged();
}

class CMaterialPropsModel : public QAbstractItemModel
{
public:
	class Item
	{
	public:
		enum Flags {
			Item_Bold = 1,
			Item_Indented = 2
		};

	public:
		CMaterialPropsModel*	m_model;

		FEMaterial*		m_pm;			// material pointer
		int				m_paramId;	// index of parameter (or -1 if this is property)
		int				m_propId;		// index of property (or -1 if this is parameter)
		int				m_matIndex;	// index into property's material array

		int		m_nrow;	// row index into parent's children array

		int		m_flag;	// used to decided if the item will show up as bold or not. Default: parameters = no, properties = yes. 

	public:
		Item*			m_parent;		// pointer to parent
		vector<Item*>	m_children;	// list of children

	public:
		Item() { m_model = nullptr; m_pm = nullptr; m_parent = nullptr; m_paramId = -1; m_propId = -1; m_matIndex = 0; m_nrow = -1; m_flag = 0;  }
		Item(FEMaterial* pm, int paramId = -1, int propId = -1, int matIndex = 0, int nrow = -1) {
			m_model = nullptr;
			m_pm = pm; m_paramId = paramId; m_propId = propId; m_matIndex = matIndex; m_nrow = nrow;
			m_flag = (propId != -1 ? 1 : 0);
		}
		~Item() { for (int i = 0; i < m_children.size(); ++i) delete m_children[i]; m_children.clear(); }

		bool isParameter() const { return (m_paramId >= 0); }
		bool isProperty() const { return (m_propId >= 0); }

		int flag() const { return m_flag; }

		Param* parameter() { return (m_paramId >= 0 ? m_pm->GetParamPtr(m_paramId) : nullptr); }

		Item* addChild(FEMaterial* pm, int paramId, int propId, int matIndex)
		{
			Item* item = new Item(pm, paramId, propId, matIndex, (int)m_children.size());
			item->m_model = m_model; assert(m_model);
			item->m_parent = this;
			m_children.push_back(item);

			if (propId >= 0)
			{
				FEMaterialProperty& p = pm->GetProperty(propId);
				FEMaterial* pm = p.GetMaterial(matIndex);
				if (pm) item->addChildren(pm);
			}
			return item;
		}

		void addFiberParameters(FEOldFiberMaterial* pm)
		{
			pm->UpdateData(false);
			for (int i = 0; i < pm->Parameters(); ++i)
			{
				Param& p = pm->GetParam(i);
				if ((p.IsVisible() || p.IsEditable()) && (p.IsPersistent() || (m_pm == nullptr)))
				{
					Item* it = addChild(pm, i, -1, 0);
					if (i == 0) it->m_flag = Item_Bold;
					else it->m_flag = Item_Indented;
				}
			}
		}

		void addParameters(FEMaterial* pm)
		{
			pm->UpdateData(false);
			for (int i = 0; i < pm->Parameters(); ++i)
			{
				Param& p = pm->GetParam(i);
				if (p.IsVisible())
				{
					if (p.IsEditable() && (p.IsPersistent() || (m_pm == nullptr)))
						addChild(pm, i, -1, 0);
					else if (p.IsPersistent() == false)
					{
						const FEMultiMaterial* mmat = dynamic_cast<const FEMultiMaterial*>(pm->GetParentMaterial());
						if (mmat)
						{
							addChild(pm, i, -1, 0);
						}
					}
				}
			}
		}

		void addChildren(FEMaterial* pm)
		{
			addParameters(pm);

			if (dynamic_cast<FETransverselyIsotropic*>(pm))
			{
				FETransverselyIsotropic* tiso = dynamic_cast<FETransverselyIsotropic*>(pm);
				addFiberParameters(tiso->GetFiberMaterial());
			}
			else if (pm->HasMaterialAxes())
			{
				addParameters(pm->m_axes);
			}

			for (int i = 0; i < pm->Properties(); ++i)
			{
				FEMaterialProperty& p = pm->GetProperty(i);
				int nc = p.Size();
				for (int j = 0; j < nc; ++j) addChild(pm, -1, i, j);

				if ((p.maxSize() == FEMaterialProperty::NO_FIXED_SIZE) && ((p.GetFlags() & FEMaterialProperty::NON_EXTENDABLE) == 0))
				{
					addChild(pm, -1, i, -1);
				}
			}
		}

		FEModel* GetFEModel();

		QVariant data(int column, int role)
		{
			if (m_paramId >= 0)
			{
				Param& p = m_pm->GetParam(m_paramId);
				if (column == 0)
				{
					QString name(p.GetLongName());
					if (m_flag & Item_Indented)
					{
						name = "  " + name;
					}
					return name;
				}

				if (role == Qt::DisplayRole)
				{
					switch (p.GetParamType())
					{
					case Param_FLOAT:
					{
						QString v = QString::number(p.val<double>());
						const char* szunit = p.GetUnit();
						if (szunit)
						{
							QString unitString = Units::GetUnitString(szunit);
							if (unitString.isEmpty() == false)
								v += QString(" %1").arg(unitString);
						}
						return v;
					}
					break;
					case Param_CHOICE:
					case Param_INT:
					{
						int n = p.val<int>();
						if (p.GetEnumNames())
						{
							return GetFEModel()->GetEnumValue(p.GetEnumNames(), n);
						}
						return n;
					}
					break;
					case Param_VEC3D:
					{
						QString v = Vec3dToString(p.val<vec3d>());
						const char* szunit = p.GetUnit();
						if (szunit)
						{
							QString unitString = Units::GetUnitString(szunit);
							if (unitString.isEmpty() == false)
								v += QString(" %1").arg(unitString);
						}
						return v;
					}
					break;
					case Param_BOOL:
					{
						bool b = p.val<bool>();
						return (b ? "Yes" : "No");
					}
					break;
					case Param_VEC2I: return Vec2iToString(p.val<vec2i>()); break;
					case Param_MAT3D:
					{
						QString v = Mat3dToString(p.val<mat3d>());
						const char* szunit = p.GetUnit();
						if (szunit)
						{
							QString unitString = Units::GetUnitString(szunit);
							if (unitString.isEmpty() == false)
								v += QString(" %1").arg(unitString);
						}
						return v;
					}
					break;
					case Param_MATH:
					{
						string s = p.GetMathString();
						QString v = QString::fromStdString(s);
						const char* szunit = p.GetUnit();
						if (szunit)
						{
							QString unitString = Units::GetUnitString(szunit);
							if (unitString.isEmpty() == false)
								v += QString(" %1").arg(unitString);
						}
						return v;
					}
					break;
					case Param_STRING:
					{
						string s = p.GetStringValue();
						QString v = QString::fromStdString(s);
						const char* szunit = p.GetUnit();
						if (szunit)
						{
							QString unitString = Units::GetUnitString(szunit);
							if (unitString.isEmpty() == false)
								v += QString(" %1").arg(unitString);
						}
						return v;
					}
					break;
					default:
						assert(false);
						return "in progress";
					}
				}
				else
				{
					switch (p.GetParamType())
					{
					case Param_FLOAT: return p.val<double>(); break;
					case Param_INT: 
					case Param_CHOICE:
						return p.val<int>(); break;
					case Param_VEC3D: return Vec3dToString(p.val<vec3d>()); break;
					case Param_BOOL: return (p.val<bool>() ? 1 : 0); break;
					case Param_VEC2I:return Vec2iToString(p.val<vec2i>()); break;
					case Param_MAT3D: return Mat3dToString(p.val<mat3d>()); break;
					case Param_MATH: return QString::fromStdString(p.GetMathString()); break;
					case Param_STRING: return QString::fromStdString(p.GetStringValue()); break;
					default:
						assert(false);
						return "in progress";
					}
				}
			}
			else if (m_propId >= 0)
			{
				FEMaterialProperty& p = m_pm->GetProperty(m_propId);
				if (column == 0)
				{
					QString s = QString::fromStdString(p.GetName());
					if (p.maxSize() != 1)
					{
						if (m_matIndex >= 0)
						{
							s += QString(" - %1").arg(m_matIndex + 1);
							FEMaterial* pm = p.GetMaterial(m_matIndex);
							if (pm && (pm->GetName().empty() == false))
							{
								QString name = QString::fromStdString(pm->GetName());
								s += QString(" [%1]").arg(name);
							}
						}
						else 
							s = QString("<add %1>").arg(s);
					}
					return s;
				}
				else
				{
					FEMaterial* pm = (m_matIndex >= 0 ? p.GetMaterial(m_matIndex) : nullptr);
					if (pm == nullptr) return QString("(select)");
					else   if (dynamic_cast<FEReactionSpecies*>(pm))
					{
						FEModel* fem = GetFEModel();
						FEReactionSpecies* prm = dynamic_cast<FEReactionSpecies*>(pm);

						int ntype = prm->GetSpeciesType();
						int index = prm->GetIndex();
						const char* sz = nullptr;
						if (ntype == FEReactionSpecies::SOLUTE_SPECIES)
						{
							sz = fem->GetVariableName("$(Solutes)", index);
						}
						else if (ntype == FEReactionSpecies::SBM_SPECIES)
						{
							sz = fem->GetVariableName("$(SBMs)", index);
						}
						else
						{
							assert(false);
						}

						return (sz ? sz : "(invalid species)");
					}
					else return pm->TypeStr();
				}
			}
			else return "No data";
		}

		bool setData(int column, const QVariant& value)
		{
			if (column != 1) return false;

			if (m_paramId >= 0)
			{
				Param& p = m_pm->GetParam(m_paramId);
				switch (p.GetParamType())
				{
				case Param_FLOAT: 
				{
					double f = value.toDouble();
					p.SetFloatValue(f);
				}
				break;
				case Param_CHOICE:
				case Param_INT: 
				{
					int n = value.toInt();
					p.SetIntValue(n);
				}
				break;
				case Param_VEC3D: p.SetVec3dValue(StringToVec3d(value.toString())); break;
				case Param_VEC2I: p.SetVec2iValue(StringToVec2i(value.toString())); break;
				case Param_MAT3D: p.SetMat3dValue(StringToMat3d(value.toString())); break;
				case Param_BOOL:
				{
					int n = value.toInt();
					p.SetBoolValue(n != 0);
				}
				break;
				case Param_MATH:
				{
					string s = value.toString().toStdString();
					p.SetMathString(s);
				}
				break;
				case Param_STRING:
				{
					string s = value.toString().toStdString();
					p.SetStringValue(s);
				}
				break;
				default:
					assert(false);
				}

				return m_pm->UpdateData(true);
			}
			else if (isProperty())
			{
				int matId = value.toInt();

				FEMaterialProperty& matProp = m_pm->GetProperty(m_propId);

				if (matId == -2)
				{
					FEMaterial* pm = matProp.GetMaterial(m_matIndex);
					if (pm) matProp.RemoveMaterial(pm);
				}
				else
				{
					// check if this is a different type
					if (m_matIndex >= 0)
					{
						FEMaterial* oldMat = m_pm->GetProperty(m_propId).GetMaterial();

						if (dynamic_cast<FEReactionSpecies*>(oldMat))
						{
							FEReactionSpecies* rs = dynamic_cast<FEReactionSpecies*>(oldMat);
							rs->SetIndex(matId);
							return true;
						}

						if (oldMat && (oldMat->Type() == matId))
						{
							// the type has not changed, so don't replace the material
							return false;
						}
					}

					FEMaterialFactory& MF = *FEMaterialFactory::GetInstance();
					FEMaterial* pmat = MF.Create(value.toInt());
					if (pmat)
					{
						if (m_matIndex >= 0)
							m_pm->GetProperty(m_propId).SetMaterial(pmat, m_matIndex);
						else
						{
							m_pm->GetProperty(m_propId).AddMaterial(pmat);
						}
					}
				}
				return true;
			}
			return false;
		}

		Item* parent() { return m_parent; }

		Item* child(int n) { return ((n >= 0) && (n < children()) ? m_children[n] : nullptr); }

		int row() const { return m_nrow; }

		int children() const { return (int)m_children.size(); }
	};

public:
	explicit CMaterialPropsModel(QObject* parent = nullptr) : QAbstractItemModel(parent)
	{
		m_root = nullptr;
		m_valid = true;
	}

	~CMaterialPropsModel() { delete m_root; }

	void SetMaterial(GMaterial* mat)
	{
		m_valid = true;
		beginResetModel();
		delete m_root;
		m_root = nullptr;
		m_mat = mat;
		if (mat)
		{
			m_root = new Item();
			m_root->m_model = this;
			m_root->addChildren(m_mat->GetMaterialProperties());
		}
		endResetModel();
	}

	bool isProperty(const QModelIndex& index)
	{
		if (index.isValid() == false) return false;
		Item* item = static_cast<Item*>(index.internalPointer());
		return item->isProperty();
	}

	QVariant data(const QModelIndex& index, int role) const override
	{
		if (!index.isValid()) return QVariant();

		if (role == Qt::SizeHintRole)
		{
			QFont font("Times", 12);
			QFontInfo fi(font);
			return QSize(10, 3*fi.pixelSize()/2);
		}

		Item* item = static_cast<Item*>(index.internalPointer());

/*		if (role == Qt::BackgroundRole)
		{
			// This color has to match the color in CMaterialPropsView::drawBranches
			if (item->isProperty()) return QColor(Qt::darkGray);
		}
*/
		if ((role == Qt::FontRole) && (item->m_flag & Item::Item_Bold))
		{
			QFont font;
			font.setBold(true);
			return font;
		}

		if ((role != Qt::DisplayRole)&& (role != Qt::EditRole)) return QVariant();

		return item->data(index.column(), role);
	}

	Qt::ItemFlags flags(const QModelIndex& index) const override
	{
		if (!index.isValid()) return Qt::ItemIsEnabled;
		if (index.column() == 1)
		{
			Item* item = static_cast<Item*>(index.internalPointer());
			if (item->isParameter())
				return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;

			if (item->isProperty())
				return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
		}
		return QAbstractItemModel::flags(index);
	}

	bool setData(const QModelIndex& index, const QVariant& value, int role) override
	{
		if (index.isValid() && (role == Qt::EditRole))
		{
			Item* item = static_cast<Item*>(index.internalPointer());
			if (item->setData(index.column(), value))
			{
				m_valid = false;
			}
			emit dataChanged(index, index);
			return true;
		}
		return false;
	}

	QVariant headerData(int section, Qt::Orientation orientation, int role) const override
	{
		if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
			return (section == 0? "Property" : "Value");
		return QAbstractItemModel::headerData(section, orientation, role);
	}

	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override
	{
		if (!hasIndex(row, column, parent)) return QModelIndex();
		Item* parentItem = nullptr;
		if (!parent.isValid()) parentItem = m_root;
		else parentItem = static_cast<Item*>(parent.internalPointer());
		Item* childItem = parentItem->child(row);
		if (childItem) return createIndex(row, column, childItem);
		return QModelIndex();
	}

	QModelIndex parent(const QModelIndex &index) const override
	{
		if (!index.isValid()) return QModelIndex();
		Item* item = static_cast<Item*>(index.internalPointer());
		Item* parent = item->parent();
		if (parent == m_root) return QModelIndex();
		return createIndex(parent->row(), 0, parent);
	}

	int rowCount(const QModelIndex &parent = QModelIndex()) const override
	{
		if (parent.column() > 0) return 0;
		Item* parentItem = nullptr;
		if (!parent.isValid()) parentItem = m_root;
		else parentItem = static_cast<Item*>(parent.internalPointer());

		return (parentItem ? parentItem->children() : 0);
	}

	int columnCount(const QModelIndex &parent = QModelIndex()) const override
	{
		return 2;
	}

	bool IsValid() const { return m_valid; }

	void ResetModel()
	{
		SetMaterial(m_mat);
	}

private:
	GMaterial*		m_mat;
	Item*			m_root;
	bool		m_valid;
};

FEModel* CMaterialPropsModel::Item::GetFEModel()
{
	return m_model->m_mat->GetModel();
}

//=================================================================================================
CMaterialPropsDelegate::CMaterialPropsDelegate(QObject* parent) : QStyledItemDelegate(parent) {}

// in MaterialEditor.cpp
void FillComboBox(QComboBox* pc, int nclass, int module, bool btoplevelonly);

QWidget* CMaterialPropsDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	if (index.isValid())
	{
		CMaterialPropsModel::Item* item = static_cast<CMaterialPropsModel::Item*>(index.internalPointer());

		if (item->isParameter())
		{
			Param* p = item->parameter();

			// check for variable parameters first
			if (p->IsVariable())
			{
				CEditVariableParam* pw = new CEditVariableParam(parent);
				pw->setParam(p);
				return pw;
			}

			if (p->GetParamType() == Param_FLOAT)
			{
				QLineEdit* pw = new QLineEdit(parent);
				pw->setValidator(new QDoubleValidator);
				return pw;
			}
			if (p->GetParamType() == Param_VEC2I)
			{
				QLineEdit* pw = new QLineEdit(parent);
				return pw;
			}
			if (p->GetParamType() == Param_VEC3D)
			{
				QLineEdit* pw = new QLineEdit(parent);
				return pw;
			}
			if ((p->GetParamType() == Param_INT) || (p->GetParamType() == Param_CHOICE))
			{
				if (p->GetEnumNames())
				{
					QComboBox* box = new QComboBox(parent);
					QStringList enumValues = GetEnumValues(item->GetFEModel(), p->GetEnumNames());
					box->addItems(enumValues);
					QObject::connect(box, SIGNAL(currentIndexChanged(int)), this, SLOT(OnEditorSignal()));
					return box;
				}
			}
			if (p->GetParamType() == Param_BOOL)
			{ 
				QComboBox* pw = new QComboBox(parent);
				pw->addItems(QStringList() << "No" << "Yes");
				QObject::connect(pw, SIGNAL(currentIndexChanged(int)), this, SLOT(OnEditorSignal()));
				return pw;
			}
		}
		else if (item->isProperty())
		{
			FEMaterial* pm = item->m_pm;
			FEMaterialProperty& matProp = pm->GetProperty(item->m_propId);
			FEMaterial* pmat = pm->GetProperty(item->m_propId).GetMaterial(item->m_matIndex);

			QComboBox* pc = new QComboBox(parent);

			if (dynamic_cast<FEReactionSpecies*>(pmat))
			{
				FEReactionSpecies* rs = dynamic_cast<FEReactionSpecies*>(pmat);
				FEModel& fem = *item->GetFEModel();
				int ntype = rs->GetSpeciesType();
				int index = rs->GetIndex();
				char buf[1024] = { 0 };
				if (ntype == FEReactionSpecies::SBM_SPECIES)
				{
					fem.GetVariableNames("$(SBMs)", buf);
				}
				else if (ntype == FEReactionSpecies::SOLUTE_SPECIES)
				{
					fem.GetVariableNames("$(Solutes)", buf);
				}
				else
				{
					assert(false);
				}

				int n = 0;
				char* sz = buf;
				while (*sz)
				{
					pc->addItem(sz, n++);
					sz += strlen(sz) + 1;
				}
				pc->setCurrentIndex(index);
			}
			else
			{
				FillComboBox(pc, matProp.GetClassID(), 0xFFFF, false);

				pc->insertSeparator(pc->count());
				pc->addItem("(remove)", -2);

				if (pmat)
				{
					int index = pmat->Type();
					for (int i = 0; i < pc->count(); ++i)
					{
						int matid = pc->itemData(i, Qt::UserRole).toInt();
						if (matid == index)
						{
							pc->setCurrentIndex(i);
							break;
						}
					}
				}
				else pc->setCurrentIndex(-1);
			}

			QObject::connect(pc, SIGNAL(currentIndexChanged(int)), this, SLOT(OnEditorSignal()));

			return pc;
		}
	}
	return QStyledItemDelegate::createEditor(parent, option, index);
}

void CMaterialPropsDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
	if (!index.isValid()) return;
	if (dynamic_cast<QComboBox*>(editor))
	{
		QComboBox* pw = dynamic_cast<QComboBox*>(editor);
		CMaterialPropsModel::Item* item = static_cast<CMaterialPropsModel::Item*>(index.internalPointer());
		if (item->isParameter())
		{
			// We only want to do this for enum parameters
			Param* p = item->parameter();
			if (p && (p->GetEnumNames()))
			{
				QVariant v = item->data(1, Qt::EditRole);
				pw->setCurrentIndex(v.toInt());
			}
		}
	}
	else QStyledItemDelegate::setEditorData(editor, index);
}

void CMaterialPropsDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	if (!index.isValid()) return;
	if (dynamic_cast<QComboBox*>(editor))
	{
		QComboBox* pw = dynamic_cast<QComboBox*>(editor);
		CMaterialPropsModel::Item* item = static_cast<CMaterialPropsModel::Item*>(index.internalPointer());
		if (item->isParameter())
		{
			Param* p = item->parameter();
			if (p && (p->GetEnumNames() || (p->GetParamType() == Param_BOOL)))
			{
				model->setData(index, pw->currentIndex());
				return;
			}
		}
		else if (item->isProperty())
		{
			int matId = pw->currentData(Qt::UserRole).toInt();
			model->setData(index, matId);
			return;
		}
	}
	QStyledItemDelegate::setModelData(editor, model, index);
}

void CMaterialPropsDelegate::OnEditorSignal()
{
	QWidget* sender = dynamic_cast<QWidget*>(QObject::sender());
	emit commitData(sender);
}

//=================================================================================================
CMaterialPropsView::CMaterialPropsView(QWidget* parent) : QTreeView(parent)
{
	setSelectionBehavior(QAbstractItemView::SelectRows);
	setSelectionMode(QAbstractItemView::SingleSelection);
	setUniformRowHeights(true);
	setEditTriggers(QAbstractItemView::AllEditTriggers);

	setItemDelegate(new CMaterialPropsDelegate);
	m_model = new CMaterialPropsModel;
	setModel(m_model);

	QObject::connect(m_model, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&, const QVector<int>&)), this, SLOT(onModelDataChanged()));
}

void CMaterialPropsView::SetMaterial(GMaterial* mat)
{
	m_model->SetMaterial(mat);
	expandAll();
	setColumnWidth(0, width() / 2);
}

void CMaterialPropsView::drawBranches(QPainter* painter, const QRect& rect, const QModelIndex& index) const
{
/*	if (index.isValid())
	{
		if (m_model->isProperty(index))
		{
			painter->fillRect(rect, Qt::darkGray);
		}
	}
*/	QTreeView::drawBranches(painter, rect, index);
}

void CMaterialPropsView::onModelDataChanged()
{
	if (m_model->IsValid() == false)
	{
		m_model->ResetModel();
		expandAll();
	}
}
