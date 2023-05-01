/******************************************************/
/**                                                  **/
/**      Features/BombTimer.cpp                      **/
/**      |_ @package GO:SX Legit                     **/
/**      |_ @created 2018-01-22                      **/
/**      |_ @author André "vAx" Kalisch              **/
/**                                                  **/
/**      Copyright © 2017 @author                    **/
/**      All rights reserved.                        **/
/**                                                  **/
/******************************************************/

#include "BombTimer.h"

CBombTimer::CBombTimer() {}


int CheatSettings::Drawing::bomb_timer = 0;



std::wstring StringToWString2(std::string const &s) {
    std::wstring_convert<
        usable_facet<std::codecvt<char32_t,char,std::mbstate_t>>
        ,char32_t> convert;
    std::u32string utf32 = convert.from_bytes(s);
    static_assert(sizeof(wchar_t)==sizeof(char32_t),"char32_t and wchar_t must have same size");
    return {begin(utf32),end(utf32)};
}


void DrawString2(int x, int y, Color color, HFONT font, bool bCenter, const char* szString) {
    std::wstring buffer = StringToWString2(szString);
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


C_BasePlantedC4* GetPlantedBomb() {
    for (int i = pEngine->GetMaxClients(); i < pEntList->GetHighestEntityIndex(); i++) {
        C_BaseEntity* entity = (C_BaseEntity*)pEntList->GetClientEntity(i);
        if (!entity || entity->IsDormant()) {
            continue;
        }
        
        if (entity->GetClientClass()->m_ClassID == en_class_id::CPlantedC4) {
            return (C_BasePlantedC4*)entity;
        }
    }
    
    return nullptr;
}

C_BasePlayer* GetBombDefuser(C_BasePlantedC4* player) {
    return (C_BasePlayer*)pEntList->GetClientEntityFromHandle(player->GetBombDefuser()
    );
}

void CBombTimer::PaintTraverse() {
    if ((BombDisplayType)CheatSettings::Drawing::bomb_timer == DISPLAY_NONE) {
        return;
    }

    C_BasePlayer* LocalPlayer = (C_BasePlayer*)pEntList->GetClientEntity(pEngine->GetLocalPlayer());
    if (!LocalPlayer) {
        return;
    }

    C_BasePlantedC4* bomb = GetPlantedBomb();
    if (!bomb || !bomb->IsBombTicking()) {
        return;
    }

    BombTimerTime = bomb->GetBombTime() - (0x20 * LocalPlayer->GetTickBase());
    switch ((BombDisplayType)CheatSettings::Drawing::bomb_timer) {
        case DISPLAY_BOTH:
            DrawOnScreenTimer(bomb);
            DrawWorldModelTimer(bomb);
        break;
        case DISPLAY_ONSCREEN:
            DrawOnScreenTimer(bomb);
        break;
        case DISPLAY_INWORLD:
            DrawWorldModelTimer(bomb);
        break;
        default:
        break;
    }
}

void CBombTimer::DrawOnScreenTimer(C_BasePlantedC4 *bomb) {
    if (!bomb || !bomb->IsBombTicking()) {
        return;
    }

    float TimerLength = bomb->GetTimerLength();
    float BombTimer = bomb->GetBombTime() - (0x20 * LocalPlayer->GetTickBase());

    float TimerPercent = BombTimer / TimerLength * 100.0f;
    //float BombOnScreenSize = *Glob::SDLResW / 100 * TimerPercent;
    float BombOnScreenSize = 1920 / 100 * TimerPercent;
    
    //float HalfScreenWidth = *Glob::SDLResW / 2;
    float HalfScreenWidth = 1920 / 2;
    
    float HalfBombOnScreenSize = BombOnScreenSize / 2;
    float OnScreenLeftPadding = HalfScreenWidth - HalfBombOnScreenSize;

    //DrawRect((int)OnScreenLeftPadding, 0, (int)BombOnScreenSize, 6, GetBombColor(bomb));
    
    
    pSurface->DrawSetColor(GetBombColor(bomb));
    pSurface->DrawFilledRect((int)OnScreenLeftPadding, 0, (int)OnScreenLeftPadding + (int)BombOnScreenSize, 0 + 6);
    pSurface->DrawSetColor(255, 255, 255);
}

void CBombTimer::DrawWorldModelTimer(C_BasePlantedC4 *bomb) {
    if (!bomb || !bomb->IsBombTicking()) {
        return;
    }

    Vector BombPosition, BombMins, BombMaxs;
    BombPosition = bomb->GetVecOrigin();
    BombPosition.y -= 3.0f;

    Vector BombScreenPosition;
    if (!WorldToScreen(BombPosition, BombScreenPosition)) {
        return;
    }

    if (BombTimerTime > 0.01f && !bomb->IsBombDefused()) {
        char buffer[64];
        sprintf(buffer, "Bomb: %.1f", BombTimerTime);

        DrawString2((int)BombScreenPosition.x, (int)BombScreenPosition.y, GetBombColor(bomb), eFont, true, buffer);
    }
}

Color CBombTimer::GetBombColor(C_BasePlantedC4* bomb) {
    bool IsBombDefusing = false;
    C_BasePlayer* BombDefuser = GetBombDefuser(bomb);
    //C_BasePlayer* BombDefuser = bomb->GetBombDefuser();
    if (BombDefuser && BombDefuser->IsDefusing()) {
        IsBombDefusing = true;
    }
    Color BombColor = Color(200, 0, 0);
    if (LocalPlayer->GetTeam() == TEAM_COUNTER_TERRORIST) {
        bool WithDefuseKit = LocalPlayer->HasDefuser();
        if (BombTimerTime >= 5.5f && WithDefuseKit) {
            BombColor = Color(0, 200, 0);
        } else if (BombTimerTime >= 10.5f  && !WithDefuseKit) {
            BombColor = Color(0, 200, 0);
        }
    } else if (LocalPlayer->GetTeam() == TeamID::TEAM_TERRORIST) {
        BombColor = Color(0, 200, 0);
    }

    if (IsBombDefusing) {
        BombColor = Color(0, 200, 0);
    }

    return BombColor;
}

//std::shared_ptr<Features::CBombTimer> BombTimer = std::make_unique<Features::CBombTimer>();
