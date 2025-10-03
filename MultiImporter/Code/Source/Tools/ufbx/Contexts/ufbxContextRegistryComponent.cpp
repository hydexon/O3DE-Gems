#include "ufbxContextRegistryComponent.h"

#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <SceneAPI/SceneCore/Events/AssetImportRequest.h>
#include <SceneAPI/SceneBuilder/ImportContextRegistry.h>

#include "ufbxImportContextProvider.h"

using namespace AZ::SceneAPI;

void MultiImporter::ufbxImporterContextRegistryComponent::Activate()
{
    if(auto* registry = AZ::SceneAPI::SceneBuilder::ImportContextRegistryInterface::Get()) {
        auto ufbxCtxProvider = aznew ufbxImportContextProvider();
        registry->RegisterContextProvider(ufbxCtxProvider);

        AZ_Info("SceneAPI", "ufbx import context provider has been registered to the SceneBuilder service");
    }
    
}

void MultiImporter::ufbxImporterContextRegistryComponent::Deactivate()
{

}

void MultiImporter::ufbxImporterContextRegistryComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
{
    provided.push_back(AZ_CRC_CE("ufbxImporterContextRegistryService"));
}

void MultiImporter::ufbxImporterContextRegistryComponent::Reflect(AZ::ReflectContext* context)
{
    AZ::SerializeContext* serializeContext = azrtti_cast<AZ::SerializeContext*>(context);
    if(serializeContext)
    {
        serializeContext->Class<ufbxImporterContextRegistryComponent, SceneCore::SceneSystemComponent>()->Version(1)->Attribute(
            AZ::Edit::Attributes::SystemComponentTags, AZStd::vector<AZ::Crc32>({ Events::AssetImportRequest::GetAssetImportRequestComponentTag() })
        );
    }

}