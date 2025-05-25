#include "parser.h"
#include "socket_init.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>

#define BUF_SIZE 65536

volatile sig_atomic_t stop_flag = 0;

void handle_sigint(int sig)
{
    stop_flag = 1;
}

int main(int argc, char *argv[])
{
    signal(SIGINT, handle_sigint);
    
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <interface>\n", argv[0]);
        return 1;
    }

    const char *ifname = argv[1];
    int sock = create_bound_socket(ifname);
    if (sock < 0)
    {
        fprintf(stderr, "Socket initialization failed\n");
        return 1;
    }

    unsigned char buffer[BUF_SIZE];
    printf("Listening on interface: %s\n", ifname);

    while (!stop_flag)
    {
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(sock, &read_fds);

        int ret = select(sock + 1, &read_fds, NULL, NULL, NULL);
        if (ret < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }
            perror("select");
            break;
        }

        if (FD_ISSET(sock, &read_fds))
        {
            ssize_t len = recv(sock, buffer, sizeof(buffer), 0);
            if (len < 0)
            {
                perror("recv");
                break;
            }

            parse_ethernet_frame(buffer, len);
        }
    }

    close(sock);
    printf("Capture ended.\n");
    return 0;
}
