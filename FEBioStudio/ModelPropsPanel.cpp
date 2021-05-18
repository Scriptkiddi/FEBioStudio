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
#include "ModelPropsPanel.h"
#include "PropertyListView.h"
#include "PropertyListForm.h"
#include "ToolBox.h"
#include "SelectionBox.h"
#include <QStackedWidget>
#include <QLabel>
#include <QLineEdit>
#include <QBoxLayout>
#include <QMessageBox>
#include <QFormLayout>
#include <QTabWidget>
#include "ModelDocument.h"
#include "MainWindow.h"
#include "ObjectProps.h"
#include <GeomLib/GPrimitive.h>
#include <FEMLib/FEInitialCondition.h>
#include <FEMLib/FESurfaceLoad.h>
#include <FEMLib/FEBodyLoad.h>
#include <FEMLib/FEMultiMaterial.h>
#include <FEMLib/FEModelConstraint.h>
#include <QGridLayout>
#include <QComboBox>
#include <QCheckBox>
#include "CColorButton.h"
#include "MeshInfoPanel.h"
#include <GLWLib/convert.h>
#include <MeshTools/GGroup.h>
#include <CUILib/ImageViewer.h>
#include <CUILib/HistogramViewer.h>
#include <PostLib/ImageModel.h>
#include <PostGL/GLPlot.h>
#include <MeshTools/GModel.h>
#include "Commands.h"
#include "MaterialPropsView.h"

//=============================================================================
CObjectPropsPanel::CObjectPropsPanel(QWidget* parent) : QWidget(parent)
{
	QGridLayout* l = new QGridLayout;

	l->addWidget(new QLabel("Name:"), 0, 0, Qt::AlignRight);
	l->addWidget(m_name = new QLineEdit, 0, 1);
	m_name->setObjectName("name");

	l->addWidget(m_col = new CColorButton, 0, 2);
	m_col->setObjectName("col");

	l->addWidget(new QLabel("Type:"), 1, 0, Qt::AlignRight);
	l->addWidget(m_type = new QLabel, 1, 1);

	l->addWidget(m_statusLabel = new QLabel("Active:"), 2, 0, Qt::AlignRight);
	l->addWidget(m_status = new QCheckBox, 2, 1);
	m_status->setObjectName("status");

	setLayout(l);

	QMetaObject::connectSlotsByName(this);
}

void CObjectPropsPanel::setName(const QString& name)
{
	m_name->setText(name);
}

void CObjectPropsPanel::setType(const QString& name)
{
	m_type->setText(name);
}

void CObjectPropsPanel::setColor(const QColor& col)
{
	m_col->setColor(col);
}

void CObjectPropsPanel::showColor(bool b)
{
	m_col->setVisible(b);
}

void CObjectPropsPanel::showStatus(bool b)
{
	m_status->setVisible(b);
	m_statusLabel->setVisible(b);
}

void CObjectPropsPanel::setNameReadOnly(bool b)
{
	m_name->setReadOnly(b);
}

void CObjectPropsPanel::setStatus(bool b)
{
	m_status->setChecked(b);
}

void CObjectPropsPanel::on_name_textEdited(const QString& t)
{
	emit nameChanged(t);
}

void CObjectPropsPanel::on_col_colorChanged(QColor c)
{
	emit colorChanged(c);
}

void CObjectPropsPanel::on_status_clicked(bool b)
{
	emit statusChanged(b);
}

//=============================================================================
CBCObjectPropsPanel::CBCObjectPropsPanel(QWidget* parent) : QWidget(parent)
{
	QGridLayout* l = new QGridLayout;

	l->addWidget(new QLabel("Name:"), 0, 0);
	l->addWidget(m_name = new QLineEdit, 0, 1);
	m_name->setObjectName("name");

	l->addWidget(new QLabel("Type:"), 1, 0);
	l->addWidget(m_type = new QLabel, 1, 1);

	l->addWidget(new QLabel("Step:"), 2, 0);
	l->addWidget(m_list = new QComboBox, 2, 1);
	m_list->setObjectName("list");

	l->addWidget(new QLabel("Active:"), 3, 0, Qt::AlignRight);
	l->addWidget(m_state = new QCheckBox, 3, 1);
	m_state->setObjectName("state");

	setLayout(l);

	QMetaObject::connectSlotsByName(this);
}

void CBCObjectPropsPanel::setStepValues(const vector<pair<QString, int> >& l)
{
	m_list->clear();
	for (size_t i=0; i<l.size(); ++i)
	{
		const pair<QString,int>& item = l[i];
		m_list->addItem(item.first, item.second);
	}
}

void CBCObjectPropsPanel::setStepID(int n)
{
	int nitem = m_list->findData(n);
	m_list->setCurrentIndex(nitem);
}

int CBCObjectPropsPanel::currentStepID()
{
	return m_list->currentData().toInt();
}

void CBCObjectPropsPanel::setName(const QString& name)
{
	m_name->setText(name);
}

void CBCObjectPropsPanel::setType(const QString& name)
{
	m_type->setText(name);
}

void CBCObjectPropsPanel::on_name_textEdited(const QString& t)
{
	emit nameChanged(t);
}

void CBCObjectPropsPanel::on_list_currentIndexChanged(int n)
{
	emit stepChanged(n);
}

void CBCObjectPropsPanel::showActiveState(bool b)
{
	m_state->setVisible(b);
}

void CBCObjectPropsPanel::setActiveState(bool b)
{
	m_state->setChecked(b);
}

void CBCObjectPropsPanel::on_state_toggled(bool b)
{
	emit stateChanged(b);
}

//=============================================================================
CGItemPropsPanel::CGItemPropsPanel(QWidget* parent) : QWidget(parent)
{
	QGridLayout* l = new QGridLayout;

	l->addWidget(new QLabel("Name:"), 0, 0, Qt::AlignRight);
	l->addWidget(m_name = new QLineEdit, 0, 1);
	m_name->setObjectName("name");

	l->addWidget(new QLabel("Type:"), 1, 0, Qt::AlignRight);
	l->addWidget(m_type = new QLabel, 1, 1);

	l->addWidget(new QLabel("ID:"), 2, 0, Qt::AlignRight);
	l->addWidget(m_id = new QLabel, 2, 1);

	setLayout(l);

	QMetaObject::connectSlotsByName(this);
}

void CGItemPropsPanel::setName(const QString& name)
{
	m_name->setText(name);
}

void CGItemPropsPanel::setType(const QString& name)
{
	m_type->setText(name);
}

void CGItemPropsPanel::setID(int nid)
{
	m_id->setText(QString::number(nid));
}

void CGItemPropsPanel::on_name_textEdited(const QString& t)
{
	emit nameChanged(t);
}

