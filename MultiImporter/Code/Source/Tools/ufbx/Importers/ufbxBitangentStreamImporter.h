#pragma once

#include "../Contexts/ufbxImportContexts.h"
#include <SceneAPI/SceneCore/Components/LoadingComponent.h>

namespace MultiImporter
{
    class ufbxBitangentStreamImporter : public AZ::SceneAPI::SceneCore::LoadingComponent
    {
    public:
        AZ_COMPONENT(ufbxBitangentStreamImporter, "{0199B032-98F2-74BD-8F9F-4E5F1040939F}", AZ::SceneAPI::SceneCore::LoadingComponent);

        ufbxBitangentStreamImporter();
        ~ufbxBitangentStreamImporter() override = default;
        
        static void Reflect(AZ::ReflectContext* context);
        AZ::SceneAPI::Events::ProcessingResult ImportBitangentStreams(ufbxSceneNodeAppendedContext& context);
    };
}