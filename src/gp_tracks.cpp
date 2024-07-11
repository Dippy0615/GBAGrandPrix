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
    bn::vector<TrackSegment, 32> get_track(int level)
    {
        bn::vector<TrackSegment, 32> segments;
        switch(level)
        {
            case 0: default:
                for(int i=0; i<2; i++)
                {
                    TrackSegment segment = TrackSegment(1000*i, 1000, i==1 ? 30 : 0);
                    if(i==0)
                    {
                        for(int j=0;j<5;j++)
                        {
                            bn::sprite_ptr finishline_sprite = bn::sprite_items::spr_finishline.create_sprite(-64+(j*32), gp::FINISHLINE_POSITION);
                            TrackObject finishline_part = TrackObject(gp::OBJ_FINISHLINE, gp::FINISHLINE_POSITION, finishline_sprite);
                            segment.add_object(finishline_part);
                        }

                        bn::sprite_ptr mud_sprite = bn::sprite_items::spr_roadblock.create_sprite(-64, -256);
                        TrackObject mud = TrackObject(gp::OBJ_ROADBLOCK, 500, mud_sprite);
                        segment.add_object(mud);
                    }
                    segments.push_back(segment);
                }
                return segments;
                break;
        }
    }
}