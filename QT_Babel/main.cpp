#include "mainwindow.h"
#include <QApplication>
#include <QWidget>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    std::list<std::string > s;
    w.setListitems(s);
    w.show();

    return a.exec();
}
