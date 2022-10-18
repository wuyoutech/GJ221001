#ifndef __FIFO_H_
#define __FIFO_H_

#include <stdbool.h>
#include <stdint.h>

struct fifo {
    uint32_t max_fifo_length;
    char *data;
    volatile uint32_t p_in;
    volatile uint32_t p_out;
};

void fifo_init(struct fifo *fifo, char *data, uint32_t max_length);
bool fifo_is_full(struct fifo *fifo);
bool fifo_is_empty(struct fifo *fifo);
uint32_t fifo_get_length(struct fifo *fifo);
bool fifo_dequeue(struct fifo *fifo, char *data);
bool fifo_enqueue(struct fifo *fifo, char data);
void fifo_clear(struct fifo *fifo);
int fifo_find(struct fifo *fifo, char dest);
bool fifo_copy_data(struct fifo *fifo, char *data, uint32_t length);
int fifo_contain_line(struct fifo *fifo);
int fifo_read_line(struct fifo *fifo, char *dest);

#endif
