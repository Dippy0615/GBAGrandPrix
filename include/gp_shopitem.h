#ifndef GP_SHOPTIEM_H
#define GP_SHOPITEM_H

#include "bn_sprite_ptr.h"
#include "bn_string_view.h"

namespace gp
{
    class ShopItem
    {
        private:
            bn::string_view _name;
            int _type;
            bn::sprite_ptr _sprite;
        public:
            ShopItem(bn::string_view name, int type, bn::sprite_ptr sprite);
            bool _active;
            bn::string_view name();
    };
}

#endif