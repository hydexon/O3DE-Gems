
#include <AzCore/Serialization/SerializeContext.h>
#include "MultiImporterEditorSystemComponent.h"

#include <MultiImporter/MultiImporterTypeIds.h>

namespace MultiImporter
{
    AZ_COMPONENT_IMPL(MultiImporterEditorSystemComponent, "MultiImporterEditorSystemComponent",
        MultiImporterEditorSystemComponentTypeId, BaseSystemComponent);

    void MultiImporterEditorSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<MultiImporterEditorSystemComponent, MultiImporterSystemComponent>()
                ->Version(0);
        }
    }

    MultiImporterEditorSystemComponent::MultiImporterEditorSystemComponent() = default;

    MultiImporterEditorSystemComponent::~MultiImporterEditorSystemComponent() = default;

    void MultiImporterEditorSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        BaseSystemComponent::GetProvidedServices(provided);
        provided.push_back(AZ_CRC_CE("MultiImporterEditorService"));
    }

    void MultiImporterEditorSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        BaseSystemComponent::GetIncompatibleServices(incompatible);
        incompatible.push_back(AZ_CRC_CE("MultiImporterEditorService"));
    }

    void MultiImporterEditorSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        BaseSystemComponent::GetRequiredServices(required);
    }

    void MultiImporterEditorSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
        BaseSystemComponent::GetDependentServices(dependent);
    }

    void MultiImporterEditorSystemComponent::Activate()
    {
        MultiImporterSystemComponent::Activate();
        AzToolsFramework::EditorEvents::Bus::Handler::BusConnect();
    }

    void MultiImporterEditorSystemComponent::Deactivate()
    {
        AzToolsFramework::EditorEvents::Bus::Handler::BusDisconnect();
        MultiImporterSystemComponent::Deactivate();
    }

} // namespace MultiImporter
