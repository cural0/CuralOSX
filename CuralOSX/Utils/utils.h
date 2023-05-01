#pragma once

#include "ValveSDK.h"

struct boxstruct { int x, y, w, h; };


extern bool IsVisible(C_BasePlayer* LocalPlayer, C_BasePlayer* pPlayer, Vector position, float fov = 180.f, bool smokeCheck = false);
extern bool IsVisible(C_BasePlayer* LocalPlayer, C_BasePlayer* pPlayer, int hitbox, float fov = 180.f, bool smokeCheck = false);
extern long GetTimeStamp();
extern void AngleVectors(const Vector & angles, Vector * forward);
extern void AngleVectors2(const Vector & angles, Vector * forward, Vector * right, Vector * up);
extern void VectorAngles(const Vector& forward, Vector &angles);
extern void FixMovement(Vector& oang, CUserCmd* cmd);
extern bool DrawPlayerBox(C_BaseEntity* pEntity, boxstruct& str);
extern void CorrectMovement(Vector vOldAngles, CUserCmd* pCmd, float fOldForward, float fOldSidemove);
extern void GetEpochTime();
extern Vector GetHitboxPosition(C_BaseEntity* pEntity, int Hitbox);
extern Vector GetHitboxPosition2(C_BaseEntity* pEntity, int Hitbox, Vector vMin, Vector vMax);
extern WeaponCSInfo_t* GetCSWpnData(C_BaseCombatWeapon* pWeapon);
extern C_BaseCombatWeapon* UpdateAccuracyPenalty(C_BaseCombatWeapon* pWeapon);
extern bool IsVisible(C_BaseEntity* local, C_BaseEntity* entity);
extern void Normalize(Vector & vIn, Vector & vOut);
extern void NormalizeVector(Vector & vecIn);
extern C_BaseCombatWeapon* GetActiveWeapon(C_BaseEntity* local);
extern void MoveFix(CUserCmd *cmd, Vector &realvec);
extern Vector CalcAngle(Vector src, Vector dst);
extern float GetFOV(Vector start, Vector src, Vector dst);
extern void ClampMoves(float& flForward, float& flSide, float& flUp);
extern void ClampYaw(float& angle);
extern string GetWeaponName(C_BaseCombatWeapon* pWeapon);
extern float distance_point_to_line(Vector Point, Vector LineOrigin, Vector Dir);
extern void SinCos(float a, float* s, float*c);
extern Vector ExtrapolateTick( Vector p0, Vector v0 );
extern Vector GetPredictedPosition(int Hitbox, C_BaseEntity* pEntity);
extern void DrawLine(int x, int y, int xx, int yy, Color color);
extern bool WorldToScreen(Vector& vFrom, Vector& vTo);
extern void DrawBox(int x, int y, int w, int h, Color color);
extern C_BaseEntity* GetBomb();


extern float RandomFloat(float min, float max);

