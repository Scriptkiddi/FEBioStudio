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
#include "ModelViewer.h"
#include "ui_modelviewer.h"
#include "MainWindow.h"
#include "ModelDocument.h"
#include <FEMLib/FEInitialCondition.h>
#include <FEMLib/FEBodyLoad.h>
#include <QMessageBox>
#include <QMenu>
#include <QInputDialog>
#include "DlgEditOutput.h"
#include "MaterialEditor.h"
#include <FEMLib/FEMultiMaterial.h>
#include <FEMLib/FEMKernel.h>
#include <FEMLib/FESurfaceLoad.h>
#include <FEMLib/FEModelConstraint.h>
#include <GeomLib/GObject.h>
#include <GeomLib/MeshLayer.h>
#include <MeshTools/GModel.h>
#include "Commands.h"

CModelViewer::CModelViewer(CMainWindow* wnd, QWidget* parent) : CCommandPanel(wnd, parent), ui(new Ui::CModelViewer)
{
	ui->setupUi(wnd, this);
	m_currentObject = 0;
}

void CModelViewer::blockUpdate(bool block)
{
	ui->m_blockUpdate = block;
}

void CModelViewer::Update(bool breset)
{
	if (ui->m_blockUpdate) return;

	CModelDocument* doc = dynamic_cast<CModelDocument*>(GetDocument());

//	FSObject* po = m_currentObject;

	// rebuild the model tree
	ui->tree->Build(doc);
	if (ui->m_search->isVisible()) ui->m_search->Build(doc);

	// update the props panel
	ui->props->Update();

//	if (po) Select(po);
}

// get the currently selected object
FSObject* CModelViewer::GetCurrentObject()
{
	return m_currentObject;
}

void CModelViewer::UpdateObject(FSObject* po)
{
	ui->tree->UpdateObject(po);

	if (po && (po == m_currentObject))
	{
		QTreeWidgetItem* current = ui->tree->currentItem();
		if (current)
		{
			int n = current->data(0, Qt::UserRole).toInt();
			assert(ui->tree->m_data[n].obj == m_currentObject);
			SetCurrentItem(n);
		}
	}
}

void CModelViewer::Select(FSObject* po)
{
	if (po == nullptr) m_currentObject = nullptr;
	ui->unCheckSearch();
	ui->tree->Select(po);
}

// select a list of objects
void CModelViewer::SelectObjects(const std::vector<FSObject*>& objList)
{
	ui->unCheckSearch();
	ui->tree->Select(objList);
}

void CModelViewer::on_modelTree_currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* prev)
{
	if (current)
	{
		QVariant v = current->data(0, Qt::UserRole);
		SetCurrentItem(v.toInt());
	}
	else 
	{
		m_currentObject = nullptr;
		ui->props->SetObjectProps(0, 0, 0);
	}

	FSObject* po = GetCurrentObject();
	if (current)
	{
		ui->tree->UpdateItem(current);
	}
	emit currentObjectChanged(po);
}

void CModelViewer::on_modelTree_itemDoubleClicked(QTreeWidgetItem* item, int column)
{
	OnOpenJob();
}

void CModelViewer::SetCurrentItem(int item)
{
	if (item >= 0)
	{
		CModelTreeItem& it = ui->tree->m_data[item];
		CPropertyList* props = it.props;
		FSObject* po = it.obj;
		if (it.flag & CModelTree::OBJECT_NOT_EDITABLE)
			ui->props->SetObjectProps(0, 0, 0);
		else
			ui->props->SetObjectProps(po, props, it.flag);
		m_currentObject = po;
	}
	else
	{
		ui->props->SetObjectProps(0, 0, 0);
		m_currentObject = 0;
	}
}

void CModelViewer::on_searchButton_toggled(bool b)
{
	if (b) 
	{
		ui->m_search->Build(GetDocument());
		ui->props->SetObjectProps(0, 0, 0);
	}
	ui->showSearchPanel(b);
}

void CModelViewer::on_syncButton_clicked()
{
	CModelDocument* pdoc = dynamic_cast<CModelDocument*>(GetDocument());
	GModel& mdl = *pdoc->GetGModel();
	FESelection* sel = pdoc->GetCurrentSelection();
	if (sel) 
	{
        int N = sel->Size();
            
        vector<FSObject*> objList;
        GObjectSelection* os = dynamic_cast<GObjectSelection*>(sel);
        if (os)
        {
            for (int i=0; i<N; ++i)
            {
                objList.push_back(os->Object(i));
            }
        }
            
        GPartSelection* gs = dynamic_cast<GPartSelection*>(sel);
        if (gs)
        {
            GPartSelection::Iterator it(gs);
            for (int i=0; i<N; ++i, ++it)
            {
                GPart* pg = it;
                if (pg)
                {
                    objList.push_back(pg);
                }
            }
        }
            
        GFaceSelection* ss = dynamic_cast<GFaceSelection*>(sel);
        if (ss)
        {
            GFaceSelection::Iterator it(ss);
            for (int i=0; i<N; ++i, ++it)
            {
                GFace* pg = it;
                if (pg)
                {
                    objList.push_back(pg);
                }
            }
        }
            
            
        GEdgeSelection* es = dynamic_cast<GEdgeSelection*>(sel);
        if (es)
        {
            GEdgeSelection::Iterator it(es);
            for (int i=0; i<N; ++i, ++it)
            {
                GEdge* pg = it;
                if (pg)
                {
                    objList.push_back(pg);
                }
            }
        }
            
        GNodeSelection* ns = dynamic_cast<GNodeSelection*>(sel);
        if (ns)
        {
            GNodeSelection::Iterator it(ns);
            for (int i=0; i<N; ++i, ++it)
            {
                GNode* pg = it;
                if (pg)
                {
                    objList.push_back(pg);
                }
            }
        }

		GDiscreteSelection* ds = dynamic_cast<GDiscreteSelection*>(sel);
		if (ds)
		{
			int N = mdl.DiscreteObjects();
			for (int i=0; i<N; ++i)
			{
				GDiscreteObject* po = mdl.DiscreteObject(i);
				if (dynamic_cast<GDiscreteElementSet*>(po))
				{
					GDiscreteElementSet* pds = dynamic_cast<GDiscreteElementSet*>(po);
					int NE = pds->size();
					for (int j=0; j<NE; ++j)
					{
						GDiscreteElement& de = pds->element(j);
						if (de.IsSelected())
						{
							objList.push_back(&de);
						}
					}
				}
			} 
		}
            
        if (objList.size() == 1)
        {
            Select(objList[0]);
        }
        else
        {
            SelectObjects(objList);
        }
    }
}

