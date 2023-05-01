/******************************************************/
/**                                                  **/
/**      Features/BackTracking.h                     **/
/**      |_ @package GO:SX Legit                     **/
/**      |_ @created 2018-03-29                      **/
/**      |_ @author André "vAx" Kalisch              **/
/**                                                  **/
/**      Copyright © 2017 @author                    **/
/**      All rights reserved.                        **/
/**                                                  **/
/******************************************************/

#ifndef Features_BackTracking_h
#define Features_BackTracking_h

#include <stdio.h>
#include "ValveSDK.h"
#include "TouchBar.h"
#include "CMath.h"
#include "Utils.h"
#include "weapons.h"
#include "CheatSettings.h"


struct BacktrackData_t {
    BacktrackData_t() {
        SimulationTime = -1;
        Hitbox = {0, 0, 0};
        Origin = {0, 0, 0};
    }
    
    BacktrackData_t(float simtime, Vector hitbox) {
        SimulationTime = simtime;
        Hitbox = hitbox;
        isValid = true;
    }
    
    bool isValid = false;
    float SimulationTime;
    Vector Hitbox;
    matrix3x4_t BoneMatrix[MAXSTUDIOBONES];
    Vector Origin;
};

typedef std::vector<std::vector<BacktrackData_t>> vecMulti;
typedef std::vector<BacktrackData_t> vecSingle;


class CBackTracking {
    public:
    CBackTracking();
    void CreateMove(CUserCmd* cmd);
    void PaintTraverse();
    void InvalidateData(int PlayerIndex = -1);
    BacktrackData_t GetTick(int PlayerIndex, int Tick);
    vecSingle GetPlayer(int PlayerIndex);
    protected:
    vecMulti BacktrackData = {};
        // std::map<int, std::map<int, BacktrackData_t>> BTData = {};
    };

//extern std::shared_ptr<Features::CBackTracking> Backtracking;


#endif /** !Features_BackTracking_h */
