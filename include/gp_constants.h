#ifndef GP_CONSTANTS_H
#define GP_CONSTANTS_H

#include "bn_fixed.h"

namespace gp
{
    constexpr int CAR_MAX_SPEED = 30;
    constexpr int CAR_MAX_SPEED_MUD = 10;
    constexpr bn::fixed CAR_ACCEL = 0.035;
    constexpr bn::fixed CAR_ACCEL_MUD = 0.005;
    constexpr bn::fixed CAR_DEACCEL = 0.25;
    constexpr int CAR_BOOSTER_SPEED = 40;
    constexpr int CAR_HIT_TIME = 90;
    constexpr int CAR_STATE_NORMAL = 0;
    constexpr int CAR_STATE_HIT = 1;
    constexpr int CAR_STATE_HIT2 = 2;
    constexpr int TRACK_TURN_ADJUST = 40;
    constexpr int OBJ_MUDSLICK = 0;
    constexpr int OBJ_FINISHLINE = 1;
    constexpr int OBJ_ROADBLOCK = 2;
    constexpr int OBJ_COIN = 3;
    constexpr int OBJ_MUD = 4;
    constexpr int OBJ_MUD_BOTTOM = 5;
    constexpr int OBJ_LEAFPILE = 6;
    constexpr int OBJ_BOOSTERPAD = 7;
    constexpr int FINISHLINE_POSITION = 128;
    constexpr int SIGN_WARNING = 0;
    constexpr int BAR_X = 106;
    constexpr int BAR_Y = -24;
    constexpr int PLR_ICON_Y = 25;
    constexpr int MAX_LAPS = 3;
    constexpr int SEGMENT_OBJ_MAX = 20;
    constexpr int SEGMENTS_MAX = 9;
}

#endif