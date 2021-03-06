# Building FastFingers From Source

FastFingers uses the [CMake](https://cmake.org/) and
[GNU Make](https://www.gnu.org/software/make/) build tools. Along with these tools, some libraries need to be installed
to build FastFingers from the source. In the section below these dependencies are described.

## Table of contents

* [Dependencies](#dependencies)
    + [Install Dependencies for Ubuntu/Debian](#install-dependencies-for-ubuntu-debian)
    + [Install Dependencies for Fedora/CentOS](#install-dependencies-for-fedora-centos)
    + [Install Dependencies for Arch/Manjaro](#install-dependencies-for-arch-manjaro)
* [Building and Installing](#building-and-installing)
    + [Configure the CMake](#configure-the-cmake)
    + [Compile the source files](#compile-the-source-files)
    + [Install System-wide](#install-system-wide)

## Dependencies

To build FastFingers you need to install the following libraries:

* GTK3
* CMake
* pkg-config
* A C compiler (Like GCC)
* A build system (Like GNU Make or Ninja)

You can install the required libraries and tools from package repositories:

### Install Dependencies for Ubuntu/Debian

```bash
sudo apt-get install build-essential cmake libgtk-3-0 libgtk-3-dev
```

### Install Dependencies for Fedora/CentOS

```bash
sudo dnf install make gcc cmake pkg-config gtk3 gtk3-devel
```

### Install Dependencies for Arch/Manjaro

```bash
sudo pacman -S base-devel gtk3 cmake
```

## Building and Installing

### Configure the CMake

```bash
cmake -S fastfingers/src -B fastfingers/src/build -DCMAKE_BUILD_TYPE=Release
```

### Compile the source files

```bash
cmake --build fastfingers/src/build --config Release
```

### *Install System-wide _(Optional)_

If you want to install the application system-wide, you can run the following command. This step requires root
permission to install binaries, desktop files and icons.

```bash
sudo cmake --build fastfingers/src/build --target install
```

## Run the FastFingers and Cheatsheet

### Run from the build directory

**Run FastFingers**

```bash
fastfingers/src/build/fastfingers
```

**Run Cheatsheet**

```bash
fastfingers/src/build/fastfingers-cheatsheet
```

**NOTE** You can add "{installation path}/fastfingers/src/build" to your PATH environment variable so that you can start
FastFingers and Cheatsheet from any directory.