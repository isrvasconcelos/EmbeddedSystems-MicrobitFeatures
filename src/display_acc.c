#include "display_acc.h"

void acc_read(struct i2c_dev acc, struct mb_display *disp, uint16_t delay) {
	i2c_util_read_bytes(&acc, ACC_OUT_X_MSB, acc_data, 6);
	struct mb_image pixel = {};

	if((acc_data[2] > 196 && acc_data[4] > 170)) {
		uint8_t x=0;
		uint8_t y=0;

		/* X-Offsets to position the board */
		if(acc_data[0] < 69) {
			x = acc_data[0]/10;

		} else if(acc_data[0] > 196) {
			x = (acc_data[0]-196)/10;
		} 

		/* Y-Offsets to position the board */
		if(acc_data[2] > 196) {
			y = (acc_data[2]-196)/10;
		} 

		pixel.row[y] = BIT(x);
		mb_display_image(disp, MB_DISPLAY_MODE_SINGLE,
			K_MSEC(delay), &pixel, 1);

	}
}
