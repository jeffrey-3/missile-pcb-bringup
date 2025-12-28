#include "hal.h"

void *_sbrk(int incr) {
    extern char _end;
    static unsigned char *heap = NULL;
    unsigned char *prev_heap;
    if (heap == NULL) heap = (unsigned char *) &_end;
    prev_heap = heap;
    heap += incr;
    return prev_heap;
}
