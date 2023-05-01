#pragma once

//#include "ValveSDK.h"

#include "../Vector.h"
#include "CommonFunctions.h"
#include "IBaseClientDLL.h"
//#include "IVModuleInfo.h"
#include <stdio.h>
#include "TouchBar.h"
#include "CMath.h"
#include "cstrike_icons.h"


struct model_t {
    char name[255];
};

enum MoveType_t
{
    MOVETYPE_NONE = 0,
    MOVETYPE_ISOMETRIC,
    MOVETYPE_WALK,
    MOVETYPE_STEP,
    MOVETYPE_FLY,
    MOVETYPE_FLYGRAVITY,
    MOVETYPE_VPHYSICS,
    MOVETYPE_PUSH,
    MOVETYPE_NOCLIP,
    MOVETYPE_LADDER,
    MOVETYPE_OBSERVER,
    MOVETYPE_CUSTOM,
    MOVETYPE_LAST = MOVETYPE_CUSTOM,
    MOVETYPE_MAX_BITS = 4
};

class ICollideable
{
public:
    virtual void pad0( );
    virtual const Vector& OBBMins( ) const;
    virtual const Vector& OBBMaxs( ) const;
};

class IHandleEntity
{
public:
    virtual ~IHandleEntity() {};
};

class IClientUnknown : public IHandleEntity {};
class IClientRenderable
{
public:
    virtual ~IClientRenderable() {};
    
    model_t* GetModel()
    {
        typedef model_t* (* oGetModel)(void*);
        return getvfunc<oGetModel>(this, 8)(this);
    }
    
    bool SetupBones(matrix3x4_t* pBoneMatrix, int nMaxBones, int nBoneMask, float flCurTime = 0)
    {
        typedef bool (* oSetupBones)(void*, matrix3x4_t*, int, int, float);
        return getvfunc<oSetupBones>(this, 13)(this, pBoneMatrix, nMaxBones, nBoneMask, flCurTime);
    }
};

class IClientNetworkable
{
public:
    virtual ~IClientNetworkable() {};
    
    ClientClass* GetClientClass()
    {
        typedef ClientClass* (* oGetClientClass)(void*);
        return getvfunc<oGetClientClass>(this, 2)(this);
    }
};

class IClientThinkable
{
public:
    virtual ~IClientThinkable() {};
};

class IClientEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable, public IClientThinkable
{
public:
    virtual ~IClientEntity() {};
    
    int GetIndex()
    {
        return *(int*)((uintptr_t)this + 0x94);
    }
};

class C_BaseEntity : public IClientEntity
{
public:
    int GetId()
    {
        return *(int*)((uintptr_t)this + 0x94);
    }
    
    int* GetModelIndex()
    {
        return (int*)((uintptr_t)this + offsets.DT_BaseViewModel.m_nModelIndex);
    }
    
    int GetHealth()
    {
        return *(int*)((uintptr_t)this + offsets.DT_BasePlayer.m_iHealth);
    }
    
    int GetArmor()
    {
        return *(int*)((uintptr_t)this + offsets.DT_CSPlayer.m_ArmorValue);
    }
    
    int GetMoney()
    {
        return *(int*)((uintptr_t)this + offsets.DT_BasePlayer.m_iAccount);
    }
    
    int HasHelmet()
    {
        return *(int*)((uintptr_t)this + offsets.DT_CSPlayer.m_bHasHelmet);
    }
    
    int HasDefuser()
    {
        return *(int*)((uintptr_t)this + offsets.DT_CSPlayer.m_bHasDefuser);
    }
    
    int GetTeam()
    {
        return *(int*)((uintptr_t)this + offsets.DT_BaseEntity.m_iTeamNum);
    }
    
    bool IsDormant()
    {
        //return false;
        //return *(bool*)((uintptr_t)this + offsets.DT_BasePlayer.m_bDormant);
        return *(bool*)((uintptr_t)this + 0x125);
    }
    
