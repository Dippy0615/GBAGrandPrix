#ifndef GP_TRACKS_H
#define GP_TRACKS_H

#include "bn_vector.h"

#include "gp_tracksegment.h"
#include "gp_trackobject.h"

namespace gp
{
    void make_finishline(TrackSegment* segment);
    bn::vector<TrackSegment, 32> get_track(int level);
}

#endif