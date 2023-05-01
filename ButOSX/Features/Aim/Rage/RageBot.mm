/******************************************************/
/**                                                  **/
/**      Rage/RageBot.mm                             **/
/**      |_ @package GO:SX Legit                     **/
/**      |_ @created 2018-08-13                      **/
/**      |_ @author André "vAx" Kalisch              **/
/**                                                  **/
/**      Copyright © 2017 @author                    **/
/**      All rights reserved.                        **/
/**                                                  **/
/******************************************************/

#include "RageBot.h"

float CheatSettings::Rage::fov_multiplier = 35.0f;
bool CheatSettings::Rage::auto_shoot = 1;
bool CheatSettings::Rage::auto_scope = 0;
bool CheatSettings::Rage::autocrouch = false;
bool CheatSettings::Rage::auto_stop = false;
bool CheatSettings::Rage::hitchance = false;
int CheatSettings::Rage::hitchance_shots = 30;
float CheatSettings::Rage::hitchance_percent = 65.0f;
bool CheatSettings::Rage::hit_scan = true;
int CheatSettings::Rage::fixed_bone = 0;
bool CheatSettings::Rage::auto_wall = true;
float CheatSettings::Rage::autowall_min_damage = 5.0f;
bool CheatSettings::Rage::silent_aim = true;
//bool *Glob::AimbotIsAiming;



CRageBot::CRageBot() {}

void CRageBot::CreateMove(CUserCmd* pCmd) {
    userCommand = pCmd;
    
    if (!aimButton_RageBot->state) {
        return;
    }
    
    C_BasePlayer* LocalPlayer = (C_BasePlayer*)pEntList->GetClientEntity(pEngine->GetLocalPlayer());
    if (!LocalPlayer || !LocalPlayer->IsValidLivePlayer()) {
        Reset();
        
        return;
    }

    C_BaseCombatWeapon* currentWeapon = (C_BaseCombatWeapon*)pEntList->GetClientEntityFromHandle(LocalPlayer->GetActiveWeapon());
    if (!currentWeapon || !currentWeapon->CanShoot()) {
        Reset();
        
        return;
    }

    int currentWeaponID = currentWeapon->EntityId();
    if (
        CWeaponManager::isC4(currentWeaponID) ||
        CWeaponManager::isKnife(currentWeaponID) ||
        CWeaponManager::isGrenade(currentWeaponID)
    ) {
        Reset();
        
        return;
    }

    ButtonCode_t aimKey = (ButtonCode_t)MOUSE_LEFT;
    ButtonCode_t volverKey = MOUSE_RIGHT;
    if (aimKey != ButtonCode_t::KEY_FIRST && aimKey != ButtonCode_t::MOUSE_LEFT) {
        volverKey = aimKey;
    } else {
        aimKey = ButtonCode_t::MOUSE_LEFT;
    }

    bool AutoShoot = CheatSettings::Rage::auto_shoot || (
        (pInputSystem->IsButtonDown(aimKey) && currentWeaponID != en_item_definition_index::weapon_revolver) ||
        (pInputSystem->IsButtonDown(volverKey) && currentWeaponID == en_item_definition_index::weapon_revolver)
    );
    if (!AutoShoot) {
        Reset();

        return;
    }

    C_BasePlayer* TargetEntity = nullptr;
    if (!m_lockedEntity) {
        TargetEntity = FindTarget(LocalPlayer);
        m_lockedEntity = TargetEntity;
    } else {
        TargetEntity = m_lockedEntity;
    }

    if (!TargetEntity || !TargetEntity->IsAlive() || TargetEntity->IsDormant()) {
        Reset();
        
        return;
    }

    CEnginePrediction::Instance()->Start(pCmd);
    
    AimTarget(LocalPlayer, TargetEntity);
    
    CEnginePrediction::Instance()->End();
}

