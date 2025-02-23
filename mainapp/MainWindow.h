#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMap>
#include <QSet>
#include "Component.h"
#include "ComponentLoader.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onLoadComponent(const QString &componentName);
    void onUnloadComponent(const QString &componentName);
    void updateComponentButtons();
    void refreshWidgets();

private:
    QWidget *m_centralWidget;
    QVBoxLayout *m_layout;
    QVBoxLayout *m_buttonLayout;
    QWidget *m_widgetContainer;
    QVBoxLayout *m_widgetLayout;
    Nedrysoft::ComponentSystem::ComponentLoader *m_componentLoader;
    QSet<QString> m_loadedComponents;  // Track which components are loaded
};

#endif // MAINWINDOW_H 