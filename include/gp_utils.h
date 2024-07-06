#include "bn_fixed.h"

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