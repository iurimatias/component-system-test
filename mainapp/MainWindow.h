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
    void onReloadPlugins();

private:
    QString getPluginPath(const QString& name);
    QStringList findAvailablePlugins();
    void setupPluginButtons(QVBoxLayout* buttonLayout);
    void clearPluginButtons();
    
    QMap<QString, IComponent*> loadedComponents;
    QMap<QString, QWidget*> componentWidgets;
    QMap<QString, QPushButton*> loadButtons;
    QMap<QString, QPushButton*> unloadButtons;
    QVBoxLayout* layout;
    QVBoxLayout* buttonLayout;  // Store buttonLayout for reuse
    QWidget* centralWidget;
}; 