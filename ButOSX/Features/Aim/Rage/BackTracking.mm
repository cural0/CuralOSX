/******************************************************/
/**                                                  **/
/**      Features/BackTracking.cpp                   **/
/**      |_ @package GO:SX Legit                     **/
/**      |_ @created 2018-03-29                      **/
/**      |_ @author André "vAx" Kalisch              **/
/**                                                  **/
/**      Copyright © 2017 @author                    **/
/**      All rights reserved.                        **/
/**                                                  **/
/******************************************************/

#include "BackTracking.h"


int CheatSettings::Backtracking::backtrack_ticks = 12;
int CheatSettings::Backtracking::backtrack_hitbox = 7;
int CheatSettings::Backtracking::backtrack_visual_type = 0;
bool CheatSettings::Backtracking::enabled = true;
bool CheatSettings::Backtracking::visibility_check = true;
Color CheatSettings::Backtracking::backtrack_color = Color(0, 0, 0, 255);

CBackTracking::CBackTracking() {}

void CBackTracking::CreateMove(CUserCmd* cmd) {
    if (!aimButton_BackTrack->state) {
        return;
    }
    
    if (CheatSettings::Backtracking::backtrack_ticks > 12) {
        CheatSettings::Backtracking::backtrack_ticks = 12;
    }
    
    C_BasePlayer* LocalPlayer = (C_BasePlayer*)pEntList->GetClientEntity(pEngine->GetLocalPlayer());
    if (!LocalPlayer || !LocalPlayer->IsAlive() || LocalPlayer->IsDormant()) {
        return;
    }
    
    QAngle viewAngle;
    pEngine->GetViewAngles(viewAngle);
    
    int bestTargetIndex = -1;
    float bestFov = FLT_MAX;
    try {
        for (int i = 1; i < pEngine->GetMaxClients(); i++) {
            C_BasePlayer* PlayerEntity = (C_BasePlayer*)pEntList->GetClientEntity(i);
            if (!PlayerEntity || PlayerEntity->IsDormant() || !PlayerEntity->IsAlive() || PlayerEntity == LocalPlayer || PlayerEntity->GetTeam() == LocalPlayer->GetTeam()) {
                continue;
            }
            
            float simtime = PlayerEntity->GetSimulationTime();
            Vector Hitbox = PlayerEntity->GetBonePosition(CheatSettings::Backtracking::backtrack_hitbox);
            int currentTick = cmd->command_number % (CheatSettings::Backtracking::backtrack_ticks + 1);
            BacktrackData[i][currentTick] = {simtime,  Hitbox};
            
            if (CheatSettings::Backtracking::backtrack_visual_type == 2 && currentTick == CheatSettings::Backtracking::backtrack_ticks) {
                PlayerEntity->SetupBones(BacktrackData[i][currentTick].BoneMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, PlayerEntity->GetSimulationTime());
            }
            
            if (CheatSettings::Backtracking::backtrack_visual_type == 3) {
                BacktrackData[i][currentTick].Origin = PlayerEntity->GetVecOrigin();
            }
            
            Vector ViewDir = Math::AngleVector(viewAngle + LocalPlayer->GetAimPunchAngle());
            float FOVDistance = Math::DistancePointToLine(Hitbox, LocalPlayer->GetEyePosition(), ViewDir);
            
            if (bestFov > FOVDistance) {
                bestFov = FOVDistance;
                bestTargetIndex = i;
            }
        }
        
        float bestTargetSimTime = 0.0f;
        if (bestTargetIndex != -1) {
            float tempFloat = FLT_MAX;
            Vector ViewDir = Math::AngleVector(cmd->viewangles + (LocalPlayer->GetAimPunchAngle() * 2.f));
            for (int t = 0; t <= CheatSettings::Backtracking::backtrack_ticks; ++t) {
                BacktrackData_t CurrTick = GetTick(bestTargetIndex, t);
                if (CurrTick.SimulationTime == -1.0f) {
                    continue;
                }
                float tempFOVDistance = Math::DistancePointToLine(CurrTick.Hitbox, LocalPlayer->GetEyePosition(), ViewDir);
                if (
                    tempFloat > tempFOVDistance &&
                    CurrTick.SimulationTime > LocalPlayer->GetSimulationTime() - 1
                ) {
                    tempFloat = tempFOVDistance;
                    bestTargetSimTime = CurrTick.SimulationTime;
                }
            }
            if (cmd->buttons & IN_ATTACK) {
                cmd->tick_count = TIME_TO_TICKS(bestTargetSimTime);
            }
        }
    } catch (std::exception e) {
        pCvar->ConsolePrintf("%s \n", "Failed to retrieve all backtrack ticks!");
        throw std::invalid_argument("Failed to retrieve all backtrack ticks!");
    }
}

