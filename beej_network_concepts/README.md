# Beej's Network Programming
My code snippets from [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/) in C.
- Ch 3: learning the basics of the POSIX sockets library
- Ch 6: making a TCP client-server and also a UDP client-server

## Building
In the project root directory:
```
$ cmake -S . -B build
$ cmake --build build
```
The executable's will now be available in `./build/{sub project name}/{executable name}`, ex. `/build/beej_network_concepts/ch3`

## Usage
**ch3_inet.c and ch5_getaddrinfo.c**
Don't really do anything, just for learning how to use `inet_pton()` and `getaddrinfo()`

Run all of these in project root directory  
**ch5_showip.c:**
```
$ ./build/beej_network_concepts/ch5_showup www.google.com
IP addresses for www.google.com:

    IPv6: 2607:f8b0:4005:803::2004
    IPv4: 142.250.191.68
```

**ch6_server.c and ch6_client.c:**
```
# in terminal 1
$ ./build/beej_network_concepts/ch6_server
server: waiting for connections...

# open another terminal
$ ./build/beej_network_concepts/ch6_client localhost 8888
client: connecting to 127.0.0.1
client: received 'Hello world!'
```

**ch6_listener.c and ch6_talker.c:**
```
# in terminal 1:
$ ./build/beej_network_concepts/ch6_listener

# in terminal 2
$ ./build/beej_network_concepts/ch6_talker localhost "Hello UDP World"
client: connecting to 127.0.0.1
client: received 'Hello world!'

# Listener output:
listener: got address 
Listener: waiting to recvfrom...
listener: got packet from 127.0.0.1
listener: packet is 15 bytes long
listener: packet contains "Hello UDP World"
```

## File Descriptions:
- ch3_inet:
- ch5_getaddrinfo:
- ch6_server:
- ch6_client:
- tcp_client_server_multiprocessed:
- ch6_listener:
- ch6_talker:
- ch7_poll: 

## Todo
- Ch 7
- Ch 8