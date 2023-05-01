
#include "utils.h"

bool IsVisible(C_BasePlayer* LocalPlayer, C_BasePlayer* pPlayer, Vector position, float fov, bool smokeCheck) {
    if (!LocalPlayer || !pPlayer) {
        return false;
    }

    if (pPlayer == LocalPlayer) {
        return false;
    }

    Vector LocalPlayerEyes = LocalPlayer->GetEyePosition();

    QAngle viewAngles;
    pEngine->GetViewAngles(viewAngles);
    viewAngles -= LocalPlayer->GetAimPunchAngle();

    if (Math::GetFov(viewAngles, LocalPlayerEyes, position) > fov) {
        return false;
    }

    Ray_t ray;
    trace_t tr;
    ray.Init(LocalPlayerEyes, position);
    CTraceFilter traceFilter;
    traceFilter.pSkip = LocalPlayer;

    pEngineTrace->TraceRay(ray, MASK_SHOT, &traceFilter, &tr);

    if (tr.allsolid || tr.startsolid) {
        return false;
    }

//    if (smokeCheck && LineGoesThroughSmoke(LocalPlayerEyes, position)) {
//        return false;
//    }

    return tr.m_pEnt == pPlayer;
}

long GetTimeStamp() {
    timeval time;
    gettimeofday(&time, NULL);
    long millis = (time.tv_sec * 1000) + (time.tv_usec / 1000);

    return millis;
}


void AngleVectors(const Vector & angles, Vector * forward)
{
    Assert(s_bMathlibInitialized);
    Assert(forward);
    
    float	sp, sy, cp, cy;
    
    sy = sin(DEG2RAD(angles[1]));
    cy = cos(DEG2RAD(angles[1]));
    
    sp = sin(DEG2RAD(angles[0]));
    cp = cos(DEG2RAD(angles[0]));
    
    forward->x = cp*cy;
    forward->y = cp*sy;
    forward->z = -sp;
}

void AngleVectors2(const Vector & angles, Vector * forward, Vector * right, Vector * up)
{
    float angle;
    static float sr, sp, sy, cr, cp, cy;
    
    angle = angles[1] * (M_PI * 2 / 360);
    sy = sin(angle);
    cy = cos(angle);
    
    angle = angles[0] * (M_PI * 2 / 360);
    sp = sin(angle);
    cp = cos(angle);
    
    angle = angles[2] * (M_PI * 2 / 360);
    sr = sin(angle);
    cr = cos(angle);
    
    if (forward)
    {
        forward->x = cp*cy;
        forward->y = cp*sy;
        forward->z = -sp;
    }
    if (right)
    {
        right->x = (-1 * sr*sp*cy + -1 * cr*-sy);
        right->y = (-1 * sr*sp*sy + -1 * cr*cy);
        right->z = -1 * sr*cp;
    }
    if (up)
    {
        up->x = (cr*sp*cy + -sr*-sy);
        up->y = (cr*sp*sy + -sr*cy);
        up->z = cr*cp;
    }
}

void NormalizeAngles(Vector& angle)
{
    while (angle.x > 89.0f)
        angle.x -= 180.f;
    
    while (angle.x < -89.0f)
        angle.x += 180.f;
    
    while (angle.y > 180.f)
        angle.y -= 360.f;
    
    while (angle.y < -180.f)
        angle.y += 360.f;
}
void SinCos(float a, float* s, float*c)
{
    *s = sin(a);
    *c = cos(a);
}

void ClampAngles(Vector& angle)
{
    if (angle.y > 180.0f)
        angle.y = 180.0f;
    else if (angle.y < -180.0f)
        angle.y = -180.0f;
    
    if (angle.x > 89.0f)
        angle.x = 89.0f;
    else if (angle.x < -89.0f)
        angle.x = -89.0f;
    
    angle.z = 0;
}

float ClampYaw(float val)
{
    
    while(val > 180)
    {
        val -= 360;
    }
    
    while(val < -180)
    {
        val += 360;
    }
    
    return val;
}

void VectorAngles(const Vector& forward, Vector &angles)
{
    float	tmp, yaw, pitch;
    
    if (forward[1] == 0 && forward[0] == 0)
    {
        yaw = 0;
        if (forward[2] > 0)
            pitch = 270;
        else
            pitch = 90;
    }
    else
    {
        yaw = (atan2(forward[1], forward[0]) * 180 / M_PI);
        if (yaw < 0)
            yaw += 360;
        
        tmp = sqrt (forward[0]*forward[0] + forward[1]*forward[1]);
        pitch = (atan2(-forward[2], tmp) * 180 / M_PI);
        if (pitch < 0)
            pitch += 360;
    }
    
    angles[0] = pitch;
    angles[1] = yaw;
    angles[2] = 0;
}


