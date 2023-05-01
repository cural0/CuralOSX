//
//  MyAimBot.cpp
//  ButOSX
//
//  Created by Michał Kural on 25/03/2023.
//  Copyright © 2023 Lyceion. All rights reserved.
//
#include <map>
#include "MyAimBot.hpp"
#include "CheatSettings.h"


CAim::CAim() {}

bool CheatSettings::Aim::rage = 0;
int CheatSettings::Aim::bone = 0;
float CheatSettings::Aim::fov = 3.5f;
float CheatSettings::Aim::smooth_factor = 0.75000f;
float CheatSettings::Aim::fov_multiplier;
bool CheatSettings::Aim::teamcheck = 1;
bool CheatSettings::Aim::auto_shoot;
bool CheatSettings::Aim::auto_scope;
bool CheatSettings::Extras::aimbot_key_check = 0;

std::map<int, const char*> CAim::GetBoneList() {
    std::map<int, const char*> select;

    select[0] = "0"; // HITBOX_HEAD
    select[1] = "1"; // HITBOX_NECK
    select[2] = "2"; // HITBOX_PELVIS
    select[3] = "3"; // HITBOX_BODY
    select[4] = "4"; // HITBOX_THORAX
    select[5] = "5"; // HITBOX_CHEST
    select[6] = "6"; // HITBOX_UPPER_CHEST
    select[7] = "7"; // HITBOX_RIGHT_THIGH

    return select;
}

const char* CAim::GetBoneValues[] {
    "HEAD",
    "NECK",
    "PELVIS",
    "BODY",
    "THORAX",
    "CHEST",
    "UPPER_CHEST",
    "RIGHT_THIGH",
    "LEFT_THIGH",
    "RIGHT_CALF",
    "LEFT_CALF"
};

bool IsVisible2(C_BaseEntity* pEntity, C_BaseEntity* pLocal) {
    Ray_t ray;
    trace_t trace;
    CTraceFilter filter;
    filter.pSkip = pLocal;
    
    ray.Init(pLocal->GetEyePosition(), pEntity->GetEyePosition());
    pEngineTrace->TraceRay(ray, 0x4600400B, &filter, &trace);
    
    return (trace.m_pEnt == pEntity || trace.fraction > 0.99f);
}

bool IsValid(QAngle angle)
{
    return std::isfinite(angle.x) && std::isfinite(angle.y) && std::isfinite(angle.z);
}

bool CAim::IsVisible(C_BasePlayer* localPlayer, Vector position, bool exact)
{
    if(!localPlayer->IsValidLivePlayer()) {
        return false;
    }
    
    
    
    trace_t tr;
    Ray_t ray;
    CTraceFilter filter;
    filter.pSkip = localPlayer;
    
    ray.Init(localPlayer->GetEyePosition(), position);
    uint32_t RayMask = MASK_SHOT;
    pEngineTrace->TraceRay(ray, RayMask, &filter, &tr);
    
    if (tr.allsolid || tr.startsolid) {
        return false;
    }
    float maxFraction = tr.fraction > 0.99f;
    if(exact) {
        maxFraction = tr.fraction >= 1.f;
    }
    if (tr.m_pEnt)
    {
        if (tr.m_pEnt == localPlayer || maxFraction)
        {
            return true;
        }
    }
    
    if (tr.fraction >= 1.0f)
    {
        return true;
    }

    return false;
}

