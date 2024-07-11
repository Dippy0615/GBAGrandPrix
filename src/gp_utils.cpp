#include "bn_fixed.h"

#include "gp_utils.h"

namespace gp
{
    bn::fixed lerp(bn::fixed a, bn::fixed b, bn::fixed t)
    {
        return a + (b - a) * t;
    }

    bn::fixed min(bn::fixed a, bn::fixed b)
    {
        return b > a ? b : a;
    }

    bn::fixed angle_wrap(bn::fixed angle)
    {
        if (angle<0) angle+=360;
        if (angle>360) angle-=360;
        return angle;
    }

    bool approx(bn::fixed a, bn::fixed min, bn::fixed max)
    {
        return a >= min && a <= max;
    }

    int sign(bn::fixed a)
    {
        if (a==0) return 0;
        return a>0 ? 1 : -1;
    }
}