void CModelViewer::on_selectButton_clicked()
{
	// make sure we have an object
	if (m_currentObject == 0) return;
	FSObject* po = m_currentObject;

	CModelDocument* pdoc = dynamic_cast<CModelDocument*>(GetDocument());

	CCommand* pcmd = 0;
	if (dynamic_cast<GObject*>(po))
	{
		GObject* pm = dynamic_cast<GObject*>(po);
		if (pm->IsVisible() && !pm->IsSelected()) pcmd = new CCmdSelectObject(pdoc->GetGModel(), pm, false);
	}
	else if (dynamic_cast<FEModelComponent*>(po))
	{
		FEModelComponent* pbc = dynamic_cast<FEModelComponent*>(po);
		if (dynamic_cast<FEConstBodyForce*>(pbc) == 0)
		{
			FEItemListBuilder* pitem = pbc->GetItemList();
			if (pitem == 0) QMessageBox::critical(this, "FEBio Studio", "Invalid pointer to FEItemListBuilder object in CModelEditor::OnSelectObject");
			else SelectItemList(pitem);
		}
	}
	else if (dynamic_cast<FEItemListBuilder*>(po))
	{
		FEItemListBuilder* pi = dynamic_cast<FEItemListBuilder*>(po);
		SelectItemList(pi);
	}
	else if (dynamic_cast<GPart*>(po))
	{
		OnSelectPart();
	}
	else if (dynamic_cast<GFace*>(po))
	{
		OnSelectSurface();
	}
	else if (dynamic_cast<GEdge*>(po))
	{
		OnSelectCurve();
	}
	else if (dynamic_cast<GNode*>(po))
	{
		OnSelectNode();
	}
	else if (dynamic_cast<GDiscreteElement*>(po))
	{
		GDiscreteElement* ps = dynamic_cast<GDiscreteElement*>(po);
		ps->Select();
	}
	else if (dynamic_cast<GDiscreteObject*>(po))
	{
		GDiscreteObject* ps = dynamic_cast<GDiscreteObject*>(po);
		GModel& fem = pdoc->GetFEModel()->GetModel();
		int n = fem.FindDiscreteObjectIndex(ps);
		pcmd = new CCmdSelectDiscrete(&fem, &n, 1, false);
	}
	else if (dynamic_cast<FESoloInterface*>(po))
	{
		FESoloInterface* pci = dynamic_cast<FESoloInterface*>(po);
		FEItemListBuilder* pl = pci->GetItemList();
		if (pl == 0) QMessageBox::critical(this, "FEBio Studio", "Invalid pointer to FEItemListBuilder object in CModelEditor::OnSelectObject");
		else SelectItemList(pl);
	}
	else if (dynamic_cast<FEPairedInterface*>(po))
	{
		FEPairedInterface* pci = dynamic_cast<FEPairedInterface*>(po);
		FEItemListBuilder* pml = pci->GetSecondarySurface();
		FEItemListBuilder* psl = pci->GetPrimarySurface();

		if (pml == 0) QMessageBox::critical(this, "FEBio Studio", "Invalid pointer to FEItemListBuilder object in CModelEditor::OnSelectObject");
		else SelectItemList(pml);

		if (psl == 0) QMessageBox::critical(this, "FEBio Studio", "Invalid pointer to FEItemListBuilder object in CModelEditor::OnSelectObject");
		else SelectItemList(psl, true);
	}
	else if (dynamic_cast<GMaterial*>(po))
	{
		GMaterial* mat = dynamic_cast<GMaterial*>(po);
		GModel* mdl = pdoc->GetGModel();
		list<GPart*> partList = mdl->FindPartsFromMaterial(mat->GetID());

		vector<int> partIdList;
		for (GPart* pg : partList) partIdList.push_back(pg->GetID());
		pdoc->SetSelectionMode(SELECT_PART);
		pcmd = new CCmdSelectPart(mdl, partIdList, false);
	}

	if (pcmd) pdoc->DoCommand(pcmd);
	GetMainWindow()->UpdateToolbar();
	GetMainWindow()->Update(this);
}

void CModelViewer::SelectItemList(FEItemListBuilder *pitem, bool badd)
{
	CCommand* pcmd = 0;

	int n = pitem->size();
	if (n == 0) return;

	int* pi = new int[n];
	FEItemListBuilder::Iterator it = pitem->begin();
	for (int i = 0; i<n; ++i, ++it) pi[i] = *it;

	CModelDocument* pdoc = dynamic_cast<CModelDocument*>(GetDocument());
	FEModel* ps = pdoc->GetFEModel();
	GModel* mdl = pdoc->GetGModel();

	switch (pitem->Type())
	{
	case GO_PART: pdoc->SetSelectionMode(SELECT_PART); pcmd = new CCmdSelectPart(mdl, pi, n, badd); break;
	case GO_FACE: pdoc->SetSelectionMode(SELECT_FACE); pcmd = new CCmdSelectSurface(mdl, pi, n, badd); break;
	case GO_EDGE: pdoc->SetSelectionMode(SELECT_EDGE); pcmd = new CCmdSelectEdge(mdl, pi, n, badd); break;
	case GO_NODE: pdoc->SetSelectionMode(SELECT_NODE); pcmd = new CCmdSelectNode(mdl, pi, n, badd); break;
	case FE_PART:
		{
			pdoc->SetSelectionMode(SELECT_OBJECT);
			pdoc->SetItemMode(ITEM_ELEM);

			FEGroup* pg = dynamic_cast<FEGroup*>(pitem);
			CCmdGroup* pcg = new CCmdGroup("Select Elements"); pcmd = pcg;
			FEMesh* pm = dynamic_cast<FEMesh*>(pg->GetMesh());
			pcg->AddCommand(new CCmdSelectObject(mdl, pg->GetGObject(), badd));
			pcg->AddCommand(new CCmdSelectElements(pm, pi, n, badd));
		}
		break;
	case FE_SURFACE:
		{
			pdoc->SetSelectionMode(SELECT_OBJECT);
			pdoc->SetItemMode(ITEM_FACE);

			FEGroup* pg = dynamic_cast<FEGroup*>(pitem);
			CCmdGroup* pcg = new CCmdGroup("Select Faces"); pcmd = pcg;
			FEMesh* pm = dynamic_cast<FEMesh*>(pg->GetMesh());
			pcg->AddCommand(new CCmdSelectObject(mdl, pg->GetGObject(), badd));
			pcg->AddCommand(new CCmdSelectFaces(pm, pi, n, badd));
		}
		break;
	case FE_NODESET:
		{
			pdoc->SetSelectionMode(SELECT_OBJECT);
			pdoc->SetItemMode(ITEM_NODE);

			FEGroup* pg = dynamic_cast<FEGroup*>(pitem);
			CCmdGroup* pcg = new CCmdGroup("Select Nodes"); pcmd = pcg;
			FEMesh* pm = dynamic_cast<FEMesh*>(pg->GetMesh());
			pcg->AddCommand(new CCmdSelectObject(mdl, pg->GetGObject(), badd));
			pcg->AddCommand(new CCmdSelectFENodes(pm, pi, n, badd));
		}
		break;
	}

	if (pcmd)
	{
		pdoc->DoCommand(pcmd);
		GetMainWindow()->UpdateToolbar();
		GetMainWindow()->RedrawGL();
	}

	delete[] pi;
}

void CModelViewer::UpdateSelection()
{
	if (ui->m_search->isVisible())
		ui->m_search->GetSelection(m_selection);
	else
		ui->tree->GetSelection(m_selection);
}

void CModelViewer::Show()
{
	parentWidget()->raise();
}

bool CModelViewer::IsFocus()
{
	return ui->tree->hasFocus();
}

bool CModelViewer::OnDeleteEvent()
{
	on_deleteButton_clicked();
	return true;
}

void CModelViewer::RefreshProperties()
{
	ui->props->Refresh();
}

void CModelViewer::on_deleteButton_clicked()
{
	OnDeleteItem();
}

void CModelViewer::on_props_nameChanged(const QString& txt)
{
	QTreeWidgetItem* item = ui->tree->currentItem();
	assert(item);
	if (item) item->setText(0, txt);
}

void CModelViewer::on_props_selectionChanged()
{
	ui->tree->UpdateObject(ui->props->GetCurrentObject());
}

void CModelViewer::on_props_dataChanged(bool b)
{
	if (b)
	{
		Update();
	}
	else
	{
		ui->tree->UpdateObject(ui->props->GetCurrentObject());
		ui->m_search->UpdateObject(ui->props->GetCurrentObject());

		CMainWindow* wnd = GetMainWindow();
		wnd->RedrawGL();
	}
}

