//
//  ValveSDK.h
//  ButOSX
//
//  Created by Can on 23.01.2021.
//  Copyright © 2021 Lyceion. All rights reserved.
//

#ifndef ValveSDK_h
#define ValveSDK_h

#include <stdio.h>
#include <cstdint>
#include <unordered_map>
#include <sys/mman.h>
#include <dlfcn.h>
#include <fstream>
#include <mach/mach_traps.h>
#include <mach/mach_init.h>
#include <mach/mach_error.h>
#include <mach/mach.h>
#include <mach-o/dyld_images.h>
#include <mach-o/loader.h>
#include <libproc.h>
#include <sys/stat.h>
#include <cstddef>
#include <codecvt>
#include <cstring>
#include <sstream>
#include <vector>
#include <string>
#include <strings.h>
#include <ios>
#include <iomanip>
#include <pwd.h>
#include <zconf.h>
#include <fstream>
#include <cstdio>





#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_opengl2.h"
#include "imgui_impl_sdl.h"

using namespace std;
// SDK
#include "Vector.h"
#include "Others.h"
#include "CommonFunctions.h"
#include "Color.h"
#include "Interfaces/IBaseClientDLL.h"
#include "Interfaces/ISurface.h"
#include "Interfaces/IPanel.h"
#include "Interfaces/ICvar.h"
#include "Interfaces/CGlobalVarsBase.h"
#include "Interfaces/IGameEvent.h"
#include "Offsets.hpp"
#include "Interfaces/IClientEntityList.h"
#include "Interfaces/IVDebugOverlay.h"
#include "Interfaces/IEngineClient.h"
#include "Interfaces/IEngineTrace.h"
#include "Classes/IMaterial.h"
#include "Interfaces/IVModuleInfo.h"
#include "Interfaces/IVModelRender.h"
#include "Interfaces/IVMaterialSystem.h"
#include "Interfaces/IInputSystem.h"
#include "Classes/Classes.h"
#include "VMT.h"
#include "Interfaces/IPhysicsSurfaceProps.h"
#include "Interfaces/IMoveHelper.h"
#include "Interfaces/IGameMovement.h"
#include "Interfaces/IPrediction.h"



// Interfaces
#include "Interfaces.hpp"

// Utils
#include "NetVarManager.hpp"




#endif /* ValveSDK_h */
