#ifndef GP_CAR_H
#define GP_CAR_H

#include "bn_sprite_ptr.h"

namespace gp
{
    class Car
    {
        private:
            bn::sprite_ptr _sprite;
            bn::fixed _distance;
            bn::fixed _speed;
        public:
            Car(bn::sprite_ptr sprite);
            void update();
            void set_x(bn::fixed x);
            bn::fixed x();
            void accelerate(bn::fixed delta);
            void deccelerate(bn::fixed delta);
            bn::fixed speed();
            void set_angle(bn::fixed angle);
            bn::fixed distance();
            void set_distance(bn::fixed dist);
    };
}

#endif