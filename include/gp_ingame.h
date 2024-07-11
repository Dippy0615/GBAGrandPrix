#ifndef GP_INGAME_H
#define GP_INGAME_H

#include "gp_scene.h"

namespace gp
{
    class Ingame
    {
        private:
        public:
            Ingame();
            gp::Scene execute(int level);
    };
}

#endif