#include "bn_core.h"
#include "bn_optional.h"
#include "bn_log.h"

#include "gp_scene.h"
#include "gp_menu.h"
#include "gp_ingame.h"
#include "gp_postgame.h"
#include "gp_trackselect.h"
#include "gp_shop.h"
#include "gp_highscores.h"
#include "gp_globals.h"
#include "gp_constants.h"
#include "gp_score.h"

int gp::coins;
bn::array<bool, 3> gp::cars;
int gp::current_car;
bn::vector<gp::Score, 4> gp::scores;

int main()
{
    bn::core::init();

    gp::Scene scene = gp::Scene::Menu;

    int current_track = 0;
    gp::cars = {true, false, false};

    gp::current_car = gp::TECHNO_CAR_ID;
    gp::coins = 0;

    gp::scores.push_back(gp::Score(0, 0, 0, 0));
    gp::scores.push_back(gp::Score(0, 0, 0, 0));
    gp::scores.push_back(gp::Score(0, 0, 0, 0));
    gp::scores.push_back(gp::Score(0, 0, 0, 0));

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
        if(scene == gp::Scene::Shop)
        {
            gp::Shop shop = gp::Shop();
            scene = shop.execute();
        }
        if(scene == gp::Scene::Highscores)
        {
            gp::Highscores highscores = gp::Highscores();
            scene = highscores.execute();
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
