cmake_minimum_required(VERSION 3.14)
project(beej_network_concepts)

add_executable(ch3_inet ch3_inet.c)
add_executable(ch5_getaddrinfo ch5_getaddrinfo.c)
add_executable(ch5_showip ch5_showip.c)
add_executable(ch6_server ch6_server.c)
add_executable(ch6_client ch6_client.c)
add_executable(ch6_listener ch6_listener.c)
add_executable(ch6_talker ch6_talker.c)
add_executable(tcp_cient_server_multiprocessed tcp_client_server_multiprocessed.c)
add_executable(ch7_poll ch7_poll.c)

# turn on all gcc compiler warnings
if(MSVC)
    add_compile_option(-W4 -WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()