void CAim::CreateMove(CUserCmd *pCmd) {

    if (!aimButton_AimBot->state) {
        return;
    }
    if (CheatSettings::Extras::aimbot_key_check){
        if (!pInputSystem->IsButtonDown((ButtonCode_t)KEY_LSHIFT)) {
            return;}
    }
    
    C_BasePlayer* LocalPlayer((C_BasePlayer*)pEntList->GetClientEntity(pEngine->GetLocalPlayer()));
    if(!LocalPlayer || !LocalPlayer->IsValidLivePlayer()) {
        if(LockedTargetEntity) {
            Reset();
        }
        return;
    }
    C_BaseCombatWeapon* currentWeapon = (C_BaseCombatWeapon*)pEntList->GetClientEntityFromHandle(LocalPlayer->GetActiveWeapon());
    //C_BaseCombatWeapon* currentWeapon = LocalPlayer->GetActiveWeapon();
    /*if(!currentWeapon) {
        if(LockedTargetEntity) {
            Reset();
            pCvar->ConsolePrintf("%s \n", "weapon broken");
        }
        return;
        
    }
    
    int currentWeaponID = currentWeapon->EntityId();
    if (
        CWeaponManager::isC4(currentWeaponID) ||
        CWeaponManager::isKnife(currentWeaponID) ||
        CWeaponManager::isGrenade(currentWeaponID) ||
        CWeaponManager::isNonAimWeapon(currentWeaponID)
    ) {
        if(LockedTargetEntity) {
            Reset();
            pCvar->ConsolePrintf("%s \n", "weapon is knife or grenade or c4");
        }
        return;
    }
    
    if (
        (
            !(pCmd->buttons & IN_ATTACK) &&
            currentWeaponID != en_item_definition_index::weapon_revolver
        ) ||
        (
            !(pCmd->buttons & IN_ATTACK2) &&
            currentWeaponID == en_item_definition_index::weapon_revolver
        )
    ) {
        if(LockedTargetEntity) {
            Reset();
        }
        return;
    }*/
    
    C_BaseEntity* TargetEntity = nullptr;
    if (!LockedTargetEntity) {
        TargetEntity = FindTarget(LocalPlayer);
        if (!TargetEntity || !TargetEntity->IsValidLivePlayer() || TargetEntity->IsImmune()) {
            Reset();
            return;
        }
        LockedTargetEntity = TargetEntity;
    } else {
        TargetEntity = LockedTargetEntity;
    }

    if (!TargetEntity || !TargetEntity->IsValidLivePlayer() || TargetEntity->IsImmune()) {
        Reset();
        return;
    }
    
    //if (INIGET_BOOL("Rage", "enabled") && INIGET_BOOL("Rage", "engine_predict") && INIGET_INT("Rage", "engine_predict_mode") == 0) {
    if (CheatSettings::Aim::rage){
        CEnginePrediction::Instance()->Run(pCmd, currentWeapon, LocalPlayer);
    }
    
   // CEnginePrediction::Instance()->Start(pCmd);

    StartAim(LocalPlayer, TargetEntity, pCmd);
   // CEnginePrediction::Instance()->End();
}


C_BaseEntity* CAim::FindTarget(C_BasePlayer* LocalPlayer) {
    float fov = CheatSettings::Aim::fov;
    C_BaseEntity* TargetEntity = nullptr;
    
    if(CheatSettings::Aim::rage){
        fov = fov * CheatSettings::Aim::fov_multiplier;
    }

    Vector LocalPlayerEyePosition = LocalPlayer->GetEyePosition();
    QAngle LocalPlayerViewAngle = LocalPlayer->GetHeadRotation();
    
    for(int i = 1; i < pEngine->GetMaxClients(); i++) {
        C_BaseEntity* PossibleTarget = (C_BaseEntity*)pEntList->GetClientEntity(i);;
        
        if(!PossibleTarget || PossibleTarget == LocalPlayer || !PossibleTarget->IsValidLivePlayer() || PossibleTarget->IsImmune() || PossibleTarget->IsGhost() || PossibleTarget->GetHealth() < 1) {
            continue;
        }

        if(CheatSettings::Aim::teamcheck && LocalPlayer->GetTeam() == PossibleTarget->GetTeam()) {
            continue;
        }
        if(PossibleTarget->GetTeam() < 2)
        {
            continue;
        }
        
        /*int aimHitbox;
        if (INIGET_BOOL("Rage", "enabled") && INIGET_BOOL("Rage", "hit_scan")) {
            aimHitbox = MakeHitscan(LocalPlayer, PossibleTarget);
        } else {
            aimHitbox = INIGET_INT("AimHelper", "aim_hitbox");
        }*/
        
        Vector PossibleTargetHitbox;
        
        if (CheatSettings::Aim::rage == 1) {
            //pGlobalVars->curtime;;
            
            PossibleTargetHitbox = ExtrapolateTick(GetHitboxPosition(PossibleTarget, CheatSettings::Aim::bone), PossibleTarget->GetVelocity());
        } else {
            PossibleTargetHitbox = GetHitboxPosition(PossibleTarget, CheatSettings::Aim::bone);
        }
        //Vector PossibleTargetHitbox = ExtrapolateTick(GetHitboxPosition(PossibleTarget, CheatSettings::Aim::bone), PossibleTarget->GetVelocity());
        
        if(!IsVisible(LocalPlayer, PossibleTargetHitbox)) {
            continue;
        }

        //if(!IsVisible2(PossibleTarget, LocalPlayer)){
        //    continue;
        //}

        float FieldOfViewToTarget = Math::GetFov(LocalPlayerViewAngle, LocalPlayerEyePosition, PossibleTargetHitbox);

        if(FieldOfViewToTarget < fov) {
            TargetEntity = PossibleTarget;
            fov = FieldOfViewToTarget;
            //pCvar->ConsolePrintf("%s \n", "got target, its valid and alive, proceed to aiming");
        }
        
    }

    return TargetEntity;
    
}

