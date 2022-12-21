# cas-sw
CAS software main repo

## Steps to running the Blinky test program using Mbed

1: Download the MBED IDE if you haven't already at https://os.mbed.com/docs/mbed-studio/current/getting-started/index.html.

2: Download the STM32CubeProgrammer if you haven't already at https://www.st.com/en/development-tools/stm32cubeprog.html.

3: In the terminal, clone this repository with `git clone https://github.com/calstar/cas-sw --recursive`
(the --recursive is necessary to get the mbed OS submodule)

4: Open the MBED IDE.

5: Select "Open Workspace". Set the workspace directory to be cas-sw/Mbed.

6: Set "Active program" to "Blinky", set "Target" to "CAS_CORE" and set "Build profile" to "Debug".

7: Build the program with the build button immediately below the "Build profile" selector.

8: Connect your computer's USB port to the ST-LINK programmer, and connect the ST-LINK programmer to the JTAG port on CAS. This should cause the LED on the ST-LINK programmer to light up.

9: Open the STM32CubeProgrammer.

10: press the green 'connect' button.

11: Press the blue "Open File" button. Select the file cas-sw/Mbed/Blinky/BUILD/CAS_CORE/ARMC6/Blinky.bin

12: Press the blue "Download" button.

13: Press the "Device Memory" button and then press the blue "Read" button.

14: Open the CPU tab on the left.

15: Press the "Hardware Reset" button and then press the "Run" button.

16: The STM32CubeProgrammer should tell you that the device has become disconnected, which means that the program is running. The LED should be blinking on the CAS board.

