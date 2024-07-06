#ifndef GP_TRACKSEGMENT_H
#define GP_TRACKSEGMENT_H

#include "bn_fixed.h"
#include "bn_vector.h"

#include "gp_trackobject.h"

namespace gp
{
    class TrackSegment
    {
        private:
            int _position;
            int _length;
            int _curve;
            bn::vector<TrackObject, 16> _objects;
        public:
            TrackSegment(int pos, int len, int curve);
            int position();
            int length();
            int end();
            int curve();
            void add_object(TrackObject obj);
            bn::vector<TrackObject, 16>* get_objects();
    };
}

#endif