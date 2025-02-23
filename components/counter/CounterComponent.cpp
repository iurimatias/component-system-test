#include "CounterComponent.h"
#include "CounterWidget.h"
#include <IComponentManager>

CounterComponent::CounterComponent() : m_widget(nullptr) {
}

CounterComponent::~CounterComponent() {
    // Widget cleanup is handled in finaliseEvent
}

auto CounterComponent::initialiseEvent() -> void {
    m_widget = new CounterWidget();
    m_widget->setAttribute(Qt::WA_DeleteOnClose);  // Ensure widget is deleted when closed
    m_widget->setProperty("componentName", "Counter");  // Set component name property
    Nedrysoft::ComponentSystem::addObject(m_widget);
}

auto CounterComponent::initialisationFinishedEvent() -> void {
    // Nothing to do here
}

auto CounterComponent::finaliseEvent() -> void {
    if (m_widget) {
        Nedrysoft::ComponentSystem::removeObject(m_widget);
        m_widget->deleteLater();  // Schedule deletion safely
        m_widget = nullptr;
    }
} 