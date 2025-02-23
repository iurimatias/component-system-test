#include "QMLCounterComponent.h"
#include <QQuickWidget>
#include <QUrl>

QMLCounterComponent::QMLCounterComponent(QObject* parent)
    : QObject(parent)
{
}

QMLCounterComponent::~QMLCounterComponent()
{
}

void QMLCounterComponent::initialize() {
    // Any initialization code if needed
}

QQuickWidget* QMLCounterComponent::createWidget() {
    QQuickWidget* view = new QQuickWidget();
    view->setResizeMode(QQuickWidget::SizeRootObjectToView);
    view->setSource(QUrl("qrc:/counter.qml"));
    view->setMinimumSize(200, 150);
    return view;
}

void QMLCounterComponent::destroyWidget(QQuickWidget* widget) {
    delete widget;
} 