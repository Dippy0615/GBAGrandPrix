#include "bn_core.h"
#include "bn_optional.h"
#include "bn_blending.h"
#include "bn_blending_actions.h"
#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_items_bg_checkerboard.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_items_spr_track_ground.h"
#include "bn_sprite_items_spr_track_jungle.h"
#include "bn_string_view.h"
#include "bn_sprite_text_generator.h"
#include "bn_vector.h"
#include "bn_keypad.h"
#include "bn_camera_ptr.h"

#include "common_variable_8x16_sprite_font.h"

#include "gp_scene.h"
#include "gp_trackselect.h"
#include "gp_utils.h"

namespace gp
{
    TrackSelect::TrackSelect(){}

    void TrackSelect::fade_out()
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

    bn::optional<int> TrackSelect::execute()
    {
        //Setup the background
        bn::regular_bg_ptr bg = bn::regular_bg_items::bg_checkerboard.create_bg(0, 0);
        bg.set_blending_enabled(true);
        
        //Setup the camera
        bn::camera_ptr cam = bn::camera_ptr::create(0, 0);

        //Setup the sprites
        int margin = 200;
        bn::sprite_ptr ground = bn::sprite_items::spr_track_ground.create_sprite(0,-16);
        ground.set_blending_enabled(true);
        ground.set_camera(cam);

        bn::sprite_ptr jungle = bn::sprite_items::spr_track_jungle.create_sprite(margin,-16);
        jungle.set_blending_enabled(true);
        jungle.set_camera(cam);

        //Setup the text
        bn::string_view menu_text[] = {"GROUND", "JUNGLE", "SKY", "SPACE"};
        bn::sprite_text_generator text(common::variable_8x16_sprite_font);
        bn::vector<bn::sprite_ptr, 16> text_sprites;
        text.set_center_alignment();
        text.generate(0, -64, "SELECT YOUR TRACK", text_sprites);
        text.generate(0, 32, menu_text[0], text_sprites);
        for(bn::sprite_ptr spr : text_sprites)
        {
            spr.set_blending_enabled(true);
        }

        int track = 0;

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
            
            //Handle the buttons
            if (bn::keypad::pressed(bn::keypad::key_type::RIGHT) && track<3)
            {
                track++;
                text_sprites.clear();
                text.generate(0, -64, "SELECT YOUR TRACK", text_sprites);
                text.generate(0, 32, menu_text[track], text_sprites);
                for(bn::sprite_ptr spr : text_sprites)
                {
                    spr.set_blending_enabled(true);
                }
            }
            
            if (bn::keypad::pressed(bn::keypad::key_type::LEFT) && track>0)
            {
                track--;
                text_sprites.clear();
                text.generate(0, -64, "SELECT YOUR TRACK", text_sprites);
                text.generate(0, 32, menu_text[track], text_sprites);
                for(bn::sprite_ptr spr : text_sprites)
                {
                    spr.set_blending_enabled(true);
                }
            }

            if (bn::keypad::pressed(bn::keypad::key_type::A))
            {
                fade_out();
                return track;
            }

            if (bn::keypad::pressed(bn::keypad::key_type::B))
            {
                fade_out();
                return bn::nullopt_t(track);
            }

            //Move the camera
            cam.set_x(gp::lerp(cam.x(), track*margin, bn::fixed(0.1)));

            //Scroll the BG
            bg.set_x(bg.x() + bn::fixed(0.5));
            bg.set_y(bg.y() + bn::fixed(0.5));

            bn::core::update();
        }
    }
}