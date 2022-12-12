#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    ui->appName->setText(QString("%1 (%2)").arg(APP_NAME, APP_ARCH));
    ui->version->setText(QString("v%1").arg(APP_VERSION));
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
