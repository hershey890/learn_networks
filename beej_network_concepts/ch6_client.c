#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

// const char* PORT = "8888";
const int MAXDATASIZE = 100; // max number of bytes we can get at once  

// get sockaddr, IPv4, or IPv6:
void *get_in_addr(struct sockaddr *sa) {
    return sa->sa_family == AF_INET ? 
        (void*)&(((struct sockaddr_in*)sa)->sin_addr) :
        (void*)&(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[]) {
    int sock_fd, numbytes;
    char buf[MAXDATASIZE];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    if(argc != 3) {
        fprintf(stderr, "usage: ch6_client hostname port\n");
        exit(1);
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if((rv = getaddrinfo(argv[1], argv[2], &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if((sock_fd = socket(p->ai_family, p->ai_socktype, 
                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }
        if(connect(sock_fd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sock_fd);
            perror("client: connect");
            continue;
        }
        break;
    }

    if(p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), 
        s, sizeof(s));
    printf("client: connecting to %s\n", s);

    freeaddrinfo(servinfo);

    if((numbytes = recv(sock_fd, buf, MAXDATASIZE-1, 0)) == -1) {
        perror("recv");
        exit(1);
    }

    buf[numbytes] = '\0';

    printf("client: received '%s'\n", buf);

    close(sock_fd);

    return 0;
}