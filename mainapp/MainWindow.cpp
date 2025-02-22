#include "MainWindow.h"
#include "IComponent.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QPluginLoader>
#include <QDir>
#include <QDebug>
#include <QCoreApplication>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    // Set up central widget and layout
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    layout = new QVBoxLayout(centralWidget);

    // Create buttons for loading and unloading components
    QPushButton* loadCounterButton = new QPushButton("Load Counter", this);
    QPushButton* unloadCounterButton = new QPushButton("Unload Counter", this);
    QPushButton* loadColorChangerButton = new QPushButton("Load Color Changer", this);
    QPushButton* unloadColorChangerButton = new QPushButton("Unload Color Changer", this);

    // Add buttons to layout
    QVBoxLayout* buttonLayout = new QVBoxLayout();
    buttonLayout->addWidget(loadCounterButton);
    buttonLayout->addWidget(unloadCounterButton);
    buttonLayout->addWidget(loadColorChangerButton);
    buttonLayout->addWidget(unloadColorChangerButton);
    layout->addLayout(buttonLayout);
    layout->addStretch(); // Push components to top

    // Connect buttons to slots
    connect(loadCounterButton, &QPushButton::clicked, this, &MainWindow::onLoadCounter);
    connect(unloadCounterButton, &QPushButton::clicked, this, &MainWindow::onUnloadCounter);
    connect(loadColorChangerButton, &QPushButton::clicked, this, &MainWindow::onLoadColorChanger);
    connect(unloadColorChangerButton, &QPushButton::clicked, this, &MainWindow::onUnloadColorChanger);
}

QString MainWindow::getPluginPath(const QString& name) {
    QString pluginPath = QDir::currentPath() + "/plugins/";
#ifdef Q_OS_MAC
    return pluginPath + name + ".dylib";
#else
    return pluginPath + "lib" + name + ".so";
#endif
}

void MainWindow::onLoadCounter() {
    if (!loadedComponents.contains("counter")) {
        QString pluginPath = getPluginPath("counter");
        qDebug() << "Loading plugin from:" << pluginPath;
        QPluginLoader loader(pluginPath);
        QObject* plugin = loader.instance();
        
        if (plugin) {
            IComponent* component = qobject_cast<IComponent*>(plugin);
            if (component) {
                QWidget* widget = component->createWidget();
                layout->addWidget(widget);
                loadedComponents["counter"] = component;
                componentWidgets["counter"] = widget;
            }
        } else {
            qDebug() << "Failed to load counter plugin:" << loader.errorString();
        }
    }
}

void MainWindow::onUnloadCounter() {
    if (loadedComponents.contains("counter")) {
        QWidget* widget = componentWidgets["counter"];
        layout->removeWidget(widget);
        loadedComponents["counter"]->destroyWidget(widget);
        componentWidgets.remove("counter");
        
        QString pluginPath = getPluginPath("counter");
        QPluginLoader loader(pluginPath);
        loader.unload();
        
        loadedComponents.remove("counter");
    }
}

void MainWindow::onLoadColorChanger() {
    if (!loadedComponents.contains("colorchanger")) {
        QString pluginPath = getPluginPath("colorchanger");
        qDebug() << "Loading plugin from:" << pluginPath;
        QPluginLoader loader(pluginPath);
        QObject* plugin = loader.instance();
        
        if (plugin) {
            IComponent* component = qobject_cast<IComponent*>(plugin);
            if (component) {
                QWidget* widget = component->createWidget();
                layout->addWidget(widget);
                loadedComponents["colorchanger"] = component;
                componentWidgets["colorchanger"] = widget;
            }
        } else {
            qDebug() << "Failed to load color changer plugin:" << loader.errorString();
        }
    }
}

void MainWindow::onUnloadColorChanger() {
    if (loadedComponents.contains("colorchanger")) {
        QWidget* widget = componentWidgets["colorchanger"];
        layout->removeWidget(widget);
        loadedComponents["colorchanger"]->destroyWidget(widget);
        componentWidgets.remove("colorchanger");
        
        QString pluginPath = getPluginPath("colorchanger");
        QPluginLoader loader(pluginPath);
        loader.unload();
        
        loadedComponents.remove("colorchanger");
    }
} 