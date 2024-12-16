#ifndef MPR121_H
#define MPR121_H

#include <stdint.h>  // For uint8_t
#include <stddef.h>  // For size_t

int mpr121_init(void);
int mpr121_read(uint8_t reg, uint8_t *data, size_t len);

#endif // MPR121_H