#include <AzCore/Debug/Trace.h>
#include <AzCore/Settings/SettingsRegistry.h>
#include <AzToolsFramework/Debug/TraceContext.h>
#include <SceneAPI/SceneCore/Utilities/Reporting.h>
#include <AzCore/std/containers/vector.h>
#include <AzCore/std/tuple.h>

#include "ufbxNodeWrapper.h"
#include "ufbxMaterialWrapper.h"
#include "ufbxSceneWrapper.h"
#include "ufbx.h"

#include "../Utils/ufbxTypeConversions.h"

using namespace AZ;

MultiImporter::ufbxSceneWrapper::ufbxSceneWrapper()
    : m_vertices(0)
{
}

MultiImporter::ufbxSceneWrapper::ufbxSceneWrapper(ufbx_scene *ufbxScene)
    : m_ufbxScene(ufbxScene)
    , m_vertices(0)
{
}

bool MultiImporter::ufbxSceneWrapper::LoadSceneFromFile(const char *fileName, const AZ::SceneAPI::SceneImportSettings &importSettings)
{
    AZ_Trace(SceneAPI::Utilities::LogWindow, "ufbxSceneWrapper::LoadSceneFromFile %s", fileName);
    AZ_TraceContext("Filename", fileName);

    ufbx_load_opts load_opts = {
        .allow_nodes_out_of_root = importSettings.m_optimizeScene,
        .target_unit_meters = 1.0f,
        .generate_missing_normals = true,
        .evaluate_skinning = true
    };
    ufbx_error fbxError;

    m_ufbxScene = ufbx_load_file(fileName, &load_opts, &fbxError);
    if(!m_ufbxScene) 
    {
        AZ_Trace(SceneAPI::Utilities::ErrorWindow, "Failed to import ufbx Scene. Error Returned: %s", fbxError.description.data);
        return false;
    }
    
    //Embedded Textures?
    calculateVerticesAndAABB();

    return true;
}

const std::shared_ptr<AZ::SDKNode::NodeWrapper> MultiImporter::ufbxSceneWrapper::GetRootNode() const
{
    return std::shared_ptr<AZ::SDKNode::NodeWrapper>(new MultiImporter::ufbxNodeWrapper(m_ufbxScene->root_node));
}

std::shared_ptr<AZ::SDKNode::NodeWrapper> MultiImporter::ufbxSceneWrapper::GetRootNode()
{
    return std::shared_ptr<AZ::SDKNode::NodeWrapper>(new MultiImporter::ufbxNodeWrapper(m_ufbxScene->root_node));
}

void MultiImporter::ufbxSceneWrapper::Clear()
{
    if(m_ufbxScene) {
        ufbx_free_scene(m_ufbxScene);
        m_ufbxScene = nullptr;
    }
}

AZStd::pair<AZ::SDKScene::SceneWrapperBase::AxisVector, int32_t> MultiImporter::ufbxSceneWrapper::GetUpVectorAndSign() const
{
    ufbx_coordinate_axis axis = m_ufbxScene->settings.axes.up;
    switch (axis)
    {
    case UFBX_COORDINATE_AXIS_POSITIVE_Y: return AZStd::pair<AxisVector, int32_t>(AxisVector::Y, 1);
    case UFBX_COORDINATE_AXIS_POSITIVE_X: return AZStd::pair<AxisVector, int32_t>(AxisVector::X, 1);
    case UFBX_COORDINATE_AXIS_POSITIVE_Z: return AZStd::pair<AxisVector, int32_t>(AxisVector::Z, 1);
    case UFBX_COORDINATE_AXIS_NEGATIVE_X: return AZStd::pair<AxisVector, int32_t>(AxisVector::X, -1);
    case UFBX_COORDINATE_AXIS_NEGATIVE_Y: return AZStd::pair<AxisVector, int32_t>(AxisVector::Y, -1);
    case UFBX_COORDINATE_AXIS_NEGATIVE_Z: return AZStd::pair<AxisVector, int32_t>(AxisVector::Z, -1);
    default:
        break;
    }
    return AZStd::pair<AxisVector, int32_t>(AxisVector::Z, 1);

}

AZStd::pair<AZ::SDKScene::SceneWrapperBase::AxisVector, int32_t> MultiImporter::ufbxSceneWrapper::GetFrontVectorAndSign() const
{
    ufbx_coordinate_axis axis = m_ufbxScene->settings.axes.front;
    switch (axis)
    {
    case UFBX_COORDINATE_AXIS_POSITIVE_Y: return AZStd::pair<AxisVector, int32_t>(AxisVector::Y, 1);
    case UFBX_COORDINATE_AXIS_POSITIVE_X: return AZStd::pair<AxisVector, int32_t>(AxisVector::X, 1);
    case UFBX_COORDINATE_AXIS_POSITIVE_Z: return AZStd::pair<AxisVector, int32_t>(AxisVector::Z, 1);
    case UFBX_COORDINATE_AXIS_NEGATIVE_X: return AZStd::pair<AxisVector, int32_t>(AxisVector::X, -1);
    case UFBX_COORDINATE_AXIS_NEGATIVE_Y: return AZStd::pair<AxisVector, int32_t>(AxisVector::Y, -1);
    case UFBX_COORDINATE_AXIS_NEGATIVE_Z: return AZStd::pair<AxisVector, int32_t>(AxisVector::Z, -1);
    default:
        break;
    }

    return AZStd::pair<AxisVector, int32_t>(AxisVector::Y, 1);
}

