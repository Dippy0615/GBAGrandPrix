#include "bn_keypad.h"
#include "bn_fixed.h"
#include "bn_sprite_ptr.h"
#include "bn_top_left_rect.h"

#include "gp_car.h"
#include "gp_constants.h"
#include "gp_utils.h"

namespace gp
{
    Car::Car(bn::sprite_ptr sprite) : _sprite(sprite)
    {
        _sprite.set_bg_priority(0);
        _sprite.set_scale(0.75);
        _distance = 0;
        _speed = 0;
        _hit = -1;
        _state = gp::CAR_STATE_NORMAL;
        _rect.set_width(32);
        _rect.set_height(48);
    }

    void Car::set_x(bn::fixed x)
    {
        _sprite.set_x(x);
    }

    bn::fixed Car::x()
    {
        return _sprite.x();
    }
    
    void Car::set_angle(bn::fixed angle)
    {
        _sprite.set_rotation_angle(angle);
    }

    void Car::set_distance(bn::fixed dist)
    {
        _distance = dist;
    }

    bn::fixed Car::distance()
    {
        return _distance;
    }

    void Car::accelerate(bn::fixed delta)
    {
        _speed += delta;
    }

    void Car::deccelerate(bn::fixed delta)
    {
        _speed -= delta;
        if (_speed<0) _speed = 0;
    }

    bn::fixed Car::speed()
    {
        return _speed;
    }

    void Car::update()
    {
        if(_hit>-1) 
        {
            _hit--;
            if (_hit%5==0) _sprite.set_visible(!_sprite.visible());
        }
        else
        {
            _sprite.set_visible(true);
            _state = gp::CAR_STATE_NORMAL;
        }

        if(_state==gp::CAR_STATE_NORMAL)
        {
            if (_speed>gp::CAR_MAX_SPEED) _speed = gp::CAR_MAX_SPEED;
            _distance += (_speed/5);
        }
        else if(_state==gp::CAR_STATE_HIT)
        {
            _distance += (_speed/5);
        }
        else if(_state==gp::CAR_STATE_HIT2)
        {
            if(_hit<30)
            {
                _sprite.set_x(gp::lerp(_sprite.x(), 0, 0.1));
            }   
            _speed = 0; 
        }
        _rect.set_x((int)_sprite.x());
        _rect.set_y((int)_sprite.y());
    }

    bn::top_left_rect Car::get_rect()
    {
        return _rect;
    }
}