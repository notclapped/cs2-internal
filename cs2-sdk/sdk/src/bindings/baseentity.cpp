#include "pch.hpp"

#include <bindings/baseentity.hpp>
#include <logger/logger.hpp>

#include <math/types/bbox.hpp>
#include <math/math.hpp>

#include <signatures/signatures.hpp>
#include <virtual/virtual.hpp>

#include <interfaces/schemasystem.hpp>

#include <imgui/imgui_internal.h>

bool C_BaseEntity::IsPlayerController() {
    CEntityIdentity* identity = m_pEntity();
    if (!identity) return false;
    const char* designer_name = identity->m_designerName();
    if (!designer_name) return false;
    return strcmp(designer_name, "cs_player_controller") == 0 || strcmp(designer_name, "controller") == 0;
}

bool C_BaseEntity::IsWeapon() {
    CEntityIdentity* identity = m_pEntity();
    if (!identity) return false;
    const char* designer_name = identity->m_designerName();
    if (!designer_name) return false;
    return strstr(designer_name, "weapon_") != nullptr;
}

bool C_BaseEntity::IsProjectile() {
    CEntityIdentity* identity = m_pEntity();
    if (!identity) return false;
    const char* designer_name = identity->m_designerName();
    if (!designer_name) return false;
    return strstr(designer_name, "_projectile") != nullptr || strstr(designer_name, "decoy") != nullptr || strstr(designer_name, "flashbang") != nullptr;
}

bool C_BaseEntity::IsPlantedC4() {
    CEntityIdentity* identity = m_pEntity();
    if (!identity) return false;
    const char* designer_name = identity->m_designerName();
    if (!designer_name) return false;
    return strcmp(designer_name, "planted_c4") == 0;
}

bool C_BaseEntity::IsChicken() {
    CEntityIdentity* identity = m_pEntity();
    if (!identity) return false;
    const char* designer_name = identity->m_designerName();
    if (!designer_name) return false;
    return strcmp(designer_name, "chicken") == 0;
}

bool C_BaseEntity::IsHostage() {
    CEntityIdentity* identity = m_pEntity();
    if (!identity) return false;
    const char* designer_name = identity->m_designerName();
    if (!designer_name) return false;
    return strcmp(designer_name, "hostage_entity") == 0;
}

bool C_BaseEntity::CalculateBBoxByCollision(BBox_t& out) {
    CGameSceneNode* node = m_pGameSceneNode();
    if (!node) return false;

    CCollisionProperty* collision = m_pCollision();
    if (!collision) return false;

    const Vector mins = collision->m_vecMins(), maxs = collision->m_vecMaxs();

    out.Invalidate();

    for (int i = 0; i < 8; ++i) {
        const Vector worldPoint{i & 1 ? maxs.x : mins.x, i & 2 ? maxs.y : mins.y, i & 4 ? maxs.z : mins.z};

        if (!CMath::Get().WorldToScreen(worldPoint.Transform(node->m_nodeToWorld().ToMatrix()), out.m_Vertices[i])) return false;

        out.m_Mins = ImMin(out.m_Mins, out.m_Vertices[i]);
        out.m_Maxs = ImMax(out.m_Maxs, out.m_Vertices[i]);
    }

    return true;
}

bool C_BaseEntity::CalculateBBoxByHitbox(BBox_t& out) {
    constexpr int MAX_HITBOXES = 64;

    CHitBoxSet* hitBoxSet = GetHitboxSet(0);
    if (!hitBoxSet) return false;

    const CUtlVector<CHitBox>& hitBoxes = hitBoxSet->m_HitBoxes();
    if (hitBoxes.m_Size > MAX_HITBOXES) {
        CLogger::Log("[bbox] hitBoxTransforms[{}] way too small! (Debug?)", MAX_HITBOXES);
        return false;
    }

    CTransform hitBoxTransforms[MAX_HITBOXES];
    int hitBoxCount = HitboxToWorldTransforms(hitBoxSet, hitBoxTransforms);
    if (!hitBoxCount) return false;

    Vector mins{std::numeric_limits<float>::max()}, maxs{-std::numeric_limits<float>::max()};
    for (int i = 0; i < hitBoxCount; ++i) {
        const matrix3x4_t hitBoxMatrix = hitBoxTransforms[i].ToMatrix();
        auto hitBox = hitBoxes.AtPtr(i);

        Vector worldMins, worldMaxs;
        CMath::Get().TransformAABB(hitBoxMatrix, hitBox->m_vMinBounds(), hitBox->m_vMaxBounds(), worldMins, worldMaxs);

        mins = mins.Min(worldMins);
        maxs = maxs.Max(worldMaxs);
    }

    out.Invalidate();

    for (int i = 0; i < 8; ++i) {
        const Vector worldPoint{i & 1 ? maxs.x : mins.x, i & 2 ? maxs.y : mins.y, i & 4 ? maxs.z : mins.z};

        if (!CMath::Get().WorldToScreen(worldPoint, out.m_Vertices[i])) return false;

        out.m_Mins = ImMin(out.m_Mins, out.m_Vertices[i]);
        out.m_Maxs = ImMax(out.m_Maxs, out.m_Vertices[i]);
    }

    return true;
}

CHitBoxSet* C_BaseEntity::GetHitboxSet(int i) { return signatures::GetHitboxSet.GetPtr().Call<CHitBoxSet* (*)(void*, int)>(this, i); }

int C_BaseEntity::HitboxToWorldTransforms(CHitBoxSet* hitBoxSet, CTransform* hitboxToWorld) {
    return signatures::HitboxToWorldTransforms.GetPtr().Call<int (*)(void*, CHitBoxSet*, CTransform*, int)>(this, hitBoxSet, hitboxToWorld,
                                                                                                            1024);
}
