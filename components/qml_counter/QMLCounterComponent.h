#pragma once

#include <QObject>
#include <QWidget>
#include <QQuickWidget>
#include <QtPlugin>
#include <interfaces/IComponent.h>

class QMLCounterComponent : public QObject, public IComponent {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.logos.component.IComponent")
    Q_INTERFACES(IComponent)

public:
    explicit QMLCounterComponent(QObject* parent = nullptr);
    ~QMLCounterComponent() override;

    QWidget* createWidget() override;
    void destroyWidget(QWidget* widget) override;
}; 