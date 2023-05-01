//
//  weapons.cpp
//  GO:SX v3
//
//  Created by Andre Kalisch on 25.01.17.
//  Copyright © 2017 Andre Kalisch. All rights reserved.
//

#include "weapons.h"


bool CWeaponManager::IsDelayedWeapon(int weaponID) {
    return isPistol(weaponID) ||
           isShotgun(weaponID) ||
           weaponID == en_item_definition_index::weapon_ssg08 ||
           weaponID == en_item_definition_index::weapon_awp;
}

int CWeaponManager::getDelay(int weaponID) {
    if (isPistol(weaponID) && weaponID != en_item_definition_index::weapon_tec9) {
        return 70;
    }
    
    if (isShotgun(weaponID)) {
        return 30;
    }
    
    if (isSniper(weaponID)) {
        return 100;
    }
    
    return 3;
}

bool CWeaponManager::isKnife(int weaponID) {
    if (
       weaponID == en_item_definition_index::weapon_knife ||
       weaponID == en_item_definition_index::weapon_knife_t ||
       weaponID == en_item_definition_index::weapon_bayonet ||
       weaponID == en_item_definition_index::weapon_knife_flip ||
       weaponID == en_item_definition_index::weapon_knife_gut ||
       weaponID == en_item_definition_index::weapon_knife_karambit ||
       weaponID == en_item_definition_index::weapon_knife_m9_bayonet ||
       weaponID == en_item_definition_index::weapon_knife_tactical ||
       weaponID == en_item_definition_index::weapon_knife_falchion ||
       weaponID == en_item_definition_index::weapon_knife_survival_bowie ||
       weaponID == en_item_definition_index::weapon_knife_butterfly ||
       weaponID == en_item_definition_index::weapon_knife_push
    ) {
        return true;
    }
    
    return false;
}

bool CWeaponManager::IsValidWeapon(int weaponID) {
    return (!isGrenade(weaponID) && !isKnife(weaponID) && !isC4(weaponID));
}

bool CWeaponManager::isGrenade(int weaponID) {
    if (
       weaponID == en_item_definition_index::weapon_flashbang ||
       weaponID == en_item_definition_index::weapon_hegrenade ||
       weaponID == en_item_definition_index::weapon_smokegrenade ||
       weaponID == en_item_definition_index::weapon_molotov ||
       weaponID == en_item_definition_index::weapon_decoy ||
       weaponID == en_item_definition_index::weapon_incgrenade
    ) {
        return true;
    }
    
    return false;
}

bool CWeaponManager::isC4(int weaponID) {
    if (weaponID == en_item_definition_index::weapon_c4) {
        return true;
    }
    
    return false;
}

bool CWeaponManager::isPistol(int weaponID) {
    if (
       weaponID == en_item_definition_index::weapon_deagle ||
       weaponID == en_item_definition_index::weapon_elite ||
       weaponID == en_item_definition_index::weapon_fiveseven ||
       weaponID == en_item_definition_index::weapon_glock ||
       weaponID == en_item_definition_index::weapon_tec9 ||
       weaponID == en_item_definition_index::weapon_taser ||
       weaponID == en_item_definition_index::weapon_hkp2000 ||
       weaponID == en_item_definition_index::weapon_p250 ||
       weaponID == en_item_definition_index::weapon_usp_silencer ||
       weaponID == en_item_definition_index::weapon_cz75a ||
       weaponID == en_item_definition_index::weapon_revolver
    ) {
        return true;
    }
    
    return false;
}

bool CWeaponManager::isShotgun(int weaponID) {
    if(
       weaponID == en_item_definition_index::weapon_xm1014 ||
       weaponID == en_item_definition_index::weapon_mag7 ||
       weaponID == en_item_definition_index::weapon_nova ||
       weaponID == en_item_definition_index::weapon_sawedoff
    ) {
        return true;
    }
    
    return false;
}


bool CWeaponManager::isSniper(int weaponID) {
    if (
       weaponID == en_item_definition_index::weapon_g3sg1 ||
       weaponID == en_item_definition_index::weapon_scar20 ||
       weaponID == en_item_definition_index::weapon_ssg08 ||
       weaponID == en_item_definition_index::weapon_awp
    ) {
        return true;
    }
    
    return false;
}

bool CWeaponManager::needsSmooth(int weaponID) {
    bool ret = true;
    if (isSniper(weaponID)) {
        ret = false;
    }
    
    if (isShotgun(weaponID)) {
        ret = false;
    }
    
    return ret;
}

bool CWeaponManager::isRCSWeapon(int weaponID) {
    bool ret = true;

    if (isSniper(weaponID)) {
        ret = false;
    }
    
    if (isPistol(weaponID) && weaponID != en_item_definition_index::weapon_cz75a) {
        ret = false;
    }
    
    if (isShotgun(weaponID)) {
        ret = false;
    }
    
    return ret;
}

