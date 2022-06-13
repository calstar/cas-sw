/* Application to communicate with SAM-M8Q GNSS module and get GPS readings.
 * The CPU sends SPI messages to the SC18IS600 translator module, and this
 * translator module sends I2C messages to the SAM-M8Q module. 
 */

#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/spi.h>
#include <drivers/gpio.h>
#include "sam_m8q.h"
#include "sc18is600.h"
#include "cas_spi.h"

void main(void) {

	printk("--- SAM-M8Q GPS Module ---\n");

	// This application uses SPI bus 3, aka cas_onboard_spi
	const struct device *spi_dev = DEVICE_DT_GET(DT_NODELABEL(cas_onboard_spi));
	if (spi_dev == NULL) {
		printk("Error in main.c: Failed to get device binding.\n");
		return NULL;
	}
	if (!device_is_ready(spi_dev)) {
		printk("Error in main.c: Device is not ready.\n");
		return NULL;
	}

	// This application uses pin C8 as the CS pin
	const struct spi_cs_control ctrl = {
    	.gpio_dev = DEVICE_DT_GET(DT_NODELABEL(gpioc)),
    	.gpio_pin = 8,
    	.gpio_dt_flags = GPIO_ACTIVE_LOW,
    };
	const struct spi_config cfg = {
		.frequency = 1000000,
		.operation = SPI_OP_MODE_MASTER | SPI_TRANSFER_MSB | SPI_WORD_SET(8),
		.cs = &ctrl,
	};

	Position *pos = sam_m8q_get_position(spi_dev, &cfg);
	// printk("Longitude: %d, Latitude: %d, Altitude: %d\n", pos->longitude, pos->Latitude, pos->altitude);

	return;

}