#include "ColorChangerComponent.h"
#include "ColorChangerWidget.h"
#include <IComponentManager>

ColorChangerComponent::ColorChangerComponent() : m_widget(nullptr) {
}

ColorChangerComponent::~ColorChangerComponent() {
    // Widget cleanup is handled in finaliseEvent
}

auto ColorChangerComponent::initialiseEvent() -> void {
    m_widget = new ColorChangerWidget();
    m_widget->setAttribute(Qt::WA_DeleteOnClose);  // Ensure widget is deleted when closed
    m_widget->setProperty("componentName", "ColorChanger");  // Set component name property
    Nedrysoft::ComponentSystem::addObject(m_widget);
}

auto ColorChangerComponent::initialisationFinishedEvent() -> void {
    // Nothing to do here
}

auto ColorChangerComponent::finaliseEvent() -> void {
    if (m_widget) {
        Nedrysoft::ComponentSystem::removeObject(m_widget);
        m_widget->deleteLater();  // Schedule deletion safely
        m_widget = nullptr;
    }
} 