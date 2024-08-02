#include "bn_sprite_items_spr_mudslick.h"
#include "bn_sprite_items_spr_finishline.h"
#include "bn_sprite_items_spr_roadblock.h"
#include "bn_sprite_items_spr_coin.h"
#include "bn_sprite_items_spr_mud.h"
#include "bn_sprite_items_spr_mud_bottom.h"
#include "bn_sprite_items_spr_leafpile.h"
#include "bn_sprite_items_spr_boosterpad.h"
#include "bn_sprite_ptr.h"
#include "bn_vector.h"
#include "bn_log.h"

#include "gp_tracks.h"
#include "gp_constants.h"
#include "gp_tracksegment.h"
#include "gp_trackobject.h"

namespace gp
{
    void make_finishline(TrackSegment* segment)
    {
        for(int j=0;j<5;j++)
        {
            bn::sprite_ptr finishline_sprite = bn::sprite_items::spr_finishline.create_sprite(-64+(j*32), gp::FINISHLINE_POSITION);
            TrackObject finishline_part = TrackObject(gp::OBJ_FINISHLINE, gp::FINISHLINE_POSITION, finishline_sprite);
            segment->add_object(finishline_part);
        }
    }

    void make_mud(TrackSegment* segment, int x, int dist, int length)
    {
        for(int i=0; i<length; i++)
        {
            bn::sprite_ptr mud_sprite = bn::sprite_items::spr_mud.create_sprite(x, dist);
            if(i==0) mud_sprite.set_item(bn::sprite_items::spr_mud_bottom);
            TrackObject mud = TrackObject(i==0 ? gp::OBJ_MUD_BOTTOM : gp::OBJ_MUD, dist+(i*32), mud_sprite);
            segment->add_object(mud);
        }
    }

