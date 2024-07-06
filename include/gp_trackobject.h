#ifndef GP_TRACKOBJECT_H
#define GP_TRACKOBJECT_H

#include "bn_sprite_ptr.h"

namespace gp
{
    class TrackObject
    {
        private:
            int _type;
            int _position;
        public:
            TrackObject(int type, int position, bn::sprite_ptr sprite);
            int type();
            int position();
            void update();
            bn::sprite_ptr _sprite;
    };
}

#endif