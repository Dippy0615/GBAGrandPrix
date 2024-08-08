#ifndef GP_TRACKSELECT_H
#define GP_TRACKSELECT_H

#include "bn_blending_actions.h"
#include "bn_optional.h"
#include "bn_sprite_ptr.h"
#include "bn_vector.h"
#include "bn_camera_ptr.h"

#include "gp_scene.h"

namespace gp
{
    class TrackSelect
    {
        private:
        public:
            TrackSelect();
            bn::optional<int> execute();
            bn::optional<bn::blending_fade_alpha_to_action> _fade_action;
            void fade_out();
    };
}

#endif