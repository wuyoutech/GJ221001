#include <stdint.h>
#include <stm32f10x.h>

volatile uint32_t system_ticks = 0;
void SysTick_Handler(void) { system_ticks++; }
uint32_t sys_now(void) { return system_ticks; }
void sys_configuration(void) { SysTick_Config(SystemCoreClock / 1000); }
