# pid-balancetriangle
Code and notes for a PID-controlled self-balancing triangle, such as available at https://www.aliexpress.com/item/1005003676839138.html

![](images/balancetriangle.jpg)

## Operation
- Press both buttons to enter setup mode.
- Press S1 while tilting the triangle left/right to decrease/increase energy store
- Press S2 while tilting the triangle left/right to decrease/increase Cen_SET
- Press both buttons again to save parameters

## Code
1.) Open the Keil project triangle.uvprojx from the project\USER directory in Keil uVision v5
2.) Make changes to source code as required:
 - USER\main.c contains high-level program loop
 - BALANCE\control.c contains PID control code, key handling etc.
 - BALANCE\show.c contains OLED display code
3.) To configure for target device, Under Project -> Options for Target 1
 - Device Tab: STM32F103C6
 - Output Tab: Select folder for objects as \OBJ
4.) Build by selecting Project - Build Target 
5.) To upload to the device, connect an STLink USB dongle, with 3V, GND, DIO and CLK.
6.) Then load STM32CubeProgrammer, Connect to the device, select the .hex file compiled by Keil, and then upload. 
