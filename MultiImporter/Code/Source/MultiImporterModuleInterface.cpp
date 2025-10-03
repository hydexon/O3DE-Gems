
#include "MultiImporterModuleInterface.h"
#include <AzCore/Memory/Memory.h>

#include <MultiImporter/MultiImporterTypeIds.h>

#include <Clients/MultiImporterSystemComponent.h>

namespace MultiImporter
{
    AZ_TYPE_INFO_WITH_NAME_IMPL(MultiImporterModuleInterface,
        "MultiImporterModuleInterface", MultiImporterModuleInterfaceTypeId);
    AZ_RTTI_NO_TYPE_INFO_IMPL(MultiImporterModuleInterface, AZ::Module);
    AZ_CLASS_ALLOCATOR_IMPL(MultiImporterModuleInterface, AZ::SystemAllocator);

    MultiImporterModuleInterface::MultiImporterModuleInterface()
    {
        // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
        // Add ALL components descriptors associated with this gem to m_descriptors.
        // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and EditContext.
        // This happens through the [MyComponent]::Reflect() function.
        m_descriptors.insert(m_descriptors.end(), {
            MultiImporterSystemComponent::CreateDescriptor(),
            });
    }

    AZ::ComponentTypeList MultiImporterModuleInterface::GetRequiredSystemComponents() const
    {
        return AZ::ComponentTypeList{
            azrtti_typeid<MultiImporterSystemComponent>(),
        };
    }
} // namespace MultiImporter
