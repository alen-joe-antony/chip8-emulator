# Chip8Emulator

This is a simple Chip8Emulator implemented in C++ drawing inspiration from the Udemy course [Creating A Chip-8 Emulator In C](https://www.udemy.com/course/creating-a-chip-8-emulator-in-c/). It is designed as a hands-on project to understand the fundementals of emulators. It relies on the [SDL](https://www.libsdl.org/) library to display the screen of the emulator. 


## Table of Contents

- [Overview](#overview)
- [Dependencies](#dependencies)
- [Build](#build)
- [Usage](#usage)
- [Limitations](#limitations)
- [References](#references)

## Overview

Chip-8 is a simple, interpreted programming language designed in the 1970s for early microcomputers. It was primarily used for the development of simple video games. Chip-8 programs run on a virtual machine, which was implemented on various systems of that era. The language itself includes a set of 35 opcodes, providing basic functionalities such as graphics rendering, input handling, and audio output.

The ease of implementing a Chip-8 emulator makes it a good first project for learning about emulation and low-level programming.

## Dependencies

- SDL Library


## Build

To build the Chip8 emulator learning project, follow these steps:

**Clone the repository**

```bash
git clone https://github.com/your-username/chip8-emulator.git
cd chip8-emulator
```

**Build the project**

```bash
make all 
```

This will generate the executable file for the emulator in the ./bin directory..


## Usage

Run the compiled executable with the path to a Chip8 ROM as commandline parameter to launch the Chip8 emulator.

```bash
./bin/main.exe ./path-to-rom-dir/chip8-roms/pong
```


## Limitations

This emulator only supports the basic Chip8 instructions. It does not currently support extended instructions such as the Super Chip8.


## References

- [Creating A Chip-8 Emulator In C](https://www.udemy.com/course/creating-a-chip-8-emulator-in-c/)
- [Chip-8 Technical Reference v1.0](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)

