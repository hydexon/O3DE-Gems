#pragma once

#include "../Contexts/ufbxImportContexts.h"
#include <SceneAPI/SceneCore/Components/LoadingComponent.h>

namespace MultiImporter
{
    class ufbxSkinImporter : public AZ::SceneAPI::SceneCore::LoadingComponent
    {
    public:
        AZ_COMPONENT(ufbxSkinImporter, "{0199AD4D-8E74-7181-B8A5-90ED44E4D5D4}", AZ::SceneAPI::SceneCore::LoadingComponent);

        ufbxSkinImporter();
        ~ufbxSkinImporter() override = default;

        static void Reflect(AZ::ReflectContext* context);

        AZ::SceneAPI::Events::ProcessingResult ImportSkin(ufbxNodeEncounteredContext& context);

    };
}