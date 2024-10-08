#include "bn_sprite_ptr.h"
#include "bn_sprite_tiles_ptr.h"
#include "bn_rect.h"
#include "bn_log.h"
#include "bn_fixed.h"
#include "bn_optional.h"
#include "bn_sprite_items_spr_coin.h"
#include "bn_sprite_items_spr_boosterpad.h"

#include "gp_trackobject.h"
#include "gp_constants.h"
#include "gp_sign.h"

namespace gp
{
    TrackObject::TrackObject(int type, int position, bn::sprite_ptr sprite) : 
    _type(type), _position(position), _sprite(sprite)
    {
        _sprite.set_bg_priority(0);
        _sprite.set_z_order(1);
        _coin_frame = 0;
    }

    int TrackObject::type()
    {
        return _type;
    }

    int TrackObject::position()
    {
        return _position;
    }

    bn::rect TrackObject::get_rect()
    {
        switch(_type)
        {
            default:
                return bn::rect((int)_sprite.x(), (int)_sprite.y(), 0, 0);
                break;
            case gp::OBJ_MUDSLICK: case gp::OBJ_COIN: case gp::OBJ_LEAFPILE:
                return bn::rect((int)_sprite.x(), (int)_sprite.y(), 16, 16);
                break;
            case gp::OBJ_FINISHLINE: case gp::OBJ_MUD: case gp::OBJ_MUD_BOTTOM: case gp::OBJ_BOOSTERPAD:
                return bn::rect((int)_sprite.x(), (int)_sprite.y(), 32, 32);
                break;
            case gp::OBJ_ROADBLOCK:
                return bn::rect((int)_sprite.x(), (int)_sprite.y(), 30, 12);
                break;
        }
    }

    void TrackObject::animate(int frame)
    {
        if(_type!=gp::OBJ_COIN && _type!=gp::OBJ_BOOSTERPAD) return;
        
        switch(_type)
        {
            default: break;
            case gp::OBJ_COIN:
                frame /= 4;
                _sprite.set_tiles(bn::sprite_items::spr_coin.tiles_item().create_tiles(frame % 4));
                break;
            case gp::OBJ_BOOSTERPAD:
                frame /= 3;
                _sprite.set_tiles(bn::sprite_items::spr_boosterpad.tiles_item().create_tiles(frame % 16));
                break;
        }
        
    }
}