#include "ufbxSkinImporter.h"

#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/std/smart_ptr/make_shared.h>
#include <AzToolsFramework/Debug/TraceContext.h>
#include <SceneAPI/SceneData/GraphData/MeshData.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <SceneAPI/SceneCore/Utilities/Reporting.h>

#include "../Wrappers/ufbxSceneWrapper.h"
#include "../Wrappers/ufbxNodeWrapper.h"

#include "ufbxMeshImporterUtils.h"
#include "ufbx.h"


MultiImporter::ufbxSkinImporter::ufbxSkinImporter()
{
    BindToCall(&ufbxSkinImporter::ImportSkin);
}

void MultiImporter::ufbxSkinImporter::Reflect(AZ::ReflectContext *context)
{
    AZ::SerializeContext* serializeContext = azrtti_cast<AZ::SerializeContext*>(context);
    if(serializeContext)
    {
        serializeContext->Class<ufbxSkinImporter, AZ::SceneAPI::SceneCore::LoadingComponent>()->Version(1);
    }

}

AZ::SceneAPI::Events::ProcessingResult MultiImporter::ufbxSkinImporter::ImportSkin(ufbxNodeEncounteredContext &context)
{
    AZ_TraceContext("Importer (ufbx)", "Skin");

    const ufbx_node* currentNode = context.m_sourceNode.GetUbfxNode();
    const ufbx_scene* scene = context.m_sourceScene.GetUfbxScene();

    if(!context.m_sourceNode.ContainsMesh() || !Utils::IsSkinnnedMesh(*currentNode, *context.m_sourceScene.GetUfbxScene()))
    {
        return AZ::SceneAPI::Events::ProcessingResult::Ignored;
    }
    
    if(Utils::BuildSceneMeshFromUbfxMesh(currentNode, scene, context.m_sourceSceneSystem, context.m_createdData,[]{ return AZStd::make_shared<AZ::SceneData::GraphData::MeshData>(); }))
    {
        return AZ::SceneAPI::Events::ProcessingResult::Success;
    }

    return AZ::SceneAPI::Events::ProcessingResult::Failure;
}
