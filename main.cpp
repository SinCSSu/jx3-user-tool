#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    if(w.show_flag)
    {
        w.show();
        return a.exec();
    }
    return 0;
}
