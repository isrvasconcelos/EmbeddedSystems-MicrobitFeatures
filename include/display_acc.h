#ifndef __DISPLAY_ACC_H
#define __DISPLAY_ACC_H

#include <display/mb_display.h>
#include <misc/printk.h>
#include "i2c_util.h"

#define ACC_DEV_ADDR     0x1D
#define ACC_WHO_AM_I_REG 0x0D
#define ACC_TEST_VALUE   0x5A
#define ACC_OUT_X_MSB 0x01

uint8_t acc_data[2];

void acc_read(struct i2c_dev acc, struct mb_display *disp, uint16_t delay);

#endif //__DISPLAY_ACC_H
