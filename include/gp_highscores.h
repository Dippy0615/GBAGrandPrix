#ifndef GP_HIGHSCORES_H
#define GP_HIGHSCORES_H

#include "bn_blending_actions.h"
#include "bn_optional.h"

#include "gp_scene.h"

namespace gp
{
    class Highscores
    {
        private:
        public:
            Highscores();
            gp::Scene execute();
            bn::optional<bn::blending_fade_alpha_to_action> _fade_action;
            void fade_out();
    };
}

#endif