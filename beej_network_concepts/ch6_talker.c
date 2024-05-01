/**
 * @brief datagram/UDP sockets client demo
 * 
 * Pairs with listener.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

const char *PORT = "8888";

int main(int argc, char *argv[]) {
    if(argc != 3) {
        fprintf(stderr, "usage: ch6_talker hostname message\n");
        exit(1);
    }

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // use AF_INET to force IPv4
    hints.ai_socktype = SOCK_DGRAM;

    struct addrinfo *servinfo;
    int rv;
    if((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    struct addrinfo *p;
    int sock_fd;
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if((sock_fd = socket(p->ai_family, p->ai_socktype, 
                p->ai_protocol)) == -1) {
            perror("talker: socket");
            continue;
        }
        break;
    }

    if(p == NULL) {
        fprintf(stderr, "talker: failed to create socket\n");
        return 2;
    }

    int numbytes;
    if((numbytes = sendto(sock_fd, argv[2], strlen(argv[2]), 0, 
            p->ai_addr, p->ai_addrlen)) == -1) {
        perror("talker: sendto");
        exit(1);
    }

    freeaddrinfo(servinfo);

    printf("talker: send %d bytes to %s\n", numbytes, argv[1]);
    close(sock_fd);

    return 0;
}