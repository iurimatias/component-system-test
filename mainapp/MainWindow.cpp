#include "MainWindow.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QMenuBar>
#include <QMenu>
#include <QLabel>
#include <QHBoxLayout>
#include <QDir>
#include <QDebug>
#include <ComponentSystem>
#include <QPluginLoader>
#include <QAction>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    m_centralWidget(new QWidget(this)),
    m_layout(new QVBoxLayout()),
    m_buttonLayout(new QVBoxLayout()),
    m_componentLoader(new Nedrysoft::ComponentSystem::ComponentLoader()) {

    setCentralWidget(m_centralWidget);
    m_centralWidget->setLayout(m_layout);

    // Create component names section at the top
    QWidget* componentNamesWidget = new QWidget();
    QVBoxLayout* componentNamesLayout = new QVBoxLayout(componentNamesWidget);
    m_layout->addWidget(componentNamesWidget);

    // Add a header and reload button in a horizontal layout
    QWidget* headerWidget = new QWidget();
    QHBoxLayout* headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(0, 0, 0, 0);

    QLabel* headerLabel = new QLabel("Loaded Components:");
    headerLabel->setStyleSheet("font-weight: bold;");
    headerLayout->addWidget(headerLabel);

    QPushButton* reloadButton = new QPushButton("Reload Components");
    reloadButton->setMaximumWidth(150);
    headerLayout->addWidget(reloadButton);
    headerLayout->addStretch();

    componentNamesLayout->addWidget(headerWidget);

    // Create a container for component entries
    QWidget* entriesContainer = new QWidget();
    QVBoxLayout* entriesLayout = new QVBoxLayout(entriesContainer);
    entriesLayout->setContentsMargins(0, 0, 0, 0);
    componentNamesLayout->addWidget(entriesContainer);

    // Connect reload button
    connect(reloadButton, &QPushButton::clicked, this, [this, entriesLayout]() {
        qDebug() << "Reloading components...";

        // Clear existing component entries
        QLayoutItem* child;
        while ((child = entriesLayout->takeAt(0)) != nullptr) {
            if (child->widget()) {
                child->widget()->deleteLater();
            }
            delete child;
        }

        // Clear existing component widgets
        while ((child = m_widgetLayout->takeAt(0)) != nullptr) {
            if (child->widget()) {
                child->widget()->setParent(nullptr);
                child->widget()->deleteLater();
            }
            delete child;
        }

        // Unload all current components and delete the loader
        m_componentLoader->unloadComponents();
        delete m_componentLoader;
        
        // Create a new component loader
        m_componentLoader = new Nedrysoft::ComponentSystem::ComponentLoader();
        m_loadedComponents.clear();

        // Re-add components directory
        QString componentPath = QDir::cleanPath(QDir::currentPath() + "/components");
        qDebug() << "Current directory:" << QDir::currentPath();
        qDebug() << "Looking for components in:" << componentPath;
        m_componentLoader->addComponents(componentPath);

        // Load all components and create their widgets
        m_componentLoader->loadComponents([this](Nedrysoft::ComponentSystem::Component* component) {
            qDebug() << "Loading component:" << component->name() << "from" << component->filename();
            m_loadedComponents.insert(component->name());
            return true;
        });

        // Re-add component names and buttons
        auto components = m_componentLoader->components();
        qDebug() << "Found" << components.size() << "components";
        for (auto component : components) {
            qDebug() << "Component" << component->name() << "loaded state:" << component->isLoaded();
            if (component->isLoaded()) {
                // Create horizontal layout for component name and buttons
                QWidget* componentWidget = new QWidget();
                QHBoxLayout* componentLayout = new QHBoxLayout(componentWidget);
                componentLayout->setContentsMargins(0, 0, 0, 0);

                // Add component name with filled bullet since it's loaded
                QLabel* nameLabel = new QLabel(QString("• %1").arg(component->name()));
                componentLayout->addWidget(nameLabel);

                // Add load button (initially hidden since component is loaded)
                QPushButton* loadButton = new QPushButton("Load");
                loadButton->setMaximumWidth(100);
                loadButton->hide();
                componentLayout->addWidget(loadButton);

                // Add unload button (initially shown since component is loaded)
                QPushButton* unloadButton = new QPushButton("Unload");
                unloadButton->setMaximumWidth(100);
                unloadButton->show();
                componentLayout->addWidget(unloadButton);

                // Add stretch to keep buttons on the right
                componentLayout->addStretch();

                // Connect load button
                connect(loadButton, &QPushButton::clicked, this, [this, component, loadButton, unloadButton, nameLabel]() {
                    qDebug() << "Loading component:" << component->name();
                    
                    // Create a new plugin loader for this component
                    auto pluginLoader = new QPluginLoader(component->filename());
                    
                    if (!pluginLoader->load()) {
                        qWarning() << "Failed to load component:" << component->name();
                        delete pluginLoader;
                        return;
                    }

                    auto componentInterface = qobject_cast<Nedrysoft::ComponentSystem::IComponent*>(pluginLoader->instance());
                    if (componentInterface) {
                        // Initialize the component
                        componentInterface->initialiseEvent();
                        componentInterface->initialisationFinishedEvent();
                        
                        // Add to loaded components list
                        m_loadedComponents.insert(component->name());
                        
                        // Update button visibility
                        loadButton->hide();
                        unloadButton->show();
                        nameLabel->setText(QString("• %1").arg(component->name()));

                        // Refresh widgets to show the newly loaded component
                        QTimer::singleShot(0, this, &MainWindow::refreshWidgets);
                    } else {
                        qWarning() << "Failed to get component interface for:" << component->name();
                        pluginLoader->unload();
                        delete pluginLoader;
                    }
                });

                // Connect unload button
                connect(unloadButton, &QPushButton::clicked, this, [this, component, componentWidget, loadButton, unloadButton, nameLabel]() {
                    // Find and remove this component's widget from the UI
                    auto widgets = Nedrysoft::ComponentSystem::getObjects<QWidget>();
                    for (auto widget : widgets) {
                        if (widget->property("componentName").toString() == component->name()) {
                            Nedrysoft::ComponentSystem::removeObject(widget);
                            widget->setParent(nullptr);
                            widget->hide();
                            widget->deleteLater();
                        }
                    }

                    // Remove from loaded components list
                    m_loadedComponents.remove(component->name());

                    // Call finaliseEvent on the component to clean it up
                    if (auto componentInterface = qobject_cast<Nedrysoft::ComponentSystem::IComponent*>(
                        QPluginLoader(component->filename()).instance())) {
                        componentInterface->finaliseEvent();
                    }

                    // Update button visibility
                    loadButton->show();
                    unloadButton->hide();
                    nameLabel->setText(QString("○ %1").arg(component->name()));  // Change bullet to empty circle
                });

                entriesLayout->addWidget(componentWidget);
            }
        }

        entriesLayout->addStretch();  // Add stretch to keep entries at top
        
        // Refresh widgets to show newly loaded components
        QTimer::singleShot(100, this, &MainWindow::refreshWidgets);  // Increased delay to ensure components are ready
    });

    // Create widget container for component widgets
    m_widgetContainer = new QWidget();
    m_widgetLayout = new QVBoxLayout(m_widgetContainer);
    m_layout->addWidget(m_widgetContainer);

    // Store loaded components state
    m_loadedComponents.clear();

    // Add components directory
    QString componentPath = QDir::cleanPath(QDir::currentPath() + "/build/bin/components");
    qDebug() << "Looking for components in:" << componentPath;
    m_componentLoader->addComponents(componentPath);

    // Load all components by default
    m_componentLoader->loadComponents([this](Nedrysoft::ComponentSystem::Component* component) {
        qDebug() << "Loading component:" << component->name();
        m_loadedComponents.insert(component->name());
        return true;  // Load all components
    });

    // Add component names and buttons
    auto components = m_componentLoader->components();
    for (auto component : components) {
        if (component->isLoaded()) {
            // Create horizontal layout for component name and buttons
            QWidget* componentWidget = new QWidget();
            QHBoxLayout* componentLayout = new QHBoxLayout(componentWidget);
            componentLayout->setContentsMargins(0, 0, 0, 0);

            // Add component name
            QLabel* nameLabel = new QLabel(QString("• %1").arg(component->name()));
            componentLayout->addWidget(nameLabel);

            // Add load button (initially hidden)
            QPushButton* loadButton = new QPushButton("Load");
            loadButton->setMaximumWidth(100);
            loadButton->hide();  // Initially hidden since component is loaded
            componentLayout->addWidget(loadButton);

            // Add unload button
            QPushButton* unloadButton = new QPushButton("Unload");
            unloadButton->setMaximumWidth(100);
            componentLayout->addWidget(unloadButton);

            // Add stretch to keep buttons on the right
            componentLayout->addStretch();

            // Connect load button
            connect(loadButton, &QPushButton::clicked, this, [this, component, loadButton, unloadButton, nameLabel]() {
                qDebug() << "Loading component:" << component->name();
                
                // Create a new plugin loader for this component
                auto pluginLoader = new QPluginLoader(component->filename());
                
                if (!pluginLoader->load()) {
                    qWarning() << "Failed to load component:" << component->name();
                    delete pluginLoader;
                    return;
                }

                auto componentInterface = qobject_cast<Nedrysoft::ComponentSystem::IComponent*>(pluginLoader->instance());
                if (componentInterface) {
                    // Initialize the component
                    componentInterface->initialiseEvent();
                    componentInterface->initialisationFinishedEvent();
                    
                    // Add to loaded components list
                    m_loadedComponents.insert(component->name());
                    
                    // Update button visibility
                    loadButton->hide();
                    unloadButton->show();
                    nameLabel->setText(QString("• %1").arg(component->name()));

                    // Refresh widgets to show the newly loaded component
                    QTimer::singleShot(0, this, &MainWindow::refreshWidgets);
                } else {
                    qWarning() << "Failed to get component interface for:" << component->name();
                    pluginLoader->unload();
                    delete pluginLoader;
                }
            });

            // Connect unload button
            connect(unloadButton, &QPushButton::clicked, this, [this, component, componentWidget, loadButton, unloadButton, nameLabel]() {
                // Find and remove this component's widget from the UI
                auto widgets = Nedrysoft::ComponentSystem::getObjects<QWidget>();
                for (auto widget : widgets) {
                    if (widget->property("componentName").toString() == component->name()) {
                        Nedrysoft::ComponentSystem::removeObject(widget);
                        widget->setParent(nullptr);
                        widget->hide();
                        widget->deleteLater();
                    }
                }

                // Remove from loaded components list
                m_loadedComponents.remove(component->name());

                // Call finaliseEvent on the component to clean it up
                if (auto componentInterface = qobject_cast<Nedrysoft::ComponentSystem::IComponent*>(
                    QPluginLoader(component->filename()).instance())) {
                    componentInterface->finaliseEvent();
                }

                // Update button visibility
                loadButton->show();
                unloadButton->hide();
                nameLabel->setText(QString("○ %1").arg(component->name()));  // Change bullet to empty circle
            });

            entriesLayout->addWidget(componentWidget);
        }
    }

    entriesLayout->addStretch();  // Add stretch to keep entries at top

    // Give components a chance to initialize and create their widgets
    QTimer::singleShot(0, this, &MainWindow::refreshWidgets);
}

