//
//  GameHooker.cpp
//  ButOSX
//
//  Created by Can on 1.11.2020.
//  Copyright © 2020 Lyceion. All rights reserved.
//

#include "GameHooker.hpp"

void hkDrawModelExecute(void* thisptr, void* context, void* state, ModelRenderInfo_t& model_info, matrix3x4_t* pCustomBoneToWorld) {
    //DME THINGS...
    dmeVMT->GetMethod<tDrawModelExecute>(DME_INDEX)(thisptr, context, state, model_info, pCustomBoneToWorld); //Get from my old source probably pasted.
    pModelRender->ForcedMaterialOverride(0);

}

bool hkCreateMove(void* thisptr, float inputSampleTime, CUserCmd* cmd)
{
    crtmVMT->GetMethod<tCreateMove>(CMV_INDEX)(thisptr, inputSampleTime, cmd);
    CAim* g_cAim = nullptr;
    CBackTracking* g_cBack = nullptr;
    RCS* g_rcs = nullptr;
    CRageBot* g_rage = nullptr;
    std::shared_ptr<CAntiAim> g_cAntiAim = nullptr;
    
    //AlwaysRCS
    if(true) {
         if(!g_rcs) {
             g_rcs = new RCS();
         }
        g_rcs->AlwaysRCS(cmd);
     }

    
    //Anti Aim
    
    if (CheatSettings::Aim::rage && aimButton_AntiAim->state) {
        if (!g_cAntiAim) {
            g_cAntiAim = CAntiAim::Instance();
        }

        g_cAntiAim->CreateMove(cmd);
    }

    Miscs::Movement::BunnyHop(cmd);
    Aim::CTriggerBot::CreateMove(cmd);

    //BackTracking
    if(true) {
        if(!g_cBack) {
            g_cBack = new CBackTracking();
        }
        g_cBack->CreateMove(cmd);
    }
    
    
    //Engine Prediction and aimbot
    if (aimButton_AimBot->state) {
        if (!g_cAim) {
            g_cAim = new CAim();
        }
        bool EnginePredictActive = CheatSettings::Aim::rage;

        if (EnginePredictActive) {
            CEnginePrediction::Instance()->Start(cmd);
        }

        g_cAim->CreateMove(cmd);

        if (EnginePredictActive) {
            CEnginePrediction::Instance()->End();
        }
    }
    
    
    //RageBot
    
    if (aimButton_RageBot->state) {
        if (!g_rage) {
            g_rage = new CRageBot();
        }
        bool EnginePredictActive = 1;

        if (EnginePredictActive) {
            CEnginePrediction::Instance()->Start(cmd);
        }

        g_rage->CreateMove(cmd);

        if (EnginePredictActive) {
            CEnginePrediction::Instance()->End();
        }
    }
    //Features::CBackTracking::CreatemMove(cmd);
//    Miscs::Movement::TriggerBot(cmd);


 
    return false;
}

void hkFrameStageNotify(void* thisptr, FrameStage stage) {
   
    
    if (stage == FrameStage::RENDER_START) {
        Visuals::Others::NightMode();
        Visuals::Others::NoVisRecoil();
        Visuals::Others::NoFlash();
        Visuals::Others::SniperCrosshair();
        Visuals::Others::RecoilCrosshair();
        Visuals::Others::GrenadePrediction();
        
        
    }
    fsnVMT->GetMethod<tFrameStageNotify>(FSN_INDEX)(thisptr, stage);
}

HFONT eFont;
void hkPaintTraverse(void* thisptr, VPANEL vguiPanel, bool forceRepaint, bool allowForce) {
    paintVMT->GetMethod<tPaintTraverse>(PTV_INDEX)(thisptr, vguiPanel, forceRepaint, allowForce);
    static VPANEL currentPanel = 0;
    if(!currentPanel)
        if(strstr(pPanel->GetName(vguiPanel), xorstr("FocusOverlayPanel"))) {
            eFont = pSurface->CreateFont(); // ESP Font
            pSurface->SetFontGlyphSet(eFont, xorstr("Segoe Ui"), 10, 80, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);
            currentPanel = vguiPanel;
        }
    if(vguiPanel == currentPanel){
        Visuals::ESP::ESPSurface();
        CRadar* g_radar = nullptr;
        if(visButton_Radar->state) {
            if(!g_radar) {
                g_radar = new CRadar();
            }
            g_radar->PaintTraverse();
        }
        
    }
    
}

void hkLockCursor(void* thisptr)
{
    if(butButton_Menu->state){
        pSurface->UnlockCursor();
        return;
    }
    lkcVMT->GetMethod<tLockCursor>(LKC_INDEX)(thisptr);
}

void GameHooker::Init(){
    LoadModules();
    ScanSigs();
    LoadInterfaces();
    HookVMTs();
    Offsets::getOffsets();
    InitGlobalVars();
}

Memory::Module* CLIENTMODULE;
Memory::Module* ENGINEMODULE;
Memory::Module* INPUTSYSTEMM;
Memory::Module* VGUI2MODULEM;
Memory::Module* MATRLSMODULE;
Memory::Module* VGUIMATSURFM;