//=============================================================================
class Ui::CModelPropsPanel
{
	enum {
		OBJECT_PANEL,
		BCOBJECT_PANEL,
		GITEM_PANEL,
		MESHINFO_PANEL,
		PARTINFO_PANEL,
		PROPS_PANEL,
		SELECTION1_PANEL,
		SELECTION2_PANEL,
		IMAGE_PANEL
	};

public:
	QStackedWidget*	stack;
	QStackedWidget*	propStack;
	::CSelectionBox* sel1;
	::CSelectionBox* sel2;
	::CPropertyListView* props;
	::CPropertyListForm* form;
	CMaterialPropsView*	mat;

	CToolBox* tool;
	CObjectPropsPanel*	obj;
	CBCObjectPropsPanel*	bcobj;
	CGItemPropsPanel*		gitem;
	CMeshInfoPanel*	mesh;
	CPartInfoPanel* part;
	QTabWidget* imageTab;

	CImageViewer*		imageView;
	CHistogramViewer*	histoView;

	bool		m_showImageTools;

public:
	void setupUi(QWidget* parent)
	{
		m_showImageTools = false;

		props = new ::CPropertyListView; props->setObjectName("props");
		form  = new ::CPropertyListForm; form->setObjectName("form");
		mat   = new CMaterialPropsView; mat->setObjectName("mat");

		obj = new CObjectPropsPanel;
		obj->setObjectName("object");

		bcobj = new CBCObjectPropsPanel;
		bcobj->setObjectName("bcobject");

		gitem = new CGItemPropsPanel;
		gitem->setObjectName("gitem");

		propStack = new QStackedWidget;
		propStack->addWidget(props);
		propStack->addWidget(form);
		propStack->addWidget(mat);

		sel1 = new ::CSelectionBox;
		sel1->setObjectName("select1");

		sel2 = new ::CSelectionBox;
		sel2->setObjectName("select2");

		mesh = new CMeshInfoPanel;
		part = new CPartInfoPanel;

		imageView = new CImageViewer;
		histoView = new CHistogramViewer;
		imageTab = new QTabWidget;
		imageTab->addTab(imageView, "Image Viewer");
		imageTab->addTab(histoView, "Histogram");

		// compose toolbox
		tool = new CToolBox;
		tool->addTool("Object", obj);
		tool->addTool("Object", bcobj);
		tool->addTool("Object", gitem);
		tool->addTool("Mesh Info", mesh);
		tool->addTool("Mesh Info", part);
		tool->addTool("Properties", propStack);
		tool->addTool("Selection", sel1);
		tool->addTool("Selection", sel2);
		tool->addTool("3D Image", imageTab);

		// hide all panels initially
//		tool->getToolItem(OBJECT_PANEL)->setVisible(false);
		tool->getToolItem(BCOBJECT_PANEL)->setVisible(false);
		tool->getToolItem(MESHINFO_PANEL)->setVisible(false);
		tool->getToolItem(PARTINFO_PANEL)->setVisible(false);
//		tool->getToolItem(PROPS_PANEL)->setVisible(false);
		tool->getToolItem(SELECTION1_PANEL)->setVisible(false);
		tool->getToolItem(SELECTION2_PANEL)->setVisible(false);

		stack = new QStackedWidget;
		QLabel* label = new QLabel("");
		label->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
		stack->addWidget(label);
		stack->addWidget(tool);

		QVBoxLayout* mainLayout = new QVBoxLayout;
		mainLayout->setContentsMargins(0,0,0,0);
		mainLayout->addWidget(stack);
		parent->setLayout(mainLayout);

		QMetaObject::connectSlotsByName(parent);
	}

	void showObjectInfo(bool b, bool showColor = false, bool editName = true, QColor col = QColor(0,0,0), bool showActive = false, bool isActive = false) 
	{ 
		obj->showColor(showColor);
		obj->showStatus(showActive);
		if (showActive) obj->setStatus(isActive);
		if (showColor) obj->setColor(col);
		tool->getToolItem(OBJECT_PANEL)->setVisible(b); 
		obj->setNameReadOnly(!editName);
	}

	void showBCObjectInfo(bool b, bool showActiveState = false, bool isActive = false)
	{
		tool->getToolItem(BCOBJECT_PANEL)->setVisible(b);
		if (showActiveState)
			bcobj->setActiveState(isActive);
	}

	void showGItemInfo(bool b, const QString& name = "", const QString& type = "", int nid = -1)
	{
		if (b)
		{
			gitem->setName(name);
			gitem->setType(type);
			gitem->setID(nid);
		}
		tool->getToolItem(GITEM_PANEL)->setVisible(b);
	}

	void showPropsPanel(bool b) { tool->getToolItem(PROPS_PANEL)->setVisible(b); }
	void showSelectionPanel1(bool b) { tool->getToolItem(SELECTION1_PANEL)->setVisible(b); }
	void showSelectionPanel2(bool b) { tool->getToolItem(SELECTION2_PANEL)->setVisible(b); }

	void setSelection1Title(const QString& t) { tool->getToolItem(SELECTION1_PANEL)->setTitle(t); }
	void setSelection2Title(const QString& t) { tool->getToolItem(SELECTION2_PANEL)->setTitle(t); }

	void setName(const QString& txt) { obj->setName(txt); }
	void setType(const QString& txt) { obj->setType(txt); }

	void setBCName(const QString& txt) { bcobj->setName(txt); }
	void setBCType(const QString& txt) { bcobj->setType(txt); }

	void setPropertyList(CPropertyList* pl)
	{
		propStack->setCurrentIndex(0);
		props->Update(pl);
		form->setPropertyList(0);
		mat->SetMaterial(nullptr);
	}

	void setPropertyForm(CPropertyList* pl)
	{
		propStack->setCurrentIndex(1);
		props->Update(0);
		form->setPropertyList(pl);
		mat->SetMaterial(nullptr);
	}

	void setMaterialData(GMaterial* pm)
	{
		propStack->setCurrentIndex(2);
		props->Update(0);
		form->setPropertyList(0);
		mat->SetMaterial(pm);
	}

	void showImagePanel(bool b, Post::CImageModel* img = nullptr)
	{
		if (b && (m_showImageTools==false))
		{
			m_showImageTools = true;

			imageView->SetImageModel(img);
			histoView->SetImageModel(img);
		}
		else if ((b == false) && m_showImageTools)
		{
			m_showImageTools = false;

			imageView->SetImageModel(nullptr);
			histoView->SetImageModel(nullptr);
		}
		tool->getToolItem(IMAGE_PANEL)->setVisible(b);
	}

	void showProperties(bool b)
	{
		if (b == false)
		{
			stack->setCurrentIndex(0);
			setPropertyList(0);
		}
		else
		{
			stack->setCurrentIndex(1);
		}
	}

	::CSelectionBox* selectionPanel(int n)
	{
		return (n==0?sel1 : sel2);
	}

	void setStepList(vector<pair<QString, int> >& l)
	{
		bcobj->setStepValues(l);
	}

