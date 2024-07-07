#include "bn_sprite_ptr.h"
#include "bn_rect.h"

#include "gp_trackobject.h"
#include "gp_constants.h"

namespace gp
{
    TrackObject::TrackObject(int type, int position, bn::sprite_ptr sprite) : 
    _type(type), _position(position), _sprite(sprite)
    {
        _sprite.set_bg_priority(0);
        _sprite.set_z_order(1);
        _rx = 0;
        _ry = 0;
        switch(_type)
        {
            default:
                break;
            case gp::OBJ_MUDSLICK:
                _rw = 16;
                _rh = 16;
                break;
            case gp::OBJ_FINISHLINE:
                _rw = 32;
                _rh = 32;
                break;
            case gp::OBJ_ROADBLOCK:
                _rw = 32;
                _rh = 12;
                break;
        }
        _rect.set_width(_rw);
        _rect.set_height(_rh);
    }

    int TrackObject::type()
    {
        return _type;
    }

    int TrackObject::position()
    {
        return _position;
    }

    void TrackObject::update()
    {
        _rect.set_x((int)_sprite.x() + _rx);
        _rect.set_y((int)_sprite.y() + _ry);
    }

    bn::rect TrackObject::get_rect()
    {
        return _rect;
    }
}