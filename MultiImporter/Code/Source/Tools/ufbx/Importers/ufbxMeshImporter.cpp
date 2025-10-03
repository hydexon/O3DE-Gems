#include "ufbxMeshImporter.h"

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



MultiImporter::ufbxMeshImporter::ufbxMeshImporter()
{
    BindToCall(&ufbxMeshImporter::ImportMesh);
}

void MultiImporter::ufbxMeshImporter::Reflect(AZ::ReflectContext* context)
{
    AZ::SerializeContext* serializeContext = azrtti_cast<AZ::SerializeContext*>(context);
    if(serializeContext)
    {
        serializeContext->Class<ufbxMeshImporter, AZ::SceneAPI::SceneCore::LoadingComponent>()->Version(1);
    }
}

AZ::SceneAPI::Events::ProcessingResult MultiImporter::ufbxMeshImporter::ImportMesh(ufbxNodeEncounteredContext &context)
{
    AZ_TraceContext("ufbxImporter", "Mesh");

    const ufbx_node* currentNode = context.m_sourceNode.GetUbfxNode();
    const ufbx_scene* scene = context.m_sourceScene.GetUfbxScene();

    AZ_Trace(AZ::SceneAPI::Utilities::LogWindow, "Importing Mesh using ufbx subsystem");
    
    if(!context.m_sourceNode.ContainsMesh() || Utils::IsSkinnnedMesh(*currentNode, *scene) )
    {
       return AZ::SceneAPI::Events::ProcessingResult::Ignored;
    }

    if(Utils::BuildSceneMeshFromUbfxMesh(currentNode, scene, context.m_sourceSceneSystem, context.m_createdData, [] { return AZStd::make_shared<AZ::SceneData::GraphData::MeshData>(); }) )
    {
        return AZ::SceneAPI::Events::ProcessingResult::Success;
    }

    return AZ::SceneAPI::Events::ProcessingResult::Failure;
}
