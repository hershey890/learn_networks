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

const char* PORT = "3490"; // the port users will be connecting to
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
    return 0;
}