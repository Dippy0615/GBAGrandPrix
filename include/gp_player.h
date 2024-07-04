#ifndef GP_PLAYER_H
#define GP_PLAYER_H

#include "bn_fixed.h"

#include "gp_car.h"

namespace gp
{
    class Player
    {
        private:
            Car _car;
        public:
            Player(Car car);
            void update();
            bn::fixed speed();
    };
}

#endif