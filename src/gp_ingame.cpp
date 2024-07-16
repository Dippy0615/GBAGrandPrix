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
#include "bn_sprite_items_spr_coin.h"
#include "bn_sprite_items_spr_bar_mid.h"
#include "bn_sprite_items_spr_plr_icon.h"
#include "bn_sprite_animate_actions.h"
#include "bn_array.h"
#include "bn_log.h"
#include "bn_math.h"
#include "bn_vector.h"
#include "bn_rect.h"
#include "bn_sprite_text_generator.h"
#include "bn_string.h"

#include "common_variable_8x16_sprite_font.h"

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
        int track_length = segments[segments.size()-1].end();

        int current_segment_index = 0;
        TrackSegment* current_segment = &segments[current_segment_index];
        
        //Setup the objects
        bn::vector<TrackObject, 16>* objects = current_segment->get_objects();
        
        //Setup the signs
        bn::array<bool, 16> signs;
        signs.fill(false);
        bn::vector<Sign, 16> signs_obj;

        //Setup the text
        bn::sprite_text_generator text_mph(common::variable_8x16_sprite_font);
        text_mph.set_center_alignment();
        text_mph.set_bg_priority(0);
        bn::vector<bn::sprite_ptr, 32> text_mph_sprites;

        //Setup the progress bar
        bn::vector<bn::sprite_ptr, 4> bar;
        for(int i=0;i<bar.max_size();i++)
        {
            bn::sprite_ptr test = bn::sprite_items::spr_bar_mid.create_sprite(gp::BAR_X,gp::BAR_Y+(i*16));
            test.set_bg_priority(0);
            bar.push_back(test);
        }
        bn::sprite_ptr plr_icon = bn::sprite_items::spr_plr_icon.create_sprite(gp::BAR_X,gp::PLR_ICON_Y);
        plr_icon.set_bg_priority(0);

        //Setup the lap counter
        bn::sprite_text_generator lap_counter(common::variable_8x16_sprite_font);
        lap_counter.set_center_alignment();
        lap_counter.set_bg_priority(0);
        bn::vector<bn::sprite_ptr, 32> lap_counter_sprites;

        int time = 0;
        int lap = 0;

        while(true)
        {
            time++;

            //Move to the next segment
            if (player_car->distance()>current_segment->end())
            {
                current_segment_index++;
                if(current_segment_index>=segments.size()) //Lap
                {
                    if(lap<3) lap++;
                    current_segment_index = 0;
                    player_car->set_distance(0);
                }
                current_segment = &segments[current_segment_index];
                objects = current_segment->get_objects();

                //Reset signs stuff
                signs.fill(false);
                signs_obj.clear();
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
            int index = 0;
            int type = 0;
            for(auto it = objects->begin(), end = objects->end(); it != end; )
            {
                TrackObject object = *it;
                type = object.type();
                bn::fixed ypos = player_car->distance() - object.position();
                object._sprite.set_y(ypos);
                //Handle sign
                if(!signs[index])
                {
                    signs[index] = true;
                    bn::sprite_ptr sign_sprite = bn::sprite_items::spr_warningsign.create_sprite(object._sprite.x(), -64);
                    sign_sprite.set_scale(0.75);
                    if(signs_obj.size()<16)
                    {
                        sign_sprite.set_visible(false);
                        signs_obj.push_back(Sign(sign_sprite));
                    }
                }
                else
                {
                    if(type==gp::OBJ_FINISHLINE || type==gp::OBJ_COIN)
                    {
                        signs_obj[index]._sprite.set_visible(false);
                    }
                    else
                    {
                        if(ypos<-350 || ypos>-100)
                        {
                            signs_obj[index]._sprite.set_visible(false);
                        }
                        else if (ypos>-350)
                        {
                            signs_obj[index].flash();
                        }
                    }
                    
                }
                
                //Coin animation
                object.coin_animate(time / 3);
                
                //Collision
                if(player_car->get_rect().intersects(object.get_rect()))
                {
                    switch(type)
                    {
                        default:
                            break;
                        case gp::OBJ_MUDSLICK:
                            if(player_car->_state==gp::CAR_STATE_NORMAL && player_car->_inv==-1)
                            {
                                player_car->_hit = gp::CAR_HIT_TIME;
                                player_car->_state = gp::CAR_STATE_HIT;
                            }
                            break;
                        case gp::OBJ_ROADBLOCK:
                            if(player_car->_state==gp::CAR_STATE_NORMAL && player_car->_inv==-1)
                            {
                                player_car->_hit = gp::CAR_HIT_TIME;
                                player_car->_state = gp::CAR_STATE_HIT2;
                            }
                            break;
                        case gp::OBJ_COIN:
                            it = objects->erase(it);
                            end = objects->end();

                            signs[index] = false;
                            
                            auto s_it = signs_obj.begin();
                            auto s_end = signs_obj.end();
                            int s_index = 0;
                            while(s_it != s_end)
                            {
                                if(s_index==index)
                                {
                                    s_it = signs_obj.erase(s_it);
                                    s_end = signs_obj.end();
                                    break;
                                }
                                else
                                {
                                    s_it++;
                                    s_index++;
                                }
                            }
                            break;
                    }
                }

                it++;
                index++;
            }

            player.update();
            if(road_angle<-5) player_car->set_x(player_car->x() + 0.75);
            if(road_angle>5) player_car->set_x(player_car->x() - 0.75);
            
            //----HUD stuff----

            //MPH text
            text_mph_sprites.clear();
            bn::string<16> text;
            text = "MPH " + bn::to_string<16>((int)player_car->speed() * 7);
            text_mph.generate(-88, 70, text, text_mph_sprites);

            //Player icon
            bn::fixed icony = (player_car->distance() / track_length) * (16 * (bar.max_size()-1));
            plr_icon.set_y(gp::PLR_ICON_Y-icony);

            //Lap counter
            lap_counter_sprites.clear();
            bn::string<16> lap_text;
            lap_text = bn::to_string<3>(lap+1) + "/3";
            lap_counter.generate(88, 70, lap_text, lap_counter_sprites);

            bn::core::update();
        }
    }
}