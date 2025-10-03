#pragma once

#include <SceneAPI/SDKWrapper/SceneWrapper.h>
#include <SceneAPI/SceneCore/Import/SceneImportSettings.h>

struct ufbx_scene;

namespace MultiImporter
{
    class ufbxSceneWrapper : public AZ::SDKScene::SceneWrapperBase
    {
    public:
        AZ_RTTI(ufbxSceneWrapper, "{01992A4B-CFA6-76DE-92C8-3F287737385E}", AZ::SDKScene::SceneWrapperBase);
        ufbxSceneWrapper();
        ufbxSceneWrapper(ufbx_scene* ufbxScene);
        ~ufbxSceneWrapper() override = default;

        bool LoadSceneFromFile(const char* fileName, const AZ::SceneAPI::SceneImportSettings& importSettings) override;

        const std::shared_ptr<AZ::SDKNode::NodeWrapper> GetRootNode() const override;
        std::shared_ptr<AZ::SDKNode::NodeWrapper> GetRootNode() override;
        void Clear() override;
        AZStd::pair<AxisVector, int32_t> GetUpVectorAndSign() const override;
        AZStd::pair<AxisVector, int32_t> GetFrontVectorAndSign() const override;
        AZStd::pair<AxisVector, int32_t> GetRightVectorAndSign() const override;
        AZStd::optional<AZ::SceneAPI::DataTypes::MatrixType> UseForcedRootTransform() const override;
        float GetUnitSizeInMeters() const override;
        AZ::Aabb GetAABB() const override;
        uint32_t GetVerticesCount() const override { return m_vertices; }

        ufbx_scene* GetUfbxScene() const;
    protected:
        ufbx_scene* m_ufbxScene = nullptr;
        AZ::Vector3 m_aabbMin;
        AZ::Vector3 m_aabbMax;
        AZ::Aabb m_aabb;
        size_t m_vertices;

        void calculateVerticesAndAABB();
        
    };
}