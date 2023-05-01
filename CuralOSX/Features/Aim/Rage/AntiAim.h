//
//  AntiAim.hpp
//  GOSX Pro
//
//  Created by Andre Kalisch on 19.04.17.
//  Copyright © 2017 Andre Kalisch. All rights reserved.
//

#ifndef AntiAim_h
#define AntiAim_h

#include "ValveSDK.h"
#include "Utils.h"
#include <map>
#include "CheatSettings.h"

enum EAntiAimPitch {
    Emotion = 0,
    Up = 1,
    Down = 2,
    CustomPitch = 3
};

enum EAntiAimYaw {
    Backwards = 0,
    Left = 1,
    Right = 2,
    CustomYaw = 3
};

class CAntiAim {
public:
    CAntiAim();
    static std::shared_ptr<CAntiAim> Instance() {
        if (!instance) {
            instance = std::make_unique<CAntiAim>();
        }

        return instance;
    }
    static std::map<int, const char*> GetPitchList();
    static std::map<int, const char*> GetPitchValues();
    static std::map<int, const char*> GetYawList();
    static std::map<int, const char*> GetYawValues();
    void SetAtTargets(Vector targetAngle);
    void AAPitch(QAngle& angle);
    void AAYaw(QAngle& angle);
    void CreateMove(CUserCmd* pCmd);
private:
    static std::shared_ptr<CAntiAim> instance;
    Vector atTargets;
};

#endif /* AntiAim_h */
