#include <iostream>
#include <QJsonParseError>
#include <assert.h>
#include "tests.h"
#include "lamp.h"
#include "ledlamp.h"
#include "lampcollection.h"

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

void testLampType() {
    Lamp lamp;
    LedLamp ledlamp;
    assert(lamp.getType() == 0);
    assert(ledlamp.getType() == 1);
};

void testLampCollectionConstructor() {
    //Тестирование конструктора инициализации коллекции
    LampCollection room1(2, 2);
    for (int i=0; i<2; i++)
        for (int j=0; j<2; j++) {
            Lamp* lamp = room1.getLamp(i, j);
            assert(not lamp);
        }
};

void testLampCollectionCopyConstructor() {
    //Тестирование конструктора копирования коллекции
    LampCollection room1(2, 2);
    Lamp lamp1(100);
    Lamp lamp2(120, 200);
    room1.setLamp(0, 1, &lamp1);
    room1.setLamp(1, 1, &lamp2);
    LampCollection room2 = room1;
    assert(room1.isEqual(room2));
};

void testLampCollectionSetLamp() {
    //Тестирование методов установки и получения лампы по индексам
    LampCollection room(5, 5);
    Lamp lamp(100, 160);
    room.setLamp(2, 3, &lamp);
    Lamp lamp2(*room.getLamp(2, 3));
    assert(lamp.getPower() == lamp2.getPower());
    assert(lamp.getIntensity() == lamp2.getIntensity());
    try {
        room.setLamp(-8, 7, &lamp);
        assert(false);
    }  catch (const out_of_range &exc) {
        assert(true);
    };
    try {
        room.getLamp(8, 2);
        assert(false);
    }  catch (const out_of_range &exc) {
        assert(true);
    }
};

void testLampCollectionNumberOfLamps() {
    //Тестирование методов подсчёта ламп в коллекции
    LampCollection room(5, 3);
    assert(5 == room.getLength());
    assert(3 == room.getWidth());
    assert(15 == room.getNumberOfLamps());
};

void testLampCollectionGetIlluminance() {
    //Тестирование метода получения освещённости
    LampCollection room(1, 1);
    Lamp lamp(100, 100);
    room.setLamp(0, 0, &lamp);
    assert(room.getIlluminance(0, 0, 0, 2) == 25);
};

void testLampCollectionJson() {
    //Тестирование записи в файл json и чтения из файла
    Lamp lamp(60, 90);
    LampCollection room1(2, 2);
    room1.setLamp(0, 0, &lamp);
    printLampCollection(room1);
    if (LampCollectionToJson("data.json", room1)) {
        try {
            LampCollection room2(LampCollectionFromJson("data.json"));
            assert(room1.isEqual(room2));
        }  catch (QJsonParseError) {
            assert(false);
        }
    }
    else
        assert(false);
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
    testLampType();
    cout << "All Lamp tests are passed" << endl;
};

void testLampCollection() {
    //Тестирование класса Коллекция ламп
    testLampCollectionConstructor();
    testLampCollectionCopyConstructor();
    testLampCollectionSetLamp();
    testLampCollectionNumberOfLamps();
    testLampCollectionJson();
    testLampCollectionGetIlluminance();
    cout << "All LampCollection tests are passed" << endl;
};
