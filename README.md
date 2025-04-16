# Firefly
A lightweight, efficient PWM-based LED control system that simulates the natural glowing effect of fireflies.

## Installation and Usage

### Toolchain
- The open-source [Small Device C Compiler (SDCC)](http://sdcc.sourceforge.net/)
  - Requires version 4.2.0 or newer.
- The open-source [Easy PDK Programmer](https://github.com/free-pdk/easy-pdk-programmer-software)
  - Requires version 1.3 or newer
- The open-source [pdk-includes](https://github.com/free-pdk/pdk-includes) repository
  - Integrated via local copy into the include/ directory
- The open-source [easy-pdk-includes](https://github.com/free-pdk/easy-pdk-includes) repository
  - Integrated via local copy into the include/ directory
- Also requires 'make' and other common build tools

- `sudo apt install gawk`

### Build
```
make clean
make build
make size (also calls build)
make program (also calls size which calls build)
make run
```

## Troubleshooting

If the programming fails saying `Erasing IC... FPDK_ERROR: command ack failed / wrong icid`, it is because the IC is not detected. To fix this, try the following:
remove the dev board from the programmer and then unplug and replug the programmer.