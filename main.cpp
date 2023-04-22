#include "themewidget.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    QMainWindow window;
    auto widget = new ThemeWidget();
    window.setCentralWidget(widget);
    window.resize(800, 600);
    window.show();

    return QApplication::exec();
}
