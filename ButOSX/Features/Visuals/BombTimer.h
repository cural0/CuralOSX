/******************************************************/
/**                                                  **/
/**      Features/BombTimer.h                        **/
/**      |_ @package GO:SX Legit                     **/
/**      |_ @created 2018-01-22                      **/
/**      |_ @author André "vAx" Kalisch              **/
/**                                                  **/
/**      Copyright © 2017 @author                    **/
/**      All rights reserved.                        **/
/**                                                  **/
/******************************************************/

#ifndef Features_BombTimer_h
#define Features_BombTimer_h

//#include "Engine/Drawing/manager.h"
#include "ValveSDK.h"
#include "CheatSettings.h"
#include "Utils.h"
#include "Visuals.hpp"



enum BombDisplayType {
    DISPLAY_NONE = 0,
    DISPLAY_BOTH = 1,
    DISPLAY_ONSCREEN = 2,
    DISPLAY_INWORLD = 3
};


class CBombTimer {
    public:
        CBombTimer();
        void PaintTraverse();
        void DrawOnScreenTimer(C_BasePlantedC4* bomb);
        void DrawWorldModelTimer(C_BasePlantedC4* bomb);
        Color GetBombColor(C_BasePlantedC4* bomb);
    protected:
        C_BasePlayer* LocalPlayer = nullptr;
        float BombTimerTime = 0.0f;
    };


//extern std::shared_ptr<Features::CBombTimer> BombTimer;

#endif /** !Features_BombTimer_h */
