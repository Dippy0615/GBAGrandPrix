#include "bn_sprite_ptr.h"

#include "gp_trackobject.h"
#include "gp_constants.h"

namespace gp
{
    TrackObject::TrackObject(int type, int position, bn::sprite_ptr sprite) : _type(type), _position(position), _sprite(sprite)
    {
        _sprite.set_bg_priority(0);
        _sprite.set_z_order(1);
    }

    int TrackObject::type()
    {
        return _type;
    }

    int TrackObject::position()
    {
        return _position;
    }
}