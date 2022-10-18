#include <fifo.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void fifo_init(struct fifo *fifo, char *data, uint32_t max_length) {
    fifo->data = data;
    fifo->max_fifo_length = max_length;
    fifo->p_in = 0;
    fifo->p_out = 0;
}

bool fifo_is_full(struct fifo *fifo) {
    if ((fifo->p_in + 1) % fifo->max_fifo_length == fifo->p_out)
        return true;
    return false;
}

bool fifo_is_empty(struct fifo *fifo) {
    if (fifo->p_in == fifo->p_out)
        return true;
    return false;
}

uint32_t fifo_get_length(struct fifo *fifo) {
    return (fifo->p_in - fifo->p_out + fifo->max_fifo_length) %
           fifo->max_fifo_length;
}

bool fifo_dequeue(struct fifo *fifo, char *data) {
    if (fifo_is_empty(fifo)) {
        return false;
    } else {
        if (data != NULL)
            *data = fifo->data[fifo->p_out];
        fifo->p_out = (fifo->p_out + 1) % fifo->max_fifo_length;
        return true;
    }
}

bool fifo_enqueue(struct fifo *fifo, char data) {
    if (fifo_is_full(fifo)) {
        return false;
    } else {
        fifo->data[fifo->p_in] = data;
        fifo->p_in = (fifo->p_in + 1) % fifo->max_fifo_length;
        return true;
    }
}

void fifo_clear(struct fifo *fifo) {
    /* note: this function do not clear data array */
    fifo->p_out = fifo->p_in;
}

bool fifo_copy_data(struct fifo *fifo, char *data, uint32_t length) {
    if (length > fifo_get_length(fifo))
        return false;
    uint32_t temp = fifo->p_out;
    for (uint32_t i = 0; i < length; i++) {
        data[i] = fifo->data[temp];
        temp = (temp + 1) % fifo->max_fifo_length;
    }
    return true;
}

int fifo_find(struct fifo *fifo, char dest) {
    uint32_t temp = fifo->p_out;
    while (temp != fifo->p_in) {
        if (fifo->data[temp] == dest) {
            return (temp - fifo->p_out + fifo->max_fifo_length) %
                   fifo->max_fifo_length;
        }
        temp = temp + 1 % fifo->max_fifo_length;
    }
    return -1;
}

int fifo_contain_line(struct fifo *fifo) {
    uint32_t temp = fifo->p_out;
    while (temp != fifo->p_in) {
        if (fifo->data[temp] == '\r' || fifo->data[temp] == '\n') {
            return (temp - fifo->p_out + fifo->max_fifo_length) %
                   fifo->max_fifo_length;
        }
        temp = temp + 1 % fifo->max_fifo_length;
    }
    return -1;
}

int fifo_read_line(struct fifo *fifo, char *dest) {
    int index = fifo_contain_line(fifo);
    if (index < 0) {
        return index;
    }
    for (int i = 0; i < index; i++) {
        fifo_dequeue(fifo, dest);
        dest++;
    }
    fifo_dequeue(fifo, NULL);
    *dest = 0;
    return index;
}