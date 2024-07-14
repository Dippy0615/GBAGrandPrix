#include "bn_sprite_items_spr_mudslick.h"
#include "bn_sprite_items_spr_finishline.h"
#include "bn_sprite_items_spr_roadblock.h"
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

    bn::vector<TrackSegment, 16> get_track(int level)
    {
        bn::vector<TrackSegment, 16> segments;
        switch(level)
        {
            case 0: default:
                TrackSegment segment1 = TrackSegment(0, 1000, 0);
                make_finishline(&segment1);
                TrackObject block = TrackObject(gp::OBJ_ROADBLOCK, 500, bn::sprite_items::spr_roadblock.create_sprite(64, 500));
                segment1.add_object(block);
                segments.push_back(segment1);
                
                TrackSegment segment2 = TrackSegment(1000, 1000, -30);
                segments.push_back(segment2);

                TrackSegment segment3 = TrackSegment(2000, 2000, 0);
                TrackObject mud = TrackObject(gp::OBJ_MUDSLICK, 2400, bn::sprite_items::spr_mudslick.create_sprite(0, 2400));
                TrackObject block2 = TrackObject(gp::OBJ_ROADBLOCK, 3100, bn::sprite_items::spr_roadblock.create_sprite(-48, 3100));
                TrackObject mud2 = TrackObject(gp::OBJ_MUDSLICK, 3650, bn::sprite_items::spr_mudslick.create_sprite(32, 3650));
                
                segment3.add_object(mud);
                segment3.add_object(block2);
                segment3.add_object(mud2);
                segments.push_back(segment3);

                TrackSegment segment4 = TrackSegment(4000, 1000, -30);
                segments.push_back(segment4);

                TrackSegment segment5 = TrackSegment(5000, 1000, 0);
                TrackObject mud3 = TrackObject(gp::OBJ_MUDSLICK, 5550, bn::sprite_items::spr_mudslick.create_sprite(0, 5250));
                
                segment5.add_object(mud3);
                segments.push_back(segment5);

                TrackSegment segment6 = TrackSegment(6000, 1000, 30);
                segments.push_back(segment6);

                TrackSegment segment7 = TrackSegment(7000, 1000, 0);
                TrackObject block3 = TrackObject(gp::OBJ_ROADBLOCK, 7600, bn::sprite_items::spr_roadblock.create_sprite(24, 7600));

                segment7.add_object(block3);
                segments.push_back(segment7);

                return segments;
                break;
        }
    }
}