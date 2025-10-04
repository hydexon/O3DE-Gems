#pragma once

#include "../Contexts/ufbxImportContexts.h"
#include <SceneAPI/SceneCore/Components/LoadingComponent.h>

namespace MultiImporter
{
    class ufbxTransformImporter : public AZ::SceneAPI::SceneCore::LoadingComponent
    {
    public:
        AZ_COMPONENT(ufbxTransformImporter, "{0199AD4D-8E74-7181-B8A5-90ED44E4D5D4}", AZ::SceneAPI::SceneCore::LoadingComponent);

        ufbxTransformImporter();
        ~ufbxTransformImporter() override = default;

        static void Reflect(AZ::ReflectContext* context);

        AZ::SceneAPI::Events::ProcessingResult ImportTransform(ufbxSceneNodeAppendedContext& context);

    };
}