    unsigned char GetLifeState()
    {
        return *(unsigned char*)((uintptr_t)this + offsets.DT_BasePlayer.m_lifeState);
    }
    
    bool IsAlive()
    {
        return this->GetHealth() > 0;
    }
    
    bool IsValidLivePlayer()
    {
        return !IsDormant() && IsAlive();
        //return IsAlive();
    }
    
//    bool IsEnemy()
//    {
//        const static auto mp_teammates_are_enemies = pCvar->FindVar(xorstr("mp_teammates_are_enemies"));
//
//        if (mp_teammates_are_enemies->GetInt())
//            return this != localPlayer;
//
//        return this->GetTeam() != csgo::game::local->m_iTeamNum ( );
//    } I'M GONNA FIX WHEN I DO OPTIMIZATION.
    
    int* GetWeapons()
    {
        return (int*)((uintptr_t)this + offsets.DT_BaseCombatCharacter.m_hMyWeapons);
    }
    
    void* GetActiveWeapon()
    {
        return (void*)((uintptr_t)this + offsets.DT_BaseCombatCharacter.m_hActiveWeapon);
    }
    
//    C_BaseCombatWeapon* GetActiveWeapon2()
//    {
//        return (C_BaseCombatWeapon*)pEntList->GetClientEntityFromHandle(local->GetActiveWeapon());
//        //return (void*)((uintptr_t)this + offsets.DT_BaseCombatCharacter.m_hActiveWeapon);
//    }
    
    void* GetViewModel()
    {
        return (void*)((uintptr_t)this + offsets.DT_BasePlayer.m_hViewModel);
    }
    
    Vector GetVecOrigin()
    {
        return *(Vector*)((uintptr_t)this + offsets.DT_BaseEntity.m_vecOrigin);
    }
    
    Vector GetVecViewOffset()
    {
        return *(Vector*)((uintptr_t)this + offsets.DT_BasePlayer.m_vecViewOffset);
    }
    
    Vector GetEyePosition()
    {
        return this->GetVecOrigin() + this->GetVecViewOffset();
    }
    
    int* GetFlags()
    {
        return (int*)((uintptr_t)this + offsets.DT_BasePlayer.m_fFlags);
    }
    
    float GetFOV()
    {
        return *(float*)((uintptr_t)this + offsets.DT_CSPlayer.m_iDefaultFOV);
    }
    
    MoveType_t GetMoveType()
    {
        return *(MoveType_t*)((uintptr_t)this + offsets.DT_BaseEntity.m_MoveType);
    }
    
    float GetFlashDuration()
    {
        return *(float*)((uintptr_t)this + offsets.DT_CSPlayer.m_flFlashDuration);
    }
    
    float* GetFlashMaxAlpha()
    {
        return (float*)((uintptr_t)this + offsets.DT_CSPlayer.m_flFlashMaxAlpha);
    }
    
    bool IsDefusing()
    {
        return *(bool*)((uintptr_t)this + offsets.DT_CSPlayer.m_bIsDefusing);
    }
    
    bool IsGrabbingHostage()
    {
        return *(bool*)((uintptr_t)this + offsets.DT_CSPlayer.m_bIsGrabbingHostage);
    }
    
    bool IsScoped()
    {
        return *(bool*)((uintptr_t)this + offsets.DT_CSPlayer.m_bIsScoped);
    }
    
    bool IsRescuing()
    {
        return *(bool*)((uintptr_t)this + offsets.DT_CSPlayer.m_bIsRescuing);
    }
    
    bool IsImmune()
    {
        return *(bool*)((uintptr_t)this + offsets.DT_CSPlayer.m_bGunGameImmunity);
    }
    
    bool IsGhost() {
        return *(bool*)((uintptr_t)this + offsets.DT_CSPlayer.m_bIsPlayerGhost);
    }
    
