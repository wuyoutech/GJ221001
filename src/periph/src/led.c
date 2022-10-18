
#include <stdbool.h>
#include <stm32f10x.h>

#include <sys.h>

void led_configuration(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    GPIO_PinRemapConfig(GPIO_Remap_SWJ_NoJTRST, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure = {.GPIO_Mode = GPIO_Mode_Out_PP,
                                           .GPIO_Pin = GPIO_Pin_13,
                                           .GPIO_Speed = GPIO_Speed_50MHz};

    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET);
}

void sys_led_set(bool status) {
    GPIO_WriteBit(GPIOC, GPIO_Pin_13, status ? Bit_RESET : Bit_SET);
}

enum { blink_normal = 500, blink_comm = 50 };
static uint32_t blink_interval_ms = blink_normal;
void led_blinking_task(void) {
    static uint32_t start_ms = 0;
    static bool led_state = false;

    if (sys_now() - start_ms < blink_interval_ms)
        return;
    start_ms += blink_interval_ms;
    sys_led_set(led_state);
    led_state = led_state ? false : true;
}

void led_set_normal(void) { blink_interval_ms = blink_normal; }
void led_set_comm(void) { blink_interval_ms = blink_comm; }