	void setCurrentStepID(int n)
	{
		bcobj->setStepID(n);
	}

	int current_bcobject_value()
	{
		return bcobj->currentStepID();
	}

	void showMeshInfoPanel(bool b)
	{
		tool->getToolItem(MESHINFO_PANEL)->setVisible(b);
	}

	void showPartInfoPanel(bool b)
	{
		tool->getToolItem(PARTINFO_PANEL)->setVisible(b);
	}

	void setObject(GObject* po)
	{
		mesh->setInfo(po);
	}

	void setPart(GPart* pg)
	{
		part->setInfo(pg);
	}
};

//=============================================================================
CModelPropsPanel::CModelPropsPanel(CMainWindow* wnd, QWidget* parent) : QWidget(parent), m_wnd(wnd), ui(new Ui::CModelPropsPanel)
{
	m_currentObject = 0;
	m_isUpdating = false;
	ui->setupUi(this);
}

void CModelPropsPanel::Update()
{
	CModelDocument* doc = dynamic_cast<CModelDocument*>(m_wnd->GetDocument());
	if (doc == nullptr) return;

	// rebuild the step list
	FEModel* fem = doc->GetFEModel();
	int N = fem->Steps();
	vector<pair<QString,int> > steps(N);
	for (int i=0; i<N; ++i)
	{
		FEStep* step = fem->GetStep(i);
		steps[i].first = QString::fromStdString(step->GetName());
		steps[i].second = step->GetID();
	}

	m_isUpdating = true;
	ui->setStepList(steps);
	m_isUpdating = false;
}

void CModelPropsPanel::Refresh()
{
	if (m_currentObject)
	{
		m_currentObject->UpdateData(false);
		ui->props->Refresh();
	}
}

void CModelPropsPanel::SetObjectProps(FSObject* po, CPropertyList* props, int flags)
{
	if ((po == 0) && (props == 0))
	{
		ui->showProperties(false);
		m_currentObject = 0;
	}
	else
	{
		ui->showProperties(true);
		ui->showImagePanel(false);
		Post::CImageSource* imgSrc = dynamic_cast<Post::CImageSource*>(po);
		if (imgSrc)
		{
			Post::CImageModel* img = imgSrc->GetImageModel();
			if (img)
			{
				ui->showPropsPanel(false);
				ui->showImagePanel(true, img);
				props = nullptr;
			}
		}

		m_currentObject = po;
		SetSelection(0, 0);
		SetSelection(1, 0);

		ui->showBCObjectInfo(false);
		ui->showGItemInfo(false);

		if (dynamic_cast<GObject*>(m_currentObject))
			ui->showMeshInfoPanel(true);
		else
			ui->showMeshInfoPanel(false);

		if (dynamic_cast<GPart*>(m_currentObject))
			ui->showPartInfoPanel(true);
		else
			ui->showPartInfoPanel(false);

		if (dynamic_cast<FEMaterial*>(m_currentObject))
		{
			// don't show the object info pane
			ui->showObjectInfo(false);
		}
		else
		{
			// set the object's name
			if (m_currentObject)
			{
				QString name = QString::fromStdString(m_currentObject->GetName());
				ui->setName(name);

				std::string stype = CGLDocument::GetTypeString(m_currentObject);
				QString type(stype.c_str());
				ui->setType(type);

				bool nameEditable = !(flags & 0x08);

				// show the color if it's a material or an object
				// TODO: maybe encode that in the flag?
				if (dynamic_cast<GObject*>(po))
				{
					GObject* go = dynamic_cast<GObject*>(po);
					ui->showObjectInfo(true, true, nameEditable, toQColor(go->GetColor()));
					ui->showMeshInfoPanel(true);
					ui->setObject(go);
				}
				else if (dynamic_cast<GMaterial*>(po))
				{
					GMaterial* mo = dynamic_cast<GMaterial*>(po);
					ui->showObjectInfo(true, true, nameEditable, toQColor(mo->Diffuse()));
				}
				else if (dynamic_cast<GDiscreteElementSet*>(po))
				{
					GDiscreteElementSet* pd = dynamic_cast<GDiscreteElementSet*>(po);
					ui->showObjectInfo(true, true, nameEditable, toQColor(pd->GetColor()));
				}
				else if (dynamic_cast<FEStepComponent*>(po))
				{
					FEStepComponent* pc = dynamic_cast<FEStepComponent*>(po);

					ui->showObjectInfo(false, false, nameEditable);

					ui->setBCName(name);
					ui->setBCType(type);
					ui->setCurrentStepID(pc->GetStep());
					ui->showBCObjectInfo(true, true, pc->IsActive());
				}
				else if (dynamic_cast<Post::CGLObject*>(po))
				{
					Post::CGLObject* plot = dynamic_cast<Post::CGLObject*>(po);
					ui->showObjectInfo(true, false, nameEditable, QColor(0, 0, 0), true, plot->IsActive());
				}
				else if (dynamic_cast<GItem*>(po))
				{
					GItem* git = dynamic_cast<GItem*>(po);
					QString typeStr("unknown");
					if (dynamic_cast<GPart*>(git)) {
						typeStr = "Part"; ui->setPart(dynamic_cast<GPart*>(git)); }
					if (dynamic_cast<GFace*>(git)) typeStr = "Surface";
					if (dynamic_cast<GEdge*>(git)) typeStr = "Edge";
					if (dynamic_cast<GNode*>(git)) typeStr = "Node";

					ui->showObjectInfo(false);
					ui->showGItemInfo(true, QString::fromStdString(git->GetName()), typeStr, git->GetID());
				}
				else ui->showObjectInfo(true, false, nameEditable);
			}
			else ui->showObjectInfo(false);
		}

		// show the property list
		if (dynamic_cast<GMaterial*>(po))
		{
			GMaterial* mo = dynamic_cast<GMaterial*>(po);
			ui->setMaterialData(mo);
			ui->showPropsPanel(true);
		}
		else if (props)
		{
			if (flags & 1)
				ui->setPropertyForm(props);
			else
				ui->setPropertyList(props);

			ui->showPropsPanel(true);
		}
		else ui->showPropsPanel(false);

		ui->showSelectionPanel1(true); ui->setSelection1Title("Selection");
		ui->showSelectionPanel2(false);
		FEBoundaryCondition* pbc = dynamic_cast<FEBoundaryCondition*>(m_currentObject);
		if (pbc) { SetSelection(0, pbc->GetItemList()); return; }

		FEInitialCondition* pic = dynamic_cast<FEInitialCondition*>(m_currentObject);
		if (pic) { SetSelection(0, pic->GetItemList()); return; }

		FELoad* pbl = dynamic_cast<FELoad*>(m_currentObject);
		if (pbl) { SetSelection(0, pbl->GetItemList()); return; }

		FESoloInterface* solo = dynamic_cast<FESoloInterface*>(m_currentObject);
		if (solo) { SetSelection(0, solo->GetItemList()); return; }

		FESurfaceConstraint* psc = dynamic_cast<FESurfaceConstraint*>(m_currentObject);
		if (psc) { SetSelection(0, psc->GetItemList()); return;	}

		GMaterial* mat = dynamic_cast<GMaterial*>(m_currentObject);
		if (mat) { SetSelection(mat); return;	}

		FEItemListBuilder* pl = dynamic_cast<FEItemListBuilder*>(m_currentObject);
		if (pl) { SetSelection(0, pl); return; }

		FEPairedInterface* pi = dynamic_cast<FEPairedInterface*>(m_currentObject);
		if (pi)
		{
			ui->setSelection1Title("Primary");
			ui->setSelection2Title("Secondary");
			ui->showSelectionPanel2(true);
			SetSelection(0, pi->GetPrimarySurface());
			SetSelection(1, pi->GetSecondarySurface());
			return;
		}

		GDiscreteElementSet* ds = dynamic_cast<GDiscreteElementSet*>(m_currentObject);
		if (ds)
		{
			SetSelection(ds);
			return;
		}

		ui->showSelectionPanel1(false);
		ui->showSelectionPanel2(false);
	}
}

