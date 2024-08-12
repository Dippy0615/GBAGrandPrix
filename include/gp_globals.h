#ifndef GP_GLOBALS_H
#define GP_GLOBALS_H

#include "gp_score.h"

#include "bn_array.h"
#include "bn_vector.h"

namespace gp
{
    extern int current_car;
    extern bn::array<bool, 3> cars;
    extern int coins;
    extern bn::vector<gp::Score, 4> scores;
    extern bool lost;
}

#endif