#include "bn_core.h"
#include "bn_optional.h"
#include "bn_blending.h"
#include "bn_blending_actions.h"
#include "bn_affine_bg_ptr.h"
#include "bn_affine_bg_items_title.h"
#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_items_bg_checkerboard.h"
#include "bn_regular_bg_items_bg_menu_overlay.h"
#include "bn_keypad.h"
#include "bn_sprite_text_generator.h"
#include "bn_string.h"
#include "bn_vector.h"
#include "bn_sprite_ptr.h"
#include "bn_sound_items.h"
#include "bn_sound_actions.h"

#include "common_variable_8x16_sprite_font.h"
#include "fixed_32x64_sprite_font.h"

#include "gp_scene.h"
#include "gp_title.h"

namespace gp
{
    Title::Title(){}

    void Title::fade_out()
    {
        bn::blending::set_fade_alpha(0);
        bn::blending::set_fade_color(bn::blending::fade_color_type:: WHITE);
        _fade_action = bn::blending_fade_alpha_to_action(25, 1);
        while(!_fade_action.value().done()){
            _fade_action.value().update();
            for(int i=0;i<2;i++)
                bn::core::update();
        }
        bn::blending::set_fade_alpha(1);
    }

    gp::Scene Title::execute()
    {
        //Setup the road & background
        bn::regular_bg_ptr bg2 = bn::regular_bg_items::bg_checkerboard.create_bg(0, 0);
        bg2.set_blending_enabled(true);


        //Setup the text
        bn::sprite_text_generator text(common::variable_8x16_sprite_font);
        bn::sprite_text_generator huge_text(fixed_32x64_sprite_font);
        text.set_center_alignment();
        huge_text.set_center_alignment();
        bn::vector<bn::sprite_ptr, 16> text_sprites;
        bn::vector<bn::sprite_ptr, 16> huge_text_sprites;

        bn::fixed text_x = 168;
        bn::fixed huge_y = 0;
        huge_text.generate(0, huge_y, "GBA", huge_text_sprites);
        for(bn::sprite_ptr spr: huge_text_sprites){
            spr.set_blending_enabled(true);
        }
        
        //fade in
        bn::blending::set_fade_alpha(1);
        bn::blending::set_fade_color(bn::blending::fade_color_type::WHITE);
        _fade_action = bn::blending_fade_alpha_to_action(15, 0);
        while(!_fade_action.value().done()){
            _fade_action.value().update();
            for(int i=0;i<2;i++)
            {
                bn::core::update();
            }
        }

        int time = 0;
        bool small_text = false;

        while(true)
        {
            time++;

            if(time>30)
            {
                if(huge_y>-48) huge_y-=1;
                else small_text = true;
            }

            if(small_text)
            {
                text_sprites.clear();
                text.generate(text_x, 0, "GRAND PRIX!!", text_sprites);
                if(text_x>0)
                {
                    text_x-=4;
                }
                else
                {
                    text.generate(text_x, 64, "PRESS START", text_sprites);
                    for(bn::sprite_ptr spr: text_sprites){
                        spr.set_blending_enabled(true);
                    }

                    if(bn::keypad::pressed(bn::keypad::key_type::START)) 
                    {
                        bn::sound_items::menusel.play(1);
                        fade_out();
                        return gp::Scene::Menu;
                    }
                }     
            }
            for(bn::sprite_ptr spr: text_sprites){
                spr.set_blending_enabled(true);
            }

            huge_text_sprites.clear();
            huge_text.generate(0, huge_y, "GBA", huge_text_sprites);
            for(bn::sprite_ptr spr: huge_text_sprites){
                spr.set_blending_enabled(true);
            }

            bn::core::update();
        }
    }
}