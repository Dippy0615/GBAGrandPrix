#ifndef GP_CREDITS_H
#define GP_CREDITS_H

#include "bn_blending_actions.h"
#include "bn_optional.h"

#include "gp_scene.h"

namespace gp
{
    class Credits
    {
        private:
        public:
            Credits();
            gp::Scene execute();
            bn::optional<bn::blending_fade_alpha_to_action> _fade_action;
            void fade_out();
    };
}

#endif