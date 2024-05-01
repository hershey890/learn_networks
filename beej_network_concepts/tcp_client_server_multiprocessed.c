/**
 * @brief Vanilla TCP client server with 2 processes via fork()
 */
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <netdb.h>
#include <sys/wait.h>
#include <arpa/inet.h>

const char *PORT = "8888";
const char *HOST = "localhost";
const char BACKLOG = 10; // how many pending connections queue will hold
const size_t MAXDATASIZE = 100; // max number of bytes we can get at once

void errExit(const char* msg) {
    perror(msg);
    exit(1);
}

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

void server(struct addrinfo *servinfo, struct addrinfo *rp, int sock_fd) {
    for(rp = servinfo; rp != NULL; rp = rp->ai_next) {
        sock_fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if(sock_fd == -1)
            continue;
        if(bind(sock_fd, rp->ai_addr, rp->ai_addrlen) == 0)
            break; // success
        close(sock_fd);
    }

    freeaddrinfo(servinfo);

    if(rp == NULL)
        errExit("bind");
        if(listen(sock_fd, BACKLOG) == -1)
            errExit("listen");

    printf("Server: waiting for connections...\n");

    struct sockaddr_storage their_addr;
    socklen_t addr_size = sizeof(their_addr);
    
    // accept the connection
    int new_fd = accept(sock_fd, (struct sockaddr*)&their_addr, &addr_size);
    if(new_fd == -1)
        errExit("accept");

    char t[INET6_ADDRSTRLEN];
    inet_ntop(their_addr.ss_family, 
        get_in_addr((struct sockaddr*)&their_addr), t, sizeof(t));

    char *msg = "Hello world!";
    if(send(new_fd, msg, strlen(msg), 0) == -1)
        errExit("send");
    close(new_fd);
    
    printf("Server: got connection from %s\n", t);

    wait(NULL);
}

void client(struct addrinfo *servinfo, struct addrinfo *rp, int sock_fd) {
    for(rp = servinfo; rp != NULL; rp = rp->ai_next) {
        sock_fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if(sock_fd == -1)
            continue;
        if(connect(sock_fd, rp->ai_addr, rp->ai_addrlen) == -1) {
            close(sock_fd);
            perror("client: connect");
            continue;
        }
        break; // success
    }

    if(rp == NULL)
        errExit("bind");

    char t[INET6_ADDRSTRLEN];
    inet_ntop(rp->ai_family, get_in_addr((struct sockaddr *)rp->ai_addr),
        t, sizeof(t));
    printf("client: connecting to %s\n", t);
    
    freeaddrinfo(servinfo);

    char buf[MAXDATASIZE];
    int numbytes;
    if((numbytes = recv(sock_fd, buf, MAXDATASIZE-1, 0)) == -1) {
        perror("recv");
        exit(1);
    }
    buf[numbytes] = '\0';

    printf("client: received '%s'\n", buf);

    close(sock_fd);
}

int main(void) {        
    /* Create a new socket */
    struct addrinfo hints, *servinfo, *rp;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags = AI_PASSIVE; // use my IP
    int s = getaddrinfo(HOST, PORT, &hints, &servinfo);
    if(s) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(1);
    }
    int sock_fd;

    /* Reap all dead processes */
    struct sigaction sa = {.sa_handler = sigchld_handler};
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if(sigaction(SIGCHLD, &sa, NULL) == -1)
        errExit("sigaction");

    /* Spawn a child process to act as the client  */
    pid_t pid = fork();
    if(pid < 0) {
        errExit("fork");
    } else if (pid > 0) {
        server(servinfo, rp, sock_fd);
    } else {
        sleep(1);
        printf("Child process - client\n");
        client(servinfo, rp, sock_fd);
        exit(0);
    }

    return 0;
}