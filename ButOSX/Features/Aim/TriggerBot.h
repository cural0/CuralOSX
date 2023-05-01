/******************************************************/
/**                                                  **/
/**      Features/TriggerBot.h                       **/
/**      |_ @package GO:SX Legit                     **/
/**      |_ @created 2018-01-10                      **/
/**      |_ @author André "vAx" Kalisch              **/
/**                                                  **/
/**      Copyright © 2017 @author                    **/
/**      All rights reserved.                        **/
/**                                                  **/
/******************************************************/

#ifndef Features_TriggerBot_h
#define Features_TriggerBot_h

#include "ValveSDK.h"
#include "cmath"
#include "Utils.h"
#include "MyAimBot.hpp"
#include    <chrono>
#include    <thread>
#include "CheatSettings.h"
#include "AutoWalls.h"
//#include "SDK/Utils.h"
//#include "SDK/CCSPlayer.h"
//#include "Engine/Weapons/manager.h"



namespace Aim {
    namespace CTriggerBot {
        extern void CreateMove(CUserCmd* pCmd);
        extern void Reset();
//        long triggerTime = 0;
//        C_BasePlayer* triggerTarget = nullptr;
//        QAngle lastRCSPunch = {0.0000f, 0.0000f, 0.0000f};
    }

}


/*
namespace Features {
    class CTriggerBot {
    public:
     extern void TriggerBot(CUserCmd* pCmd);
    protected:
        void Reset();
        long triggerTime = 0;
        C_BasePlayer* triggerTarget = nullptr;
    };
}

//extern std::shared_ptr<Features::CTriggerBot> TriggerBot;
*/
#endif /** !Features_TriggerBot_h */
