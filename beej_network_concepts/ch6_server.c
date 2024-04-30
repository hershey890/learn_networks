#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

const char* PORT = "8888"; // the port users will be connecting to
const char BACKLOG = 10; // how many pending connections queue will hold

void sigchld_handler(int s) {
    // waitpid() might overwrite errno, so we wait and restore it
    int saved_errno = errno;
    while(waitpid(-1, NULL, WNOHANG) > 0);
    errno = saved_errno;
}

// get sockaddr, IPv4, or IPv6:
void *get_in_addr(struct sockaddr *sa) {
    return sa->sa_family == AF_INET ? 
        (void*)&(((struct sockaddr_in*)sa)->sin_addr) :
        (void*)&(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void) {
    int sock_fd, new_fd; // listen on sock_fd and create new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes = 1;
    char s[INET6_ADDRSTRLEN];
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags = AI_PASSIVE; // use my IP

    if((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all thr results and bind to the first one we can
    for(p = servinfo; p != NULL; p -> ai_next) {
        if((sock_fd = socket(p->ai_family, p->ai_socktype, 
                p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }
        if(setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, 
                sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }
        if(bind(sock_fd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sock_fd);
            perror("server: bind");
            continue;
        }
        break;
    }

    freeaddrinfo(servinfo);

    if(p == NULL) {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if(listen(sock_fd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if(sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    printf("server: waiting for connections...\n");

    while(1) { // main accept() loop
        sin_size = sizeof(their_addr);
        new_fd = accept(sock_fd, (struct sockaddr *)&their_addr, &sin_size);
        if(new_fd == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr *)&their_addr),
            s, sizeof(s));

        if(!fork()) { // this is the child process
            close(sock_fd); // the chld doesn't need the listener
            char *msg = "Hello world!";
            if(send(new_fd, msg, strlen(msg), 0) == -1)
                perror("send");
            close(new_fd);
            printf("Server: Request received from %s\n", s);
            exit(0);
        }
        close(new_fd); // parent doesn't need this
    }

    return 0;
}