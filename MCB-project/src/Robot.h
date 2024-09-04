#ifndef ROBOT_H_
#define ROBOT_H_

#include <cmath>

#include "tap/algorithms/smooth_pid.hpp"
#include "tap/board/board.hpp"
#include "tap/motor/dji_motor.hpp"

#include "Gimbal/GimbalSubsystem.h"

#include "drivers_singleton.hpp"

namespace ThornBots
{
class Robot
{
private:  // Private Variables
    tap::Drivers* drivers;
    GimbalSubsystem* gimbalSubsystem;
    modm::IOStream* s;

    tap::motor::DjiMotor* motor1;
    tap::motor::DjiMotor* motor2;
    tap::motor::DjiMotor* motor3;
    tap::motor::DjiMotor* motor4;

    tap::algorithms::SmoothPid* pidController1;
    tap::algorithms::SmoothPid* pidController2;
    tap::algorithms::SmoothPid* pidController3;
    tap::algorithms::SmoothPid* pidController4;

public:  // Public Methods
    Robot(tap::Drivers* driver, GimbalSubsystem* gimbalSubsystem, modm::IOStream* s);

    void initialize();

    void update();

    inline void stopRobot()
    {
        motor1->setDesiredOutput(0);
        motor2->setDesiredOutput(0);
        motor3->setDesiredOutput(0);
        motor4->setDesiredOutput(0);
        drivers->djiMotorTxHandler.encodeAndSendCanData();
    }
};
}  // namespace ThornBots

#endif  // ROBOT_H_