#include "bn_core.h"
#include "bn_fixed.h"
#include "bn_keypad.h"
#include "bn_affine_bg_ptr.h"
#include "bn_affine_bg_items_bg_road.h"
#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_items_bg_grass.h"
#include "bn_regular_bg_items_bg_jungle.h"
#include "bn_regular_bg_items_bg_sky.h"
#include "bn_regular_bg_items_bg_space.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_items_spr_car.h"
#include "bn_sprite_items_spr_car_pink.h"
#include "bn_sprite_items_spr_truck.h"
#include "bn_sprite_items_spr_kart.h"
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
#include "bn_optional.h"
#include "bn_sram.h"

#include "common_variable_8x16_sprite_font.h"

#include "gp_scene.h"
#include "gp_ingame.h"
#include "gp_postgame.h"
#include "gp_car.h"
#include "gp_player.h"
#include "gp_tracksegment.h"
#include "gp_utils.h"
#include "gp_constants.h"
#include "gp_trackobject.h"
#include "gp_tracks.h"
#include "gp_globals.h"

namespace gp
{
    Ingame::Ingame(){}

    void Ingame::fade_out()
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
    
    Scene Ingame::execute(int level)
    {
        //Setup the road & background
        bn::regular_bg_ptr bg = bn::regular_bg_items::bg_grass.create_bg(0, 0);
        
        switch(level)
        {
            default: break;
            case 1: bg.set_item(bn::regular_bg_items::bg_jungle); break;
            case 2: bg.set_item(bn::regular_bg_items::bg_sky); break;
            case 3: bg.set_item(bn::regular_bg_items::bg_space); break;
        }

        bn::affine_bg_ptr road = bn::affine_bg_items::bg_road.create_bg(0, 0);
        bg.set_priority(1);
        bg.set_blending_enabled(true);
        road.set_priority(0);
        road.set_blending_enabled(true);
        bn::fixed road_angle = 0;

        //Setup the cars
        bn::sprite_ptr car_sprite = bn::sprite_items::spr_car.create_sprite(0,24);
        switch(current_car)
        {
            case gp::TECHNO_CAR_ID: default: car_sprite.set_scale(0.75); break;
            case gp::CYBER_TRUCK_ID: car_sprite.set_item(bn::sprite_items::spr_truck); car_sprite.set_scale(0.95); break;
            case gp::ELECTRO_MOBILE_ID: car_sprite.set_item(bn::sprite_items::spr_car_pink); car_sprite.set_scale(0.80); break;
        }
        Car car = Car(car_sprite, current_car);
        Player player = Player(car);
        Car* player_car = player.get_car();

        //Setup the track        
        bn::vector<TrackSegment, gp::SEGMENTS_MAX> segments =  gp::get_track(level);
        int track_length = segments[segments.size()-1].end();

        int current_segment_index = 0;
        TrackSegment* current_segment = &segments[current_segment_index];
        
        //Setup the objects
        bn::vector<TrackObject, gp::SEGMENT_OBJ_MAX>* objects = current_segment->get_objects();
        
        //Setup the signs
        bn::array<bool, gp::SEGMENT_OBJ_MAX> signs;
        signs.fill(false);
        bn::vector<Sign, gp::SEGMENT_OBJ_MAX> signs_obj;

        //Setup the text
        bn::sprite_text_generator text_mph(common::variable_8x16_sprite_font);
        text_mph.set_center_alignment();
        text_mph.set_bg_priority(0);
        bn::vector<bn::sprite_ptr, 8> text_mph_sprites;

        bn::sprite_text_generator text_finish(common::variable_8x16_sprite_font);
        text_finish.set_center_alignment();
        text_finish.set_bg_priority(0);
        bn::vector<bn::sprite_ptr, 12> text_finish_sprites;

        //Setup the progress bar
        bn::vector<bn::sprite_ptr, 4> bar;
        for(int i=0;i<bar.max_size();i++)
        {
            bn::sprite_ptr test = bn::sprite_items::spr_bar_mid.create_sprite(gp::BAR_X,gp::BAR_Y+(i*16));
            test.set_bg_priority(0);
            test.set_blending_enabled(true);
            bar.push_back(test);
        }
        bn::sprite_ptr plr_icon = bn::sprite_items::spr_plr_icon.create_sprite(gp::BAR_X,gp::PLR_ICON_Y);
        plr_icon.set_bg_priority(0);
        plr_icon.set_blending_enabled(true);


        //Setup the lap counter
        bn::sprite_text_generator lap_counter(common::variable_8x16_sprite_font);
        lap_counter.set_center_alignment();
        lap_counter.set_bg_priority(0);
        bn::vector<bn::sprite_ptr, 5> lap_counter_sprites;

        int time = 0;
        int lap = 0;
        bool finish = false;
        int finish_time = 0;

        int milliseconds = 0;
        int seconds = 0;
        int minutes = 0;
        bn::fixed distance = 0;

        //fade in
        bn::blending::set_fade_alpha(1);
        bn::blending::set_fade_color(bn::blending::fade_color_type::WHITE);
        _fade_action = bn::blending_fade_alpha_to_action(30, 0);
        while(!_fade_action.value().done()){
            _fade_action.value().update();
            for(int i=0;i<2;i++)
                bn::core::update();

        }
        bn::blending::set_fade_alpha(0);

        while(true)
        {
            time++;
            if(!finish)
            {
                milliseconds++;
                if(milliseconds>59)
                {
                    milliseconds = 0;
                    seconds++;
                }
                if(seconds>59)
                {
                    seconds = 0;
                    minutes++;
                }
            }

            if(finish)
            {
                finish_time++;
                if(finish_time==160)
                {
                    text_finish_sprites.clear();
                    bn::string<16> text2;
                    text2 = bn::to_string<16>(minutes) +" : ";
                    text2 += bn::to_string<16>(seconds)+" : ";
                    text2 += bn::to_string<16>((int)(milliseconds/bn::fixed(.6)));
                    text_finish.generate(0, 0, text2, text_finish_sprites);
                    for(bn::sprite_ptr spr: text_finish_sprites){
                         spr.set_blending_enabled(true);
                    }
                }
                if(finish_time>400)
                {
                    gp::Score score = scores[level];
                    gp::Score new_score = gp::Score(level, milliseconds, seconds, minutes);
                    if(score.is_empty() || score<new_score) scores[level] = new_score;

                    fade_out();
                    return gp::Scene::Postgame;
                }
            }

            //Move to the next segment
            if (distance>current_segment->end())
            {
                //Move objects offscreen
                for(auto it = objects->begin(), end = objects->end(); it != end; )
                {
                    TrackObject object = *it;
                    object._sprite.set_y(object._sprite.y() + 200);
                    it++;
                }

                current_segment_index++;
                if(current_segment_index>=segments.size()) //Lap
                {
                    if(lap<2) lap++;
                    else 
                    {
                        finish = true;
                        player._finish = true;
                        text_finish.generate(0, 0, "FINISH!", text_finish_sprites);
                        for(bn::sprite_ptr spr: text_finish_sprites){
                            spr.set_blending_enabled(true);
                        }
                    }
                    current_segment_index = 0;
                    distance = 0;
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
            int cols = 0;

            if(!finish)
            {
                for(auto it = objects->begin(), end = objects->end(); it != end; )
                {
                    if(index>=objects->size()) break;

                    TrackObject object = *it;
                    type = object.type();
                    bn::fixed ypos = distance - object.position();
                    object._sprite.set_y(ypos);

                    //Handle sign
                    if(!signs[index])
                    {
                        signs[index] = true;
                        bn::sprite_ptr sign_sprite = bn::sprite_items::spr_warningsign.create_sprite(object._sprite.x(), -64);
                        sign_sprite.set_scale(0.75);
                        if(signs_obj.size()<gp::SEGMENT_OBJ_MAX)
                        {
                            sign_sprite.set_visible(false);
                            signs_obj.push_back(Sign(sign_sprite));
                        }
                    }
                    else
                    {
                        if(type==gp::OBJ_FINISHLINE || type==gp::OBJ_COIN || type==gp::OBJ_BOOSTERPAD)
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
                    
                    //Animate if necessary
                    object.animate(time);
                    
                    //Collision
                    if(player_car->get_rect().intersects(object.get_rect()))
                    {
                        cols++;
                        switch(type)
                        {
                            default:
                                break;
                            case gp::OBJ_MUDSLICK: case gp::OBJ_LEAFPILE:
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
                            {
                                coins++;
                                it = objects->erase(it);
                                end = objects->end();

                                signs[index] = false;
                                auto s_it = signs_obj.begin();
                                s_it += index;
                                signs_obj.erase(s_it);
                                break;
                            }
                            case gp::OBJ_MUD: case gp::OBJ_MUD_BOTTOM:
                                player_car->_mud = true;
                                break;
                            case gp::OBJ_BOOSTERPAD:
                                if(player_car->speed()<gp::CAR_BOOSTER_SPEED)
                                {
                                    player_car->accelerate(gp::CAR_BOOSTER_SPEED);
                                }
                                break;
                        }
                    }
                    if(cols==0)
                    {
                        player_car->_mud = false;
                    }

                    it++;
                    index++;
                }
            }

            player.update();
            if(!finish) distance += (player_car->speed() / 5);
            if(road_angle<-5) player_car->set_x(player_car->x() + 0.75);
            if(road_angle>5) player_car->set_x(player_car->x() - 0.75);
            
            //----HUD stuff----

            //MPH text
            text_mph_sprites.clear();
            bn::string<8> text;
            text = "MPH " + bn::to_string<16>((int)player_car->speed() * 7);
            text_mph.generate(-88, 70, text, text_mph_sprites);
            for(bn::sprite_ptr spr: text_mph_sprites){
                spr.set_blending_enabled(true);
            }

            //Player icon
            bn::fixed icony = (distance / track_length) * (16 * (bar.max_size()-1));
            plr_icon.set_y(gp::PLR_ICON_Y-icony);

            //Lap counter
            lap_counter_sprites.clear();
            bn::string<4> lap_text;
            lap_text = bn::to_string<3>(lap+1) + "/3";
            lap_counter.generate(88, 70, lap_text, lap_counter_sprites);
            for(bn::sprite_ptr spr: lap_counter_sprites){
                spr.set_blending_enabled(true);
            }

            bn::core::update();
        }
    }
}