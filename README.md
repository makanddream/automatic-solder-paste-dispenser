<!-- PROJECT SHIELDS -->
[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]

<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/makanddream/automatic-solder-paste-dispenser">
    <img src="docs/ASPD100_logo.png" alt="Logo" width="50" height="100">
  </a>

  <h3 align="center">Automatic Solder Paste Dispenser<br>ASPD100</h3>

  <p align="center">
    An awesome tool for your workshop!
    <br />
    <a href="https://github.com/makanddream/automatic-solder-paste-dispenser"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://github.com/makanddream/automatic-solder-paste-dispenser">View Demo</a>
    ·
    <a href="https://github.com/makanddream/automatic-solder-paste-dispenser/issues">Report Bug</a>
    ·
    <a href="https://github.com/makanddream/automatic-solder-paste-dispenser/issues">Request Feature</a>
  </p>
</div>

| 3D Case Design | 3D PCB Design | First proto | Second proto |
| :--------: | :--------: | :--------: | :--------: |
| <img src="https://github.com/makanddream/automatic-solder-paste-dispenser/blob/master/docs/3DModelDesign.png" width="150"> | <img src="https://github.com/makanddream/automatic-solder-paste-dispenser/blob/master/docs/3D_ASPD100_PCB_DESIGN.PNG" width="220"> | <img src="https://github.com/makanddream/automatic-solder-paste-dispenser/blob/master/docs/FirstPrototype.jpg" width="220"> | <img src="https://github.com/makanddream/automatic-solder-paste-dispenser/blob/master/docs/SecondPrototype.jpg" width="220">

<!-- TABLE OF CONTENTS -->
<details open="open">
  <summary>Table of Contents</summary>
  <ol>
    <li><a href="#summary">Summary</a></li>
    <li><a href="#description">Description</a></li>
    <li><a href="#requirements">Requirements</a></li>
    <li><a href="#hardware">Hardware</a></li>
    <li><a href="#translations">Translations</a></li>
    <li><a href="#purchase">Purchase</a></li>
    <li><a href="#license">License</a></li>
  </ol>
</details>

## Summary

The idea of this project it's to have a method to apply the solder paste between the stencil method and the syringe method.

The project is a pen to apply automatically the solder paste with preset values.
The preset values are the differents size of components footprints (Metric or Imperial).

<p align="center">
	<img width="250" src="https://github.com/diy-dream/automatic-solder-paste-dispenser/blob/master/docs/SizeFootprint.png" width="250">
</p>

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

## Purchase

The product will be available soon on the https://www.tindie.com/ platform.

## License
The code created by the community is GNU GPLv3. Unless noted elsewhere. Other components such as HAL/FreeRTOS have their own licence.

<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/makanddream/automatic-solder-paste-dispenser.svg?style=for-the-badge
[contributors-url]: https://github.com/makanddream/automatic-solder-paste-dispenser/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/makanddream/automatic-solder-paste-dispenser.svg?style=for-the-badge
[forks-url]: https://github.com/makanddream/automatic-solder-paste-dispenser/network/members
[stars-shield]: https://img.shields.io/github/stars/makanddream/automatic-solder-paste-dispenser.svg?style=for-the-badge
[stars-url]: https://github.com/othneildrew/Best-README-Template/stargazers
[issues-shield]: https://img.shields.io/github/issues/makanddream/automatic-solder-paste-dispenser.svg?style=for-the-badge
[issues-url]: https://github.com/makanddream/automatic-solder-paste-dispenser/issues
[license-shield]: https://img.shields.io/github/license/makanddream/automatic-solder-paste-dispenser.svg?style=for-the-badge
[license-url]: https://github.com/makanddream/automatic-solder-paste-dispenser/blob/master/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/alan-le-fol
