#pragma once

#include <SceneAPI/SceneBuilder/ImportContexts/ImportContextProvider.h>

#include <AzCore/RTTI/RTTIMacros.h>
#include "../Wrappers/ufbxSceneWrapper.h"
#include "../Wrappers/ufbxNodeWrapper.h"

namespace MultiImporter
{
    class ufbxImportContextProvider : public AZ::SceneAPI::SceneBuilder::ImportContextProvider
    {
    public:
        AZ_RTTI(ufbxImportContextProvider, "{01992B0E-D711-7153-8A3B-3CB5D698B995}")

        ufbxImportContextProvider() = default;
        ~ufbxImportContextProvider() override;

        AZStd::shared_ptr<AZ::SceneAPI::SceneBuilder::NodeEncounteredContext> CreateNodeEncounteredContext(
            AZ::SceneAPI::Containers::Scene& scene,
            AZ::SceneAPI::Containers::SceneGraph::NodeIndex currentGraphPosition,
            const AZ::SceneAPI::SceneSystem& sourceSceneSystem,
            AZ::SceneAPI::SceneBuilder::RenamedNodesMap& nodeNameMap,
            AZ::SDKScene::SceneWrapperBase& sourceScene,
            AZ::SDKNode::NodeWrapper& sourceNode) override;

        AZStd::shared_ptr<AZ::SceneAPI::SceneBuilder::SceneDataPopulatedContextBase> CreateSceneDataPopulatedContext(
            AZ::SceneAPI::SceneBuilder::NodeEncounteredContext& parent,
            AZStd::shared_ptr<AZ::SceneAPI::DataTypes::IGraphObject> graphData,
            const AZStd::string& dataName) override;

        AZStd::shared_ptr<AZ::SceneAPI::SceneBuilder::SceneNodeAppendedContextBase> CreateSceneNodeAppendedContext(
            AZ::SceneAPI::SceneBuilder::SceneDataPopulatedContextBase& parent,
            AZ::SceneAPI::Containers::SceneGraph::NodeIndex newIndex) override;


        AZStd::shared_ptr<AZ::SceneAPI::SceneBuilder::SceneAttributeDataPopulatedContextBase> CreateSceneAttributeDataPopulatedContext(
            AZ::SceneAPI::SceneBuilder::SceneNodeAppendedContextBase& parent,
            AZStd::shared_ptr<AZ::SceneAPI::DataTypes::IGraphObject> nodeData,
            const AZ::SceneAPI::Containers::SceneGraph::NodeIndex attributeNodeIndex,
            const AZStd::string& dataName) override;

        AZStd::shared_ptr<AZ::SceneAPI::SceneBuilder::SceneAttributeNodeAppendedContextBase> CreateSceneAttributeNodeAppendedContext(
            AZ::SceneAPI::SceneBuilder::SceneAttributeDataPopulatedContextBase& parent,
            AZ::SceneAPI::Containers::SceneGraph::NodeIndex newIndex) override;

        AZStd::shared_ptr<AZ::SceneAPI::SceneBuilder::SceneNodeAddedAttributesContextBase> CreateSceneNodeAddedAttributesContext(
            AZ::SceneAPI::SceneBuilder::SceneNodeAppendedContextBase& parent
        ) override;

        AZStd::shared_ptr<AZ::SceneAPI::SceneBuilder::SceneNodeFinalizeContextBase> CreateSceneNodeFinalizeContext(
            AZ::SceneAPI::SceneBuilder::SceneNodeAddedAttributesContextBase& parent
        ) override;

        AZStd::shared_ptr<AZ::SceneAPI::SceneBuilder::FinalizeSceneContextBase> CreateFinalizeSceneContext(
            AZ::SceneAPI::Containers::Scene& scene,
            const AZ::SceneAPI::SceneSystem& sourceSceneSystem,
            AZ::SDKScene::SceneWrapperBase& sourceScene,
            AZ::SceneAPI::SceneBuilder::RenamedNodesMap& nodeNameMap
        ) override;

        AZStd::string GetImporterName() const override 
        {
            return "ufbx";
        }

        bool CanHandleExtension(AZStd::string_view file_extension) const override
        {
            //AZ_Info("SceneAPI", "ufbxImportContextProvider , trying to get extensions: %s", AZ_STRING_ARG(file_extension));
            return (file_extension.contains("fbx"));
        }

        AZStd::unique_ptr<AZ::SDKScene::SceneWrapperBase> CreateSceneWrapper() const override
        {
            return AZStd::make_unique<ufbxSceneWrapper>();
        }
    };
}