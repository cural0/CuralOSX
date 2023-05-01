/******************************************************/
/**                                                  **/
/**      Features/Radar.h                            **/
/**      |_ @package GO:SX Legit                     **/
/**      |_ @created 2018-01-19                      **/
/**      |_ @author André "vAx" Kalisch              **/
/**                                                  **/
/**      Copyright © 2017 @author                    **/
/**      All rights reserved.                        **/
/**                                                  **/
/******************************************************/

#ifndef Features_Radar_h
#define Features_Radar_h

#include "ValveSDK.h"
#include "CheatSettings.h"
#include "Utils.h"
#include "manager.h"
//#include "SDK/CCSPlayer.h"
//#include "Engine/Drawing/manager.h"



class CRadar {
    public:
        CRadar();
        void PaintTraverse();
        void DrawGuiRadar();
        void DrawRadarPlayer(C_BasePlayer* entity, QAngle ViewAngle, bool HasBomb);
        void DrawRadarBombsites(QAngle ViewAngle);
        void InGameRadar();
        Vector2D WorldToRadar(const Vector location, const Vector origin, const QAngle angles, int width, float scale = 16.f);
    protected:
        bool bRadarHide;
        int radar_x;
        int radar_y;
        C_BasePlayer* LocalPlayer = nullptr;
    };

extern std::shared_ptr<CRadar> Radar;

#endif /** !Features_Radar_h */
