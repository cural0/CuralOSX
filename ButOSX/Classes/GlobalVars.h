//
//  GlobalVars.h
//  ButOSX
//
//  Created by Michał Kural on 12/04/2023.
//  Copyright © 2023 Lyceion. All rights reserved.
//

#ifndef GlobalVars_h
#define GlobalVars_h

namespace Glob {
    extern C_BaseCombatWeapon*                  LastActiveWeapon;
    extern bool*                                IsWeaponSwitched;
    extern bool                                 AimbotIsAiming;
    extern float*                               ZoomedFov;
    extern int*                                 SDLResW;
    extern int*                                 SDLResH;
#ifdef GOSX_THIRDPERSON
    extern Vector*                              ThirdpersonAngles;
#endif
#ifdef GOSX_RAGE_MODE
    extern bool*                                SendPacket;
    extern bool*                                PostProcessing;
#endif
#ifdef GOSX_AUTO_ACCEPT
    extern bool                                 IsEmitSoundHooked;
#endif
};


#endif /* GlobalVars_h */
