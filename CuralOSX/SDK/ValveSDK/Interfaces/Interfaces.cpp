//
//  Interfaces.cpp
//  ButOSX
//
//  Created by Can on 23.01.2021.
//  Copyright © 2021 Lyceion. All rights reserved.
//

#include "ValveSDK.h"
#include "VMT.h"

IPanel* pPanel = nullptr;
ICvar* pCvar = nullptr;
IVModelRender* pModelRender = nullptr;
IBaseClientDLL* pClient = nullptr;
IEngineClient* pEngine = nullptr;
IVDebugOverlay* pOverlay = nullptr;
IVModelInfo* pModelInfo = nullptr;
ISurface* pSurface = nullptr;
IEngineTrace* pEngineTrace = nullptr;
IInputSystem* pInputSystem = nullptr;
IClientEntityList* pEntList = nullptr;
IClientMode* pClientMode = nullptr;
IVMaterialSystem* pMaterialSystem = nullptr;
IAppSystem* pAppSystem = nullptr;
IPhysicsSurfaceProps* Physics = nullptr;
CGlobalVarsBase* pGlobalVars = nullptr;
IMoveHelper* pMoveHelper = nullptr;
IPrediction* pPrediction = nullptr;
IGameMovement* pGameMovement = nullptr;


VMT* paintVMT = nullptr;
VMT* dmeVMT = nullptr;
VMT* fsnVMT = nullptr;
VMT* crtmVMT = nullptr;
VMT* lkcVMT = nullptr;