void CModelPropsPanel::SetSelection(int n, FEItemListBuilder* item)
{
	CSelectionBox* sel = ui->selectionPanel(n);

	if (item == 0)
	{
		sel->setName("");
		sel->setType("");
		sel->clearData();
		return;
	}

	// set the name
	QString name = QString::fromStdString(item->GetName());
	sel->showNameType(true);
	sel->setName(name);
	sel->enableAllButtons(true);
	sel->clearData();
	sel->setCollapsed(false);

	// set the type
	QString type("(unknown)");
	switch (item->Type())
	{
	case GO_PART: 
		{	
			sel->setType("Domains");
			GPartList& g = dynamic_cast<GPartList&>(*item);
			vector<GPart*> parts = g.GetPartList();
			FEItemListBuilder::Iterator it = item->begin();
			for (int i=0; i<parts.size(); ++i, ++it)
			{
				GPart* pg = parts[i];
				if (pg) sel->addData(QString::fromStdString(pg->GetName()), pg->GetID());
				else sel->addData(QString("[invalid reference]"), *it, 1);
			}
		}
		break;
	case GO_FACE:
		{	
			sel->setType("Surfaces");
			GFaceList& g = dynamic_cast<GFaceList&>(*item);
			vector<GFace*> surfs = g.GetFaceList();
			FEItemListBuilder::Iterator it = item->begin();
			for (int i=0; i<surfs.size(); ++i, ++it)
			{
				GFace* pg = surfs[i];
				if (pg) sel->addData(QString::fromStdString(pg->GetName()), pg->GetID());
				else sel->addData(QString("[invalid reference]"), *it, 1);
			}
		}
		break;
	case GO_EDGE: 
		{	
			sel->setType("Curves");
			GEdgeList& g = dynamic_cast<GEdgeList&>(*item);
			vector<GEdge*> edges = g.GetEdgeList();
			FEItemListBuilder::Iterator it = item->begin();
			for (int i=0; i<edges.size(); ++i, ++it)
			{
				GEdge* pg = edges[i];
				if (pg) sel->addData(QString::fromStdString(pg->GetName()), pg->GetID());
				else sel->addData(QString("[invalid reference]"), *it, 1);
			}
		}
		break;
	case GO_NODE:
		{	
			sel->setType("Nodes");
			GNodeList& g = dynamic_cast<GNodeList&>(*item);
			vector<GNode*> nodes = g.GetNodeList();
			FEItemListBuilder::Iterator it = item->begin();
			for (int i=0; i<nodes.size(); ++i, ++it)
			{
				GNode* pg = nodes[i];
				if (pg) sel->addData(QString::fromStdString(pg->GetName()), pg->GetID());
				else sel->addData(QString("[invalid reference]"), *it, 1);
			}
		}
		break;
	default:
		switch (item->Type())
		{
		case FE_PART   : type = "Elements"; break;
		case FE_SURFACE: type = "Facets"; break;
		case FE_EDGESET: type = "Edges"; break;
		case FE_NODESET: type = "Nodes"; break;
		default:
			assert(false);
		}

		FEGroup* pg = dynamic_cast<FEGroup*>(item);
		if (pg)
		{
			FEMesh* mesh = pg->GetMesh();
			if (mesh)
			{
				GObject* po = mesh->GetGObject();
				if (po)
				{
					type += QString(" [%1]").arg(QString::fromStdString(po->GetName()));
				}
			}
		}

		sel->setType(type);

		// set the data
		vector<int> items;
		items.insert(items.end(), item->begin(), item->end());

//		sort(items.begin(), items.end());
//		unique(items.begin(), items.end());

		sel->setCollapsed(true);
		for (int i=0; i<(int)items.size();++i) sel->addData(QString::number(items[i]), items[i], 0, false);
	}
}

void CModelPropsPanel::SetSelection(GMaterial* pmat)
{
	// get the document
	CModelDocument* doc = dynamic_cast<CModelDocument*>(m_wnd->GetDocument());
	FEModel& fem = *doc->GetFEModel();
	GModel& mdl = fem.GetModel();

	// clear the name
	::CSelectionBox* sel = ui->selectionPanel(0);
	sel->showNameType(false);
	sel->enableAllButtons(true);
	sel->setCollapsed(false);

	// set the type
	sel->setType("Domains");

	// set the items
	sel->clearData();
	int N = mdl.Parts();
	for (int i = 0; i<mdl.Parts(); ++i)
	{
		GPart* pg = mdl.Part(i);
		GMaterial* pgm = fem.GetMaterialFromID(pg->GetMaterialID());
		if (pgm && (pgm->GetID() == pmat->GetID()))
		{
			int n = pg->GetID();
			sel->addData(QString::fromStdString(pg->GetName()), n);
		}
	}
}

void CModelPropsPanel::SetSelection(GDiscreteElementSet* set)
{
	// clear the name
	::CSelectionBox* sel = ui->selectionPanel(0);
	sel->showNameType(false);
	sel->enableAddButton(false);
	sel->enableRemoveButton(false);
	sel->enableDeleteButton(false);
	sel->setCollapsed(true);

	// set the type
	sel->setType("Discrete Elements");

	// set the items
	sel->clearData();
	int N = set->size();
	for (int i = 0; i<N; ++i)
	{
		GDiscreteElement& de = set->element(i);
		sel->addData(QString::fromStdString(de.GetName()), i);
	}
}


void CModelPropsPanel::on_select1_addButtonClicked() { addSelection(0); }
void CModelPropsPanel::on_select2_addButtonClicked() { addSelection(1); }

