#ifndef GP_SHOPTIEM_H
#define GP_SHOPTIEM_H

#include "bn_sprite_ptr.h"
#include "bn_string_view.h"
#include "bn_camera_ptr.h"

namespace gp
{
    class ShopItem
    {
        private:
            bn::string_view _name;
            int _type;
            bn::sprite_ptr _sprite;
            int _price;
        public:
            ShopItem(bn::string_view name, int type, int price, bn::sprite_ptr sprite, bn::camera_ptr cam);
            bool _active;
            bn::string_view name();
            int type();
            int price();
    };
}

#endif