void CModelViewer::on_filter_currentIndexChanged(int n)
{
	FSObject* po = GetCurrentObject();
	ui->tree->SetFilter(n);
	Update(true);
	Select(po);
}

void CModelViewer::OnDeleteItem()
{
	UpdateSelection();

	CModelDocument* doc = dynamic_cast<CModelDocument*>(GetDocument());
	for (int i=0; i<(int)m_selection.size(); ++i)
	{
		doc->DeleteObject(m_selection[i]);
	}
	Select(nullptr);
	Update();
	GetMainWindow()->RedrawGL();
}

void CModelViewer::OnAddMaterial()
{
	CMainWindow* wnd = GetMainWindow();
	wnd->on_actionAddMaterial_triggered();
}

void CModelViewer::OnUnhideAllObjects()
{
	CModelDocument* doc = dynamic_cast<CModelDocument*>(GetDocument());
	GModel* m = doc->GetGModel();
	m->ShowAllObjects();
	Update();
	GetMainWindow()->RedrawGL();
}

void CModelViewer::OnCreateNewMeshLayer()
{
	CModelDocument* doc = dynamic_cast<CModelDocument*>(GetDocument());
	GModel* gm = doc->GetGModel();
	int layers = gm->MeshLayers();
	QString s = QString("Layer") + QString::number(layers + 1);
	QString newLayer = QInputDialog::getText(this, "New Layer", "Layer name:", QLineEdit::Normal, s);
	if (newLayer.isEmpty() == false)
	{
		string layerName = newLayer.toStdString();
		int n = gm->FindMeshLayer(layerName);
		if (n >= 0)
		{
			QMessageBox::critical(this, "FEBio Studio", "Failed creating layer. Layer name already taken.");
		}
		else
		{
			CCmdGroup* cmd = new CCmdGroup(string("Add mesh layer: ") + layerName);
			cmd->AddCommand(new CCmdAddMeshLayer(gm, layerName));
			cmd->AddCommand(new CCmdSetActiveMeshLayer(gm, layers));
			doc->DoCommand(cmd);
			Update();
			GetMainWindow()->RedrawGL();
		}
	}
}

void CModelViewer::OnDeleteMeshLayer()
{
	CModelDocument* doc = dynamic_cast<CModelDocument*>(GetDocument());
	GModel* gm = doc->GetGModel();
	int layers = gm->MeshLayers();
	int activeLayer = gm->GetActiveMeshLayer();
	if ((activeLayer == 0) || (layers == 1))
	{
		QMessageBox::warning(this, "FEBio Studio", "You cannot delete the Default mesh layer.");
		return;
	}
	else
	{
		if (QMessageBox::question(this, "FEBio Studio", "Are you sure you want to delete the current mesh layer?"))
		{
			// to delete the active mesh layer, we must first select a different layer as the active layer.
			// We'll choose the default layer
			string s = gm->GetMeshLayerName(activeLayer);
			CCmdGroup* cmd = new CCmdGroup(string("Delete mesh layer: " + s));
			cmd->AddCommand(new CCmdSetActiveMeshLayer(gm, 0));
			cmd->AddCommand(new CCmdDeleteMeshLayer(gm, activeLayer));
			doc->DoCommand(cmd);
		}
	}
}

void CModelViewer::OnUnhideAllParts()
{
	GObject* po = dynamic_cast<GObject*>(m_currentObject);
	if (po)
	{
		CModelDocument* doc = dynamic_cast<CModelDocument*>(GetDocument());
		GModel* m = doc->GetGModel();
		m->ShowAllParts(po);
		Update();
		GetMainWindow()->RedrawGL();
	}
}

void CModelViewer::OnAddBC()
{
	CMainWindow* wnd = GetMainWindow();
	wnd->on_actionAddBC_triggered();
}

void CModelViewer::OnAddSurfaceLoad()
{
	CMainWindow* wnd = GetMainWindow();
	wnd->on_actionAddSurfLoad_triggered();
}

void CModelViewer::OnAddBodyLoad()
{
	CMainWindow* wnd = GetMainWindow();
	wnd->on_actionAddBodyLoad_triggered();
}

void CModelViewer::OnAddInitialCondition()
{
	CMainWindow* wnd = GetMainWindow();
	wnd->on_actionAddIC_triggered();
}

void CModelViewer::OnAddContact()
{
	CMainWindow* wnd = GetMainWindow();
	wnd->on_actionAddContact_triggered();
}

void CModelViewer::OnAddConstraint()
{
	CMainWindow* wnd = GetMainWindow();
	wnd->on_actionAddConstraint_triggered();
}

void CModelViewer::OnAddRigidConstraint()
{
	CMainWindow* wnd = GetMainWindow();
	wnd->on_actionAddRigidConstraint_triggered();
}

void CModelViewer::OnAddRigidConnector()
{
	CMainWindow* wnd = GetMainWindow();
	wnd->on_actionAddRigidConnector_triggered();
}

void CModelViewer::OnAddStep()
{
	CMainWindow* wnd = GetMainWindow();
	wnd->on_actionAddStep_triggered();
}

void CModelViewer::OnHideObject()
{
	CModelDocument* doc = dynamic_cast<CModelDocument*>(GetDocument());
	GModel& m = doc->GetFEModel()->GetModel();

	for (int i=0; i<m_selection.size(); ++i)
	{
		GObject* po = dynamic_cast<GObject*>(m_selection[i]); assert(po);
		if (po) 
		{
			m.ShowObject(po, false);

			QTreeWidgetItem* item = ui->tree->FindItem(po);
			if (item) item->setForeground(0, Qt::gray);
		}
	}

	CMainWindow* wnd = GetMainWindow();
	wnd->RedrawGL();
}

void CModelViewer::OnShowObject()
{
	CModelDocument* doc = dynamic_cast<CModelDocument*>(GetDocument());
	GModel& m = doc->GetFEModel()->GetModel();

	for (int i=0; i<(int)m_selection.size(); ++i)
	{
		GObject* po = dynamic_cast<GObject*>(m_selection[i]); assert(po);
		if (po)
		{
			m.ShowObject(po, true);

			QTreeWidgetItem* item = ui->tree->FindItem(po);
			if (item) item->setForeground(0, Qt::black);
		}
	}
	CMainWindow* wnd = GetMainWindow();
	wnd->RedrawGL();
}

void CModelViewer::OnSelectObject()
{
	CModelDocument* doc = dynamic_cast<CModelDocument*>(GetDocument());
	GModel& m = doc->GetFEModel()->GetModel();

	CMainWindow* wnd = GetMainWindow();
	wnd->SetSelectionMode(SELECT_OBJECT);

	vector<GObject*> sel; 
	for (int i = 0; i<m_selection.size(); ++i)
	{
		GObject* po = dynamic_cast<GObject*>(m_selection[i]); assert(po);
		if (po && po->IsVisible()) sel.push_back(po);
	}

	if (sel.empty() == false)
	{
		doc->DoCommand(new CCmdSelectObject(&m, sel, true));
		wnd->RedrawGL();
	}
}

void CModelViewer::OnDeleteAllDiscete()
{
	QString q("Are you sure you want to delete all discrete objects? This cannot be undone.");
	if (QMessageBox::question(this, "Delete All", q) == QMessageBox::Yes)
	{
		CModelDocument* doc = dynamic_cast<CModelDocument*>(GetDocument()); assert(doc);
		if (doc == nullptr) return;
		GModel& m = doc->GetFEModel()->GetModel();
		m.ClearDiscrete();

		Select(nullptr);
		Update();
		GetMainWindow()->RedrawGL();
	}
}

