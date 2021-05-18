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

#include "LocalDatabaseHandler.h"
#include <QString>

#ifdef MODEL_REPO
#include <sqlite3.h>
#include <vector>
#include <string>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QStandardPaths>
#include <QDir>
#include <QFile>
//#include <QStringList>
#include <QVariantMap>
#include "RepositoryPanel.h"

#include <iostream>

static int addCategoryCallback(void *dbPanel, int argc, char **argv, char **azColName)
{
	((CRepositoryPanel*) dbPanel)->AddCategory(argv);

	return 0;
}

static int addProjectCallback(void *dbPanel, int argc, char **argv, char **azColName)
{
	((CRepositoryPanel*) dbPanel)->AddProject(argv);

	return 0;
}

static int addProjectFilesCallback(void *dbPanel, int argc, char **argv, char **azColName)
{
	((CRepositoryPanel*) dbPanel)->AddProjectFile(argv);

	return 0;
}

static int setProjectDataCallback(void *dbPanel, int argc, char **argv, char **azColName)
{
	((CRepositoryPanel*) dbPanel)->SetProjectData(argv);

	return 0;
}

static int setFileDataCallback(void *dbPanel, int argc, char **argv, char **azColName)
{
	((CRepositoryPanel*) dbPanel)->SetFileData(argv);

	return 0;
}

static int addCurrentTagCallback(void *dbPanel, int argc, char **argv, char **azColName)
{
	((CRepositoryPanel*) dbPanel)->AddCurrentTag(argv);

	return 0;
}

static int addCurrentFileTagCallback(void *dbPanel, int argc, char **argv, char **azColName)
{
	((CRepositoryPanel*) dbPanel)->AddCurrentFileTag(argv);

	return 0;
}

class CLocalDatabaseHandler::Imp
{
public:
	Imp(CRepositoryPanel* dbPanel)
		: dbPanel(dbPanel), db(NULL)	{}

	void updateDBPath()
	{
		dbPath = dbPanel->GetRepositoryFolder() += "/localdb.db";
	}

	void openDatabase(std::string schema)
	{
		char *zErrMsg = 0;

		// When the repository is refreshed, this is called again, and the current db needs to be closed
		// before it is deleted.
		if(db) sqlite3_close(db);

		// Delete local copy of the model database in order to write a new one.
		QFile::remove(dbPath);

		int rc = sqlite3_open(dbPath.toStdString().c_str(), &db);

		if( rc )
		{
			fprintf(stderr, "Can't open database: %s\nError: %s\n", dbPath.toStdString().c_str(), sqlite3_errmsg(db));
			sqlite3_close(db);
			return;
		}

		rc = sqlite3_exec(db,schema.c_str(), NULL, NULL, &zErrMsg);

		if( rc!=SQLITE_OK )
		{
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		}

	}

	void execute(std::string& query, int (*callback)(void*,int,char**,char**)=NULL, void* arg = NULL)
	{
		int rc;
		char *zErrMsg = 0;

		rc = sqlite3_exec(db, query.c_str(), callback, arg, &zErrMsg);

		if( rc!=SQLITE_OK )
		{
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		}
	}

	void getTable(std::string& query, char ***table, int* rows, int* cols)
	{
		int rc;
		char *zErrMsg = 0;

		rc = sqlite3_get_table(db, query.c_str(), table, rows, cols, &zErrMsg);

		if( rc!=SQLITE_OK )
		{
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		}
	}

	void insert(std::string& tableName, std::vector<std::string>& columns, std::string& values, std::string conflict = "")
		{
			std::string query("INSERT INTO ");
			query += tableName;
			query += "(" + columns[0];
			for(int index = 1; index < columns.size(); index++)
			{
				query += ", " + columns[index];
			}
			query += ") ";
			query += "VALUES " + values;

			execute(query);
		}

