#ifndef GP_MENU_H
#define GP_MENU_H

#include "bn_blending_actions.h"
#include "bn_optional.h"
#include "bn_sprite_ptr.h"
#include "bn_vector.h"
#include "bn_camera_ptr.h"

#include "gp_scene.h"

namespace gp
{
    class Menu
    {
        private:
        public:
            Menu();
            gp::Scene execute();
            bn::optional<bn::blending_fade_alpha_to_action> _fade_action;
            void fade_out();
            void update_header_text(bn::vector<bn::sprite_ptr, 16> sprites, bn::fixed speed);
            void update_menu_text(bn::vector<bn::sprite_ptr, 16> sprites, bn::camera_ptr camera, bn::fixed time);
    };
}

#endif