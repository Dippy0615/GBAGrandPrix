#ifndef GP_TRACKSEGMENT_H
#define GP_TRACKSEGMENT_H

#include "bn_fixed.h"

namespace gp
{
    class TrackSegment
    {
        private:
            int _position;
            int _length;
            int _curve;
        public:
            TrackSegment(int pos, int len, int curve);
            int position();
            int length();
            int end();
            int curve();
    };
}

#endif