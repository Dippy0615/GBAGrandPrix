#include "bn_keypad.h"
#include "bn_fixed.h"
#include "bn_sprite_ptr.h"
#include "bn_rect.h"

#include "gp_car.h"
#include "gp_constants.h"

namespace gp
{
    Car::Car(bn::sprite_ptr sprite) : _sprite(sprite)
    {
        _sprite.set_bg_priority(0);
        _sprite.set_scale(0.75);
        _distance = 0;
        _speed = 0;
        _hit = -1;
        _rect.set_width(32);
        _rect.set_height(64);
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
        else _sprite.set_visible(true);

        if (_speed>gp::CAR_MAX_SPEED) _speed = gp::CAR_MAX_SPEED;
        _distance += (_speed/5);

        _rect.set_x((int)_sprite.x()+16);
        _rect.set_y((int)_sprite.y());
    }

    bn::rect Car::get_rect()
    {
        return _rect;
    }
}