	void upsert(std::string& tableName, std::vector<std::string>& columns, std::string& values, std::string conflict = "")
	{
		std::string query("INSERT INTO ");
		query += tableName;
		query += "(" + columns[0];
		for(int index = 1; index < columns.size(); index++)
		{
			query += ", " + columns[index];
		}
		query += ") ";
		query += "VALUES " + values;

		if(conflict.compare("") !=0)
		{
			query += " ON CONFLICT(ID) DO UPDATE SET ";
			query += columns[0] + "=excluded." + columns[0];
			for(int index = 1; index < columns.size(); index++)
			{
				query += ", " + columns[index] + "=excluded." + columns[index];
			}
		}
		query += ";";

		execute(query);
	}

	void checkLocalCopies()
	{
		char **table;
		int rows, cols;

		std::string query = "SELECT ID FROM filenames";
		getTable(query, &table, &rows, &cols);

		std::string hasCopy = "UPDATE filenames set localCopy = 1 WHERE ID IN (";
		std::string noCopy = "UPDATE filenames set localCopy = 0 WHERE ID IN (";

		bool updateHasCopy = false;
		bool updateNoCopy = false;

		for(int row = 1; row < rows + 1; row++)
		{
			QString filename = GetFullFilename(std::stoi(table[row]), 1);


			QFile file(filename);
			if(file.exists())
			{
				hasCopy += table[row];
				hasCopy += ", ";
				updateHasCopy = true;
			}
			else
			{
				noCopy += table[row];
				noCopy += ", ";
				updateNoCopy = true;
			}

		}

		// Remove the last, unnecessary comma and space
		hasCopy.pop_back();
		hasCopy.pop_back();
		noCopy.pop_back();
		noCopy.pop_back();


		hasCopy += ");";
		noCopy += ");";

		sqlite3_free_table(table);

		if(updateHasCopy) execute(hasCopy);
		if(updateNoCopy) execute(noCopy);
	}

	QString ProjectNameFromID(int ID)
	{
		char **table;
		int rows, cols;

		std::string query = "SELECT name FROM projects WHERE ID = " + std::to_string(ID);

		getTable(query, &table, &rows, &cols);

		QString name;
		if(rows == 1)
		{
			name = table[1];
		}

		sqlite3_free_table(table);

		return name;
	}

	QString GetFilePath(int ID, int type)
	{
		char **table;
		int rows, cols;

		std::string query("");
		QString path;

		// If it's an entire project file
		if(type == FULL)
		{
			query += "SELECT categories.category FROM projects JOIN categories ON projects.category = categories.ID WHERE projects.ID = ";
			query += std::to_string(ID);

			getTable(query, &table, &rows, &cols);

			if(rows == 1)
			{
				path += table[1];
			}
		}
		// If it's a single file from a project
		else
		{
			query += "SELECT categories.category, projects.name FROM projects JOIN categories ON projects.category = categories.ID JOIN filenames ON projects.ID = filenames.project WHERE filenames.ID = ";
			query += std::to_string(ID);

			getTable(query, &table, &rows, &cols);

			if(rows == 1)
			{
				path += table[2];
				path += "/";
				path += table[3];
			}
		}

		sqlite3_free_table(table);

		return path;
	}

	QString GetFileName(int ID, int type)
	{
		char **table;
		int rows, cols;

		std::string query;
		QString filename;

		// If it's an entire project file
		if(type == FULL)
		{
			query += "SELECT name FROM projects WHERE ID = ";
			query += std::to_string(ID);
		}
		// If it's a single file from a project
		else
		{
			query += "SELECT filename FROM filenames WHERE ID = ";
			query += std::to_string(ID);

		}

		getTable(query, &table, &rows, &cols);

		if(rows == 1)
		{
			filename += table[1];

			if(type == FULL) filename += ".prj";
		}

		sqlite3_free_table(table);

		return filename;
	}

	QString GetFullFilename(int ID, int type)
	{
		QString filename = dbPanel->GetRepositoryFolder();
		filename += "/";
		filename += GetFilePath(ID, type);
		filename += "/";
		filename += GetFileName(ID, type);

		return filename;
	}

