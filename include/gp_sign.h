#ifndef GP_SIGN_H
#define GP_SIGN_H

#include "bn_sprite_ptr.h"

namespace gp
{
    class Sign
    {
        private:
            int _type;
            int _flash_counter;
        public:
            bn::sprite_ptr _sprite;
            Sign(bn::sprite_ptr sprite);
            void flash();
    };
}

#endif