bool CWeaponManager::IsScopeWeapon(int weaponID) {
    if (
        weaponID == en_item_definition_index::weapon_aug ||
        weaponID == en_item_definition_index::weapon_ssg08 ||
        weaponID == en_item_definition_index::weapon_sg556 ||
        weaponID == en_item_definition_index::weapon_g3sg1 ||
        weaponID == en_item_definition_index::weapon_scar20 ||
        weaponID == en_item_definition_index::weapon_awp
    ) {
        return true;
    }

    return false;
}

bool CWeaponManager::isNonAimWeapon(int weaponID) {
    if (
       weaponID == en_item_definition_index::weapon_scar20 ||
       weaponID == en_item_definition_index::weapon_g3sg1
    ) {
        return true;
    }
    
    return false;
}

std::string CWeaponManager::GetWeaponIcon(int weaponID) {
    switch (weaponID) {
        case weapon_knife_survival_bowie:
            return ICON_CSGO_KNIFE_BOWIE;
        case weapon_knife_butterfly:
            return ICON_CSGO_KNIFE_BUTTERFLY;
        case weapon_knife_ursus:
            return ICON_CSGO_KNIFE_URSUS;
        case weapon_knife_gypsy_jackknife:
            return ICON_CSGO_KNIFE_NAVAJA;
        case weapon_knife_stiletto:
            return ICON_CSGO_KNIFE_STILETTO;
        case weapon_knife_widowmaker:
            return ICON_CSGO_KNIFE_TALON;
        case weapon_knife:
            return ICON_CSGO_KNIFE_CT;
        case weapon_knife_falchion:
            return ICON_CSGO_KNIFE_FALCHION;
        case weapon_knife_flip:
            return ICON_CSGO_KNIFE_FLIP;
        case weapon_knife_gut:
            return ICON_CSGO_KNIFE_GUT;
        case weapon_knife_karambit:
            return ICON_CSGO_KNIFE_KARAMBIT;
        case weapon_knife_m9_bayonet:
            return ICON_CSGO_KNIFE_M9BAYONET;
        case weapon_knife_t:
            return ICON_CSGO_KNIFE_T;
        case weapon_knife_tactical:
            return ICON_CSGO_KNIFE_HUNTSMAN;
        case weapon_knife_push:
            return ICON_CSGO_KNIFE_SHADOWDAGGERS;
        case weapon_deagle:
            return ICON_CSGO_DEAGLE;
        case weapon_elite:
            return ICON_CSGO_ELITE;
        case weapon_fiveseven:
            return ICON_CSGO_FIVESEVEN;
        case weapon_glock:
            return ICON_CSGO_GLOCK;
        case weapon_hkp2000:
            return ICON_CSGO_P2000;
        case weapon_mp5sd:
            return ICON_CSGO_MP5SD;
        case weapon_p250:
            return ICON_CSGO_P250;
        case weapon_usp_silencer:
            return ICON_CSGO_USPS;
        case weapon_tec9:
            return ICON_CSGO_TEC9;
        case weapon_revolver:
            return ICON_CSGO_REVOLVER;
        case weapon_mac10:
            return ICON_CSGO_MAC10;
        case weapon_ump45:
            return ICON_CSGO_UMP45;
        case weapon_bizon:
            return ICON_CSGO_PPBIZON;
        case weapon_mp7:
            return ICON_CSGO_MP7;
        case weapon_mp9:
            return ICON_CSGO_MP9;
        case weapon_p90:
            return ICON_CSGO_P90;
        case weapon_galilar:
            return ICON_CSGO_GALIL;
        case weapon_famas:
            return ICON_CSGO_FAMAS;
        case weapon_m4a1_silencer:
            return ICON_CSGO_M4A1S;
        case weapon_m4a1:
            return ICON_CSGO_M4A4;
        case weapon_aug:
            return ICON_CSGO_AUG;
        case weapon_sg556:
            return ICON_CSGO_SG558;
        case weapon_ak47:
            return ICON_CSGO_AK47;
        case weapon_g3sg1:
            return ICON_CSGO_G3SG1;
        case weapon_scar20:
            return ICON_CSGO_SCAR20;
        case weapon_awp:
            return ICON_CSGO_AWP;
        case weapon_ssg08:
            return ICON_CSGO_SSG08;
        case weapon_xm1014:
            return ICON_CSGO_XM1014;
        case weapon_sawedoff:
            return ICON_CSGO_SAWEDOFF;
        case weapon_mag7:
            return ICON_CSGO_MAG7;
        case weapon_nova:
            return ICON_CSGO_NOVA;
        case weapon_negev:
            return ICON_CSGO_NEGEV;
        case weapon_m249:
            return ICON_CSGO_M249;
        case weapon_taser:
            return ICON_CSGO_ZEUS;
        case weapon_flashbang:
            return ICON_CSGO_FLASH;
        case weapon_hegrenade:
            return ICON_CSGO_HE;
        case weapon_smokegrenade:
            return ICON_CSGO_SMOKE;
        case weapon_molotov:
            return ICON_CSGO_MOLOTOV;
        case weapon_decoy:
            return ICON_CSGO_DECOY;
        case weapon_incgrenade:
            return ICON_CSGO_INC;
        case weapon_c4:
            return ICON_CSGO_C4;
        case weapon_cz75a:
            return ICON_CSGO_CZ75A;
        default:
            return " ";
    }
}

