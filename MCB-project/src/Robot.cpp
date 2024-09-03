#include "Robot.h"

namespace ThornBots
{
Robot::Robot(tap::Drivers* driver, GimbalSubsystem* gimbalSubsystem)
{
    this->drivers = driver;
    this->gimbalSubsystem = gimbalSubsystem;
}

void Robot::initialize()
{
    Board::initialize();
    drivers->can.initialize();
    drivers->remote.initialize();

    modm::delay_ms(
    2500);  // Delay 2.5s to allow the IMU to turn on and get working before we move it around
    // TODO: Finish this (Add creating timers, maybe some code to setup the IMU and make sure it's
    // reading correctly, ect)

}

void Robot::update()
{
    //  DO NOT REMOVE
    if (!drivers->remote.isConnected())
    {
        // I WILL COME FOR U IN YOUR SLEEP if you del ;)
        stopRobot();
        return;
    }

    drivers->djiMotorTxHandler.encodeAndSendCanData();      // send all msg to the motors
}

}  // namespace ThornBots