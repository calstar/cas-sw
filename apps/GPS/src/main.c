/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include "../../cas-zephyr-drivers/zephyr/drivers/SAM_M8Q_driver/gnss.h"

#define DEVICE_NODE sam_m8q_node

void main(void) {

	struct device *dev = DEVICE_DT_GET(DT_NODELABEL(DEVICE_NODE));
	if (dev == NULL) {
		printk("Failed to get device binding.\n");
		return;
	}
	if (!device_is_ready(dev)) {
		printk("Device is not ready.\n");
		return;
	}
	if (gnss_enable(dev, NULL) != 0) {
		printk("Error in enabling device.");
		return;
	}

	printk("Sucesfully initialized device");
	return;

}







