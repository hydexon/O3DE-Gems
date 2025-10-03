
#include "MultiImporterSystemComponent.h"

#include <MultiImporter/MultiImporterTypeIds.h>

#include <AzCore/Serialization/SerializeContext.h>

namespace MultiImporter
{
    AZ_COMPONENT_IMPL(MultiImporterSystemComponent, "MultiImporterSystemComponent",
        MultiImporterSystemComponentTypeId);

    void MultiImporterSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<MultiImporterSystemComponent, AZ::Component>()
                ->Version(0)
                ;
        }
    }

    void MultiImporterSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("MultiImporterService"));
    }

    void MultiImporterSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("MultiImporterService"));
    }

    void MultiImporterSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
    }

    void MultiImporterSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
    }

    MultiImporterSystemComponent::MultiImporterSystemComponent()
    {
        if (MultiImporterInterface::Get() == nullptr)
        {
            MultiImporterInterface::Register(this);
        }
    }

    MultiImporterSystemComponent::~MultiImporterSystemComponent()
    {
        if (MultiImporterInterface::Get() == this)
        {
            MultiImporterInterface::Unregister(this);
        }
    }

    void MultiImporterSystemComponent::Init()
    {
    }

    void MultiImporterSystemComponent::Activate()
    {
        MultiImporterRequestBus::Handler::BusConnect();
        AZ::TickBus::Handler::BusConnect();
    }

    void MultiImporterSystemComponent::Deactivate()
    {
        AZ::TickBus::Handler::BusDisconnect();
        MultiImporterRequestBus::Handler::BusDisconnect();
    }

    void MultiImporterSystemComponent::OnTick([[maybe_unused]] float deltaTime, [[maybe_unused]] AZ::ScriptTimePoint time)
    {
    }

} // namespace MultiImporter
