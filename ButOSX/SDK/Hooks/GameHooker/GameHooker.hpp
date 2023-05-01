//
//  GameHooker.hpp
//  ButOSX
//
//  Created by Can on 1.11.2020.
//  Copyright © 2020 Lyceion. All rights reserved.
//

#ifndef GameHooker_hpp
#define GameHooker_hpp

#include <stdio.h>
#include "PatternScanner.hpp"
#include "ValveSDK.h"
#include "OpenGLHooker.hpp"
#include "TouchBar.h"
#include "Memory.hpp"
//#include "DiscordRPC.hpp"

#include "Visuals.hpp"
#include "Miscs.hpp"
#include "Triggerbot.h"
#include "MyAimBot.hpp"
#include "BackTracking.h"
#include "AlwaysRCS.h"
#include "RageBot.h"
#include "AntiAim.h"
#include "EnginePrediction.h"
#include "Radar.h"

// FUNC INDEXES
#define DME_INDEX 21
#define CMV_INDEX 25 //24 on windows. NOTED!
#define FSN_INDEX 37
#define PTV_INDEX 42
#define LKC_INDEX 67

//DLL NAMES

typedef void(*tDrawModelExecute)(void* thisptr, void* context, void* state, ModelRenderInfo_t& model_info, matrix3x4_t* pCustomBoneToWorld);
extern void hkDrawModelExecute(void* thisptr, void* context, void* state, ModelRenderInfo_t& model_info, matrix3x4_t* pCustomBoneToWorld);

typedef bool(*tCreateMove)(void* thisptr, float inputSampleTime, CUserCmd* cmd);
extern bool hkCreateMove(void* thisptr, float inputSampleTime, CUserCmd* cmd);

typedef void(*tFrameStageNotify)(void* thisptr, FrameStage stage);
extern void hkFrameStageNotify(void* thisptr, FrameStage stage);

typedef void(*tPaintTraverse)(void* thisptr, VPANEL vguipanel, bool forceRepaint, bool allowForce);
extern void hkPaintTraverse(void* thisptr, VPANEL vguiPanel, bool forceRepaint, bool allowForce);

typedef void(*tLockCursor)(void* thisptr);
extern void hkLockCursor(void* thisptr);


namespace GameHooker{
void Init();
void Destroy();
void LoadModules();
void ScanSigs();
void HookVMTs();
void LoadInterfaces();
void InitGlobalVars();

extern C_PatternScanner* sigScanner;
extern uintptr_t clientModePointer;
}
#endif /* GameHooker_hpp */
