//
//  ESP.cpp
//  ButOSX
//
//  Created by Can on 23.01.2021.
//  Copyright © 2021 Lyceion. All rights reserved.
//

//FULLY STOLEN RN I'M GONNE SWITCH TO THE IMGUI RENDER AND WRITE FROM SCRATH. THAT WAS FOR SHOWDOWN! 28.01.2021
//YEAH ESP IS PASTED FROM MICROWAVE. DONT JUDGE ME PLS BUT IT DOESN'T MEAN IT'S FORKED FROM MICROWAVE! 03.05.2021

#include "Visuals.hpp"
#include "CheatSettings.h"
#include "imgui.h"
#include "TouchBar.h"
#include "Utils.h"
#include "AutoWalls.h"
#include "manager.h"


bool CheatSettings::ESP::enabled;
bool CheatSettings::ESP::box = false;
bool CheatSettings::ESP::name = false;
bool CheatSettings::ESP::health = true;
bool CheatSettings::ESP::skeleton = true;


//On OS X wstring uses UTF-32 rather than UTF-16. -> https://stackoverflow.com/questions/10737644/convert-const-char-to-wstring
std::wstring StringToWString(std::string const &s) {
    std::wstring_convert<
        usable_facet<std::codecvt<char32_t,char,std::mbstate_t>>
        ,char32_t> convert;
    std::u32string utf32 = convert.from_bytes(s);
    static_assert(sizeof(wchar_t)==sizeof(char32_t),"char32_t and wchar_t must have same size");
    return {begin(utf32),end(utf32)};
}

void FillRGBA(int x, int y, int w, int h, Color color) {
    pSurface->DrawSetColor(color);
    pSurface->DrawFilledRect(x, y, x +  w, y + h);
    pSurface->DrawSetColor(255, 255, 255);
}

void DrawBox(int x, int y, int w, int h, Color color) {
    pSurface->DrawSetColor(color);
    pSurface->DrawOutlinedRect(x, y, x + w, y + h);
    pSurface->DrawSetColor(255, 255, 255);
}

void DrawBoxOutline(int x, int y, int w, int h, Color color) {
    DrawBox(x + 1, y + 1, w - 2, h - 2, Color(0, 0, 0, 120));
    DrawBox(x - 1, y - 1, w + 2, h + 2, Color(0, 0, 0, 120));
    DrawBox(x, y, w, h, color);
}

void DrawLine(int x, int y, int xx, int yy, Color color) {
    pSurface->DrawSetColor(color);
    pSurface->DrawLine(x, y, xx, yy);
    pSurface->DrawSetColor(255, 255, 255);
}

void DrawString(int x, int y, Color color, HFONT font, bool bCenter, const char* szString) {
    std::wstring buffer = StringToWString(szString);
    int strSize = (int)wcslen(buffer.c_str());
    if(bCenter) {
        int wide, tall;
        pSurface->GetTextSize(font, buffer.c_str(), wide, tall);
        x -= wide / 2;
        y -= tall / 2;
    }
    pSurface->DrawSetTextPos(x, y);
    pSurface->DrawSetTextFont(font);
    pSurface->DrawSetTextColor(color);
    pSurface->DrawPrintText(buffer.c_str(), strSize);
    pSurface->DrawSetTextColor(255, 255, 255, 255);
}

void DrawHealthbar(int x, int y, int w, int h, int health, Color color) {
    
    if(health > 100)
        health = 100;
    
    int hw = h - ((h) * health) / 100;
    FillRGBA(x, y - 1, w, h + 2, Color(0, 0, 0, 120));
    FillRGBA(x, y + hw - 1, w, h - hw + 2, color);
    DrawBox(x, y - 1, w, h + 2, Color(0, 0, 0, 200));
}




bool WorldToScreen(Vector& vFrom, Vector& vTo) {
    return (pOverlay->ScreenPosition(vFrom, vTo) != 1);
}

struct bBoxStyle {
    int x, y, w, h;
};

void test2() {
    return;
}

bool DrawPlayerBox(C_BaseEntity* pEntity, bBoxStyle& boxes) {
    if (!pEntity)
        return false;
    
    Vector vOrigin = pEntity->GetVecOrigin();
    Vector min = pEntity->GetCollideable()->OBBMins() + vOrigin;
    Vector max = pEntity->GetCollideable()->OBBMaxs() + vOrigin;
    
    Vector points[] = { Vector(min.x, min.y, min.z),
        Vector(min.x, max.y, min.z),
        Vector(max.x, max.y, min.z),
        Vector(max.x, min.y, min.z),
        Vector(max.x, max.y, max.z),
        Vector(min.x, max.y, max.z),
        Vector(min.x, min.y, max.z),
        Vector(max.x, min.y, max.z) };

    Vector flb;
    Vector brt;
    Vector blb;
    Vector frt;
    Vector frb;
    Vector brb;
    Vector blt;
    Vector flt;
    
    
    if (!WorldToScreen(points[3], flb) || !WorldToScreen(points[5], brt)
        || !WorldToScreen(points[0], blb) || !WorldToScreen(points[4], frt)
        || !WorldToScreen(points[2], frb) || !WorldToScreen(points[1], brb)
        || !WorldToScreen(points[6], blt) || !WorldToScreen(points[7], flt))
        return false;
    
    Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };
    
    float left = flb.x;        // left
    float top = flb.y;        // top
    float right = flb.x;    // right
    float bottom = flb.y;    // bottom
    
    for (int i = 1; i < 8; i++)
    {
        if (left > arr[i].x)
            left = arr[i].x;
        if (bottom < arr[i].y)
            bottom = arr[i].y;
        if (right < arr[i].x)
            right = arr[i].x;
        if (top > arr[i].y)
            top = arr[i].y;
    }
    
    boxes.x = left;
    boxes.y = top;
    boxes.w = right - left;
    boxes.h = bottom - top;
    return true;
}

