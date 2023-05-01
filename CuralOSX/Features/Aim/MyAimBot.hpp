//
//  MyAimBot.hpp
//  ButOSX
//
//  Created by Michał Kural on 25/03/2023.
//  Copyright © 2023 Lyceion. All rights reserved.
//

#ifndef MyAimBot_hpp
#define MyAimBot_hpp

#include <stdio.h>
#include "ValveSDK.h"
#include "TouchBar.h"
#include "CMath.h"
#include "Utils.h"
#include "weapons.h"
#include "EnginePrediction.h"




class CAim {
public:
    CAim();
    
    
    C_BaseEntity* FindTarget(C_BasePlayer* LocalPlayer);
    bool IsVisible(C_BasePlayer* localPlayer, Vector position, bool exact = false);
    void StartAim(C_BasePlayer* LocalPlayer, C_BaseEntity* AimTarget, CUserCmd* pCmd);
    void CreateMove(CUserCmd* pCmd);
    static std::map<int, const char*> GetBoneList();
    static const char* GetBoneValues[];
    bool HasTarget();
protected:
    void AutoStop(C_BasePlayer* LocalPlayer, C_BaseEntity* EntityPlayer, CUserCmd* pCmd);
    void AutoCrouch(C_BasePlayer* LocalPlayer, C_BaseEntity* EntityPlayer, CUserCmd* pCmd);
    void Smooth(QAngle& angle);
    static void Salt(float& smooth);
    void RecoilControlBot(QAngle& angle, C_BasePlayer* LocalPlayer, C_BaseEntity* TargetEntity, CUserCmd* pCmd);
    void Reset();
    void AutoShoot(C_BaseEntity* player, C_BaseCombatWeapon* activeWeapon, CUserCmd* pCmd);
private:
    C_BaseEntity* LockedTargetEntity = nullptr;
    int LockedEntityBone = -5;
    bool HasValidTargetAndIsAiming = false;
};

#endif /* MyAimBot_hpp */
