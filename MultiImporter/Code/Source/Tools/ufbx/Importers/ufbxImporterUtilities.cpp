#include "ufbxImporterUtilities.h"

#include "../Wrappers/ufbxNodeWrapper.h"
#include "../Wrappers/ufbxSceneWrapper.h"
#include "ufbx.h"

struct PoseMatch {
    ufbx_pose* pose;
    size_t node_index;
};

size_t FindBindPoseContainingNode(const ufbx_scene* scene, const ufbx_node* target, std::vector<PoseMatch>& matches)
{
    size_t count = 0;
    for(size_t pi = 0; pi < scene->poses.count; pi++)
    {
        ufbx_pose* pose = scene->poses.data[pi];
        if(!pose->is_bind_pose)
            continue;

        for(size_t ni = 0; ni < pose->bone_poses.count; ni++)
        {
            if(pose->bone_poses.data[ni].bone_node == target)
            {
                PoseMatch pm;
                pm.pose = pose;
                pm.node_index = ni;
                matches.push_back(pm);
                count++;
                break;
            }
        }
    }
    return count;
}

bool MultiImporter::IsSkinnedMesh(const MultiImporter::ufbxNodeWrapper &sourceNode)
{
    ufbx_node& node = *sourceNode.GetUbfxNode();
    if(node.mesh == NULL)
        return false;

    return node.mesh->skin_deformers.count > 0;
}

bool MultiImporter::GetBindPoseLocalTransform(const MultiImporter::ufbxSceneWrapper &sceneWrapper, MultiImporter::ufbxNodeWrapper &nodeWrapper, AZ::Transform &xf)
{
    const ufbx_scene* scene = sceneWrapper.GetUfbxScene();
    const ufbx_node* node = nodeWrapper.GetUbfxNode();

    std::vector<PoseMatch> PoseMatchList;
    ufbx_transform nodeTransform;
    if(FindBindPoseContainingNode(scene, node, PoseMatchList) > 0)
    {
       nodeTransform = scene->nodes[PoseMatchList[0].node_index]->local_transform;
    } else{ return false; }

    ufbx_node* parent = node->parent;
    ufbx_transform parentTransform;

    if(parent) {
        PoseMatchList.clear();
        if(FindBindPoseContainingNode(scene, parent, PoseMatchList) > 0)
        {
            parentTransform = scene->nodes[PoseMatchList[0].node_index]->local_transform;
        }
        else 
        {
        }
    }

    return false;
}
