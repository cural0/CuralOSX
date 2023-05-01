/******************************************************/
/**                                                  **/
/**      Features/AlwaysRCS.cpp                      **/
/**      |_ @package GO:SX Legit                     **/
/**      |_ @created 2018-01-18                      **/
/**      |_ @author André "vAx" Kalisch              **/
/**                                                  **/
/**      Copyright © 2017 @author                    **/
/**      All rights reserved.                        **/
/**                                                  **/
/******************************************************/

#include "AlwaysRCS.h"



float CheatSettings::Aim::rcs_level = 1.000f;

QAngle lastRCSPunch = {0.0000f, 0.0000f, 0.0000f};


RCS::RCS() {}


void SmoothAngle(QAngle& angle, QAngle CurrentAngle, CUserCmd* pCmd, bool forceConstantSpeed) {


    QAngle delta = angle - CurrentAngle;
    Math::NormalizeAngles(delta);

    float smooth = powf(0.62f, 0.4f);
    smooth = std::min(0.959999f, smooth);

//    if (Options::Aimbot::smooth_salting) {
//        CLegitBot::Salt(smooth);
//    }

    QAngle toChange = QAngle();

    if (forceConstantSpeed) {
        float coeff = (1.0f - smooth) / delta.Length() * 4.0f;
        coeff = std::min(1.0f, coeff);
        toChange = delta * coeff;
    } else {
        toChange = delta - delta * smooth;
    }

    angle = CurrentAngle + toChange;
}


void RCS::AlwaysRCS(CUserCmd *cmd) {
 //   if (!miscButton_AlwaysRCS->state || *Glob::AimbotIsAiming) {
  //      lastRCSPunch = {0.0000f, 0.0000f, 0.0000f};
  //      return;
 //   }
    if(!miscButton_AlwaysRCS->state)
        return;
    
    if(!pEngine->IsInGame())
        return;
    
    if (!(cmd->buttons & IN_ATTACK)) {
        lastRCSPunch = {0.0000f, 0.0000f, 0.0000f};
        return;
    }
    C_BasePlayer* pLocal = (C_BasePlayer*)pEntList->GetClientEntity(pEngine->GetLocalPlayer());

    if (!pLocal || !pLocal->IsAlive()) {
        lastRCSPunch = {0.0000f, 0.0000f, 0.0000f};
        return;
    }

    auto activeWeapon = pLocal->GetActiveWeapon();
    if (!activeWeapon) {
        lastRCSPunch = {0.0000f, 0.0000f, 0.0000f};
        return;
    }

    QAngle ViewAngle;
    pEngine->GetViewAngles(ViewAngle);

    QAngle CurrentAimPunch = pLocal->GetAimPunchAngle();
    if (pLocal->GetShotsFired() <= 0) {
        return;
    }

    QAngle NewPunch = {
        CurrentAimPunch.x - lastRCSPunch.x,
        CurrentAimPunch.y - lastRCSPunch.y,
        0
    };

//    if (NewPunch.Length() > 0.0f || NewPunch.Length() < 150.0f) {
    if (pLocal->GetShotsFired() > 1) {
        ViewAngle.x -= NewPunch.x * CheatSettings::Aim::rcs_level;
        ViewAngle.y -= NewPunch.y * CheatSettings::Aim::rcs_level;

        SmoothAngle(ViewAngle, cmd->viewangles, cmd, true);

        Math::NormalizeAngles(ViewAngle);
        Math::ClampAngle(ViewAngle);

 
        pEngine->SetViewAngles(ViewAngle);

    }
    
    lastRCSPunch = CurrentAimPunch;
}

//std::shared_ptr<Features::CAlwaysRCS> AlwaysRCS = std::make_unique<Features::CAlwaysRCS>();


