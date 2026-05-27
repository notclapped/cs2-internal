#include "pch.hpp"

#include <bindings/playerpawn.hpp>
#include <bindings/entityidentity.hpp>
#include <bindings/entityinstance.hpp>

bool C_CSPlayerPawnBase::IsPlayerPawn() {
    CEntityIdentity* identity = m_pEntity();
    if (!identity) return false;
    const char* name = identity->m_designerName();
    if (!name) return false;
    return strcmp(name, "player") == 0 ||
           strcmp(name, "cs_player_pawn") == 0 ||
           strcmp(name, "c_cs_player_for_precache") == 0;
}

bool C_CSPlayerPawnBase::IsObserverPawn() {
    CEntityIdentity* identity = m_pEntity();
    if (!identity) return false;
    const char* name = identity->m_designerName();
    if (!name) return false;
    return strcmp(name, "cs_observer_pawn") == 0 ||
           strcmp(name, "observer_pawn") == 0;
}
