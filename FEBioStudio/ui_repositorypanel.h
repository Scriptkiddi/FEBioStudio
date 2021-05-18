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

#ifdef MODEL_REPO
#include <unordered_map>
#include <QApplication>
#include <QLocale>
#include <QPalette>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QToolButton>
#include <QToolBar>
#include <QBoxLayout>
#include <QSplitter>
#include <QToolButton>
#include <QStackedLayout>
#include <QStackedWidget>
#include <QFormLayout>
#include <QLineEdit>
#include <QTextBrowser>
#include <QProgressBar>
#include <QLabel>
#include <QFont>
#include <QPushButton>
#include <QTreeWidget>
#include <QHeaderView>
#include <QJsonDocument>
#include <QByteArray>
#include <QDir>
#include <QFileIconProvider>
#include <JlCompress.h>
#include <QStandardPaths>
#include "ToolBox.h"
#include "PublicationWidgetView.h"
#include "IconProvider.h"
#include "MultiLineLabel.h"
#include "WrapLabel.h"
#include "TagLabel.h"

#include <iostream>
#include <QDebug>

enum ITEMTYPES {PROJECTITEM = 1001, FOLDERITEM = 1002, FILEITEM = 1003};

class CustomTreeWidgetItem : public QTreeWidgetItem
{
public:
	CustomTreeWidgetItem(QString name, int type)
		: QTreeWidgetItem(QStringList(name), type), localCopy(0), totalCopies(0), m_size(0)
	{

	}

	virtual CustomTreeWidgetItem* getProjectItem() = 0;

	bool LocalCopy() { return localCopy >= totalCopies; }

	int GetLocalCopy() { return localCopy; }
	int GetTotalCopies() { return totalCopies; }

	virtual void UpdateLocalCopyColor()
	{
		if(LocalCopy())
		{
			setForeground(0, qApp->palette().color(QPalette::Active, QPalette::Text));
			setForeground(1, qApp->palette().color(QPalette::Active, QPalette::Text));
		}
		else
		{
			setForeground(0, qApp->palette().color(QPalette::Disabled, QPalette::Text));
			setForeground(1, qApp->palette().color(QPalette::Disabled, QPalette::Text));
		}
	}


	void AddLocalCopy()
	{
		localCopy++;
		UpdateLocalCopyColor();

		if(type() != PROJECTITEM)
		{
			static_cast<CustomTreeWidgetItem*>(parent())->AddLocalCopy();

		}
	}

	void SubtractLocalCopy()
	{
		localCopy--;
		UpdateLocalCopyColor();

		if(type() != PROJECTITEM)
		{
			static_cast<CustomTreeWidgetItem*>(parent())->SubtractLocalCopy();

		}
	}

	void setLocalCopyRecursive(bool lc)
	{
		for(int index = 0; index < childCount(); index++)
		{
			static_cast<CustomTreeWidgetItem*>(child(index))->setLocalCopyRecursive(lc);
		}

		if(lc)
		{
			AddLocalCopy();
		}
		else
		{
			SubtractLocalCopy();
		}

	}

	void AddTotalCopy()
	{
		totalCopies++;
		UpdateLocalCopyColor();
	}

	void UpdateCopies()
	{
		int lc = 0;
		int tc = 0;

		for(int index = 0; index < childCount(); index++)
		{
			CustomTreeWidgetItem* current = static_cast<CustomTreeWidgetItem*>(child(index));
			current->UpdateCopies();

			lc += current->GetLocalCopy();
			tc += current->GetTotalCopies();
		}

		localCopy += lc;
		totalCopies += tc;
		UpdateLocalCopyColor();
	}

	void UpdateSize()
	{
		int currentSize = 0;

		for(int index = 0; index < childCount(); index++)
		{
			CustomTreeWidgetItem* current = static_cast<CustomTreeWidgetItem*>(child(index));
			current->UpdateSize();

			currentSize += current->m_size;
		}

		m_size += currentSize;

		setText(1, qApp->topLevelWidgets()[0]->locale().formattedDataSize(m_size, 2, QLocale::DataSizeTraditionalFormat));
	}

protected:
	int localCopy;
	int totalCopies;

