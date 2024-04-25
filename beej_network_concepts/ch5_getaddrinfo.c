#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main(void) {
    /* Example 1*/
    int status;

    struct addrinfo hints;
    struct addrinfo *servinfo; // Will point to the results

    memset(&hints, 0, sizeof(hints));   // Make sure the struct is empty
    hints.ai_family = AF_UNSPEC;        // Don't care if IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;    // TCP stream sockets
    hints.ai_flags = AI_PASSIVE;        // Fill in my IP for me, uses address of local host

    if((status = getaddrinfo(NULL, "3490", &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo erro: %s\n", gai_strerror(status));
        // perror("getaddrinfo error: %s\n", gai_strerror(status));
        return 1;
    }

    // servinfo now points to a linked list of 1 or more struct addrinfos
    // do whatever you need to do until you don't need servinfo anymore
    freeaddrinfo(servinfo); // Free the linked list

    /* Example 2 */
    // end of pg 21, start of pg 22
    memset(&hints, 0, sizeof(hints));   // Make sure the struct is empty
    hints.ai_family = AF_UNSPEC;        // Don't care if IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;    // TCP stream sockets

    // use getaddrinfo to get the address info for google.com
    if((status = getaddrinfo("www.google.com", NULL, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        return 1;
    }
}