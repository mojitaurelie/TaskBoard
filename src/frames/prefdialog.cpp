#include "prefdialog.h"
#include "ui_prefdialog.h"

#include <QColorDialog>
#include <QUuid>

#include "../tools.h"

PrefDialog::PrefDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrefDialog)
{
    ui->setupUi(this);
    connect(ui->addStatusButton, &QPushButton::clicked, this, &PrefDialog::onAddStatusButtonClick);
    connect(ui->addPriorityButton, &QPushButton::clicked, this, &PrefDialog::onAddPriorityButtonClick);
    connect(ui->statusListWidget, &QListWidget::currentRowChanged, this, &PrefDialog::onItemSelectionChange);
    connect(ui->priorityListWidget, &QListWidget::currentRowChanged, this, &PrefDialog::onItemSelectionChange);
    connect(ui->colorStatusButton, &QToolButton::clicked, this, &PrefDialog::onButtonColorButtonClick);
    connect(ui->colorPriorityButton, &QToolButton::clicked, this, &PrefDialog::onButtonColorButtonClick);
    connect(ui->nameStatusEdit, &QLineEdit::textEdited, this, &PrefDialog::onNameChange);
    connect(ui->namePriorityEdit, &QLineEdit::textEdited, this, &PrefDialog::onNameChange);
    connect(ui->removeStatusButton, &QPushButton::clicked, this, &PrefDialog::onRemoveStatusButtonClick);
    connect(ui->removePriorityButton, &QPushButton::clicked, this, &PrefDialog::onRemovePriorityButtonClick);
}

PrefDialog::~PrefDialog()
{
    delete ui;
}

QVector<Priority> PrefDialog::getPriorities()
{
    int count = ui->priorityListWidget->count();
    QVector<Priority> res;

    for (int i = 0; i < count; i++)
    {
        QListWidgetItem *item = ui->priorityListWidget->item(i);
        Priority p(priorityUUIDRef[i], item->text(), item->background().color());
        res.append(p);
    }

    return res;
}

QVector<Status> PrefDialog::getStatus()
{
    int count = ui->statusListWidget->count();
    QVector<Status> res;

    for (int i = 0; i < count; i++)
    {
        QListWidgetItem *item = ui->statusListWidget->item(i);
        Status s(statusUUIDRef[i], item->text(), item->background().color());
        res.append(s);
    }

    return res;
}

void PrefDialog::onAddStatusButtonClick()
{
    QColor bgColor = Tools::getRandomColor();
    QListWidgetItem *item = new QListWidgetItem("New status");
    QUuid uuid = QUuid::createUuid();
    statusUUIDRef.append(uuid.toString(QUuid::WithoutBraces));

    setItemColor(item, bgColor);

    ui->statusListWidget->addItem(item);
}

void PrefDialog::onAddPriorityButtonClick()
{
    QColor bgColor = Tools::getRandomColor();
    QListWidgetItem *item = new QListWidgetItem("Low");
    QUuid uuid = QUuid::createUuid();
    priorityUUIDRef.append(uuid.toString(QUuid::WithoutBraces));

    setItemColor(item, bgColor);

    ui->priorityListWidget->addItem(item);
}

void PrefDialog::onItemSelectionChange(int index)
{
    QListWidget *listWidget = static_cast<QListWidget*>(QObject::sender());
    QPushButton *removeBtn;
    QToolButton *colorBtn;
    QLineEdit *nameEdit;
    QLineEdit *colorEdit;

    if (listWidget == ui->statusListWidget)
    {
        removeBtn = ui->removeStatusButton;
        colorBtn = ui->colorStatusButton;
        nameEdit = ui->nameStatusEdit;
        colorEdit = ui->colorStatusEdit;
    }
    else
    {
        removeBtn = ui->removePriorityButton;
        colorBtn = ui->colorPriorityButton;
        nameEdit = ui->namePriorityEdit;
        colorEdit = ui->colorPriorityEdit;
    }
    bool d = (index == -1);
    removeBtn->setDisabled(d);
    colorBtn->setDisabled(d);
    nameEdit->setDisabled(d);
    colorEdit->setDisabled(d);
    if (!d)
    {
        QListWidgetItem *current = listWidget->item(index);
        nameEdit->setText(current->text());
        colorEdit->setText(current->background().color().name());
    }
    else
    {
        nameEdit->clear();
        colorEdit->clear();
    }
}

void PrefDialog::onButtonColorButtonClick()
{
    QColorDialog dialog(this);
    if (dialog.exec() == DialogCode::Accepted)
    {
        QToolButton *sender = static_cast<QToolButton*>(QObject::sender());
        QLineEdit *colorEdit;
        QListWidgetItem *item;
        if (sender == ui->colorStatusButton)
        {
            colorEdit = ui->colorStatusEdit;
            item = ui->statusListWidget->currentItem();
        }
        else
        {
            colorEdit = ui->colorPriorityEdit;
            item = ui->priorityListWidget->currentItem();
        }
        QColor selectedColor = dialog.selectedColor();
        colorEdit->setText(selectedColor.name());
        setItemColor(item, selectedColor);
    }
}

void PrefDialog::onNameChange()
{
    QLineEdit *sender = static_cast<QLineEdit*>(QObject::sender());
    QListWidgetItem *item;
    if (sender == ui->nameStatusEdit)
    {
        if (ui->statusListWidget->selectedItems().size() != 1)
        {
            return;
        }
        item = ui->statusListWidget->currentItem();
    }
    else
    {
        if (ui->priorityListWidget->selectedItems().size() != 1)
        {
            return;
        }
        item = ui->priorityListWidget->currentItem();
    }
    item->setText(sender->text());
}

void PrefDialog::onRemoveStatusButtonClick()
{
    if (ui->statusListWidget->selectedItems().size() != 1)
    {
        return;
    }
    int index = ui->statusListWidget->currentRow();
    delete ui->statusListWidget->takeItem(index);
    statusUUIDRef.removeAt(index);
}

void PrefDialog::onRemovePriorityButtonClick()
{
    if (ui->priorityListWidget->selectedItems().size() != 1)
    {
        return;
    }
    int index = ui->priorityListWidget->currentRow();
    delete ui->priorityListWidget->takeItem(index);
    priorityUUIDRef.removeAt(index);
}

void PrefDialog::setItemColor(QListWidgetItem *item, QColor bgColor)
{
    QColor fgColor = Tools::getForegroundColor(bgColor);

    // Set background
    QBrush bg = item->background();
    bg.setStyle(Qt::BrushStyle::SolidPattern);
    bg.setColor(bgColor);
    item->setBackground(bg);

    // Set foreground
    QBrush fg = item->foreground();
    fg.setStyle(Qt::BrushStyle::SolidPattern);
    fg.setColor(fgColor);
    item->setForeground(fg);
}
