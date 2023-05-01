//
//  Interfaces.hpp
//  ButOSX
//
//  Created by Can on 23.01.2021.
//  Copyright © 2021 Lyceion. All rights reserved.
//

#ifndef Interfaces_hpp
#define Interfaces_hpp

#include <stdio.h>
extern ISurface* pSurface;
extern IPanel* pPanel;
extern ICvar* pCvar;
extern IBaseClientDLL* pClient;
extern IClientEntityList* pEntList;
extern IVDebugOverlay* pOverlay;
extern IEngineClient* pEngine;
extern IVModelInfo* pModelInfo;
extern IEngineTrace* pEngineTrace;
extern IClientMode* pClientMode;
extern IVModelRender* pModelRender;
extern IVMaterialSystem* pMaterialSystem;
extern IAppSystem* pAppSystem;
extern IClientMode* pClientMod;
extern IInputSystem* pInputSystem;
extern IPhysicsSurfaceProps* Physics;
extern CGlobalVarsBase* pGlobalVars;
extern IMoveHelper* pMoveHelper;
extern IPrediction* pPrediction;
extern IGameMovement* pGameMovement;

extern VMT* paintVMT;
extern VMT* dmeVMT;
extern VMT* fsnVMT;
extern VMT* crtmVMT;
extern VMT* lkcVMT;
#endif /* Interfaces_hpp */
