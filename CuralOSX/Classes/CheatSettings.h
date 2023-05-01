//
//  CheatSettings.h
//  ButOSX
//
//  Created by Can on 30.10.2020.
//  Copyright © 2020 Lyceion. All rights reserved.
//

#ifndef CheatSettings_h
#define CheatSettings_h
namespace CheatSettings {
    extern bool WaterMark;
    namespace ESP{
        extern bool enabled;
        extern bool box;
        extern bool name;
        extern bool health;
        extern bool skeleton;
        extern bool fovcircle;
    extern bool show_wallbang_indicator;
    }
namespace Radar{
extern int     size;
extern int     pos_x;
extern int     pos_y;
extern int     zoom;
extern int     style;

}
    namespace Backtracking{
        extern int backtrack_ticks;
        extern int backtrack_hitbox;
        extern int backtrack_visual_type;
        extern bool enabled;
        extern bool visibility_check;
        extern Color backtrack_color;
    }
    namespace Aim{
        extern bool teamcheck;
        extern float fov;
        extern int bone;
        extern float rcs_level;
        extern int trigger_bot_delay;
        extern float smooth_factor;
        extern bool dm_mode;
        extern bool rage;
        extern float fov_multiplier;
        extern bool smokecheck;
        extern bool full_legit;
        extern bool fov_enabled;
        extern int bone_mode;
        extern bool delayed_shot;
        extern bool smooth_salting;
        extern int antiaim_pitch;
        extern int antiaim_custom_pitch;
        extern int antiaim_yaw;
        extern int antiaim_custom_yaw;
        extern int fixed_bone;
        extern bool auto_shoot;
        extern bool auto_scope;
    
        
    }
    namespace Rage{
        extern float fov_multiplier;
        extern bool auto_shoot;
        extern bool auto_scope;
        extern bool autocrouch;
        extern bool auto_stop;
        extern bool hitchance;
        extern int hitchance_shots;
        extern float hitchance_percent;
        extern bool hit_scan;
        extern int fixed_bone;
        extern bool auto_wall;
        extern float autowall_min_damage;
        extern bool silent_aim;
        extern bool anti_aim;
    }
namespace Extras{
        extern bool auto_cock_revolver;
        extern bool aimbot_key_check;
}
        namespace Drawing{
        extern int bomb_timer;
        }
        extern bool NightMode;
};
#endif /* CheatSettings_h */
