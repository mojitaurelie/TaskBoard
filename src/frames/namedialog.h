#ifndef NAMEDIALOG_H
#define NAMEDIALOG_H

#include <QDialog>

namespace Ui {
class NameDialog;
}

class NameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NameDialog(QString label, QString defaultName, QWidget *parent = nullptr);
    ~NameDialog();
    const QString getChoosenName();

private:
    Ui::NameDialog *ui;

    QString defaultName;
};

#endif // NAMEDIALOG_H
