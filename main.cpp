//Autore: Marco Martini 1189321
#include "eegplayer.h"
#include "chart.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    EEGPlayer w;
    w.show();
    return a.exec();
}


