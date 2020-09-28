#ifndef LAMPCOLLECTION_H
#define LAMPCOLLECTION_H
#include "lamp.h"
#include <QString>

class LampCollection {
public:
    static const float HEIGHT_MIN;
    static const float HEIGHT_DEFAULT;
    static const float HEIGHT_MAX;
    LampCollection(const int n, const int m);
    LampCollection(const LampCollection &collection);
    double getIlluminance(const float x, const float y, const float z, const float height = HEIGHT_DEFAULT) const;
    void setLamp(const int x, const int y, Lamp &lamp);
    Lamp &getLamp(const int x, const int y) const;
    int getLength() const;
    int getWidth() const;
    int getNumberOfLamps() const;
    void printLamps() const;
    bool isEqual(const LampCollection &collection) const;
    ~LampCollection();
private:
    Lamp **collection_;
    int n_, m_;
};
void printLampCollection(const LampCollection &collection);
bool LampCollectionToJson(const QString &string, const LampCollection &collection);
LampCollection& LampCollectionFromJson(const QString &string);

#endif // LAMPCOLLECTION_H