	QString GetFullPath(int ID, int type)
		{
			QString filename = dbPanel->GetRepositoryFolder();
			filename += "/";
			filename += GetFilePath(ID, type);
			filename += "/";

			return filename;
		}

	QString CategoryFromID(int ID)
	{
		char **table;
		int rows, cols;

		std::string query("SELECT categories.category FROM projects JOIN categories ON projects.category = categories.ID WHERE projects.ID = ");
		query += std::to_string(ID);

		getTable(query, &table, &rows, &cols);

		QString category;
		if(rows == 1)
		{
			category = table[1];
		}

		sqlite3_free_table(table);

		return category;
	}

	int ProjectIDFromFileID(int ID)
	{
		char **table;
		int rows, cols;

		std::string query("SELECT project FROM filenames WHERE ID = ");
		query += std::to_string(ID);

		getTable(query, &table, &rows, &cols);

		int projID = 0;
		if(rows == 1)
		{
			projID = std::stoi(table[1]);
		}

		sqlite3_free_table(table);

		return projID;
	}

	int CategoryIDFromName(std::string name)
	{
		char **table;
		int rows, cols;

		std::string query("SELECT ID FROM categories WHERE category = '");
		query += name;
		query += "'";

		getTable(query, &table, &rows, &cols);

		int catID = 1;
		if(rows == 1)
		{
			catID = std::stoi(table[1]);
		}

		sqlite3_free_table(table);

		return catID;
	}

	bool isValidUpload(QString& projectName, QString& category)
	{
		char **table;
		int rows, cols;

		std::string query = QString("SELECT projects.ID FROM projects JOIN categories ON "
				"projects.category = categories.ID WHERE projects.name = '%1' "
				"AND categories.category = '%3'").arg(projectName).arg(category).toStdString();

		getTable(query, &table, &rows, &cols);

		sqlite3_free_table(table);

		return rows == 0;
	}

	qint64 currentProjectsSize(QString& username)
	{
		char **table;
		int rows, cols;

		std::string query = QString("SELECT filenames.size FROM filenames JOIN projects ON filenames.project = projects.ID "
				"JOIN users ON users.ID = projects.owner WHERE users.username = '%1'").arg(username).toStdString();

		getTable(query, &table, &rows, &cols);

		qint64 totalSize = 0;
		for(int row = 1; row < rows + 1; row++)
		{
			totalSize += QString(table[row]).toLongLong();
		}

		sqlite3_free_table(table);

		return totalSize;
	}

	qint64 projectSize(int ID)
	{
		char **table;
		int rows, cols;

		std::string query = QString("SELECT size FROM filenames WHERE project = '%1'").arg(ID).toStdString();

		getTable(query, &table, &rows, &cols);

		qint64 totalSize = 0;
		for(int row = 1; row < rows + 1; row++)
		{
			totalSize += QString(table[row]).toLongLong();
		}

		sqlite3_free_table(table);

		return totalSize;
	}

public:
	sqlite3* db;
	CRepositoryPanel* dbPanel;
	QString dbPath;
};

CLocalDatabaseHandler::CLocalDatabaseHandler(CRepositoryPanel* dbPanel)
{
	imp = new Imp(dbPanel);
}

CLocalDatabaseHandler::~CLocalDatabaseHandler(){}

void CLocalDatabaseHandler::init(std::string schema)
{
	imp->updateDBPath();

	imp->openDatabase(schema);
}

void CLocalDatabaseHandler::update(QJsonDocument& jsonDoc)
{
	QJsonArray jsonProjects = jsonDoc.array();
	for(QJsonValueRef jsonProject : jsonProjects)
	{
		QJsonObject projectObj = jsonProject.toObject();
		std::string name = projectObj.value("name").toString().toStdString();

		std::string conflict("");
		std::vector<std::string> columnNames;
		QJsonArray columns = projectObj.value("columns").toArray();
		for(QJsonValueRef column : columns)
		{
			std::string columnName = column.toString().toStdString();
			if(columnName.compare("ID") == 0) conflict = "ID";
			columnNames.push_back(column.toString().toStdString());
		}

		if(columnNames.size() == 0) continue;

		std::string values = projectObj.value("values").toString().toStdString();

		imp->insert(name, columnNames, values, conflict);
	}

	imp->checkLocalCopies();
}

