/*
 * Copyright (c) 2017 Intel Corporation
 * SPDX-License-Identifier: Apache-2.0
 *
 *  Description: Early LED Matrix tests.
 */

#include <zephyr.h>
#include <misc/printk.h>
#include <board.h>
#include <gpio.h>
#include <device.h>

#include <display/mb_display.h>

void main(void)
{
	struct mb_display *disp = mb_display_get();

	/* Show some scrolling text ("Hello Zephyr!") */
	mb_display_print(disp, MB_DISPLAY_MODE_DEFAULT | MB_DISPLAY_FLAG_LOOP,
			 K_MSEC(500), "ECOM042.2017.2");
}g
