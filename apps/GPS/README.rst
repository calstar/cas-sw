SAM_M8Q GPS Module
###########

This application reads PVT (position/velocity/time) data from the SAM_M8Q on the radio board and prints it out over the console. This application sends and recieves ubx messages over the I2C bus (also referred to as DDC in the module's datasheet). The I2C bus used is I2C1 on the cas bus.

Many parts of the code are derived from this library: https://github.com/melopero/Melopero_SAM-M8Q_Arduino_Library

Currently the application sends and recieves valid ubx messages, but in its current state, it is unable to get an accurate time and position reading because the module has so far been unable to obtain a fix on a satellite. More details here in this forum question posted by me: https://portal.u-blox.com/s/question/0D52p0000CUFSvMCQX/samm8q-reports-that-the-timedate-is-invalid

The application also prints out several nmea messages, which may be useful in debugging.

**Because the SAM_M8Q is physically connected to I2C2 on the cas bus, before running this application, you need to physically connect I2C1 to I2C2 on the cas bus with jumper wires. This is necessary because the STM32 core does not have a direct connection to the I2C2 bus.**

It would be possible in the future to update this application to directly use the I2C2 bus instead, eliminating the need for jumper wires. This would be done by having the STM32 core send SPI messages to the SC18IS600 i2c-to-spi translator module, and telling that module to send messages directly to the SAM_M8Q over the I2C2 bus. If this is done, note that the SC18IS600 is limited to a maximum of 96 bytes for I2C messages, meaning that you would need to select a different ubx message type other than PVT, because PVT data is be too long.
