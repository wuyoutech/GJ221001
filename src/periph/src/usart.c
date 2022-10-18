#include <stdio.h>
#include <stm32f10x.h>
#include <string.h>

#include <agc.h>
#include <att.h>
#include <fifo.h>
#include <frequency.h>
#include <led.h>
#include <pa.h>
#include <sys.h>

#define MAX_COMMAND_LENGTH (64)
char receive_buffer[MAX_COMMAND_LENGTH];

struct fifo receive_fifo;

void usart_configuration(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

    USART_DeInit(USART3);

    GPIO_InitTypeDef GPIO_InitStructure = {.GPIO_Mode = GPIO_Mode_AF_PP,
                                           .GPIO_Pin = GPIO_Pin_10,
                                           .GPIO_Speed = GPIO_Speed_50MHz};
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    USART_InitTypeDef USART_InitStructure = {
        .USART_BaudRate = 10*1000*1000, // 10Mbps
        .USART_HardwareFlowControl = USART_HardwareFlowControl_None,
        .USART_Mode = USART_Mode_Tx | USART_Mode_Rx,
        .USART_Parity = USART_Parity_No,
        .USART_StopBits = USART_StopBits_1,
        .USART_WordLength = USART_WordLength_8b};

    USART_Init(USART3, &USART_InitStructure);

    USART_Cmd(USART3, ENABLE);
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    NVIC_InitTypeDef NVIC_InitStructure = {.NVIC_IRQChannel = USART3_IRQn,
                                           .NVIC_IRQChannelCmd = ENABLE,
                                           .NVIC_IRQChannelPreemptionPriority =
                                               1,
                                           .NVIC_IRQChannelSubPriority = 3};
    NVIC_Init(&NVIC_InitStructure);
    fifo_init(&receive_fifo, receive_buffer, MAX_COMMAND_LENGTH);
}

void usart_out(char *string) {
    while (*string != 0) {
        USART_SendData(USART3, *string);
        while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET)
            ;
        string++;
    }
}

void USART3_IRQHandler(void) {
    if (USART_GetITStatus(USART3, USART_IT_RXNE)) {
        fifo_enqueue(&receive_fifo, USART_ReceiveData(USART3));
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
    }
}

void usart_receive_task(void) {
    static uint32_t start_ms = 0;
    if (sys_now() - start_ms < 100)
        return;
    start_ms += 100;

    char data[64];
    int result = fifo_read_line(&receive_fifo, data);
    if (result > 0) {
        led_set_comm();

        /* handle data here */
        char *paras[10], *p = NULL;
        int i = 0;
        p = strtok(data, " ");

        while (p) {
            paras[i] = p;
            i++;
            p = strtok(NULL, " ");
            if (i > 8)
                break;
        }

        if (strcmp(paras[0], "channel") == 0) {
            if (strcmp(paras[1], "single") == 0) {
                channel_single();
            } else if (strcmp(paras[1], "dual") == 0) {
                channel_dual();
            } else {
                int channel;
                sscanf(paras[1], "%d", &channel);
                set_channel(channel);
            }
        } else if (strcmp(paras[0], "frequency") == 0) {
            int frequency;
            sscanf(paras[1], "%d", &frequency);
            set_frequency(frequency);
        } else if (strcmp(paras[0], "agc") == 0) {
            if (strcmp(paras[1], "on") == 0) {
                agc_set(true);
            } else {
                agc_set(false);
            }
        } else if (strcmp(paras[0], "pa") == 0) {
            if (strcmp(paras[1], "on") == 0) {
                pa_on();
            } else if (strcmp(paras[1], "off") == 0) {
                pa_off();
            } else if (strcmp(paras[1], "pulse") == 0) {
                // pulse
            }
        } else if (strcmp(paras[0], "att") == 0) {
            int value;
            sscanf(paras[1], "%d", &value);
            att_value_set(value);
        }
    } else {
        led_set_normal();
    }
}
