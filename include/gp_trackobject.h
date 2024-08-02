#ifndef GP_TRACKOBJECT_H
#define GP_TRACKOBJECT_H

#include "bn_sprite_ptr.h"
#include "bn_rect.h"
#include "bn_fixed.h"

#include "gp_sign.h"

namespace gp
{
    class TrackObject
    {
        private:
            int _type;
            int _position;
            bn::fixed _coin_frame;
        public:
            TrackObject(int type, int position, bn::sprite_ptr sprite);
            int type();
            int position();
            bn::sprite_ptr _sprite;
            bn::rect get_rect();
            void animate(int frame);
    };
}

#endif