void CModelViewer::OnShowAllDiscrete()
{
	CModelDocument* doc = dynamic_cast<CModelDocument*>(GetDocument()); assert(doc);
	if (doc == nullptr) return;
	GModel& m = doc->GetFEModel()->GetModel();
	
	for (int i = 0; i < m.DiscreteObjects(); ++i)
	{
		GDiscreteObject* pd = m.DiscreteObject(i);
		if (pd->IsVisible() == false) pd->Show();
	}
	GetMainWindow()->RedrawGL();
}

void CModelViewer::OnHideAllDiscrete()
{
	CModelDocument* doc = dynamic_cast<CModelDocument*>(GetDocument()); assert(doc);
	if (doc == nullptr) return;
	GModel& m = doc->GetFEModel()->GetModel();

	for (int i = 0; i < m.DiscreteObjects(); ++i)
	{
		GDiscreteObject* pd = m.DiscreteObject(i);
		if (pd->IsVisible()) pd->Hide();
	}
	GetMainWindow()->RedrawGL();
}

void CModelViewer::OnSelectDiscreteObject()
{
	CModelDocument* doc = dynamic_cast<CModelDocument*>(GetDocument());
	GModel& m = doc->GetFEModel()->GetModel();

	CMainWindow* wnd = GetMainWindow();
	wnd->SetSelectionMode(SELECT_DISCRETE);

	vector<GDiscreteObject*> sel;
	for (int i=0; i<(int)m_selection.size(); ++i)
	{
		GDiscreteObject* po = dynamic_cast<GDiscreteObject*>(m_selection[i]); assert(po);
		if (po) sel.push_back(po);
	}

	if (sel.empty() == false)
	{
		doc->DoCommand(new CCmdSelectDiscrete(&m, sel, true));
		wnd->RedrawGL();
	}
}

void CModelViewer::OnDetachDiscreteObject()
{
	GDiscreteElementSet* set = dynamic_cast<GDiscreteElementSet*>(m_currentObject); assert(set);
	if (set == 0) return;

	CMainWindow* wnd = GetMainWindow();
	CModelDocument* doc = dynamic_cast<CModelDocument*>(GetDocument());
	GModel& m = doc->GetFEModel()->GetModel();

	GObject* po = m.DetachDiscreteSet(set);
	if (po)
	{
		const std::string& name = "Detached_" + set->GetName();
		po->SetName(name);
		doc->DoCommand(new CCmdAddAndSelectObject(&m, po));
		Update();
		Select(po);
		wnd->RedrawGL();
	}
}

void CModelViewer::OnHideDiscreteObject()
{
	vector<GDiscreteObject*> sel;
	for (int i = 0; i < (int)m_selection.size(); ++i)
	{
		GDiscreteObject* po = dynamic_cast<GDiscreteObject*>(m_selection[i]); assert(po);
		if (po && po->IsVisible()) po->Hide();
	}
	CMainWindow* wnd = GetMainWindow();
	wnd->RedrawGL();
}

void CModelViewer::OnShowDiscreteObject()
{
	vector<GDiscreteObject*> sel;
	for (int i = 0; i < (int)m_selection.size(); ++i)
	{
		GDiscreteObject* po = dynamic_cast<GDiscreteObject*>(m_selection[i]); assert(po);
		if (po && (po->IsVisible() == false)) po->Show();
	}
	CMainWindow* wnd = GetMainWindow();
	wnd->RedrawGL();
}

void CModelViewer::OnChangeDiscreteType()
{
	GDiscreteSpringSet* set = dynamic_cast<GDiscreteSpringSet*>(m_currentObject); assert(set);
	if (set == 0) return;

	QStringList items; items << "Linear" << "Nonlinear" << "Hill";
	QString item = QInputDialog::getItem(this, "Discrete Set Type", "Type:", items, 0, false);
	if (item.isEmpty() == false)
	{
		FEDiscreteMaterial* mat = nullptr;
		if (item == "Linear"   ) mat = new FELinearSpringMaterial();
		if (item == "Nonlinear") mat = new FENonLinearSpringMaterial();
		if (item == "Hill"     ) mat = new FEHillContractileMaterial();
		if (mat)
		{
			delete set->GetMaterial();
			set->SetMaterial(mat);

			Update();
			Select(set);
		}
		else
		{
			QMessageBox::critical(this, "FEBio Studio", "Failed to assign new material.");
		}
	}
}

void CModelViewer::OnHidePart()
{
	CModelDocument* doc = dynamic_cast<CModelDocument*>(GetDocument());
	GModel& m = doc->GetFEModel()->GetModel();

	for (int i=0; i<(int)m_selection.size(); ++i)
	{
		GPart* pg = dynamic_cast<GPart*>(m_selection[i]); assert(pg);
		if (pg) 
		{
			m.ShowPart(pg, false);

			QTreeWidgetItem* item = ui->tree->FindItem(pg);
			if (item) item->setForeground(0, Qt::gray);
		}
	}

	CMainWindow* wnd = GetMainWindow();
	wnd->RedrawGL();
}

void CModelViewer::OnSelectPartElements()
{
	CModelDocument* doc = dynamic_cast<CModelDocument*>(GetDocument());
	GModel& m = doc->GetFEModel()->GetModel();

	if (m_selection.size() != 1) return;
	GPart* pg = dynamic_cast<GPart*>(m_selection[0]); assert(pg);

	GObject* po = dynamic_cast<GObject*>(pg->Object());
	if (po == nullptr) return;

	FEMesh* pm = po->GetFEMesh();
	if (pm == nullptr) return;

	// set the correct selection mode
	doc->SetSelectionMode(SELECT_OBJECT);
	doc->SetItemMode(ITEM_ELEM);

	// make sure this object is selected first
	doc->DoCommand(new CCmdSelectObject(&m, po, false));

	// now, select the elements
	int lid = pg->GetLocalID();
	vector<int> elemList;
	for (int i = 0; i < pm->Elements(); ++i)
	{
		FEElement& el = pm->Element(i);
		if (el.m_gid == lid) elemList.push_back(i);
	}

	// select elements
	doc->DoCommand(new CCmdSelectElements(pm, elemList, false));

	CMainWindow* wnd = GetMainWindow();
	wnd->RedrawGL();
}

void CModelViewer::OnShowPart()
{
	CModelDocument* doc = dynamic_cast<CModelDocument*>(GetDocument());
	GModel& m = doc->GetFEModel()->GetModel();

	for (int i = 0; i<(int)m_selection.size(); ++i)
	{
		GPart* pg = dynamic_cast<GPart*>(m_selection[i]); assert(pg);
		if (pg) 
		{
			m.ShowPart(pg);

			QTreeWidgetItem* item = ui->tree->FindItem(pg);
			if (item) item->setForeground(0, Qt::black);
		}
	}
	CMainWindow* wnd = GetMainWindow();
	wnd->RedrawGL();
}

void CModelViewer::OnSelectPart()
{
	CMainWindow* wnd = GetMainWindow();
	wnd->SetSelectionMode(SELECT_PART);

	UpdateSelection();

	vector<int> part;
	for (int i = 0; i<(int)m_selection.size(); ++i)
	{
		GPart* pg = dynamic_cast<GPart*>(m_selection[i]); assert(pg);
		if (pg && pg->IsVisible()) part.push_back(pg->GetID());
	}
	CModelDocument* doc = dynamic_cast<CModelDocument*>(GetDocument());
	CCmdSelectPart* cmd = new CCmdSelectPart(doc->GetGModel(), part, false);
	doc->DoCommand(cmd);
	wnd->RedrawGL();
}

