#include <stdint.h>
#include <stm32f10x.h>

#include <frequency.h>

void frequency_control_configuration(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure = {.GPIO_Mode = GPIO_Mode_Out_PP,
                                           .GPIO_Speed = GPIO_Speed_50MHz,
                                           .GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_9};
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin =
        GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    set_channel(0);
    set_frequency(66000);
}

void set_channel(uint8_t channel_number) {
    volatile uint16_t value = GPIOB->ODR;
    switch (channel_number) {
    case 0:
        value &= ~(1U << 5);
        value &= ~(1U << 9);
        break;
    case 1:
        value &= ~(1U << 5);
        value |= (1U << 9);
        break;
    case 2:
        value |= (1U << 5);
        value &= ~(1U << 9);
        break;
    case 3:
        value |= (1U << 5);
        value |= (1U << 9);
        break;
    default:
        break;
    }
    GPIOB->ODR = value;
}

void channel_single(void) {
    GPIO_InitTypeDef GPIO_InitStructure = {.GPIO_Mode = GPIO_Mode_IN_FLOATING,
                                           .GPIO_Speed = GPIO_Speed_50MHz,
                                           .GPIO_Pin = GPIO_Pin_9};
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void channel_dual(void) {
    GPIO_InitTypeDef GPIO_InitStructure = {.GPIO_Mode = GPIO_Mode_Out_PP,
                                           .GPIO_Speed = GPIO_Speed_50MHz,
                                           .GPIO_Pin = GPIO_Pin_9};
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void set_frequency(uint32_t frequency) {
    frequency = frequency - 66000;
    uint32_t value = frequency / 1000;
    if (frequency - (value * 1000) > 500)
        value = value + 1;
        
    GPIO_WriteBit(GPIOA, GPIO_Pin_3,
                  ((value >> 0) & 0x01) > 0 ? Bit_SET : Bit_RESET);
    GPIO_WriteBit(GPIOA, GPIO_Pin_4,
                  ((value >> 1) & 0x01) > 0 ? Bit_SET : Bit_RESET);
    GPIO_WriteBit(GPIOA, GPIO_Pin_5,
                  ((value >> 2) & 0x01) > 0 ? Bit_SET : Bit_RESET);
    GPIO_WriteBit(GPIOA, GPIO_Pin_6,
                  ((value >> 3) & 0x01) > 0 ? Bit_SET : Bit_RESET);
    GPIO_WriteBit(GPIOA, GPIO_Pin_7,
                  ((value >> 4) & 0x01) > 0 ? Bit_SET : Bit_RESET);
}