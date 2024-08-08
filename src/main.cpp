#include "bn_core.h"
#include "bn_optional.h"

#include "gp_scene.h"
#include "gp_menu.h"
#include "gp_ingame.h"
#include "gp_postgame.h"
#include "gp_trackselect.h"

int main()
{
    bn::core::init();

    gp::Scene scene = gp::Scene::Menu;

    int current_track = 0;

    while(true)
    {
        bn::core::update();
        if (scene == gp::Scene::Menu)
        {
            gp::Menu menu = gp::Menu();
            scene = menu.execute();
        }
        if (scene == gp::Scene::TrackSelect)
        {
            gp::TrackSelect trackselect = gp::TrackSelect();
            auto track = trackselect.execute();
            if(track.has_value())
            {
                current_track = track.value();
                scene = gp::Scene::Ingame;
            }
            else
            {
                scene = gp::Scene::Menu;
            }
        }
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
