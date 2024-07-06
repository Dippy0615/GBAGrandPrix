#include "bn_core.h"
#include "bn_fixed.h"
#include "bn_keypad.h"
#include "bn_affine_bg_ptr.h"
#include "bn_affine_bg_items_bg_road.h"
#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_items_bg_grass.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_items_spr_car.h"
#include "bn_array.h"
#include "bn_log.h"
#include "bn_math.h"

#include "gp_scene.h"
#include "gp_ingame.h"
#include "gp_car.h"
#include "gp_player.h"
#include "gp_tracksegment.h"
#include "gp_utils.h"
#include "gp_constants.h"

namespace gp
{
    Ingame::Ingame(){}

    Scene Ingame::execute()
    {
        //Setup the road & background
        bn::regular_bg_ptr bg = bn::regular_bg_items::bg_grass.create_bg(0, 0);
        bn::affine_bg_ptr road = bn::affine_bg_items::bg_road.create_bg(0, 0);
        bg.set_priority(1);
        road.set_priority(0);
        road.set_pivot_x(0);
        road.set_pivot_y(0);
        bn::fixed road_angle = 0;
        
        //Setup the cars
        bn::sprite_ptr car_sprite = bn::sprite_items::spr_car.create_sprite(0,24);
        Car car = Car(car_sprite);
        Player player = Player(car);

        //Setup the track        
        bn::array<TrackSegment, 2> segments;

        for(int i=0; i<2; i++)
        {
            TrackSegment segment;
            segment.position = 1000*i;
            segment.length = 1000;
            segment.curve = 0;
            if(i==1) segment.curve = 30;
            segments[i] = segment;
        }

        int current_segment_index = 0;
        TrackSegment current_segment = segments[current_segment_index];
        bn::fixed last_angle = 0;
        int turning = -1;
        bool has_turned = false;

        while(true)
        {
            if(turning>-1) turning--;
            Car* player_car = player.get_car();

            //Move to the next segment
            int segment_end = current_segment.position + current_segment.length;
            if (player_car->distance()>segment_end)
            {
                current_segment_index++;
                last_angle = current_segment.curve;
                has_turned = false;
                if(current_segment_index>=segments.size()) //Lap
                {
                    current_segment_index = 0;
                    player_car->set_distance(0);
                }
                current_segment = segments[current_segment_index];
            }

            //Scroll the road & background
            bn::fixed speed = player_car->speed()/3;
            bg.set_x(bg.x() + (gp::sign(current_segment.curve) * (speed)));
            bg.set_y(bg.y() + (speed));
            
            //Handle the road's angle
            road_angle = gp::lerp(road_angle, current_segment.curve, 0.1);
            road.set_rotation_angle(road_angle);
            player_car->set_angle(road_angle);
            
            if (turning==-1 && !has_turned && (last_angle != current_segment.curve))
            {
                turning = gp::TRACK_TURN_ADJUST;
                has_turned = true;
            }

            player.update();
            
            bn::core::update();
        }
    }
}