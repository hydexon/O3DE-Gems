#include "ufbxMeshImporterUtils.h"

#include "ufbx.h"
#include <AzCore/Casting/numeric_cast.h>
#include <AzCore/std/numeric.h>
#include <AzCore/std/smart_ptr/make_shared.h>
#include <SceneAPI/SceneBuilder/SceneSystem.h>
#include "../Contexts/ufbxImportContexts.h"

#include <SceneAPI/SceneCore/Utilities/Reporting.h>
#include <SceneAPI/SceneData/GraphData/BlendShapeData.h>
#include <SceneAPI/SceneData/GraphData/BoneData.h>
#include <SceneAPI/SceneData/GraphData/MeshData.h>

bool MultiImporter::Utils::IsSkinnnedMesh(const ufbx_node &node, const ufbx_scene &scene)
{
    if(node.mesh == NULL)
        return false;

    return node.mesh->skin_deformers.count > 0;
}

bool MultiImporter::Utils::BuildSceneMeshFromUbfxMesh(const ufbx_node *currentNode, const ufbx_scene *scene, const AZ::SceneAPI::SceneSystem &sceneSystem, AZStd::vector<AZStd::shared_ptr<AZ::SceneAPI::DataTypes::IGraphObject>> &meshes, const AZStd::function<AZStd::shared_ptr<AZ::SceneData::GraphData::MeshData>()> &makeMeshFunc)
{
    if(!currentNode || !scene)
        return false;

    auto newMesh = makeMeshFunc();
    newMesh->SetUnitSizeInMeters(sceneSystem.GetUnitSizeInMeters());
    newMesh->SetOriginalUnitSizeInMeters(sceneSystem.GetOriginalUnitSizeInMeters());

    int minMeshMaterialIndex = INT_MAX;
    int maxMeshMaterialIndex = INT_MIN;

    ufbx_mesh* srcMesh = currentNode->mesh;
    auto materialIndices = srcMesh->face_material.data;

    int fbxPolygonCount = srcMesh->num_faces; //Known as faces!

    for(int polIdx = 0; polIdx < fbxPolygonCount; ++polIdx) 
    {
        const int fbxPolVertCnt = srcMesh->faces[polIdx].num_indices;
        if(fbxPolVertCnt <= 2)
            continue;
            
        // Get the material index of each polygon (or face).
        const int meshMaterialIndex = materialIndices ? materialIndices[polIdx] : -1;
        minMeshMaterialIndex = AZ::GetMin<int>(minMeshMaterialIndex, meshMaterialIndex);
        maxMeshMaterialIndex = AZ::GetMax<int>(maxMeshMaterialIndex, meshMaterialIndex);
    }

    if(minMeshMaterialIndex > maxMeshMaterialIndex)
        return false;

    if(maxMeshMaterialIndex < 0) 
        minMeshMaterialIndex = maxMeshMaterialIndex = 0;
    else if (minMeshMaterialIndex < 0 ) {
        minMeshMaterialIndex = 0;
        ++maxMeshMaterialIndex;
    }

    //Fill the geometry.

    //Control points contain positions of vertices:
    const size_t controlPoints = srcMesh->vertex_position.values.count;

    materialIndices = nullptr;
    materialIndices = srcMesh->face_material.data; //per polygon/face

    fbxPolygonCount = srcMesh->num_faces;
    for(int fbxPolygonIdx = 0; fbxPolygonIdx < fbxPolygonCount; ++fbxPolygonIdx)
    {
        const int fbxFaceVertexCount = srcMesh->faces[fbxPolygonIdx].num_indices; //Get Polygon/Face size.
        if(fbxFaceVertexCount <= 2)
            continue;

        //Ensure the validty of the material index for the polygon

    }


    return true;
}

MultiImporter::Utils::GetMeshDataFromParentResult MultiImporter::Utils::GetMeshDataFromParent(ufbxSceneNodeAppendedContext &context)
{
    using namespace AZ::SceneAPI;
    const DataTypes::IGraphObject* const parentData = context.m_scene.GetGraph().GetNodeContent(context.m_currentGraphPosition).get();

    if(!parentData)
    {
        AZ_Error(Utilities::ErrorWindow, false,
            "GetMeshDataFromParent failed because the parent was null, it should only be called with a valid parent node");

        return AZ::Failure(Events::ProcessingResult::Failure);
    }

    if(!parentData->RTTI_IsTypeOf(AZ::SceneData::GraphData::MeshData::TYPEINFO_Uuid()))
    {
        if(parentData->RTTI_IsTypeOf(AZ::SceneData::GraphData::BoneData::TYPEINFO_Uuid()))
        {
            return AZ::Failure(Events::ProcessingResult::Ignored);
        }
        AZ_Error(Utilities::ErrorWindow, false, "Tried to get mesh data from parent from non-mesh parent data");
        return AZ::Failure(Events::ProcessingResult::Failure);
    }
    
    const AZ::SceneData::GraphData::MeshData* const parentMeshData = azrtti_cast<const AZ::SceneData::GraphData::MeshData* const>(parentData);
    return AZ::Success(parentMeshData);

}