void CModelViewer::OnSelectSurface()
{
	CMainWindow* wnd = GetMainWindow();
	wnd->SetSelectionMode(SELECT_FACE);

	UpdateSelection();

	vector<int> surf;
	for (int i = 0; i<(int)m_selection.size(); ++i)
	{
		GFace* pg = dynamic_cast<GFace*>(m_selection[i]); assert(pg);
		if (pg && pg->IsVisible()) surf.push_back(pg->GetID());
	}
	CModelDocument* doc = dynamic_cast<CModelDocument*>(GetDocument());
	CCmdSelectSurface* cmd = new CCmdSelectSurface(doc->GetGModel(), surf, false);
	doc->DoCommand(cmd);
	wnd->RedrawGL();
}

void CModelViewer::OnSelectCurve()
{
	CMainWindow* wnd = GetMainWindow();
	wnd->SetSelectionMode(SELECT_EDGE);

	UpdateSelection();

	vector<int> edge;
	for (int i = 0; i<(int)m_selection.size(); ++i)
	{
		GEdge* pg = dynamic_cast<GEdge*>(m_selection[i]); assert(pg);
		if (pg && pg->IsVisible()) edge.push_back(pg->GetID());
	}

	CModelDocument* doc = dynamic_cast<CModelDocument*>(GetDocument());
	CCmdSelectEdge* cmd = new CCmdSelectEdge(doc->GetGModel(), edge, false);
	doc->DoCommand(cmd);
	wnd->RedrawGL();
}

void CModelViewer::OnSelectNode()
{
	CMainWindow* wnd = GetMainWindow();
	wnd->SetSelectionMode(SELECT_NODE);

	UpdateSelection();

	vector<int> node;
	for (int i = 0; i<(int)m_selection.size(); ++i)
	{
		GNode* pg = dynamic_cast<GNode*>(m_selection[i]); assert(pg);
		if (pg && pg->IsVisible()) node.push_back(pg->GetID());
	}

	CModelDocument* doc = dynamic_cast<CModelDocument*>(GetDocument());
	CCmdSelectNode* cmd = new CCmdSelectNode(doc->GetGModel(), node, false);
	doc->DoCommand(cmd);
	wnd->RedrawGL();
}

void CModelViewer::OnCopyMaterial()
{
	GMaterial* pmat = dynamic_cast<GMaterial*>(m_currentObject); assert(pmat);
	if (pmat == 0) return;

	// create a copy of the material
	FEMaterial* pm = pmat->GetMaterialProperties();
	FEMaterial* pmCopy = 0;
	if (pm)
	{
		pmCopy = FEMaterialFactory::Create(pm->Type());
		pmCopy->copy(pm);
	}
	GMaterial* pmat2 = new GMaterial(pmCopy);

	// get material ID
	int nid = pmat2->GetID();

	// add the material to the material deck
	CModelDocument* doc = dynamic_cast<CModelDocument*>(GetDocument());
	doc->DoCommand(new CCmdAddMaterial(doc->GetFEModel(), pmat2));

	// update the model viewer
	Update();
	Select(pmat2);
}

void CModelViewer::OnChangeMaterial()
{
	GMaterial* gmat = dynamic_cast<GMaterial*>(m_currentObject); assert(gmat);
	if (gmat == 0) return;

	CModelDocument* doc = dynamic_cast<CModelDocument*>(GetDocument());
	FEProject& prj = doc->GetProject();

	CMaterialEditor dlg(prj, this);
	dlg.SetInitMaterial(gmat);
	if (dlg.exec())
	{
		FEMaterial* pmat = dlg.GetMaterial();
		gmat->SetMaterialProperties(pmat);
		gmat->SetName(dlg.GetMaterialName().toStdString());
		Update();
		Select(gmat);
	}
}

void CModelViewer::OnMaterialHideParts()
{
	CModelDocument* pdoc = dynamic_cast<CModelDocument*>(GetDocument());
	FEModel* fem = pdoc->GetFEModel();
	GModel& mdl = fem->GetModel();
	list<GPart*> partList;
	for (int i = 0; i < m_selection.size(); ++i)
	{
		GMaterial* mat = dynamic_cast<GMaterial*>(m_selection[i]); assert(mat);
		if (mat)
		{
			list<GPart*> partList_i = mdl.FindPartsFromMaterial(mat->GetID());
			if (partList_i.empty() == false)
			{
				partList.insert(partList.end(), partList_i.begin(), partList_i.end());
			}

		}
	}
	if (partList.empty() == false)
	{
		pdoc->DoCommand(new CCmdHideParts(&mdl, partList));
		GetMainWindow()->RedrawGL();
	}
}

void CModelViewer::OnMaterialShowParts()
{
	CModelDocument* pdoc = dynamic_cast<CModelDocument*>(GetDocument());
	FEModel* fem = pdoc->GetFEModel();
	GModel& mdl = fem->GetModel();
	list<GPart*> partList;
	for (int i = 0; i < m_selection.size(); ++i)
	{
		GMaterial* mat = dynamic_cast<GMaterial*>(m_selection[i]); assert(mat);
		if (mat)
		{
			list<GPart*> partList_i = mdl.FindPartsFromMaterial(mat->GetID());
			if (partList_i.empty() == false)
			{
				partList.insert(partList.end(), partList_i.begin(), partList_i.end());
			}

		}
	}
	if (partList.empty() == false)
	{
		pdoc->DoCommand(new CCmdShowParts(&mdl, partList));
		GetMainWindow()->RedrawGL();
	}
}


void CModelViewer::OnMaterialHideOtherParts()
{
	GMaterial* mat = dynamic_cast<GMaterial*>(m_currentObject); assert(mat);
	if (mat == 0) return;

	CModelDocument* pdoc = dynamic_cast<CModelDocument*>(GetDocument());
	FEModel* fem = pdoc->GetFEModel();
	GModel& mdl = fem->GetModel();
	list<GPart*> partList = mdl.FindPartsFromMaterial(mat->GetID(), false);

	pdoc->DoCommand(new CCmdHideParts(&mdl, partList));
	GetMainWindow()->RedrawGL();
}

void CModelViewer::OnCopyInterface()
{
	FEInterface* pic = dynamic_cast<FEInterface*>(m_currentObject); assert(pic);
	if (pic == 0) return;

	CModelDocument* pdoc = dynamic_cast<CModelDocument*>(GetDocument());
	FEModel* fem = pdoc->GetFEModel();

	// copy the interface
	FEMKernel* fecore = FEMKernel::Instance();
	FEInterface* piCopy = dynamic_cast<FEInterface*>(fecore->Create(fem, FE_INTERFACE, pic->Type()));
	assert(piCopy);

	// create a name
	string name = defaultInterfaceName(fem, pic);
	piCopy->SetName(name);

	// copy parameters
	piCopy->GetParamBlock() = pic->GetParamBlock();

	// add the interface to the doc
	FEStep* step = fem->GetStep(pic->GetStep());
	pdoc->DoCommand(new CCmdAddInterface(step, piCopy));

	// update the model viewer
	Update();
	Select(piCopy);
}

