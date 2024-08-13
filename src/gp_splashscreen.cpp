#include "bn_core.h"
#include "bn_optional.h"
#include "bn_blending.h"
#include "bn_blending_actions.h"
#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_items_gbajam24_logo.h"
#include "bn_keypad.h"
#include "bn_sound_items.h"
#include "bn_sound_actions.h"

#include "gp_scene.h"
#include "gp_splashscreen.h"

namespace gp
{
    SplashScreen::SplashScreen(){}

    void SplashScreen::fade_out()
    {
        bn::blending::set_fade_alpha(0);
        bn::blending::set_fade_color(bn::blending::fade_color_type:: WHITE);
        _fade_action = bn::blending_fade_alpha_to_action(30, 1);
        while(!_fade_action.value().done()){
            _fade_action.value().update();
            for(int i=0;i<2;i++)
                bn::core::update();
        }
        bn::blending::set_fade_alpha(1);
        for(int i=0;i<22;i++)
            bn::core::update();
    }

    gp::Scene SplashScreen::execute()
    {
        //Setup the road & background
        bn::regular_bg_ptr bg = bn::regular_bg_items::gbajam24_logo.create_bg(0, 0);
        bg.set_blending_enabled(true);

        int timer = 0;

        //fade in
        bn::blending::set_fade_alpha(1);
        bn::blending::set_fade_color(bn::blending::fade_color_type::WHITE);
        _fade_action = bn::blending_fade_alpha_to_action(30, 0);
        for(int i=0;i<22;i++)
            bn::core::update();
        while(!_fade_action.value().done()){
            _fade_action.value().update();
            for(int i=0;i<2;i++)
            {
                bn::core::update();
            }
        }

        while(true)
        {
            timer++;
            if(timer==2)
            {
                bn::sound_items::logo.play(1);
            }

            if (bn::keypad::held(bn::keypad::key_type::A))
            {
                timer = 60*3;
            }

            if(timer>=60*3)
            {
                fade_out();
                return gp::Scene::Title;
            }
            bn::core::update();
        }
    }
}