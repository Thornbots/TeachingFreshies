#include "tap/communication/serial/uart_terminal_device.hpp"

#include "Gimbal/GimbalSubsystem.h"
#include "Robot.h"

#include "drivers_singleton.hpp"



static tap::arch::PeriodicMicroTimer RunTimer(
    10000);  // Don't ask me why. This only works as a global. #Certified Taproot Moment


int main()
{
    src::Drivers* drivers = src::DoNotUse_getDrivers();
    Board::initialize();

    tap::communication::serial::UartTerminalDevice ter(drivers);
    ter.initialize();
    modm::IOStream s(ter);

    drivers->leds.init();
    bool led_state = true;

    ThornBots::GimbalSubsystem* gimbalSubsystem = new ThornBots::GimbalSubsystem(drivers);
    ThornBots::Robot* robot = new ThornBots::Robot(drivers, gimbalSubsystem, &s);

    robot->initialize();


    while (1)
    {

        if (RunTimer.execute())
        {  // Calling this function every 10 us at max
            drivers->leds.set(tap::gpio::Leds::Green, led_state);
            led_state = !led_state;

            robot->update();
        }
        drivers->canRxHandler.pollCanData();  // checks to see if a msg is waiting
        modm::delay_us(10);
    }
}
