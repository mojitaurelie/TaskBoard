#ifndef PREFDIALOG_H
#define PREFDIALOG_H

#include <QDialog>
#include <QColor>
#include <QListWidgetItem>

#include "../models/priority.h"
#include "../models/status.h"

namespace Ui {
class PrefDialog;
}

class PrefDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PrefDialog(QVector<Status> status, QVector<Priority> priorities, QWidget *parent = nullptr);
    ~PrefDialog();

    QVector<Priority> getPriorities();
    QVector<Status> getStatus();

private slots:
    void onAddStatusButtonClick();
    void onAddPriorityButtonClick();
    void onItemSelectionChange(int);
    void onButtonColorButtonClick();
    void onNameChange();
    void onRemoveStatusButtonClick();
    void onRemovePriorityButtonClick();

private:
    Ui::PrefDialog *ui;

    QVector<QString> statusUUIDRef;
    QVector<QString> priorityUUIDRef;

    void setItemColor(QListWidgetItem*, QColor);
};

#endif // PREFDIALOG_H
