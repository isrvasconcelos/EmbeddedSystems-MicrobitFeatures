#ifndef __DISPLAY_TEMP_H
#define __DISPLAY_TEMP_H

#include <display/mb_display.h>
#include <misc/printk.h>
#include <sensor.h>
#include <zephyr.h>
#include <stdio.h>

static struct sensor_value temp_value;
static uint8_t current_temperature;

void temp_read(struct device *temp_dev, 
	       struct mb_display *disp, 
	       uint16_t delay);

#endif //__DISPLAY_TEMP_H
