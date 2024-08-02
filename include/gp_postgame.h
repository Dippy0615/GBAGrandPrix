#ifndef GP_POSTGAME_H
#define GP_POSTGAME_H

#include "bn_blending_actions.h"
#include "bn_optional.h"

#include "gp_scene.h"

namespace gp
{
    class Postgame
    {
        private:
        public:
            Postgame();
            gp::Scene execute();
            bn::optional<bn::blending_fade_alpha_to_action> _fade_action;
            void fade_out();
    };
}

#endif