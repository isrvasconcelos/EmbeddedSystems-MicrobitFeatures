/*
 * Author: Israel Vasconcelos
 * Date: May/2018
 * Computing Institute - Federal University of Alagoas
 * Board: BBC Microbit
 *
 * This application brings a table-driven
 * state machine triggered by the pushbuttons A and B.
 * 
 * Description:
 * State 1 - Display: Scroll a text on LED Matrix display.
 * State 2 - Accelerometer: Move a point on LED Matrix based on accel. samples.
 * State 3 - Compass: Draw a single point which always points to the north.
 * State 4 - Thermometer: Scroll the temperat. on LED Matrix in degree Celsius.
 * State 5 - Bluetooth: Transmit data through a simple bluetooth application.
 */


#include <pwm.h>
#include <gpio.h>
#include <board.h>
#include <zephyr.h>
#include <device.h>
#include <misc/printk.h>
#include <display/mb_display.h>

#include "version.h"
#include "i2c_util.h"
#include "display_acc.h"
#include "display_temp.h"
#include "display_compass.h"


/* Sensor interfaces */
static struct i2c_dev acc; /* Accelerometer */
static struct i2c_dev compass;  /* Magnetometer */

static struct device *pwm; /* Pushbutton */
static struct device *gpio; /* Pushbutton */
static struct device *temp_dev;  /* Termomether */


/* Sensor flags */
bool acc_enabled = false;
bool scroll_enabled = false;
bool compass_enabled = false;
bool bluetooth_enabled = false;
bool temperature_enabled = false;

#define RESET_ALL_SENSORS() ({ 		\
	acc_enabled = false; 		\
	scroll_enabled = false;		\
	compass_enabled = false; 	\
	bluetooth_enabled = false; 	\
	temperature_enabled = false; 	\
})


/* Base structures for the table-driven state machine */
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


/* The functions below handles the flags with state machine  */
void s1_display() { /* Show some scrolling text */
	printk("1: Display text.\n");
	scroll_enabled = true;
}

void s2_accelerometer() { /* Enable the accelerometer with LED Matrix */
	printk("2: Accelerometer enabled.\n");
	acc_enabled = true;
}

void s3_compass() { /* Enable the compass and point to the north */
	printk("3: Compass enabled.\n");
	compass_enabled = true;
}

void s4_temperature() { /* Enable the thermometer and scroll the temperature */
	printk("4: Temperature enabled.\n");
	temperature_enabled = true;
}

void s5_bluetooth() { /* Enable the bluetooth and transmit some data */
	printk("5: Bluetooth enabled.\n");
	bluetooth_enabled = true;
}

mstate_t machine[] = { /* Table-driven State Machine */
	{ .events={Q1, Q2, Q5}, .action=s1_display},
	{ .events={Q2, Q3, Q1}, .action=s2_accelerometer},
	{ .events={Q3, Q4, Q2}, .action=s3_compass},
	{ .events={Q4, Q5, Q3}, .action=s4_temperature},
	{ .events={Q5, Q1, Q4}, .action=s5_bluetooth}
};


/* Pushbutton */
static void button_pressed(struct device *dev, 
			   struct gpio_callback *cb, 
			   u32_t pins)
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

	gpio_pin_configure(gpio, SW0_GPIO_PIN, (GPIO_DIR_IN | 
						GPIO_INT | 
						GPIO_INT_EDGE | 
						GPIO_INT_ACTIVE_LOW));

	gpio_pin_configure(gpio, SW1_GPIO_PIN, (GPIO_DIR_IN | 
						GPIO_INT | 
						GPIO_INT_EDGE | 
						GPIO_INT_ACTIVE_LOW));

	gpio_init_callback(&button_cb, button_pressed, BIT(SW0_GPIO_PIN) | 
						       BIT(SW1_GPIO_PIN));

	gpio_add_callback(gpio, &button_cb);
	pwm = device_get_binding(CONFIG_PWM_NRF5_SW_0_DEV_NAME);

	gpio_pin_enable_callback(gpio, SW0_GPIO_PIN);
	gpio_pin_enable_callback(gpio, SW1_GPIO_PIN);
}


/* Main execution */
void main(void)
{
        SYS_LOG_WRN("Firmware version: v%d.%d.%d",
                        VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD);

	uint16_t delay = 100;
	uint8_t enable_reg = 0x01;
	struct mb_image pixel = {};
	struct mb_display *disp = mb_display_get();

	temp_dev = device_get_binding("TEMP_0");
	if (!temp_dev) {
		printk("error: no temp device\n");
	}

	/* Initializing and enabling magnetometer sensor */
	i2c_util_dev_init(&compass, COMPASS_DEV_ADDR, "COMPASS", 
				    COMPASS_WHO_AM_I_REG, COMPASS_TEST_VALUE);
	i2c_util_write_bytes(&compass, 0x10, &enable_reg, sizeof(enable_reg));


	/* Initializing and enabling accelerometer sensor */
	i2c_util_dev_init(&acc, ACC_DEV_ADDR, "ACC", ACC_WHO_AM_I_REG,
	                        ACC_TEST_VALUE);
	i2c_util_write_bytes(&acc, 0x2A, &enable_reg, sizeof(enable_reg));

	configure_buttons();

	while(1) {
		if(scroll_enabled) { /* Scroll a text on LED Matrix */
			delay = 330;
			char scrolling_text[15] = "ECOM042.2017.2";

			mb_display_print(disp, MB_DISPLAY_MODE_DEFAULT | 
					       MB_DISPLAY_FLAG_LOOP, 
					       K_MSEC(delay), 
					       scrolling_text);
			
			RESET_ALL_SENSORS(); /* Avoid unwanted display flush */
		} 

		if(acc_enabled) { /* Accelerometer flag is enabled */
			delay = 100;
			acc_read(acc, disp, delay);
		} 

		if(compass_enabled) { /* Compass flag is enabled */
			delay=100;
			compass_read(compass, acc, disp, delay); 
		}

		if(temperature_enabled) { /* Thermometer flag is enabled */
			delay=1000;
			temp_read(temp_dev, disp, delay); 
		}

		if(bluetooth_enabled) { /* Bluetooth flag is enabled */
			delay=500;
			mb_display_print(disp, MB_DISPLAY_MODE_SINGLE,
					 K_MSEC(delay/2), "BT");
		}

		k_sleep(delay);
	}

}
