#include "bn_core.h"
#include "bn_optional.h"
#include "bn_blending.h"
#include "bn_blending_actions.h"
#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_items_bg_checkerboard.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_text_generator.h"
#include "bn_vector.h"
#include "bn_fixed.h"
#include "bn_keypad.h"

#include "common_variable_8x16_sprite_font.h"

#include "gp_scene.h"
#include "gp_postgame.h"

namespace gp
{
    Postgame::Postgame(){}

    void Postgame::fade_out()
    {
        bn::blending::set_fade_alpha(0);
        bn::blending::set_fade_color(bn::blending::fade_color_type:: WHITE);
        _fade_action = bn::blending_fade_alpha_to_action(15, 1);
        while(!_fade_action.value().done()){
            _fade_action.value().update();
            for(int i=0;i<2;i++)
                bn::core::update();

        }
        bn::blending::set_fade_alpha(1);
    }

    Scene Postgame::execute()
    {
        //Setup the background
        bn::regular_bg_ptr bg = bn::regular_bg_items::bg_checkerboard.create_bg(0, 0);
        bg.set_blending_enabled(true);

        //Setup the text
        bn::sprite_text_generator text(common::variable_8x16_sprite_font);
        text.set_center_alignment();
        bn::vector<bn::sprite_ptr, 8> text_sprites;
        text.generate(0, -48, "POST GAME", text_sprites);
        text.generate(0, 0, "STATS + MENU GOES HERE", text_sprites);
        for(bn::sprite_ptr spr: text_sprites){
            spr.set_blending_enabled(true);
        }

        //fade in
        bn::blending::set_fade_alpha(1);
        bn::blending::set_fade_color(bn::blending::fade_color_type::WHITE);
        _fade_action = bn::blending_fade_alpha_to_action(30, 0);
        while(!_fade_action.value().done()){
            _fade_action.value().update();
            for(int i=0;i<2;i++)
            {
                bg.set_x(bg.x() + bn::fixed(0.5));
                bg.set_y(bg.y() + bn::fixed(0.5));
                bn::core::update();
            }
        }
        bn::blending::set_fade_alpha(0);

        while(true)
        {
            if (bn::keypad::pressed(bn::keypad::key_type::A))
            {
                fade_out();
                return gp::Scene::Menu;
            }

            //Scroll the BG
            bg.set_x(bg.x() + bn::fixed(0.5));
            bg.set_y(bg.y() + bn::fixed(0.5));
            

            bn::core::update();
        }
    }
}