#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QBrush>
#include <QJsonDocument>

#include "../models/priority.h"
#include "../models/status.h"
#include "../models/board.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openPreferences();
    void openAbout();
    void onNewBoardClick();
    void onNewTaskClick();
    void onBoardSelected(int i);
    void onEditTask(QTreeWidgetItem*);
    void onRemoveBoardMenu();
    void onRemoveTaskMenu();
    void onEditNameBoardMenu();
    void onEditNameTaskMenu();
    void prepareBoardMenu(const QPoint &pos);
    void prepareTaskMenu(const QPoint &pos);

private:
    Ui::MainWindow *ui;

    void init();

    int16_t selectedBoardIndex;
    QVector<Priority> priorities;
    QVector<Status> status;
    QVector<Board*> boards;

    QVector<Priority> defaultPriorities();
    QVector<Status> defaultStatus();

    const QString getPriorityLabel(QString uuid);
    const QString getStatusLabel(QString uuid);

    const QColor getPriorityColor(QString uuid, QColor defaultColor);
    const QColor getStatusColor(QString uuid, QColor defaultColor);

    const QJsonDocument getJsonSave();

    void redrawBoardList();
    void redrawTaskTree();
    void save();
};
#endif // MAINWINDOW_H
