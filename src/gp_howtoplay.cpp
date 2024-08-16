#include "bn_core.h"
#include "bn_optional.h"
#include "bn_blending.h"
#include "bn_blending_actions.h"
#include "bn_sprite_text_generator.h"
#include "bn_sprite_ptr.h"
#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_items_bg_checkerboard.h"
#include "bn_regular_bg_items_bg_menu_overlay.h"
#include "bn_vector.h"
#include "bn_string_view.h"
#include "bn_string.h"
#include "bn_fixed.h"
#include "bn_log.h"
#include "bn_keypad.h"
#include "bn_sound_items.h"
#include "bn_sound_actions.h"

#include "common_variable_8x8_sprite_font.h"
#include "common_variable_8x16_sprite_font.h"

#include "gp_scene.h"
#include "gp_howtoplay.h"

namespace gp
{
    HowToPlay::HowToPlay(){}

    void HowToPlay::fade_out()
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

    gp::Scene HowToPlay::execute()
    {
        //Setup the background
        bn::regular_bg_ptr bg = bn::regular_bg_items::bg_checkerboard.create_bg(0, 0);
        bg.set_blending_enabled(true);
        bn::regular_bg_ptr bg2 = bn::regular_bg_items::bg_menu_overlay.create_bg(0, 32);
        bg2.set_blending_enabled(true);

        //Setup the text
        int page = 0;
        int pages = 5;
        bn::string_view text_lines[][8] = {
            {
                "GBA GRAND PRIX is a Game Boy Advance",
                "remake of NOKIA GRAND PRIX.",
                "",
                "In GBA GRAND PRIX, you will drive on",
                "4 tracks, with the goal of completing them",
                "as quickly as possible.",
                "Records will be saved for you to",
                "look at when you want to."
            },
            {
                "There are 2 coins that are placed",
                "on each track. Collect them to buy new cars",
                "that look cool and have a higher top speed.",
                "",
                "But sure to stay on the track!",
                "If you fall off of the track, the coins you",
                "got during the race will not be added to",
                "your total."
            },
            {
                "When driving, you also have to look out",
                "for obstacles.",
                "A yellow sign will appear on the top of",
                "the screen when you are approaching one.",
                "Move out of the way to avoid getting hit!"
            },
            {
                "There will also be blue orbs on the track.",
                "These orbs can give you a burst of speed,",
                "by filling up the boost bar at the bottom",
                "of the screen.",
                "You can boost when the bar is full."
            },
            {
                "CONTROLS:",
                "",
                "Left: Move left",
                "Right: Move right",
                "A: Accelerate",
                "B: Brake",
                "R: Boost"
            }
        };
        
        bn::sprite_text_generator text(common::variable_8x16_sprite_font);
        bn::sprite_text_generator small_text(common::variable_8x8_sprite_font);
        bn::vector<bn::sprite_ptr, 16> text_sprites;
        bn::vector<bn::sprite_ptr, 64> small_text_sprites;
        text.set_center_alignment();
        small_text.set_left_alignment();
        text.generate(0, -74, "HOW TO PLAY", text_sprites);
        for(bn::sprite_ptr spr : text_sprites)
        {
            spr.set_blending_enabled(true);
        }
        small_text_sprites.clear();
        int text_size2 = (int)(sizeof(text_lines[page]) / sizeof(bn::string_view));
        for(int i=0;i<text_size2;i++)
        {
            small_text.generate(-118, -58+(i*16), text_lines[page][i], small_text_sprites);
        }
        for(bn::sprite_ptr spr : small_text_sprites)
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
                //Scroll the BG
                bg.set_x(bg.x() + bn::fixed(0.5));
                bg.set_y(bg.y() + bn::fixed(0.5));
                bn::core::update();
            }
        }

        while(true)
        {
            if (bn::keypad::pressed(bn::keypad::key_type::RIGHT)) 
            {
                if(page<pages-1) 
                {
                    bn::sound_items::select.play(1);
                    page++;
                }

                small_text_sprites.clear();
                int text_size = (int)(sizeof(text_lines[page]) / sizeof(bn::string_view));
                for(int i=0;i<text_size;i++)
                {
                    small_text.generate(-118, -58+(i*16), text_lines[page][i], small_text_sprites);
                }
            }

            if (bn::keypad::pressed(bn::keypad::key_type::LEFT)) 
            {
                if(page>0)
                {
                    bn::sound_items::select.play(1);
                    page--;
                }

                small_text_sprites.clear();
                int text_size = (int)(sizeof(text_lines[page]) / sizeof(bn::string_view));
                for(int i=0;i<text_size;i++)
                {
                    small_text.generate(-118, -58+(i*16), text_lines[page][i], small_text_sprites);
                }
            }

            if (bn::keypad::pressed(bn::keypad::key_type::B)) 
            {
                bn::sound_items::error.play(1);
                fade_out();
                return gp::Scene::Menu;
            }

            text_sprites.clear();
            text.generate(0, -74, "HOW TO PLAY", text_sprites);
            text.generate(0, 74, "Page "+bn::to_string<8>(page+1)+"/"+bn::to_string<8>(pages), text_sprites);

            for(bn::sprite_ptr spr : text_sprites)
            {
                spr.set_blending_enabled(true);
            }
            for(bn::sprite_ptr spr : small_text_sprites)
            {
                spr.set_blending_enabled(true);
            }

            //Scroll the BG
            bg.set_x(bg.x() + bn::fixed(0.5));
            bg.set_y(bg.y() + bn::fixed(0.5));

            bn::core::update();
        }
    }
}