void CLocalDatabaseHandler::GetCategories()
{
	std::string query("SELECT category FROM categories");

	imp->execute(query, addCategoryCallback, imp->dbPanel);
}

void CLocalDatabaseHandler::GetProjects()
{
	std::string query("SELECT projects.ID, projects.name, users.username, categories.category, projects.authorized FROM projects JOIN categories ON projects.category = categories.ID JOIN users ON projects.owner = users.ID");

	imp->execute(query, addProjectCallback, imp->dbPanel);
}

QStringList CLocalDatabaseHandler::GetTags()
{
	char **table;
	int rows, cols;

	QStringList tags;

	// Matches owners, names, and descriptions of projects
	QString query = QString("SELECT tag FROM tags;");
	std::string queryStd = query.toStdString();

	imp->getTable(queryStd, &table, &rows, &cols);

	for(int row = 1; row <= rows; row++)
	{
		tags.append(table[row]);
	}

	sqlite3_free_table(table);

	return tags;
}

void CLocalDatabaseHandler::GetProjectFiles(int ID)
{
	std::string query("SELECT ID, filename, localCopy, size from filenames where project = ");
	query += std::to_string(ID);

	imp->execute(query, addProjectFilesCallback, imp->dbPanel);
}

void CLocalDatabaseHandler::GetProjectData(int ID)
{
	std::string query("SELECT name, description, username FROM projects JOIN users on users.id = projects.owner WHERE projects.id = ");
	query += std::to_string(ID);

	imp->execute(query, setProjectDataCallback, imp->dbPanel);
}

void CLocalDatabaseHandler::GetFileData(int ID)
{
	std::string query("SELECT filename, description FROM filenames WHERE id = ");
	query += std::to_string(ID);

	imp->execute(query, setFileDataCallback, imp->dbPanel);
}

void CLocalDatabaseHandler::GetFileTags(int ID)
{
	std::string query("SELECT tags.tag FROM tags JOIN fileTags on tags.id = fileTags.tag WHERE fileTags.file = ");
	query += std::to_string(ID);

	imp->execute(query, addCurrentFileTagCallback, imp->dbPanel);
}

void CLocalDatabaseHandler::GetCategoryMap(std::map<int, std::string>& categoryMap)
{
	char **table;
	int rows, cols;

	std::string query = "SELECT * FROM categories";

	imp->getTable(query, &table, &rows, &cols);

	// Extract information about each project
	for(int row = 1; row <= rows; row++)
	{
		int rowStart = row*cols;

		categoryMap[std::stoi(table[rowStart])] = std::string(table[rowStart + 1]);
	}

	sqlite3_free_table(table);
}

QList<QList<QVariant>> CLocalDatabaseHandler::GetProjectFileInfo(int projID)
{
	QList<QList<QVariant>> fileInfo;

	char **table;
	int rows, cols;

	QString query = QString("SELECT ID, filename, description, size FROM filenames WHERE project = %1").arg(projID);
	std::string queryStd = query.toStdString();

	imp->getTable(queryStd, &table, &rows, &cols);

	for(int row = 1; row <= rows; row++)
	{
		QList<QVariant> currentInfo;

		int rowStart = row*cols;

		currentInfo.push_back(QString(table[rowStart + 1]));
		currentInfo.push_back(QString(table[rowStart + 2]));
		currentInfo.push_back(QString(table[rowStart + 3]).toLongLong());

		char **table2;
		int rows2, cols2;

		int fileID = std::stoi(table[rowStart]);
		QString query2 = QString("SELECT tags.tag FROM fileTags JOIN tags ON fileTags.tag = tags.ID WHERE fileTags.file = %1").arg(fileID);
		std::string queryStd2 = query2.toStdString();

		imp->getTable(queryStd2, &table2, &rows2, &cols2);

		QStringList tags;
		for(int row2 = 1; row2 <= rows2; row2++)
		{
			tags.push_back(QString(table2[row2]));
		}

		currentInfo.push_back(tags);

		sqlite3_free_table(table2);

		fileInfo.push_back(currentInfo);
	}

	sqlite3_free_table(table);


	return fileInfo;
}

