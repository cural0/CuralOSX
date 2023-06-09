//
//  IGameMovement.h
//  GOSX Pro
//
//  Created by Andre Kalisch on 04.04.17.
//  Copyright © 2017 Andre Kalisch. All rights reserved.
//

#ifndef IGameMovement_h
#define IGameMovement_h

class CMoveData
{
public:
    bool            m_bFirstRunOfFunctions : 1;
    bool            m_bGameCodeMovedPlayer : 1;
    int             m_nPlayerHandle;    // edict index on server, client entity handle on client
    int             m_nImpulseCommand;    // Impulse command issued.
    Vector          m_vecViewAngles;    // Command view angles (local space)
    Vector          m_vecAbsViewAngles;    // Command view angles (world space)
    int             m_nButtons;            // Attack buttons.
    int             m_nOldButtons;        // From host_client->oldbuttons;
    float           m_flForwardMove;
    float           m_flSideMove;
    float           m_flUpMove;
    float           m_flMaxSpeed;
    float           m_flClientMaxSpeed;
    // Variables from the player edict (sv_player) or entvars on the client.
    // These are copied in here before calling and copied out after calling.
    Vector          m_vecVelocity;        // edict::velocity        // Current movement direction.
    Vector          m_vecAngles;        // edict::angles
    Vector          m_vecOldAngles;
    // Output only
    float           m_outStepHeight;    // how much you climbed this move
    Vector          m_outWishVel;        // This is where you tried
    Vector          m_outJumpVel;        // This is your jump velocity
    // Movement constraints    (radius 0 means no constraint)
    Vector          m_vecConstraintCenter;
    float           m_flConstraintRadius;
    float           m_flConstraintWidth;
    float           m_flConstraintSpeedFactor;
    float           m_flUnknown[5];
    Vector          m_vecAbsOrigin;        // edict::origin
};


class IGameMovement
{
public:
    void ProcessMovement(C_BaseEntity* player, CMoveData* move) {
        typedef void (* oProcessMovement)(void*, C_BaseEntity*, CMoveData*);
        return getvfunc<oProcessMovement>(this, 2)(this, player, move);
    }

    void StartTrackPredictionErrors(C_BaseEntity* player) {
        typedef void (* oStartTrackPredictionErrors)(void*, C_BaseEntity*);
        return getvfunc<oStartTrackPredictionErrors>(this, 4)(this, player);
    }

    void FinishTrackPredictionErrors(C_BaseEntity* player) {
        typedef void (* oFinishTrackPredictionErrors)(void*, C_BaseEntity*);
        return getvfunc<oFinishTrackPredictionErrors>(this, 5)(this, player);
    }
};


#endif /* IGameMovement_h */
