/******************************************************/
/**                                                  **/
/**      Rage/RageBot.h                              **/
/**      |_ @package GO:SX Legit                     **/
/**      |_ @created 2018-08-13                      **/
/**      |_ @author André "vAx" Kalisch              **/
/**                                                  **/
/**      Copyright © 2017 @author                    **/
/**      All rights reserved.                        **/
/**                                                  **/
/******************************************************/

#define Rage_RageBot_h

#include "AimHelper.h"


//#include "Resolver.h"
#include "AutoWalls.h"
#include "CheatSettings.h"
#include "Utils.h"
#include "EnginePrediction.h"

#define XM_2PI 6.283185307f


    class CRageBot : public CAimHelper {
    public:
        CRageBot();
        void CreateMove(CUserCmd* pCmd);
        void AimTarget(C_BasePlayer* LocalPlayer, C_BasePlayer* TargetEntity);
        C_BasePlayer* FindTarget(C_BasePlayer* LocalPlayer);
        void AutoCrouch(C_BasePlayer* LocalPlayer);
        void AutoStop(C_BasePlayer* LocalPlayer);
        void AutoShoot(C_BasePlayer* LocalPlayer, C_BasePlayer* TargetPlayer, C_BaseCombatWeapon* activeWeapon);
        bool CanHitEntity(C_BasePlayer* LocalPlayer, C_BasePlayer* TargetEntity, Vector EndPosition);
        bool HitChance(QAngle AimAngle, C_BasePlayer* LocalPlayer, C_BasePlayer* TargetPlayer, C_BaseCombatWeapon* activeWeapon);
        bool DelayedShot(C_BasePlayer* LocalPlayer, C_BasePlayer* TargetEntity, C_BaseCombatWeapon* weapon, QAngle AimAngles);
    private:
        CUserCmd* userCommand = nullptr;
    };


//extern std::shared_ptr<Features::CRageBot> RageBot;



  /** !Rage_RageBot_h */