void CorrectMovement(Vector vOldAngles, CUserCmd* pCmd, float fOldForward, float fOldSidemove)
{
    // side/forward move correction
    float deltaView;
    float f1;
    float f2;
    
    if (vOldAngles.y < 0.f)
        f1 = 360.0f + vOldAngles.y;
    else
        f1 = vOldAngles.y;
    
    if (pCmd->viewangles.y < 0.0f)
        f2 = 360.0f + pCmd->viewangles.y;
    else
        f2 = pCmd->viewangles.y;
    
    if (f2 < f1)
        deltaView = abs(f2 - f1);
    else
        deltaView = 360.0f - abs(f1 - f2);
    
    deltaView = 360.0f - deltaView;
    
    pCmd->forwardmove = cos(DEG2RAD(deltaView)) * fOldForward + cos(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
    pCmd->sidemove = sin(DEG2RAD(deltaView)) * fOldForward + sin(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
}



void Normalize(Vector & vIn, Vector & vOut)
{
    float flLen = vIn.Length();
    if (flLen == 0)
    {
        vOut.Init(0, 0, 1);
        return;
    }
    flLen = 1 / flLen;
    vOut.Init(vIn.x * flLen, vIn.y * flLen, vIn.z * flLen);
}

void NormalizeVector(Vector& vecIn) {
    for (int axis = 0; axis < 3; ++axis)
    {
        while (vecIn[axis] > 180.f)
            vecIn[axis] -= 360.f;
        
        while (vecIn[axis] < -180.f)
            vecIn[axis] += 360.f;
        
    }
    
    vecIn[2] = 0.f;
}

void FixMovement(Vector& oang, CUserCmd* pCmd)
{
    Vector vMove(pCmd->forwardmove, pCmd->sidemove, pCmd->upmove);
    float flSpeed = sqrt(vMove.x * vMove.x + vMove.y * vMove.y), flYaw;
    Vector vMove2;
    VectorAngles(vMove, vMove2);
    
    flYaw = DEG2RAD(pCmd->viewangles.y - oang.y + vMove2.y);
    pCmd->forwardmove = cos(flYaw) * flSpeed;
    pCmd->sidemove = sin(flYaw) * flSpeed;
    
    if (pCmd->viewangles.x < -90.f || pCmd->viewangles.x > 90.f)
        pCmd->forwardmove = -pCmd->forwardmove;
}

void GetEpochTime()
{
    auto duration = std::chrono::system_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}



Vector GetHitboxPosition(C_BaseEntity* pEntity, int Hitbox)
{
    matrix3x4_t matrix[MAXSTUDIOBONES];
    
    if (!pEntity->SetupBones(matrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, pGlobalVars->curtime))
        return Vector(0, 0, 0);
    
    studiohdr_t* hdr = pModelInfo->GetStudioModel(pEntity->GetModel());
    mstudiohitboxset_t* set = hdr->pHitboxSet(0);
    mstudiobbox_t* hitbox = set->pHitbox(Hitbox);
    
    if (!hitbox)
        return Vector(0, 0, 0);
    
    Vector vMin, vMax, vCenter, sCenter;
    Math::VectorTransform(hitbox->bbmin, matrix[hitbox->bone], vMin);
    Math::VectorTransform(hitbox->bbmax, matrix[hitbox->bone], vMax);
    vCenter = (vMin + vMax) *0.5f;
    
    return vCenter;
}

Vector GetHitboxPosition2(C_BaseEntity* pEntity, int Hitbox, Vector vMin, Vector vMax)
{
    matrix3x4_t matrix[MAXSTUDIOBONES];
    
    if (!pEntity->SetupBones(matrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, pGlobalVars->curtime))
        return Vector(0, 0, 0);
    
    studiohdr_t* hdr = pModelInfo->GetStudioModel(pEntity->GetModel());
    mstudiohitboxset_t* set = hdr->pHitboxSet(0);
    mstudiobbox_t* hitbox = set->pHitbox(Hitbox);
    
    if (!hitbox)
        return Vector(0, 0, 0);
    
    Vector vCenter;
    Math::VectorTransform(hitbox->bbmin, matrix[hitbox->bone], vMin);
    Math::VectorTransform(hitbox->bbmax, matrix[hitbox->bone], vMax);
    vCenter = (vMin + vMax) *0.5f;
    
    return vCenter;
}

WeaponCSInfo_t* GetCSWpnData(C_BaseCombatWeapon* pWeapon){
    
    typedef WeaponCSInfo_t* (*oGetCSWpnData)(void*);
    return getvfunc<oGetCSWpnData>(pWeapon, 512)(pWeapon);
}

C_BaseCombatWeapon* UpdateAccuracyPenalty(C_BaseCombatWeapon* pWeapon) {
    typedef void(*oUpdateAccuracyPenalty)(void*);
    getvfunc<oUpdateAccuracyPenalty>(pWeapon, 536)(pWeapon);
    
    return pWeapon;
}

bool IsVisible(C_BasePlayer* LocalPlayer, C_BasePlayer* pPlayer, int hitbox, float fov, bool smokeCheck) {
    Vector vMin, vMax;
    return IsVisible(LocalPlayer, pPlayer, Math::ExtrapolateTick(GetHitboxPosition2(pPlayer, hitbox, vMin, vMax), pPlayer->GetVelocity()), fov, smokeCheck);
    //return IsVisible(LocalPlayer, pPlayer, pPlayer->GetPredictedPosition(hitbox, vMin, vMax), fov, smokeCheck);
}

bool IsVisible(C_BaseEntity* local, C_BaseEntity* entity)
{
    Ray_t ray;
    trace_t trace;
    CTraceFilter filter;
    filter.pSkip = local;
    
    ray.Init(local->GetEyePosition(), entity->GetEyePosition());
    pEngineTrace->TraceRay(ray, 0x4600400B, &filter, &trace);
    return (trace.m_pEnt == entity || trace.fraction > 0.99f);
}

Vector ExtrapolateTick( Vector p0, Vector v0 )
{
    return p0 + ( v0 * pGlobalVars->interval_per_tick );
}

Vector GetPredictedPosition(int Hitbox, C_BaseEntity* pEntity)
{
    //Vector v0 = GetHitboxPosition(pEntity, Hitbox);
    return ExtrapolateTick(GetHitboxPosition(pEntity, Hitbox), pEntity->GetVelocity());
}

string GetWeaponName(C_BaseCombatWeapon* pWeapon) {
    if(!pWeapon) { return "nullptr"; }
    short ID = *pWeapon->GetItemDefinitionIndex();
    
    switch (ID) {
            /*pistols*/
        case 4:
            return "Glock";
        case 2:
            return "Dualies";
        case 36:
            return "P250";
        case 30:
            return "Tec 9";
        case 1:
            return "Deagle";
        case 32:
            return "P2000";
        case 3:
            return "Five Seven";
        case 64:
            return "Revolver";
        case 63:
            return "CZ75";
        case 61:
            return "USP";
            /*heavy*/
        case 35:
            return "Nova";
        case 25:
            return "XM1014";
        case 29:
            return "Sawed Off";
        case 14:
            return "M249";
        case 28:
            return "Negev";
        case 27:
            return "Mag 7";
            /*smgs*/
        case 17:
            return "Mac 10";
        case 33:
            return "MP7";
        case 24:
            return "UMP45";
        case 19:
            return "P90";
        case 26:
            return "Bizon";
        case 34:
            return "MP9";
            /*C4*/
        case 49:
            return "C4";
            /*rifles*/
        case 10:
            return "Famas";
        case 16:
            return "M4A1";
        case 40:
            return "Scout";
        case 8:
            return "AUG";
        case 9:
            return "AWP";
        case 38:
            return "Scar 20";
        case 13:
            return "Galil";
        case 7:
            return "AK47";
        case 39:
            return "SG553";
        case 11:
            return "G3SG1";
        case 60:
            return "M4A1-S";
            /*grenades*/
        case 46:
        case 48:
            return "Molotov";
        case 44:
            return "Grenade";
        case 43:
            return "Flash";
        case 45:
            return "Smoke";
        case 47:
            return "Decoy";
            /*other*/
        case 31:
            return "Zeus";
        default:
            return "Knife";
    }
}

void MoveFix(CUserCmd *cmd, Vector &realvec)
{
    Vector vMove(cmd->forwardmove, cmd->sidemove, cmd->upmove);
    float flSpeed = sqrt(vMove.x * vMove.x + vMove.y * vMove.y), flYaw;
    Vector vMove2;
    VectorAngles(vMove, vMove2);
    
    flYaw = DEG2RAD(cmd->viewangles.y - realvec.y + vMove2.y);
    cmd->forwardmove = cos(flYaw) * flSpeed;
    cmd->sidemove = sin(flYaw) * flSpeed;
    
    if (cmd->viewangles.x < -90.f || cmd->viewangles.x > 90.f)
        cmd->forwardmove = -cmd->forwardmove;
}

Vector CalcAngle(Vector src, Vector dst)
{
    Vector delta, angles;
    delta.x = (src.x - dst.x);
    delta.y = (src.y - dst.y);
    delta.z = (src.z - dst.z);
    
    double hyp = sqrt(delta.x * delta.x + delta.y * delta.y);
    
    angles.x = (float)(atanf(delta.z / hyp) * M_RADPI);
    angles.y = (float)(atanf(delta.y / delta.x) * M_RADPI);
    
    angles.z = 0.0f;
    if (delta.x >= 0.0)
    {
        angles.y += 180.0f;
    }
    return angles;
}

/*C_BaseCombatWeapon* GetActiveWeapon(C_BaseEntity* local)
{
    auto hWeapon = local->GetActiveWeapon();
    if (!hWeapon)
        return nullptr;
    
    return (C_BaseCombatWeapon*)pEntList->GetClientEntityFromHandle(hWeapon);
}*/

float GetFOV(Vector start, Vector src, Vector dst)
{
    Vector ang, aim;
    ang = CalcAngle(src, dst);
    AngleVectors(start, &aim);
    AngleVectors(ang, &ang);
    
    float mag_s = std::sqrt((aim[0] * aim[0]) + (aim[1] * aim[1]) + (aim[2] * aim[2]));
    float mag_d = mag_s;
    float u_dot_v = aim[0] * ang[0] + aim[1] * ang[1] + aim[2] * ang[2];
    float fov = std::acos(u_dot_v / (mag_s * mag_d)) * (180.f / M_PI_F);
    return fov;
}





void ClampMoves(float& flForward, float& flSide, float& flUp)
{
    if(flForward > 450) flForward = 450;
    if(flSide > 450) flSide = 450;
    if(flUp > 450) flUp = 450;
    if(flForward < -450) flForward = -450;
    if(flSide < -450) flSide = -450;
    if(flUp < -450) flUp = -450;
}

void ClampYaw(float& angle)
{
    while(angle > 180)
    {
        angle -= 360;
    }
    while(angle < -180)
    {
        angle += 360;
    }
}

float RandomFloat(float min, float max)
{
    return min + ((float)rand() / RAND_MAX) * (min - max);
}

/*
void DrawLine(int x, int y, int xx, int yy, Color color) {
    pSurface->DrawSetColor(color);
    pSurface->DrawLine(x, y, xx, yy);
    pSurface->DrawSetColor(255, 255, 255);
}


bool WorldToScreen(Vector& vFrom, Vector& vTo) {
    return (pOverlay->ScreenPosition(vFrom, vTo) != 1);
}


void DrawBox(int x, int y, int w, int h, Color color) {
    pSurface->DrawSetColor(color);
    pSurface->DrawOutlinedRect(x, y, x + w, y + h);
    pSurface->DrawSetColor(255, 255, 255);
}
*/
/*float distance_point_to_line(Vector Point, Vector LineOrigin, Vector Dir)
{
    auto PointDir = Point - LineOrigin;
    
    auto TempOffset = PointDir.Dot(Dir) / (Dir.x*Dir.x + Dir.y*Dir.y + Dir.z*Dir.z);
    if (TempOffset < 0.000001f)
        return FLT_MAX;
    
    auto PerpendicularPoint = LineOrigin + (Dir * TempOffset);
    
    return (Point - PerpendicularPoint).Length();
}*/


C_BaseEntity* GetBomb() {
    for (int i = pEngine->GetMaxClients(); i < pEntList->GetHighestEntityIndex(); i++) {
        C_BaseEntity* entity = (C_BaseEntity*)pEntList->GetClientEntity(i);
        if (!entity || entity->IsDormant()) {
            continue;
        }
        
        if (entity->GetClientClass()->m_ClassID == en_class_id::CC4) {
            return (C_BaseEntity*)entity;
        }
    }
    
    return nullptr;
}
