#include "ufbx/ufbx.h"

#include "ufbxNodeWrapper.h"
#include <AzCore/std/string/string.h>

MultiImporter::ufbxNodeWrapper::ufbxNodeWrapper(ufbx_node* node) 
    : m_fbxNode(node)
{
}

const char* MultiImporter::ufbxNodeWrapper::GetName() const
{
    return m_fbxNode->name.data;
}

AZ::u64 MultiImporter::ufbxNodeWrapper::GetUniqueId() const
{
    AZStd::string fingerprintString;
    fingerprintString.append(GetName());
    AZStd::string extraInfo = AZStd::string::format("%i", static_cast<int>(m_fbxNode->children.count)); 
    fingerprintString.append(extraInfo);
    AZ::Sha1 sha;
    sha.ProcessBytes(AZStd::as_bytes(AZStd::span(fingerprintString)));
    AZ::u32 digest[5];
    sha.GetDigest(digest);
    return (static_cast<AZ::u64>(digest[0]) << 32 | digest[1]);
}

int MultiImporter::ufbxNodeWrapper::GetChildCount() const 
{
    return m_fbxNode->children.count;
}

const std::shared_ptr<AZ::SDKNode::NodeWrapper> MultiImporter::ufbxNodeWrapper::GetChild(int childIndex) const
{
    ufbx_node* child = m_fbxNode->children[childIndex];
    AZ_Error("MultiImporter", child, "Cannot get child ufbx_node at index %d", childIndex);
    return child ? std::shared_ptr<AZ::SDKNode::NodeWrapper>(new ufbxNodeWrapper(child)) : nullptr;
}

int MultiImporter::ufbxNodeWrapper::GetMaterialCount() const
{
    return m_fbxNode->materials.count;
}

ufbx_node *MultiImporter::ufbxNodeWrapper::GetUbfxNode() const
{
    return m_fbxNode;
}

const bool MultiImporter::ufbxNodeWrapper::ContainsMesh()
{
    return m_fbxNode->mesh != NULL;
}
