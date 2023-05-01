//
//  EnginePrediction.hpp
//  GOSX Pro
//
//  Created by Andre Kalisch on 18.04.17.
//  Copyright © 2017 Andre Kalisch. All rights reserved.
//

#ifndef EnginePrediction_h
#define EnginePrediction_h

#include "ValveSDK.h"
#include <map>
#include "Utils.h"


extern float m_flOldCurtime;
extern float m_flOldFrametime;

class CEnginePrediction {
public:
    CEnginePrediction();
    static std::shared_ptr<CEnginePrediction> Instance() {
        if (!instance) {
            instance = std::make_unique<CEnginePrediction>();
        }

        return instance;
    }
    static std::map<int, const char*> GetModeList();
    static std::map<int, const char*> GetModeValues();
    void Run(CUserCmd* cmd, C_BaseCombatWeapon* pWeapon, C_BasePlayer* pLocal);
    void Start(CUserCmd* cmd);
    void End();
private:
    static std::shared_ptr<CEnginePrediction> instance;
    float m_flOldCurtime = 0.f;
    float m_flOldFrametime = 0.f;
    int oldPFlags;
};

#endif /* EnginePrediction_h */
