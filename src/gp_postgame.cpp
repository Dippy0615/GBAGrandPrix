#include "bn_core.h"
#include "bn_optional.h"
#include "bn_blending.h"
#include "bn_blending_actions.h"
#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_items_bg_checkerboard.h"

#include "gp_scene.h"
#include "gp_postgame.h"

namespace gp
{
    Postgame::Postgame(){}

    void Postgame::fade_out()
    {
        bn::blending::set_fade_alpha(0);
        bn::blending::set_fade_color(bn::blending::fade_color_type:: WHITE);
        _fade_action = bn::blending_fade_alpha_to_action(15, 1);
        while(!_fade_action.value().done()){
            _fade_action.value().update();
            for(int i=0;i<2;i++)
                bn::core::update();

        }
        bn::blending::set_fade_alpha(1);
    }

    Scene Postgame::execute()
    {
        //Setup the background
        bn::regular_bg_ptr bg = bn::regular_bg_items::bg_checkerboard.create_bg(0, 0);
        bg.set_blending_enabled(true);
    
        //fade in
        bn::blending::set_fade_alpha(1);
        bn::blending::set_fade_color(bn::blending::fade_color_type::WHITE);
        _fade_action = bn::blending_fade_alpha_to_action(30, 0);
        while(!_fade_action.value().done()){
            _fade_action.value().update();
            for(int i=0;i<2;i++)
                bn::core::update();
        }
        bn::blending::set_fade_alpha(0);

        while(true)
        {
            bn::core::update();
        }
    }
}