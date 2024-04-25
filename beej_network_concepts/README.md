# Beej's Network Programming
My code snippets from [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/) in C.

## Requirements
- gcc
- CMake

## Building
In the project root directory:
```
$ cmake -S . -B build
$ cmake --build build
```
The executable's will now be available in `./build/{sub project name}/{executable name}`, ex. `/build/beej_network_concepts/ch3`

## Covered Concepts