#ifndef NEDRYSOFT_COLORCHANGER_COLORCHANGERCOMPONENT_H
#define NEDRYSOFT_COLORCHANGER_COLORCHANGERCOMPONENT_H

#include "ColorChangerSpec.h"
#include <QObject>
#include <IComponent>

class NEDRYSOFT_COLORCHANGER_DLLSPEC ColorChangerComponent :
        public QObject,
        public Nedrysoft::ComponentSystem::IComponent {

    private:
        Q_OBJECT
        Q_PLUGIN_METADATA(IID NedrysoftComponentInterfaceIID FILE "metadata.json")
        Q_INTERFACES(Nedrysoft::ComponentSystem::IComponent)

    public:
        /**
         * @brief       Constructs the ColorChangerComponent.
         */
        ColorChangerComponent();

        /**
         * @brief       Destroys the ColorChangerComponent.
         */
        ~ColorChangerComponent();

    public:
        /**
         * @brief       The initialiseEvent is called by the component loader to initialise the component.
         *
         * @details     Called by the component loader after all components have been loaded, called in load order.
         *
         * @see         Nedrysoft::ComponentSystem::IComponent::initialiseEvent
         */
        auto initialiseEvent() -> void override;

        /**
         * @brief       The initialisationFinishedEvent function is called by the component loader after all
         *              components have been initialised.
         *
         * @details     Called by the component loader after all components have been
         *              initialised, called in reverse load order.
         *
         * @see         Nedrysoft::ComponentSystem::IComponent::initialisationFinishedEvent
         */
        auto initialisationFinishedEvent() -> void override;

        /**
         * @brief       The finaliseEvent method is called before the component is unloaded.
         *
         * @details     Called by the component loader in reverse load order to shutdown the component.
         *
         * @see         Nedrysoft::ComponentSystem::IComponent::finaliseEvent
         */
        auto finaliseEvent() -> void override;

    private:
        QWidget* m_widget;
};

#endif // NEDRYSOFT_COLORCHANGER_COLORCHANGERCOMPONENT_H 