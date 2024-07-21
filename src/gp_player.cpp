#include "bn_keypad.h"
#include "bn_fixed.h"
#include "bn_log.h"

#include "gp_constants.h"
#include "gp_player.h"
#include "gp_car.h"

namespace gp
{
    Player::Player(Car car) : _car(car)
    {
        _control = true;
    }

    Car* Player::get_car()
    {
        return &_car;
    }

    void Player::update()
    {
        Car* car = Player::get_car();
        _control = car->_hit==-1;

        //BN_LOG("player x ", car->x());
        //BN_LOG("rect top left x ", car->get_rect().top_left().x());
        if(_control)
        {
            if (bn::keypad::held(bn::keypad::key_type::LEFT))
            {
                _car.set_x(_car.x() - 2);
            }
            
            if (bn::keypad::held(bn::keypad::key_type::RIGHT))
            {
                _car.set_x(_car.x() + 2);
            }
            
            if (bn::keypad::held(bn::keypad::key_type::A))
            {
                _car.accelerate(car->_mud ? gp::CAR_ACCEL_MUD : gp::CAR_ACCEL);
            }
            else
            {
                _car.deccelerate(gp::CAR_DEACCEL);
            }
        }
        else
        {
            _car.deccelerate(gp::CAR_DEACCEL);
        }
        _car.update();
    }
}