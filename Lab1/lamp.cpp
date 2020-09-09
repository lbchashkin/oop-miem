#include "lamp.h"
#include <iostream>

using namespace std;

Lamp::Lamp(const int power, const int intensity) {
    /*
    Конструктор по умолчанию:
    мощность = 60Вт, сила света = 100Кд
    Конструктор иницализации:
    -с одним параметром (сила света = 100Кд)
    -с двумя параметрами (мощность, сила света)
    */
    setPower(power);
    setIntensity(intensity);
};

Lamp::Lamp(const Lamp &lamp) {
    //Конструктор копирования
    setPower(lamp.getPower());
    setIntensity(lamp.getIntensity());
};

void Lamp::printParams() const {
    //Вывод на экран параметров лампы
    cout << "Power: " << power_ << endl;
    cout << "Intensity: " << intensity_ << endl;
};
int Lamp::getPower() const {
    //Мощность лампы
    return power_;
};

int Lamp::getIntensity() const {
    //Возвращает силу света лампы
    return intensity_;
};

void Lamp::setPower(const int power) {
    //Проверяет корректность мощности и устанавливает значение соответствующего атрибута
    if (power <= POWER_MIN)
        power_ = POWER_MIN;
    else if (power >= POWER_MAX)
        power_ = POWER_MAX;
    else
        power_ = power;
};

void Lamp::setIntensity(const int intensity) {
    //Проверяет корректность силы света и устанавливает значение соответствующего атрибута
    if (intensity <= INTENSITY_MIN)
        intensity_ = INTENSITY_MIN;
    else if (intensity >= INTENSITY_MAX)
        intensity_ = INTENSITY_MAX;
    else
        intensity_ = intensity;
};