void GameHooker::LoadModules(){
    CLIENTMODULE = new Memory::Module(xorstr("client.dylib"));
    ENGINEMODULE = new Memory::Module(xorstr("engine.dylib"));
    INPUTSYSTEMM = new Memory::Module(xorstr("inputsystem.dylib"));
    VGUI2MODULEM = new Memory::Module(xorstr("vgui2.dylib"));
    MATRLSMODULE = new Memory::Module(xorstr("materialsystem.dylib"));
    VGUIMATSURFM = new Memory::Module(xorstr("vguimatsurface.dylib"));
}

void GameHooker::Destroy(){}

uintptr_t GameHooker::clientModePointer;
C_PatternScanner* GameHooker::sigScanner;
void GameHooker::ScanSigs(){
    sigScanner = C_PatternScanner::get();
    clientModePointer = sigScanner->get_pointer(CLIENTMODULE->Name, (unsigned char*)xorstr("\x48\x8B\xB7\x00\x00\x00\x00\x48\x8D\x3D\x00\x00\x00\x00\x5D\xE9"), xorstr("xxx????xxx????xx"), 0xA) + 0x4;
}

void GameHooker::HookVMTs(){
    dmeVMT              = new VMT(pModelRender, (void*)hkDrawModelExecute, DME_INDEX);
    crtmVMT             = new VMT(pClientMode,  (void*)hkCreateMove, CMV_INDEX);
    fsnVMT              = new VMT(pClient,      (void*)hkFrameStageNotify, FSN_INDEX);
    paintVMT            = new VMT(pPanel,       (void*)hkPaintTraverse, PTV_INDEX);
    //lkcVMT              = new VMT(pSurface,     (void*)hkLockCursor, LKC_INDEX);
}

void GameHooker::LoadInterfaces(){
    pClientMode         = reinterpret_cast<IClientMode*>(clientModePointer);
    pPanel              = VGUI2MODULEM->GetInterface<IPanel>(xorstr("VGUI_Panel"));
    pCvar               = MATRLSMODULE->GetInterface<ICvar>(xorstr("VEngineCvar"));
    pModelRender        = ENGINEMODULE->GetInterface<IVModelRender>(xorstr("VEngineModel"));
    pClient             = CLIENTMODULE->GetInterface<IBaseClientDLL>(xorstr("VClient"));
    pEngine             = ENGINEMODULE->GetInterface<IEngineClient>(xorstr("VEngineClient"));
    pOverlay            = ENGINEMODULE->GetInterface<IVDebugOverlay>(xorstr("VDebugOverlay"));
    pModelInfo          = ENGINEMODULE->GetInterface<IVModelInfo>(xorstr("VModelInfoClient"));
    pSurface            = VGUIMATSURFM->GetInterface<ISurface>(xorstr("VGUI_Surface"));
    pEngineTrace        = ENGINEMODULE->GetInterface<IEngineTrace>(xorstr("EngineTraceClient"));
    pInputSystem        = INPUTSYSTEMM->GetInterface<IInputSystem>(xorstr("InputSystemVersion"));
    pMaterialSystem     = MATRLSMODULE->GetInterface<IVMaterialSystem>(xorstr("VMaterialSystem"));
    pEntList            = CLIENTMODULE->GetInterface<IClientEntityList>(xorstr("VClientEntityList"));
    if(!pGlobalVars) {
        uintptr_t globalVarsPointer = sigScanner->get_pointer(CLIENTMODULE->Name,(unsigned char*)xorstr("\x48\x8D\x05\x00\x00\x00\x00\x48\x8B\x00\xF3\x0F\x10\x00\x00\xF3\x0F\x11\x83\x00\x00\x00\x00"), xorstr("xxx????xxxxxx??xxxx????"), 0x3) + 0x4;
        pGlobalVars         =*(CGlobalVarsBase**)(globalVarsPointer);
    }
    pMoveHelper;
    pPrediction         = CLIENTMODULE->GetInterface<IPrediction>("./csgo/bin/osx64/client.dylib", "VClientPrediction");
    pGameMovement           = CLIENTMODULE->GetInterface<IGameMovement>("./csgo/bin/osx64/client.dylib", "GameMovement");
    
}

void GameHooker::InitGlobalVars() {
    /*if (!Glob::IsWeaponSwitched) {
        Glob::IsWeaponSwitched = new bool();
        *Glob::IsWeaponSwitched = false;
    }*/
    
//    if (!Glob::AimbotIsAiming) {
//        Glob::AimbotIsAiming = new bool();
 //       *Glob::AimbotIsAiming = false;
//    }
    
    /*if (!Glob::ZoomedFov) {
        Glob::ZoomedFov = new float();
        *Glob::ZoomedFov = 0.0f;
    }
    
    if (!Glob::SDLResW) {
        Glob::SDLResW = new int();
        *Glob::SDLResW = 0;
    }
    
    if (!Glob::SDLResH) {
        Glob::SDLResH = new int();
        *Glob::SDLResH = 0;
    }*/
}
