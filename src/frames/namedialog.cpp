#include "namedialog.h"
#include "ui_namedialog.h"

NameDialog::NameDialog(QString label, QString defaultName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NameDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(label);
    this->defaultName = defaultName;
}

NameDialog::~NameDialog()
{
    delete ui;
}

const QString NameDialog::getChoosenName()
{
    if (ui->lineEdit->text().length() > 0)
    {
        return ui->lineEdit->text();
    }
    return defaultName;
}
