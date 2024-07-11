#ifndef GP_UTILS_H
#define GP_UTILS_H

#include "bn_fixed.h"

namespace gp
{
    bn::fixed lerp(bn::fixed a, bn::fixed b, bn::fixed t);
    bn::fixed min(bn::fixed a, bn::fixed b);
    bn::fixed angle_wrap(bn::fixed angle);
    bool approx(bn::fixed a, bn::fixed min, bn::fixed max);
    int sign(bn::fixed a);
}

#endif