void CRageBot::AimTarget(C_BasePlayer* LocalPlayer, C_BasePlayer* TargetEntity) {
    C_BaseCombatWeapon* currentWeapon = (C_BaseCombatWeapon*)pEntList->GetClientEntityFromHandle(LocalPlayer->GetActiveWeapon());
    if (!currentWeapon) {
        return;
    }
    QAngle oldAngle;
    pEngine->GetViewAngles(oldAngle);

    QAngle AimAngles;

    Hitboxes aimBone = HITBOX_NONE;
    if (m_lockedBone == Hitboxes::HITBOX_NONE) {
        if (CheatSettings::Rage::hit_scan) {
            aimBone = (Hitboxes)MakeHitscan(TargetEntity);
        } else {
            aimBone = (Hitboxes)CheatSettings::Rage::fixed_bone;
        }
        m_lockedBone = aimBone;
    } else {
        aimBone = m_lockedBone;
    }

    if (aimBone == Hitboxes::HITBOX_NONE) {
        return;
    }
    
    Vector vMin, vMax;
    Vector EnemyPos = Math::ExtrapolateTick(GetHitboxPosition2(TargetEntity, (int)aimBone, vMin, vMax), TargetEntity->GetVelocity());
    //Vector EnemyPos = TargetEntity->GetPredictedPosition((int)aimBone, vMin, vMax);
    if (!IsVisible(LocalPlayer, TargetEntity, aimBone, 360.0f, false)) {
        if (CheatSettings::Rage::auto_wall) {
            if (AutoWalls->GetDamage(LocalPlayer, TargetEntity, aimBone) < CheatSettings::Rage::autowall_min_damage) {
                return;
            }
        } else {
            return;
        }
    }

    Vector dir = LocalPlayer->GetEyePosition() - EnemyPos;
    Math::VectorNormalize(dir);
    Math::VectorAngles(dir, AimAngles);

    QAngle AimPunch = LocalPlayer->GetAimPunchAngle();
    if (AimPunch.x != 0.0f && AimPunch.y != 0.0f) {
        AimAngles -= AimPunch * 2.0f;
    }

    Math::NormalizeAngles(AimAngles);
    Math::ClampAngle(AimAngles);

    if (AimAngles == oldAngle || !AimAngles.IsValid()) {
        return;
    }

    Glob::AimbotIsAiming = true;
    if (CheatSettings::Rage::silent_aim) {
        userCommand->viewangles = AimAngles;
    } else {
        pEngine->SetViewAngles(AimAngles);
    }
    
    if (!DelayedShot(LocalPlayer, TargetEntity, currentWeapon, AimAngles)) {
        if (
            CheatSettings::Rage::auto_scope &&
            CWeaponManager::IsScopeWeapon(currentWeapon->EntityId()) &&
            GetCSWpnData(currentWeapon)->m_iZoomLevels > 0 &&
            LocalPlayer->IsScoped()
        ) {
            userCommand->buttons |= IN_ATTACK2;
        }
        
        return;
    }
    
    if (
        CheatSettings::Rage::auto_scope &&
        CWeaponManager::IsScopeWeapon(currentWeapon->EntityId()) &&
        GetCSWpnData(currentWeapon)->m_iZoomLevels > 0 &&
        !LocalPlayer->IsScoped()
    ) {
        userCommand->buttons |= IN_ATTACK2;
    } else {
        AutoStop(LocalPlayer);
        
        if (CheatSettings::Rage::autocrouch) {
            userCommand->buttons |= IN_DUCK;
        }
        
        AutoCockRevolver(userCommand);
    }
}

void CRageBot::AutoCrouch(C_BasePlayer* LocalPlayer) {
    if (!CheatSettings::Rage::autocrouch) {
        return;
    }

    if (!HasTarget()) {
        return;
    }

    if (!LocalPlayer || !LocalPlayer->IsValidLivePlayer()) {
        return;
    }

    userCommand->buttons |= IN_DUCK;
}

void CRageBot::AutoStop(C_BasePlayer* LocalPlayer) {
    if (!CheatSettings::Rage::auto_stop) {
        return;
    }

    if (!HasTarget()) {
        return;
    }

    if (!LocalPlayer || !LocalPlayer->IsValidLivePlayer()) {
        return;
    }

    userCommand->forwardmove = 0.0f;
    userCommand->sidemove = 0.0f;
}

