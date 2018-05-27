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
#include <sensor.h>
#include <stdio.h>

#include <display/mb_display.h>

#include "version.h"
#include "i2c_util.h"

#define ACC_DEV_ADDR     0x1D
#define ACC_WHO_AM_I_REG 0x0D
#define ACC_TEST_VALUE   0x5A
#define ACC_OUT_X_MSB 0x01

#define COMPASS_DEV_ADDR     0x0e
#define COMPASS_WHO_AM_I_REG 0x07
#define COMPASS_TEST_VALUE 0xC4

struct i2c_dev acc, compass;
u8_t data[2];
bool compass_enabled = false;
bool acc_enabled = false;
bool temperature_enabled = false;
bool bluetooth_enabled = false;

#define RESET_ALL_SENSORS() ({ 		\
	compass_enabled = false; 	\
	acc_enabled = false; 		\
	temperature_enabled = false; 	\
	bluetooth_enabled = false; 	\
})

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

static struct device *temp_dev;
static struct sensor_value temp_value;
static uint8_t current_temperature;

void s1_display() { /* Show some scrolling text ("ECOM042.2017.2") */
	printk("1: Display text.\n");

	struct mb_display *disp = mb_display_get();
	mb_display_print(disp, MB_DISPLAY_MODE_DEFAULT | MB_DISPLAY_FLAG_LOOP, 
		K_MSEC(500), "ECOM042.2017.2");
}

void s2_accelerometer() {
	printk("2: Accelerometer enabled.\n");
	acc_enabled=true;

}

void s3_compass() {
	printk("3: Compass enabled.\n");
	compass_enabled=true;
}

void s4_temperature() {
	printk("4: Temperature enabled.\n");
	temperature_enabled=true;
}