bool TestTrace(C_BaseEntity* pEntity, C_BaseEntity* pLocal) {
    Ray_t ray;
    trace_t trace;
    CTraceFilter filter;
    filter.pSkip = pLocal;
    
    ray.Init(pLocal->GetEyePosition(), pEntity->GetEyePosition());
    pEngineTrace->TraceRay(ray, 0x4600400B, &filter, &trace);
    
    return (trace.m_pEnt == pEntity || trace.fraction > 0.99f);
}


void DrawSkeleton(C_BaseEntity* PlayerEntity, C_BaseEntity* LocalPlayer, Color color){


    if (!PlayerEntity || !PlayerEntity->IsValidLivePlayer() || PlayerEntity == LocalPlayer) {
        return;
    }

    QAngle viewAngle;
    pEngine->GetViewAngles(viewAngle);

    studiohdr_t* pStudioModel = pModelInfo->GetStudioModel(PlayerEntity->GetModel());
    if (!pStudioModel) {
        return;
    }
    
    static matrix3x4_t matrix[MAXSTUDIOBONES];
    if (!PlayerEntity->SetupBones(matrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, PlayerEntity->GetSimulationTime())) {
        return;
    }
    static int iChestBone = 6;
    
    for (int i = 0; i < pStudioModel->numbones; i++) {
        mstudiobone_t* pBone = pStudioModel->pBone(i);
        if (!pBone || !(pBone->flags & BONE_USED_BY_HITBOX) || pBone->parent == -1) {
            continue;
        }

        Vector vBone1, vBoneOut1, vBone2, vBoneOut2;
        vBone1 = {matrix[i].m_flMatVal[0][3], matrix[i].m_flMatVal[1][3], matrix[i].m_flMatVal[2][3]};
        vBone2 = {matrix[pBone->parent].m_flMatVal[0][3], matrix[pBone->parent].m_flMatVal[1][3], matrix[pBone->parent].m_flMatVal[2][3]};
        
        Vector vBreastBone;
        Vector ChestboneParent = {matrix[iChestBone + 1].m_flMatVal[0][3], matrix[iChestBone + 1].m_flMatVal[1][3], matrix[iChestBone + 1].m_flMatVal[2][3]};
        Vector Chestbone = {matrix[iChestBone].m_flMatVal[0][3], matrix[iChestBone].m_flMatVal[1][3], matrix[iChestBone].m_flMatVal[2][3]};
        Vector vUpperDirection = ChestboneParent - Chestbone;
        vBreastBone = Chestbone + vUpperDirection / 2;
        Vector vDeltaChild = vBone1 - vBreastBone;
        Vector vDeltaParent = vBone2 - vBreastBone;

        if ((vDeltaParent.Length() < 9.0f && vDeltaChild.Length() < 9.0f)) {
            vBone2 = vBreastBone;
        }
        
        if (i == iChestBone - 1) {
            vBone1 = vBreastBone;
        }
        
        if ((abs(vDeltaChild.z) < 5 && (vDeltaParent.Length() < 5.0f && vDeltaChild.Length() < 5.0f)) || i == iChestBone) {
            continue;
        }

        if (!WorldToScreen(vBone1, vBoneOut1) || !WorldToScreen(vBone2, vBoneOut2)) {
            continue;
        }
   //     Color boneColor = Options::Drawing::color_bone_esp;
        
        DrawLine(vBoneOut1.x, vBoneOut1.y, vBoneOut2.x, vBoneOut2.y, color);
    }
}





extern void Visuals::ESP::ESPSurface() {
    if(!visButton_ESP->state)
        return;
    C_BasePlayer* pLocal = (C_BasePlayer*)pEntList->GetClientEntity(pEngine->GetLocalPlayer());
    for(int i = 0; i < pEntList->GetHighestEntityIndex(); i++) {
        C_BaseEntity* pEntity = (C_BaseEntity*)pEntList->GetClientEntity(i);
        if(!pEntity || !pLocal || pEntity->GetHealth() < 1 || pEntity->IsDormant() || pEntity->IsGhost() || pEntity == pLocal)
            continue;
        bBoxStyle players;
        auto isVisible = TestTrace(pEntity, pLocal);
        IEngineClient::player_info_t pInfo;
        pEngine->GetPlayerInfo(i, &pInfo);
        if(DrawPlayerBox(pEntity, players)) {
            if(CheatSettings::ESP::box){
                if(pEntity->GetTeam() == TeamID::TEAM_TERRORIST)
                    DrawBoxOutline(players.x, players.y, players.w, players.h, isVisible ? Color::Red() : Color::Yellow());
                if(pEntity->GetTeam() == TeamID::TEAM_COUNTER_TERRORIST)
                    DrawBoxOutline(players.x, players.y, players.w, players.h, isVisible ? Color::Green() : Color::Blue());
            }
            if(CheatSettings::ESP::name)
                DrawString(players.x + players.w / 2, players.y - 12, Color::White(), eFont, true, pInfo.name);
            if(CheatSettings::ESP::health)
                DrawHealthbar(players.x - 5, players.y, 3, players.h, pEntity->GetHealth(), Color::Green());
            if(CheatSettings::ESP::skeleton){
                if(pEntity->GetTeam() == TeamID::TEAM_TERRORIST)
                    DrawSkeleton(pEntity, pLocal, isVisible ? Color::Red() : Color::Yellow());
                if(pEntity->GetTeam() == TeamID::TEAM_COUNTER_TERRORIST)
                    DrawSkeleton(pEntity, pLocal, isVisible ? Color::Green() : Color::Blue());
            }
                
        }
    }
}
