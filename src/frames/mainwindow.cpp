#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QUuid>
#include <QColor>

#include "prefdialog.h"
#include "aboutdialog.h"
#include "namedialog.h"
#include "taskdialog.h"
#include "../tools.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionPreferences, &QAction::triggered, this, &MainWindow::openPreferences);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::openAbout);
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::onNewBoardClick);
    connect(ui->listWidget, &QListWidget::currentRowChanged, this, &MainWindow::onBoardSelected);
    connect(ui->actionNew_task, &QAction::triggered, this, &MainWindow::onNewTaskClick);
    connect(ui->treeWidget, &QTreeWidget::itemDoubleClicked, this, &MainWindow::onEditTask);
    this->priorities = defaultPriorities();
    this->status = defaultStatus();
}

MainWindow::~MainWindow()
{
    for (int i = 0; i < boards.count(); i++)
    {
        Board *b = boards.takeAt(i);
        delete b;
    }
    delete ui;
}

void MainWindow::openPreferences()
{
    PrefDialog dialog(this);
    if (dialog.exec() == QDialog::DialogCode::Accepted)
    {
        this->priorities = dialog.getPriorities();
        this->status = dialog.getStatus();
    }
}

void MainWindow::openAbout()
{
    AboutDialog dialog(this);
    dialog.exec();
}

void MainWindow::onNewBoardClick()
{
    NameDialog dialog("Create a board", "New empty board", this);
    if (dialog.exec() == QDialog::DialogCode::Accepted)
    {
        QString name = dialog.getChoosenName();
        Board *b = new Board(name);
        boards.append(b);
        QListWidgetItem *item = new QListWidgetItem(name);
        ui->listWidget->addItem(item);
    }
}

void MainWindow::onNewTaskClick()
{
    if (selectedBoardIndex > -1)
    {
        TaskDialog dialog(status, priorities, this);
        if (dialog.exec() == QDialog::DialogCode::Accepted)
        {
            Task t = dialog.getTask();
            Board *b = boards[selectedBoardIndex];
            b->add(t);
            QTreeWidgetItem *item = new QTreeWidgetItem();
            item->setText(0, t.getTitle());
            item->setText(1, getStatusLabel(t.getStatusUUID()));
            item->setText(2, getPriorityLabel(t.getPriorityUUID()));
            item->setText(3, t.getExpectedFor().toString());

            QBrush bgColor = item->background(1);
            QBrush fgColor = item->foreground(1);
            bgColor.setColor(getStatusColor(t.getStatusUUID(), bgColor.color()));
            bgColor.setStyle(Qt::BrushStyle::SolidPattern);
            fgColor.setColor(Tools::getForegroundColor(bgColor.color()));
            item->setBackground(1, bgColor);
            item->setForeground(1, fgColor);

            bgColor = item->background(2);
            fgColor = item->foreground(2);
            bgColor.setColor(getPriorityColor(t.getPriorityUUID(), bgColor.color()));
            bgColor.setStyle(Qt::BrushStyle::SolidPattern);
            fgColor.setColor(Tools::getForegroundColor(bgColor.color()));
            item->setBackground(2, bgColor);
            item->setForeground(2, fgColor);

            ui->treeWidget->addTopLevelItem(item);
        }
    }
}

void MainWindow::onBoardSelected(int i)
{
    selectedBoardIndex = i;
    if (selectedBoardIndex > -1)
    {
        Board *b = boards[selectedBoardIndex];
        ui->label->setText(b->getName());
        ui->actionNew_task->setDisabled(false);
    }
    else
    {
        ui->label->setText("<- Select a board");
        ui->actionNew_task->setDisabled(true);
    }
    redrawTaskTree();
}

void MainWindow::onEditTask(QTreeWidgetItem *item)
{
    if (item != nullptr && selectedBoardIndex > -1)
    {
        Board *b = boards[selectedBoardIndex];
        int row = ui->treeWidget->indexOfTopLevelItem(item);
        Task *t = b->taskAt(row);
        if (t != nullptr)
        {
            TaskDialog dialog(t, status, priorities, this);
            if (dialog.exec() == QDialog::DialogCode::Accepted)
            {
                Task editedTask = dialog.getTask();
                t->update(editedTask);
                item->setText(0, editedTask.getTitle());
                item->setText(1, getStatusLabel(editedTask.getStatusUUID()));
                item->setText(2, getPriorityLabel(editedTask.getPriorityUUID()));
                item->setText(3, editedTask.getExpectedFor().toString());

                QBrush bgColor = item->background(1);
                QBrush fgColor = item->foreground(1);
                bgColor.setColor(getStatusColor(editedTask.getStatusUUID(), bgColor.color()));
                bgColor.setStyle(Qt::BrushStyle::SolidPattern);
                fgColor.setColor(Tools::getForegroundColor(bgColor.color()));
                item->setBackground(1, bgColor);
                item->setForeground(1, fgColor);

                bgColor = item->background(2);
                fgColor = item->foreground(2);
                bgColor.setColor(getPriorityColor(editedTask.getPriorityUUID(), bgColor.color()));
                bgColor.setStyle(Qt::BrushStyle::SolidPattern);
                fgColor.setColor(Tools::getForegroundColor(bgColor.color()));
                item->setBackground(2, bgColor);
                item->setForeground(2, fgColor);
            }
        }
    }
}

