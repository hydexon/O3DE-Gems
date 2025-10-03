#include "ufbxImportContextProvider.h"
#include "ufbxImportContexts.h"

#include <AzCore/std/smart_ptr/make_shared.h>

MultiImporter::ufbxImportContextProvider::~ufbxImportContextProvider() = default;

AZStd::shared_ptr<AZ::SceneAPI::SceneBuilder::NodeEncounteredContext> MultiImporter::ufbxImportContextProvider::CreateNodeEncounteredContext(AZ::SceneAPI::Containers::Scene &scene, AZ::SceneAPI::Containers::SceneGraph::NodeIndex currentGraphPosition, const AZ::SceneAPI::SceneSystem &sourceSceneSystem, AZ::SceneAPI::SceneBuilder::RenamedNodesMap &nodeNameMap, AZ::SDKScene::SceneWrapperBase &sourceScene, AZ::SDKNode::NodeWrapper &sourceNode)
{
    //Need to cast to NodeWrapper specific implementations:
    auto ufbxNode  = azrtti_cast<ufbxNodeWrapper*>(&sourceNode);
    auto ufbxScene = azrtti_cast<ufbxSceneWrapper*>(&sourceScene);
    
    if(!ufbxNode)
    {
        AZ_Error("SceneBuilder", false, "Incorrect node type. Cannot create NodeEncounteredContext");
        return nullptr;
    }

    if(!ufbxScene)
    {
        AZ_Error("SceneBuilder", false, "Incorrect scene type. Cannot create NodeEncounteredContext");
        return nullptr;
    }


    auto context = AZStd::make_shared<ufbxNodeEncounteredContext>(scene, currentGraphPosition, *ufbxScene, sourceSceneSystem, nodeNameMap, *ufbxNode);
    context->m_contextProvider = this;
    return context;
}

AZStd::shared_ptr<AZ::SceneAPI::SceneBuilder::SceneDataPopulatedContextBase> MultiImporter::ufbxImportContextProvider::CreateSceneDataPopulatedContext(AZ::SceneAPI::SceneBuilder::NodeEncounteredContext &parent, AZStd::shared_ptr<AZ::SceneAPI::DataTypes::IGraphObject> graphData, const AZStd::string &dataName)
{
    //Downcast:
    ufbxNodeEncounteredContext* ufbxParent = azrtti_cast<ufbxNodeEncounteredContext*>(&parent);
    if(!ufbxParent)
    {
        AZ_Error("SceneBuilder", false, "Incorrect type of parent. Cannot create SceneDataPopulatedContext");
        return nullptr;
    }

    auto context = AZStd::make_shared<ufbxSceneDataPopulatedContext>(*ufbxParent, AZStd::move(graphData), dataName);
    context->m_contextProvider = this;
    return context;
}

AZStd::shared_ptr<AZ::SceneAPI::SceneBuilder::SceneNodeAppendedContextBase> MultiImporter::ufbxImportContextProvider::CreateSceneNodeAppendedContext(AZ::SceneAPI::SceneBuilder::SceneDataPopulatedContextBase &parent, AZ::SceneAPI::Containers::SceneGraph::NodeIndex newIndex)
{
    //Downcast again:
    ufbxSceneDataPopulatedContext* ufbxParent = azrtti_cast<ufbxSceneDataPopulatedContext*>(&parent);
    if(!ufbxParent)
    {
        AZ_Error("SceneBuilder", false, "Incorrect type of parent. Cannot creat SceneNodeAppendedContext ufbx");
        return nullptr;
    }

    auto context = AZStd::make_shared<ufbxSceneNodeAppendedContext>(*ufbxParent, newIndex);
    context->m_contextProvider = this;
    return context;

}

AZStd::shared_ptr<AZ::SceneAPI::SceneBuilder::SceneAttributeDataPopulatedContextBase> MultiImporter::ufbxImportContextProvider::CreateSceneAttributeDataPopulatedContext(AZ::SceneAPI::SceneBuilder::SceneNodeAppendedContextBase &parent, AZStd::shared_ptr<AZ::SceneAPI::DataTypes::IGraphObject> nodeData, const AZ::SceneAPI::Containers::SceneGraph::NodeIndex attributeNodeIndex, const AZStd::string &dataName)
{
    //Downcast, once again:
    ufbxSceneNodeAppendedContext* ufbxParent = azrtti_cast<ufbxSceneNodeAppendedContext*>(&parent);
    if(!ufbxParent)
    {
        AZ_Error("SceneBuilder", false, "Incorrect type of ufbx parent, Cannot create ufbxSceneAttributeDataPopulatedContext");
        return nullptr;
    }

    auto context = AZStd::make_shared<ufbxSceneAttributeDataPopulatedContext>(*ufbxParent, AZStd::move(nodeData), attributeNodeIndex, dataName);
    context->m_contextProvider = this;
    return context;
}

