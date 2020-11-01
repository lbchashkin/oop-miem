#include "ledlamp.h"
#include <iostream>

using namespace std;

LedLamp::LedLamp () : Lamp() {
    //Конструктор по умолчанию
    setRGBRed(0);
    setRGBBlue(0);
    setRGBGreen(0);
};

LedLamp::LedLamp (int power, int max_intensity, unsigned char R,
                  unsigned char G, unsigned char B) :
    Lamp(power, max_intensity) {
    //Конструктор инициализации
    setRGBRed(R);
    setRGBGreen(G);
    setRGBBlue(B);
};

LedLamp::LedLamp (LedLamp &lamp):Lamp(lamp) {
    //Конструктор копирования
    setRGBRed(lamp.getRGBRed());
    setRGBGreen(lamp.getRGBGreen());
    setRGBBlue(lamp.getRGBBlue());
};

void LedLamp::printParams() const {
    //Вывод на экран параметров лампы
    cout << "LedLamp" << endl;
    cout << "Power: " << this->getPower() << endl;
    cout << "Intensity: " << this->getIntensity() << endl;
    cout << "RGB" << endl;
    cout << "R:" << (int)_rgb.R << endl;
    cout << "G:" << (int)_rgb.G << endl;
    cout << "B:" << (int)_rgb.B << endl;
};

int LedLamp::getType() const {
    //Возвращает тип объекта:
    //0 - лампа, 1 - светодиодная лампа
    return 1;
};

double LedLamp::getCurrentIntensity() const {
    //Возвращает текущую интенсивность пропорционально интенсивностям каналов RGB
    return 0.299*_rgb.R+0.587*_rgb.G+0.114*_rgb.B;
};

void LedLamp::setRGBRed(unsigned char intensity) {
    //Устанавливает интенсивность красного канала (0-255)
    setRGBChannel(&LedLamp::RGB::R, intensity);
};

void LedLamp::setRGBGreen(unsigned char intensity) {
    //Устанавливает интенсивность зелёного канала (0-255)
    setRGBChannel(&LedLamp::RGB::G, intensity);
};

void LedLamp::setRGBBlue(unsigned char intensity) {
    //Устанавливает интенсивность синего канала (0-255)
    setRGBChannel(&LedLamp::RGB::B, intensity);
};

unsigned char LedLamp::getRGBRed() const {
    //Возвращает интенсивность красного канала (0-255)
    return getRGBChannel(&LedLamp::RGB::R);
};

unsigned char LedLamp::getRGBBlue() const {
    //Возвращает интенсивность синего канала (0-255)
    return getRGBChannel(&LedLamp::RGB::B);
};

unsigned char LedLamp::getRGBGreen() const {
    //Возвращает интенсивность зелёного канала (0-255)
    return getRGBChannel(&LedLamp::RGB::G);
};

void LedLamp::setRGBChannel(unsigned char LedLamp::RGB::*field, const unsigned char intensity) {
    //Устаналвливает интенсивность одного из каналов RGB
    _rgb.*field = intensity;
};

unsigned char LedLamp::getRGBChannel(const unsigned char LedLamp::RGB::*field) const {
    //Возвращает интенсивность одного из каналов RGB
    return _rgb.*field;
};
