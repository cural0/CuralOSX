//
//  EnginePrediction.cpp
//  GOSX Pro
//
//  Created by Andre Kalisch on 18.04.17.
//  Copyright © 2017 Andre Kalisch. All rights reserved.
//

#include "EnginePrediction.h"

std::shared_ptr<CEnginePrediction> CEnginePrediction::instance = nullptr;

CEnginePrediction::CEnginePrediction() {}

std::map<int, const char*> CEnginePrediction::GetModeList() {
    std::map<int, const char*> select;

    select[0] = "0"; // simple
    select[1] = "1"; // full

    return select;
}

std::map<int, const char*> CEnginePrediction::GetModeValues() {
    std::map<int, const char*> values;

    values[0] = "simple"; // simple
    values[1] = "full"; // full

    return values;
}

void CEnginePrediction::Run(CUserCmd* cmd, C_BaseCombatWeapon* pWeapon, C_BasePlayer* pLocal) {
    if (!pMoveHelper) {
        return;
    }

    float backup = pGlobalVars->frametime;
    int oldFlags = *pLocal->GetFlags();

    pGlobalVars->frametime = pGlobalVars->interval_per_tick;

    pWeapon->UpdateAccuracyPenalty();

    CMoveData data;
    memset(&data, 0, sizeof(data));

    pPrediction->SetupMove(pLocal, cmd, pMoveHelper, &data);
    pGameMovement->ProcessMovement(pLocal, &data);
    pPrediction->FinishMove(pLocal, cmd, &data);

    pGlobalVars->frametime = backup;
    *pLocal->GetFlags() = oldFlags;
}


void CEnginePrediction::Start(CUserCmd* cmd) {
    if (!pMoveHelper) {
        return;
    }

    C_BasePlayer* LocalPlayer((C_BasePlayer*)pEntList->GetClientEntity(pEngine->GetLocalPlayer()));
    if (!LocalPlayer) {
        return;
    }

    C_BaseCombatWeapon* weapon = (C_BaseCombatWeapon*)pEntList->GetClientEntityFromHandle(LocalPlayer->GetActiveWeapon());
    if (!weapon) {
        return;
    }

    oldPFlags = *LocalPlayer->GetFlags();

    m_flOldCurtime = pGlobalVars->curtime;
    m_flOldFrametime = pGlobalVars->frametime;

    pGlobalVars->curtime = LocalPlayer->GetTickBase() * pGlobalVars->interval_per_tick;
    pGlobalVars->frametime = pGlobalVars->interval_per_tick;

    UpdateAccuracyPenalty(weapon);

    CMoveData data;
    memset(&data, 0, sizeof(data));

    pGameMovement->StartTrackPredictionErrors(LocalPlayer);

    pMoveHelper->SetHost(LocalPlayer);
    pPrediction->SetupMove(LocalPlayer, cmd, pMoveHelper, &data);
    pGameMovement->ProcessMovement(LocalPlayer, &data);
    pPrediction->FinishMove(LocalPlayer, cmd, &data);
}

void CEnginePrediction::End() {
    if (!pMoveHelper) {
        return;
    }

    C_BasePlayer* LocalPlayer((C_BasePlayer*)pEntList->GetClientEntity(pEngine->GetLocalPlayer()));
    if (!LocalPlayer) {
        return;
    }

    pGameMovement->FinishTrackPredictionErrors(LocalPlayer);
    pMoveHelper->SetHost(0);

    *LocalPlayer->GetFlags() == oldPFlags;

    pGlobalVars->curtime = m_flOldCurtime;
    pGlobalVars->frametime = m_flOldFrametime;
}