AZStd::pair<AZ::SDKScene::SceneWrapperBase::AxisVector, int32_t> MultiImporter::ufbxSceneWrapper::GetRightVectorAndSign() const
{
    ufbx_coordinate_axis axis = m_ufbxScene->settings.axes.right;
    switch (axis)
    {
    case UFBX_COORDINATE_AXIS_POSITIVE_Y: return AZStd::pair<AxisVector, int32_t>(AxisVector::Y, 1);
    case UFBX_COORDINATE_AXIS_POSITIVE_X: return AZStd::pair<AxisVector, int32_t>(AxisVector::X, 1);
    case UFBX_COORDINATE_AXIS_POSITIVE_Z: return AZStd::pair<AxisVector, int32_t>(AxisVector::Z, 1);
    case UFBX_COORDINATE_AXIS_NEGATIVE_X: return AZStd::pair<AxisVector, int32_t>(AxisVector::X, -1);
    case UFBX_COORDINATE_AXIS_NEGATIVE_Y: return AZStd::pair<AxisVector, int32_t>(AxisVector::Y, -1);
    case UFBX_COORDINATE_AXIS_NEGATIVE_Z: return AZStd::pair<AxisVector, int32_t>(AxisVector::Z, -1);
    default:
        break;
    }

    return AZStd::pair<AxisVector, int32_t>(AxisVector::X, 1);
}

AZStd::optional<AZ::SceneAPI::DataTypes::MatrixType> MultiImporter::ufbxSceneWrapper::UseForcedRootTransform() const
{
    return AZStd::optional<AZ::SceneAPI::DataTypes::MatrixType>();
}

float MultiImporter::ufbxSceneWrapper::GetUnitSizeInMeters() const
{
    return m_ufbxScene->settings.unit_meters;
}

AZ::Aabb MultiImporter::ufbxSceneWrapper::GetAABB() const
{
    return AZ::Aabb::CreateFromMinMax(m_aabbMin, m_aabbMax);
}

ufbx_scene *MultiImporter::ufbxSceneWrapper::GetUfbxScene() const
{
    return m_ufbxScene;
}

void MultiImporter::ufbxSceneWrapper::calculateVerticesAndAABB()
{
    m_aabbMin = AZ::Vector3(+INFINITY);
    m_aabbMax = AZ::Vector3(-INFINITY);

    //Calculate the AABB from every mesh found in the scene.
    //Format goes: isLocal, AABBMin, AABBMax, meshIndex.
    AZStd::vector<AZStd::tuple<bool, AZ::Vector3, AZ::Vector3, int>> meshAAbbList;

    for(size_t meshIdx = 0; meshIdx < m_ufbxScene->meshes.count; meshIdx++)
    {
        ufbx_mesh* mesh = m_ufbxScene->meshes[meshIdx];
        AZ::Vector3 meshAABBMin = AZ::Vector3(+INFINITY);
        AZ::Vector3 meshAABBMax = AZ::Vector3(-INFINITY);

        for(size_t i = 0; i < mesh->vertices.count; i++) {
            ufbx_vec3 _pos = mesh->skinned_position.values.data[i];
            AZ::Vector3 pos = AZ::Vector3(static_cast<float>(_pos.x), static_cast<float>(_pos.y), static_cast<float>(_pos.z));
            auto mTmpMin = meshAABBMin.GetMin(pos);
            auto mTmpMax = meshAABBMax.GetMax(pos);
            meshAABBMin = mTmpMin;
            meshAABBMax = mTmpMax;
        }
        meshAAbbList.push_back(AZStd::make_tuple(mesh->skinned_is_local, meshAABBMin, meshAABBMax, meshIdx));
        m_vertices += mesh->num_vertices;
    }
    
    //Now calculate the world-space AABB of the scene.
    for(const auto& it : meshAAbbList)
    {
        bool isMeshAABBLocal = AZStd::get<0>(it);
        auto meshAabbMin = AZStd::get<1>(it);
        auto meshAabbMax = AZStd::get<2>(it);
        int meshIndex = AZStd::get<3>(it);

        AZ::Vector3 aabbOrigin = (meshAabbMax + meshAabbMin) * 0.5f;
        AZ::Vector3 aabbExtent = (meshAabbMax - meshAabbMax) * 0.5f;
        if(isMeshAABBLocal)
        {
            ufbx_mesh* fbxMesh = m_ufbxScene->meshes[meshIndex];
            for(size_t i = 0; i < fbxMesh->instances.count; i++) 
            {
                auto insNodeInstance = fbxMesh->instances[i]->typed_id;
                ufbx_node* node = m_ufbxScene->nodes[insNodeInstance];
                auto geometryToWorld = MultiImporter::ConvertUfbxMatrixToAzMatrix(node->geometry_to_world);
                AZ::Vector3 worldOrigin = geometryToWorld.TransformPoint(aabbOrigin);
                AZ::Vector3 worldExtent = MultiImporter::TransformExtent(geometryToWorld, aabbExtent);
                m_aabbMin = m_aabbMin.GetMin(worldOrigin - worldExtent);
                m_aabbMax = m_aabbMax.GetMax(worldOrigin + worldExtent);
            }
        } else {
            auto tmpMin = m_aabbMin.GetMin(meshAabbMin);
            auto tmpMax = m_aabbMax.GetMax(meshAabbMax);
            m_aabbMin = tmpMin;
            m_aabbMax = tmpMax;
        }
    }
}
