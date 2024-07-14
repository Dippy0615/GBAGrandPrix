#include "bn_core.h"
#include "bn_fixed.h"
#include "bn_keypad.h"
#include "bn_affine_bg_ptr.h"
#include "bn_affine_bg_items_bg_road.h"
#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_items_bg_grass.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_items_spr_car.h"
#include "bn_sprite_items_spr_mudslick.h"
#include "bn_sprite_items_spr_finishline.h"
#include "bn_sprite_items_spr_roadblock.h"
#include "bn_sprite_items_spr_warningsign.h"
#include "bn_array.h"
#include "bn_log.h"
#include "bn_math.h"
#include "bn_vector.h"
#include "bn_rect.h"

#include "gp_scene.h"
#include "gp_ingame.h"
#include "gp_car.h"
#include "gp_player.h"
#include "gp_tracksegment.h"
#include "gp_utils.h"
#include "gp_constants.h"
#include "gp_trackobject.h"
#include "gp_tracks.h"

namespace gp
{
    Ingame::Ingame(){}

    Scene Ingame::execute(int level)
    {
        //Setup the road & background
        bn::regular_bg_ptr bg = bn::regular_bg_items::bg_grass.create_bg(0, 0);
        bn::affine_bg_ptr road = bn::affine_bg_items::bg_road.create_bg(0, 0);
        bg.set_priority(1);
        road.set_priority(0);
        bn::fixed road_angle = 0;

        //Setup the cars
        bn::sprite_ptr car_sprite = bn::sprite_items::spr_car.create_sprite(0,24);
        Car car = Car(car_sprite);
        Player player = Player(car);
        Car* player_car = player.get_car();

        //Setup the track        
        bn::vector<TrackSegment, 16> segments =  gp::get_track(level);
        
        int current_segment_index = 0;
        TrackSegment* current_segment = &segments[current_segment_index];
        
        //Setup the objects
        bn::vector<TrackObject, 16>* objects = current_segment->get_objects();
        
        while(true)
        {
            //Move to the next segment
            if (player_car->distance()>current_segment->end())
            {
                current_segment_index++;
                if(current_segment_index>=segments.size()) //Lap
                {
                    current_segment_index = 0;
                    player_car->set_distance(0);
                }
                current_segment = &segments[current_segment_index];
                objects = current_segment->get_objects();
            }

            //Scroll the background
            bn::fixed speed = player_car->speed()/3;
            bg.set_x(bg.x() + (gp::sign(current_segment->curve()) * (speed)));
            bg.set_y(bg.y() + (speed));
            
            //Handle the road's angle
            road_angle = gp::lerp(road_angle, current_segment->curve(), 0.1);
            bn::fixed true_angle = gp::angle_wrap(road_angle);
            road.set_rotation_angle(true_angle);
            player_car->set_angle(true_angle);

            //Handle track objects
            for(auto it = objects->begin(), end = objects->end(); it != end; )
            {
                TrackObject object = *it;
                bn::fixed ypos = player_car->distance() - object.position();
                object._sprite.set_y(ypos);
                
                //Collision
                if(player_car->get_rect().intersects(object.get_rect()))
                {
                    switch(object.type())
                    {
                        default:
                            break;
                        case gp::OBJ_MUDSLICK:
                            if(player_car->_state==gp::CAR_STATE_NORMAL)
                            {
                                player_car->_hit = gp::CAR_HIT_TIME;
                                player_car->_state = gp::CAR_STATE_HIT;
                            }
                            break;
                        case gp::OBJ_ROADBLOCK:
                            if(player_car->_state==gp::CAR_STATE_NORMAL)
                            {
                                player_car->_hit = gp::CAR_HIT_TIME;
                                player_car->_state = gp::CAR_STATE_HIT2;
                            }
                            break;
                    }
                }

                it++;
            }

            player.update();
            
            bn::core::update();
        }
    }
}