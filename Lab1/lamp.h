#ifndef LAMP_H
#define LAMP_H


class Lamp
{
public:
    static const int POWER_MIN;
    static const int POWER_MAX;
    static const int POWER_DEFAULT;
    static const int INTENSITY_MIN;
    static const int INTENSITY_MAX;
    static const int INTENSITY_DEFAULT;
    Lamp(const int power=POWER_DEFAULT, const int intensity=INTENSITY_DEFAULT);
    Lamp(const Lamp &lamp);
    virtual void printParams() const;
    virtual int getType() const;
    int getPower() const;
    int getIntensity() const;
    void setPower(const int power);
    void setIntensity(const int intensity);
    virtual ~Lamp() {};
private:
    int power_, intensity_;
};

#endif // LAMP_H
