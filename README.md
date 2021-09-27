# CppND-System-Monitor

This repository contains the result of the second project of the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213). The project concerns a system monitor for the Linux Operating System and emphasized learnings from the Object Oriented Programming (OOP) course of the program.

## Dependencies 

[ncurses](https://www.gnu.org/software/ncurses/) is a library that facilitates text-based graphical output in the terminal. This project relies on ncurses for display output.

Install ncurses within your own Linux environment: `sudo apt install libncurses5-dev libncursesw5-dev`

You also need `cmake` and `make` installed 

## Make
This project uses [Make](https://www.gnu.org/software/make/). The Makefile has four targets:
* `build` compiles the source code and generates an executable
* `format` applies [ClangFormat](https://clang.llvm.org/docs/ClangFormat.html) to style the source code
* `debug` compiles the source code and generates an executable, including debugging symbols
* `clean` deletes the `build/` directory, including all of the build artifacts

## Instructions

1. Clone the project repository

2. Build the project: `make build`

3. Run the resulting executable: `./build/monitor`

![System Monitor](images/rishil_monitor.png)




