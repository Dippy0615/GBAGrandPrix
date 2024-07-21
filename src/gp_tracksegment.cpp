#include "bn_vector.h"

#include "gp_tracksegment.h"
#include "gp_trackobject.h"

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

    void TrackSegment::add_object(TrackObject obj)
    {
        _objects.push_back(obj);
    }

    bn::vector<TrackObject, gp::SEGMENT_OBJ_MAX>* TrackSegment::get_objects()
    {
        return &_objects;
    }
}