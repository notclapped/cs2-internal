#pragma once
#include <bindings/baseentity.hpp>
#include <imgui/imgui.h>

struct BBox_t {
    ImVec2 m_Mins = {0.f, 0.f};
    ImVec2 m_Maxs = {0.f, 0.f};
    ImVec2 m_Vertices[8] = {};
    bool m_Valid = false;

    void Invalidate() { m_Valid = false; }
};

class CCachedBaseEntity {
   public:
    enum class Type : int { UNKNOWN = 0, PLAYER, GUN, CAPTIVE, HEN, HOSTAGE };

    CCachedBaseEntity() = default;

    CCachedBaseEntity(C_BaseEntity* ent, int index) : m_pEntity(ent), m_iIndex(index) {}
    virtual ~CCachedBaseEntity() = default;

    template <typename T = C_BaseEntity>
    T* Get() const {
        return reinterpret_cast<T*>(m_pEntity);
    }

    int GetIndex() const { return m_iIndex; }
    void SetHandle(CBaseHandle handle) { m_hHandle = handle; }
    CBaseHandle GetHandle() const { return m_hHandle; }

    virtual Type GetType() const { return Type::UNKNOWN; }
    virtual bool CanDoESP();
    virtual void DrawESP();
    virtual void CalculateDrawInfo();
    virtual void InvalidateDrawInfo();
    virtual void DrawBoundingBox(ImU32 color);

   public:
    BBox_t m_BBox;
    BBox_t m_RenderBBox;  // Dedicated buffer to survive multiple engine passes

   private:
    C_BaseEntity* m_pEntity = nullptr;
    int m_iIndex = -1;
    CBaseHandle m_hHandle{};
};
