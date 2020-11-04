#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"
#include "lampdialog.h"
#include <iostream>
#include <QHeaderView>
#include <QStringLiteral>
#include <QAbstractItemModel>
#include <QFileDialog>
#include <QString>
#include <QJsonParseError>
#include <QMessageBox>

using namespace std;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    lampcollection = NULL;
    ui->setupUi(this);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::setTableWidget(int n, int m) {
    //Изменение размеров коллекции
    if (n>0 && m>0) {
        ui->tableWidget->setRowCount(n);
        ui->tableWidget->setColumnCount(m);
        for (int i = 0; i<n; i++)
            for (int j=0; j<m; j++)
            {
                QAbstractItemModel *model = ui->tableWidget->model();
                model->setData(model->index(i, j), QStringLiteral("#%1,%2").arg(i).arg(j));
                ui->tableWidget->item(i, j)->setFlags(Qt::ItemIsEnabled);
            }
        ui->n_number->setText(QString::number(lampcollection->getLength()));
        ui->m_number->setText(QString::number(lampcollection->getWidth()));
        ui->count->setText(QString::number(lampcollection->getNumberOfLamps()));
        ui->x_SpinBox->setEnabled(1);
        ui->x_SpinBox->setValue(0);
        ui->x_SpinBox->setMaximum(n-0.01);
        ui->y_SpinBox->setEnabled(1);
        ui->y_SpinBox->setValue(0);
        ui->y_SpinBox->setMaximum(m-0.01);
        ui->z_SpinBox->setEnabled(1);
        ui->z_SpinBox->setValue(0);
        ui->z_SpinBox->setMaximum(LampCollection::HEIGHT_DEFAULT-0.01);
        ui->h_SpinBox->setEnabled(1);
        ui->h_SpinBox->setValue(LampCollection::HEIGHT_DEFAULT);
        ui->h_SpinBox->setMaximum(LampCollection::HEIGHT_MAX);
        ui->h_SpinBox->setMinimum(LampCollection::HEIGHT_MIN);
        ui->pushButton->setEnabled(1);
        ui->checkBox->setEnabled(1);
    }
}

void MainWindow::on_tableWidget_itemDoubleClicked(QTableWidgetItem *item)
{
    //Двойное нажатие
    Lamp* lamp = lampcollection->getLamp(item->row(), item->column());
    int k;
    if (!lamp)
        k = 0;
    else
        k = lamp->getType()+1;
    LampDialog d(this, lamp, k);
    if (d.exec()) {
        lampcollection->setLamp(item->row(), item->column(), d.getLamp());
        setTableWidget(lampcollection->getLength(), lampcollection->getWidth());
    }
}

void MainWindow::on_action_triggered()
{
    //Создание коллекции
    Dialog d(this);
    if (d.exec()) {
        int n = d.getRowNumber();
        int m = d.getColumnNumber();
        if (lampcollection)
            delete lampcollection;
        lampcollection = new LampCollection(n, m);
        setTableWidget(n, m);
    }
}

void MainWindow::on_action_2_triggered()
{
    //Открытие файла
    QString filename = QFileDialog::getOpenFileName(this, "Open LampCollection", ".", "JSON files (*.json)");
    if (!filename.isEmpty())
        try {
            LampCollection* templampcollection = new LampCollection(LampCollectionFromJson(filename));
            if (lampcollection)
                delete lampcollection;
            lampcollection = templampcollection;
            int n = lampcollection->getLength();
            int m = lampcollection->getWidth();
            setTableWidget(n, m);
        }  catch (exception) {
            //Ошибка
            QMessageBox::critical(this, "Ошибка", "Ошибка при чтении данных");
        }
}

void MainWindow::on_action_3_triggered()
{
    //Сохранение коллекции
    QString filename = QFileDialog::getSaveFileName(this, "Save LampCollection", ".", "JSON files (*.json)");
    if (not filename.isEmpty()) {
        if (lampcollection && filename.endsWith(".json"))
            LampCollectionToJson(filename, *lampcollection);
        else if (!filename.endsWith(".json"))
            QMessageBox::critical(this, "Ошибка", "Неверный формат!");
        else
            QMessageBox::critical(this, "Ошибка", "Коллекция не задана!");
    }
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    //Выставить значения по умолчанию
    if (arg1) {
        ui->x_SpinBox->setValue(0);
        ui->x_SpinBox->setEnabled(0);
        ui->y_SpinBox->setValue(0);
        ui->y_SpinBox->setEnabled(0);
        ui->z_SpinBox->setValue(0);
        ui->z_SpinBox->setEnabled(0);
        ui->h_SpinBox->setValue(LampCollection::HEIGHT_DEFAULT);
        ui->h_SpinBox->setEnabled(0);
    }
    else {
        ui->x_SpinBox->setEnabled(1);
        ui->y_SpinBox->setEnabled(1);
        ui->z_SpinBox->setEnabled(1);
        ui->h_SpinBox->setEnabled(1);
    }
}

void MainWindow::on_h_SpinBox_valueChanged(double arg1)
{
    //Изменение значения высоты
    if (ui->z_SpinBox->isEnabled()) {
        if (ui->z_SpinBox->value() > arg1)
            ui->z_SpinBox->setValue(arg1-0.01);
        ui->z_SpinBox->setMaximum(arg1-0.01);
    }
}

void MainWindow::on_pushButton_clicked()
{
    //Расчёт освещённости в точке
    float x, y, z, h;
    x = ui->x_SpinBox->value();
    y = ui->y_SpinBox->value();
    z = ui->z_SpinBox->value();
    h = ui->h_SpinBox->value();
    ui->result->setEnabled(1);
    double result = lampcollection->getIlluminance(x, y, z, h);;
    ui->result->setText(QString::number(result, 'f', 4));
    ui->result->setEnabled(0);
}

void MainWindow::on_action_4_triggered()
{
    //О программе
    QMessageBox::about(this, "О программе", "<h2>Объектно-ориентированное програмиирование</h2>" \
                                            "<font size=4>Программа для работы с классами Лампа и Коллекция Ламп</font><br><font size=4>Вариант 14</font>" \
                                            "<br><font size=4>Разработчик: Чашкин Леонид, БИВ194</font><p align='center'>МИЭМ НИУ ВШЭ, 2020</p>");
}
