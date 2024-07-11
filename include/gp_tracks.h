#ifndef GP_TRACKS_H
#define GP_TRACKS_H

#include "bn_vector.h"

#include "gp_tracksegment.h"

namespace gp
{
    bn::vector<TrackSegment, 32> get_track(int level);
}

#endif