#include <zephyr.h>
#include <device.h>
#include <drivers/i2c.h>
#include "mpr121.h"
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(zmk, CONFIG_ZMK_LOG_LEVEL);

#define I2C_DEV DT_LABEL(DT_NODELABEL(i2c0))
#define MPR121_ADDR 0x5A

static const struct device *i2c_dev;

static int mpr121_init(void) {
    i2c_dev = device_get_binding(I2C_DEV);
    if (!i2c_dev) {
        printk("I2C: Device driver not found.\n");
        return -ENODEV;
    }

    // Example: Write to MPR121 to set up configuration (if needed)
    // uint8_t config_data[] = {0x80, 0x63}; // Example configuration data
    // int ret = i2c_write(i2c_dev, config_data, sizeof(config_data), MPR121_ADDR);
    // if (ret) {
    //     printk("Failed to configure MPR121\n");
    //     return ret;
    // }

    LOG_INF("MPR121 initialized successfully.\n");
    return 0;
}

static int mpr121_read(uint8_t reg, uint8_t *data, size_t len) {
    int ret = i2c_burst_read(i2c_dev, MPR121_ADDR, reg, data, len);
    if (ret) {
        LOG_ERR("Failed to read from MPR121\n");
        return ret;
    }
    return 0;
}

void main(void) {
    int ret = mpr121_init();
    if (ret) {
        LOG_ERR("MPR121 initialization failed\n");
        return;
    }

    uint8_t touch_status[2];
    while (1) {
        ret = mpr121_read(0x00, touch_status, sizeof(touch_status));
        if (ret == 0) {
            uint16_t touch = (touch_status[1] << 8) | touch_status[0];
            LOG_INF("Touch status: 0x%04x\n", touch);
        }
        k_sleep(K_MSEC(100));
    }
}