MainWindow::~MainWindow() {
    // Clear widgets before unloading components
    QLayoutItem *child;
    while ((child = m_widgetLayout->takeAt(0)) != nullptr) {
        if (child->widget()) {
            child->widget()->setParent(nullptr);  // Remove parent to prevent double deletion
        }
        delete child;
    }

    // Now unload components
    m_componentLoader->unloadComponents();
    delete m_componentLoader;
}

void MainWindow::onLoadComponent(const QString &componentName) {
    // Not used for now
}

void MainWindow::onUnloadComponent(const QString &componentName) {
    // Not used for now
}

void MainWindow::updateComponentButtons() {
    // Not used for now
}

void MainWindow::refreshWidgets() {
    // Clear existing widgets from widget container
    QLayoutItem *child;
    while ((child = m_widgetLayout->takeAt(0)) != nullptr) {
        if (child->widget()) {
            child->widget()->setParent(nullptr);  // Remove parent to prevent double deletion
        }
        delete child;
    }

    // Get all registered widgets from the component system
    auto widgets = Nedrysoft::ComponentSystem::getObjects<QWidget>();
    for (auto widget : widgets) {
        if (widget->parent() == nullptr) {  // Only handle top-level widgets
            widget->setParent(m_widgetContainer);
            m_widgetLayout->addWidget(widget);
            widget->show();
        }
    }
} 