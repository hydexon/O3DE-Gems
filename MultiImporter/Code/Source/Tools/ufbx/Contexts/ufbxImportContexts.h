#pragma once

#include <AzCore/RTTI/RTTI.h>
#include <AzCore/std/string/string.h>
#include <SceneAPI/SceneBuilder/ImportContexts/ImportContexts.h>
#include <SceneAPI/SceneCore/Containers/SceneGraph.h>

namespace AZ 
{
    namespace SceneAPI 
    {
        namespace SceneBuilder 
        {
            class RenamedNodesMap;
        }
    }
}

namespace MultiImporter
{
    class ufbxNodeWrapper;
    class ufbxSceneWrapper;
    
    struct ufbxImportContext 
    {
        AZ_RTTI(ufbxImportContext, "{01992AB1-BEB0-77CC-877A-B3B9A02132EB}");

        ufbxImportContext(const ufbxSceneWrapper& sourceScene, const AZ::SceneAPI::SceneSystem& sourceSceneSystem, ufbxNodeWrapper& sourceNode);

        const ufbxSceneWrapper& m_sourceScene;
        ufbxNodeWrapper& m_sourceNode;
        const AZ::SceneAPI::SceneSystem& m_sourceSceneSystem;
    };

    struct ufbxNodeEncounteredContext
        : public ufbxImportContext
        , public AZ::SceneAPI::SceneBuilder::NodeEncounteredContext
    {
        AZ_RTTI(ufbxNodeEncounteredContext, "{01992ABB-3ED5-7388-81DE-A7C6E22638F3}", ufbxImportContext, AZ::SceneAPI::SceneBuilder::NodeEncounteredContext);

        ufbxNodeEncounteredContext(AZ::SceneAPI::Containers::Scene& scene,
            AZ::SceneAPI::Containers::SceneGraph::NodeIndex currentGraphPosition,
            const ufbxSceneWrapper& sourceScene,
            const AZ::SceneAPI::SceneSystem& sourceSceneSystem,
            AZ::SceneAPI::SceneBuilder::RenamedNodesMap& nodeNameMap,
            ufbxNodeWrapper& sourceNode
        );

        ufbxNodeEncounteredContext(AZ::SceneAPI::Events::ImportEventContext& parent,
            AZ::SceneAPI::Containers::SceneGraph::NodeIndex currentGraphPosition,
            const ufbxSceneWrapper& sourceScene,
            const AZ::SceneAPI::SceneSystem& sourceSceneSystem,
            AZ::SceneAPI::SceneBuilder::RenamedNodesMap& nodeNameMap,
            ufbxNodeWrapper& sourceNode
        );
    };

    struct ufbxSceneDataPopulatedContext
        : public ufbxImportContext
        , public AZ::SceneAPI::SceneBuilder::SceneDataPopulatedContextBase
    {
        AZ_RTTI(ufbxSceneDataPopulatedContext, "{01992AC5-9CE6-7602-B997-E85836186ECD}", ufbxImportContext, AZ::SceneAPI::SceneBuilder::SceneDataPopulatedContextBase);
        
        ufbxSceneDataPopulatedContext(ufbxNodeEncounteredContext& parent,
            AZStd::shared_ptr<AZ::SceneAPI::DataTypes::IGraphObject> graphData,
            const AZStd::string& dataName
        );

        ufbxSceneDataPopulatedContext(AZ::SceneAPI::Containers::Scene& scene,
            AZ::SceneAPI::Containers::SceneGraph::NodeIndex currentGraphPosition,
            const ufbxSceneWrapper& sourceScene,
            const AZ::SceneAPI::SceneSystem& sourceSceneSystem,
            AZ::SceneAPI::SceneBuilder::RenamedNodesMap& nodeNameMap,
            ufbxNodeWrapper& sourceNode,
            AZStd::shared_ptr<AZ::SceneAPI::DataTypes::IGraphObject> nodeData,
            const AZStd::string& dataName
        );
    };