void CModelViewer::OnCopyBC()
{
	FEBoundaryCondition* pbc = dynamic_cast<FEBoundaryCondition*>(m_currentObject); assert(pbc);
	if (pbc == 0) return;

	CModelDocument* pdoc = dynamic_cast<CModelDocument*>(GetDocument());
	FEModel* fem = pdoc->GetFEModel();

	// copy the bc
	FEMKernel* fecore = FEMKernel::Instance();
	FEBoundaryCondition* pbcCopy = dynamic_cast<FEBoundaryCondition*>(fecore->Create(fem, FE_ESSENTIAL_BC, pbc->Type()));
	assert(pbcCopy);

	// create a name
	string name = defaultBCName(fem, pbc);
	pbcCopy->SetName(name);

	// copy parameters
	pbcCopy->GetParamBlock() = pbc->GetParamBlock();

	// add the bc to the doc
	FEStep* step = fem->GetStep(pbc->GetStep());
	pdoc->DoCommand(new CCmdAddBC(step, pbcCopy));

	// update the model viewer
	Update();
	Select(pbcCopy);
}

void CModelViewer::OnCopyIC()
{
	FEInitialCondition* pic = dynamic_cast<FEInitialCondition*>(m_currentObject); assert(pic);
	if (pic == 0) return;

	CModelDocument* pdoc = dynamic_cast<CModelDocument*>(GetDocument());
	FEModel* fem = pdoc->GetFEModel();

	// copy the ic
	FEMKernel* fecore = FEMKernel::Instance();
	FEInitialCondition* picCopy = dynamic_cast<FEInitialCondition*>(fecore->Create(fem, FE_INITIAL_CONDITION, pic->Type()));
	assert(picCopy);

	// create a name
	string name = defaultICName(fem, pic);
	picCopy->SetName(name);

	// copy parameters
	picCopy->GetParamBlock() = pic->GetParamBlock();

	// add the ic to the doc
	FEStep* step = fem->GetStep(pic->GetStep());
	pdoc->DoCommand(new CCmdAddIC(step, picCopy));

	// update the model viewer
	Update();
	Select(picCopy);
}

void CModelViewer::OnCopyRigidConnector()
{
	FERigidConnector* pc = dynamic_cast<FERigidConnector*>(m_currentObject); assert(pc);
	if (pc == 0) return;

	CModelDocument* pdoc = dynamic_cast<CModelDocument*>(GetDocument());
	FEModel* fem = pdoc->GetFEModel();

	// copy the load
	FEMKernel* fecore = FEMKernel::Instance();
	FERigidConnector* pcCopy =  dynamic_cast<FERigidConnector*>(fecore->Create(fem, FE_RIGID_CONNECTOR, pc->Type()));
	assert(pcCopy);

	// create a name
	string name = defaultRigidConnectorName(fem, pc);
	pcCopy->SetName(name);

	// copy parameters
	pcCopy->GetParamBlock() = pc->GetParamBlock();

	// add the load to the doc
	FEStep* step = fem->GetStep(pc->GetStep());
	pdoc->DoCommand(new CCmdAddRigidConnector(step, pcCopy));

	// update the model viewer
	Update();
	Select(pcCopy);
}

void CModelViewer::OnCopyConstraint()
{
	FEModelConstraint* pc = dynamic_cast<FEModelConstraint*>(m_currentObject); assert(pc);

	CModelDocument* pdoc = dynamic_cast<CModelDocument*>(GetDocument());
	FEModel* fem = pdoc->GetFEModel();

	// copy the load
	FEMKernel* fecore = FEMKernel::Instance();
	FEModelConstraint* pcCopy = dynamic_cast<FEModelConstraint*>(fecore->Create(fem, FE_CONSTRAINT, pc->Type()));
	assert(pcCopy);

	// create a name
	string name = defaultConstraintName(fem, pc);
	pcCopy->SetName(name);

	// copy parameters
	pcCopy->GetParamBlock() = pc->GetParamBlock();

	// add the constraint to the doc
	FEStep* step = fem->GetStep(pc->GetStep());
	pdoc->DoCommand(new CCmdAddConstraint(step, pcCopy));

	// update the model viewer
	Update();
	Select(pcCopy);
}

void CModelViewer::OnCopyLoad()
{
	FELoad* pl = dynamic_cast<FELoad*>(m_currentObject); assert(pl);
	if (pl == 0) return;

	CModelDocument* pdoc = dynamic_cast<CModelDocument*>(GetDocument());
	FEModel* fem = pdoc->GetFEModel();

	// copy the load
	FEMKernel* fecore = FEMKernel::Instance();
	FELoad* plCopy = 0;
	if (dynamic_cast<FESurfaceLoad*>(pl))
		plCopy = dynamic_cast<FELoad*>(fecore->Create(fem, FE_SURFACE_LOAD, pl->Type()));
	else if (dynamic_cast<FEBodyLoad*>(pl))
		plCopy = dynamic_cast<FELoad*>(fecore->Create(fem, FE_BODY_LOAD, pl->Type()));
	else if (dynamic_cast<FENodalLoad*>(pl))
		plCopy = new FENodalLoad(fem);
	assert(plCopy);

	// create a name
	string name = defaultLoadName(fem, pl);
	plCopy->SetName(name);

	// copy parameters
	plCopy->GetParamBlock() = pl->GetParamBlock();

	// add the load to the doc
	FEStep* step = fem->GetStep(pl->GetStep());
	pdoc->DoCommand(new CCmdAddLoad(step, plCopy));

	// update the model viewer
	Update();
	Select(plCopy);
}

void CModelViewer::OnCopyRigidConstraint()
{
	FERigidConstraint* pc = dynamic_cast<FERigidConstraint*>(m_currentObject); assert(pc);
	if (pc == 0) return;

	CModelDocument* pdoc = dynamic_cast<CModelDocument*>(GetDocument());
	FEModel* fem = pdoc->GetFEModel();

	// copy the load
	FEMKernel* fecore = FEMKernel::Instance();
	FERigidConstraint* pcCopy = dynamic_cast<FERigidConstraint*>(fecore->Create(fem, FE_RIGID_CONSTRAINT, pc->Type()));
	assert(pcCopy);

	// create a name
	string name = defaultRigidConstraintName(fem, pc);
	pcCopy->SetName(name);

	// copy parameters
	pcCopy->GetParamBlock() = pc->GetParamBlock();

	// add the load to the doc
	FEStep* step = fem->GetStep(pc->GetStep());
	pdoc->DoCommand(new CCmdAddRC(step, pcCopy));

	// update the model viewer
	Update();
	Select(pcCopy);
}

void CModelViewer::OnCopyStep()
{
	FEAnalysisStep* ps = dynamic_cast<FEAnalysisStep*>(m_currentObject); assert(ps);
	if (ps == 0) return;

	CModelDocument* pdoc = dynamic_cast<CModelDocument*>(GetDocument());
	FEModel* fem = pdoc->GetFEModel();

	// copy the step
	FEMKernel* fecore = FEMKernel::Instance();
	FEAnalysisStep* psCopy = dynamic_cast<FEAnalysisStep*>(fecore->Create(fem, FE_ANALYSIS, ps->GetType()));
	assert(psCopy);

	// create a name
	string name = defaultStepName(fem, ps);
	psCopy->SetName(name);

	// copy parameters
	psCopy->GetParamBlock() = ps->GetParamBlock();
	psCopy->GetSettings() = ps->GetSettings();

	// add the step to the doc
	pdoc->DoCommand(new CCmdAddStep(fem, psCopy));

	// update the model viewer
	Update();
	Select(psCopy);
}

void CModelViewer::OnStepMoveUp()
{
	FEAnalysisStep* ps = dynamic_cast<FEAnalysisStep*>(m_currentObject); assert(ps);
	if (ps == 0) return;

	CModelDocument* pdoc = dynamic_cast<CModelDocument*>(GetDocument());
	FEModel* fem = pdoc->GetFEModel();

	int n = fem->GetStepIndex(ps); assert(n >= 1);
	if (n > 1)
	{
		pdoc->DoCommand(new CCmdSwapSteps(fem, ps, fem->GetStep(n - 1)));
		Update();
		Select(ps);
	}
}

