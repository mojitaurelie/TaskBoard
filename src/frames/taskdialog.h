#ifndef TASKDIALOG_H
#define TASKDIALOG_H

#include <QDialog>
#include <QVector>

#include "../models/status.h"
#include "../models/priority.h"
#include "../models/task.h"

namespace Ui {
class TaskDialog;
}

class TaskDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TaskDialog(QVector<Status>, QVector<Priority>, QWidget *parent = nullptr);
    TaskDialog(Task*, QVector<Status>, QVector<Priority>, QWidget *parent = nullptr);
    ~TaskDialog();

    Task getTask();

private:
    Ui::TaskDialog *ui;

    QVector<Status> status;
    QVector<Priority> priorities;
};

#endif // TASKDIALOG_H
