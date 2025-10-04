
#include <MultiImporter/MultiImporterTypeIds.h>
#include <MultiImporterModuleInterface.h>
#include "MultiImporterEditorSystemComponent.h"

#include <AzCore/Module/DynamicModuleHandle.h>

#include "ufbx/Contexts/ufbxContextRegistryComponent.h"
#include "ufbx/Contexts/ufbxImportContextProvider.h"
#include "ufbx/Importers/ufbxMeshImporter.h"
#include "ufbx/Importers/ufbxSkinImporter.h"
#include "ufbx/Importers/ufbxTransformImporter.h"
#include "ufbx/Importers/ufbxBitangentStreamImporter.h"

namespace MultiImporter
{
    class MultiImporterEditorModule
        : public MultiImporterModuleInterface
    {
    public:
        AZ_RTTI(MultiImporterEditorModule, MultiImporterEditorModuleTypeId, MultiImporterModuleInterface);
        AZ_CLASS_ALLOCATOR(MultiImporterEditorModule, AZ::SystemAllocator);

        MultiImporterEditorModule()
        {
            m_sceneCoreModule = AZ::DynamicModuleHandle::Create("SceneCore");
            m_sceneCoreModule->Load(AZ::DynamicModuleHandle::LoadFlags::InitFuncRequired);
            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            // Add ALL components descriptors associated with this gem to m_descriptors.
            // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and EditContext.
            // This happens through the [MyComponent]::Reflect() function.
            m_descriptors.insert(m_descriptors.end(), {
                MultiImporterEditorSystemComponent::CreateDescriptor(),
                ufbxImporterContextRegistryComponent::CreateDescriptor(),
                ufbxMeshImporter::CreateDescriptor(),
                ufbxSkinImporter::CreateDescriptor(),
                ufbxTransformImporter::CreateDescriptor(),
                ufbxBitangentStreamImporter::CreateDescriptor()
            });
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         * Non-SystemComponents should not be added here
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList {
                azrtti_typeid<MultiImporterEditorSystemComponent>(),
            };
        }
    private:
        AZStd::unique_ptr<AZ::DynamicModuleHandle> m_sceneCoreModule;
    };
}// namespace MultiImporter

#if defined(O3DE_GEM_NAME)
AZ_DECLARE_MODULE_CLASS(AZ_JOIN(Gem_, O3DE_GEM_NAME, _Editor), MultiImporter::MultiImporterEditorModule)
#else
AZ_DECLARE_MODULE_CLASS(Gem_MultiImporter_Editor, MultiImporter::MultiImporterEditorModule)
#endif
