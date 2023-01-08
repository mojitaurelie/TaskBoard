#include "mainwindow.h"
#include "ui_mainwindow.h"

#define PRIORITIES_KEY "priorities"
#define STATUS_KEY "status"
#define BOARDS_KEY "boards"

#include <QUuid>
#include <QColor>
#include <QJsonArray>
#include <QJsonObject>
#include <QMessageBox>

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
    init();
    this->selectedBoardIndex = -1;
    // Change "name" column size
    ui->taskList->header()->resizeSection(0, 520);
    // Change "status" column size
    ui->taskList->header()->resizeSection(1, 150);
    connect(ui->actionPreferences, &QAction::triggered, this, &MainWindow::openPreferences);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::openAbout);
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::onNewBoardClick);
    connect(ui->actionNew_task, &QAction::triggered, this, &MainWindow::onNewTaskClick);
    connect(ui->boardList, &QListWidget::currentRowChanged, this, &MainWindow::onBoardSelected);
    connect(ui->boardList, &QListWidget::customContextMenuRequested, this, &MainWindow::prepareBoardMenu);
    connect(ui->taskList, &QTreeWidget::itemDoubleClicked, this, &MainWindow::onEditTask);
    connect(ui->taskList, &QTreeWidget::customContextMenuRequested, this, &MainWindow::prepareTaskMenu);
    ui->boardList->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->taskList->setContextMenuPolicy(Qt::CustomContextMenu);
}

MainWindow::~MainWindow()
{
    for (uint16_t i = 0; i < boards.count(); i++)
    {
        Board *b = boards.takeAt(i);
        delete b;
    }
    delete ui;
}

void MainWindow::openPreferences()
{
    PrefDialog dialog(status, priorities, this);
    if (dialog.exec() == QDialog::DialogCode::Accepted)
    {
        this->priorities = dialog.getPriorities();
        this->status = dialog.getStatus();
        save();
        redrawTaskTree();
    }
}

void MainWindow::openAbout()
{
    AboutDialog dialog(this);
    dialog.exec();
}

void MainWindow::prepareBoardMenu(const QPoint &pos)
{
    if (ui->boardList->selectedItems().length() == 1) {
        QMenu menu(this);

        QAction *renameAction = new QAction(tr("Rename this board"), this);
        connect(renameAction, &QAction::triggered, this, &MainWindow::onEditNameBoardMenu);
        menu.addAction(renameAction);

        menu.addSeparator();

        QAction *deleteAction = new QAction(tr("Delete this board"), this);
        connect(deleteAction, &QAction::triggered, this, &MainWindow::onRemoveBoardMenu);
        menu.addAction(deleteAction);

        menu.exec(ui->boardList->mapToGlobal(pos));
    }
}

void MainWindow::prepareTaskMenu(const QPoint &pos)
{
    if (ui->taskList->selectedItems().length() == 1) {
        QMenu menu(this);

        QAction *renameAction = new QAction(tr("Edit the task"), this);
        connect(renameAction, &QAction::triggered, this, &MainWindow::onEditNameTaskMenu);
        menu.addAction(renameAction);

        menu.addSeparator();

        QAction *deleteAction = new QAction(tr("Delete from the board"), this);
        connect(deleteAction, &QAction::triggered, this, &MainWindow::onRemoveTaskMenu);
        menu.addAction(deleteAction);

        menu.exec(ui->taskList->mapToGlobal(pos));
    }
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
        ui->boardList->addItem(item);
        save();
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
            updateTaskRow(item, t);
            ui->taskList->addTopLevelItem(item);
            save();
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
        int row = ui->taskList->indexOfTopLevelItem(item);
        Task *t = b->taskAt(row);
        if (t != nullptr)
        {
            TaskDialog dialog(t, status, priorities, this);
            if (dialog.exec() == QDialog::DialogCode::Accepted)
            {
                Task editedTask = dialog.getTask();
                t->update(editedTask);
                updateTaskRow(item, editedTask);
                save();
            }
        }
    }
}

