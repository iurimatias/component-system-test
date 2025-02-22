#include "MainWindow.h"
#include <QApplication>
// #include <QQuickWindow>

int main(int argc, char *argv[]) {
    // Set up high DPI scaling
    // QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGLRhi);
    QApplication app(argc, argv);
    MainWindow window;
    window.resize(400, 300);
    window.show();
    return app.exec();
} 