#include "bn_core.h"
#include "bn_optional.h"
#include "bn_log.h"
#include "bn_sram.h"

#include "gp_scene.h"
#include "gp_menu.h"
#include "gp_ingame.h"
#include "gp_postgame.h"
#include "gp_trackselect.h"
#include "gp_shop.h"
#include "gp_highscores.h"
#include "gp_credits.h"
#include "gp_globals.h"
#include "gp_constants.h"
#include "gp_score.h"

int gp::coins;
bn::array<bool, 3> gp::cars;
int gp::current_car;
bn::vector<gp::Score, 4> gp::scores;

namespace{
    struct sram_data{
            bn::array<uint8_t, 3*4> saved_times;
            uint8_t saved_coins;
            bn::array<bool, 3> saved_cars;
        };
}

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

    sram_data saved_data;
    bn::sram::read(saved_data);
    for(int i=0;i<4;i++)
    {
        gp::scores[i] = gp::Score(i, saved_data.saved_times[i*3], saved_data.saved_times[i*3+1], saved_data.saved_times[i*3+2]);
    }
    gp::coins = saved_data.saved_coins;
    for(int i=0;i<3;i++)
    {
        gp::cars[i] = saved_data.saved_cars[i];
    }
    gp::cars[0] = true;

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
        if(scene == gp::Scene::Credits)
        {
            gp::Credits credits = gp::Credits();
            scene = credits.execute();
        }
        if (scene == gp::Scene::Ingame)
        {
            gp::Ingame ingame = gp::Ingame();
            scene = ingame.execute(current_track);
            sram_data data;
            for(int i=0;i<4;i++)
            {
                data.saved_times[i*3] = (uint8_t)gp::scores[i].millis();
                data.saved_times[i*3+1] = (uint8_t)gp::scores[i].secs();
                data.saved_times[i*3+2] = (uint8_t)gp::scores[i].mins();
            }
            data.saved_coins = (uint8_t)gp::coins;
            for(int i=0;i<3;i++)
            {
                data.saved_cars[i] = gp::cars[i];
            }
            bn::sram::write(data);
        }
        if(scene == gp::Scene::Postgame)
        {
            gp::Postgame postgame = gp::Postgame();
            scene = postgame.execute();
        }
    }
}
