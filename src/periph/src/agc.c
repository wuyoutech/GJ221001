#include <stdbool.h>
#include <stm32f10x.h>

#include <agc.h>

void agc_confgiuration(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure = {.GPIO_Mode = GPIO_Mode_Out_PP,
                                           .GPIO_Pin = GPIO_Pin_11,
                                           .GPIO_Speed = GPIO_Speed_50MHz};
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    agc_set(true);
}

void agc_set(bool new_state) {
    GPIO_WriteBit(GPIOA, GPIO_Pin_11, new_state ? Bit_SET : Bit_RESET);
}