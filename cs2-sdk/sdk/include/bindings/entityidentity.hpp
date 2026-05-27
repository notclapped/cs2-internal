#pragma once

class CEntityIdentity {
   public:
    SCHEMA(uint32_t, m_flags, "CEntityIdentity", "m_flags");
    SCHEMA(const char*, m_designerName, "CEntityIdentity", "m_designerName");
};
