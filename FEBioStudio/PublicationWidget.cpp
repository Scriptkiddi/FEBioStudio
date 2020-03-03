#include <vector>
#include <QWidget>
#include <QResizeEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QRect>
#include <QVBoxLayout>
#include <QString>
#include <QStringList>
#include <QLabel>
#include <QToolButton>
#include <QCheckBox>
#include <QAction>
#include "PublicationWidget.h"

#include <iostream>

class WrapLabel : public QWidget
{
public:
	WrapLabel(QString text, QWidget* parent = nullptr)
		: QWidget(parent), processEvent(true)
	{
		words = text.split(" ");

		QLabel temp;
		for(QString word : words)
		{
			lengths.push_back(temp.fontMetrics().horizontalAdvance(word));
		}

		spaceSize = temp.fontMetrics().width(" ");

		layout = new QVBoxLayout;
		layout->setContentsMargins(0, 0, 0, 0);
		setLayout(layout);

		setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Maximum);

		setContentsMargins(0, 0, 0, 0);

	}

protected:
	void resizeEvent(QResizeEvent *event) override
	{

		if(event->oldSize().width() != event->size().width())
		{

			for(auto label : labels)
			{
				delete label;
			}

			labels.clear();

			int width = event->size().width();

			int index, currentWidth = 0;
			QString currentString;
			for(index = 0; index < lengths.size(); index++)
			{


				if(currentWidth + lengths[index] > width)
				{
					QLabel* next = new QLabel(currentString);
					next->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
					layout->addWidget(next);
					labels.push_back(next);

					currentString.clear();
					currentWidth = 0;

				}

				currentString += words[index] + " ";

				currentWidth += lengths[index] + spaceSize;

			}

			QLabel* next = new QLabel(currentString);
			next->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
			layout->addWidget(next);
			labels.push_back(next);
		}

		QWidget::resizeEvent(event);
	}

private:
	QVBoxLayout* layout;

	QStringList words;
	std::vector<int> lengths;
	std::vector<QLabel*> labels;
	int spaceSize;
	bool processEvent;

};

class Ui::CPublicationWidget
{
public:
	QWidget* shortWidget;
	QAction* expand;
	QLabel* shortLabel;
	QCheckBox* shortCheckBox;

	QWidget* fullWidget;
	QAction* shrink;
	WrapLabel* fullLabel;
	QLabel* DOILabel;
	QCheckBox* fullCheckBox;

	QAction* select;

public:

	void setup(::CPublicationWidget* parent)
	{
		QVBoxLayout* layout = new QVBoxLayout;
		layout->setContentsMargins(3, 0, 3, 0);
		layout->setSizeConstraint(QLayout::SetMinimumSize);

		QAction* select = new QAction(parent);
		select->setIcon(QIcon(":/icons/check.png"));
		select->setObjectName("select");

		if(parent->isExpandable())
		{
			shortWidget = new QWidget;
			QHBoxLayout* shortLayout = new QHBoxLayout;

			shortLabel = new QLabel(parent->ShortText());
			shortLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
			shortLayout->addWidget(shortLabel);

			expand = new QAction(parent);
			expand->setIcon(QIcon(":/icons/selectAdd.png"));
			expand->setObjectName("expand");
			QToolButton* expandBtn = new QToolButton;
			expandBtn->setDefaultAction(expand);
			shortLayout->addWidget(expandBtn);

			if(parent->getSelection() == ::CPublicationWidget::CHECKBOX)
			{
				shortCheckBox = new QCheckBox(parent);
				shortCheckBox->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);

				QObject::connect(shortCheckBox, &QCheckBox::stateChanged, parent, &::CPublicationWidget::checkBox_stateChanged);

				shortLayout->addWidget(shortCheckBox);
			}
			else if(parent->getSelection() == ::CPublicationWidget::BUTTON)
			{
				QToolButton* shortSelectBtn = new QToolButton;
				shortSelectBtn->setDefaultAction(select);

				shortLayout->addWidget(shortSelectBtn);
			}

			shortWidget->setLayout(shortLayout);
			layout->addWidget(shortWidget);
		}

		fullWidget = new QWidget;
		if(parent->isExpandable())
		{
			fullWidget->setHidden(true);
		}
		QVBoxLayout* fullLayout = new QVBoxLayout;
		QHBoxLayout* hlayout = new QHBoxLayout;

		fullLabel = new WrapLabel(parent->FullText());

		hlayout->addWidget(fullLabel);

		QVBoxLayout* fullVBLayout = new QVBoxLayout;
		fullVBLayout->setAlignment(Qt::AlignTop);
		hlayout->addLayout(fullVBLayout);

		if(parent->isExpandable())
		{
			shrink = new QAction(parent);
			shrink->setIcon(QIcon(":/icons/selectSub.png"));
			shrink->setObjectName("shrink");
			QToolButton* shrinkBtn = new QToolButton;
			shrinkBtn->setDefaultAction(shrink);

			fullVBLayout->addWidget(shrinkBtn);
		}

		if(parent->getSelection() == ::CPublicationWidget::CHECKBOX)
		{
			fullCheckBox = new QCheckBox(parent);
			fullCheckBox->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);

			if(parent->isExpandable())
			{
				QObject::connect(fullCheckBox, &QCheckBox::stateChanged, parent, &::CPublicationWidget::checkBox_stateChanged);
			}