QVector<Priority> MainWindow::defaultPriorities()
{
    QVector<Priority> res;
    res.append(Priority(QUuid::createUuid().toString(QUuid::WithoutBraces), "Low", QColor::fromString("#309db0")));
    res.append(Priority(QUuid::createUuid().toString(QUuid::WithoutBraces), "Medium", QColor::fromString("#b08e30")));
    res.append(Priority(QUuid::createUuid().toString(QUuid::WithoutBraces), "High", QColor::fromString("#b04330")));
    return res;
}

QVector<Status> MainWindow::defaultStatus()
{
    QVector<Status> res;
    res.append(Status(QUuid::createUuid().toString(QUuid::WithoutBraces), "To Do", QColor::fromString("#8f8f8f")));
    res.append(Status(QUuid::createUuid().toString(QUuid::WithoutBraces), "Working on", QColor::fromString("#5f30b0")));
    res.append(Status(QUuid::createUuid().toString(QUuid::WithoutBraces), "Completed", QColor::fromString("#30b049")));
    return res;
}

const QString MainWindow::getPriorityLabel(QString uuid)
{
    QString res = "";
    foreach (Priority p, priorities)
    {
        if (p.getUUID() == uuid)
        {
            res = p.getName();
        }
    }
    return res;
}

const QString MainWindow::getStatusLabel(QString uuid)
{
    QString res = "";
    foreach (Status s, status)
    {
        if (s.getUUID() == uuid)
        {
            res = s.getName();
        }
    }
    return res;
}

const QColor MainWindow::getPriorityColor(QString uuid, QColor defaultColor)
{
    QColor color = defaultColor;
    foreach (Priority p, priorities)
    {
        if (p.getUUID() == uuid)
        {
            color = p.getColor();
        }
    }
    return color;
}

const QColor MainWindow::getStatusColor(QString uuid, QColor defaultColor)
{
    QColor color = defaultColor;
    foreach (Status s, status)
    {
        if (s.getUUID() == uuid)
        {
            color = s.getColor();
        }
    }
    return color;
}

void MainWindow::redrawBoardList()
{
    QListWidget *l = ui->listWidget;
    for (int i = 0; i < l->count(); i++)
    {
        delete l->takeItem(i);
    }
    foreach (Board *b, boards)
    {
        QListWidgetItem *item = new QListWidgetItem(b->getName());
        l->addItem(item);
    }
}

void MainWindow::redrawTaskTree()
{
    QTreeWidget *l = ui->treeWidget;
    for (int i = 0; i < l->topLevelItemCount(); i++)
    {
        delete l->takeTopLevelItem(i);
    }
    if (selectedBoardIndex > -1)
    {
        Board *b = boards[selectedBoardIndex];
        foreach (Task *t, b->getTasks())
        {
            QTreeWidgetItem *item = new QTreeWidgetItem();
            item->setText(0, t->getTitle());
            item->setText(1, getStatusLabel(t->getStatusUUID()));
            item->setText(2, getPriorityLabel(t->getPriorityUUID()));
            item->setText(3, t->getExpectedFor().toString());

            QBrush bgColor = item->background(1);
            QBrush fgColor = item->foreground(1);
            bgColor.setColor(getStatusColor(t->getStatusUUID(), bgColor.color()));
            bgColor.setStyle(Qt::BrushStyle::SolidPattern);
            fgColor.setColor(Tools::getForegroundColor(bgColor.color()));
            item->setBackground(1, bgColor);
            item->setForeground(1, fgColor);

            bgColor = item->background(2);
            fgColor = item->foreground(2);
            bgColor.setColor(getPriorityColor(t->getPriorityUUID(), bgColor.color()));
            bgColor.setStyle(Qt::BrushStyle::SolidPattern);
            fgColor.setColor(Tools::getForegroundColor(bgColor.color()));
            item->setBackground(2, bgColor);
            item->setForeground(2, fgColor);

            ui->treeWidget->addTopLevelItem(item);
        }
    }

}