	qint64 m_size;

};

class ProjectItem : public CustomTreeWidgetItem
{
public:
	ProjectItem(QString name, int projectID, bool owned, bool authorized)
		: CustomTreeWidgetItem(name, PROJECTITEM), m_projectID(projectID), m_ownedByUser(owned), m_authorized(authorized)
	{
		setIcon(0, CIconProvider::GetIcon("FEBioStudio"));
	}

	CustomTreeWidgetItem* getProjectItem()
	{
		return this;
	}

	void setProjectID(int project) {m_projectID = project;}
	int getProjectID() {return m_projectID;}
	bool ownedByUser() {return m_ownedByUser;}
	bool isAuthorized() {return m_authorized;}

private:
	int m_projectID;
	bool m_ownedByUser;
	bool m_authorized;
};

class FolderItem : public CustomTreeWidgetItem
{
public:
	FolderItem(QString name)
		: CustomTreeWidgetItem(name, FOLDERITEM)
	{
		setIcon(0, CIconProvider::GetIcon("folder"));
	}

	CustomTreeWidgetItem* getProjectItem()
	{
		return ((CustomTreeWidgetItem*) parent())->getProjectItem();
	}

};

class FileItem : public CustomTreeWidgetItem
{
public:
	FileItem(QString name, int fileID, bool lc, qint64 size)
		: CustomTreeWidgetItem(name, FILEITEM), m_fileID(fileID)
	{
		if(name.endsWith(".fsp"))
		{
			setIcon(0, CIconProvider::GetIcon("FEBioStudio"));
		}
		else if(name.endsWith(".fsm") || name.endsWith(".fsprj") || name.endsWith(".prv"))
		{
			setIcon(0, CIconProvider::GetIcon("PreView"));
		}
		else if(name.endsWith(".feb"))
		{
			setIcon(0, CIconProvider::GetIcon("febio"));
		}
		else if(name.endsWith(".xplt"))
		{
			setIcon(0, CIconProvider::GetIcon("PostView"));
		}
		else
		{
			setIcon(0, CIconProvider::GetIcon("new"));
		}


		localCopy = (lc ? 1 : 0);

		totalCopies = 1;

		UpdateLocalCopyColor();

		m_size = size;
	}

	virtual CustomTreeWidgetItem* getProjectItem()
	{
		return ((CustomTreeWidgetItem*) parent())->getProjectItem();
	}

	int getFileID()
	{
		return m_fileID;
	}

private:
	int m_fileID;

};

class FileSearchItem : public QTreeWidgetItem
{
public:
	FileSearchItem(FileItem* item)
		: QTreeWidgetItem(), realItem(item)
	{
		setText(0, realItem->text(0));
		setText(1, realItem->text(1));
		UpdateColor();
		setIcon(0,realItem->icon(0));
	}

	void UpdateColor()
	{
		setForeground(0, realItem->foreground(0));
		setForeground(1, realItem->foreground(1));
	}

	FileItem* getRealItem()
	{
		return realItem;
	}

private:
	FileItem* realItem;

};

class Ui::CRepositoryPanel
{
public:
	QStackedLayout* stack;

	QWidget* welcomePage;
	QPushButton* connectButton;

	// QPushButton* loginButton;
	// QAction* loginAction;

	QWidget* modelPage;
	QStackedWidget* treeStack;
	QTreeWidget* projectTree;
	QTreeWidget* fileSearchTree;

	CToolBox* projectInfoBox;

	QLabel* unauthorized;

	QFormLayout* projectInfoForm;
	QLabel* projectName;
	MultiLineLabel* projectDesc;
	QLabel* projectOwner;
	TagLabel* projectTags;

	QFormLayout* fileInfoForm;
	MultiLineLabel* filenameLabel;
	MultiLineLabel* fileDescLabel;
	TagLabel* fileTags;

