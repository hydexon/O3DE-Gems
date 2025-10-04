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

struct Vertex {
    ufbx_vec3 position;
    ufbx_vec3 normal;
    ufbx_vec2 uv;
};

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
    //const size_t controlPoints = srcMesh->vertex_position.values.count;
    for(auto& part : srcMesh->material_parts)
    {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> tri_indices;
        tri_indices.resize(srcMesh->max_face_triangles * 3);

        for(uint32_t face_index : part.face_indices) {
            ufbx_face& polygon = srcMesh->faces[face_index];

            //Trinagulate the face into tri_indices:
            uint32_t num_tris = ufbx_triangulate_face(tri_indices.data(), tri_indices.size(), srcMesh, polygon);
            //Iterate over each triangle continuously:
            for(size_t i = 0; i < num_tris * 3; i++) {
                uint32_t index = tri_indices[i];
                
                Vertex v;
                v.position = srcMesh->vertex_position[index];
                v.normal = srcMesh->vertex_normal[index];
                v.uv = srcMesh->vertex_uv[index];
                vertices.push_back(v);
            }
            if(vertices.size() ==  part.num_triangles * 3) {
                AZ_Error(AZ::SceneAPI::Utilities::ErrorWindow, false, "%s", "Vertice processing incomplete!");
                continue;
            }

            //Generate indices:
            ufbx_vertex_stream streams[1] = { { vertices.data(), vertices.size(), sizeof(Vertex) } };
            std::vector<uint32_t> indices;

            size_t num_vertices = ufbx_generate_indices(streams, 1, indices.data(), indices.size(), nullptr, nullptr);
            //Trim to only unique vertices:
            vertices.resize(num_vertices);

            //Operate:
            size_t index = 0;
            for(auto& v : vertices) {

                AZ::Vector3 position(v.position.x, v.position.y, v.position.z);
                sceneSystem.SwapVec3ForUpAxis(position);
                sceneSystem.ConvertUnit(position);
                newMesh->AddPosition(position);
                newMesh->SetVertexIndexToControlPointIndexMap(index, indices[index]);
                index++;

                AZ::Vector3 normal(v.normal.x, v.normal.y, v.normal.z);
                sceneSystem.SwapVec3ForUpAxis(normal);
                normal.NormalizeSafe();
                newMesh->AddNormal(normal);
            }

            AZ::SceneAPI::DataTypes::IMeshData::Face meshFace;
            for(auto indice : indices) {
                meshFace.vertexIndex[indice] = indices[indice];
            }
            newMesh->AddFace(meshFace, srcMesh->face_material[face_index]);
        }
    }
    meshes.push_back(newMesh);


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
