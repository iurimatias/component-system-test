#pragma once

#include <QObject>
#include <QQuickWidget>
#include <QtPlugin>
#include "IComponent.h"

class QMLCounterComponent : public QObject, public Nedrysoft::ComponentSystem::IComponent {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID NedrysoftComponentInterfaceIID FILE "metadata.json")
    Q_INTERFACES(Nedrysoft::ComponentSystem::IComponent)

public:
    explicit QMLCounterComponent(QObject* parent = nullptr);
    ~QMLCounterComponent() override;

    void initialiseEvent() override;
    void initialisationFinishedEvent() override;

private:
    QQuickWidget* createWidget();
    void destroyWidget(QQuickWidget* widget);
}; 