    /*Vector GetHitboxPosition(int Hitbox, Vector &vMin, Vector &vMax) {
        static matrix3x4_t matrix[MAXSTUDIOBONES];
        if (!this->SetupBones(matrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, this->GetSimulationTime())) {
            return Vector(0, 0, 0);
        }
        
        studiohdr_t* hdr = pModuleInfo->GetStudioModel(this->GetModel());
        mstudiohitboxset_t* set = hdr->pHitboxSet(0);
        mstudiobbox_t* hitbox = set->pHitbox(Hitbox);
        if (!hitbox) {
            return Vector(0, 0, 0);
        }
        
        Vector vCenter;
        Math::VectorTransform(hitbox->bbmin, matrix[hitbox->bone], vMin);
        Math::VectorTransform(hitbox->bbmax, matrix[hitbox->bone], vMax);
        
        vCenter = (vMin + vMax) * 0.50f;

        return vCenter;
    }*/
    
    Vector GetBonePosition(int boneIndex)
    {
        matrix3x4_t BoneMatrix[MAXSTUDIOBONES];
        
        if (!this->SetupBones(BoneMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, this->GetSimulationTime()))
            return Vector(0, 0, 0);
        
        matrix3x4_t hitbox = BoneMatrix[boneIndex];
        
        return Vector(hitbox[0][3], hitbox[1][3], hitbox[2][3]);
    }
    

    bool IsOnGround() {
        if (*this->GetFlags() & FL_ONGROUND)
        return true;
    }
    
    int* GetObserverMode()
    {
        return (int*)((uintptr_t)this + offsets.DT_BasePlayer.m_iObserverMode);
    }
    
    void* GetObserverTarget()
    {
        return (void*)((uintptr_t)this + offsets.DT_BasePlayer.m_hObserverTarget);
    }
    
    ICollideable* GetCollideable()
    {
        return (ICollideable*)((uintptr_t)this + offsets.DT_BaseEntity.m_Collision);
    }
    
    bool* GetSpotted()
    {
        return (bool*)((uintptr_t)this + offsets.DT_BaseEntity.m_bSpotted);
    }
    
    float GetSimulationTime()
    {
        return (float)((uintptr_t)this + offsets.DT_BaseEntity.m_flSimulationTime);
    }
    
    char* GetLastPlaceName()
    {
        return (char*)((uintptr_t)this + offsets.DT_BasePlayer.m_szLastPlaceName);
    }
    
    Vector GetVelocity()
    {
        return *(Vector*)((uintptr_t)this + offsets.DT_BasePlayer.m_vecVelocity);
    }
    
    /*Vector GetPredictedPosition(int Hitbox, Vector &vMin, Vector &vMax) {
        return Math::ExtrapolateTick(this->GetHitboxPosition(Hitbox, vMin, vMax), this->GetVelocity());
    }*/
    
    QAngle GetHeadRotation()
    {
        return *(QAngle*)((uintptr_t)this + offsets.DT_BasePlayer.m_angRotation);
    }
    
    float* GetLowerBodyYawTarget()
    {
        return (float*)((uintptr_t)this + offsets.DT_BasePlayer.m_flLowerBodyYawTarget);
    }
};

/* generic game classes */
class C_BasePlayer : public C_BaseEntity
{
public:
    int* GetWeapons()
    {
        return (int*)((uintptr_t)this + offsets.DT_BaseCombatCharacter.m_hMyWeapons);
    }
    
    QAngle GetAimPunchAngle()
    {
        return *(QAngle*)((uintptr_t)this + offsets.DT_BasePlayer.m_aimPunchAngle);
    }
    
    QAngle* GetViewPunchAngle()
    {
        return (QAngle*)((uintptr_t)this + offsets.DT_BasePlayer.m_viewPunchAngle);
    }
    
    unsigned int GetTickBase()
    {
        return *(unsigned int*)((uintptr_t)this + offsets.DT_BasePlayer.m_nTickBase);
    }
    
