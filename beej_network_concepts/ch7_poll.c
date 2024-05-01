/**
 * @brief TCP server to multiple clients using poll() 
 * 
 * Note does not scale well to large number of clients
 * Use libevent instead.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>

int main(void) {

}