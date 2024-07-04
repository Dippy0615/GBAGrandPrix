#ifndef GP_CONSTANTS_H
#define GP_CONSTANTS_H

#include "bn_fixed.h"

namespace gp
{
    constexpr int CAR_MAX_SPEED = 30;
    constexpr bn::fixed CAR_ACCEL = 0.05;
    constexpr bn::fixed CAR_DEACCEL = 0.25;
}

#endif