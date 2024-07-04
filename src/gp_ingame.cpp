#include "bn_core.h"
#include "bn_fixed.h"
#include "bn_keypad.h"
#include "bn_affine_bg_ptr.h"
#include "bn_affine_bg_items_bg_road.h"
#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_items_bg_grass.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_items_spr_car.h"

#include "gp_scene.h"
#include "gp_ingame.h"
#include "gp_car.h"
#include "gp_player.h"

namespace gp
{
    Ingame::Ingame(){}

    Scene Ingame::execute()
    {
        bn::regular_bg_ptr bg = bn::regular_bg_items::bg_grass.create_bg(0, 0);
        bn::affine_bg_ptr road = bn::affine_bg_items::bg_road.create_bg(0, 0);
        bg.set_priority(1);
        road.set_priority(0);
        int road_angle = 0;
        
        bn::sprite_ptr car_sprite = bn::sprite_items::spr_car.create_sprite(0,24);
        Car car = Car(car_sprite);
        Player player = Player(car);

        while(true)
        {
            /*
            if (bn::keypad::held(bn::keypad::key_type::LEFT))
            {
                road_angle++;
                if (road_angle>359) road_angle = 0;
            }
            if (bn::keypad::held(bn::keypad::key_type::RIGHT))
            {
                road_angle--;
                if (road_angle<0) road_angle = 359;
            }
            */

            road.set_rotation_angle(bn::fixed(road_angle));
            
            bg.set_y(bg.y() + (player.speed()/3));
            road.set_y(road.y() + player.speed()/2);

            player.update();
            
            bn::core::update();
        }
    }
}