void CLocalDatabaseHandler::GetProjectTags(int ID)
{
	std::string query("SELECT tags.tag FROM tags JOIN projectTags on tags.id = projectTags.tag WHERE projectTags.project = ");
	query += std::to_string(ID);

	imp->execute(query, addCurrentTagCallback, imp->dbPanel);
}

void CLocalDatabaseHandler::GetProjectPubs(int ID)
{
	char **table;
	int rows, cols;

	QVariantMap data;

	// Get all publications for this project
	QString query = QString("SELECT publications.ID, title, year, journal, volume, issue, pages, DOI FROM publications JOIN projectPubs ON publications.ID = projectPubs.publication WHERE projectPubs.project = %1").arg(ID);
	std::string queryStd = query.toStdString();

	imp->getTable(queryStd, &table, &rows, &cols);

	// Extract information about each project
	for(int row = 1; row <= rows; row++)
	{
		int rowStart = row*cols;

		data["title"] = QString(table[rowStart + 1]);
		data["year"] = QString(table[rowStart + 2]);
		data["journal"] = QString(table[rowStart + 3]);
		data["volume"] = QString(table[rowStart + 4]);
		data["issue"] = QString(table[rowStart + 5]);
		data["pages"] = QString(table[rowStart + 6]);
		data["DOI"] = QString(table[rowStart + 7]);

		// Get Authors for this publication
		char **table2;
		int rows2, cols2;

		QString query2 = QString("SELECT firstName, lastName FROM authors JOIN publicationAuthors ON publicationAuthors.author = authors.ID WHERE publicationAuthors.publication = %1 ORDER BY publicationAuthors.ordering").arg(table[rowStart]);
		std::string queryStd2 = query2.toStdString();

		imp->getTable(queryStd2, &table2, &rows2, &cols2);

		QStringList authorGiven;
		QStringList authorFamily;

		for(int row2 = 1; row2 <= rows2; row2++)
		{
			int rowStart2 = row2*cols2;

			authorGiven.push_back(table2[rowStart2]);
			authorFamily.push_back(table2[rowStart2 + 1]);

		}

		sqlite3_free_table(table2);

		data["authorGiven"] = authorGiven;
		data["authorFamily"] = authorFamily;

		imp->dbPanel->AddPublication(data);
	}


	sqlite3_free_table(table);

}

std::unordered_set<int> CLocalDatabaseHandler::FullTextSearch(QString term)
{
	if(term.isEmpty()) return std::unordered_set<int>();

	char **table;
	int rows, cols;

	std::unordered_set<int> projects;

	// Matches owners, names, and descriptions of projects
	QString query = QString("SELECT projects.ID FROM projects JOIN users ON projects.owner=users.ID WHERE username LIKE '%%1%' OR name LIKE '%%1%' OR description LIKE '%%1%'").arg(term);
	std::string queryStd = query.toStdString();

	imp->getTable(queryStd, &table, &rows, &cols);

	for(int row = 1; row <= rows; row++)
	{
		projects.insert(std::stoi(table[row]));
	}

	sqlite3_free_table(table);

	// Matches filenames and descriptions
	query = QString("SELECT project FROM filenames WHERE filename LIKE '%%1%' OR description LIKE '%%1%'").arg(term);
	queryStd = query.toStdString();

	imp->getTable(queryStd, &table, &rows, &cols);

	for(int row = 1; row <= rows; row++)
	{
		projects.insert(std::stoi(table[row]));
	}

	sqlite3_free_table(table);

	// Matches project tags
	query = QString("SELECT projectTags.project FROM tags JOIN projectTags ON tags.ID = projectTags.tag WHERE tags.tag LIKE '%%1%'").arg(term);
	queryStd = query.toStdString();

	imp->getTable(queryStd, &table, &rows, &cols);

	for(int row = 1; row <= rows; row++)
	{
		projects.insert(std::stoi(table[row]));
	}

	sqlite3_free_table(table);

	// Matches file tags
//	query = QString("SELECT filenames.project FROM tags JOIN fileTags ON tags.ID = fileTags.tag JOIN filenames ON fileNames.ID = fileTags.file WHERE tags.tag LIKE '%%1%'").arg(term);
//	queryStd = query.toStdString();
//
//	imp->getTable(queryStd, &table, &rows, &cols);
//
//	for(int row = 1; row <= rows; row++)
//	{
//		projects.insert(stoi(table[row]));
//	}
//
//	sqlite3_free_table(table);

	return projects;
}

