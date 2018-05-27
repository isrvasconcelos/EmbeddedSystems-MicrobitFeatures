#ifndef __DISPLAY_COMPASS_H
#define __DISPLAY_COMPASS_H

#include <display/mb_display.h>
#include <misc/printk.h>
#include "i2c_util.h"
#include "display_acc.h"

#define COMPASS_DEV_ADDR     0x0e
#define COMPASS_WHO_AM_I_REG 0x07
#define COMPASS_TEST_VALUE 0xC4
#define COMPASS_OUT_X_MSB 0x01
#define COMPASS_OUT_Y_MSB 0x03
#define COMPASS_OUT_Z_MSB 0x05

uint8_t compass_x_data[2];
uint8_t compass_y_data[2];
uint8_t compass_z_data[2];

static struct mb_image arrow_north = 
	MB_IMAGE({ 0, 0, 1, 0, 0 },
		 { 0, 0, 1, 0, 0 },
		 { 0, 0, 1, 0, 0 },
		 { 0, 1, 1, 1, 0 },
		 { 0, 0, 1, 0, 0 });

static struct mb_image arrow_right = 
	MB_IMAGE({ 0, 0, 0, 0, 0 },
		 { 0, 1, 0, 0, 0 },
		 { 1, 1, 1, 1, 1 },
		 { 0, 1, 0, 0, 0 },
		 { 0, 0, 0, 0, 0 });

static struct mb_image arrow_left = 
	MB_IMAGE({ 0, 0, 0, 0, 0 },
		 { 0, 0, 0, 1, 0 },
		 { 1, 1, 1, 1, 1 },
		 { 0, 0, 0, 1, 0 },
		 { 0, 0, 0, 0, 0 });

static struct mb_image clear_display = 
	MB_IMAGE({ 0, 0, 0, 0, 0 },
		 { 0, 0, 0, 0, 0 },
		 { 0, 0, 0, 0, 0 },
		 { 0, 0, 0, 0, 0 },
		 { 0, 0, 0, 0, 0 });



void compass_read(struct i2c_dev compass, 
		  struct i2c_dev acc, 
		  struct mb_display *disp, 
		  uint16_t delay);

#endif //__DISPLAY_COMPASS_H
