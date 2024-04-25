#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>

int main(void) {
    struct sockaddr_in sa4; // IPv4
    struct sockaddr_in6 sa6; // IPv6
    
    /* Turn IP Addresses into sock_addr in structs */
    int s;
    s = inet_pton(AF_INET, "10.12.110.57", &(sa4.sin_addr)); // IPv4
    if(s <= 0) {
        perror("inet_pton IPv4");
        return 1;
    }
    s = inet_pton(AF_INET6, "2001:db8:63b3:1::3490", &(sa6.sin6_addr)); // IPv6
    if(s <= 0) {
        perror("inet_pton IPv6");
        return 1;
    }

    /* Turn sockaddr_in structs into IP addresses */
    // IPv4:
    char ip4[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(sa4.sin_addr), ip4, INET_ADDRSTRLEN);
    printf("The IPv4 address is: %s\n", ip4);

    // IPv6:
    char ip6[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET6, &(sa6.sin6_addr), ip6, INET6_ADDRSTRLEN);
    printf("The IPv6 address is: %s\n", ip6);

    printf("Hello, World!\n");
    return 0;
}