void CModelPropsPanel::addSelection(int n)
{
	// get the document
	CModelDocument* pdoc = dynamic_cast<CModelDocument*>(m_wnd->GetDocument());

	// get the current selection
	FESelection* ps = pdoc->GetCurrentSelection();
	if ((ps == 0) || (ps->Size() == 0)) return;

	assert(m_currentObject);
	if (m_currentObject == 0) return;

	FEModelComponent* pmc = dynamic_cast<FEModelComponent*>(m_currentObject);
	if (pmc)
	{
		// don't allow object selections 
		if (dynamic_cast<GObjectSelection*>(ps)) 
		{
			QMessageBox::critical(this, "FEBio Studio", "You cannot apply an object to a boundary condition's selection.");
			return;
		}

		// for body loads, only allow part selections
		if (dynamic_cast<FEBodyLoad*>(pmc) && (dynamic_cast<GPartSelection*>(ps) == 0))
		{
			QMessageBox::critical(this, "FEBio Studio", "You cannot apply this selection to a body load.");
			return;
		}

		// don't allow part selections, except for initial conditions
		//		if (dynamic_cast<GPartSelection*>(ps) && (dynamic_cast<FEInitialCondition*>(m_pbc)==0)) return;

		// only allow surface selections for surface loads
		if (dynamic_cast<FESurfaceLoad*>(pmc) && (dynamic_cast<GFaceSelection*>(ps) == 0) && (dynamic_cast<FEFaceSelection*>(ps) == 0))
		{
			QMessageBox::critical(this, "FEBio Studio", "You cannot apply this selection to a surface load.");
			return;
		}

		FEItemListBuilder* pl = pmc->GetItemList();
		if (pl == 0)
		{
			FEItemListBuilder* item = ps->CreateItemList();
			if (item == nullptr)
			{
				QMessageBox::critical(this, "FEBio Studio", "You cannot assign the current selection.");
				return;
			}
			else
			{
				pdoc->DoCommand(new CCmdSetModelComponentItemList(pmc, item));
				SetSelection(0, pmc->GetItemList());
			}
		}
		else
		{
			// create the item list builder
			FEItemListBuilder* pg = ps->CreateItemList();
			if (pg == nullptr)
			{
				QMessageBox::critical(this, "FEBio Studio", "You cannot assign the current selection.");
				return;
			}

			// merge with the current list
			if (pg->Type() != pl->Type())
			{
				QMessageBox::critical(this, "FEBio Studio", "The selection is not of the correct type.");
			}
			else
			{
				// for groups, make sure that they are on the same mesh
				FEGroup* pg_prv = dynamic_cast<FEGroup*>(pl);
				FEGroup* pg_new = dynamic_cast<FEGroup*>(pg);
				if (pg_prv && pg_new && (pg_prv->GetMesh() != pg_new->GetMesh()))
				{
					QMessageBox::critical(this, "FEBio Studio", "You cannot assign the current selection.\nThe model component was already assigned to a different mesh.");
				}
				else
				{
					list<int> l = pg->CopyItems();
					pdoc->DoCommand(new CCmdAddToItemListBuilder(pl, l));
				}
			}
			SetSelection(0, pmc->GetItemList());
			delete pg;
		}

		emit selectionChanged();

		return;
	}

	FEPairedInterface* pi = dynamic_cast<FEPairedInterface*>(m_currentObject);
	if (pi)
	{
		if ((ps->Type() != SELECT_SURFACES) && (ps->Type() != SELECT_FE_FACES) && (ps->Type() != SELECT_PARTS))
		{
			QMessageBox::critical(this, "FEBio Studio", "The selection cannot be assigned to this interface.");
			return;
		}

		FEItemListBuilder* pg = ps->CreateItemList();

		FEItemListBuilder* pl = (n==0? pi->GetPrimarySurface() : pi->GetSecondarySurface());
		if (pl == 0)
		{
			if (n == 0) pi->SetPrimarySurface(pg);
			else pi->SetSecondarySurface(pg);
			SetSelection(n, pg);
		}
		else
		{
			// merge with the current list
			if (pg->Type() != pl->Type())
			{
				QMessageBox::critical(this, "FEBio Studio", "The selection is not of the correct type.");
			}
			else
			{
				// for groups, make sure that they are on the same mesh
				FEGroup* pg_prv = dynamic_cast<FEGroup*>(pl);
				FEGroup* pg_new = dynamic_cast<FEGroup*>(pg);
				if (pg_prv && pg_new && (pg_prv->GetMesh() != pg_new->GetMesh()))
				{
					QMessageBox::critical(this, "FEBio Studio", "You cannot assign the current selection.\nThe model component was already assigned to a different mesh.");
				}
				else
				{
					list<int> l = pg->CopyItems();
					pdoc->DoCommand(new CCmdAddToItemListBuilder(pl, l));
				}
			}
			SetSelection(n, pl);
			delete pg;
		}

		emit selectionChanged();
		return;
	}

	FESoloInterface* psolo = dynamic_cast<FESoloInterface*>(m_currentObject);
	if (psolo)
	{
		if (dynamic_cast<GObjectSelection*>(ps) ||
			dynamic_cast<GPartSelection*>(ps)) return;

		FEItemListBuilder* pl = psolo->GetItemList();
		if (pl == 0)
		{
			FEItemListBuilder* pg = ps->CreateItemList();
			psolo->SetItemList(pg);
			SetSelection(0, psolo->GetItemList());
		}
		else
		{
			// create the item list builder
			FEItemListBuilder* pg = ps->CreateItemList();

			// merge with the current list
			if (pg->Type() != pl->Type())
			{
				QMessageBox::critical(this, "FEBio Studio", "The selection is not of the correct type.");
			}
			else
			{
				// for groups, make sure that they are on the same mesh
				FEGroup* pg_prv = dynamic_cast<FEGroup*>(pl);
				FEGroup* pg_new = dynamic_cast<FEGroup*>(pg);
				if (pg_prv && pg_new && (pg_prv->GetMesh() != pg_new->GetMesh()))
				{
					QMessageBox::critical(this, "FEBio Studio", "You cannot assign the current selection.\nThe model component was already assigned to a different mesh.");
				}
				else
				{
					list<int> l = pg->CopyItems();
					pdoc->DoCommand(new CCmdAddToItemListBuilder(pl, l));
				}
			}
			SetSelection(0, psolo->GetItemList());
			delete pg;
		}

		emit selectionChanged();
		return;
	}

	GMaterial* pmat = dynamic_cast<GMaterial*>(m_currentObject);
	if (pmat)
	{
		if (dynamic_cast<GObjectSelection*>(ps))
		{
			GObjectSelection* pos = dynamic_cast<GObjectSelection*>(ps);
			int N = pos->Count();
			vector<GObject*> o(N);
			for (int i = 0; i<N; ++i) o[i] = pos->Object(i);
			pdoc->DoCommand(new CCmdAssignObjectListMaterial(o, pmat->GetID()));
		}
		else if (dynamic_cast<GPartSelection*>(ps))
		{
			GPartSelection* pps = dynamic_cast<GPartSelection*>(ps);
			int N = pps->Count();
			vector<int> p(N);
			GPartSelection::Iterator it(pps);
			for (int i = 0; i<N; ++i, ++it) p[i] = it->GetID();
			pdoc->DoCommand(new CCmdAssignPartMaterial(pdoc->GetGModel(), p, pmat->GetID()));
		}
		else
		{
			QMessageBox::critical(this, "FEBio Studio", "You cannot assign a material to this selection.");
		}
		SetSelection(pmat);
		m_wnd->RedrawGL();

		emit selectionChanged();
		return;
	}

	FEItemListBuilder* pl = dynamic_cast<FEItemListBuilder*>(m_currentObject);
	if (pl)
	{
		// create the item list builder
		FEItemListBuilder* pg = ps->CreateItemList();

		// merge with the current list
		if (pg->Type() != pl->Type())
		{
			QMessageBox::critical(this, "FEBio Studio", "The selection is not of the correct type.");
		}
		else
		{
			// for groups, make sure that they are on the same mesh
			FEGroup* pg_prv = dynamic_cast<FEGroup*>(pl);
			FEGroup* pg_new = dynamic_cast<FEGroup*>(pg);
			if (pg_prv && pg_new && (pg_prv->GetMesh() != pg_new->GetMesh()))
			{
				QMessageBox::critical(this, "FEBio Studio", "You cannot assign the current selection.\nThe model component was already assigned to a different mesh.");
			}
			else
			{
				list<int> l = pg->CopyItems();
				pdoc->DoCommand(new CCmdAddToItemListBuilder(pl, l));
			}
		}
		SetSelection(0, pl);

		// don't forget to clean up
		delete pg;

		emit selectionChanged();
		return;
	}
}

