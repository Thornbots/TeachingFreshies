#include "tap/communication/serial/uart_terminal_device.hpp"

#include "drivers_singleton.hpp"

using namespace tap::communication::serial;

int main()
{
    src::Drivers* drivers = src::DoNotUse_getDrivers();
    Board::initialize();

    UartTerminalDevice ter(drivers);
    ter.initialize();
    modm::IOStream s(ter);

    drivers->leds.init();
    bool led_state = true;

    while (1)
    {
        drivers->leds.set(tap::gpio::Leds::Green, led_state);
        led_state = !led_state;
        s.printf("hello world\n");
        modm::delay_ms(10);
    }
}