    int GetShotsFired()
    {
        return *(int*)((uintptr_t)this + offsets.DT_CSPlayer.m_iShotsFired);
    }
    
};

class C_BasePlantedC4 : public C_BaseEntity
{
public:
    
    bool IsBombTicking()
    {
        return (bool)((uintptr_t)this + offsets.DT_PlantedC4.m_bBombTicking);
    }
    
    float GetBombTime()
    {
        return *(float*)((uintptr_t)this + offsets.DT_PlantedC4.m_flC4Blow);
    }
    
    float GetTimerLength()
    {
        return *(float*)((uintptr_t)this + offsets.DT_PlantedC4.m_flTimerLength);
    }
    
    bool IsBombDefused()
    {
        return *(bool*)((uintptr_t)this + offsets.DT_PlantedC4.m_bBombDefused);
    }
    
    void* GetBombDefuser()
    {
        return (void*)((uintptr_t)this + offsets.DT_PlantedC4.m_hBombDefuser);
    }
};

class C_BaseAttributableItem : public C_BaseEntity
{
public:
    int* GetItemDefinitionIndex()
    {
        return (int*)((uintptr_t)this + offsets.DT_BaseAttributableItem.m_iItemDefinitionIndex);
    }
    
    int* GetItemIDHigh()
    {
        return (int*)((uintptr_t)this + offsets.DT_BaseAttributableItem.m_iItemIDHigh);
    }
    
    int* GetEntityQuality()
    {
        return (int*)((uintptr_t)this + offsets.DT_BaseAttributableItem.m_iEntityQuality);
    }
    
    char* GetCustomName()
    {
        return (char*)((uintptr_t)this + offsets.DT_BaseAttributableItem.m_szCustomName);
    }
    
    int* GetFallbackPaintKit()
    {
        return (int*)((uintptr_t)this + offsets.DT_BaseAttributableItem.m_nFallbackPaintKit);
    }
    
    int* GetFallbackSeed()
    {
        return (int*)((uintptr_t)this + offsets.DT_BaseAttributableItem.m_nFallbackSeed);
    }
    
    float* GetFallbackWear()
    {
        return (float*)((uintptr_t)this + offsets.DT_BaseAttributableItem.m_flFallbackWear);
    }
    
    int* GetFallbackStatTrak()
    {
        return (int*)((uintptr_t)this + offsets.DT_BaseAttributableItem.m_nFallbackStatTrak);
    }
    
    int* GetAccountID()
    {
        return (int*)((uintptr_t)this + offsets.DT_BaseAttributableItem.m_iAccountID);
    }
    
    bool IsPistol()
    {
        switch (*this->GetItemDefinitionIndex())
        {
            case weapon_deagle:
            case weapon_elite:
            case weapon_fiveseven:
            case weapon_glock:
            case weapon_tec9:
            case weapon_hkp2000:
            case weapon_usp_silencer:
            case weapon_p250:
            case weapon_cz75a:
            case weapon_revolver:
                return true;
            default:
                return false;
        }
    }
    
    bool IsAutomatic()
    {
        switch (*this->GetItemDefinitionIndex())
        {
            case weapon_ak47:
            case weapon_aug:
            case weapon_famas:
            case weapon_galilar:
            case weapon_m249:
            case weapon_m4a1:
            case weapon_m4a1_silencer:
            case weapon_mac10:
            case weapon_p90:
            case weapon_mp5sd:
            case weapon_ump45:
            case weapon_bizon:
            case weapon_negev:
            case weapon_mp7:
            case weapon_mp9:
            case weapon_sg556:
                return true;
            default:
                return false;
        }
    }
    
