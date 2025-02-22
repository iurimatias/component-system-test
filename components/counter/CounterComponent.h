#pragma once

#include "../../mainapp/IComponent.h"
#include <QObject>

class CounterComponent : public QObject, public IComponent {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID IComponent_iid)
    Q_INTERFACES(IComponent)

public:
    QWidget* createWidget() override;
    void destroyWidget(QWidget* widget) override;
}; 