void CAim::StartAim(C_BasePlayer* LocalPlayer, C_BaseEntity* AimTarget, CUserCmd* pCmd) {
    
    
    QAngle AimAngle;
    QAngle OldAngle;
    pEngine->GetViewAngles(OldAngle);

    float oldForward = pCmd->forwardmove;
    float oldSideMove = pCmd->sidemove;
    
    
    int LockedBone;
    if (LockedEntityBone == -5) {
        if (CheatSettings::Aim::rage == 1) {
            LockedBone = 0;
        } else {
            LockedBone = CheatSettings::Aim::bone;
        }
        LockedEntityBone = LockedBone;
    } else {
        LockedBone = LockedEntityBone;
    }
    
    Vector TargetHitbox;
    
    if (CheatSettings::Aim::rage == 1) {
        TargetHitbox = ExtrapolateTick(GetHitboxPosition(AimTarget, LockedBone), AimTarget->GetVelocity());
    } else {
        TargetHitbox = GetHitboxPosition(AimTarget, LockedBone);
    }

    //Vector TargetHitbox = GetPredictedPosition(CheatSettings::Aim::bone, AimTarget);
    
    if(!IsVisible(LocalPlayer, TargetHitbox)) {
        Reset();
        return;
    }
    
    
    Vector dir = LocalPlayer->GetEyePosition() - TargetHitbox;
    
    
    Math::VectorNormalize(dir);
    Math::VectorAngles(dir, AimAngle);

    RecoilControlBot(AimAngle, LocalPlayer, AimTarget, pCmd);
    
    if (CheatSettings::Aim::rage == 0) {
        Smooth(AimAngle);
    } else {
        CAim::AutoCrouch(LocalPlayer, AimTarget, pCmd);
        CAim::AutoStop(LocalPlayer, AimTarget, pCmd);
    }
    
    
    if (oldForward != pCmd->forwardmove) {
        oldForward = pCmd->forwardmove;
    }
    if (oldSideMove != pCmd->sidemove) {
        oldSideMove = pCmd->sidemove;
    }
    
    if (!AimTarget->IsValidLivePlayer()) {
        //pCvar->ConsolePrintf("%s \n", "reset cause target invalid");
        Reset();
        return;
    }


    if (AimAngle != OldAngle) {
        Math::NormalizeAngles(AimAngle);
        Math::ClampAngle(AimAngle);
        if(AimAngle.IsValid()) {
            if (!HasValidTargetAndIsAiming) {
                HasValidTargetAndIsAiming = true;
            }
        
        //if(true) {
            if(CheatSettings::Aim::rage == 1) {
                pCmd->viewangles = AimAngle;
            } else {
                Math::CorrectMovement(OldAngle, pCmd, oldForward, oldSideMove);
                //pCvar->ConsolePrintf("%s \n", "aiming");
                pEngine->SetViewAngles(AimAngle);
                
                
            }
            
            if (CheatSettings::Aim::auto_shoot == 1) {
                C_BaseCombatWeapon* activeWeapon = (C_BaseCombatWeapon*)pEntList->GetClientEntityFromHandle(LocalPlayer->GetActiveWeapon());
                CAim::AutoCrouch(LocalPlayer, AimTarget, pCmd);
                CAim::AutoStop(LocalPlayer, AimTarget, pCmd);
                CAim::AutoShoot(AimTarget, activeWeapon, pCmd);
            }
        }
    }
}

void CAim::Reset() {
    
    LockedEntityBone = -5;
    LockedTargetEntity = nullptr;
    HasValidTargetAndIsAiming = false;
}

void CAim::RecoilControlBot(QAngle& angle, C_BasePlayer* LocalPlayer, C_BaseEntity* TargetEntity, CUserCmd* pCmd) {
    //if (CheatSettings::Aim::rage == 0) {
    //    return;
    //}

    //if (!(pCmd->buttons & IN_ATTACK)) {
    //    return;
    //}
    //if (!CAim::HasTarget()) {
    //    return;
    //}
    
    //C_BaseCombatWeapon* currentWeapon = (C_BaseCombatWeapon*)pEntList->GetClientEntityFromHandle(LocalPlayer->GetActiveWeapon());
    //if(!currentWeapon) {
    //    return;
    //}
    //int currentWeaponID = currentWeapon->EntityId();

    QAngle CurrentPunch = LocalPlayer->GetAimPunchAngle();
    if (CurrentPunch.x != 0.0f || CurrentPunch.y != 0.0f) {
        if (CheatSettings::Aim::rage == 0) {
            //if (CWeaponManager::isRCSWeapon(currentWeaponID)) {
                //angle.x -= CurrentPunch.x * CheatSettings::Aim::rcs_level;
                //angle.y -= CurrentPunch.y * CheatSettings::Aim::rcs_level;
                angle.x -= CurrentPunch.x * 2.0f;
                angle.y -= CurrentPunch.y * 2.0f;
           // }
        } else {
            angle.x -= CurrentPunch.x * 2.0f;
            angle.y -= CurrentPunch.y * 2.0f;
        }
    }
    
}