    bool IsKnife()
    {
        switch (*this->GetItemDefinitionIndex())
        {
            case weapon_knife:
            case weapon_knife_t:
            case weapon_knife_gut:
            case weapon_knifegg:
            case weapon_knife_css:
            case weapon_knife_cord:
            case weapon_knife_flip:
            case weapon_knife_push:
            case weapon_knife_canis:
            case weapon_knife_ghost:
            case weapon_knife_ursus:
            case weapon_knife_outdoor:
            case weapon_knife_falchion:
            case weapon_knife_karambit:
            case weapon_knife_skeleton:
            case weapon_knife_stiletto:
            case weapon_knife_tactical:
            case weapon_knife_butterfly:
            case weapon_knife_widowmaker:
            case weapon_knife_m9_bayonet:
            case weapon_knife_survival_bowie:
            case weapon_knife_gypsy_jackknife:
                return true;
            default:
                return false;
        }
    }
    
    bool IsGrenade()
    {
        switch (*this->GetItemDefinitionIndex())
        {
            case weapon_smokegrenade:
            case weapon_hegrenade:
            case weapon_incgrenade:
            case weapon_flashbang:
            case weapon_molotov:
            case weapon_decoy:
                return true;
            default:
                return false;
        }
    }
    
    bool IsBomb()
    {
        return *this->GetItemDefinitionIndex() == weapon_c4;
    }
    
    bool CanScope()
    {
        switch (*this->GetItemDefinitionIndex())
        {
            case weapon_aug:
            case weapon_awp:
            case weapon_g3sg1:
            case weapon_scar20:
            case weapon_sg556:
            case weapon_ssg08:
                return true;
            default:
                return false;
        }
    }
};

class C_BaseViewModel: public C_BaseEntity {
public:
    int GetWeapon()
    {
        return *(int*)((uintptr_t)this + offsets.DT_BaseViewModel.m_hWeapon);
    }
    
    int GetOwner()
    {
        return *(int*)((uintptr_t)this + offsets.DT_BaseViewModel.m_hOwner);
    }
};

class C_BaseCombatWeapon: public C_BaseAttributableItem
{
public:
    int GetOwner()
    {
        return *(int*)((uintptr_t)this + offsets.DT_BaseCombatWeapon.m_hOwner);
    }
    
    unsigned int GetAmmo()
    {
        return *(unsigned int*)((uintptr_t)this + offsets.DT_BaseCombatWeapon.m_iClip1);
    }
    
    bool CanShoot(){
        return GetAmmo() > 0;
    }
    
    float GetPostponeFireReadyTime()
    {
        return *(float*)((uintptr_t)this + offsets.DT_BaseCombatWeapon.m_flPostponeFireReadyTime);
    }
    
    float GetNextPrimaryAttack()
    {
        return *(float*)((uintptr_t)this + offsets.DT_BaseCombatWeapon.m_flNextPrimaryAttack);
    }
    
    bool GetInReload()
    {
        return *(bool*)((uintptr_t)this + offsets.DT_BaseCombatWeapon.m_bInReload);
    }
    
    short EntityId() {
        return (short)((uintptr_t)this + offsets.DT_BaseAttributableItem.m_iItemDefinitionIndex);

    }
    
    float GetAccuracyPenalty()
    {
        return *(float*)((uintptr_t)this + offsets.DT_WeaponCSBase.m_fAccuracyPenalty);
    }
    
    void UpdateAccuracyPenalty() {
        typedef void (*oUpdateAccuracyPenalty)(void*);
        return getvfunc<oUpdateAccuracyPenalty>(this, 554)(this);
    }
    
    float GetInaccuracy()
    {
        typedef float (* oGetInaccuracy)(void*);
        return getvfunc<oGetInaccuracy>(this, 551)(this);
    }

    float GetSpread()
    {
        typedef float (* oGetSpread)(void*);
        return getvfunc<oGetSpread>(this, 552)(this);
    }
    
    /*WeaponCSInfo_t* GetCSWpnData() {
        typedef WeaponCSInfo_t* (*oGetCSWpnData)(void*);
        return getvfunc<oGetCSWpnData>(this, 512)(this);
    }*/

};

