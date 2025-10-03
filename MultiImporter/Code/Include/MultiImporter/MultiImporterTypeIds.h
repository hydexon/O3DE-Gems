
#pragma once

namespace MultiImporter
{
    // System Component TypeIds
    inline constexpr const char* MultiImporterSystemComponentTypeId = "{2D4E018E-24AD-4CF3-8617-4E5E7E0575E7}";
    inline constexpr const char* MultiImporterEditorSystemComponentTypeId = "{54A81CDD-DE79-4601-B53B-03D6FC614BC1}";

    // Module derived classes TypeIds
    inline constexpr const char* MultiImporterModuleInterfaceTypeId = "{51FC462E-928A-489A-AC5E-D37AAF8FB635}";
    inline constexpr const char* MultiImporterModuleTypeId = "{2A7D871C-3797-417E-B4B3-F91CB4A6AD4A}";
    // The Editor Module by default is mutually exclusive with the Client Module
    // so they use the Same TypeId
    inline constexpr const char* MultiImporterEditorModuleTypeId = MultiImporterModuleTypeId;

    // Interface TypeIds
    inline constexpr const char* MultiImporterRequestsTypeId = "{612846C3-E2E1-47D2-9A19-9D2955F9E4B8}";
} // namespace MultiImporter
