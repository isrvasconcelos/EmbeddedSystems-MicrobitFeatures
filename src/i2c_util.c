#include "i2c_util.h"

int i2c_util_dev_init(struct i2c_dev *i2c_dev, u16_t addr, const char *name,
                        u8_t reg_test, u8_t reg_test_expected_val)
{
        i2c_dev->addr = addr;
        strcpy(i2c_dev->name, name);
        i2c_dev->reg_test = reg_test;
        i2c_dev->reg_test_expected_val = reg_test_expected_val;

        int ret;
        i2c_dev->dev = device_get_binding(CONFIG_I2C_GPIO_0_NAME);
        if (!i2c_dev->dev) {
            printk("I2C: Device driver not found.\n");
            return 1;
        }

        ret = i2c_configure(i2c_dev->dev, I2C_SPEED_SET(I2C_SPEED_STANDARD) |
                                I2C_MODE_MASTER);
        if(ret) {
            SYS_LOG_ERR("Could not configure I2C device. Erro code: %d", ret);
            goto ret_err;
        }
        SYS_LOG_INF("device %s Configured successfuly.", i2c_dev->name);

        u8_t id = 0;
        ret = i2c_util_read_bytes(i2c_dev, i2c_dev->reg_test, &id, 1);
        if (ret) {
            SYS_LOG_WRN("Could not comunicate with %s. Error Code %d",
                        i2c_dev->name, ret);
            goto ret_err;
        }

        if (id == i2c_dev->reg_test_expected_val) {
                SYS_LOG_DBG("I2C test Success. Read value: 0x%X!", id);
        }
        else {
                SYS_LOG_WRN("I2C test Fail. Read value: 0x%X | Expected \
                                value: 0x%X", id,
                                i2c_dev->reg_test_expected_val);
                goto ret_err;
        }

ret_err:
        return ret;
}

int i2c_util_write_bytes(struct i2c_dev *i2c_dev, u8_t reg, u8_t *data, u32_t num_bytes)
{
        struct i2c_msg msgs[2];

        /* Send the address to read from */

        msgs[0].buf = &reg;
        msgs[0].len = 1;
        msgs[0].flags = I2C_MSG_WRITE;

        /* Read from device. STOP after this. */
        msgs[1].buf = data;
        msgs[1].len = num_bytes;
        msgs[1].flags = I2C_MSG_WRITE | I2C_MSG_STOP;

        return i2c_transfer(i2c_dev->dev, &msgs[0], 2, i2c_dev->addr);
}

int i2c_util_read_bytes(struct i2c_dev *i2c_dev, u8_t reg, u8_t *data, u32_t num_bytes)
{
        struct i2c_msg msgs[2];

        /* Send the address to read from */

        msgs[0].buf = &reg;
        msgs[0].len = 1;
        msgs[0].flags = I2C_MSG_WRITE;

        /* Read from device. STOP after this. */
        msgs[1].buf = data;
        msgs[1].len = num_bytes;
        msgs[1].flags = I2C_MSG_READ | I2C_MSG_STOP;

        return i2c_transfer(i2c_dev->dev, msgs, 2, i2c_dev->addr);
}

int i2c_util_test_connection(struct i2c_dev *i2c_dev)
{
        int ret;
        u8_t id = 0;
        ret = i2c_util_read_bytes(i2c_dev, i2c_dev->reg_test, &id, 1);
        if (ret) {
            SYS_LOG_WRN("Could not comunicate with %s. Error Code %d",
                        i2c_dev->name, ret);
            goto ret_err;
        }

        if (id == i2c_dev->reg_test_expected_val) {
                SYS_LOG_DBG("I2C test Success. Read value: 0x%X!", id);
        }
        else {
                SYS_LOG_WRN("I2C test Fail. Read value: 0x%X | Expected \
                                value: 0x%X", id,
                                i2c_dev->reg_test_expected_val);
                goto ret_err;
        }

ret_err:
        return ret;
}

