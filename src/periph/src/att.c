#include <stm32f10x.h>

void att_configuration(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure = {
        .GPIO_Mode = GPIO_Mode_Out_PP,
        .GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_6 | GPIO_Pin_15 | GPIO_Pin_14 |
                    GPIO_Pin_13 | GPIO_Pin_12 | GPIO_Pin_1,
        .GPIO_Speed = GPIO_Speed_50MHz};
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin =
        GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_12;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void att_value_set(int att) {
    uint32_t small = att % 40;
    uint32_t large = att / 40;

    GPIO_WriteBit(GPIOB, GPIO_Pin_8,
                  ((small >> 0) & 0x01) > 0 ? Bit_SET : Bit_RESET);
    GPIO_WriteBit(GPIOB, GPIO_Pin_6,
                  ((small >> 1) & 0x01) > 0 ? Bit_SET : Bit_RESET);
    GPIO_WriteBit(GPIOA, GPIO_Pin_12,
                  ((small >> 2) & 0x01) > 0 ? Bit_SET : Bit_RESET);
    GPIO_WriteBit(GPIOA, GPIO_Pin_10,
                  ((small >> 3) & 0x01) > 0 ? Bit_SET : Bit_RESET);
    GPIO_WriteBit(GPIOA, GPIO_Pin_9,
                  ((small >> 4) & 0x01) > 0 ? Bit_SET : Bit_RESET);
    GPIO_WriteBit(GPIOA, GPIO_Pin_8,
                  ((small >> 5) & 0x01) > 0 ? Bit_SET : Bit_RESET);

    GPIO_WriteBit(GPIOB, GPIO_Pin_15, large > 0 ? Bit_SET : Bit_RESET);
    GPIO_WriteBit(GPIOB, GPIO_Pin_14, large > 1 ? Bit_SET : Bit_RESET);
    GPIO_WriteBit(GPIOB, GPIO_Pin_13, large > 2 ? Bit_SET : Bit_RESET);
    GPIO_WriteBit(GPIOB, GPIO_Pin_12, large > 3 ? Bit_SET : Bit_RESET);
    GPIO_WriteBit(GPIOB, GPIO_Pin_1, large > 4 ? Bit_SET : Bit_RESET);
}