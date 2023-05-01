/******************************************************/
/**                                                  **/
/**      Features/TriggerBot.cpp                     **/
/**      |_ @package GO:SX Legit                     **/
/**      |_ @created 2018-01-18                      **/
/**      |_ @author André "vAx" Kalisch              **/
/**                                                  **/
/**      Copyright © 2017 @author                    **/
/**      All rights reserved.                        **/
/**                                                  **/
/******************************************************/

#include "TriggerBot.h"



C_BasePlayer* triggerTarget = nullptr;
long triggerTime = 0;

int CheatSettings::Aim::trigger_bot_delay = 20;

void Aim::CTriggerBot::CreateMove(CUserCmd *pCmd) {
    if (!miscButton_TriggerBot->state) {
        return;
    }
    C_BasePlayer* pLocal((C_BasePlayer*)pEntList->GetClientEntity(pEngine->GetLocalPlayer()));
    
    if (!pLocal || !pLocal->IsValidLivePlayer()) {
        Reset();
        return;
    }
    
    C_BaseCombatWeapon* activeWeapon = (C_BaseCombatWeapon*)pLocal->GetActiveWeapon();
    if (!activeWeapon) {
        Reset();
        return;
    }

    if (pInputSystem->IsButtonDown((ButtonCode_t)MOUSE_4)) {
        if (!pLocal->IsValidLivePlayer()) {
            return;
        }
   //     pCvar->ConsolePrintf("%s ", "\n mouse4");
//       if (activeWeapon->GetAmmo() == 0) {
//           pCvar->ConsolePrintf("%s ", "ammmo 0");
//            Reset();
//            return;
//        }
//        auto weapon = activeWeapon->GetItemDefinitionIndex();
//        if (activeWeapon->GetItemDefinitionIndex() > (int*)1 || activeWeapon->GetItemDefinitionIndex() <= (int*)40) {
//            pCvar->ConsolePrintf(weapon);
//            pCvar->ConsolePrintf("%s ", "\n wrong weapon");
//            Reset();
//            return;
//        }

        if (!triggerTarget) {
    //        pCvar->ConsolePrintf("%s ", "\n valid target");
            Vector traceStart, traceEnd;
            trace_t tr;

            QAngle viewAngles = pCmd->viewangles;
            QAngle viewAnglesRcs = viewAngles + (pLocal->GetAimPunchAngle() * 2.0f);

            Math::AngleVectors(viewAnglesRcs, traceEnd);

            traceStart = pLocal->GetEyePosition();
            traceEnd = traceStart + (traceEnd * 8192.0f);

            Ray_t ray;
            ray.Init(traceStart, traceEnd);
            CTraceFilter traceFilter;
            traceFilter.pSkip = pLocal;
            pEngineTrace->TraceRay(ray, 0x46004003, &traceFilter, &tr);
//            C_BaseEntity* player = (C_BaseEntity*)tr.m_pEnt;
            C_BasePlayer* player = (C_BasePlayer*)tr.m_pEnt;
            if (!player) {
     //           pCvar->ConsolePrintf("%s ", "\n enemy is wrong");
                return;
            }

            if (player->GetClientClass()->m_ClassID != en_class_id::CCSPlayer) {
                return;
           }
            
            if (player == pLocal || player->IsDormant() || !player->IsAlive() || player->IsImmune()) {
   //             pCvar->ConsolePrintf("%s ", "\n enemy is dead");
                return;
            }
            
            if (player->GetTeam() == pLocal->GetTeam()) {
     //           pCvar->ConsolePrintf("%s ", "\n enemy is same team");
                return;
            }
            
            bool filter;
            
            switch (tr.hitgroup) {
                case HitGroups::HITGROUP_HEAD:
                    filter = true;
                    break;
                case HitGroups::HITGROUP_CHEST:
                    filter = true;
                    break;
                case HitGroups::HITGROUP_STOMACH:
                    filter = true;
                    break;
                case HitGroups::HITGROUP_LEFTARM:
                case HitGroups::HITGROUP_RIGHTARM:
                    filter = true;
                    break;
                case HitGroups::HITGROUP_LEFTLEG:
                case HitGroups::HITGROUP_RIGHTLEG:
                    filter = true;
                    break;
                default:
                    filter = false;
            }
            
            if (!filter) {
    //            pCvar->ConsolePrintf("%s ", "\n filter broken");
                return;
            }
            
            Vector vMin, vMax;
//            if (Options::Aimbot::smokecheck && Utils::LineGoesThroughSmoke(traceStart, player->GetPredictedPosition(HITBOX_BODY, vMin, vMax))) {
//                return;
//           }

            triggerTarget = player;
            return;
        } else {
            bool delayExpired = false;
            if (CheatSettings::Aim::trigger_bot_delay > 0) {
                long currTime = GetTimeStamp();
                if (triggerTime == 0) {
                    triggerTime = currTime;
                }

                long triggerDelay = CheatSettings::Aim::trigger_bot_delay;
                long currentDelay = currTime - triggerTime;
                //pCvar->ConsolePrintf("%s \n", "current delay");
                //pCvar->ConsolePrintf("%d \n", currentDelay);
                if (currentDelay > triggerDelay) {
                    delayExpired = true;
                    triggerTime = 0;
                }
            } else {
                delayExpired = true;
            }

            if (delayExpired) {
                if (activeWeapon->EntityId() == en_item_definition_index::weapon_revolver) {
                    pCmd->buttons |= IN_ATTACK2;
                } else {
                    
                    pCmd->buttons |= IN_ATTACK;
                }
                Reset();

                return;
            }

            return;
        }
    } else {
        Reset();

        return;
    }
}

void Aim::CTriggerBot::Reset() {
    triggerTime = 0;
    triggerTarget = nullptr;
}

//std::shared_ptr<Features::CTriggerBot> TriggerBot = std::make_unique<Features::CTriggerBot>();

