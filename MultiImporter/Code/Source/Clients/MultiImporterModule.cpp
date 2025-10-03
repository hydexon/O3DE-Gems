
#include <MultiImporter/MultiImporterTypeIds.h>
#include <MultiImporterModuleInterface.h>
#include "MultiImporterSystemComponent.h"

namespace MultiImporter
{
    class MultiImporterModule
        : public MultiImporterModuleInterface
    {
    public:
        AZ_RTTI(MultiImporterModule, MultiImporterModuleTypeId, MultiImporterModuleInterface);
        AZ_CLASS_ALLOCATOR(MultiImporterModule, AZ::SystemAllocator);
    };
}// namespace MultiImporter

#if defined(O3DE_GEM_NAME)
AZ_DECLARE_MODULE_CLASS(AZ_JOIN(Gem_, O3DE_GEM_NAME), MultiImporter::MultiImporterModule)
#else
AZ_DECLARE_MODULE_CLASS(Gem_MultiImporter, MultiImporter::MultiImporterModule)
#endif