void MainWindow::onRemoveBoardMenu()
{
    if (selectedBoardIndex > -1)
    {
        QMessageBox::StandardButton result = QMessageBox::question(this, "Delete a board", "Do you want to delete this board?");
        if (result == QMessageBox::Yes)
        {
            boards.removeAt(selectedBoardIndex);
            delete ui->boardList->takeItem(selectedBoardIndex);
            selectedBoardIndex = -1;
            redrawTaskTree();
            save();
        }
    }
}

void MainWindow::onRemoveTaskMenu()
{
    if (selectedBoardIndex > -1 && ui->taskList->selectedItems().length() == 1)
    {
        QMessageBox::StandardButton result = QMessageBox::question(this, "Delete a task", "Do you want to delete this task?");
        if (result == QMessageBox::Yes)
        {
            int16_t i = ui->taskList->indexOfTopLevelItem(ui->taskList->currentItem());
            Board *b = boards[selectedBoardIndex];
            b->remove(i);
            redrawTaskTree();
            save();
        }
    }
}

void MainWindow::onEditNameBoardMenu()
{
    if (selectedBoardIndex > -1)
    {
        Board *b = boards.at(selectedBoardIndex);
        NameDialog dialog("Edit board name", b->getName(), this);
        if (dialog.exec() == QDialog::DialogCode::Accepted)
        {
            QString newName= dialog.getChoosenName();
            b->setName(newName);
            ui->boardList->item(selectedBoardIndex)->setText(newName);
            ui->label->setText(newName);
            save();
        }
    }
}

void MainWindow::onEditNameTaskMenu()
{
    if (selectedBoardIndex > -1 && ui->taskList->selectedItems().length() == 1)
    {
        onEditTask(ui->taskList->currentItem());
    }
}

void MainWindow::init()
{
    if (Tools::isSaveFileExist())
    {
        QJsonDocument doc;
        if (Tools::readSaveFile(doc))
        {
            QJsonObject save = doc.object();
            QJsonArray jsonPriorities = save[PRIORITIES_KEY].toArray();
            QJsonArray jsonStatus = save[STATUS_KEY].toArray();
            QJsonArray jsonBoards = save[BOARDS_KEY].toArray();
            for (QJsonValueRef value : jsonPriorities) {
                priorities.append(Priority(value.toObject()));
            }
            for (QJsonValueRef value : jsonStatus) {
                status.append(Status(value.toObject()));
            }
            for (QJsonValueRef value : jsonBoards) {
                boards.append(new Board(value.toObject()));
            }
            redrawBoardList();
            return;
        }
    }
    this->priorities = defaultPriorities();
    this->status = defaultStatus();
    save();
}

QVector<Priority> MainWindow::defaultPriorities()
{
    QVector<Priority> res;
    res.append(Priority(QUuid::createUuid().toString(QUuid::WithoutBraces), "None", QColor(217, 217, 217)));
    res.append(Priority(QUuid::createUuid().toString(QUuid::WithoutBraces), "Low", QColor(48, 157, 176)));
    res.append(Priority(QUuid::createUuid().toString(QUuid::WithoutBraces), "Medium", QColor(176, 142, 48)));
    res.append(Priority(QUuid::createUuid().toString(QUuid::WithoutBraces), "High", QColor(176, 67, 48)));
    return res;
}

