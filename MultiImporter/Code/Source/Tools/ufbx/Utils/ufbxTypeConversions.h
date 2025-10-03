#pragma once

#include <AzCore/Math/Aabb.h>
#include <AzCore/Math/Vector3.h>

struct ufbx_matrix;

namespace MultiImporter
{
    AZ::Matrix3x4 ConvertUfbxMatrixToAzMatrix(const ufbx_matrix& um);
    AZ::Vector3 TransformExtent(const AZ::Matrix3x4& a, AZ::Vector3& b);
}
