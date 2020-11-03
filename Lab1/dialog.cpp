#include "dialog.h"
#include "ui_dialog.h"

#include <Qt>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

int Dialog::getColumnNumber() const {
    return ui->columnNumber->text().toInt();
}

int Dialog::getRowNumber() const {
    return ui->rowsNumber->text().toInt();
}

Dialog::~Dialog()
{
    delete ui;
}