AZStd::shared_ptr<AZ::SceneAPI::SceneBuilder::SceneAttributeNodeAppendedContextBase> MultiImporter::ufbxImportContextProvider::CreateSceneAttributeNodeAppendedContext(AZ::SceneAPI::SceneBuilder::SceneAttributeDataPopulatedContextBase &parent, AZ::SceneAPI::Containers::SceneGraph::NodeIndex newIndex)
{
    //And again
    ufbxSceneAttributeDataPopulatedContext* ufbxParent = azrtti_cast<ufbxSceneAttributeDataPopulatedContext*>(&parent);
    if(!ufbxParent)
    {
        AZ_Error("SceneBuilder", false, "Incorrect type of ufbx parent, Cannot create ufbxSceneAttributeNodeAppendedContext");
        return nullptr;
    }

    auto context = AZStd::make_shared<ufbxSceneAttributeNodeAppendedContext>(*ufbxParent, newIndex);
    context->m_contextProvider = this;
    return context;
}

AZStd::shared_ptr<AZ::SceneAPI::SceneBuilder::SceneNodeAddedAttributesContextBase> MultiImporter::ufbxImportContextProvider::CreateSceneNodeAddedAttributesContext(AZ::SceneAPI::SceneBuilder::SceneNodeAppendedContextBase &parent)
{
    //And again.
    ufbxSceneNodeAppendedContext* ufbxParent = azrtti_cast<ufbxSceneNodeAppendedContext*>(&parent);
    if(!ufbxParent)
    {
        AZ_Error("SceneBuilder", false, "Incorrect type of ufbx parent, Cannot create ufbxSceneAttributeNodeAppendedContext");
        return nullptr;
    }

    auto context = AZStd::make_shared<ufbxSceneNodeAddedAttributesContext>(*ufbxParent);
    context->m_contextProvider = this;
    return context;
}

AZStd::shared_ptr<AZ::SceneAPI::SceneBuilder::SceneNodeFinalizeContextBase> MultiImporter::ufbxImportContextProvider::CreateSceneNodeFinalizeContext(AZ::SceneAPI::SceneBuilder::SceneNodeAddedAttributesContextBase &parent)
{
    //And again!, almost there.
    ufbxSceneNodeAddedAttributesContext* ufbxParent = azrtti_cast<ufbxSceneNodeAddedAttributesContext*>(&parent);
    if(!ufbxParent)
    {
        AZ_Error("SceneBuilder", false, "Incorrect type of ufbx parent, Cannot create ufbxSceneNodeFinalizeContext");
        return nullptr;
    } 

    auto context = AZStd::make_shared<ufbxSceneNodeFinalizeContext>(*ufbxParent);
    context->m_contextProvider = this;
    return context;
}

AZStd::shared_ptr<AZ::SceneAPI::SceneBuilder::FinalizeSceneContextBase> MultiImporter::ufbxImportContextProvider::CreateFinalizeSceneContext(AZ::SceneAPI::Containers::Scene &scene, const AZ::SceneAPI::SceneSystem &sourceSceneSystem, AZ::SDKScene::SceneWrapperBase &sourceScene, AZ::SceneAPI::SceneBuilder::RenamedNodesMap &nodeNameMap)
{
    //Finally!
    ufbxSceneWrapper* ufbxScene = azrtti_cast<ufbxSceneWrapper*>(&sourceScene);
    if(!ufbxScene)
    {
        AZ_Error("SceneBuilder", false, "Incorrect scene type, Cannot Create ufbxFinalizeSceneContext");
        return nullptr;
    }
    auto context = AZStd::make_shared<ufbxFinalizeSceneContext>(scene, *ufbxScene, sourceSceneSystem, nodeNameMap);
    context->m_contextProvider = this;
    return context;
}