    bn::vector<TrackSegment, gp::SEGMENTS_MAX> get_track(int level)
    {
        bn::vector<TrackSegment, gp::SEGMENTS_MAX> segments;
        switch(level)
        {
            case 0: default:
            {
                //Ground
                TrackSegment segment1 = TrackSegment(0, 1000, 0);
                make_finishline(&segment1);
                segments.push_back(segment1);
                
                TrackSegment segment2 = TrackSegment(1000, 1000, -30);
                segments.push_back(segment2);

                TrackSegment segment3 = TrackSegment(2000, 2000, 0);
                segment3.add_object(TrackObject(gp::OBJ_MUDSLICK, 2400, bn::sprite_items::spr_mudslick.create_sprite(0, 2400)));
                segment3.add_object(TrackObject(gp::OBJ_BOOSTERPAD, 3200, bn::sprite_items::spr_boosterpad.create_sprite(48, 2700)));
                segment3.add_object(TrackObject(gp::OBJ_COIN, 3000, bn::sprite_items::spr_coin.create_sprite(-48, 2950)));
                segment3.add_object(TrackObject(gp::OBJ_ROADBLOCK, 3550, bn::sprite_items::spr_roadblock.create_sprite(-48, 3100)));
                segments.push_back(segment3);

                TrackSegment segment4 = TrackSegment(4000, 1000, -30);
                segments.push_back(segment4);

                TrackSegment segment5 = TrackSegment(5000, 1000, 0);
                make_mud(&segment5, 32, 5550, 9);
                make_mud(&segment5, 64, 5550, 9);
                segment5.add_object(TrackObject(gp::OBJ_COIN, 5450, bn::sprite_items::spr_coin.create_sprite(48, 5750)));
                segments.push_back(segment5);

                TrackSegment segment6 = TrackSegment(6000, 1000, 30);
                segments.push_back(segment6);

                TrackSegment segment7 = TrackSegment(7000, 1000, 0);
                segment7.add_object(TrackObject(gp::OBJ_BOOSTERPAD, 7350, bn::sprite_items::spr_boosterpad.create_sprite(-48, 7150)));
                segment7.add_object(TrackObject(gp::OBJ_ROADBLOCK, 7600, bn::sprite_items::spr_roadblock.create_sprite(24, 7600)));
                segments.push_back(segment7);
                return segments;
            }
            case 1:
            {
                //Jungle
                TrackSegment segment1 = TrackSegment(0, 1000, 0);
                make_finishline(&segment1);
                segment1.add_object(TrackObject(gp::OBJ_LEAFPILE, 700, bn::sprite_items::spr_leafpile.create_sprite(-48, 700)));
                segment1.add_object(TrackObject(gp::OBJ_LEAFPILE, 700, bn::sprite_items::spr_leafpile.create_sprite(-32, 700)));
                segments.push_back(segment1);

                TrackSegment segment2 = TrackSegment(1000, 1000, 0);
                make_mud(&segment2, -44, 1330, 8);
                segment2.add_object(TrackObject(gp::OBJ_LEAFPILE, 1750, bn::sprite_items::spr_leafpile.create_sprite(48, 1650)));
                segment2.add_object(TrackObject(gp::OBJ_LEAFPILE, 1750, bn::sprite_items::spr_leafpile.create_sprite(32, 1620)));
                segments.push_back(segment2);

                TrackSegment segment3 = TrackSegment(2000, 500, 30);
                segments.push_back(segment3);

                TrackSegment segment4 = TrackSegment(2500, 1000, 0);
                make_mud(&segment4, 48, 2800, 5);
                segment4.add_object(TrackObject(gp::OBJ_COIN, 2740, bn::sprite_items::spr_coin.create_sprite(48, 2740)));
                segments.push_back(segment4);

                TrackSegment segment5 = TrackSegment(3500, 500, 30);
                make_mud(&segment5, 32, 5550, 10);
                make_mud(&segment5, 64, 5550, 10);
                segments.push_back(segment5);

                TrackSegment segment6 = TrackSegment(4000, 500, 0);
                make_mud(&segment6, -48, 4200, 6);
                segments.push_back(segment6);

                TrackSegment segment7 = TrackSegment(4500, 1000, 0);
                segment7.add_object(TrackObject(gp::OBJ_ROADBLOCK, 5250, bn::sprite_items::spr_roadblock.create_sprite(32, 5250)));
                segment7.add_object(TrackObject(gp::OBJ_ROADBLOCK, 450, bn::sprite_items::spr_roadblock.create_sprite(-32, 5250)));
                segments.push_back(segment7);

                TrackSegment segment8 = TrackSegment(5500, 550, -30);
                segments.push_back(segment8);

                TrackSegment segment9 = TrackSegment(6000, 800, 0);
                segment9.add_object(TrackObject(gp::OBJ_ROADBLOCK, 6300, bn::sprite_items::spr_roadblock.create_sprite(48, 6300)));
                segment9.add_object(TrackObject(gp::OBJ_COIN, 6430, bn::sprite_items::spr_coin.create_sprite(48, 6430)));
                segments.push_back(segment9);
                return segments;
            }
            case 2:
            {
                //Sky
                TrackSegment segment1 = TrackSegment(0, 1000, 0);
                make_finishline(&segment1);
                
                segments.push_back(segment1);

                TrackSegment segment2 = TrackSegment(1000, 1000, 30);
                segments.push_back(segment2);

                TrackSegment segment3 = TrackSegment(2000, 2000, 0);
                make_mud(&segment3, 48, 2900, 8);
                make_mud(&segment3, 64, 2900, 8);
                segment3.add_object(TrackObject(gp::OBJ_ROADBLOCK, 2200, bn::sprite_items::spr_roadblock.create_sprite(-48, 2200)));
                segment3.add_object(TrackObject(gp::OBJ_COIN, 2850, bn::sprite_items::spr_coin.create_sprite(48, 2850)));
                segments.push_back(segment3);

                TrackSegment segment4 = TrackSegment(4000, 1000, -30);
                segments.push_back(segment4);

                TrackSegment segment5 = TrackSegment(5000, 1000, 0);
                segment5.add_object(TrackObject(gp::OBJ_COIN, 5450, bn::sprite_items::spr_coin.create_sprite(48, 5450)));
                segment5.add_object(TrackObject(gp::OBJ_ROADBLOCK, 5600, bn::sprite_items::spr_roadblock.create_sprite(48, 5600)));
                segment5.add_object(TrackObject(gp::OBJ_ROADBLOCK, 5850, bn::sprite_items::spr_roadblock.create_sprite(-48, 5850)));
                segments.push_back(segment5);

                TrackSegment segment6 = TrackSegment(6000, 1000, 0);
                make_mud(&segment5, -32, 6100, 8);
                make_mud(&segment5, -64, 6100, 8);
                return segments;
            }
            case 3:
            {
                
                TrackSegment segment1 = TrackSegment(0, 500, 0);
                make_finishline(&segment1);
                segments.push_back(segment1);

                TrackSegment segment2 = TrackSegment(500, 500, -30);
                segments.push_back(segment2);

                TrackSegment segment3 = TrackSegment(1000, 1000, 0);
                segment3.add_object(TrackObject(gp::OBJ_ROADBLOCK, 1200, bn::sprite_items::spr_roadblock.create_sprite(-48, 1200)));
                segment3.add_object(TrackObject(gp::OBJ_ROADBLOCK, 1400, bn::sprite_items::spr_roadblock.create_sprite(48, 1400)));
                make_mud(&segment3, 48, 1550, 6);
                segments.push_back(segment3);

                TrackSegment segment4 = TrackSegment(2000, 500, -30);
                segments.push_back(segment4);

                TrackSegment segment5 = TrackSegment(2500, 2000, 0);
                make_mud(&segment5, 48, 2700, 6);
                segment5.add_object(TrackObject(gp::OBJ_COIN, 3200, bn::sprite_items::spr_coin.create_sprite(48, 3100)));
                segment5.add_object(TrackObject(gp::OBJ_ROADBLOCK, 3070, bn::sprite_items::spr_roadblock.create_sprite(-48, 3050)));
                segment5.add_object(TrackObject(gp::OBJ_BOOSTERPAD, 3500, bn::sprite_items::spr_boosterpad.create_sprite(48, 3400)));
                segment5.add_object(TrackObject(gp::OBJ_ROADBLOCK, 4300, bn::sprite_items::spr_roadblock.create_sprite(-48, 4300)));
                segments.push_back(segment5);

                TrackSegment segment6 = TrackSegment(4500, 500, 30);
                segments.push_back(segment6);
                
                TrackSegment segment7 = TrackSegment(5000, 1000, 0);
                segment7.add_object(TrackObject(gp::OBJ_ROADBLOCK, 5275, bn::sprite_items::spr_roadblock.create_sprite(-48, 5275)));
                segment7.add_object(TrackObject(gp::OBJ_MUDSLICK, 5700, bn::sprite_items::spr_mudslick.create_sprite(48, 5700)));
                segments.push_back(segment7);
                
                TrackSegment segment8 = TrackSegment(6000, 500, 30);
                segments.push_back(segment8);

                TrackSegment segment9 = TrackSegment(6500, 1500, 0);
                segment9.add_object(TrackObject(gp::OBJ_COIN, 6900, bn::sprite_items::spr_coin.create_sprite(-48, 6900)));
                segment9.add_object(TrackObject(gp::OBJ_ROADBLOCK, 7650, bn::sprite_items::spr_roadblock.create_sprite(48, 7650)));
                segments.push_back(segment9);
                
                return segments;
            }
        }
        return segments;
    }
}