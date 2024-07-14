#include "bn_sprite_ptr.h"
#include "bn_sprite_items_spr_warningsign.h"

#include "gp_sign.h"
#include "gp_constants.h"

namespace gp
{
    Sign::Sign(bn::sprite_ptr sprite) : _sprite(sprite)
    {
        _sprite.set_bg_priority(0);
        _sprite.set_z_order(1);
        _flash_counter = 0;
    }

    void Sign::flash()
    {
        _flash_counter++;
        if(_flash_counter%10==0) _sprite.set_visible(!_sprite.visible());
    }
}