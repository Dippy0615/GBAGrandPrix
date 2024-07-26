#include "bn_sprite_items_spr_mudslick.h"
#include "bn_sprite_items_spr_finishline.h"
#include "bn_sprite_items_spr_roadblock.h"
#include "bn_sprite_items_spr_coin.h"
#include "bn_sprite_items_spr_mud.h"
#include "bn_sprite_items_spr_mud_bottom.h"
#include "bn_sprite_items_spr_leafpile.h"
#include "bn_sprite_ptr.h"
#include "bn_vector.h"

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

    bn::vector<TrackSegment, 16> get_track(int level)
    {
        bn::vector<TrackSegment, 16> segments;
        switch(level)
        {
            case 0: default:
            {
                //Ground
                TrackSegment segment1 = TrackSegment(0, 1000, 0);
                make_finishline(&segment1);
                TrackObject block = TrackObject(gp::OBJ_ROADBLOCK, 500, bn::sprite_items::spr_roadblock.create_sprite(64, 500));
                segments.push_back(segment1);
                
                TrackSegment segment2 = TrackSegment(1000, 1000, -30);
                segments.push_back(segment2);

                TrackSegment segment3 = TrackSegment(2000, 2000, 0);
                TrackObject mud = TrackObject(gp::OBJ_MUDSLICK, 2400, bn::sprite_items::spr_mudslick.create_sprite(0, 2400));
                TrackObject coin = TrackObject(gp::OBJ_COIN, 2850, bn::sprite_items::spr_coin.create_sprite(-48, 2700));
                TrackObject block2 = TrackObject(gp::OBJ_ROADBLOCK, 3100, bn::sprite_items::spr_roadblock.create_sprite(-48, 3100));
                TrackObject mud2 = TrackObject(gp::OBJ_MUDSLICK, 3650, bn::sprite_items::spr_mudslick.create_sprite(42, 3650));
                
                segment3.add_object(mud);
                segment3.add_object(coin);
                segment3.add_object(block2);
                segment3.add_object(mud2);
                segments.push_back(segment3);

                TrackSegment segment4 = TrackSegment(4000, 1000, -30);
                segments.push_back(segment4);

                TrackSegment segment5 = TrackSegment(5000, 1000, 0);
                make_mud(&segment5, 32, 5550, 10);
                make_mud(&segment5, 64, 5550, 10);
                
                segments.push_back(segment5);

                TrackSegment segment6 = TrackSegment(6000, 1000, 30);
                segments.push_back(segment6);

                TrackSegment segment7 = TrackSegment(7000, 1000, 0);
                TrackObject block3 = TrackObject(gp::OBJ_ROADBLOCK, 7600, bn::sprite_items::spr_roadblock.create_sprite(24, 7600));

                segment7.add_object(block3);
                segments.push_back(segment7);
                break;
            }
            case 1:
            {
                //Jungle
                TrackSegment segment1 = TrackSegment(0, 1000, 0);
                make_finishline(&segment1);
                TrackObject leaf3 = TrackObject(gp::OBJ_LEAFPILE, 700, bn::sprite_items::spr_leafpile.create_sprite(-48, 700));
                TrackObject leaf4 = TrackObject(gp::OBJ_LEAFPILE, 700, bn::sprite_items::spr_leafpile.create_sprite(-32, 700));
                segment1.add_object(leaf3);
                segment1.add_object(leaf4);
                segments.push_back(segment1);

                TrackSegment segment2 = TrackSegment(1000, 1000, 0);
                make_mud(&segment2, -44, 1330, 8);
                TrackObject leaf = TrackObject(gp::OBJ_LEAFPILE, 1750, bn::sprite_items::spr_leafpile.create_sprite(48, 1650));
                TrackObject leaf2 = TrackObject(gp::OBJ_LEAFPILE, 1750, bn::sprite_items::spr_leafpile.create_sprite(32, 1620));
                segment2.add_object(leaf);
                segment2.add_object(leaf2);
                segments.push_back(segment2);

                TrackSegment segment3 = TrackSegment(2000, 500, 30);
                segments.push_back(segment3);

                TrackSegment segment4 = TrackSegment(2500, 1000, 0);
                TrackObject coin = TrackObject(gp::OBJ_COIN, 2740, bn::sprite_items::spr_coin.create_sprite(48, 2740));
                make_mud(&segment4, 48, 2800, 5);
                segment4.add_object(coin);
                segments.push_back(segment4);

                TrackSegment segment5 = TrackSegment(3500, 500, 30);
                segments.push_back(segment5);

                TrackSegment segment6 = TrackSegment(4000, 500, 0);
                make_mud(&segment6, -48, 4200, 6);
                segments.push_back(segment6);

                TrackSegment segment7 = TrackSegment(4500, 1000, 0);
                TrackObject block = TrackObject(gp::OBJ_ROADBLOCK, 5250, bn::sprite_items::spr_roadblock.create_sprite(32, 5250));
                TrackObject block3 = TrackObject(gp::OBJ_ROADBLOCK, 450, bn::sprite_items::spr_roadblock.create_sprite(-32, 5250));
                segment7.add_object(block);
                segment7.add_object(block3);
                segments.push_back(segment7);

                TrackSegment segment8 = TrackSegment(5500, 550, -30);
                segments.push_back(segment8);

                TrackSegment segment9 = TrackSegment(6000, 800, 0);
                TrackObject block2 = TrackObject(gp::OBJ_ROADBLOCK, 6300, bn::sprite_items::spr_roadblock.create_sprite(48, 6300));
                TrackObject coin2 = TrackObject(gp::OBJ_COIN, 6430, bn::sprite_items::spr_coin.create_sprite(48, 6430));
                segment9.add_object(block2);
                segment9.add_object(coin2);
                segments.push_back(segment9);
                break;
            }
        }
        return segments;
    }
}