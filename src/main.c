#include <stm32f10x.h>

#include <agc.h>
#include <att.h>
#include <frequency.h>
#include <led.h>
#include <pa.h>
#include <sys.h>
#include <usart.h>

int main(void) {
    sys_configuration();
    led_configuration();

    usart_configuration();

    while (1) {
        led_blinking_task();
        // usart_receive_task();
    }
}