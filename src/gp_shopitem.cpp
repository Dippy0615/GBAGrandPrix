#include "bn_sprite_ptr.h"
#include "bn_string_view.h"

#include "gp_shopitem.h"

namespace gp
{
    ShopItem::ShopItem(bn::string_view name, int type, bn::sprite_ptr sprite) : _name(name), _type(type), _sprite(sprite)
    {
        _sprite.set_blending_enabled(true);
    }

    bn::string_view ShopItem::name()
    {
        return _name;
    }
}