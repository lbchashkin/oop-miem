#ifndef LAMP_H
#define LAMP_H


class Lamp
{
public:
    static const int POWER_MIN = 20;
    static const int POWER_MAX = 200;
    static const int POWER_DEFAULT = 60;
    static const int INTENSITY_MIN = 50;
    static const int INTENSITY_MAX = 500;
    static const int INTENSITY_DEFAULT = 100;
    Lamp(const int power=POWER_DEFAULT, const int intensity=INTENSITY_DEFAULT);
    Lamp(const Lamp &lamp);
    void printParams() const;
    int getPower() const;
    int getIntensity() const;
    void setPower(const int power);
    void setIntensity(const int intensity);
private:
    int power_, intensity_;
};

#endif // LAMP_H
