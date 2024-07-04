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

    }

    bn::fixed Player::speed()
    {
        return _car.speed();
    }

    void Player::update()
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
            _car.accelerate(gp::CAR_ACCEL);
        }
        else
        {
            _car.deccelerate(gp::CAR_DEACCEL);
        }
        BN_LOG("speed: ", _car.speed());
        _car.update();
    }
}