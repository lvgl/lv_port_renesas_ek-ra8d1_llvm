# LVGL ported to Renesas EK-RA8D1 using LLVM

## Overview

This project is prefofivured to use LLVM 17 for Renesas EK-RA8D1 in e2 Studio.

The EK-RA8D1 evaluation kit enables users to effortlessly evaluate the features of the RA8D1 MCU Group and develop embedded systems applications using Renesas’ Flexible Software Package (FSP) and e2 studio IDE. Utilize rich on-board features along with your choice of popular ecosystem add-ons to bring your big ideas to life.

The MCU has a Cortex-M85 core which utilizes the Helium (SIMD) instruction set of Arm. Besides that the chip is equipped with a GPU (called DAVE2D) to off load the MCU. 

## Buy

You can purchase the Renesas EK-RA8D1 board from many distributors. See the sources at https://renesas.com/ek-ra8d1

## Specification

### CPU and Memory
- **MCU:** R7FA8D1BHECBD (Cortex-M85, 480MHz)
- **RAM:** 1MB internal, 64MB external SDRAM
- **Flash:** 2MB internal, 64MB External Octo-SPI Flash
- **GPU:** Dave2D

### Display and Touch
- **Resolution:** 480x854
- **Display Size:** 4.5”
- **Interface:** 2-lane MIPI
- **Color Depth:** 24-bit
- **Technology:** IPS
- **DPI:** 217 px/inch
- **Touch Pad:** Capacitive

### Connectivity
- Camera expansion board
- Micro USB device cable (type-A male to micro-B male)
- Micro USB host cable (type-A male to micro-B male)
- Ethernet patch cable

## Getting started

### Hardware setup
- Attach the MIPI LCD PCB to the main PCB
- On SW1 DIP switched (middle of the board) 7 should be ON, all others are OFF
- Connect the USB cable to the `Debug1` (J10) connector

### Software setup
- Add LLVM
    - Download **LLVM 17** from `here <https://github.com/ARM-software/LLVM-embedded-toolchain-for-Arm/releases/tag/release-17.0.1>`__ . Other versions might work as well, but they are not tested
    - Extract the downloaded file. The target path can be selected freely.
    - In e² studio click `Help` -> `Add Renesas Toolchain`. From the list select `LLVM Embedded Toolchain for Arm` and click the `Add...` button at the bottom.
    - Browse the extracted LLVM folder then click Ok.
- Get this project
    - Clone the repository with the following command:
        ```
        git clone https://github.com/lvgl/lv_port_renesas_ek-ra8d1_llvm.git --recurse-submodules
        ```
        Downloading the `.zip` from GitHub doesn't work as it doesn't download the submodules.
    - Follow the *RA family* section of the [*documentation*](https://docs.lvgl.io/master/integration/chip/renesas.html#get-started-with-the-renesas-ecosystem) to prepare your environment and import the project


## Setting up LLVM manually

Although this project is already pre-configured for LLVM 17 you might be interested in knowing what are main steps of changing toolchain. First, be sure that LLVM is added to e² Studio as toolchain as described above.

1. Click `File` -> `Properties` -> `C/C++ Build` -> `Tool Chain Editor`
2. In `Current Toolachain` select `LLVM for Arm` and confirm the change of the Toolchain
3. Still in Project properties select `C/C++ Build` -> `Settings`. On the `Tool Settings` tab select `CPU` and as `Arm Family` the core of your MCU, e.g. `cortex-m85`.
4. In `Library Generator` -> `Settings` set `Library type` to `Pre-Built`
5. In `Linker CPP` -> `Archives` in `Archive search directories` add `script` folder
6. In `Objcopy` -> `General` set `OutFormat` to `Intel Hex`
7. On the `Toolchain` tab be sure that `LLVM for Arm` and the correct version is selected, and click `Apply`.

## Contribution and Support

If you find any issues with the development board feel free to open an Issue in this repository. For LVGL related issues (features, bugs, etc) please use the main [lvgl repository](https://github.com/lvgl/lvgl). 

If you found a bug and found a solution too please send a Pull request. If you are new to Pull requests refer to [Our Guide](https://docs.lvgl.io/master/CONTRIBUTING.html#pull-request) to learn the basics.