class C_WeaponC4 : C_BaseCombatWeapon
{
public:
    bool GetStartedArming()
    {
        return *(bool*)((uintptr_t)this + offsets.DT_WeaponC4.m_bStartedArming);
    }
    
};

class C_Chicken : C_BaseEntity
{
public:
    bool* GetShouldGlow()
    {
        return (bool*)((uintptr_t)this + offsets.DT_DynamicProp.m_bShouldGlow);
    }
};

class CSPlayerResource : public C_BasePlayer
{
public:
    
    int GetPlayerC4() {
        return *(int*)((uintptr_t)this + offsets.DT_CSPlayerResource.m_iPlayerC4);
    }

    bool HasC4(int index) {
        return *(int*)((uintptr_t)this + offsets.DT_CSPlayerResource.m_iPlayerC4) == index;
    }

    int GetMVPs(int index) {
        return *(int*)((uintptr_t)this + offsets.DT_CSPlayerResource.m_iMVPs + (index * 0x4));
    }

    int GetArmor(int index) {
        return *(int*)((uintptr_t)this + offsets.DT_CSPlayerResource.m_iArmor + (index * 0x4));
    }

    int GetScore(int index) {
        return *(int*)((uintptr_t)this + offsets.DT_CSPlayerResource.m_iScore + (index * 0x4));
    }

    int* GetCompetitiveRanking(int index) {
        return (int*)((uintptr_t)this + offsets.DT_CSPlayerResource.m_iCompetitiveRanking + (index * 0x4));
    }

    int* GetCompetitiveWins(int index) {
        return (int*)((uintptr_t)this + offsets.DT_CSPlayerResource.m_iCompetitiveWins + (index * 0x4));
    }

    /*Color GetCompTeammateColor(int index) {
        ColorRGBA temporaryColor = GetFieldValue<ColorRGBA>(Offsets::DT_CSPlayerResource::m_iCompTeammateColor + (index * 0x4));
        return Color(
            (int)((float)temporaryColor.RGBA[0] * 255.0f),
            (int)((float)temporaryColor.RGBA[1] * 255.0f),
            (int)((float)temporaryColor.RGBA[2] * 255.0f),
            (int)((float)temporaryColor.RGBA[3] * 255.0f)
        );
    }

    const char* GetClan(int index) {
        return GetFieldValue<const char*>(Offsets::DT_CSPlayerResource::m_szClan + (index * 0x10));
    }

    int* GetActiveCoinRank(int index) {
        return GetFieldPointer<int>(Offsets::DT_CSPlayerResource::m_nActiveCoinRank + (index * 0x4));
    }

    int* GetMusicID(int index) {
        return GetFieldPointer<int>(Offsets::DT_CSPlayerResource::m_nMusicID + (index * 0x4));
    }

    int* GetPersonaDataPublicCommendsLeader(int index) {
        return GetFieldPointer<int>(Offsets::DT_CSPlayerResource::m_nPersonaDataPublicCommendsLeader + (index * 0x4));
    }

    int* GetPersonaDataPublicCommendsTeacher(int index) {
        return GetFieldPointer<int>(Offsets::DT_CSPlayerResource::m_nPersonaDataPublicCommendsTeacher + (index * 0x4));
    }

    int* GetPersonaDataPublicCommendsFriendly(int index) {
        return GetFieldPointer<int>(Offsets::DT_CSPlayerResource::m_nPersonaDataPublicCommendsFriendly + (index * 0x4));
    }
     */
    Vector GetBombsiteCenterA() {
        return *(Vector*)((uintptr_t)this + offsets.DT_CSPlayerResource.m_bombsiteCenterA);
    }

    Vector GetBombsiteCenterB() {
        return *(Vector*)((uintptr_t)this + offsets.DT_CSPlayerResource.m_bombsiteCenterB);
    }

    
};
