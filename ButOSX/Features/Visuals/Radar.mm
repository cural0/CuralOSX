/******************************************************/
/**                                                  **/
/**      Features/Radar.cpp                          **/
/**      |_ @package GO:SX Legit                     **/
/**      |_ @created 2018-01-19                      **/
/**      |_ @author André "vAx" Kalisch              **/
/**                                                  **/
/**      Copyright © 2017 @author                    **/
/**      All rights reserved.                        **/
/**                                                  **/
/******************************************************/

#include "Radar.h"

ImFont* g_CSGOICONS;
ImFont* g_Font2;

int     CheatSettings::Radar::size = 180;
int     CheatSettings::Radar::pos_x = 10;
int     CheatSettings::Radar::pos_y = 50;
int     CheatSettings::Radar::zoom = 1;
int     CheatSettings::Radar::style = 1;

CRadar::CRadar() {}

void CRadar::PaintTraverse() {
    if (!visButton_Radar->state || !pEngine->IsInGame()) {
        return;
    }

    C_BasePlayer* LocalPlayer = (C_BasePlayer*)pEntList->GetClientEntity(pEngine->GetLocalPlayer());
    if (!LocalPlayer || LocalPlayer->IsDormant() || !LocalPlayer->IsAlive()) {
        return;
    }

#ifdef GOSX_STREAM_PROOF
    if (Options::Radar::style == 0 && !StreamProof->Active()) {
        InGameRadar();
        
        return;
    }
#else
    if (CheatSettings::Radar::style == 0) {
        InGameRadar();
        
        return;
    }
#endif
    
    QAngle Angle;
    pEngine->GetViewAngles(Angle);

    DrawGuiRadar();
    for (int i = 1; i <= pEngine->GetMaxClients(); i++) {
        C_BasePlayer* PlayerEntity = (C_BasePlayer*)pEntList->GetClientEntity(i);
        if (!PlayerEntity || PlayerEntity->IsDormant() || !PlayerEntity->IsAlive()) {
            continue;
        }

        if (PlayerEntity == LocalPlayer) {
            continue;
        }
        CSPlayerResource* PlayerResource = (CSPlayerResource*)pEntList->GetClientEntity(i);
        DrawRadarPlayer(PlayerEntity, Angle, PlayerResource->HasC4(i));
    }
    DrawRadarBombsites(Angle);

    C_BaseEntity* c4 = GetBomb();
//if (c4 && (*GameRules)->IsBombDropped()) {
        Vector2D BombPosition = WorldToRadar(c4->GetVecOrigin(), LocalPlayer->GetVecOrigin(), Angle, CheatSettings::Radar::size, CheatSettings::Radar::zoom);
    DrawManager->DrawRect((int)((BombPosition.x) - 3), (int)((BombPosition.y) - 3), 6, 6, (Color(253, 160, 21, 255)));
   // }
}

void CRadar::DrawGuiRadar() {
    if (!visButton_Radar->state || !pEngine->IsInGame()) {
        return;
    }

    radar_x = CheatSettings::Radar::pos_x;
    radar_y = CheatSettings::Radar::pos_y;

    if (visButton_Radar->state && bRadarHide) {
        bRadarHide = false;
    } else if (!visButton_Radar->state && !bRadarHide) {
        bRadarHide = true;
    }

    DrawManager->DrawRect(radar_x, radar_y, CheatSettings::Radar::size, CheatSettings::Radar::size, Color(0, 0, 0, 100));
    DrawManager->DrawRect(radar_x, CheatSettings::Radar::size / 2 + radar_y, CheatSettings::Radar::size, 1 , Color(255, 255, 255, 45));
    DrawManager->DrawRect(CheatSettings::Radar::size / 2 + radar_x, radar_y, 1, CheatSettings::Radar::size, Color(255, 255, 255, 45));
    
    
    CSPlayerResource* PlayerResource = (CSPlayerResource*)pEntList->GetClientEntity(pEngine->GetLocalPlayer());
    
    if (PlayerResource->HasC4(pEngine->GetLocalPlayer())) {
        DrawManager->DrawString(g_CSGOICONS, radar_x + 10, radar_y + CheatSettings::Radar::size + 10, FONTFLAG_NONE, Color(255, 255, 255), ICON_CSGO_C4, false);
    }
}

