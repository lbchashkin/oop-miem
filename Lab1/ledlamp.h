#ifndef LEDLAMP_H
#define LEDLAMP_H
#include "lamp.h"


class LedLamp: public Lamp {
public:
    static const unsigned char RGB_DEFAULT;
    LedLamp();
    LedLamp(int power, int max_intensity, unsigned char R,
            unsigned char G, unsigned char B);
    LedLamp(LedLamp &lamp);
    virtual void printParams() const;
    virtual int getType() const;
    double getCurrentIntensity() const;
    void setRGBRed(const unsigned char intensity);
    void setRGBBlue(const unsigned char intensity);
    void setRGBGreen(const unsigned char intensity);
    unsigned char getRGBRed() const;
    unsigned char getRGBBlue() const;
    unsigned char getRGBGreen() const;
    virtual ~LedLamp() {};
private:
    struct RGB {
        unsigned char R;
        unsigned char G;
        unsigned char B;
    };
    void setRGBChannel(unsigned char LedLamp::RGB::*field, const unsigned char intensity);
    unsigned char getRGBChannel(const unsigned char LedLamp::RGB::*field) const;
    RGB _rgb;
};

#endif // LEDLAMP_H
