#ifndef GP_HOWTOPLAY_H
#define GP_HOWTOPLAY_H

#include "bn_blending_actions.h"
#include "bn_optional.h"
#include "bn_string_view.h"
#include "bn_sprite_text_generator.h"
#include "bn_vector.h"
#include "bn_sprite_ptr.h"

#include "common_variable_8x8_sprite_font.h"

#include "gp_scene.h"

namespace gp
{
    class HowToPlay
    {
        private:
        public:
            HowToPlay();
            gp::Scene execute();
            bn::optional<bn::blending_fade_alpha_to_action> _fade_action;
            void fade_out();
            void draw_page(bn::string_view lines[], bn::vector<bn::sprite_ptr, 64> text_sprites);
    };
}

#endif