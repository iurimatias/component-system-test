#pragma once

#include <QMainWindow>
#include <QMap>

class QVBoxLayout;
class IComponent;
class QPushButton;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void onLoadComponent(const QString& name);
    void onUnloadComponent(const QString& name);

private:
    QString getPluginPath(const QString& name);
    QStringList findAvailablePlugins();
    QPushButton* createComponentButtons(const QString& componentName);
    
    QMap<QString, IComponent*> loadedComponents;
    QMap<QString, QWidget*> componentWidgets;
    QMap<QString, QPushButton*> loadButtons;
    QMap<QString, QPushButton*> unloadButtons;
    QVBoxLayout* layout;
    QWidget* centralWidget;
}; 