void CRadar::DrawRadarPlayer(C_BasePlayer* PlayerEntity, QAngle ViewAngle, bool HasBomb) {
    if (!visButton_Radar->state || !pEngine->IsInGame()) {
        return;
    }

    if (!LocalPlayer || !LocalPlayer->IsValidLivePlayer()) {
        return;
    }

    Color RadarPlayerColor = Color(0, 0, 0);

    int IndicatorSizeW = 6, IndicatorSizeH = 4;
    TriangleDirections dir = TriangleDirections::TRI_DOWN;
    if (PlayerEntity->GetTeam() == TEAM_TERRORIST) {
        RadarPlayerColor = Color(218, 1, 118, 255);
        dir = TriangleDirections::TRI_UP;
        if (HasBomb) {
            RadarPlayerColor = Color(210, 0, 0);
            IndicatorSizeW = 8;
            IndicatorSizeH = 6;
        }
    } else if (PlayerEntity->GetTeam() == TEAM_COUNTER_TERRORIST) {
        RadarPlayerColor = Color(26, 144, 253, 255);
    } else {
        return;
    }

    if (PlayerEntity->IsImmune()) {
        RadarPlayerColor.SetA(100);
    }

    Vector2D RadarPoint = WorldToRadar(PlayerEntity->GetVecOrigin(), LocalPlayer->GetVecOrigin(), ViewAngle, CheatSettings::Radar::size, CheatSettings::Radar::zoom);
    if (RadarPoint.x < 1 || RadarPoint.y < 1) {
        return;
    }

    DrawManager->DrawTriangle(IndicatorSizeW, IndicatorSizeH, RadarPlayerColor, (int)RadarPoint.x, (int)RadarPoint.y, dir, true);
}

void CRadar::DrawRadarBombsites(QAngle ViewAngle) {
    if (!visButton_Radar->state || !pEngine->IsInGame()) {
        return;
    }

    if (!LocalPlayer || !LocalPlayer->IsValidLivePlayer()) {
        return;
    }

    //if (!(*GameRules)->IsBombDefuseMap()) {
    //    return;
    //}
    CSPlayerResource* PlayerResource = (CSPlayerResource*)pEntList->GetClientEntity(pEngine->GetLocalPlayer());

    Vector BombsiteA = PlayerResource->GetBombsiteCenterA();
    Vector BombsiteB = PlayerResource->GetBombsiteCenterB();

    Vector2D RadarPointA = WorldToRadar(BombsiteA, LocalPlayer->GetVecOrigin(), ViewAngle, CheatSettings::Radar::size, CheatSettings::Radar::zoom);
    if (RadarPointA.x > 0 && RadarPointA.y > 0) {
        DrawManager->DrawString(g_Font2, (int)RadarPointA.x, (int)RadarPointA.y, FONTFLAG_NONE, Color(253, 160, 21, 255), true, "A");
    }

    Vector2D RadarPointB = WorldToRadar(BombsiteB, LocalPlayer->GetVecOrigin(), ViewAngle, CheatSettings::Radar::size, CheatSettings::Radar::zoom);
    if (RadarPointB.x > 0 && RadarPointB.y > 0) {
        DrawManager->DrawString(g_Font2, (int)RadarPointB.x, (int)RadarPointB.y, FONTFLAG_NONE, Color(253, 160, 21, 255), true, "B");
    }
}

Vector2D CRadar::WorldToRadar(const Vector location, const Vector origin, const QAngle angles, int width, float scale) {
    float x_diff = location.x - origin.x;
    float y_diff = location.y - origin.y;

    int iRadarRadius = width;

    float flOffset = atanf(y_diff / x_diff);
    flOffset *= 180;
    flOffset /= M_PI;

    if ((x_diff < 0) && (y_diff >= 0)) {
        flOffset = 180 + flOffset;
    } else if ((x_diff < 0) && (y_diff < 0)) {
        flOffset = 180 + flOffset;
    } else if ((x_diff >= 0) && (y_diff < 0)) {
        flOffset = 360 + flOffset;
    }

    y_diff = -1 * (sqrtf((x_diff * x_diff) + (y_diff * y_diff)));
    x_diff = 0;

    flOffset = angles.y - flOffset;

    flOffset *= M_PI;
    flOffset /= 180;

    float xnew_diff = x_diff * cosf(flOffset) - y_diff * sinf(flOffset);
    float ynew_diff = x_diff * sinf(flOffset) + y_diff * cosf(flOffset);

    xnew_diff /= scale;
    ynew_diff /= scale;

    xnew_diff = (iRadarRadius / 2) + (int)xnew_diff;
    ynew_diff = (iRadarRadius / 2) + (int)ynew_diff;

    if (xnew_diff > iRadarRadius) {
        xnew_diff = iRadarRadius - 4;
    } else if (xnew_diff < 4) {
        xnew_diff = 4;
    }

    if (ynew_diff> iRadarRadius) {
        ynew_diff = iRadarRadius;
    } else if (ynew_diff < 4) {
        ynew_diff = 0;
    }

    return Vector2D(CheatSettings::Radar::pos_x + xnew_diff, CheatSettings::Radar::pos_y + ynew_diff);
}

void CRadar::InGameRadar() {
    for (int i = 1; i <= pEngine->GetMaxClients(); i++) {
        C_BasePlayer* PlayerEntity = (C_BasePlayer*)pEntList->GetClientEntity(i);
        if (!PlayerEntity || PlayerEntity->IsDormant() || !PlayerEntity->IsAlive()) {
            continue;
        }
        
        if (PlayerEntity == LocalPlayer) {
            continue;
        }
        
        if (!(*PlayerEntity->GetSpotted())) {
            *PlayerEntity->GetSpotted() = true;
        }
    }
}

std::shared_ptr<CRadar> Radar = std::make_unique<CRadar>();
