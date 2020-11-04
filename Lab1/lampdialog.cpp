#include "lampdialog.h"
#include "ui_lampdialog.h"
#include "ledlamp.h"
#include <QString>

LampDialog::LampDialog(QWidget *parent, Lamp* lamp, int type) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::LampDialog)
{
    type_ = type;
    if (type==1)
        lamp_ = new Lamp(*lamp);
    else if (type==2)
        lamp_ = new LedLamp(*((LedLamp*)lamp));
    else
        lamp_ = NULL;
    ui->setupUi(this);
    ui->power->setMinimum(Lamp::POWER_MIN);
    ui->power->setMaximum(Lamp::POWER_MAX);
    ui->power->setValue(Lamp::POWER_DEFAULT);
    ui->intensity->setMinimum(Lamp::INTENSITY_MIN);
    ui->intensity->setMaximum(Lamp::INTENSITY_MAX);
    ui->intensity->setValue(Lamp::INTENSITY_DEFAULT);
    setDialogWindow(type_);
    if (type_)
        setValues();
}

void LampDialog::setDialogWindow(int type) {
    ui->type->setCurrentIndex(type);
    int k = 1;
    if (type != 2)
        k = 0;
    ui->blueSlider->setVisible(k);
    ui->greenSlider->setVisible(k);
    ui->redSlider->setVisible(k);
    ui->bluespinBox->setVisible(k);
    ui->greenspinBox->setVisible(k);
    ui->redspinBox->setVisible(k);
    ui->label_3->setVisible(k);
    ui->label_4->setVisible(k);
    ui->result->setVisible(k);
    if (type == 0)
        k = 0;
    else
        k = 1;
    ui->label->setVisible(k);
    ui->label_2->setVisible(k);
    ui->power->setVisible(k);
    ui->intensity->setVisible(k);
}

void LampDialog::setValues() {
    ui->power->setValue(lamp_->getPower());
    ui->intensity->setValue(lamp_->getIntensity());
    if (type_ == 2) {
        LedLamp* ledlamp = (LedLamp*)lamp_;
        ui->bluespinBox->setValue(ledlamp->getRGBBlue());
        ui->greenspinBox->setValue(ledlamp->getRGBGreen());
        ui->redspinBox->setValue(ledlamp->getRGBRed());
        setRGBIntensity();
    }
}

void LampDialog::setRGBIntensity() {
    int power = ui->power->value();
    int intensity = ui->intensity->value();
    unsigned char R = ui->redSlider->value();
    unsigned char G = ui->greenSlider->value();
    unsigned char B = ui->blueSlider->value();
    LedLamp ledlamp(power, intensity, R, G, B);
    ui->result->setText(QString::number(ledlamp.getCurrentIntensity(), 'f', 3));
}

Lamp* LampDialog::getLamp() const {
    return lamp_;
}

LampDialog::~LampDialog()
{
    if (lamp_)
        delete lamp_;
    delete ui;
}

void LampDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    if (button->text() == tr("Reset")) {
        setDialogWindow(type_);
        if (type_)
            setValues();
    }
    else if (button->text() == tr("Cancel")) {
        this->reject();
        this->close();
    }
    else {
        //Сохранить
        int type = ui->type->currentIndex();
        if (lamp_)
            delete lamp_;
        if (type == 0)
            lamp_ = NULL;
        else {
            int power = ui->power->value();
            int intensity = ui->intensity->value();
            if (type == 2) {
                unsigned char R = ui->redSlider->value();
                unsigned char G = ui->greenSlider->value();
                unsigned char B = ui->blueSlider->value();
                lamp_ = new LedLamp(power, intensity, R, G, B);
            }
            else
                lamp_ = new Lamp(power, intensity);
        }
        this->accept();
        this->close();
    }
}

void LampDialog::on_type_currentIndexChanged(int index)
{
    setDialogWindow(index);
    if (index == 2)
        setRGBIntensity();
}

void LampDialog::on_redSlider_valueChanged(int value)
{
    ui->redspinBox->setValue(value);
    setRGBIntensity();
}

void LampDialog::on_greenSlider_valueChanged(int value)
{
    ui->greenspinBox->setValue(value);
    setRGBIntensity();
}

void LampDialog::on_blueSlider_valueChanged(int value)
{
    ui->bluespinBox->setValue(value);
    setRGBIntensity();
}

void LampDialog::on_redspinBox_valueChanged(int arg1)
{
    ui->redSlider->setValue(arg1);
    setRGBIntensity();
}

void LampDialog::on_greenspinBox_valueChanged(int arg1)
{
    ui->greenSlider->setValue(arg1);
    setRGBIntensity();
}

void LampDialog::on_bluespinBox_valueChanged(int arg1)
{
    ui->blueSlider->setValue(arg1);
    setRGBIntensity();
}
