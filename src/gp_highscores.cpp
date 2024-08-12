#include "bn_core.h"
#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_items_bg_checkerboard.h"
#include "bn_regular_bg_items_bg_menu_overlay.h"
#include "bn_keypad.h"
#include "bn_string.h"
#include "bn_sprite_text_generator.h"
#include "bn_sprite_ptr.h"
#include "bn_vector.h"
#include "bn_blending.h"
#include "bn_blending_actions.h"
#include "bn_sound_items.h"
#include "bn_sound_actions.h"

#include "common_variable_8x16_sprite_font.h"

#include "gp_globals.h"
#include "gp_scene.h"
#include "gp_score.h"
#include "gp_highscores.h"

namespace gp
{
    Highscores::Highscores(){}

    void Highscores::fade_out()
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

    gp::Scene Highscores::execute()
    {
        //Setup the background
        bn::regular_bg_ptr bg = bn::regular_bg_items::bg_checkerboard.create_bg(0, 0);
        bg.set_blending_enabled(true);
        bn::regular_bg_ptr bg2 = bn::regular_bg_items::bg_menu_overlay.create_bg(0, 32);
        bg2.set_blending_enabled(true);

        //Setup the text
        bn::sprite_text_generator text(common::variable_8x16_sprite_font);
        text.set_center_alignment();
        bn::vector<bn::sprite_ptr, 24> text_sprites;
        text.generate(0, -74, "HIGHSCORES", text_sprites);

        for(int i=0;i<4;i++)
        {
            bn::string_view lev = "GROUND";
            if(i==1) lev = "JUNGLE";
            if(i==2) lev = "SKY";
            if(i==3) lev = "SPACE";
            text.generate(0, -48+(i*32), lev, text_sprites);
            bn::string_view text2 = bn::to_string<16>(scores[i].mins()) + " : " + bn::to_string<16>(scores[i].secs()) + " : " + bn::to_string<16>(scores[i].millis());
            text.generate(0, -32+(i*32), text2, text_sprites);
        }
        
        for(bn::sprite_ptr spr : text_sprites)
        {
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

        while(true)
        {
            if (bn::keypad::pressed(bn::keypad::key_type::B))
            {
                bn::sound_items::error.play(1);
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