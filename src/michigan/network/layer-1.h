#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>

/* the L2 protocols */
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <netinet/if_ether.h>
#include <linux/if.h>
#include <sys/ioctl.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <linux/if_link.h>

#ifndef LAYER_1_H
#define LAYER_1_H

#define MAX_FRAME_LENGTH 2342



int sock;
unsigned char bcast_mac[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
enum{L1_OK, L1_TIMEOUT = -1};

/* TX STUFFS */
char txbuff[MAX_FRAME_LENGTH];
char rxbuff[MAX_FRAME_LENGTH];

struct ethhdr * eh = (struct ethhdr *) txbuff;
struct sockaddr_ll broadcast;

static int sock_raw_init(const char * DEVICE, int protocol) {
    struct ifreq ifr;

    sock = socket(PF_PACKET, SOCK_RAW, htons(protocol));
    if (sock == -1) {
        perror("socket():");
        return 1;
    }

    strncpy(ifr.ifr_name, DEVICE, IFNAMSIZ);
    if (ioctl(sock, SIOCGIFINDEX, &ifr) == -1) {
        perror("SIOCGIFINDEX");
        return 1;
    }

    int if_index = ifr.ifr_ifindex;

    if (ioctl(sock, SIOCGIFHWADDR, &ifr) == -1) {
        perror("SIOCGIFINDEX");
        return 1;
    }

    fprintf(stderr,"Using device %s\n",DEVICE);

    /* bind socket to interface to receive frame ONLY from that interface */
    struct sockaddr_ll sll;
    sll.sll_family = AF_PACKET;
    sll.sll_ifindex = if_index;
    sll.sll_protocol = htons(protocol);
    if ((bind(sock, (struct sockaddr *) &sll, sizeof(sll))) == -1) {
        perror("bind(): ");
        return 1;
    }

    /* Prepare TX frame */
    broadcast.sll_pkttype = PACKET_BROADCAST;
    broadcast.sll_family = PF_PACKET;
    broadcast.sll_protocol = htons(protocol);
    broadcast.sll_ifindex = if_index;
    broadcast.sll_halen = ETH_ALEN;

    eh->h_proto = htons(protocol);
    memcpy((void *) eh->h_source, (void*) ifr.ifr_hwaddr.sa_data, ETH_ALEN);
    memcpy((void *) eh->h_dest  , (void*) bcast_mac, ETH_ALEN);

    return 0;
}

char * getTxBuffer(){
    return &txbuff[ETHER_HDR_LEN];
}

char * getRxBuffer(){
    return &rxbuff[ETHER_HDR_LEN];
}

int send(int size){
    return sendto(sock, txbuff, size + ETHER_HDR_LEN, 0, (struct sockaddr*) &broadcast, sizeof(broadcast));
}

int receive(int timeout = 0){
    int r;
    if (timeout > 0){
        struct timeval tv;
        fd_set fd_rx;
        tv.tv_sec = 0;
        tv.tv_usec = 1000 * timeout;
        FD_ZERO(&fd_rx);
        FD_SET(sock, &fd_rx);
        r = select(FD_SETSIZE, &fd_rx, NULL, NULL, &tv);
    }else{
        r = 1;
    }
    if (r) {
        int size = recvfrom(sock, rxbuff, MAX_FRAME_LENGTH, 0, 0, 0);
        return size - ETH_HLEN;
    } else {
        return L1_TIMEOUT;
    }
}

char * getRawRxBuffer(){
    return rxbuff;
}
#endif
