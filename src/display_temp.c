#include "display_temp.h"

void temp_read(struct device *temp_dev, 
	       struct mb_display *disp, 
	       uint16_t delay) 
{
	int r;
	r = sensor_sample_fetch(temp_dev);

	if (r) {
		printf("sensor_sample_fetch failed return: %d\n", r);
	}

	r = sensor_channel_get(temp_dev, SENSOR_CHAN_TEMP,
			       &temp_value);
	if (r) {
		printf("sensor_channel_get failed return: %d\n", r);
	}

	current_temperature = sensor_value_to_double(&temp_value);
	printf("Temperature is %dC\n", current_temperature);

	char scrolling_text[4];
	snprintk(scrolling_text, 4, "%02dC", current_temperature);

//	mb_display_print(disp, MB_DISPLAY_MODE_DEFAULT | MB_DISPLAY_FLAG_LOOP, 
//		K_MSEC(delay/4), scrolling_text);

			mb_display_print(disp, MB_DISPLAY_MODE_SINGLE,
					 K_MSEC(delay/4), scrolling_text);
}
