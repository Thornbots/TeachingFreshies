#include "Robot.h"

namespace ThornBots
{
Robot::Robot(tap::Drivers* driver, GimbalSubsystem* gimbalSubsystem)
{
    this->drivers = driver;
    this->gimbalSubsystem = gimbalSubsystem;
    this->motor = new tap::motor::DjiMotor(
        driver,
        tap::motor::MOTOR1,
        tap::can::CanBus::CAN_BUS1,
        false,
        "cool motor");
}

void Robot::initialize()
{
    Board::initialize();
    drivers->can.initialize();
    drivers->remote.initialize();
    motor->initialize();

    static tap::algorithms::SmoothPidConfig pid_config_dt = {20, 0, 0, 0, 8000, 1, 0, 1, 0};
    this->pidController = new tap::algorithms::SmoothPid(pid_config_dt);

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
    pidController->runControllerDerivateError(
        3000 - motor->getShaftRPM(),
        1);  // do the pid algorithm
    motor->setDesiredOutput(
        static_cast<int32_t>(pidController->getOutput()));  // set up msg so its ready to be sent
    drivers->djiMotorTxHandler.encodeAndSendCanData();      // send all msg to the motors
}

}  // namespace ThornBots