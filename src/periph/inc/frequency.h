#ifndef __FREQUENCY_H_
#define __FREQUENCY_H_

#include <stdint.h>

void frequency_control_configuration(void);
void set_channel(uint8_t channel_number);
void set_frequency(uint32_t frequency);

void channel_single(void);
void channel_dual(void);

#endif
