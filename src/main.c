/*
 * Copyright (c) 2017 Intel Corporation
 * SPDX-License-Identifier: Apache-2.0
 *
 *  Description: 1) Early LED Matrix tests.
 *  		 2) Pushbutton tests.
 */

#include <zephyr.h>
#include <misc/printk.h>
#include <board.h>
#include <gpio.h>
#include <pwm.h>
#include <device.h>

#include <display/mb_display.h>

/***************************************************************************************/
/** STATE MACHINE **/

typedef enum {
	Q1,
	Q2,
	Q3,
	Q4,
	Q5
} state_t;

typedef enum {
	IDLE,
	FORWARD,
	BACKWARD
} event_t;

typedef struct {
	state_t events[3];
	void (*action)(void);
} mstate_t;

static event_t current_event = IDLE;
static state_t current_state = Q1;


void s1_display() { /* Show some scrolling text ("ECOM042.2017.2") */
	struct mb_display *disp = mb_display_get();
	mb_display_print(disp, MB_DISPLAY_MODE_DEFAULT | MB_DISPLAY_FLAG_LOOP, 
		K_MSEC(500), "ECOM042.2017.2");
}

void s2_accelerometer() {
	struct mb_display *disp = mb_display_get();
	mb_display_print(disp, MB_DISPLAY_MODE_SINGLE,
		K_SECONDS(1), "2");
}

void s3_compass() {
	struct mb_display *disp = mb_display_get();
	mb_display_print(disp, MB_DISPLAY_MODE_SINGLE,
		K_SECONDS(1), "3");
}

void s4_temperature() {
	struct mb_display *disp = mb_display_get();
	mb_display_print(disp, MB_DISPLAY_MODE_SINGLE,
		K_SECONDS(1), "4");
}

void s5_bluetooth() {
	struct mb_display *disp = mb_display_get();
	mb_display_print(disp, MB_DISPLAY_MODE_SINGLE,
		K_SECONDS(1), "5");
}

mstate_t machine[] = {
	{ .events={Q1, Q2, Q5}, .action=s1_display},
	{ .events={Q2, Q3, Q1}, .action=s2_accelerometer},
	{ .events={Q3, Q4, Q2}, .action=s3_compass},
	{ .events={Q4, Q5, Q3}, .action=s4_temperature},
	{ .events={Q5, Q1, Q4}, .action=s5_bluetooth}
};

/***************************************************************************************/
/** PUSHBUTTON **/

static struct device *pwm;
static struct device *gpio;

static void button_pressed(struct device *dev, struct gpio_callback *cb, u32_t pins)
{
	if (pins & BIT(SW0_GPIO_PIN)) {
		printk("A pressed\n");
		current_event = BACKWARD;


	} else {
		printk("B pressed\n");
		current_event = FORWARD;
	}

	current_state = machine[current_state].events[current_event];
	machine[current_state].action();
}

static void configure_buttons(void) {

	static struct gpio_callback button_cb;

	gpio = device_get_binding(SW0_GPIO_NAME);

	gpio_pin_configure(gpio, SW0_GPIO_PIN, (GPIO_DIR_IN | GPIO_INT | GPIO_INT_EDGE | GPIO_INT_ACTIVE_LOW));
	gpio_pin_configure(gpio, SW1_GPIO_PIN, (GPIO_DIR_IN | GPIO_INT | GPIO_INT_EDGE | GPIO_INT_ACTIVE_LOW));

	gpio_init_callback(&button_cb, button_pressed, BIT(SW0_GPIO_PIN) | BIT(SW1_GPIO_PIN));
	gpio_add_callback(gpio, &button_cb);

	pwm = device_get_binding(CONFIG_PWM_NRF5_SW_0_DEV_NAME);

	gpio_pin_enable_callback(gpio, SW0_GPIO_PIN);
	gpio_pin_enable_callback(gpio, SW1_GPIO_PIN);
}


/***************************************************************************************/
/** MAIN **/
void main(void)
{
	configure_buttons();
}
