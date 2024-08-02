#ifndef GP_INGAME_H
#define GP_INGAME_H

#include "bn_blending_actions.h"
#include "bn_optional.h"

#include "gp_scene.h"

namespace gp
{
    class Ingame
    {
        private:
        public:
            Ingame();
            gp::Scene execute(int level);
            bn::optional<bn::blending_fade_alpha_to_action> _fade_action;
            void fade_out();
    };
}

#endif