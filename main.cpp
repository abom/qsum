#include <QApplication>
//#include <QTranslator>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //QTranslator translator;
    //translator.load("tr/qsum_ar.qm");
    //a.installTranslator(&translator);
    //a.setLayoutDirection(Qt::RightToLeft);
    MainWindow w;
    w.show();
    
    return a.exec();
}