			fullVBLayout->addWidget(fullCheckBox);

		}
		else if(parent->getSelection() == ::CPublicationWidget::BUTTON)
		{
			QToolButton* fullSelectBtn = new QToolButton;
			fullSelectBtn->setDefaultAction(select);

			fullVBLayout->addWidget(fullSelectBtn);
		}

		fullLayout->addLayout(hlayout);

		DOILabel = new QLabel(QString("DOI: <a href=\"https://doi.org/%1\">%1</a>").arg(parent->getDOI()));
		DOILabel->setOpenExternalLinks(true);
		fullLayout->addWidget(DOILabel);

		fullWidget->setLayout(fullLayout);
		layout->addWidget(fullWidget);

		parent->setLayout(layout);
		parent->setFrameStyle(QFrame::Box);
		parent->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
	}
};

CPublicationWidget::CPublicationWidget(Selection selection, bool expandable) : QFrame(), selection(selection), expandable(expandable), ui(new Ui::CPublicationWidget)
{

}

CPublicationWidget::CPublicationWidget(QVariantMap& data, Selection selection, bool expandable)
	: QFrame(), title(data["title"].toString()), year(data["year"].toString()), journal(data["journal"].toString()), volume(data["volume"].toString()),
	  issue(data["issue"].toString()), pages(data["pages"].toString()), DOI(data["DOI"].toString()),
	  authorGiven(data["authorGiven"].toStringList()), authorFamily(data["authorFamily"].toStringList()), selection(selection), expandable(expandable), ui(new Ui::CPublicationWidget)
{
	ui->setup(this);

	QMetaObject::connectSlotsByName(this);
}

CPublicationWidget::CPublicationWidget(QString title, QString year, QString journal, QString volume,
		QString issue, QString pages, QString DOI, QStringList authorGiven, QStringList authorFamily,
		Selection selection, bool expandable) : QFrame(), title(title), year(year), journal(journal),
			volume(volume), issue(issue), pages(pages), DOI(DOI), authorGiven(authorGiven),
			authorFamily(authorFamily), selection(selection), expandable(expandable),
			ui(new Ui::CPublicationWidget)
{
	ui->setup(this);

	QMetaObject::connectSlotsByName(this);
}

void CPublicationWidget::init()
{
	ui->setup(this);

	QMetaObject::connectSlotsByName(this);
}


void CPublicationWidget::on_expand_triggered()
{
	ui->fullWidget->setHidden(false);
	ui->shortWidget->setHidden(true);
}


void CPublicationWidget::on_shrink_triggered()
{
	ui->fullWidget->setHidden(true);
	ui->shortWidget->setHidden(false);
}

bool CPublicationWidget::isExpandable() const
{
	return expandable;
}

void CPublicationWidget::on_select_triggered()
{
	emit chosen_publication(this);
}

void CPublicationWidget::checkBox_stateChanged(int state)
{
	ui->shortCheckBox->blockSignals(true);
	ui->fullCheckBox->blockSignals(true);

	ui->shortCheckBox->setChecked(state);
	ui->fullCheckBox->setChecked(state);

	ui->shortCheckBox->blockSignals(false);
	ui->fullCheckBox->blockSignals(false);
}

QString CPublicationWidget::ShortText()
{
	if(authorFamily.length() == 0)
	{
		authorFamily.push_back("");
		authorGiven.push_back("");
	}

	return QString("%2 (%3). %1").arg(title).arg(authorFamily[0]).arg(year);
}

QString CPublicationWidget::FullText()
{
	QString authorString;
	for(int index = 0; index < authorGiven.count(); index++)
	{
		authorString += authorFamily.at(index) + ", ";
		authorString += authorGiven.at(index);

		if(index == authorGiven.count() - 1)
		{

		}
		else if(index == authorGiven.count() - 2)
		{
			authorString += " & ";
		}
		else
		{
			authorString += ", ";
		}
	}

	return QString("%1 (%2). %3. %4, %5(%6), %7.").arg(authorString).arg(year).arg(title).arg(journal).arg(volume).arg(issue).arg(pages);
}

const QStringList& CPublicationWidget::getAuthorFamily() const {
	return authorFamily;
}

void CPublicationWidget::setAuthorFamily(const QStringList &authorFamily) {
	this->authorFamily = authorFamily;
}

const QStringList& CPublicationWidget::getAuthorGiven() const {
	return authorGiven;
}

void CPublicationWidget::setAuthorGiven(const QStringList &authorGiven) {
	this->authorGiven = authorGiven;
}

const QString& CPublicationWidget::getDOI() const {
	return DOI;
}

void CPublicationWidget::setDOI(const QString &doi) {
	DOI = doi;
}

const QString& CPublicationWidget::getIssue() const {
	return issue;
}

void CPublicationWidget::setIssue(const QString &issue) {
	this->issue = issue;
}

const QString& CPublicationWidget::getJournal() const {
	return journal;
}

void CPublicationWidget::setJournal(const QString &journal) {
	this->journal = journal;
}

const QString& CPublicationWidget::getPages() const {
	return pages;
}

void CPublicationWidget::setPages(const QString &pages) {
	this->pages = pages;
}

const QString& CPublicationWidget::getTitle() const {
	return title;
}

void CPublicationWidget::setTitle(const QString &title) {
	this->title = title;
}

const QString& CPublicationWidget::getVolume() const {
	return volume;
}

void CPublicationWidget::setVolume(const QString &volume) {
	this->volume = volume;
}

const QString& CPublicationWidget::getYear() const {
	return year;
}

void CPublicationWidget::setYear(const QString &year) {
	this->year = year;
}

int CPublicationWidget::getSelection() const
{
	return selection;
}

bool CPublicationWidget::isChecked() const
{
	if(ui->fullCheckBox)
	{
		return ui->fullCheckBox->isChecked();
	}

	return false;
}


