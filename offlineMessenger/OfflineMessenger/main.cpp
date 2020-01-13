#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString s = QString("[Online] cheloo");
    s.remove(0, strlen("[Online] "));
    printf("111%s222", s.toStdString().c_str());
    fflush(stdout);
    MainWindow w;
    w.show();

    return a.exec();
}
