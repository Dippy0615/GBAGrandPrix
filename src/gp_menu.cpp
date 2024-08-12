#include "bn_core.h"
#include "bn_optional.h"
#include "bn_blending.h"
#include "bn_blending_actions.h"
#include "bn_array.h"
#include "bn_sprite_ptr.h"
#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_items_bg_checkerboard.h"
#include "bn_regular_bg_items_bg_menu_overlay.h"
#include "bn_sprite_text_generator.h"
#include "bn_vector.h"
#include "bn_string.h"
#include "bn_string_view.h"
#include "bn_keypad.h"
#include "bn_math.h"
#include "bn_camera_ptr.h"
#include "bn_sound_items.h"
#include "bn_sound_actions.h"

#include "common_variable_8x16_sprite_font.h"

#include "gp_scene.h"
#include "gp_menu.h"
#include "gp_utils.h"

namespace gp
{
    Menu::Menu(){}

    void Menu::fade_out()
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

    void Menu::update_header_text(bn::vector<bn::sprite_ptr, 16> sprites, bn::fixed speed)
    {
        for(bn::sprite_ptr spr : sprites)
        {
            spr.set_x(spr.x() - speed);
            if(spr.x()<-230) spr.set_x(spr.x()+230*2);
        }
    }

    void Menu::update_menu_text(bn::vector<bn::sprite_ptr, 16> sprites, bn::camera_ptr camera, bn::fixed time)
    {
        for(bn::sprite_ptr spr : sprites)
        {
            spr.set_camera(camera);
            spr.set_y(0 + bn::cos(time)*3);
        }
    }

    Scene Menu::execute()
    {
        //Setup the background
        bn::regular_bg_ptr bg = bn::regular_bg_items::bg_checkerboard.create_bg(0, 0);
        bg.set_blending_enabled(true);
        bn::regular_bg_ptr bg2 = bn::regular_bg_items::bg_menu_overlay.create_bg(0, 32);
        bg2.set_blending_enabled(true);

        //Setup the camera
        bn::camera_ptr cam = bn::camera_ptr::create(0, 0);

        //Setup the text
        int menu_items = 4;
        int menu_spread = 255;

        bn::string_view menu_text[] = {"PLAY", "SHOP", "HIGHSCORES", "CREDITS"};
        bn::sprite_text_generator text(common::variable_8x16_sprite_font);
        bn::vector<bn::sprite_ptr, 16> text_sprites;
        text.set_center_alignment();
        for(int i=0;i<menu_items;i++)
            text.generate(0+(i*menu_spread), 0, menu_text[i], text_sprites);
        for(bn::sprite_ptr spr : text_sprites)
        {
            spr.set_blending_enabled(true);
        }

        int menu = 0;
        bn::fixed time = 0;

        bn::vector<bn::sprite_ptr, 24> header_text_sprites;
        text.generate(0, -74, "GAME BOY ADVANCE GRAND PRIX", header_text_sprites);
        for(bn::sprite_ptr spr : header_text_sprites)
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
                time+=0.01;
                update_menu_text(text_sprites, cam, time);
                update_header_text(header_text_sprites, bn::fixed(1));
                bn::core::update();
            }
        }

        while(true)
        {
            time+=0.01;

            //Handle the menu text
            update_menu_text(text_sprites, cam, time);
            
            //Handle the header text
            update_header_text(header_text_sprites, bn::fixed(1));
            
            //Move the camera
            cam.set_x(gp::lerp(cam.x(), menu*menu_spread, bn::fixed(0.1)));

            //Handle the buttons
            if (bn::keypad::pressed(bn::keypad::key_type::LEFT)) 
            {
                if(menu>0) 
                {
                    menu--;
                    bn::sound_items::beep.play(1);
                }
                
            }
            if (bn::keypad::pressed(bn::keypad::key_type::RIGHT))
            {
                if(menu<menu_items-1)
                {
                    menu++;
                    bn::sound_items::beep.play(1);
                }
            }
            if (bn::keypad::pressed(bn::keypad::key_type::A))
            {
                bn::sound_items::menusel.play(1);
                fade_out();
                switch(menu)
                {
                    case 0: default: return gp::Scene::TrackSelect; break;
                    case 1: return gp::Scene::Shop; break;
                    case 2: return gp::Scene::Highscores; break;
                    case 3: return gp::Scene::Credits; break;
                }
            }

            //Scroll the BG
            bg.set_x(bg.x() + bn::fixed(0.5));
            bg.set_y(bg.y() + bn::fixed(0.5));

            bn::core::update();
        }
    }
}