void CModelViewer::OnStepMoveDown()
{
	FEAnalysisStep* ps = dynamic_cast<FEAnalysisStep*>(m_currentObject); assert(ps);
	if (ps == 0) return;

	CModelDocument* pdoc = dynamic_cast<CModelDocument*>(GetDocument());
	FEModel* fem = pdoc->GetFEModel();

	int n = fem->GetStepIndex(ps); assert(n >= 1);
	if (n < fem->Steps() - 1)
	{
		pdoc->DoCommand(new CCmdSwapSteps(fem, ps, fem->GetStep(n + 1)));
		Update();
		Select(ps);
	}
}

void CModelViewer::OnRerunJob()
{
	CFEBioJob* job = dynamic_cast<CFEBioJob*>(m_currentObject); assert(job);
	if (job == 0) return;

	CMainWindow* wnd = GetMainWindow();
	wnd->RunFEBioJob(job);
}

void CModelViewer::OnOpenJob()
{
	FSObject* po = GetCurrentObject();
	if (po == nullptr) return;

	CFEBioJob* job = dynamic_cast<CFEBioJob*>(po);
	if (job == nullptr) return;

	CDocument* doc = job->GetDocument();
	assert(doc);
	QString plotFile = doc->ToAbsolutePath(job->GetPlotFileName());

	GetMainWindow()->OpenPostFile(plotFile, dynamic_cast<CModelDocument*>(doc), false);
}

void CModelViewer::OnEditOutput()
{
	CModelDocument* pdoc = dynamic_cast<CModelDocument*>(GetDocument());
	FEProject& prj = pdoc->GetProject();

	CDlgEditOutput dlg(prj, this);
	dlg.exec();	
	Update();
}

void CModelViewer::OnEditOutputLog()
{
	CModelDocument* pdoc = dynamic_cast<CModelDocument*>(GetDocument());
	FEProject& prj = pdoc->GetProject();

	CDlgEditOutput dlg(prj, this, 1);
	dlg.exec();
	Update();
}

void CModelViewer::OnRemoveEmptySelections()
{
	CModelDocument* pdoc = dynamic_cast<CModelDocument*>(GetDocument());
	GModel& mdl = pdoc->GetFEModel()->GetModel();
	mdl.RemoveEmptySelections();
	Update();
}

void CModelViewer::OnRemoveAllSelections()
{
	CModelDocument* pdoc = dynamic_cast<CModelDocument*>(GetDocument());
	GModel& mdl = pdoc->GetFEModel()->GetModel();
	mdl.RemoveNamedSelections();
	Update();
}

// clear current FSObject selection
void CModelViewer::ClearSelection()
{
	m_selection.clear();
}

// set the current FSObject selection
void CModelViewer::SetSelection(std::vector<FSObject*>& sel)
{
	m_selection = sel;
}

void CModelViewer::SetSelection(FSObject* sel)
{
	m_selection.clear();
	m_selection.push_back(sel);
}

