# Learn Networks
A personal guide to learning computer networks

## Requirements
- gcc
- CMake
- Python >= 3.5

## Building
In the project root directory:
```
$ cmake -S . -B build
$ cmake --build build
```
The executable's will now be available in `./build/{sub project name}/{executable name}`, ex. `/build/beej_network_concepts/ch3`

## Covered Concepts
- POSIX Sockets (via C sockets API in ./beej_network_concepts/)
- A basic HTTP TCP Echo Server in Python (./http_server_python/)
- A basic HTTP TCP Client-Server in C (./beej/network/concepts/ch6_{client,server}.c)
- A basic HTTP UDP Client-Server in C (./beej/network/concepts/ch6_{listener,talker}.c)

## Resources
- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/)
- [Beej's Guide to Networking Concepts](https://beej.us/guide/bgnet0/)
- [Writing an HTTP server from scratch in Python](https://bhch.github.io/posts/2017/11/writing-an-http-server-from-scratch/)
- [Personal Notes](https://personalmicrosoftsoftware0-my.sharepoint.com/:w:/g/personal/hershey890_personalmicrosoftsoftware_ucla_edu/EQhKXeENeG5Cod4ZIlfMYlABsHXum5AN2TOlnL__zyaLOg?e=QBQX6d)