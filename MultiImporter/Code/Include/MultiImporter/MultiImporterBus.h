
#pragma once

#include <MultiImporter/MultiImporterTypeIds.h>

#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>

namespace MultiImporter
{
    class MultiImporterRequests
    {
    public:
        AZ_RTTI(MultiImporterRequests, MultiImporterRequestsTypeId);
        virtual ~MultiImporterRequests() = default;
        // Put your public methods here
    };

    class MultiImporterBusTraits
        : public AZ::EBusTraits
    {
    public:
        //////////////////////////////////////////////////////////////////////////
        // EBusTraits overrides
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
        //////////////////////////////////////////////////////////////////////////
    };

    using MultiImporterRequestBus = AZ::EBus<MultiImporterRequests, MultiImporterBusTraits>;
    using MultiImporterInterface = AZ::Interface<MultiImporterRequests>;

} // namespace MultiImporter