QVector<Status> MainWindow::defaultStatus()
{
    QVector<Status> res;
    res.append(Status(QUuid::createUuid().toString(QUuid::WithoutBraces), "To Do", QColor(143, 143, 143)));
    res.append(Status(QUuid::createUuid().toString(QUuid::WithoutBraces), "Working on", QColor(95, 48, 176)));
    res.append(Status(QUuid::createUuid().toString(QUuid::WithoutBraces), "Completed", QColor(48, 176, 73)));
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

const QJsonDocument MainWindow::getJsonSave()
{
    QJsonDocument doc;
    QJsonObject obj;
    QJsonArray jsonPriorities;
    foreach (Priority p, this->priorities) {
        jsonPriorities.append(p.toJson());
    }
    QJsonArray jsonStatus;
    foreach (Status s, this->status) {
        jsonStatus.append(s.toJson());
    }
    QJsonArray jsonBoards;
    foreach (Board *b, this->boards) {
        jsonBoards.append(b->toJson());
    }
    obj[PRIORITIES_KEY] = jsonPriorities;
    obj[STATUS_KEY] = jsonStatus;
    obj[BOARDS_KEY] = jsonBoards;
    doc.setObject(obj);
    return doc;
}

void MainWindow::updateTaskRow(QTreeWidgetItem *item, Task t)
{
    item->setText(0, t.getTitle());
    item->setToolTip(0, t.getDescription());
    item->setText(3, t.getExpectedFor().toString());

    if (t.getExpectedFor() < QDate::currentDate())
    {
        QBrush fgColor = item->foreground(3);
        fgColor.setColor(QColor(176, 67, 48));
        fgColor.setStyle(Qt::BrushStyle::SolidPattern);
        item->setForeground(3, fgColor);
    }
    else if (t.getExpectedFor() == QDate::currentDate())
    {
        QBrush fgColor = item->foreground(3);
        fgColor.setColor(QColor(176, 142, 48));
        fgColor.setStyle(Qt::BrushStyle::SolidPattern);
        item->setForeground(3, fgColor);
    }

    if (!t.getStatusUUID().isEmpty())
    {
        item->setText(1, getStatusLabel(t.getStatusUUID()));
        QBrush bgColor = item->background(1);
        QBrush fgColor = item->foreground(1);
        bgColor.setColor(getStatusColor(t.getStatusUUID(), bgColor.color()));
        bgColor.setStyle(Qt::BrushStyle::SolidPattern);
        fgColor.setColor(Tools::getForegroundColor(bgColor.color()));
        fgColor.setStyle(Qt::BrushStyle::SolidPattern);
        item->setBackground(1, bgColor);
        item->setForeground(1, fgColor);
    }

    if (!t.getPriorityUUID().isEmpty())
    {
        item->setText(2, getPriorityLabel(t.getPriorityUUID()));
        QBrush bgColor = item->background(2);
        QBrush fgColor = item->foreground(2);
        bgColor.setColor(getPriorityColor(t.getPriorityUUID(), bgColor.color()));
        bgColor.setStyle(Qt::BrushStyle::SolidPattern);
        fgColor.setColor(Tools::getForegroundColor(bgColor.color()));
        fgColor.setStyle(Qt::BrushStyle::SolidPattern);
        item->setBackground(2, bgColor);
        item->setForeground(2, fgColor);
    }
}


void MainWindow::redrawBoardList()
{
    QListWidget *l = ui->boardList;
    uint16_t itemCount = l->count();
    for (int16_t i = itemCount; i >= 0; i--)
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
    QTreeWidget *l = ui->taskList;
    uint16_t itemCount = l->topLevelItemCount();
    for (int16_t i = itemCount; i >= 0; i--)
    {
        delete l->takeTopLevelItem(i);
    }
    if (selectedBoardIndex > -1)
    {
        Board *b = boards[selectedBoardIndex];
        foreach (Task *t, b->getTasks())
        {
            QTreeWidgetItem *item = new QTreeWidgetItem();
            updateTaskRow(item, *t);
            ui->taskList->addTopLevelItem(item);
        }
    }

}

void MainWindow::save()
{
    QJsonDocument doc = getJsonSave();
    if (!Tools::writeSaveToFile(doc))
    {
        QMessageBox::critical(this, "Failed to save", "Failed to write the save to the file", QMessageBox::StandardButton::Ok);
    }
}