// show the context menu
void CModelViewer::ShowContextMenu(CModelTreeItem* data, QPoint pt)
{
	if (data == 0) return;

	QMenu menu(this);

	// add delete action
	bool del = false;

	CMainWindow* wnd = GetMainWindow();

	CModelDocument* doc = dynamic_cast<CModelDocument*>(GetDocument());
	GModel* gm = doc->GetGModel();

	switch (data->type)
	{
	case MT_OBJECT_LIST:
	{
		menu.addAction("Show All Objects", this, SLOT(OnUnhideAllObjects()));
		menu.addSeparator();

		QMenu* sub = new QMenu("Set Active Mesh Layer");
		int layers = gm->MeshLayers();
		int activeLayer = gm->GetActiveMeshLayer();
		for (int i = 0; i < layers; ++i)
		{
			string s = gm->GetMeshLayerName(i);
			QAction* a = sub->addAction(QString::fromStdString(s));
			a->setCheckable(true);
			if (i == activeLayer) a->setChecked(true);
		}

		QObject::connect(sub, SIGNAL(triggered(QAction*)), GetMainWindow(), SLOT(OnSelectMeshLayer(QAction*)));

		menu.addAction(sub->menuAction());
		menu.addAction("New Mesh Layer ...", this, SLOT(OnCreateNewMeshLayer()));

		if (layers > 1)
		{
			menu.addAction("Delete Active Mesh Layer", this, SLOT(OnDeleteMeshLayer()));
		}
	}
	break;
	case MT_PART_LIST:
		menu.addAction("Show All Parts", this, SLOT(OnUnhideAllParts()));
		break;
	case MT_MATERIAL_LIST:
		menu.addAction("Add Material ...", this, SLOT(OnAddMaterial()));
		menu.addAction("Export Materials ...", this, SLOT(OnExportAllMaterials()));
		menu.addAction("Import Materials ...", this, SLOT(OnImportMaterials()));
		menu.addSeparator();
		menu.addAction("Delete All", this, SLOT(OnDeleteAllMaterials()));
		break;
	case MT_BC_LIST:
		menu.addAction("Add Boundary Condition ...", this, SLOT(OnAddBC()));
		menu.addSeparator();
		menu.addAction("Delete All", this, SLOT(OnDeleteAllBC()));
		break;
	case MT_LOAD_LIST:
		menu.addAction("Add Nodal Load ...", wnd, SLOT(on_actionAddNodalLoad_triggered()));
		menu.addAction("Add Surface Load ...", this, SLOT(OnAddSurfaceLoad()));
		menu.addAction("Add Body Load ...", this, SLOT(OnAddBodyLoad()));
		menu.addSeparator();
		menu.addAction("Delete All", this, SLOT(OnDeleteAllLoads()));
		break;
	case MT_IC_LIST:
		menu.addAction("Add Initial Condition ...", this, SLOT(OnAddInitialCondition()));
		menu.addSeparator();
		menu.addAction("Delete All", this, SLOT(OnDeleteAllIC()));
		break;
	case MT_CONTACT_LIST:
		menu.addAction("Add Contact Interface ...", this, SLOT(OnAddContact()));
		menu.addSeparator();
		menu.addAction("Delete All", this, SLOT(OnDeleteAllContact()));
		break;
	case MT_CONSTRAINT_LIST:
		menu.addAction("Add Constraint ...", this, SLOT(OnAddConstraint()));
		menu.addSeparator();
		menu.addAction("Delete All", this, SLOT(OnDeleteAllConstraints()));
		break;
	case MT_RIGID_CONSTRAINT_LIST:
		menu.addAction("Add Rigid Constraint ...", this, SLOT(OnAddRigidConstraint()));
		menu.addSeparator();
		menu.addAction("Delete All", this, SLOT(OnDeleteAllRigidConstraints()));
		break;
	case MT_RIGID_CONNECTOR_LIST:
		menu.addAction("Add Rigid Connector ...", this, SLOT(OnAddRigidConnector()));
		menu.addSeparator();
		menu.addAction("Delete All", this, SLOT(OnDeleteAllRigidConnectors()));
		break;
	case MT_STEP_LIST:
		menu.addAction("Add Analysis Step ...", this, SLOT(OnAddStep()));
		menu.addSeparator();
		menu.addAction("Delete All", this, SLOT(OnDeleteAllSteps()));
		break;
	case MT_PROJECT_OUTPUT:
		menu.addAction("Edit output...", this, SLOT(OnEditOutput()));
		break;
	case MT_PROJECT_OUTPUT_LOG:
		menu.addAction("Edit output...", this, SLOT(OnEditOutputLog()));
		break;
	case MT_NAMED_SELECTION:
		menu.addAction("Remove empty", this, SLOT(OnRemoveEmptySelections()));
		menu.addAction("Remove all", this, SLOT(OnRemoveAllSelections()));
		break;
	case MT_OBJECT:
	{
		GObject* po = dynamic_cast<GObject*>(data->obj);
		if (po)
		{
			if (po->IsVisible())
			{
				menu.addAction("Select", this, SLOT(OnSelectObject()));
				menu.addAction("Hide", this, SLOT(OnHideObject()));
			}
			else
				menu.addAction("Show", this, SLOT(OnShowObject()));

			del = true;
		}
	}
	break;
	case MT_PART:
	{
		GPart* pg = dynamic_cast<GPart*>(data->obj);
		if (pg)
		{
			if (pg->IsVisible())
			{
				menu.addAction("Select", this, SLOT(OnSelectPart()));
				menu.addAction("Hide", this, SLOT(OnHidePart()));
				menu.addAction("Select elements", this, SLOT(OnSelectPartElements()));
			}
			else
				menu.addAction("Show", this, SLOT(OnShowPart()));

			// only parts of a GMeshObject can be deleted
			if (dynamic_cast<GMeshObject*>(pg->Object()))
				del = true;
		}
	}
	break;
	case MT_SURFACE:
		menu.addAction("Select", this, SLOT(OnSelectSurface()));
		break;
	case MT_EDGE:
		menu.addAction("Select", this, SLOT(OnSelectCurve()));
		break;
	case MT_NODE:
		menu.addAction("Select", this, SLOT(OnSelectNode()));
		break;
	case MT_MATERIAL:
		menu.addAction("Copy", this, SLOT(OnCopyMaterial()));
		menu.addAction("Change...", this, SLOT(OnChangeMaterial()));
		menu.addAction("Hide parts", this, SLOT(OnMaterialHideParts()));
		menu.addAction("Show parts", this, SLOT(OnMaterialShowParts()));
		menu.addAction("Hide other parts", this, SLOT(OnMaterialHideOtherParts()));
		menu.addAction("Export Material(s) ...", this, SLOT(OnExportMaterials()));
		del = true;
		break;
	case MT_DISCRETE_LIST:
		menu.addAction("Delete all", this, SLOT(OnDeleteAllDiscete()));
		menu.addAction("Hide all", this, SLOT(OnHideAllDiscrete()));
		menu.addAction("Show all", this, SLOT(OnShowAllDiscrete()));
		break;
	case MT_DISCRETE_SET:
		menu.addAction("Select", this, SLOT(OnSelectDiscreteObject()));
		menu.addAction("Hide"  , this, SLOT(OnHideDiscreteObject()));
		menu.addAction("Show"  , this, SLOT(OnShowDiscreteObject()));
		menu.addAction("Detach", this, SLOT(OnDetachDiscreteObject()));
		menu.addAction("Change Type ...", this, SLOT(OnChangeDiscreteType()));
		del = true;
		break;
	case MT_DISCRETE:
		menu.addAction("Select", this, SLOT(OnSelectDiscreteObject()));
		del = true;
		break;
	case MT_CONTACT:
	{
		menu.addAction("Copy", this, SLOT(OnCopyInterface()));
		FEPairedInterface* pci = dynamic_cast<FEPairedInterface*>(data->obj);
		if (pci)
		{
			menu.addAction("Swap Primary/Secondary", this, SLOT(OnSwapMasterSlave()));
		}
		del = true;
	}
	break;
	case MT_BC:
		menu.addAction("Copy", this, SLOT(OnCopyBC()));
		del = true;
		break;
	case MT_RIGID_CONNECTOR:
		menu.addAction("Copy", this, SLOT(OnCopyRigidConnector()));
		del = true;
		break;
	case MT_IC:
		menu.addAction("Copy", this, SLOT(OnCopyIC()));
		del = true;
		break;
	case MT_LOAD:
		menu.addAction("Copy", this, SLOT(OnCopyLoad()));
		del = true;
		break;
	case MT_RIGID_CONSTRAINT:
		menu.addAction("Copy", this, SLOT(OnCopyRigidConstraint()));
		del = true;
		break;
	case MT_CONSTRAINT:
		menu.addAction("Copy", this, SLOT(OnCopyConstraint()));
		del = true;
		break;
	case MT_STEP:
		{
			menu.addAction("Copy", this, SLOT(OnCopyStep()));
			FEAnalysisStep* step = dynamic_cast<FEAnalysisStep*>(data->obj);
			if (step)
			{
				menu.addAction("Move Up", this, SLOT(OnStepMoveUp()));
				menu.addAction("Move Down", this, SLOT(OnStepMoveDown()));
			}
			del = true;
		}
		break;
	case MT_JOBLIST:
		{
			menu.addAction("Delete All", this, SLOT(OnDeleteAllJobs()));
		}
		break;
	case MT_JOB:
		menu.addAction("Open", this, SLOT(OnOpenJob()));
		menu.addAction("Rerun job", this, SLOT(OnRerunJob()));
		del = true;
		break;
	default:
		return;
	}

	if (del) 
	{
		menu.addSeparator();
		menu.addAction("Delete", this, SLOT(OnDeleteItem()));
	}

	menu.exec(pt);
}

void CModelViewer::OnExportMaterials()
{
	vector<GMaterial*> matList;

	if (m_selection.size() == 0)
	{
		GMaterial* m = dynamic_cast<GMaterial*>(m_currentObject);
		if (m) matList.push_back(m);
	}
	else
	{
		for (int i=0; i<(int)m_selection.size(); ++i)
		{
			FSObject* po = m_selection[i];
			GMaterial* m = dynamic_cast<GMaterial*>(po);
			if (m) matList.push_back(m);
		}
	}

	if (matList.size() > 0)
		GetMainWindow()->onExportMaterials(matList);
}

void CModelViewer::OnExportAllMaterials()
{
	GetMainWindow()->onExportAllMaterials();
}

void CModelViewer::OnImportMaterials()
{
	GetMainWindow()->onImportMaterials();
}

void CModelViewer::OnDeleteAllMaterials()
{
	GetMainWindow()->DeleteAllMaterials();
}

void CModelViewer::OnSwapMasterSlave()
{
	FEPairedInterface* pci = dynamic_cast<FEPairedInterface*>(m_currentObject);
	if (pci)
	{
		pci->SwapPrimarySecondary();
		UpdateObject(m_currentObject);
	}
}

void CModelViewer::OnDeleteAllBC()
{
	GetMainWindow()->DeleteAllBC();
}

void CModelViewer::OnDeleteAllLoads()
{
	GetMainWindow()->DeleteAllLoads();
}

void CModelViewer::OnDeleteAllIC()
{
	GetMainWindow()->DeleteAllIC();
}

void CModelViewer::OnDeleteAllContact()
{
	GetMainWindow()->DeleteAllContact();
}

void CModelViewer::OnDeleteAllConstraints()
{
	GetMainWindow()->DeleteAllConstraints();
}

void CModelViewer::OnDeleteAllRigidConstraints()
{
	GetMainWindow()->DeleteAllRigidConstraints();
}

void CModelViewer::OnDeleteAllRigidConnectors()
{
	GetMainWindow()->DeleteAllRigidConnectors();
}

void CModelViewer::OnDeleteAllSteps()
{
	GetMainWindow()->DeleteAllSteps();
}

void CModelViewer::OnDeleteAllJobs()
{
	GetMainWindow()->DeleteAllJobs();
}
