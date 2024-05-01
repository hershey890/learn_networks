/**
 * @brief datagram/UDP sockets server demo
 * 
 * Pairs with talker.c
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

const char* PORT = "8888"; // the port users will be connecting to
const int MAXBUFLEN = 100;

// get sockaddr, IPv4, or IPv6:
void *get_in_addr(struct sockaddr *sa) {
    return sa->sa_family == AF_INET ? 
        (void*)&(((struct sockaddr_in*)sa)->sin_addr) :
        (void*)&(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void) {
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // use AF_INET to force IPv4
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    int rv;
    struct addrinfo *servinfo;
    if((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    getnameinfo(servinfo->ai_addr, servinfo->ai_addrlen, NULL, 0, NULL, 0, NI_NUMERICHOST);
    printf("listener: got address %s\n", servinfo->ai_addr);


    // loop through all the results and bind to the first we can
    struct addrinfo *p;
    int sock_fd;
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if((sock_fd = socket(p->ai_family, p->ai_socktype, 
                p->ai_protocol)) == -1) {
            perror("listener: socket");
            continue;
        }
        if(bind(sock_fd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sock_fd);
            perror("listener: bind");
            continue;
        }
        break;
    }

    if(p == NULL) {
        fprintf(stderr, "listener: failed to bind socket\n");
        return 2;
    }

    freeaddrinfo(servinfo);

    printf("Listener: waiting to recvfrom...\n");

    struct sockaddr_storage their_addr;
    socklen_t addrlen = sizeof(their_addr);
    int numbytes;
    char buf[MAXBUFLEN];
    if((numbytes = recvfrom(sock_fd, buf, MAXBUFLEN-1, 0,
        (struct sockaddr *)&their_addr, &addrlen)) == -1) {
        perror("recvfrom");
        exit(1);
    };

    char s[INET6_ADDRSTRLEN];
    printf("listener: got packet from %s\n",
        inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr *)&their_addr),
            s, sizeof(s)));
    printf("listener: packet is %d bytes long\n", numbytes);
    buf[numbytes] = '\0';
    printf("listener: packet contains \"%s\"\n", buf);

    close(sock_fd);

    return 0;
    
}