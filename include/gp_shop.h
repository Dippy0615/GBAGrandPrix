#ifndef GP_SHOP_H
#define GP_SHOP_H

#include "bn_blending_actions.h"
#include "bn_optional.h"
#include "bn_vector.h"

#include "gp_shopitem.h"
#include "gp_scene.h"

namespace gp
{
    class Shop
    {
        private:
        public:
            Shop();
            gp::Scene execute();
            bn::optional<bn::blending_fade_alpha_to_action> _fade_action;
            void fade_out();
            void check_shop_items(bn::vector<gp::ShopItem, 16>* items);
    };
}

#endif