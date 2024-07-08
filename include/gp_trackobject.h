#ifndef GP_TRACKOBJECT_H
#define GP_TRACKOBJECT_H

#include "bn_sprite_ptr.h"
#include "bn_top_left_rect.h"

namespace gp
{
    class TrackObject
    {
        private:
            int _type;
            int _position;
            bn::top_left_rect _rect;
            int _rx;
            int _ry;
            int _rw;
            int _rh;
        public:
            TrackObject(int type, int position, bn::sprite_ptr sprite);
            int type();
            int position();
            void update();
            bn::sprite_ptr _sprite;
            bn::top_left_rect get_rect();
    };
}

#endif