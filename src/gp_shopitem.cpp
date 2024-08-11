#include "bn_sprite_ptr.h"
#include "bn_string_view.h"
#include "bn_camera_ptr.h"

#include "gp_shopitem.h"

namespace gp
{
    ShopItem::ShopItem(bn::string_view name, int type, int price, bn::sprite_ptr sprite, bn::camera_ptr cam) : _name(name), _type(type), _price(price), _sprite(sprite)
    {
        _sprite.set_blending_enabled(true);
        _sprite.set_camera(cam);
        _active = true;
    }

    int ShopItem::type()
    {
        return _type;
    }

    int ShopItem::price()
    {
        return _price;
    }

    bn::string_view ShopItem::name()
    {
        return _name;
    }
}