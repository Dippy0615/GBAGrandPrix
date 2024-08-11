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
#include "bn_sprite_items_spr_car_pink.h"
#include "bn_sprite_items_spr_truck.h"
#include "bn_sprite_items_spr_coin.h"
#include "bn_camera_ptr.h"
#include "bn_vector.h"
#include "bn_keypad.h"
#include "bn_string_view.h"
#include "bn_string.h"
#include "bn_fixed.h"
#include "bn_log.h"

#include "common_variable_8x16_sprite_font.h"

#include "gp_scene.h"
#include "gp_shopitem.h"
#include "gp_shop.h"
#include "gp_constants.h"
#include "gp_utils.h"
#include "gp_globals.h"

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

    bn::array<bool, 3> cars;

    void Shop::check_shop_items(bn::vector<gp::ShopItem, 16>* items)
    {
        for(auto it = items->begin(), end = items->end(); it != end; )
        {
            gp::ShopItem item = *it;

            int type = item.type();
            if(type<=2 && cars[type]) //check cars
            {
                item._active = false;
            }
            it++;
        }
    }

    int coins;
    //int current_car;
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
        bn::vector<bn::sprite_ptr, 14> text_sprites;
        bn::vector<bn::sprite_ptr, 8> text_car_sprites;
        
        text.set_center_alignment();
        text.generate(0, -74, "B: EXIT    UP: CARS", text_header_sprites);
        for(bn::sprite_ptr spr : text_header_sprites)
        {
            spr.set_blending_enabled(true);
        }

        bool shop_mode = true;
        int shop_mode_y = 0;
        int cars_mode_y = -160;
        int switch_timer = 20;
        int car = 0;

        //Setup the shop's items & sprites
        int shop_item = 0;
        bn::fixed text_x = -128;
        bn::fixed coin_text_x = 128;

        int shop_margin = 180;
        bn::vector<ShopItem, 16> shop;
        shop.push_back(gp::ShopItem("CYBER-TRUCK", gp::CYBER_TRUCK_ID, 4, bn::sprite_items::spr_truck.create_sprite(0, 0), cam));
        shop.push_back(gp::ShopItem("ELECTRO-MOBILE", gp::ELECTRO_MOBILE_ID, 6, bn::sprite_items::spr_car_pink.create_sprite(shop_margin, 0), cam));
        bn::sprite_ptr coin = bn::sprite_items::spr_coin.create_sprite(84, -52);
        coin.set_blending_enabled(true);

        //Setup car sprites
        bn::sprite_ptr car1 = bn::sprite_items::spr_car.create_sprite(0, cars_mode_y);
        bn::sprite_ptr car2 = bn::sprite_items::spr_truck.create_sprite(shop_margin, cars_mode_y);
        bn::sprite_ptr car3 = bn::sprite_items::spr_car_pink.create_sprite(shop_margin*2, cars_mode_y);
        car1.set_blending_enabled(true);
        car1.set_camera(cam);
        car2.set_blending_enabled(true);
        car2.set_camera(cam);
        car3.set_blending_enabled(true);
        car3.set_camera(cam);

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
        
        for(int i=0;i<shop.size();i++ )
        {
            gp::ShopItem item = shop[i];

            int type = item.type();
            if(type<=2 && cars[type]) //check cars
            {
                shop[i]._active = false;
            }
        }

        while(true)
        {
            time++;
            switch_timer++;
            if(shop_mode)
            {
                //Handle text
                text_sprites.clear();
                text_x = gp::lerp(text_x, 0, bn::fixed(0.2));
                coin_text_x = gp::lerp(coin_text_x, 100, bn::fixed(0.2));
                
                text.generate(text_x, 48, shop[shop_item].name(), text_sprites);
                bn::string<8> price_text;
                price_text = bn::to_string<8>(shop[shop_item].price());
                text.generate(text_x, 56, price_text + " COINS", text_sprites);
                if(!shop[shop_item]._active) 
                {
                    text.generate(text_x, 64, "SOLD OUT!", text_sprites);
                }
                text.generate(coin_text_x, -52, bn::to_string<8>(coins), text_sprites);
                for(bn::sprite_ptr spr : text_sprites)
                {
                    spr.set_blending_enabled(true);
                    if(switch_timer<19) spr.set_visible(false);
                }

                //Animate coin
                coin.set_tiles(bn::sprite_items::spr_coin.tiles_item().create_tiles((time/4) % 4));

                //Handle buttons
                if (bn::keypad::pressed(bn::keypad::key_type::LEFT))
                {
                    shop_item--;
                    if(shop_item<0) shop_item = 0;
                }

                if (bn::keypad::pressed(bn::keypad::key_type::RIGHT))
                {
                    shop_item++;
                    if(shop_item>shop.size()-1) shop_item = shop.size()-1;
                }

                if (bn::keypad::pressed(bn::keypad::key_type::UP))
                {
                    shop_mode = false;
                    text_header_sprites.clear();
                    text.generate(0, -74, "B: EXIT    DOWN: SHOP", text_header_sprites);
                    for(bn::sprite_ptr spr : text_sprites)
                    {
                        spr.set_visible(false);
                    }
                    switch_timer = 0;
                }

                if (bn::keypad::pressed(bn::keypad::key_type::A))
                {
                    if(coins>=shop[shop_item].price())
                    {
                        switch(shop_item)
                        {
                            default: break;
                            case 0: //Cyber Truck
                                cars[1] = true;
                                break;
                            case 1: //Electro-Mobile
                                cars[2] = true;
                                break;
                        }

                        for(int i=0;i<shop.size();i++ )
                        {
                            gp::ShopItem item = shop[i];

                            int type = item.type();
                            if(type<=2 && cars[type]) //check cars
                            {
                                shop[i]._active = false;
                            }
                        }
                    }
                }

                //Move the camera
                cam.set_x(gp::lerp(cam.x(), shop_item*shop_margin, bn::fixed(0.1)));
            }
            else
            {
                //Handle sprites
                text_car_sprites.clear();
                bn::string<16> car_text = "TECHNO-CAR";
                if(car==1) car_text = "CYBER-TRUCK";
                if(car==2) car_text = "ELECTRO-MOBILE";
                text.generate(0, 48, car_text, text_car_sprites);
                if(car==current_car) text.generate(0, 56, "SELECTED", text_car_sprites);
                
                //Handle buttons
                if (bn::keypad::pressed(bn::keypad::key_type::LEFT))
                {
                    car--;
                    if(car<0) car = 0;
                }

                if (bn::keypad::pressed(bn::keypad::key_type::RIGHT))
                {
                    car++;
                    if(car>2) car = 2;
                }

                if (bn::keypad::pressed(bn::keypad::key_type::A))
                {
                    current_car = car;
                }

                if (bn::keypad::pressed(bn::keypad::key_type::DOWN))
                {
                    shop_mode = true;
                    text_header_sprites.clear();
                    text_car_sprites.clear();
                    text.generate(0, -74, "B: EXIT    UP: SHOP", text_header_sprites);
                    switch_timer = 0;
                }

                //Move the camera
                cam.set_x(gp::lerp(cam.x(), car*shop_margin, bn::fixed(0.1)));
            }

            for(bn::sprite_ptr spr : text_car_sprites)
            {
                spr.set_blending_enabled(true);
            }
            for(bn::sprite_ptr spr : text_header_sprites)
            {
                spr.set_blending_enabled(true);
            }

            if (bn::keypad::pressed(bn::keypad::key_type::B))
            {
                fade_out();
                return gp::Scene::Menu;
            }

            coin.set_y(-cam.y() - 52);

            //Move the camera (Y axis)
            cam.set_y(gp::lerp(cam.y(), shop_mode ? shop_mode_y : cars_mode_y, bn::fixed(0.1)));

            //Scroll the BG
            bg.set_x(bg.x() + bn::fixed(0.5));
            bg.set_y(bg.y() + bn::fixed(0.5));

            bn::core::update();
        }
    }
}