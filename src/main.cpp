#include "bn_core.h"

#include "gp_scene.h"
#include "gp_ingame.h"

int main()
{
    bn::core::init();

    gp::Scene scene = gp::Scene::Ingame;

    while(true)
    {
        bn::core::update();
        if (scene == gp::Scene::Ingame)
        {
            gp::Ingame ingame = gp::Ingame();
            scene = ingame.execute();
        }
    }
}