void CModelPropsPanel::on_select1_subButtonClicked() { subSelection(0); }
void CModelPropsPanel::on_select2_subButtonClicked() { subSelection(1); }

void CModelPropsPanel::subSelection(int n)
{
	// get the document
	CModelDocument* pdoc = dynamic_cast<CModelDocument*>(m_wnd->GetDocument());

	// get the current selection
	FESelection* ps = pdoc->GetCurrentSelection();
	if ((ps == 0) || (ps->Size() == 0)) return;

	FEBoundaryCondition* pbc = dynamic_cast<FEBoundaryCondition*>(m_currentObject);
	if (pbc)
	{
		// don't allow object selections 
		if (dynamic_cast<GObjectSelection*>(ps)) return;

		// don't allow part selections, except for initial conditions
		if (dynamic_cast<GPartSelection*>(ps) && (dynamic_cast<FEInitialCondition*>(pbc) == 0)) return;

		FEItemListBuilder* pl = pbc->GetItemList();
		if (pl)
		{
			// create the item list builder
			FEItemListBuilder* pg = ps->CreateItemList();

			// subtract from the current list
			if (pg->Type() == pl->Type())
			{
				list<int> l = pg->CopyItems();
				pdoc->DoCommand(new CCmdRemoveFromItemListBuilder(pl, l));
			}

			SetSelection(0, pbc->GetItemList());
			delete pg;
			emit selectionChanged();
		}
		return;
	}

	FESurfaceLoad* psl = dynamic_cast<FESurfaceLoad*>(m_currentObject);
	if (psl)
	{
		FEItemListBuilder* pl = psl->GetItemList();
		if (pl)
		{
			// create the item list builder
			FEItemListBuilder* pg = ps->CreateItemList();

			// subtract from the current list
			if (pg->Type() == pl->Type())
			{
				list<int> l = pg->CopyItems();
				pdoc->DoCommand(new CCmdRemoveFromItemListBuilder(pl, l));
			}

			SetSelection(0, psl->GetItemList());
			delete pg;
			emit selectionChanged();
		}
		return;
	}

	FEPairedInterface* pi = dynamic_cast<FEPairedInterface*>(m_currentObject);
	if (pi)
	{
		if (dynamic_cast<GObjectSelection*>(ps) ||
		dynamic_cast<GPartSelection*>(ps)) return;

		FEItemListBuilder* pl = (n==0? pi->GetPrimarySurface() : pi->GetSecondarySurface());

		if (pl)
		{
			// create the item list builder
			FEItemListBuilder* pg = ps->CreateItemList();

			// subtract from the current list
			if (pg->Type() == pl->Type())
			{
				list<int> l = pg->CopyItems();
				pdoc->DoCommand(new CCmdRemoveFromItemListBuilder(pl, l));
			}

			SetSelection(n, pl);

			delete pg;

			emit selectionChanged();
			return;
		}
	}

	FESoloInterface* psi = dynamic_cast<FESoloInterface*>(m_currentObject);
	if (psi)
	{
		if (dynamic_cast<GObjectSelection*>(ps) ||
			dynamic_cast<GPartSelection*>(ps)) return;

		FEItemListBuilder* pl = psi->GetItemList();
		if (pl)
		{
			// create the item list builder
			FEItemListBuilder* pg = ps->CreateItemList();

			// subtract from the current list
			if (pg->Type() == pl->Type())
			{
				list<int> l = pg->CopyItems();
				pdoc->DoCommand(new CCmdRemoveFromItemListBuilder(pl, l));
			}

			SetSelection(0, psi->GetItemList());

			delete pg;
			emit selectionChanged();
		}
		return;
	}

	GMaterial* pmat = dynamic_cast<GMaterial*>(m_currentObject);
	if (pmat)
	{
		if (dynamic_cast<GObjectSelection*>(ps))
		{
			GObjectSelection* pos = dynamic_cast<GObjectSelection*>(ps);
			int N = pos->Count();
			vector<GObject*> o(N);
			for (int i = 0; i<N; ++i) o[i] = pos->Object(i);
			pdoc->DoCommand(new CCmdAssignObjectListMaterial(o, 0));
		}
		else if (dynamic_cast<GPartSelection*>(ps))
		{
			GPartSelection* pps = dynamic_cast<GPartSelection*>(ps);
			int N = pps->Count();
			vector<int> p(N);
			GPartSelection::Iterator it(pps);
			for (int i = 0; i<N; ++i, ++it) p[i] = it->GetID();
			pdoc->DoCommand(new CCmdAssignPartMaterial(pdoc->GetGModel(), p, 0));
		}
		else
		{
			QMessageBox::critical(this, "FEBio Studio", "You cannot assign a material to this selection.");
		}
		SetSelection(pmat);
		m_wnd->RedrawGL();
		emit selectionChanged();
		return;
	}

	FEItemListBuilder* pl = dynamic_cast<FEItemListBuilder*>(m_currentObject);
	if (pl)
	{
		// create the item list builder
		FEItemListBuilder* pg = ps->CreateItemList();

		if (pg->Type() == pl->Type())
		{
			list<int> l = pg->CopyItems();
			pdoc->DoCommand(new CCmdRemoveFromItemListBuilder(pl, l));
		}
		SetSelection(0, pl);

		delete pg;

		emit selectionChanged();
		return;
	}
}

