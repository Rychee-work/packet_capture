#include "parser.h"

#include <stdio.h>

#include <arpa/inet.h>
#include <netinet/if_ether.h>


void print_mac(const unsigned char *mac)
{
    for (int i = 0; i < 6; i++)
    {
        printf("%02x", mac[i]);
        if (i < 5) printf(":");
    }
}

void parse_ethernet_frame(const unsigned char *buffer, ssize_t len)
{
    if (len < sizeof(struct ethhdr))
        return;

    struct ethhdr *eth = (struct ethhdr *)buffer;
    printf("Packet %4ld bytes | ", len);
    printf("Src MAC: "); print_mac(eth->h_source);
    printf(" -> Dst MAC: "); print_mac(eth->h_dest);
    printf(" | EtherType: 0x%04x\n", ntohs(eth->h_proto));
}
