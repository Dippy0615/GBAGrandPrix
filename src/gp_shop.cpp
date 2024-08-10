#include "bn_core.h"
#include "bn_optional.h"
#include "bn_blending.h"
#include "bn_blending_actions.h"
#include "bn_sprite_text_generator.h"
#include "bn_sprite_ptr.h"
#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_items_bg_checkerboard.h"
#include "bn_regular_bg_items_bg_menu_overlay.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_tiles_ptr.h"
#include "bn_sprite_items_spr_car.h"
#include "bn_sprite_items_spr_truck.h"
#include "bn_sprite_items_spr_coin.h"
#include "bn_camera_ptr.h"
#include "bn_vector.h"
#include "bn_keypad.h"
#include "bn_string_view.h"
#include "bn_fixed.h"

#include "common_variable_8x16_sprite_font.h"

#include "gp_scene.h"
#include "gp_shop.h"
#include "gp_shopitem.h"
#include "gp_constants.h"
#include "gp_utils.h"

namespace gp
{
    Shop::Shop(){}

    void Shop::fade_out()
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

    gp::Scene Shop::execute()
    {
        //Setup the background
        bn::regular_bg_ptr bg = bn::regular_bg_items::bg_checkerboard.create_bg(0, 0);
        bg.set_blending_enabled(true);
        bn::regular_bg_ptr bg2 = bn::regular_bg_items::bg_menu_overlay.create_bg(0, 32);
        bg2.set_blending_enabled(true);
        
        //Setup the camera
        bn::camera_ptr cam = bn::camera_ptr::create(0, 0);

        //Setup the text
        bn::sprite_text_generator text(common::variable_8x16_sprite_font);
        bn::vector<bn::sprite_ptr, 8> text_header_sprites;
        bn::vector<bn::sprite_ptr, 8> text_sprites;
        text.set_center_alignment();
        text.generate(0, -74, "B: EXIT    UP: CARS", text_header_sprites);
        for(bn::sprite_ptr spr : text_header_sprites)
        {
            spr.set_blending_enabled(true);
        }

        //Setup the shop's items & sprites
        int shop_item = 0;
        bn::fixed text_x = -128;
        bn::fixed coin_text_x = 128;

        bn::vector<ShopItem, 16> shop;
        shop.push_back(gp::ShopItem("CYBER-TRUCK", gp::SHOP_TYPE_CAR, bn::sprite_items::spr_truck.create_sprite(0, 0)));
        bn::sprite_ptr coin = bn::sprite_items::spr_coin.create_sprite(84, -52);
        coin.set_blending_enabled(true);

        int time = 0;

        //fade in
        bn::blending::set_fade_alpha(1);
        bn::blending::set_fade_color(bn::blending::fade_color_type::WHITE);
        _fade_action = bn::blending_fade_alpha_to_action(30, 0);
        while(!_fade_action.value().done()){
            _fade_action.value().update();
            for(int i=0;i<2;i++)
            {
                time++;
                coin.set_tiles(bn::sprite_items::spr_coin.tiles_item().create_tiles((time/4) % 4));

                bg.set_x(bg.x() + bn::fixed(0.5));
                bg.set_y(bg.y() + bn::fixed(0.5));
                bn::core::update();
            }
        }

        text_sprites.clear();
        text.generate(text_x, 48, shop[shop_item].name(), text_sprites);
        
        

        while(true)
        {
            time++;

            //Handle text
            text_sprites.clear();
            text_x = gp::lerp(text_x, 0, bn::fixed(0.2));
            coin_text_x = gp::lerp(coin_text_x, 100, bn::fixed(0.2));
            
            text.generate(text_x, 48, shop[shop_item].name(), text_sprites);
            text.generate(coin_text_x, -52, "0", text_sprites);
            for(bn::sprite_ptr spr : text_sprites)
            {
                spr.set_blending_enabled(true);
            }

            //Animate coin
            coin.set_tiles(bn::sprite_items::spr_coin.tiles_item().create_tiles((time/4) % 4));

            if (bn::keypad::pressed(bn::keypad::key_type::B))
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