#include "taskdialog.h"
#include "ui_taskdialog.h"

#include <QDate>

TaskDialog::TaskDialog(QVector<Status> status, QVector<Priority> priorities, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TaskDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("New task");
    this->status = status;
    this->priorities = priorities;

    foreach (Status s, this->status)
    {
        ui->statusCombo->addItem(s.getName());
    }

    foreach (Priority p, this->priorities)
    {
        ui->priorityCombo->addItem(p.getName());
    }

    QDate expectedFor = QDate::currentDate();
    expectedFor = expectedFor.addDays(10);
    ui->expectedForEdit->setDate(expectedFor);
}

TaskDialog::TaskDialog(Task *t, QVector<Status> status, QVector<Priority> priorities, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TaskDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Edit task");
    this->status = status;
    this->priorities = priorities;

    foreach (Status s, this->status)
    {
        ui->statusCombo->addItem(s.getName());
    }

    foreach (Priority p, this->priorities)
    {
        ui->priorityCombo->addItem(p.getName());
    }

    // set fields
    ui->nameEdit->setText(t->getTitle());
    ui->descriptionEdit->setMarkdown(t->getDescription());
    ui->expectedForEdit->setDate(t->getExpectedFor());

    if (t->getPriorityUUID().length() > 0)
    {
        int refindex = -1;
        for (int i = 0; i < this->priorities.count(); i++)
        {
            if (this->priorities[i].getUUID() == t->getPriorityUUID())
            {
                refindex = i;
            }
        }
        if (refindex > -1)
        {
            ui->priorityCombo->setCurrentIndex(refindex);
        }
    }

    if (t->getStatusUUID().length() > 0)
    {
        int refindex = -1;
        for (int i = 0; i < this->status.count(); i++)
        {
            if (this->status[i].getUUID() == t->getStatusUUID())
            {
                refindex = i;
            }
        }
        if (refindex > -1)
        {
            ui->statusCombo->setCurrentIndex(refindex);
        }
    }

}

TaskDialog::~TaskDialog()
{
    delete ui;
}

Task TaskDialog::getTask()
{
    QString title = ui->nameEdit->text();
    QString description = ui->descriptionEdit->toMarkdown(QTextDocument::MarkdownFeature::MarkdownDialectCommonMark);
    QDate expectedFor = ui->expectedForEdit->date();
    QString priorityUUID = "";
    if (ui->priorityCombo->currentIndex() > -1)
    {
        Priority priority = priorities[ui->priorityCombo->currentIndex()];
        priorityUUID = priority.getUUID();
    }
    QString statusUUID = "";
    if (ui->statusCombo->currentIndex() > -1)
    {
        Status s = status[ui->statusCombo->currentIndex()];
        statusUUID = s.getUUID();
    }
    return Task(title, description, expectedFor, priorityUUID, statusUUID);
}
