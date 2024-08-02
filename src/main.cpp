#include "bn_core.h"

#include "gp_scene.h"
#include "gp_ingame.h"
#include "gp_postgame.h"

int main()
{
    bn::core::init();

    gp::Scene scene = gp::Scene::Ingame;

    int current_track = 0;

    while(true)
    {
        bn::core::update();
        if (scene == gp::Scene::Ingame)
        {
            gp::Ingame ingame = gp::Ingame();
            scene = ingame.execute(current_track);
        }
        if(scene == gp::Scene::Postgame)
        {
            gp::Postgame postgame = gp::Postgame();
            scene = postgame.execute();
        }
    }
}
