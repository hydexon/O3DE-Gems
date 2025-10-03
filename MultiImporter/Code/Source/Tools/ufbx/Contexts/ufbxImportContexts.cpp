#include "ufbxImportContexts.h"
#include "../Wrappers/ufbxSceneWrapper.h"
#include "../Wrappers/ufbxNodeWrapper.h"
#include "ufbx.h"

#include <SceneAPI/SceneCore/Events/ImportEventContext.h>

MultiImporter::ufbxImportContext::ufbxImportContext(const ufbxSceneWrapper &sourceScene, const AZ::SceneAPI::SceneSystem &sourceSceneSystem, ufbxNodeWrapper &sourceNode)
    : m_sourceScene(sourceScene)
    , m_sourceSceneSystem(sourceSceneSystem)
    , m_sourceNode(sourceNode)
{
}

MultiImporter::ufbxNodeEncounteredContext::ufbxNodeEncounteredContext(AZ::SceneAPI::Containers::Scene &scene, AZ::SceneAPI::Containers::SceneGraph::NodeIndex currentGraphPosition, const ufbxSceneWrapper &sourceScene, const AZ::SceneAPI::SceneSystem &sourceSceneSystem, AZ::SceneAPI::SceneBuilder::RenamedNodesMap &nodeNameMap, ufbxNodeWrapper &sourceNode)
    : ufbxImportContext(sourceScene, sourceSceneSystem, sourceNode)
    , NodeEncounteredContext(scene, currentGraphPosition, nodeNameMap)
{
}

MultiImporter::ufbxNodeEncounteredContext::ufbxNodeEncounteredContext(AZ::SceneAPI::Events::ImportEventContext &parent, AZ::SceneAPI::Containers::SceneGraph::NodeIndex currentGraphPosition, const ufbxSceneWrapper &sourceScene, const AZ::SceneAPI::SceneSystem &sourceSceneSystem, AZ::SceneAPI::SceneBuilder::RenamedNodesMap &nodeNameMap, ufbxNodeWrapper &sourceNode)
    : ufbxImportContext(sourceScene, sourceSceneSystem, sourceNode)
    , NodeEncounteredContext(parent.GetScene(), currentGraphPosition, nodeNameMap)
{
}

MultiImporter::ufbxSceneDataPopulatedContext::ufbxSceneDataPopulatedContext(ufbxNodeEncounteredContext &parent, AZStd::shared_ptr<AZ::SceneAPI::DataTypes::IGraphObject> graphData, const AZStd::string &dataName)
    : ufbxImportContext(parent.m_sourceScene, parent.m_sourceSceneSystem, parent.m_sourceNode)
    , SceneDataPopulatedContextBase(parent, AZStd::move(graphData), dataName)
{
}

MultiImporter::ufbxSceneDataPopulatedContext::ufbxSceneDataPopulatedContext(AZ::SceneAPI::Containers::Scene &scene, AZ::SceneAPI::Containers::SceneGraph::NodeIndex currentGraphPosition, const ufbxSceneWrapper &sourceScene, const AZ::SceneAPI::SceneSystem &sourceSceneSystem, AZ::SceneAPI::SceneBuilder::RenamedNodesMap &nodeNameMap, ufbxNodeWrapper &sourceNode, AZStd::shared_ptr<AZ::SceneAPI::DataTypes::IGraphObject> nodeData, const AZStd::string &dataName)
    : ufbxImportContext(sourceScene, sourceSceneSystem, sourceNode)
    , SceneDataPopulatedContextBase(scene, currentGraphPosition, nodeNameMap, AZStd::move(nodeData), dataName)
{
}

MultiImporter::ufbxSceneNodeAppendedContext::ufbxSceneNodeAppendedContext(ufbxSceneDataPopulatedContext &parent, AZ::SceneAPI::Containers::SceneGraph::NodeIndex newIndex)
    : ufbxImportContext(parent.m_sourceScene, parent.m_sourceSceneSystem, parent.m_sourceNode)
    , SceneNodeAppendedContextBase(parent.m_scene, newIndex, parent.m_nodeNameMap)
{
}

MultiImporter::ufbxSceneNodeAppendedContext::ufbxSceneNodeAppendedContext(AZ::SceneAPI::Containers::Scene &scene, AZ::SceneAPI::Containers::SceneGraph::NodeIndex currentGraphPosition, const ufbxSceneWrapper &sourceScene, const AZ::SceneAPI::SceneSystem &sourceSceneSystem, AZ::SceneAPI::SceneBuilder::RenamedNodesMap &nodeNameMap, ufbxNodeWrapper &sourceNode)
    : ufbxImportContext(sourceScene, sourceSceneSystem, sourceNode)
    , SceneNodeAppendedContextBase(scene, currentGraphPosition, nodeNameMap)
{
}


MultiImporter::ufbxSceneAttributeDataPopulatedContext::ufbxSceneAttributeDataPopulatedContext(ufbxSceneNodeAppendedContext &parent, AZStd::shared_ptr<AZ::SceneAPI::DataTypes::IGraphObject> nodeData, const AZ::SceneAPI::Containers::SceneGraph::NodeIndex attributeNodeIndex, const AZStd::string &dataName)
    : ufbxImportContext(parent.m_sourceScene, parent.m_sourceSceneSystem, parent.m_sourceNode)
    , SceneAttributeDataPopulatedContextBase(parent, AZStd::move(nodeData), attributeNodeIndex, dataName)
{
}

MultiImporter::ufbxSceneAttributeNodeAppendedContext::ufbxSceneAttributeNodeAppendedContext(ufbxSceneAttributeDataPopulatedContext &parent, AZ::SceneAPI::Containers::SceneGraph::NodeIndex newIndex)
    : ufbxImportContext(parent.m_sourceScene, parent.m_sourceSceneSystem, parent.m_sourceNode)
    , SceneAttributeNodeAppendedContextBase(parent, newIndex)
{
}

MultiImporter::ufbxSceneNodeAddedAttributesContext::ufbxSceneNodeAddedAttributesContext(ufbxSceneNodeAppendedContext &parent)
    : ufbxImportContext(parent.m_sourceScene, parent.m_sourceSceneSystem, parent.m_sourceNode)
    , SceneNodeAddedAttributesContextBase(parent)
{
}

MultiImporter::ufbxSceneNodeFinalizeContext::ufbxSceneNodeFinalizeContext(ufbxSceneNodeAddedAttributesContext &parent)
    : ufbxImportContext(parent.m_sourceScene, parent.m_sourceSceneSystem, parent.m_sourceNode)
    , SceneNodeFinalizeContextBase(parent)
{
}

MultiImporter::ufbxFinalizeSceneContext::ufbxFinalizeSceneContext(AZ::SceneAPI::Containers::Scene &scene, const ufbxSceneWrapper &sourceScene, const AZ::SceneAPI::SceneSystem &sourceSceneSystem, AZ::SceneAPI::SceneBuilder::RenamedNodesMap &nodeNameMap)
    : FinalizeSceneContextBase(scene, nodeNameMap)
    , m_sourceScene(sourceScene)
    , m_sourceSceneSystem(sourceSceneSystem)
{
}