void CAim::Salt(float& smooth) {
    float sine = sin(pGlobalVars->tickcount);
    float salt = sine * 0.042f;
    float oval = smooth + salt;

    smooth *= oval;
}

void CAim::Smooth(QAngle& angle)
{
    if (!true) {
        return;
    }

    QAngle viewAngles = QAngle(0.f, 0.f, 0.f);
    pEngine->GetViewAngles(viewAngles);

    QAngle delta = angle - viewAngles;
    Math::NormalizeAngles(delta);

    float smooth = powf(CheatSettings::Aim::smooth_factor, 0.4f);
    smooth = std::min(0.99f, smooth);
    
    CAim::Salt(smooth);
    
    QAngle toChange = QAngle();
    
    float coeff = (1.0f - smooth) / delta.Length() * 4.0f;
    coeff = std::min(1.0f, coeff);
    toChange = delta * coeff;

    //toChange = delta - delta * smooth;
    

    angle = viewAngles + toChange;
}

bool CAim::HasTarget() {
    return HasValidTargetAndIsAiming &&
           LockedTargetEntity != nullptr &&
           LockedEntityBone != -5;
}

void CAim::AutoCrouch(C_BasePlayer* LocalPlayer, C_BaseEntity* EntityPlayer, CUserCmd* pCmd) {
   // if (!Options::Rage::autocrouch) {
   //     return;
   // }

    if (!CAim::HasTarget()) {
        return;
    }

    if (!LocalPlayer || !LocalPlayer->IsValidLivePlayer()) {
        return;
    }

    pCmd->buttons |= IN_DUCK;

}

void CAim::AutoStop(C_BasePlayer* LocalPlayer, C_BaseEntity* EntityPlayer, CUserCmd* pCmd) {
  //  if (!Options::Rage::auto_stop) {
  //      return;
  //  }

    if (!CAim::HasTarget()) {
        return;
    }

    if (!LocalPlayer || !LocalPlayer->IsValidLivePlayer()) {
        return;
    }

    pCmd->forwardmove = 0.f;
    pCmd->sidemove = 0.f;
    pCmd->upmove = 0.f;
        
    
}


void CAim::AutoShoot(C_BaseEntity* player, C_BaseCombatWeapon* activeWeapon, CUserCmd* pCmd) {
    if (CheatSettings::Aim::auto_shoot == 0) {
        return;
    }

    //if (!player || !activeWeapon || activeWeapon->GetAmmo() == 0) {
    //    return;
    //}

    if (!CAim::HasTarget()) {
        return;
    }

   // if (!CWeaponManager::IsValidWeapon(activeWeapon->EntityId())) {
    //    return;
    //}

    if (pCmd->buttons & IN_USE) {
        return;
    }

    C_BasePlayer* localplayer((C_BasePlayer*)pEntList->GetClientEntity(pEngine->GetLocalPlayer()));
    float nextPrimaryAttack = activeWeapon->GetNextPrimaryAttack();

    if (nextPrimaryAttack > pGlobalVars->curtime) {
        if (!CWeaponManager::isRCSWeapon(activeWeapon->EntityId())) {
            if (activeWeapon->EntityId() == en_item_definition_index::weapon_revolver) {
                pCmd->buttons &= ~IN_ATTACK2;
            } else {
                pCmd->buttons &= ~IN_ATTACK;
            }
        }
    } else {
        if (CheatSettings::Aim::auto_scope && GetCSWpnData(activeWeapon)->m_iZoomLevels > 0 && !localplayer->IsScoped()) {
            pCmd->buttons |= IN_ATTACK2;
        } else if (activeWeapon->EntityId() == en_item_definition_index::weapon_revolver) {
            pCmd->buttons |= IN_ATTACK2;
        } else {
            pCmd->buttons |= IN_ATTACK;
        }
    }
}
