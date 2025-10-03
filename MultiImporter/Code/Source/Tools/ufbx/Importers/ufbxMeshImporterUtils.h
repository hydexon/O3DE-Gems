#pragma once

#include <SceneAPI/SceneCore/Events/ProcessingResult.h>
#include <AzCore/std/containers/vector.h>
#include <AzCore/std/smart_ptr/shared_ptr.h>
#include <AzCore/Outcome/Outcome.h>

struct ufbx_node;
struct ufbx_scene;

namespace AZ
{
    namespace SceneData
    {
        namespace GraphData
        {
            class MeshData;
            class BlendShapeData;
        }
    }

    namespace SceneAPI
    {
        namespace DataTypes
        {
            class IGraphObject;
        }
        class SceneSystem;
    }
}

namespace MultiImporter
{
    struct ufbxSceneNodeAppendedContext;

    namespace Utils 
    {
        bool BuildSceneMeshFromUbfxMesh(const ufbx_node* currentNode, const ufbx_scene* scene,
             const AZ::SceneAPI::SceneSystem& sceneSystem, 
             AZStd::vector<AZStd::shared_ptr<AZ::SceneAPI::DataTypes::IGraphObject>>& meshes,
             const AZStd::function<AZStd::shared_ptr<AZ::SceneData::GraphData::MeshData>()>& makeMeshFunc);

        typedef AZ::Outcome<const AZ::SceneData::GraphData::MeshData*, AZ::SceneAPI::Events::ProcessingResult> GetMeshDataFromParentResult;
        GetMeshDataFromParentResult GetMeshDataFromParent(ufbxSceneNodeAppendedContext& context);

        bool IsSkinnnedMesh(const ufbx_node& node, const ufbx_scene& scene); 
    }
}
