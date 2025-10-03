#pragma once

#include <SceneAPI/SceneCore/Components/SceneSystemComponent.h>

namespace MultiImporter
{
    class ufbxImporterContextRegistryComponent
        : public AZ::SceneAPI::SceneCore::SceneSystemComponent
    {
    public:
        AZ_COMPONENT(ufbxImporterContextRegistryComponent, "{01992B48-C75B-7135-98A9-54FCBDBB4383}", AZ::SceneAPI::SceneCore::SceneSystemComponent);
        ~ufbxImporterContextRegistryComponent() override = default;

        void Activate() override;
        void Deactivate() override;

        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void Reflect(AZ::ReflectContext* context);
    };
}