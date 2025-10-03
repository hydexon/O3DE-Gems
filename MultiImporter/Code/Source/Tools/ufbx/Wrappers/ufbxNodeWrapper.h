#pragma once

#include <SceneAPI/SDKWrapper/NodeWrapper.h>

struct ufbx_node;

namespace MultiImporter
{
    class ufbxNodeWrapper : public AZ::SDKNode::NodeWrapper
    {
    public:
        AZ_RTTI(ufbxNodeWrapper, "{01983EA9-FCB7-737A-BEF7-48CAA18F820C}", AZ::SDKNode::NodeWrapper);
        ufbxNodeWrapper(ufbx_node* node);
        ~ufbxNodeWrapper() override = default;

        const char* GetName() const override;
        AZ::u64 GetUniqueId() const override;
        int GetChildCount() const override;
        const std::shared_ptr<AZ::SDKNode::NodeWrapper> GetChild(int childINdex) const override;

        int GetMaterialCount() const override;
        ufbx_node* GetUbfxNode() const;
        const bool ContainsMesh();
    protected:
        ufbx_node* m_fbxNode = nullptr;

        
    };

}