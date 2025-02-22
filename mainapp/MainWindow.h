#pragma once

#include <QMainWindow>
#include <QMap>

class QVBoxLayout;
class IComponent;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void onLoadCounter();
    void onUnloadCounter();
    void onLoadColorChanger();
    void onUnloadColorChanger();

private:
    QString getPluginPath(const QString& name);
    QMap<QString, IComponent*> loadedComponents;
    QMap<QString, QWidget*> componentWidgets;
    QVBoxLayout* layout;
    QWidget* centralWidget;
}; 