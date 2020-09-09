#include "tests.h"
#include "lamp.h"
#include <assert.h>
#include <iostream>

using namespace std;

void testLampWithDefaultConstructor() {
    //Тестирование конструктора по умолчанию
    Lamp lamp;
    assert(Lamp::POWER_DEFAULT == lamp.getPower());
    assert(Lamp::INTENSITY_DEFAULT == lamp.getIntensity());
};

void testLampWithOneParam() {
    //Тестирование инициализирующего конструктора с одним параметром (мощность)
    Lamp lamp(80);
    assert(80 == lamp.getPower());
    assert(Lamp::INTENSITY_DEFAULT == lamp.getIntensity());
};

void testLampWithTwoParams() {
    //Тестирование инициализирующего конструктора с 2 параметрами
    Lamp lamp(120, 150);
    assert(120 == lamp.getPower());
    assert(150 == lamp.getIntensity());
};

void testLampWithCopyConstructor() {
    //Тестирование копирующего конструктора
    Lamp lamp1(30, 90);
    Lamp lamp2(lamp1);
    assert(30 == lamp2.getPower());
    assert(90 == lamp2.getIntensity());
};

void testLampSetPower() {
    //Тестирование установки значения мощности
    Lamp lamp;
    lamp.setPower(90);
    assert(90 == lamp.getPower());
    lamp.setPower(Lamp::POWER_MIN-1);
    assert(Lamp::POWER_MIN == lamp.getPower());
    lamp.setPower(Lamp::POWER_MAX+1);
    assert(Lamp::POWER_MAX == lamp.getPower());
};

void testLampSetIntensity() {
    //Тестирование установки значения силы света
    Lamp lamp;
    lamp.setIntensity(300);
    assert(300 == lamp.getIntensity());
    lamp.setIntensity(Lamp::INTENSITY_MIN-1);
    assert(Lamp::INTENSITY_MIN == lamp.getIntensity());
    lamp.setIntensity(Lamp::INTENSITY_MAX+1);
    assert(Lamp::INTENSITY_MAX == lamp.getIntensity());
};

void testLampLimits() {
    //Тестирование установки предельных значений
    Lamp lamp1(Lamp::POWER_MAX, Lamp::INTENSITY_MAX);
    assert(Lamp::POWER_MAX == lamp1.getPower());
    assert(Lamp::INTENSITY_MAX == lamp1.getIntensity());
    lamp1.setPower(Lamp::POWER_MIN);
    lamp1.setIntensity(Lamp::INTENSITY_MIN);
    assert(Lamp::POWER_MIN == lamp1.getPower());
    assert(Lamp::INTENSITY_MIN == lamp1.getIntensity());
};

void testLamps() {
    //Тестирование класса Лампа
    testLampWithDefaultConstructor();
    testLampWithOneParam();
    testLampWithTwoParams();
    testLampWithCopyConstructor();
    testLampSetPower();
    testLampSetIntensity();
    testLampLimits();
    cout << "All tests are passed" << endl;
}