bool CRageBot::CanHitEntity(C_BasePlayer* LocalPlayer, C_BasePlayer* TargetEntity, Vector EndPosition) {
    if (!IsVisible(LocalPlayer, TargetEntity, EndPosition, 360.0f, false)) {
        if (AutoWalls->GetDamage(LocalPlayer, EndPosition) >= CheatSettings::Rage::autowall_min_damage) {
            return true;
        }
    } else {
        trace_t tr;
        Ray_t ray;
        ray.Init(LocalPlayer->GetEyePosition(), EndPosition);
        
        pEngineTrace->ClipRayToEntity(ray, MASK_SHOT, TargetEntity, &tr);
        
        if (tr.startsolid || tr.allsolid) {
            return false;
        }
        
        C_BasePlayer* entity = (C_BasePlayer*)tr.m_pEnt;
        if (!entity) {
            return false;
        }
        
        if (entity->GetTeam() == LocalPlayer->GetTeam()) {
            return false;
        }
        
        if (entity->IsImmune()) {
            return false;
        }
        
        if (entity != TargetEntity) {
            return false;
        }
        
        return true;
    }

    return false;
}

bool CRageBot::HitChance(QAngle AimAngle, C_BasePlayer* LocalPlayer, C_BasePlayer* TargetPlayer, C_BaseCombatWeapon* activeWeapon) {
    static int MaxTraces = CheatSettings::Rage::hitchance_shots;
    int TotalHits = 0;
    
    int NeededHits = MaxTraces * (CheatSettings::Rage::hitchance_percent / 100.f);
    
    Vector eyes = LocalPlayer->GetEyePosition();
    float flRange = GetCSWpnData(activeWeapon)->m_flRange;
    
    for (int i = 0; i < MaxTraces; i++) {
        activeWeapon->GetAccuracyPenalty();
        
        float fRand1 = RandomFloat(0.0f, M_PI * 2.0f);
        float fRandPi1 = RandomFloat(0.0f, activeWeapon->GetSpread());
        float fRand2 = RandomFloat(0.0f, M_PI * 2.0f);
        float fRandPi2 = RandomFloat(0.0f, activeWeapon->GetInaccuracy());
        
        QAngle vSpread = QAngle(
            (cos(fRand1) * fRandPi1) + (cos(fRand2) * fRandPi2),
            (sin(fRand1) * fRandPi1) + (sin(fRand2) * fRandPi2),
            0.0f
        );
        
        QAngle qViewAngles = AimAngle + vSpread;
        
        Vector forward;
        Math::AngleVectors(qViewAngles, forward);
        Vector vEnd = eyes + (forward * flRange);
        
        if (!CanHitEntity(LocalPlayer, TargetPlayer, vEnd)) {
            continue;
        }
        
        TotalHits++;
        if (TotalHits >= NeededHits) {
            return true;
        }
        
        if ((MaxTraces - i + TotalHits) < NeededHits) {
            return false;
        }
    }
    
    return false;
}

bool CRageBot::DelayedShot(C_BasePlayer* LocalPlayer, C_BasePlayer* TargetEntity, C_BaseCombatWeapon* weapon, QAngle AimAngles) {
    if (!LocalPlayer || !LocalPlayer->IsValidLivePlayer()) {
        return false;
    }
    
    if (!TargetEntity || !TargetEntity->IsValidLivePlayer() || TargetEntity->IsImmune()) {
        return false;
    }
    
    if (!weapon) {
        return false;
    }
    
    bool HitChanceHit = !CheatSettings::Rage::hitchance || HitChance(AimAngles, LocalPlayer, TargetEntity, weapon);
    
    bool AutoShoot = (HitChanceHit && (
        (userCommand->buttons & IN_ATTACK) ||
        ((userCommand->buttons & IN_ATTACK2) && weapon->EntityId() == en_item_definition_index::weapon_revolver)
    )) || (CheatSettings::Rage::auto_shoot && HitChanceHit);

    bool InValidWeapon = weapon->GetAmmo() == 0 || !CWeaponManager::IsValidWeapon(weapon->EntityId());
    bool NoDelayedWeapon = !CWeaponManager::IsDelayedWeapon(weapon->EntityId());
    if (InValidWeapon || NoDelayedWeapon) {
        if (!InValidWeapon && NoDelayedWeapon && AutoShoot) {
            if (weapon->EntityId() == en_item_definition_index::weapon_revolver) {
                userCommand->buttons |= IN_ATTACK2;
            } else {
                userCommand->buttons |= IN_ATTACK;
            }
        }
        
        if (!InValidWeapon) {
            return true;
        }
        
        return false;
    }
    
    bool WasPrimaryAttacking = (bool)(userCommand->buttons & IN_ATTACK);
    bool WasSecondaryAttacking = (bool)(userCommand->buttons & IN_ATTACK2);
    
    userCommand->buttons &= ~IN_ATTACK;
    userCommand->buttons &= ~IN_ATTACK2;
    
    if (CanHitTarget(TargetEntity, weapon, userCommand)) {
        if (AutoShoot) {
            if (weapon->EntityId() == en_item_definition_index::weapon_revolver) {
                if (WasSecondaryAttacking) {
                    userCommand->buttons |= IN_ATTACK2;
                }
            } else {
                if (WasPrimaryAttacking) {
                    userCommand->buttons |= IN_ATTACK;
                }
            }
            
            return true;
        }
    }
    
    return false;
}

