#include <tests.h>
#include <lamp.h>

int main()
{
    //Тестирование класса и пример использования
    testLamps();
    Lamp lamp(60, 90);
    lamp.printParams();
    lamp.setPower(80);
    lamp.setIntensity(120);
    lamp.printParams();
    return 0;
}
