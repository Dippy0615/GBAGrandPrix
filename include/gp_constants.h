#ifndef GP_CONSTANTS_H
#define GP_CONSTANTS_H

#include "bn_fixed.h"

namespace gp
{
    constexpr int CAR_MAX_SPEED = 30;
    constexpr bn::fixed CAR_ACCEL = 0.05;
    constexpr bn::fixed CAR_DEACCEL = 0.25;
    constexpr int TRACK_TURN_ADJUST = 40;
    constexpr int OBJ_MUDSLICK = 0;
    constexpr int OBJ_FINISHLINE = 1;
    constexpr int FINISHLINE_POSITION = 128;
}

#endif