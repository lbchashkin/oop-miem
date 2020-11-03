#ifndef LAMPDIALOG_H
#define LAMPDIALOG_H

#include "lamp.h"
#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class LampDialog;
}

class LampDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LampDialog(QWidget *parent = nullptr, Lamp* lamp=NULL, int type=0);
    Lamp* getLamp() const;
    ~LampDialog();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

    void on_type_currentIndexChanged(int index);

    void on_redSlider_valueChanged(int value);

    void on_greenSlider_valueChanged(int value);

    void on_blueSlider_valueChanged(int value);

    void on_redspinBox_valueChanged(int arg1);

    void on_greenspinBox_valueChanged(int arg1);

    void on_bluespinBox_valueChanged(int arg1);

private:
    Lamp* lamp_;
    int type_;
    void setDialogWindow(int type);
    void setValues();
    void setRGBIntensity();
    Ui::LampDialog *ui;
};

#endif // LAMPDIALOG_H
