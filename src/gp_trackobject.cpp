#include "bn_sprite_ptr.h"
#include "bn_rect.h"
#include "bn_log.h"

#include "gp_trackobject.h"
#include "gp_constants.h"

namespace gp
{
    TrackObject::TrackObject(int type, int position, bn::sprite_ptr sprite) : 
    _type(type), _position(position), _sprite(sprite)
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

    void TrackObject::update()
    {
    }

    bn::rect TrackObject::get_rect()
    {
        switch(_type)
        {
            default:
                return bn::rect((int)_sprite.x(), (int)_sprite.y(), 0, 0);
                break;
            case gp::OBJ_MUDSLICK:
                return bn::rect((int)_sprite.x(), (int)_sprite.y(), 16, 16);
                break;
            case gp::OBJ_FINISHLINE:
                return bn::rect((int)_sprite.x(), (int)_sprite.y(), 32, 32);
                break;
            case gp::OBJ_ROADBLOCK:
                return bn::rect((int)_sprite.x(), (int)_sprite.y(), 30, 12);
                break;
        }
    }
}