#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Create and show the main window
    MainWindow window;
    window.resize(400, 300);
    window.show();

    return app.exec();
} 