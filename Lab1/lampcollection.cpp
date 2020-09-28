#include <math.h>
#include <iostream>
#include <QString>
#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QDebug>
#include <exception>
#include "lampcollection.h"

using namespace std;

const float LampCollection::HEIGHT_MIN = 2;
const float LampCollection::HEIGHT_DEFAULT = 2.6;
const float LampCollection::HEIGHT_MAX = 3;

LampCollection::LampCollection(const int n, const int m) {
    //Конструктор инициализации (создаёт матрицу Ламп n*m)
    //При n < 1 или m < 1 бросает исключение out_of_range
    if (n > 0 and m > 0) {
        collection_ = new Lamp *[n];
        for (int i = 0; i < n; i++)
            collection_[i] = new Lamp [m];
        n_ = n;
        m_ = m;
    }
    else
        throw out_of_range("n and m should be positive numbers");
};

LampCollection::LampCollection(const LampCollection &collection) {
    //Конструктор копирования
    n_ = collection.getLength();
    m_ = collection.getWidth();
    collection_ = new Lamp *[n_];
    for (int i = 0; i < n_; i++) {
        collection_[i] = new Lamp [m_];
        for (int j = 0; j < m_; j ++)
            collection_[i][j] = collection.getLamp(i, j);
    }
};

Lamp &LampCollection::getLamp(const int x, const int y) const {
    //Возвращает лампу по индексам в массиве
    //При неверных индексах бросает исключение out_of_range
    if ((x >= 0) && (x < n_) && (y >= 0) && (y < m_))
        return collection_[x][y];
    else
        throw out_of_range("index out of range");
};

void LampCollection::setLamp(const int x, const int y, Lamp &lamp) {
    //Устанавливает лампу на определённое место по индексам в матрице
    //При неверных индексах бросает исключение out_of_range
    if ((x >= 0) && (x < n_) && (y >= 0) && (y < m_)) {
        collection_[x][y].~Lamp();
        collection_[x][y] = Lamp(lamp);
    }
    else
        throw out_of_range("index out of range");
};

int LampCollection::getLength() const {
    //Возвращает длину массива
    return n_;
};

int LampCollection::getWidth() const {
    //Возвращает ширину массива
    return m_;
};

int LampCollection::getNumberOfLamps() const {
    //Возвращает количество ламп в коллекции
    return n_*m_;
};

void printLampCollection(const LampCollection &collection) {
    //Выводит на экран содержимое коллекции
    int n = collection.getLength();
    int m = collection.getWidth();
    for (int i=0; i<n; i++) {
        for (int j=0; j<m; j++) {
            cout << "Lamp #" << i << "," << j << endl;
            collection.getLamp(i, j).printParams();
        }
        cout << endl;
    }
};

LampCollection& LampCollectionFromJson(const QString &string) {
    //Читает коллекцию из файла json
    QFile file(string);
    if (file.open(QFile::ReadOnly)) {
        QJsonDocument json_doc;
        json_doc = QJsonDocument::fromJson(file.readAll());
        QJsonArray matrix = json_doc.array();
        QJsonObject size = matrix[0].toObject();
        int n = size["n"].toInt();
        int m = size["m"].toInt();
        static LampCollection collection(n, m);
        for (int i=0; i<n; i++) {
            QJsonArray matrix_string = matrix[i+1].toArray();
            for (int j=0; j<m; j++) {
                QJsonObject lamp_object = matrix_string[j].toObject();
                Lamp lamp(lamp_object["Power"].toInt(), lamp_object["Intensity"].toInt());
                collection.setLamp(i, j, lamp);
            }
        }
        return collection;
    }
    else
        throw QJsonParseError();
};

bool LampCollectionToJson(const QString &string, const LampCollection &collection) {
    //Записывает содержимое коллекции в файл формата json
    QFile file(string);
    if (file.open(QFile::WriteOnly)) {
        int n = collection.getLength();
        int m = collection.getWidth();
        QJsonArray matrix;
        QJsonObject size;
        size.insert("n", n);
        size.insert("m", m);
        matrix.append(size);
        for (int i=0; i<n; i++) {
            QJsonArray matrix_string;
            for (int j=0; j<m; j++) {
                QJsonObject lamp;
                lamp.insert("Power", collection.getLamp(i, j).getPower());
                lamp.insert("Intensity", collection.getLamp(i, j).getIntensity());
                matrix_string.append(lamp);
            }
            matrix.append(matrix_string);
        }
        QJsonDocument json_doc;
        json_doc.setArray(matrix);
        if (file.write(json_doc.toJson()) == -1)
            return false;
        file.close();
        return true;
    }
    else
        return false;
};

bool LampCollection::isEqual(const LampCollection &collection) const {
    //Проверка на равенство 2 коллекций
    if (collection.getLength() == n_ && collection.getWidth() == m_) {
        for (int i=0; i<n_; i++)
            for (int j=0; j<m_; j++)
            {
                Lamp lamp1 = collection_[i][j];
                Lamp lamp2 = collection.getLamp(i, j);
                if (lamp1.getPower() != lamp2.getPower())
                    return false;
                if (lamp1.getIntensity() != lamp2.getIntensity())
                    return false;
            }
    }
    else
        return false;
    return true;
};

double LampCollection::getIlluminance(const float x, const float y, const float z, const float height) const {
    /*Возвращает освещённость от ламп в точке комнаты по координатом x, y, z
    height - высота потолка в комнате (ограничена константами HEIGHT_MIN и HEIGHT_MAX)
    Если координаты выходят за пределы, то освещённость равна 0
    Пределы:
    0 <= x < n;
    0 <= y < m;
    0 <= z < height;
    */
double illuminance = 0;
    bool is_z_good = (z < height) && (z >= 0);
    bool is_x_good = (x < n_) && (x >= 0);
    bool is_y_good = (y < m_) && (y >= 0);
    bool is_height_good = (height >= HEIGHT_MIN) && (height <= HEIGHT_MAX);
    if (is_x_good && is_y_good && is_z_good && is_height_good)
        for (int i=0; i<n_; i++)
            for (int j=0; j<m_; j++)
            {
                double r = sqrt((x-i)*(x-i) + (y-j)*(y-j) + (height-z)*(height-z));
                double cos_a = (height - z)/r;
                illuminance += collection_[i][j].getIntensity()*cos_a/(r*r);
            }
    return illuminance;
};

LampCollection::~LampCollection() {
    //Деструктор
    if (collection_) {
        for (int i = 0; i < n_; i++) {
            delete [] collection_[i];
            collection_[i] = NULL;
        }
        delete [] collection_;
        collection_ = NULL;
    };
};


