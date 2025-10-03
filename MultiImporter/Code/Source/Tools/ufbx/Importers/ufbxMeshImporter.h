#pragma once

#include "../Contexts/ufbxImportContexts.h"
#include <SceneAPI/SceneCore/Components/LoadingComponent.h>

namespace MultiImporter
{
    class ufbxMeshImporter : public AZ::SceneAPI::SceneCore::LoadingComponent
    {
    public:
        AZ_COMPONENT(ufbxMeshImporter, "{01992B7A-0CD9-7132-A6C4-4E0D45DA0502}", AZ::SceneAPI::SceneCore::LoadingComponent);

        ufbxMeshImporter();
        ~ufbxMeshImporter() override = default;

        static void Reflect(AZ::ReflectContext* context);

        AZ::SceneAPI::Events::ProcessingResult ImportMesh(ufbxNodeEncounteredContext& context);
    };

}