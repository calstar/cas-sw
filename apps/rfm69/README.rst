RFM69 Radio module
###########

This application sends arbitrary data from the RFM69 transciever on the radio board. The STM32 core communicates with the RFM69 over the SPI2 bus. Communication with the RFM69 happens exclusively by writing/reading the internal registers over SPI. Transmitting data is done by writing the data to be sent into the internal register address 0x00, which is a FIFO that holds the data to be sent. 

cas_spi.c and cas_spi.h manage the boilerplate code for sending and recieving data over spi in zephyr. These 2 files can be reused for other applications that also communicate over SPI.

Many parts of the rfm69-specific code are derived from this library: https://github.com/LowPowerLab/RFM69

In its current state, the application runs without errors and reports that the data was transmitted sucesfully, however it has not yet been tested with a reciever radio that would verify if data is actually being sent.

** Note: the JP1 jumper that connects the radio's output to the antenna has not yet been soldered on to the board. We need to add the JP1 jumper so that the radio will actually be able to transmit. **