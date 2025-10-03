#pragma once

#include <SceneAPI/SDKWrapper/MaterialWrapper.h>
#include <AzCore/std/string/string.h>

struct ufbx_material;

namespace MultiImporter 
{
    class ufbxMaterialWrapper : public AZ::SDKMaterial::MaterialWrapper
    {
    public:
        AZ_RTTI(ufbxMaterialWrapper, "{01992776-FD9E-763C-ACE0-7BDE5F2B963B}", AZ::SDKMaterial::MaterialWrapper);
        ufbxMaterialWrapper(ufbx_material* material);
        ~ufbxMaterialWrapper() override = default;

        ufbx_material* GetUfbxMaterial() const;
        AZStd::string GetName() const override;
        AZ::u64 GetUniqueId() const override;
        
        AZ::Vector3 GetDiffuseColor() const override;
        AZ::Vector3 GetSpecularColor() const override;
        AZ::Vector3 GetEmissiveColor() const override;
        float GetOpacity() const override;
        float GetShininess() const override;
        AZStd::string GetTextureFileName(MaterialMapType textureType) const override;

    protected:
        ufbx_material* m_ufbxMaterial = nullptr;

    };
}