#pragma once

#include "../Contexts/ufbxImportContexts.h"
#include <SceneAPI/SceneCore/Components/LoadingComponent.h>

namespace MultiImporter
{
    class ufbxSkinImporter : public AZ::SceneAPI::SceneCore::LoadingComponent
    {
    public:
        AZ_COMPONENT(ufbxSkinImporter, "{0199B069-C078-771C-AD65-B6127DD18D23}", AZ::SceneAPI::SceneCore::LoadingComponent);

        ufbxSkinImporter();
        ~ufbxSkinImporter() override = default;

        static void Reflect(AZ::ReflectContext* context);

        AZ::SceneAPI::Events::ProcessingResult ImportSkin(ufbxNodeEncounteredContext& context);

    };
}