void CModelPropsPanel::on_select1_delButtonClicked() { delSelection(0); }
void CModelPropsPanel::on_select2_delButtonClicked() { delSelection(1); }

void CModelPropsPanel::delSelection(int n)
{
	CModelDocument* pdoc = dynamic_cast<CModelDocument*>(m_wnd->GetDocument());

	FEItemListBuilder* pl = 0;

	FEModelComponent* pmc = dynamic_cast<FEModelComponent*>(m_currentObject);
	if (pmc)
	{
		pl = pmc->GetItemList();
		if (pl)
		{
			CSelectionBox* sel = ui->selectionPanel(n);
			list<int> items;
			sel->getSelectedItems(items);

			pdoc->DoCommand(new CCmdRemoveFromItemListBuilder(pl, items));

			SetSelection(n, pl);
			emit selectionChanged();
		}
	}
	else
	{
		FESoloInterface* psi = dynamic_cast<FESoloInterface*>(m_currentObject);
		if (psi) pl = psi->GetItemList();

		FEPairedInterface* pi = dynamic_cast<FEPairedInterface*>(m_currentObject);
		if (pi) pl = (n == 0 ? pi->GetPrimarySurface() : pi->GetSecondarySurface());

		CSelectionBox* sel = ui->selectionPanel(n);

		if (pl)
		{
			list<int> items;
			sel->getSelectedItems(items);
			pdoc->DoCommand(new CCmdRemoveFromItemListBuilder(pl, items));
			SetSelection(n, pl);
			emit selectionChanged();
		}
		else if (dynamic_cast<GMaterial*>(m_currentObject))
		{
			vector<int> items;
			sel->getSelectedItems(items);
			pdoc->DoCommand(new CCmdAssignPartMaterial(pdoc->GetGModel(), items, 0));
			SetSelection(dynamic_cast<GMaterial*>(m_currentObject));
			m_wnd->RedrawGL();
			emit selectionChanged();
		}
		else if (dynamic_cast<FEItemListBuilder*>(m_currentObject))
		{
			pl = dynamic_cast<FEItemListBuilder*>(m_currentObject);
			list<int> items;
			sel->getSelectedItems(items);
			pdoc->DoCommand(new CCmdRemoveFromItemListBuilder(pl, items));
			SetSelection(n, pl);
			emit selectionChanged();
		}
	}
}

void CModelPropsPanel::on_select1_selButtonClicked() { selSelection(0); }
void CModelPropsPanel::on_select2_selButtonClicked() { selSelection(1); }

void CModelPropsPanel::on_select1_nameChanged(const QString& t)
{
	FEItemListBuilder* pl = 0;

	FEBoundaryCondition* pbc = dynamic_cast<FEBoundaryCondition*>(m_currentObject);
	if (pbc) pl = pbc->GetItemList();

	FESoloInterface* psi = dynamic_cast<FESoloInterface*>(m_currentObject);
	if (psi) pl = psi->GetItemList();

	FEPairedInterface* pi = dynamic_cast<FEPairedInterface*>(m_currentObject);
	if (pi) pl = pi->GetPrimarySurface();

	if (pl == 0) return;

	string sname = t.toStdString();
	pl->SetName(sname);
}

void CModelPropsPanel::on_select1_clearButtonClicked() { clearSelection(0); }
void CModelPropsPanel::on_select2_clearButtonClicked() { clearSelection(1); }

void CModelPropsPanel::clearSelection(int n)
{
	CModelDocument* pdoc = dynamic_cast<CModelDocument*>(m_wnd->GetDocument());

	FEItemListBuilder* pl = 0;

	if (dynamic_cast<FEModelComponent*>(m_currentObject))
	{
		FEModelComponent* pmc = dynamic_cast<FEModelComponent*>(m_currentObject);
		pl = pmc->GetItemList();
		if (pl)
		{
			pdoc->DoCommand(new CCmdRemoveItemListBuilder(pmc));
			SetSelection(n, nullptr);
			emit selectionChanged();
		}
	}
	else if (dynamic_cast<FESoloInterface*>(m_currentObject))
	{
		FESoloInterface* psi = dynamic_cast<FESoloInterface*>(m_currentObject);
		pl = psi->GetItemList();
		if (pl)
		{
			pdoc->DoCommand(new CCmdRemoveItemListBuilder(psi));
			SetSelection(n, nullptr);
			emit selectionChanged();
		}
	}
	else if (dynamic_cast<FEPairedInterface*>(m_currentObject))
	{
		FEPairedInterface* pi = dynamic_cast<FEPairedInterface*>(m_currentObject);
		pl = (n == 0 ? pi->GetPrimarySurface() : pi->GetSecondarySurface());
		if (pl)
		{
			pdoc->DoCommand(new CCmdRemoveItemListBuilder(pi, n));
			SetSelection(n, nullptr);
			emit selectionChanged();
		}
	}
	else if (dynamic_cast<GMaterial*>(m_currentObject))
	{
		vector<int> items;
		CSelectionBox* sel = ui->selectionPanel(n);
		sel->getAllItems(items);
		pdoc->DoCommand(new CCmdAssignPartMaterial(pdoc->GetGModel(), items, 0));
		SetSelection(dynamic_cast<GMaterial*>(m_currentObject));
		m_wnd->RedrawGL();
		emit selectionChanged();
	}
}

void CModelPropsPanel::on_select2_nameChanged(const QString& t)
{
	FEItemListBuilder* pl = 0;

	FEBoundaryCondition* pbc = dynamic_cast<FEBoundaryCondition*>(m_currentObject);
	if (pbc) pl = pbc->GetItemList();

	FESoloInterface* psi = dynamic_cast<FESoloInterface*>(m_currentObject);
	if (psi) pl = psi->GetItemList();

	FEPairedInterface* pi = dynamic_cast<FEPairedInterface*>(m_currentObject);
	if (pi) pl = pi->GetSecondarySurface();

	if (pl == 0) return;

	string sname = t.toStdString();
	pl->SetName(sname);
}

