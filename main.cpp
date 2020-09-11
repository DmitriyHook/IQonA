#include "mainwindow.h"
#include <QApplication>
#include <QObject>
#include "iqon.h"

int main(int argc, char *argv[])
{
    bool fullScreen = false;

    for(int i = 1; i < argc; i++)
    {
        if(QString(argv[i]) == "fullscreen") fullScreen = true;
    }

    QApplication a(argc, argv);

    MainWindow mainWindow;
    IQon iqon;

    QObject::connect(&mainWindow, &MainWindow::countChannals_changed, &iqon, &IQon::setCountChannals);
    QObject::connect(&iqon, &IQon::countChannals_changed, &mainWindow, &MainWindow::setIQVisualizer);

    iqon.setCountChannals(1);

    if(fullScreen) mainWindow.showFullScreen();
    else mainWindow.show();


    return a.exec();
}
