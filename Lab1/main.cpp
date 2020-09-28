#include <iostream>
#include "tests.h"
#include "lamp.h"
#include "lampcollection.h"

using namespace std;

int main()
{
    //Тестирование классов и пример использования
    testLamps();
    testLampCollection();
    Lamp lamp(60, 90);
    lamp.printParams();
    lamp.setPower(80);
    lamp.setIntensity(120);
    lamp.printParams();
    LampCollection room(3, 3);
    room.setLamp(0, 1, lamp);
    room.setLamp(1, 2, lamp);
    cout << "Print LampCollection:" << endl;
    printLampCollection(room);
    cout << "Illuminance: " << room.getIlluminance(1.2, 2.3, 1.9, 2) << endl;
    return 0;
}
