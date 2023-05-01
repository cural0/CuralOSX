/******************************************************/
/**                                                  **/
/**      Features/AutoWalls.h                        **/
/**      |_ @package GO:SX Legit                     **/
/**      |_ @created 2018-05-16                      **/
/**      |_ @author André "vAx" Kalisch              **/
/**                                                  **/
/**      Copyright © 2017 @author                    **/
/**      All rights reserved.                        **/
/**                                                  **/
/******************************************************/

#ifndef Features_AutoWalls_h
#define Features_AutoWalls_h

#include "ValveSDK.h"
#include "Utils.h"
//#include "SDK/CCSPlayer.h"
//#include "SDK/Definitions.h"
//#include "Engine/Drawing/manager.h"
//#include "Engine/Weapons/manager.h"
#include "weapons.h"

struct FireBulletData {
    Vector          src;
    trace_t         enter_trace;
    Vector          direction;
    CTraceFilter    filter;
    float           trace_length;
    float           trace_length_remaining;
    float           current_damage;
    int             penetrate_count;
};

class CAutoWalls {
public:
    CAutoWalls();
    void TraceLine(Vector vecAbsStart, Vector vecAbsEnd, unsigned int mask, C_BasePlayer* ignore, trace_t* ptr);
    float GetHitgroupDamageMultiplier(int iHitGroup);
    bool TraceToExit(Vector &end, trace_t *enter_trace, Vector start, Vector dir, trace_t *exit_trace);
    void ScaleDamage(int hitgroup, C_BasePlayer* enemy, float weapon_armor_ratio, float & current_damage);
    bool HandleBulletPenetration(WeaponCSInfo_t* weaponInfo, FireBulletData &data);
    bool SimulateFireBullet(C_BasePlayer* pLocal, C_BaseCombatWeapon* pWeapon, FireBulletData &data);
    float GetDamage(C_BasePlayer* LocalPlayer, C_BasePlayer* pEntity, int HitboxIndex);
    float GetDamage(C_BasePlayer* LocalPlayer, Vector HitboxPosition);
    float GetDamageByAngle(C_BasePlayer* LocalPlayer, Vector AimAngles);
    bool CanWallbang(C_BasePlayer* LocalPlayer, float &dmg);
    void PaintTraverse();
};

extern std::shared_ptr<CAutoWalls> AutoWalls;

#endif /** !Features_AutoWalls_h */
