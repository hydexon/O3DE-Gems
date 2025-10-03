#include "ufbx.h"
#include "ufbxMaterialWrapper.h"
#include <AzCore/Debug/Trace.h>
#include <AzToolsFramework/Debug/TraceContext.h>
#include <SceneAPI/SceneCore/Utilities/Reporting.h>

MultiImporter::ufbxMaterialWrapper::ufbxMaterialWrapper(ufbx_material* material)
    : m_ufbxMaterial(material)
{}

ufbx_material* MultiImporter::ufbxMaterialWrapper::GetUfbxMaterial() const
{
    return m_ufbxMaterial;
}

AZStd::string MultiImporter::ufbxMaterialWrapper::GetName() const 
{
    return m_ufbxMaterial->name.data;
}

AZ::u64 MultiImporter::ufbxMaterialWrapper::GetUniqueId() const
{
    AZStd::string fingerprintString;
    fingerprintString.append(GetName());
    AZStd::string extraInformation = AZStd::string::format("%lu%i%lu", m_ufbxMaterial->textures.count, m_ufbxMaterial->typed_id, m_ufbxMaterial->props.props.count);
    fingerprintString.append(extraInformation);
    AZ::Sha1 sha;
    sha.ProcessBytes(AZStd::as_bytes(AZStd::span(fingerprintString)));
    AZ::u32 digest[5]; //sha1 populate a 5 element array of AZ:u32
    sha.GetDigest(digest);
    return (static_cast<AZ::u64>(digest[0]) << 32) | digest[1];
}

AZ::Vector3 MultiImporter::ufbxMaterialWrapper::GetDiffuseColor() const
{
    ufbx_vec3 color = m_ufbxMaterial->fbx.diffuse_color.value_vec3;
    return AZ::Vector3(color.x, color.y, color.z);
}

AZ::Vector3 MultiImporter::ufbxMaterialWrapper::GetSpecularColor() const
{
    ufbx_vec3 color = m_ufbxMaterial->fbx.specular_color.value_vec3;
    return AZ::Vector3(color.x, color.y, color.z);
}

AZ::Vector3 MultiImporter::ufbxMaterialWrapper::GetEmissiveColor() const
{
    ufbx_vec3 color = m_ufbxMaterial->fbx.emission_color.value_vec3;
    return AZ::Vector3(color.x, color.y, color.z);
}

float MultiImporter::ufbxMaterialWrapper::GetOpacity() const
{
    float transparency = m_ufbxMaterial->fbx.transparency_factor.value_real;
    return transparency;
}

float MultiImporter::ufbxMaterialWrapper::GetShininess() const
{
    float shininess = m_ufbxMaterial->fbx.reflection_factor.value_real;
    return shininess;
}


AZStd::string MultiImporter::ufbxMaterialWrapper::GetTextureFileName(AZ::SDKMaterial::MaterialWrapper::MaterialMapType textureType) const
{
    ufbx_string absTexFilePath = {0};
    AZStd::string texType = "";
    ufbx_texture* tex = NULL;
    //TODO: How to deal with embedded textures.
    
    switch (textureType)
    {
    case MaterialMapType::Diffuse:
        tex = m_ufbxMaterial->fbx.diffuse_color.texture;
        break; 
    case MaterialMapType::Specular:
        tex = m_ufbxMaterial->fbx.specular_color.texture;
        break;
    case MaterialMapType::Bump:
        tex = m_ufbxMaterial->fbx.bump.texture;
        break;
    case MaterialMapType::Normal:
        tex = m_ufbxMaterial->fbx.normal_map.texture;
        break;
    case MaterialMapType::Metallic:
        tex = m_ufbxMaterial->pbr.metalness.texture;
        break;
    case MaterialMapType::Roughness:
        tex = m_ufbxMaterial->pbr.roughness.texture;
        break;
    case MaterialMapType::AmbientOcclusion:
        tex = m_ufbxMaterial->pbr.ambient_occlusion.texture;
        break;
    case MaterialMapType::Emissive:
        tex = m_ufbxMaterial->pbr.emission_color.texture;
        break;
    case MaterialMapType::BaseColor:
        tex = m_ufbxMaterial->pbr.base_color.texture;
        if(tex != NULL)
            tex = m_ufbxMaterial->fbx.diffuse_color.texture;
        break;
    default:
        using namespace AZ;
        AZ_TraceContext("Unknown Value", aznumeric_cast<int>(textureType));
        AZ_TracePrintf(SceneAPI::Utilities::WarningWindow, "Unrecognized MaterialMapType retrieved");
        return AZStd::string();
    }
    if(tex != NULL) 
        absTexFilePath = tex->absolute_filename;

    return AZStd::string(absTexFilePath.data);
}