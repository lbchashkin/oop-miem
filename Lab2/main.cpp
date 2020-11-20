#include "mainwindow.h"
#include <iostream>
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator qtLanguageTranslator;
    qtLanguageTranslator.load(QString("QtLanguage_ru"));
    qApp->installTranslator(&qtLanguageTranslator);
    MainWindow w;
    w.show();
    return a.exec();
}
