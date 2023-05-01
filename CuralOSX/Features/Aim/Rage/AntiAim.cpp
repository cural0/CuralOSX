//
//  AntiAim.cpp
//  GOSX Pro
//
//  Created by Andre Kalisch on 19.04.17.
//  Copyright © 2017 Andre Kalisch. All rights reserved.
//

#include "AntiAim.h"

std::shared_ptr<CAntiAim> CAntiAim::instance = nullptr;

int CheatSettings::Aim::antiaim_pitch = 0;
int CheatSettings::Aim::antiaim_custom_pitch = 0.00000;
int CheatSettings::Aim::antiaim_yaw = 0;
int CheatSettings::Aim::antiaim_custom_yaw = 0.00000;

CAntiAim::CAntiAim() {}

std::map<int, const char*> CAntiAim::GetPitchList() {
    std::map<int, const char*> select;

    select[0] = "0"; // Emotion
    select[1] = "1"; // Up
    select[2] = "2"; // Down
    select[3] = "3"; // Custom

    return select;
}
std::map<int, const char*> CAntiAim::GetPitchValues() {
    std::map<int, const char*> values;

    values[0] = "Emotion";
    values[1] = "Up";
    values[2] = "Down";
    values[3] = "Custom";

    return values;
}
std::map<int, const char*> CAntiAim::GetYawList() {
    std::map<int, const char*> select;

    select[0] = "0"; // Backwards
    select[1] = "1"; // Left
    select[2] = "2"; // Right
    select[3] = "3"; // Custom

    return select;
}

std::map<int, const char*> CAntiAim::GetYawValues() {
    std::map<int, const char*> values;

    values[0] = "Backwards";
    values[1] = "Left";
    values[2] = "Right";
    values[3] = "Custom";

    return values;
}

void CAntiAim::AAPitch(QAngle& angle) {
    EAntiAimPitch AntiaimTypePitch = (EAntiAimPitch)CheatSettings::Aim::antiaim_pitch;

    switch (AntiaimTypePitch) {
        case EAntiAimPitch::Emotion:
            angle.x = 82.0f;
            break;
        case EAntiAimPitch::Down:
            angle.x = 89.0f;
            break;
        case EAntiAimPitch::Up:
            angle.x = -89.0f;
            break;
        case EAntiAimPitch::CustomPitch:
            angle.x = 89.f - CheatSettings::Aim::antiaim_custom_pitch;
            break;
        default:
            angle.x -= 0.0f;
            break;
    }
}

void CAntiAim::AAYaw(QAngle& angle) {
    EAntiAimYaw AntiaimTypeYaw = (EAntiAimYaw)CheatSettings::Aim::antiaim_yaw;

    switch (AntiaimTypeYaw) {
        case EAntiAimYaw::Backwards:
            angle.y -= 180.0f;
            break;
        case EAntiAimYaw::Left:
            angle.y += 90.0f;
            break;
        case EAntiAimYaw::Right:
            angle.y -= 90.0f;
            break;
        case EAntiAimYaw::CustomYaw:
            angle.y = 180.f - CheatSettings::Aim::antiaim_custom_yaw;
            break;
        default:
            angle.y -= 0.0f;
            break;
    }
}

void CAntiAim::CreateMove(CUserCmd* pCmd) {
    C_BasePlayer* pLocal((C_BasePlayer*)pEntList->GetClientEntity(pEngine->GetLocalPlayer()));
    if (!pLocal || !pLocal->IsAlive()) {
        return;
    }

    //C_BaseCombatWeapon* pWeapon = pLocal->GetActiveWeapon();
    //if (!pWeapon) {
    //    return;
    //}

    QAngle oldAngle = pCmd->viewangles;
    float oldForward = pCmd->forwardmove;
    float oldSideMove = pCmd->sidemove;

    QAngle angle = pCmd->viewangles;

    if (
        pCmd->buttons & IN_USE ||
        pCmd->buttons & IN_ATTACK
    ) {
        return;
    }

    if (pLocal->GetMoveType() == MOVETYPE_LADDER || pLocal->GetMoveType() == MOVETYPE_NOCLIP) {
        return;
    }

    //if (pLocal->IsAlive() && pWeapon->GetCSWpnData()->GetWeaponType() == CSWeaponType::WEAPONTYPE_KNIFE) {
    //    return;
    //}

   // if (pLocal->IsAlive() && pWeapon->GetCSWpnData()->GetWeaponType() == CSWeaponType::WEAPONTYPE_GRENADE) {
   //     return;
   // }

    if (
        miscButton_TriggerBot->state &&
        pInputSystem->IsButtonDown((ButtonCode_t)MOUSE_4)
        )
     {
        return;
    }

    AAYaw(angle);
    Math::NormalizeAngles(angle);

    AAPitch(angle);
    Math::NormalizeAngles(angle);
    
    Math::ClampAngle(angle);

    pCmd->viewangles = angle;

    Math::CorrectMovement(oldAngle, pCmd, oldForward, oldSideMove);
}
