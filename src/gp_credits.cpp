#include "bn_core.h"
#include "bn_optional.h"
#include "bn_blending.h"
#include "bn_blending_actions.h"
#include "bn_sprite_text_generator.h"
#include "bn_sprite_ptr.h"
#include "bn_vector.h"
#include "bn_string_view.h"
#include "bn_fixed.h"
#include "bn_log.h"
#include "bn_keypad.h"
#include "bn_sound_items.h"
#include "bn_sound_actions.h"

#include "common_variable_8x16_sprite_font.h"

#include "gp_scene.h"
#include "gp_credits.h"

namespace gp
{
    Credits::Credits(){}

    void Credits::fade_out()
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

    gp::Scene Credits::execute()
    {

        bn::sprite_text_generator text(common::variable_8x16_sprite_font);
        text.set_center_alignment();
        bn::vector<bn::sprite_ptr, 48> text_sprites;
        bn::string_view text_lines[] = {
            "GAME BOY ADVANCE GRAND PRIX",
            "A GBA JAM 2024 GAME",
            "",
            "PROGRAMMED BY:",
            "DIPPY",
            "USING BUTANO",
            "BY GVALIENTE",
            "github.com/GValiente/butano",
            "",
            "GRAPHICS:",
            "TRACK BACKGROUNDS:",
            "FREEPIK.COM",
            "MUD & MUDSLICKS:",
            "FREEPIK.COM",
            "CAR SPRITES:",
            "TOP DOWN CARS",
            "BY TOKKA",
            "tokka.itch.io/top-down-car",
            "GBA JAM LOGO:",
            "GBA JAM ORGANIZERS",
            "REST IS DRAWN BY DIPPY",
            "",
            "SOUND EFFECTS:",
            "BeepPing2.flac",
            "freesound.org/s/259701/",
            "Confirm Sound",
            "freesound.org/s/667844/",
            "menuSel.wav",
            "freesound.org/s/87035/",
            "Cash register",
            "freesound.org/s/721774/",
            "Error",
            "freesound.org/s/664038/",
            "Menu_Select_00.wav",
            "freesound.org/s/270401/",
            "Clank Car Crash Collision",
            "freesound.org/s/151624/",
            "crash.wav",
            "freesound.org/s/40158/",
            "Squelch_2.wav",
            "freesound.org/s/148974/",
            "Boost.wav",
            "freesound.org/s/420998/",
            "flags_1.wav",
            "freesound.org/s/386569/",
            "Coin_C_06",
            "freesound.org/s/350869/",
            "Rustling Foliage",
            "freesound.org/s/635053/",
            "Logotype, Race 02.wav",
            "freesound.org/s/523210/",
            "",
            "MUSIC:",
            "ALL SONGS ARE COMPOSED BY:",
            "DIPPY",
            "",
            "",
        };

        //fade in
        bn::blending::set_fade_alpha(1);
        bn::blending::set_fade_color(bn::blending::fade_color_type::WHITE);
        _fade_action = bn::blending_fade_alpha_to_action(30, 0);
        while(!_fade_action.value().done()){
            _fade_action.value().update();
            for(int i=0;i<2;i++)
            {
                bn::core::update();
            }
        }

        int time = 0;
        int timer = 160;
        int text_index = 0;
        int text_size = (int)(sizeof(text_lines) / sizeof(bn::string_view));
        bn::fixed text_speed = 0;

        while(true)
        {
            time++;
            timer--;
            text_speed = 0.25;
            if (bn::keypad::held(bn::keypad::key_type::DOWN) && time>4)
            {
                //times 4
                text_speed = 1;
                timer-=3;
            }

            if (bn::keypad::held(bn::keypad::key_type::B))
            {
                bn::sound_items::error.play(1);
                fade_out();
                return gp::Scene::Menu;
            }

            if(time==4 || timer<0)
            {
                timer = 160;
                if(text_index<=text_size-1)
                {
                    if(text_lines[text_index].empty())
                    {
                        text_index++;
                    }
                    else
                    {
                        text.generate(0, 90, text_lines[text_index], text_sprites);
                        text_index++;
                    }
                }
                else
                {
                    fade_out();
                    return gp::Scene::Menu;
                }
            }

            for(auto it = text_sprites.begin(), end = text_sprites.end(); it != end; )
            {
                bn::sprite_ptr spr = *it;

                if(spr.y()<-122) 
                {
                    it = text_sprites.erase(it);
                    end = text_sprites.end();
                }

                spr.set_blending_enabled(true);
                spr.set_y(spr.y()-bn::fixed(text_speed));
                it++;
            }

            bn::core::update();
        }
    }
}