std::unordered_set<int> CLocalDatabaseHandler::FileSearch(QString term)
{
	char **table;
	int rows, cols;

	std::unordered_set<int> files;

	// Matches filenames and descriptions
	QString query = QString("SELECT ID FROM filenames WHERE filename LIKE '%%1%' OR description LIKE '%%1%'").arg(term);
	std::string queryStd = query.toStdString();

	imp->getTable(queryStd, &table, &rows, &cols);

	for(int row = 1; row <= rows; row++)
	{
		files.insert(std::stoi(table[row]));
	}

	sqlite3_free_table(table);

	// Matches file tags
	query = QString("SELECT fileTags.file FROM tags JOIN fileTags ON tags.ID = fileTags.tag WHERE tags.tag LIKE '%%1%'").arg(term);
	queryStd = query.toStdString();

	imp->getTable(queryStd, &table, &rows, &cols);

	for(int row = 1; row <= rows; row++)
	{
		files.insert(std::stoi(table[row]));
	}

	sqlite3_free_table(table);

	return files;
}

QString CLocalDatabaseHandler::ProjectNameFromID(int ID)
{
	return imp->ProjectNameFromID(ID);
}

QString CLocalDatabaseHandler::FilePathFromID(int ID, int type)
{
	return imp->GetFilePath(ID, type);
}

QString CLocalDatabaseHandler::FileNameFromID(int ID, int type)
{
	return imp->GetFileName(ID, type);
}

QString CLocalDatabaseHandler::FullFileNameFromID(int ID, int type)
{
	return imp->GetFullFilename(ID, type);
}

QString CLocalDatabaseHandler::CategoryFromID(int ID)
{
	return imp->CategoryFromID(ID);
}

int CLocalDatabaseHandler::ProjectIDFromFileID(int ID)
{
	return imp->ProjectIDFromFileID(ID);
}

int CLocalDatabaseHandler::CategoryIDFromName(std::string name)
{
	return imp->CategoryIDFromName(name);
}

bool CLocalDatabaseHandler::isValidUpload(QString& projectName, QString& category)
{
	return imp->isValidUpload(projectName, category);
}

qint64 CLocalDatabaseHandler::currentProjectsSize(QString username)
{
	return imp->currentProjectsSize(username);
}

qint64 CLocalDatabaseHandler::projectsSize(int ID)
{
	return imp->projectSize(ID);
}



#else

CLocalDatabaseHandler::CLocalDatabaseHandler(CRepositoryPanel* dbPanel) {}
CLocalDatabaseHandler::~CLocalDatabaseHandler(){}
void CLocalDatabaseHandler::update(QJsonDocument& jsonDoc){}
bool CLocalDatabaseHandler::isValidUpload(QString& projectName, QString& category) { return false;  }
qint64 CLocalDatabaseHandler::currentProjectsSize(QString username) { return 0; }
QString CLocalDatabaseHandler::ProjectNameFromID(int ID) { return ""; }
qint64 CLocalDatabaseHandler::projectsSize(int ID) { return 0; }
#endif

