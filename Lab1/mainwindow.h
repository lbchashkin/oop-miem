#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include "lampcollection.h"

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
    void on_tableWidget_itemDoubleClicked(QTableWidgetItem *item);

    void on_action_triggered();

    void on_action_2_triggered();

    void on_action_3_triggered();

    void on_checkBox_stateChanged(int arg1);

    void on_h_SpinBox_valueChanged(double arg1);

    void on_pushButton_clicked();

    void on_action_4_triggered();

private:
    void setTableWidget(int n, int m);
    LampCollection *lampcollection;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
