/******************************************************/
/**                                                  **/
/**      Features/AimHelper.h                        **/
/**      |_ @package GO:SX Legit                     **/
/**      |_ @created 2018-01-18                      **/
/**      |_ @author André "vAx" Kalisch              **/
/**                                                  **/
/**      Copyright © 2017 @author                    **/
/**      All rights reserved.                        **/
/**                                                  **/
/******************************************************/

#ifndef Features_AimHelper_h
#define Features_AimHelper_h

#include "ValveSDK.h"
#include "cmath"
#include    <chrono>
#include    <thread>
#include "AutoWalls.h"
#include "GlobalVars.h"



class CAimHelper {
public:
    
    bool NeedsReset();
    int MakeHitscan(C_BasePlayer* pEntity);
    void RecoilControl(QAngle& angle, C_BasePlayer* LocalPlayer = nullptr);
    bool CanHitTarget(C_BasePlayer* TargetEntity, C_BaseCombatWeapon* weapon, CUserCmd* pCmd);
    bool HitChance(C_BasePlayer* TargetEntity, Vector targetPosition, CUserCmd* pCmd);
    bool HasTarget();
    void Reset();
    void AutoCockRevolver(CUserCmd* pCmd);
    QAngle GetPunchAngle(C_BasePlayer* LocalPlayer = nullptr);
    bool CanAttack();
    QAngle GetAimAngles(C_BasePlayer* LocalPlayer = nullptr);
    
// Faceit / SMAC safe aim

    static bool GetMouseData(float &m_flSensitivity, float &m_flYaw, float &m_flPitch);
    static QAngle AnglesToPixels(QAngle viewAngle, QAngle aimAngle, float sensitivity, float mpitch, float myaw);
    static QAngle PixelsDeltaToAnglesDelta(QAngle PixelsDelta, float sensitivity, float mpitch, float myaw);

    C_BasePlayer* LocalPlayer = nullptr;
    C_BasePlayer* m_lockedEntity = nullptr;
    Hitboxes m_lockedBone = Hitboxes::HITBOX_NONE;
    
    QAngle OldPunch = QAngle(0.0f, 0.0f, 0.0f);
    QAngle CurrentAngle = QAngle(-1.0f, -1.0f, -1.0f);
    QAngle CurrentRawAngle = QAngle(-1.0f, -1.0f, -1.0f);
    
    ConVar* sensitivity = nullptr;
    ConVar* m_yaw = nullptr;
    ConVar* m_pitch = nullptr;

};

#endif /** !Features_AimHelper_h */