void CModelPropsPanel::selSelection(int n)
{
	CModelDocument* pdoc = dynamic_cast<CModelDocument*>(m_wnd->GetDocument());
	GModel* mdl = pdoc->GetGModel();
	FEModel* ps = pdoc->GetFEModel();

	assert(m_currentObject);
	if (m_currentObject == 0) return;

	CSelectionBox* sel = ui->selectionPanel(n);

	// get the selection list
	vector<int> l;
	sel->getSelectedItems(l);
	if (l.empty())
	{
		QMessageBox::information(this, "FEBio Studio", "Nothing to select");
		return;
	}

	// create the selection command
	FEItemListBuilder* pl = 0;

	FESoloInterface* psi = dynamic_cast<FESoloInterface*>(m_currentObject);
	if (psi) pl = psi->GetItemList();

	FEModelComponent* pmc = dynamic_cast<FEModelComponent*>(m_currentObject);
	if (pmc) pl = pmc->GetItemList();

	FEPairedInterface* pi = dynamic_cast<FEPairedInterface*>(m_currentObject);
	if (pi) pl = (n==0? pi->GetPrimarySurface() : pi->GetSecondarySurface());

	GGroup* pg = dynamic_cast<GGroup*>(m_currentObject);
	if (pg) pl = pg;

	FEGroup* pf = dynamic_cast<FEGroup*>(m_currentObject);
	if (pf) pl = pf;

	CCommand* pcmd = 0;
	if (pl)
	{
		switch (pl->Type())
		{
		case GO_NODE: pdoc->SetSelectionMode(SELECT_NODE); pcmd = new CCmdSelectNode(mdl, &l[0], (int)l.size(), false); break;
		case GO_EDGE: pdoc->SetSelectionMode(SELECT_EDGE); pcmd = new CCmdSelectEdge(mdl, &l[0], (int)l.size(), false); break;
		case GO_FACE: pdoc->SetSelectionMode(SELECT_FACE); pcmd = new CCmdSelectSurface(mdl, &l[0], (int)l.size(), false); break;
		case GO_PART: pdoc->SetSelectionMode(SELECT_PART); pcmd = new CCmdSelectPart(mdl, &l[0], (int)l.size(), false); break;
		default:
			if (dynamic_cast<FEGroup*>(pl))
			{
				pdoc->SetSelectionMode(SELECT_OBJECT);
				FEGroup* pg = dynamic_cast<FEGroup*>(pl);
				FEMesh* pm = dynamic_cast<FEMesh*>(pg->GetMesh());
				assert(pm);
				switch (pg->Type())
				{
				case FE_NODESET: pdoc->SetItemMode(ITEM_NODE); pcmd = new CCmdSelectFENodes(pm, &l[0], (int)l.size(), false); break;
				case FE_EDGESET: pdoc->SetItemMode(ITEM_EDGE); pcmd = new CCmdSelectFEEdges(pm, &l[0], (int)l.size(), false); break;
				case FE_SURFACE: pdoc->SetItemMode(ITEM_FACE); pcmd = new CCmdSelectFaces(pm, &l[0], (int)l.size(), false); break;
				case FE_PART   : pdoc->SetItemMode(ITEM_ELEM); pcmd = new CCmdSelectElements(pm, &l[0], (int)l.size(), false); break;
				default:
					assert(false);
				}

				// make sure the parent object is selected
				GObject* po = pm->GetGObject();
				assert(po);
				if (po && !po->IsSelected())
				{
					CCmdGroup* pgc = new CCmdGroup("Select");
					pgc->AddCommand(new CCmdSelectObject(mdl, po, false));
					pgc->AddCommand(pcmd);
					pcmd = pgc;
				}
			}
		}
	}
	else if (dynamic_cast<GMaterial*>(m_currentObject))
	{
		pdoc->SetSelectionMode(SELECT_PART);
		pcmd = new CCmdSelectPart(mdl, &l[0], (int)l.size(), false);
	}
	else if (dynamic_cast<GDiscreteElementSet*>(m_currentObject))
	{
		pdoc->SetSelectionMode(SELECT_DISCRETE);
		GDiscreteElementSet* ds = dynamic_cast<GDiscreteElementSet*>(m_currentObject);
		pcmd = new CCmdSelectDiscreteElements(ds, l, false);
	}

	// execute command
	if (pcmd)
	{
		pdoc->DoCommand(pcmd);
		m_wnd->UpdateToolbar();
		m_wnd->Update();
	}
}

void CModelPropsPanel::on_object_nameChanged(const QString& txt)
{
	if (m_currentObject)
	{
		std::string sname = txt.toStdString();
		m_currentObject->SetName(sname.c_str());

		emit nameChanged(txt);
	}
}

void CModelPropsPanel::on_bcobject_nameChanged(const QString& txt)
{
	if (m_currentObject)
	{
		std::string sname = txt.toStdString();
		m_currentObject->SetName(sname.c_str());

		emit nameChanged(txt);
	}
}

void CModelPropsPanel::on_gitem_nameChanged(const QString& txt)
{
	if (m_currentObject)
	{
		std::string sname = txt.toStdString();
		m_currentObject->SetName(sname.c_str());

		emit nameChanged(txt);
	}
}

void CModelPropsPanel::on_object_colorChanged(const QColor& col)
{
	GObject* po = dynamic_cast<GObject*>(m_currentObject);
	if (po)
	{
		po->SetColor(toGLColor(col));
	}

	GMaterial* mo = dynamic_cast<GMaterial*>(m_currentObject);
	if (mo)
	{
		mo->AmbientDiffuse(toGLColor(col));
	}

	GDiscreteObject* pd = dynamic_cast<GDiscreteObject*>(m_currentObject);
	if (pd)
	{
		pd->SetColor(toGLColor(col));
	}

	m_wnd->RedrawGL();
}

void CModelPropsPanel::on_props_dataChanged(int n)
{
	Post::CGLObject* po = dynamic_cast<Post::CGLObject*>(m_currentObject);
	if (po) po->Update();
	m_wnd->RedrawGL();
}

void CModelPropsPanel::on_form_dataChanged(bool itemModified)
{
	m_wnd->RedrawGL();
	emit dataChanged(itemModified);
}

void CModelPropsPanel::on_bcobject_stepChanged(int n)
{
	if (m_isUpdating) return;

	FEStepComponent* pc = dynamic_cast<FEStepComponent*>(m_currentObject);
	if (pc == 0) return;

	int stepId = ui->current_bcobject_value();
	if ((stepId !=-1) && (pc->GetStep() != stepId))
	{
		CModelDocument* doc = dynamic_cast<CModelDocument*>(m_wnd->GetDocument());

		FEModel* fem = doc->GetFEModel();

		fem->AssignComponentToStep(pc, fem->GetStep(n));

		// Changing the step of a BC requires the whole model tree to be rebuild
		emit dataChanged(true);
	}
}

void CModelPropsPanel::on_bcobject_stateChanged(bool isActive)
{
	if (m_isUpdating) return;

	FEStepComponent* pc = dynamic_cast<FEStepComponent*>(m_currentObject);
	if (pc == 0) return;

	pc->Activate(isActive);

	emit dataChanged(false);
}

void CModelPropsPanel::on_object_statusChanged(bool b)
{
	if (m_isUpdating) return;

	Post::CGLObject* po = dynamic_cast<Post::CGLObject*>(m_currentObject);
	if (po == 0) return;

	po->Activate(b);

	emit dataChanged(false);
}
