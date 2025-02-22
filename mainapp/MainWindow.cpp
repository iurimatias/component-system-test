#include "MainWindow.h"
#include "IComponent.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QPluginLoader>
#include <QDir>
#include <QDebug>
#include <QCoreApplication>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    // Set up central widget and layout
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    layout = new QVBoxLayout(centralWidget);

    // Create reload button at the top
    QHBoxLayout* topLayout = new QHBoxLayout();
    QPushButton* reloadButton = new QPushButton("Reload Plugins", this);
    topLayout->addWidget(reloadButton);
    topLayout->addStretch();
    layout->addLayout(topLayout);

    // Create a layout for component buttons
    buttonLayout = new QVBoxLayout();
    layout->addLayout(buttonLayout);
    layout->addStretch(); // Push components to top

    // Connect reload button
    connect(reloadButton, &QPushButton::clicked, this, &MainWindow::onReloadPlugins);

    // Initial plugin setup
    setupPluginButtons(buttonLayout);
}

void MainWindow::clearPluginButtons() {
    // Delete all existing buttons and clear maps
    for (auto button : loadButtons.values()) {
        button->deleteLater();
    }
    for (auto button : unloadButtons.values()) {
        button->deleteLater();
    }
    loadButtons.clear();
    unloadButtons.clear();
}

void MainWindow::setupPluginButtons(QVBoxLayout* buttonLayout) {
    // Find and create buttons for available plugins
    QStringList plugins = findAvailablePlugins();
    for (const QString& plugin : plugins) {
        QPushButton* loadButton = new QPushButton("Load " + plugin, this);
        QPushButton* unloadButton = new QPushButton("Unload " + plugin, this);
        
        // Store buttons for later use
        loadButtons[plugin] = loadButton;
        unloadButtons[plugin] = unloadButton;
        
        // Add buttons to layout
        buttonLayout->addWidget(loadButton);
        buttonLayout->addWidget(unloadButton);
        
        // Connect buttons to slots using lambda functions to pass the plugin name
        connect(loadButton, &QPushButton::clicked, this, [this, plugin]() {
            onLoadComponent(plugin);
        });
        connect(unloadButton, &QPushButton::clicked, this, [this, plugin]() {
            onUnloadComponent(plugin);
        });
        
        // Set initial button states
        loadButton->setEnabled(!loadedComponents.contains(plugin));
        unloadButton->setEnabled(loadedComponents.contains(plugin));
    }
}

void MainWindow::onReloadPlugins() {
    QStringList currentPlugins = loadedComponents.keys();
    
    // Unload all currently loaded plugins
    for (const QString& plugin : currentPlugins) {
        onUnloadComponent(plugin);
    }
    
    // Clear existing buttons
    clearPluginButtons();
    
    // Set up new buttons
    setupPluginButtons(buttonLayout);
}

QStringList MainWindow::findAvailablePlugins() {
    QStringList result;
    QDir pluginsDir(QDir::currentPath() + "/plugins");
    
    // List all files in the plugins directory
    QStringList entries = pluginsDir.entryList(QDir::Files);
    
    // Filter and clean up plugin names
    for (const QString& entry : entries) {
        QString baseName = entry;
#ifdef Q_OS_MAC
        if (baseName.endsWith(".dylib")) {
            baseName.chop(6); // Remove .dylib
            result.append(baseName);
        }
#else
        if (baseName.startsWith("lib") && baseName.endsWith(".so")) {
            baseName.remove(0, 3); // Remove lib
            baseName.chop(3);      // Remove .so
            result.append(baseName);
        }
#endif
    }
    
    return result;
}

QString MainWindow::getPluginPath(const QString& name) {
    QString pluginPath = QDir::currentPath() + "/plugins/";
#ifdef Q_OS_MAC
    return pluginPath + name + ".dylib";
#else
    return pluginPath + "lib" + name + ".so";
#endif
}

void MainWindow::onLoadComponent(const QString& name) {
    if (!loadedComponents.contains(name)) {
        QString pluginPath = getPluginPath(name);
        qDebug() << "Loading plugin from:" << pluginPath;
        QPluginLoader loader(pluginPath);
        QObject* plugin = loader.instance();
        
        if (plugin) {
            IComponent* component = qobject_cast<IComponent*>(plugin);
            if (component) {
                QWidget* widget = component->createWidget();
                layout->addWidget(widget);
                loadedComponents[name] = component;
                componentWidgets[name] = widget;
                
                // Update button states
                loadButtons[name]->setEnabled(false);
                unloadButtons[name]->setEnabled(true);
            }
        } else {
            qDebug() << "Failed to load plugin:" << name << "-" << loader.errorString();
        }
    }
}

void MainWindow::onUnloadComponent(const QString& name) {
    if (loadedComponents.contains(name)) {
        QWidget* widget = componentWidgets[name];
        layout->removeWidget(widget);
        loadedComponents[name]->destroyWidget(widget);
        componentWidgets.remove(name);
        
        QString pluginPath = getPluginPath(name);
        QPluginLoader loader(pluginPath);
        loader.unload();
        
        loadedComponents.remove(name);
        
        // Update button states if the buttons still exist
        if (loadButtons.contains(name)) {
            loadButtons[name]->setEnabled(true);
            unloadButtons[name]->setEnabled(false);
        }
    }
} 