	::CPublicationWidgetView* projectPubs;

	QToolBar* toolbar;

	QAction* actionRefresh;
	QAction* actionDownload;
	QAction* actionOpen;
	QAction* actionOpenFileLocation;
	QAction* actionDelete;

	QAction* actionUpload;

	QAction* actionDeleteRemote;
	QAction* actionModify;

	QAction* actionFindInTree;

	QLineEdit* searchLineEdit;
	QAction* actionSearch;
	QAction* actionClearSearch;

	QWidget* loadingPage;
	QLabel* loadingLabel;
	QProgressBar* loadingBar;
	QPushButton* loadingCancel;

public:
	CRepositoryPanel() : currentProject(nullptr), openAfterDownload(nullptr){}

	void setupUi(::CRepositoryPanel* parent)
	{
		stack = new QStackedLayout(parent);

		// Weclome Page
		QVBoxLayout* welcomeVBLayout = new QVBoxLayout;
		welcomeVBLayout->setAlignment(Qt::AlignCenter);

		QLabel* welcomeLabel = new QLabel("To access the project repository, please click the Connect button below.");
		welcomeLabel->setWordWrap(true);
		welcomeLabel->setAlignment(Qt::AlignCenter);
		welcomeVBLayout->addWidget(welcomeLabel);

		QHBoxLayout* connectButtonLayout = new QHBoxLayout;
		connectButtonLayout->addStretch();
		connectButton = new QPushButton("Connect");
		connectButton->setObjectName("connectButton");
		connectButtonLayout->addWidget(connectButton);
		connectButtonLayout->addStretch();
		welcomeVBLayout->addLayout(connectButtonLayout);

		welcomePage = new QWidget;
		welcomePage->setLayout(welcomeVBLayout);

		stack->addWidget(welcomePage);

		// Model view page
		QVBoxLayout* modelVBLayout = new QVBoxLayout;

		toolbar = new QToolBar();

		actionRefresh = new QAction(CIconProvider::GetIcon("refresh"), "Refresh", parent);
		actionRefresh->setObjectName("actionRefresh");
		actionRefresh->setIconVisibleInMenu(false);
		toolbar->addAction(actionRefresh);

		actionDownload = new QAction(CIconProvider::GetIcon("download"), "Download", parent);
		actionDownload->setObjectName("actionDownload");
		actionDownload->setIconVisibleInMenu(false);
		toolbar->addAction(actionDownload);

		actionOpen = new QAction(CIconProvider::GetIcon("open"), "Open Local Copy", parent);
		actionOpen->setObjectName("actionOpen");
		actionOpen->setIconVisibleInMenu(false);
		toolbar->addAction(actionOpen);

		actionOpenFileLocation = new QAction(CIconProvider::GetIcon("openContaining"), "Open File Location", parent);
		actionOpenFileLocation->setObjectName("actionOpenFileLocation");
		actionOpenFileLocation->setIconVisibleInMenu(false);
		toolbar->addAction(actionOpenFileLocation);

		actionDelete = new QAction(CIconProvider::GetIcon("delete"), "Delete Local Copy", parent);
		actionDelete->setObjectName("actionDelete");
		actionDelete->setIconVisibleInMenu(false);
		toolbar->addAction(actionDelete);

		toolbar->addSeparator();
		QWidget* empty = new QWidget();
		empty->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
		toolbar->addWidget(empty);

		actionDeleteRemote = new QAction(CIconProvider::GetIcon("deleteRemote"), "Delete From Repository", parent);
		actionDeleteRemote->setObjectName("actionDeleteRemote");
		toolbar->addAction(actionDeleteRemote);

		actionModify = new QAction(CIconProvider::GetIcon("edit"), "Modify Project", parent);
		actionModify->setObjectName("actionModify");
		toolbar->addAction(actionModify);


		actionUpload = new QAction(CIconProvider::GetIcon("upload"), "Upload", parent);
		actionUpload->setObjectName("actionUpload");
		toolbar->addAction(actionUpload);

		modelVBLayout->addWidget(toolbar);

		QToolBar* searchBar = new QToolBar;
		searchBar->addWidget(searchLineEdit = new QLineEdit);
		actionSearch = new QAction(CIconProvider::GetIcon("search"), "Search", parent);
		actionSearch->setObjectName("actionSearch");
		searchBar->addAction(actionSearch);
		actionClearSearch = new QAction(CIconProvider::GetIcon("clear"), "Clear", parent);
		actionClearSearch->setObjectName("actionClearSearch");
		searchBar->addAction(actionClearSearch);

		modelVBLayout->addWidget(searchBar);

		actionFindInTree = new QAction("Show in Project Tree", parent);
		actionFindInTree->setObjectName("actionFindInTree");

		QSplitter* splitter = new QSplitter;
		splitter->setOrientation(Qt::Vertical);

		treeStack = new QStackedWidget;

		projectTree = new QTreeWidget;
		projectTree->setObjectName("treeWidget");
		projectTree->setColumnCount(2);
		projectTree->setHeaderLabels(QStringList() << "Projects" << "Size");
		projectTree->setSelectionMode(QAbstractItemView::SingleSelection);
		projectTree->setContextMenuPolicy(Qt::CustomContextMenu);
		projectTree->header()->setSectionResizeMode(0, QHeaderView::Stretch);
		projectTree->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
		projectTree->header()->setStretchLastSection(false);
		treeStack->addWidget(projectTree);

		fileSearchTree = new QTreeWidget;
		fileSearchTree->setObjectName("fileSearchTree");
		fileSearchTree->setColumnCount(2);
		fileSearchTree->setHeaderLabels(QStringList() << "Files" << "Size");
		fileSearchTree->setSelectionMode(QAbstractItemView::SingleSelection);
		fileSearchTree->setContextMenuPolicy(Qt::CustomContextMenu);
		fileSearchTree->header()->setSectionResizeMode(0, QHeaderView::Stretch);
		fileSearchTree->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
		fileSearchTree->header()->setStretchLastSection(false);
		treeStack->addWidget(fileSearchTree);

		splitter->addWidget(treeStack);

		projectInfoBox = new CToolBox;
		QWidget* projectDummy = new QWidget;
		QVBoxLayout* modelInfoLayout = new QVBoxLayout;
		projectDummy->setLayout(modelInfoLayout);

		modelInfoLayout->addWidget(unauthorized = new QLabel("<font color='red'>This project has not yet been approved by our "
				"reviewers. It is visible only to you. For now, you may only modify the metadata or delete the project. Once "
				"approved, it will available for all users.</font>"));
		unauthorized->setWordWrap(true);
		unauthorized->hide();

		QHBoxLayout* centerName = new QHBoxLayout;
		centerName->addStretch();
		centerName->addWidget(projectName = new QLabel);
		centerName->addStretch();

		modelInfoLayout->addLayout(centerName);

		QFont font = projectName->font();
		font.setBold(true);
		font.setPointSize(14);
		projectName->setFont(font);

		modelInfoLayout->addWidget(projectDesc = new MultiLineLabel);

		QFrame* line = new QFrame();
		line->setFrameShape(QFrame::HLine);
		modelInfoLayout->addWidget(line);

		projectInfoForm = new QFormLayout;
		projectInfoForm->setHorizontalSpacing(10);
		projectInfoForm->addRow("Owner:", projectOwner = new QLabel);

		modelInfoLayout->addLayout(projectInfoForm);

		modelInfoLayout->addWidget(projectTags = new TagLabel);
		projectTags->setObjectName("projectTags");

		projectInfoBox->addTool("Project Info", projectDummy);

		projectInfoBox->addTool("Publications", projectPubs = new ::CPublicationWidgetView(::CPublicationWidgetView::LIST, false));
		projectInfoBox->getToolItem(1)->hide();

		QWidget* fileDummy = new QWidget;
		QVBoxLayout* fileInfoLayout = new QVBoxLayout;
		fileDummy->setLayout(fileInfoLayout);

		fileInfoForm = new QFormLayout;
		fileInfoForm->setHorizontalSpacing(10);
		fileInfoForm->addRow("Filename:", filenameLabel = new MultiLineLabel);
		fileInfoForm->addRow("Description:", fileDescLabel = new MultiLineLabel);
		fileInfoLayout->addLayout(fileInfoForm);

		fileInfoLayout->addWidget(fileTags = new TagLabel);
		fileTags->setObjectName("fileTags");

		projectInfoBox->addTool("File Info", fileDummy);
		projectInfoBox->getToolItem(2)->hide();

		splitter->addWidget(projectInfoBox);

		modelVBLayout->addWidget(splitter);

		modelPage = new QWidget;
		modelPage->setLayout(modelVBLayout);

		stack->addWidget(modelPage);

		// Loading Page
		loadingPage = new QWidget;
		QVBoxLayout* loadingLayout = new QVBoxLayout;
		loadingLayout->setAlignment(Qt::AlignCenter);

		loadingLayout->addWidget(loadingLabel = new QLabel);
		loadingLayout->setAlignment(loadingLabel, Qt::AlignCenter);
		loadingLayout->addWidget(loadingBar = new QProgressBar);
		loadingLayout->addWidget(loadingCancel = new QPushButton("Cancel"));
		loadingCancel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
		loadingLayout->setAlignment(loadingCancel, Qt::AlignCenter);

		loadingPage->setLayout(loadingLayout);
		stack->addWidget(loadingPage);

		// setLoginVisible(true);
	}

