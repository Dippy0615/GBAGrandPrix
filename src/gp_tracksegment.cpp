#include "gp_tracksegment.h"

namespace gp
{
    TrackSegment::TrackSegment(int pos, int len, int curve) : _position(pos), _length(len), _curve(curve)
    {

    }

    int TrackSegment::position()
    {
        return _position;
    }

    int TrackSegment::length()
    {
        return _length;
    }

    int TrackSegment::curve()
    {
        return _curve;
    }

    int TrackSegment::end()
    {
        return _position + _length;
    }
}