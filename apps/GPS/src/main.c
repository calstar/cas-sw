/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/i2c.h>

#define I2C_NODE i2c2
#define DEVICE_I2C_ADDR 0x42

// The sam-m8q has three registers (0xFD, 0xFE, and 0xFF) which can be
// read through I2C. The 0xFF register contains the current data stream.
#define DATA_REG 0xFF

struct device* initialize_i2c_device() {
	struct device *i2c_dev = DEVICE_DT_GET(DT_NODELABEL(I2C_NODE));
	if (i2c_dev == NULL) {
		printk("Failed to get device binding.\n");
		return NULL;
	}
	if (!device_is_ready(i2c_dev)) {
		printk("Device is not ready.\n");
		return NULL;
	}
	return i2c_dev;
}

int ddc_read(struct device* i2c_device, int* read_buf) {

}

int ddc_write(struct device* i2c_device, int* write_buf) {

}

int ubx_configure(struct device* i2c_device) {

}

void main(void) {

	int rc;
	struct device* i2c_dev = initialize_i2c_device();
	if (i2c_dev == NULL) {
		printk("Failed to initialize device.\n");
		return;
	}

}