	CustomTreeWidgetItem* addFile(QString &path, int index, int fileID, bool localCopy, qint64 size)
	{
		int pos = path.right(path.length() - index).indexOf("/");

		if(pos == -1)
		{
			FileItem* child = new FileItem(path.right(path.length() - index), fileID, localCopy, size);

			fileItemsByID[fileID] = child;

			return child;
		}

		CustomTreeWidgetItem* child = addFile(path, index + (pos + 1), fileID, localCopy, size);
		CustomTreeWidgetItem* parent;

		try
		{
			parent = currentProjectFolders.at(path.left(pos + index).toStdString());
		}
		catch(std::out_of_range& e)
		{
			parent = new FolderItem(path.right(path.length() - index).left(pos));

			currentProjectFolders[path.left(pos + index).toStdString()] = parent;
		}

		parent->addChild(child);

		return parent;
	}

	void unhideAll()
	{
		for(auto current : projectItemsByID)
		{
			current.second->setHidden(false);
		}

		for(auto current : fileItemsByID)
		{
			current.second->setHidden(false);
		}
	}

	void showLoadingPage(QString message, bool progress = false)
	{
		loadingLabel->setText(message);

		loadingBar->setVisible(progress);
		loadingBar->setValue(0);
		loadingCancel->setVisible(progress);

		stack->setCurrentIndex(2);
	}

	void setProjectTags()
	{
		if(currentTags.isEmpty())
		{
			projectTags->hide();
		}
		else
		{
			projectTags->show();
			projectTags->setTagList(currentTags);
		}
	}

	void setFileDescription(QString description)
	{
		fileInfoForm->removeRow(fileDescLabel);

		if(!description.isEmpty())
		{
			fileInfoForm->insertRow(1, "Description:", fileDescLabel = new MultiLineLabel(description));
		}

	}

	void setFileTags()
	{
		if(currentFileTags.isEmpty())
		{
			fileTags->hide();
		}
		else
		{
			fileTags->show();
			fileTags->setTagList(currentFileTags);
		}
	}


public:
	ProjectItem* currentProject;
	std::unordered_map<std::string, CustomTreeWidgetItem*> currentProjectFolders;
	QStringList currentTags;
	QStringList currentFileTags;
	std::unordered_map<int, ProjectItem*> projectItemsByID;
	std::unordered_map<int, FileItem*> fileItemsByID;

	CustomTreeWidgetItem* openAfterDownload;
};

#endif