void s5_bluetooth() {
	printk("5: Bluetooth enabled.\n");
	bluetooth_enabled=true;
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

	RESET_ALL_SENSORS();
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

        SYS_LOG_WRN("Firmware version: v%d.%d.%d",
                        VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD);

	struct mb_display *disp = mb_display_get();
	//struct mb_image pixel = {};

        i2c_util_dev_init(&acc, ACC_DEV_ADDR, "ACC", ACC_WHO_AM_I_REG,
                                ACC_TEST_VALUE);

        i2c_util_dev_init(&compass, COMPASS_DEV_ADDR, "COMPASS",
COMPASS_WHO_AM_I_REG, COMPASS_TEST_VALUE);

	temp_dev = device_get_binding("TEMP_0");
	if (!temp_dev) {
		printk("error: no temp device\n");
	}

//	printf("temp device is %p, name is %s\n",
//	       temp_dev, temp_dev->config->name);

	configure_buttons();
	
	uint8_t enable_reg = 0x01;
	i2c_util_write_bytes(&acc, 0x2A, &enable_reg, sizeof(enable_reg));

	i2c_util_write_bytes(&compass, 0x10, &enable_reg, sizeof(enable_reg));

	uint16_t delay = 500;

	while (1) {
		if(compass_enabled) { /* Compass flag is enabled */
			delay=100;

			/* Calibration with accelerometer */
			u8_t data2[2], data3[2], data4[2], data5[2];
			i2c_util_read_bytes(&acc, ACC_OUT_X_MSB, data2, 6);

			//printk("x%d, y%d, z%d\n", data2[0], data2[2], data2[4]);

			/* Device positioning: Valid accelerometer range */
			bool position_y = data2[2] > 30 && data2[2] < 60;
			bool position_z = data2[4] > 180 && data2[4] < 210;
			if( position_y && position_z ) {
				//delay=250;
				i2c_util_read_bytes(&compass, 0x01, data3, sizeof(data));
				i2c_util_read_bytes(&compass, 0x03, data4, sizeof(data));
				i2c_util_read_bytes(&compass, 0x05, data5, sizeof(data));
				printk("x%d, y%d, z%d\n", data3[1], data4[1], data5[1]);


				struct mb_image arrow_north = 
					MB_IMAGE({ 0, 0, 1, 0, 0 },
						 { 0, 0, 1, 0, 0 },
						 { 0, 0, 1, 0, 0 },
						 { 0, 1, 1, 1, 0 },
						 { 0, 0, 1, 0, 0 });

				struct mb_image arrow_right = 
					MB_IMAGE({ 0, 0, 0, 0, 0 },
						 { 0, 1, 0, 0, 0 },
						 { 1, 1, 1, 1, 1 },
						 { 0, 1, 0, 0, 0 },
						 { 0, 0, 0, 0, 0 });

				struct mb_image arrow_left = 
					MB_IMAGE({ 0, 0, 0, 0, 0 },
						 { 0, 0, 0, 1, 0 },
						 { 1, 1, 1, 1, 1 },
						 { 0, 0, 0, 1, 0 },
						 { 0, 0, 0, 0, 0 });

				struct mb_image clear_display = 
					MB_IMAGE({ 0, 0, 0, 0, 0 },
						 { 0, 0, 0, 0, 0 },
						 { 0, 0, 0, 0, 0 },
						 { 0, 0, 0, 0, 0 },
						 { 0, 0, 0, 0, 0 });

				struct mb_image arrow = clear_display;

				if(data5[1] < 100) {
					arrow = arrow_left;
				}

				else if(data5[1] > 200) {
					arrow = arrow_right;
				} 

				else if(data5[1] > 101 && data5[1] < 199) {
					arrow = arrow_north;
				} else {
					arrow = clear_display;
				}

				mb_display_image(disp, MB_DISPLAY_MODE_SINGLE, K_MSEC(delay),
						 &arrow, 1);

			} else if((data2[2] < 60 && data2[4] > 180)) {

				struct mb_image pixel = {};

				uint8_t x,y=0;
				y = data2[2]/10; 

				if(data2[0] > 20 && data2[0] < 50) {
					x = data2[0]/10;

				} else if(data2[0] > 220 && data2[0] < 250) {
					x = (data2[0]-220)/10;
					//x = 3+x;
				}

				if(y > 3) {
					y=y-1;
				} else if (y == 2) {
					y=y-1;
				}

				pixel.row[y] = BIT(x);
				mb_display_image(disp, MB_DISPLAY_MODE_SINGLE,
						K_MSEC(delay), &pixel, 1);
			}

		}

		if(acc_enabled) { /* Accelerometer flag is enabled */
			delay=100;
			i2c_util_read_bytes(&acc, ACC_OUT_X_MSB, data, 6);

			struct mb_image pixel = {};

			if((data[2] > 196 && data[4] > 170)) {

				uint8_t x,y=0;

				if(data[0] < 69) {
					x = data[0]/10;

				} else if(data[0] > 196) {
					x = (data[0]-196)/10;
				} 

				if(data[2] > 196) {
					y = (data[2]-196)/10;
				} 

				pixel.row[y] = BIT(x);
				mb_display_image(disp, MB_DISPLAY_MODE_SINGLE,
						K_MSEC(delay), &pixel, 1);
			}



		}

		if(temperature_enabled) { /* Temperature sensor flag is enabled */
			delay=1500;

			int r;
			r = sensor_sample_fetch(temp_dev);
			if (r) {
				printf("sensor_sample_fetch failed return: %d\n", r);
				break;
			}

			r = sensor_channel_get(temp_dev, SENSOR_CHAN_TEMP,
					       &temp_value);
			if (r) {
				printf("sensor_channel_get failed return: %d\n", r);
				break;
			}

			current_temperature = sensor_value_to_double(&temp_value);

			printf("Temperature is %dC\n", current_temperature);

			char output[4];
			snprintk(output, 4, "%02dC", current_temperature);
			mb_display_print(disp, MB_DISPLAY_MODE_SINGLE, K_SECONDS(0.5), output);
		}

		k_sleep(delay);
	}
}
