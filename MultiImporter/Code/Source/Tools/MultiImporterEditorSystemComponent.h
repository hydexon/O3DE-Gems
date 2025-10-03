
#pragma once

#include <AzToolsFramework/API/ToolsApplicationAPI.h>

#include <Clients/MultiImporterSystemComponent.h>

namespace MultiImporter
{
    /// System component for MultiImporter editor
    class MultiImporterEditorSystemComponent
        : public MultiImporterSystemComponent
        , protected AzToolsFramework::EditorEvents::Bus::Handler
    {
        using BaseSystemComponent = MultiImporterSystemComponent;
    public:
        AZ_COMPONENT_DECL(MultiImporterEditorSystemComponent);

        static void Reflect(AZ::ReflectContext* context);

        MultiImporterEditorSystemComponent();
        ~MultiImporterEditorSystemComponent();

    private:
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        // AZ::Component
        void Activate() override;
        void Deactivate() override;
    };
} // namespace MultiImporter
