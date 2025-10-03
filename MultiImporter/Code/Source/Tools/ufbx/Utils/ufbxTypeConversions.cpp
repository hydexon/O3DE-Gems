#include "ufbx.h"
#include <AzCore/Math/Matrix4x4.h>
#include "ufbxTypeConversions.h"

AZ::Matrix3x4 MultiImporter::ConvertUfbxMatrixToAzMatrix(const ufbx_matrix &um)
{
    return AZ::Matrix3x4::CreateFromRows(
        AZ::Vector4(static_cast<float>(um.m00), static_cast<float>(um.m01), static_cast<float>(um.m02), static_cast<float>(um.m03)),
        AZ::Vector4(static_cast<float>(um.m10), static_cast<float>(um.m11), static_cast<float>(um.m12), static_cast<float>(um.m13)),
        AZ::Vector4(static_cast<float>(um.m20), static_cast<float>(um.m21), static_cast<float>(um.m22), static_cast<float>(um.m23))
    );

}

AZ::Vector3 MultiImporter::TransformExtent(const AZ::Matrix3x4 &a, AZ::Vector3 &b)
{
    AZ::Vector3 ex =  a.TransformVector(AZ::Vector3(b.GetX(), 0, 0));
    AZ::Vector3 ey =  a.TransformVector(AZ::Vector3(0, b.GetY(), 0));
    AZ::Vector3 ez =  a.TransformVector(AZ::Vector3(0, 0, b.GetZ()));

    return AZ::Vector3(
        fabsf(ex.GetX()) + fabsf(ex.GetX()) + fabsf(ex.GetX()),
        fabsf(ey.GetY()) + fabsf(ey.GetY()) + fabsf(ey.GetY()),
        fabsf(ez.GetZ()) + fabsf(ez.GetZ()) + fabsf(ez.GetZ())
    );
}