BacktrackData_t CBackTracking::GetTick(int PlayerIndex, int Tick) {
    try {
        vecMulti::iterator it = BacktrackData.begin() + PlayerIndex;
        if (it == BacktrackData.end()) {
            return {};
        }
        
        vecSingle BacktrackTick = CBackTracking::BacktrackData.at(PlayerIndex);
        vecSingle::iterator sit = BacktrackTick.begin() + Tick;
        if (sit == BacktrackTick.end()) {
            return {};
        }
        
        return BacktrackTick.at(Tick);
    } catch (...) {
        pCvar->ConsolePrintf("%s \n", "Something went wrong when getting specific tick!");
        throw std::invalid_argument("Something went wrong when getting specific tick!");
    }
}

vecSingle CBackTracking::GetPlayer(int PlayerIndex) {
    try {
        vecMulti::iterator it = BacktrackData.begin() + PlayerIndex;
        if (it != BacktrackData.end()) {
            return CBackTracking::BacktrackData.at(PlayerIndex);
        }
    } catch (...) {
        pCvar->ConsolePrintf("%s \n", "Something went wrong when getting all ticks for Player!");
        throw std::invalid_argument("Something went wrong when getting all ticks for Player!");
    }
    
    return {};
}

void CBackTracking::PaintTraverse() {
    if (!CheatSettings::Backtracking::enabled || CheatSettings::Backtracking::backtrack_visual_type == 0) {
        return;
    }
    
    C_BasePlayer* LocalPlayer = (C_BasePlayer*)pEntList->GetClientEntity(pEngine->GetLocalPlayer());
    if (!LocalPlayer || !LocalPlayer->IsAlive() || LocalPlayer->IsDormant()) {
        return;
    }

    for (int i = 1; i < pEngine->GetMaxClients(); i++) {
        C_BasePlayer* PlayerEntity = (C_BasePlayer*)pEntList->GetClientEntity(i);
        if (!PlayerEntity || PlayerEntity->IsDormant() || !PlayerEntity->IsAlive() || PlayerEntity == LocalPlayer || PlayerEntity->GetTeam() == LocalPlayer->GetTeam()) {
            continue;
        }
        
        if (CheatSettings::Backtracking::backtrack_visual_type == 2) {
            BacktrackData_t LastTick = GetTick(i, CheatSettings::Backtracking::backtrack_ticks);
            if (LastTick.SimulationTime == -1.0f) {
                continue;
            }
            
            studiohdr_t* pStudioModel = nullptr;
            try {
                pStudioModel = pModelInfo->GetStudioModel(PlayerEntity->GetModel());
            } catch (...) {
                pCvar->ConsolePrintf("%s \n", "Failed to get StudioModel for Player Entity");
                throw std::invalid_argument("Failed to get StudioModel for Player Entity");
            }
            
            if (!pStudioModel) {
                continue;
            }

            for (int i = 0; i < pStudioModel->numbones; ++i) {
                try {
                    mstudiobone_t* pBone = pStudioModel->pBone(i);
                    if (!pBone || !(pBone->flags & BONE_USED_BY_HITBOX) || pBone->parent == -1) {
                        continue;
                    }
                    
                    Vector vBoneStartWorld = {LastTick.BoneMatrix[i][0][3], LastTick.BoneMatrix[i][1][3], LastTick.BoneMatrix[i][2][3]};
                    Vector vBoneEndWorld = {LastTick.BoneMatrix[pBone->parent][0][3], LastTick.BoneMatrix[pBone->parent][1][3], LastTick.BoneMatrix[pBone->parent][2][3]};
                    
                    int iChestBone = 6;
                    Vector vBreastBone;
                    Vector vUpperDirection = {
                        LastTick.BoneMatrix[iChestBone + 1][0][3] - LastTick.BoneMatrix[iChestBone][0][3],
                        LastTick.BoneMatrix[iChestBone + 1][1][3] - LastTick.BoneMatrix[iChestBone][1][3],
                        LastTick.BoneMatrix[iChestBone + 1][2][3] - LastTick.BoneMatrix[iChestBone][2][3]
                    };
                    Vector vChestBone = {
                        LastTick.BoneMatrix[iChestBone][0][3],
                        LastTick.BoneMatrix[iChestBone][1][3],
                        LastTick.BoneMatrix[iChestBone][2][3]
                    };
                    vBreastBone = vChestBone + vUpperDirection / 2;
                    Vector vDeltaChild = vBoneStartWorld - vBreastBone;
                    Vector vDeltaParent = vBoneEndWorld - vBreastBone;
                    
                    if ((vDeltaParent.Length() < 9.0f && vDeltaChild.Length() < 9.0f)) {
                        vBoneEndWorld = vBreastBone;
                    }
                    
                    if (i == iChestBone - 1) {
                        vBoneStartWorld = vBreastBone;
                    }
                    
                    if ((abs(vDeltaChild.z) < 5 && (vDeltaParent.Length() < 5.0f && vDeltaChild.Length() < 5.0f)) || i == iChestBone) {
                        continue;
                    }
                    
                    if (
                        CheatSettings::Backtracking::visibility_check &&
                        (
                            !IsVisible(LocalPlayer, PlayerEntity, vBoneStartWorld) ||
                            !IsVisible(LocalPlayer, PlayerEntity, vBoneEndWorld)
                        )
                    ) {
                        continue;
                    }
                    
                    Vector vBone1, vBone2;
                    if (!WorldToScreen(vBoneStartWorld, vBone1) || !WorldToScreen(vBoneEndWorld, vBone2)) {
                        continue;
                    }
                    
                    DrawLine((int)vBone1.x, (int)vBone1.y, (int)vBone2.x, (int)vBone2.y, CheatSettings::Backtracking::backtrack_color);
                } catch (...) {
                    pCvar->ConsolePrintf("%s \n", "Something went wrong when drawing backtrack skeleton.");
                    throw std::invalid_argument("Something went wrong when drawing backtrack skeleton.");
                }
            }
        } else {
            for (int tk = 0; tk <= CheatSettings::Backtracking::backtrack_ticks; ++tk) {
                if (CheatSettings::Backtracking::backtrack_visual_type == 1) {
                    try {
                        BacktrackData_t CurrentTick = GetTick(i, tk);
                        if (
                            CheatSettings::Backtracking::visibility_check &&
                            !IsVisible(LocalPlayer, PlayerEntity, CurrentTick.Hitbox)
                        ) {
                            continue;
                        }
                        
                        Vector HitboxInWorld;
                        if (!WorldToScreen(CurrentTick.Hitbox, HitboxInWorld)) {
                            continue;
                        }
                    
                        DrawBox((int)(HitboxInWorld.x - 1.5f), (int)(HitboxInWorld.y - 1.5f), 3, 3, CheatSettings::Backtracking::backtrack_color);
                    } catch (...) {
                        pCvar->ConsolePrintf("%s \n", "Something went wrong when drawing backtrack dots.");
                        throw std::invalid_argument("Something went wrong when drawing backtrack dots.");
                    }
                }
            }
        }
    }
}

void CBackTracking::InvalidateData(int PlayerIndex) {
    try {
        if (PlayerIndex != -1) {
            vecMulti::iterator it = BacktrackData.begin() + PlayerIndex;
            if (it != BacktrackData.end()) {
                BacktrackData.at(PlayerIndex).clear();
            }
            
            return;
        }
        
        BacktrackData.clear();
    } catch (...) {
        pCvar->ConsolePrintf("%s \n", "Something went wrong when clearing the backtrack data.");
        throw std::invalid_argument("Something went wrong when clearing the backtrack data.");
    }
}

std::shared_ptr<CBackTracking> Backtracking = std::make_unique<CBackTracking>();
