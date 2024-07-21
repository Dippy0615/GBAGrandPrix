#ifndef GP_TRACKS_H
#define GP_TRACKS_H

#include "bn_vector.h"

#include "gp_tracksegment.h"
#include "gp_trackobject.h"

namespace gp
{
    void make_finishline(TrackSegment* segment);
    void make_mud(TrackSegment* segment, int x, int dist, int length);
    bn::vector<TrackSegment, 16> get_track(int level);
}

#endif