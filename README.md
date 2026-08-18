# LVGL ported to Renesas EK-RA8D1 using LLVM

## Overview

This project is pre-configured to use the LLVM Embedded Toolchain for Arm (ATfE) 21.1.1 with
FSP 6.5.1 for the Renesas EK-RA8D1 in e2 studio. Link Time Optimization is enabled.

The EK-RA8D1 evaluation kit enables users to effortlessly evaluate the features of the RA8D1 MCU Group and develop embedded systems applications using Renesas’ Flexible Software Package (FSP) and e2 studio IDE. Utilize rich on-board features along with your choice of popular ecosystem add-ons to bring your big ideas to life.

The MCU has a Cortex-M85 core and a GPU (called DAVE2D) to off load the MCU. Rendering is
accelerated by DAVE2D; LVGL's Helium (SIMD) and Arm-2D software backends are not used, because
the FSP LVGL pack ships LVGL with those blend backends removed.

## Buy

You can purchase the Renesas EK-RA8D1 board from many distributors. See the sources at https://renesas.com/ek-ra8d1

## Benchmark

During the benchmark 2 frame buffers were used in the external SDRAM. LVGL was configured to
`LV_DISPLAY_RENDER_MODE_DIRECT` and the buffers were swapped on VSYNC to avoid tearing. Running
LVGL's benchmark demo, this project reaches about 26 FPS at 22% CPU load.

As observed in the video, the FPS only drops in highly complex scenarios, while CPU usage remains
low. For instance, when multiple ARGB images were rotated, the FPS dropped to 12 and the rendering
time increased to 66 ms, but the CPU usage stayed at 10%. Using software rendering only the FPS
would be significantly lower, and the CPU usage would peak at 100%.

Check out EK-RA8D1 in action, running LVGL's benchmark demo:
[![image](https://github.com/lvgl/lv_port_renesas_ek-ra8d1/assets/7599318/7dab86d6-b092-495b-a989-2555118d7570)
](https://www.youtube.com/watch?v=WkJPB8wto_U)

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

- [Install e2 studio](https://www.renesas.com/en/software-tool/e2studio-information-rz-family) for your OS.
  - When prompted, choose "Custom Install".
  - Ensure "RA" is included in your selection of "Device Families" to install.
  - Ensure "Renesas FSP Smart Configurator Core" and "Renesas FSP Smart Configurator ARM"
    are included in your selection of "Customize Features".
  - Ensure "LLVM Embedded Toolchain for Arm 21.1.1" is selected.
- Install FSP Packs. **v6.5.1 is required.**
  [Download it here](https://github.com/renesas/fsp/releases/tag/v6.5.1) under "Assets".
  - On **Windows**, download the `FSP_Packs_v6.5.1.exe` file and run it.
  - On **Linux**, download the `FSP_Packs_v6.5.1.zip` file and extract it into your packs folder:
    - Locate the e2 studio install location. The most reliable way is to go through
      **Help > CMSIS Pack Management > Renesas RA** — the packs location is shown at the top of the menu.
    - If the location is something like `~/.eclipse/com.renesas.platform_808163849/internal/projectgen/ra/packs`
      and the pack was downloaded to `Downloads`, run:
      ```bash
      unzip -o ~/Downloads/FSP_Packs_v6.5.1.zip -d ~/.eclipse/com.renesas.platform_808163849
      ```
      The directory structure in the ZIP overlaps with the packs install location. This is expected.
- If the LLVM toolchain is not already registered, click **Help > Add Renesas Toolchain**, select
  **LLVM Embedded Toolchain for Arm**, click **Add...** and browse to the toolchain folder.

### Run the project

- Clone this repository:
    ```bash
    git clone https://github.com/lvgl/lv_port_renesas_ek-ra8d1_llvm.git
    ```
    LVGL ships with the FSP LVGL pack, so there are no submodules to check out.
- Open e2 studio and go to **File > Open Projects from File System...**. Click "Directory",
  navigate to the cloned project, then click "Finish".
- Ensure "LLVM Embedded Toolchain for Arm 21.1.1" is selected in
  **Project > Properties > C/C++ Build > Settings > Toolchain**.
- Click the hammer to build, then the bug icon to flash and debug.
- **After changing the FSP version, run Project > Clean before building.** Clicking Debug does not
  force a full rebuild, and objects left from the previous FSP and its bundled LVGL will link but
  misbehave at runtime.
- Link Time Optimization (LTO) is enabled by default and may erroneously eliminate functions during
  linking. Check the compiler log to find those functions and mark them with `__attribute__((used))`
  in the C files, for example `__attribute__((used)) void some_func(int x) { ... }`.


## Setting up LLVM manually

Although this project is already pre-configured for the LLVM Embedded Toolchain for Arm (ATfE) 21.1.1, you might be interested in knowing what are main steps of changing toolchain. First, be sure that LLVM is added to e² Studio as toolchain as described above.

1. Click `File` -> `Properties` -> `C/C++ Build` -> `Tool Chain Editor`
2. In `Current Toolachain` select `LLVM for Arm` and confirm the change of the Toolchain
3. Still in Project properties select `C/C++ Build` -> `Settings`. On the `Tool Settings` tab select `CPU` and as `Arm Family` the core of your MCU, e.g. `cortex-m85`.
4. In `Library Generator` -> `Settings` set `Library type` to `Pre-Built`
5. In `Linker CPP` -> `Archives` in `Archive search directories` add `script` folder
6. In `Objcopy` -> `General` set `OutFormat` to `Intel Hex`
7. On the `Toolchain` tab be sure that `LLVM for Arm` and the correct version is selected, and click `Apply`.

## LVGL Pro

FSP 6.5.1 uses LVGL v9.5.0 which you can build your UI for using [LVGL Pro](https://lvgl.io/pro).

Check out the official [LVGL Pro e2 studio integration documentation](https://lvgl.io/docs/pro/integration/renesas).

## Contribution and Support

If you find any issues with the development board feel free to open an Issue in this repository. For LVGL related issues (features, bugs, etc) please use the main [lvgl repository](https://github.com/lvgl/lvgl). 

If you found a bug and found a solution too please send a Pull request. If you are new to Pull requests refer to [Our Guide](https://docs.lvgl.io/master/CONTRIBUTING.html#pull-request) to learn the basics.

