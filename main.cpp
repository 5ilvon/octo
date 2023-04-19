#include "themewidget.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>

#include "NetworkManagerWrapper.hpp"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    //QMainWindow window;
    //ThemeWidget *widget = new ThemeWidget();
    //window.setCentralWidget(widget);
    //window.resize(900, 600);
    //window.show();

    auto network = new NetworkManagerWrapper();

    //return 0;
    return a.exec();
}