    struct ufbxSceneNodeAppendedContext
        : public ufbxImportContext
        , public AZ::SceneAPI::SceneBuilder::SceneNodeAppendedContextBase
    {
        AZ_RTTI(ufbxSceneNodeAppendedContext, "{01992AD8-D2AD-748F-B444-E5D712EB62A5}", ufbxImportContext, AZ::SceneAPI::SceneBuilder::SceneNodeAppendedContextBase);

        ufbxSceneNodeAppendedContext(ufbxSceneDataPopulatedContext& parent,  AZ::SceneAPI::Containers::SceneGraph::NodeIndex newIndex);
        ufbxSceneNodeAppendedContext(AZ::SceneAPI::Containers::Scene& scene,
            AZ::SceneAPI::Containers::SceneGraph::NodeIndex currentGraphPosition,
            const ufbxSceneWrapper& sourceScene,
            const AZ::SceneAPI::SceneSystem& sourceSceneSystem,
            AZ::SceneAPI::SceneBuilder::RenamedNodesMap& nodeNameMap,
            ufbxNodeWrapper& sourceNode
        );
    };

    struct ufbxSceneAttributeDataPopulatedContext
        : public ufbxImportContext
        , public AZ::SceneAPI::SceneBuilder::SceneAttributeDataPopulatedContextBase
    {
        AZ_RTTI(ufbxSceneAttributeDataPopulatedContext, "{01992ADE-EDB7-721E-BC35-992F4C003472}", ufbxImportContext, AZ::SceneAPI::SceneBuilder::SceneAttributeDataPopulatedContextBase);

        ufbxSceneAttributeDataPopulatedContext(ufbxSceneNodeAppendedContext& parent, AZStd::shared_ptr<AZ::SceneAPI::DataTypes::IGraphObject> nodeData, const AZ::SceneAPI::Containers::SceneGraph::NodeIndex attributeNodeIndex, const AZStd::string& dataName);
    };

    struct ufbxSceneAttributeNodeAppendedContext
        : public ufbxImportContext
        , public AZ::SceneAPI::SceneBuilder::SceneAttributeNodeAppendedContextBase
    {
        AZ_RTTI(ufbxSceneAttributeNodeAppendedContext, "{01992AF7-F6A4-735A-83F8-9EE4F4D6BE4D}", ufbxImportContext, AZ::SceneAPI::SceneBuilder::SceneAttributeNodeAppendedContextBase);

        ufbxSceneAttributeNodeAppendedContext(
            ufbxSceneAttributeDataPopulatedContext& parent, AZ::SceneAPI::Containers::SceneGraph::NodeIndex newIndex
        );
    };

    struct ufbxSceneNodeAddedAttributesContext 
        : public ufbxImportContext
        , public AZ::SceneAPI::SceneBuilder::SceneNodeAddedAttributesContextBase
    {
        AZ_RTTI(ufbxSceneNodeAddedAttributesContext, "{01992AFC-2E38-703F-BB6D-8951EB456CD8}", ufbxImportContext, AZ::SceneAPI::SceneBuilder::SceneNodeAddedAttributesContextBase);
        ufbxSceneNodeAddedAttributesContext(ufbxSceneNodeAppendedContext& parent);
    };

    struct ufbxSceneNodeFinalizeContext
        : public ufbxImportContext
        , public AZ::SceneAPI::SceneBuilder::SceneNodeFinalizeContextBase
    {
        AZ_RTTI(ufbxSceneNodeFinalizeContext, "{01992AFD-7B24-779E-B688-FB45B4E98E53}", ufbxImportContext, AZ::SceneAPI::SceneBuilder::SceneNodeFinalizeContextBase);
        ufbxSceneNodeFinalizeContext(ufbxSceneNodeAddedAttributesContext& parent);
    };

    struct ufbxFinalizeSceneContext
        : public AZ::SceneAPI::SceneBuilder::FinalizeSceneContextBase
    {
        AZ_RTTI(ufbxFinalizeSceneContext, "{01992B01-0CD0-7729-851A-58109136A844}", AZ::SceneAPI::SceneBuilder::FinalizeSceneContextBase);
        ufbxFinalizeSceneContext(AZ::SceneAPI::Containers::Scene& scene,
            const ufbxSceneWrapper& sourceScene,
            const AZ::SceneAPI::SceneSystem& sourceSceneSystem,
            AZ::SceneAPI::SceneBuilder::RenamedNodesMap& nodeNameMap);

        const ufbxSceneWrapper& m_sourceScene;
        const AZ::SceneAPI::SceneSystem& m_sourceSceneSystem;
    };
    
}