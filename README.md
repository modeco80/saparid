# saparid

A (WIP) server for Sony SAPARi/"Community Places", without any (hard-coded) limits whatsoever (and/or needing Wine/a X11 desktop just to run a server).

**NOTE**: This project is WIP for the time being. It most definitely will not be functional until some sort of beta release.

# Building

You will need:
- CMake 3.25 or higher
- Python 3.9 or higher
- A C++20 compiler of some sort
- Boost 1.81 or higher

It's probably a good idea to install/use vcpkg if you're on Windows (this project provides a manifest!). If you do so the cmake presets include a -vcpkg preset which uses `C:\\vcpkg` by default.

## Visual Studio

Open the folder in VS, pick the vcpkg preset for your desired configuration, and build away!

## CLion

Open the folder in CLion, enable the "release" and "debug" presets, and build away!

Note that if you're on windows you should also probably use the vcpkg presets.

## Command-line

```
$ cmake -Bbuild --preset [release or debug]
$ cmake --build build
```
