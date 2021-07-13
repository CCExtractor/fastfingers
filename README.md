# FastFingers App

FastFingers is an open-source application that helps its users to remove the habit of using a mouse, by helping them to memorize the shortcuts they want to learn.

## Dependencies
**GTK 3 (see: https://www.gtk.org/docs/installations/linux/)**   
  * **Arch:** gtk3
  * **Debian/Ubuntu:** libgtk-3-0, libgtk-3-dev 
  * **Fedora:** gtk3, gtk3-devel

**CMake**   
  * **Arch, Debian/Ubuntu, Fedora:** CMake

A C compiler and build tool, such as GCC and GNU Make or Ninja.

## Installation

Download the source code and move to the source directory

```bash
git clone https://github.com/aliereny/fastfingers.git
cd fastfingers/src
```
Create the build directory and move to it

```bash
mkdir build
cd build
```

Generate the binary files by CMake CLI

```bash
cmake ..
sudo cmake --build .
```
The apps are ready to run. The apps should be installed in the applications sections of your desktop environment where you can run them from. Also you can run them from the commands "fastfingers" and "fastfingers-cheatsheet"

https://youtu.be/6ckgOJjpszU

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License
[GPL-2.0 License](https://github.com/CCExtractor/fastfingers/blob/main/LICENSE)
