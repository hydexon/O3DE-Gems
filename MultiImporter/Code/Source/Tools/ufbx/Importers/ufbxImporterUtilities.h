#pragma once

#include <AzCore/std/containers/unordered_map.h>
#include <AzCore/std/string/string.h>
#include <SceneAPI/SceneCore/DataTypes/MatrixType.h>


namespace MultiImporter
{
    class ufbxNodeWrapper;
    class ufbxSceneWrapper;

    bool IsSkinnedMesh(const ufbxNodeWrapper& sourceNode);
    bool GetBindPoseLocalTransform(const ufbxSceneWrapper& sceneWrapper, ufbxNodeWrapper& nodeWrapper, AZ::Transform& xf);
}