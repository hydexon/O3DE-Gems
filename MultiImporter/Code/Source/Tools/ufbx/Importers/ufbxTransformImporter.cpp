#include  "ufbxTransformImporter.h"

#include <AzCore/Debug/Trace.h>
#include <AzCore/std/containers/unordered_set.h>
#include <AzCore/std/containers/queue.h>
#include <AzCore/StringFunc/StringFunc.h>
#include <SceneAPI/SceneCore/Utilities/Reporting.h>
#include <AzToolsFramework/Debug/TraceContext.h>


#include "../Wrappers/ufbxNodeWrapper.h"
#include "../Wrappers/ufbxSceneWrapper.h"
#include "ufbx.h"

MultiImporter::ufbxTransformImporter::ufbxTransformImporter()
{
    BindToCall(&ufbxTransformImporter::ImportTransform);
}

void MultiImporter::ufbxTransformImporter::Reflect(AZ::ReflectContext *context)
{
    AZ::SerializeContext* serializeContext = azrtti_cast<AZ::SerializeContext*>(context);
    if(serializeContext)
    {
        serializeContext->Class<ufbxTransformImporter, AZ::SceneAPI::SceneCore::LoadingComponent>()->Version(1);
    }

}

AZ::SceneAPI::Events::ProcessingResult MultiImporter::ufbxTransformImporter::ImportTransform(ufbxSceneNodeAppendedContext &context)
{
    AZ_TraceContext("Importer (ufbx)", "transform");
    const ufbx_node* currentNode = context.m_sourceNode.GetUbfxNode();
    const ufbx_scene* scene = context.m_sourceScene.GetUfbxScene();

    if(currentNode == scene->root_node) {
        return AZ::SceneAPI::Events::ProcessingResult::Ignored;
    }



    return AZ::SceneAPI::Events::ProcessingResult::Ignored;
}
