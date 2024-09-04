#include "Robot.h"

namespace ThornBots
{
Robot::Robot(tap::Drivers* driver, GimbalSubsystem* gimbalSubsystem, modm::IOStream* s)
{
    this->drivers = driver;
    this->gimbalSubsystem = gimbalSubsystem;
    this->s = s;
}

void Robot::initialize()
{
    drivers->can.initialize();
    drivers->remote.initialize();


    this->motor1 = new tap::motor::DjiMotor(
        drivers,
        tap::motor::MOTOR1,
        tap::can::CanBus::CAN_BUS1,
        false,
        "cool motor1");
    this->motor2 = new tap::motor::DjiMotor(
        drivers,
        tap::motor::MOTOR2,
        tap::can::CanBus::CAN_BUS1,
        false,
        "cool motor2");
    this->motor3 = new tap::motor::DjiMotor(
        drivers,
        tap::motor::MOTOR3,
        tap::can::CanBus::CAN_BUS1,
        false,
        "cool motor3");
    this->motor4 = new tap::motor::DjiMotor(
        drivers,
        tap::motor::MOTOR4,
        tap::can::CanBus::CAN_BUS1,
        false,
        "cool motor4");

    motor1->initialize();
    motor2->initialize();
    motor3->initialize();
    motor4->initialize();

    static tap::algorithms::SmoothPidConfig pid_config_dt = {20, 0, 0, 0, 8000, 1, 0, 1, 0};
    this->pidController1 = new tap::algorithms::SmoothPid(pid_config_dt);
    this->pidController2 = new tap::algorithms::SmoothPid(pid_config_dt);
    this->pidController3 = new tap::algorithms::SmoothPid(pid_config_dt);
    this->pidController4 = new tap::algorithms::SmoothPid(pid_config_dt);

    modm::delay_ms(
    2500);  // Delay 2.5s to allow the IMU to turn on and get working before we move it around
    // TODO: Finish this (Add creating timers, maybe some code to setup the IMU and make sure it's
    // reading correctly, ect)

}

void Robot::update()
{
    drivers->remote.read();  
    //  DO NOT REMOVE
    if (!drivers->remote.isConnected())
    {
        s->printf("no remote connected...\n");         // print out value
        // I WILL COME FOR U IN YOUR SLEEP if you del ;)
        stopRobot();
        return;
    }

    //TODO: implement gimbal
    // gimbalSubsystem->turretMove();

    // get remote value
    double remote_h = drivers->remote.getChannel(tap::communication::serial::Remote::Channel::RIGHT_HORIZONTAL);
    double remote_v = drivers->remote.getChannel(tap::communication::serial::Remote::Channel::RIGHT_VERTICAL);

    s->printf("remote: %f, %f\n", remote_h, remote_v);         // print out value

    pidController1->runControllerDerivateError(
        3000*remote_h - motor1->getShaftRPM(), 1);  // do the pid algorithm
    pidController2->runControllerDerivateError(
        3000*remote_h - motor2->getShaftRPM(), 1);  // do the pid algorithm
    pidController3->runControllerDerivateError(
        3000*remote_h - motor3->getShaftRPM(), 1);  // do the pid algorithm
    pidController4->runControllerDerivateError(
        3000*remote_h - motor4->getShaftRPM(), 1);  // do the pid algorithm
    motor1->setDesiredOutput(
        static_cast<int32_t>(pidController1->getOutput()));  // set up msg so its ready to be sent
    motor2->setDesiredOutput(
        static_cast<int32_t>(pidController2->getOutput()));  // set up msg so its ready to be sent
    motor3->setDesiredOutput(
        static_cast<int32_t>(pidController3->getOutput()));  // set up msg so its ready to be sent
    motor4->setDesiredOutput(
        static_cast<int32_t>(pidController4->getOutput()));  // set up msg so its ready to be sent


    drivers->djiMotorTxHandler.encodeAndSendCanData();      // send all msg to the motors
}

}  // namespace ThornBots