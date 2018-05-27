#ifndef __I2C_UTIL_H
#define __I2C_UTIL_H

#include <zephyr.h>
#include <misc/printk.h>
#include <board.h>
#include <gpio.h>
#include <device.h>
#include <i2c.h>
#include <string.h>

#define SYS_LOG_DOMAIN "PROJECT"
#include <logging/sys_log.h>

#define I2C_DEVICE_NAME_LENGTH 10

struct i2c_dev {
        struct device *dev;
        char name[I2C_DEVICE_NAME_LENGTH];
        u16_t addr;
        u8_t reg_test;
        u8_t reg_test_expected_val;
};

int i2c_util_dev_init(struct i2c_dev *i2c_dev, u16_t addr, const char *name,
                        u8_t reg_test, u8_t reg_test_expected_val);
int i2c_util_write_bytes(struct i2c_dev *i2c_dev, u8_t reg, u8_t *data, u32_t num_bytes);
int i2c_util_read_bytes(struct i2c_dev *i2c_dev, u8_t reg, u8_t *data, u32_t num_bytes);
int i2c_util_test_connection(struct i2c_dev *i2c_dev);

#endif //__I2C_UTIL_H
