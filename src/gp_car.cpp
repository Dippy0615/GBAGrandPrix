#include "bn_keypad.h"
#include "bn_fixed.h"
#include "bn_sprite_ptr.h"
#include "bn_rect.h"

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
        _rect.set_width(28);
        _rect.set_height(44);
        _inv = -1;
        _mud = false;
        _max_speed = gp::CAR_MAX_SPEED;
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

    void Car::flash(int time)
    {
        if(time%5==0) _sprite.set_visible(!_sprite.visible());
    }

    void Car::update()
    {
        if(_hit>-1) 
        {
            _hit--;
        }
        else
        {
            if(_state!=gp::CAR_STATE_NORMAL)
            {
                _inv = 100;
            }
            _state = gp::CAR_STATE_NORMAL;
        }

        if(_inv>-1) _inv--;
        if(_hit>-1) flash(_hit);
        if(_inv>-1) flash(_inv);
        if(_hit==-1 && _inv==-1) _sprite.set_visible(true);

        _max_speed = _mud ? gp::CAR_MAX_SPEED_MUD : gp::CAR_MAX_SPEED;

        if(_state==gp::CAR_STATE_NORMAL)
        {
            if (_speed>_max_speed)
            {
                _speed = gp::lerp(_speed, _max_speed, 0.1);
            }
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

    bn::rect Car::get_rect()
    {
        return _rect;
    }
}