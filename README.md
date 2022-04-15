# Automatic Solder Paste Dispenser (ASPD100)

| 3D Case Design | 3D PCB Design | First proto | Second proto |
| ------------- | ------------- | ------------- | ------------- |
| <img src="https://github.com/makanddream/automatic-solder-paste-dispenser/blob/master/docs/3DModelDesign.png" width="150"> | <img src="https://github.com/makanddream/automatic-solder-paste-dispenser/blob/master/docs/3D_ASPD100_PCB_DESIGN.PNG" width="220"> | <img src="https://github.com/makanddream/automatic-solder-paste-dispenser/blob/master/docs/FirstPrototype.jpg" width="220"> | <img src="https://github.com/makanddream/automatic-solder-paste-dispenser/blob/master/docs/SecondPrototype.jpg" width="220">

## Table of contents
* [Summary](#summary)
* [Requirements](#requirements)
* [Hardware](#hardware)
* [LICENCE](#licence)

## Summary

The idea of this project it's to have a method to apply the solder paste between the stencil method and the syringe method.

The project is a pen to apply automatically the solder paste with preset values.
The preset values are the differents size of components footprints (Metric or Imperial).

<img align="center" src="https://github.com/diy-dream/automatic-solder-paste-dispenser/blob/master/docs/SizeFootprint.png" width="250">

## Description

Each time I create a new PCB with CMS components I have two methods to apply the solder paste. 

#### Method 1 : Stencil to apply the solder paste.

* Avantage:
	* Uniformity. Apply an even amount of solder paste across every SMD pad on your PCB.
	* Efficiency. Apply solder paste to the entire board all at once instead of one pad at a time.
	* Reliability. Reliably assembled board since each pad receives the same amount of solder paste.

* Disadvantage:
	* The stencil is customized to the PCB designer. It is not economical (raw materials) when only 1 or 2 prototype is build.

#### Method 2 : Syringe to apply the solder paste.

* Avantage:
	* No raw materials is needed to create a stencil to apply the solder paste on custom PCB. 
		* 1 syringe = x specific PCB layout.
		* 1 stencil = 1 specific PCB layout.

* Disadvantage:
	* Uniformity more difficult.
	* Apply solder paste one pad at a time.
	* Each pad not receives the same amount of solder paste.

#### Conclusion
I want something in between these two methods for these reasons:
* Reduce the hardship of the second method
* Reduce the use of raw material used to create a stencil for each new PCB layout.
	
## Requirements

* STM32CubeIDE 1.9.0
* Python 3.0 for building the translation module
* WinMerge for the script : "script-copy-new-generate-files-project.bat"

## Hardware

* STM32L432kc
* 0.91 Inch OLED I2C Display SSD1306, 128x32
* JS1400AFQ
* TMC2208 (Stepper motor driver)

## Translations

The translation module was adapted to my hardware from an existing open source project (https://github.com/Ralim/IronOS). All the credit goes to them!

## Purchase info

The product will be available soon on the https://www.tindie.com/ platform.

## Licence
The code created by the community is GNU GPLv3. Unless noted elsewhere. Other components such as HAL/FreeRTOS have their own licence.