C_BasePlayer* CRageBot::FindTarget(C_BasePlayer* LocalPlayer) {
    C_BasePlayer* m_bestEnt = nullptr;
    C_BaseCombatWeapon* currentWeapon = (C_BaseCombatWeapon*)pEntList->GetClientEntityFromHandle(LocalPlayer->GetActiveWeapon());
    if (!currentWeapon) {
        return nullptr;
    }
    
    float m_bestFov = CheatSettings::Aim::fov;
    if (CheatSettings::Rage::fov_multiplier > 1.0f) {
        m_bestFov *= CheatSettings::Rage::fov_multiplier;
    }
    
    TeamID localTeam = (TeamID)LocalPlayer->GetTeam();
    
    for (int index = 1; index < pEngine->GetMaxClients(); index++) {
        C_BasePlayer* possibleTargetEntity = (C_BasePlayer*)pEntList->GetClientEntity(index);
        if (
            !possibleTargetEntity ||
            !possibleTargetEntity->IsAlive() ||
            possibleTargetEntity->IsDormant() ||
            possibleTargetEntity->IsImmune() ||
            possibleTargetEntity == LocalPlayer
        ) {
            continue;
        }
        
        //Features::CResolver::StoreVars(possibleTargetEntity);
        
        if (localTeam == (TeamID)possibleTargetEntity->GetTeam()) {
            continue;
        }
        
        QAngle viewAngle;
        pEngine->GetViewAngles(viewAngle);
        
        Hitboxes aimBone = Hitboxes::HITBOX_NONE;
        if (m_lockedBone == Hitboxes::HITBOX_NONE) {
            if (CheatSettings::Rage::hit_scan) {
                aimBone = (Hitboxes)MakeHitscan(possibleTargetEntity);
            } else {
                aimBone = (Hitboxes)CheatSettings::Rage::fixed_bone;
            }
            m_lockedBone = aimBone;
        } else {
            aimBone = (Hitboxes)m_lockedBone;
        }
        
        if (aimBone == Hitboxes::HITBOX_NONE) {
            continue;
        }
        
        if (!IsVisible(LocalPlayer, possibleTargetEntity, aimBone, 360.0f, false)) {
            if (CheatSettings::Rage::auto_wall) {
                if (AutoWalls->GetDamage(LocalPlayer, possibleTargetEntity, aimBone) < CheatSettings::Rage::autowall_min_damage) {
                    continue;
                }
            } else {
                continue;
            }
        }
        
        QAngle AimPunch = LocalPlayer->GetAimPunchAngle();
        if (AimPunch.x != 0.0f && AimPunch.y != 0.0f) {
            viewAngle -= AimPunch * 2.0f;
        }
        
        Vector vMin, vMax;
        float fov = Math::GetFov(
            viewAngle,
            LocalPlayer->GetEyePosition(),
            Math::ExtrapolateTick(GetHitboxPosition2(possibleTargetEntity, (int)aimBone, vMin, vMax), possibleTargetEntity->GetVelocity())
        );
        
        if (fov < m_bestFov) {
            m_bestFov = fov;
            m_bestEnt = possibleTargetEntity;
        }
    }
    
    return m_bestEnt;
}

//std::shared_ptr<Features::CRageBot> RageBot = std::make_unique<Features::CRageBot>();

