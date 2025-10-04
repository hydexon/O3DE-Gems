#include "ufbxBitangentStreamImporter.h"
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/std/smart_ptr/make_shared.h>
#include <AzToolsFramework/Debug/TraceContext.h>
#include <SceneAPI/SceneBuilder/Importers/ImporterUtilities.h>
#include <SceneAPI/SceneData/GraphData/MeshVertexBitangentData.h>

#include "../Wrappers/ufbxNodeWrapper.h"
#include "../Wrappers/ufbxSceneWrapper.h"

#include "ufbx/ufbx.h"

MultiImporter::ufbxBitangentStreamImporter::ufbxBitangentStreamImporter()
{
    BindToCall(&ufbxBitangentStreamImporter::ImportBitangentStreams);
}

void MultiImporter::ufbxBitangentStreamImporter::Reflect(AZ::ReflectContext *context)
{
    AZ::SerializeContext* serializeContext = azrtti_cast<AZ::SerializeContext*>(context);
    if(serializeContext)
    {
        serializeContext->Class<ufbxBitangentStreamImporter, AZ::SceneAPI::SceneCore::LoadingComponent>()->Version(1);
    }

}

AZ::SceneAPI::Events::ProcessingResult MultiImporter::ufbxBitangentStreamImporter::ImportBitangentStreams(ufbxSceneNodeAppendedContext &context)
{
    if(!context.m_sourceNode.ContainsMesh())
    {
        return AZ::SceneAPI::Events::ProcessingResult::Ignored;
    }
    const ufbx_node* node = context.m_sourceNode.GetUbfxNode();
    //const ufbx_scene* scene = context.m_sourceScene.GetUfbxScene();

    if(!node->mesh->vertex_bitangent.exists)
    {
        return AZ::SceneAPI::Events::ProcessingResult::Ignored;
    }




    return AZ::SceneAPI::Events::ProcessingResult::Ignored;
}
