#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "packets.h"

struct Buf input, output;
extern int client_sock_fd;


void PrintInput()
{
    printf("%s\n", (char*)input.buf);
}

void BufWrite(struct Buf *pkt, const uint8_t *buf, size_t len)
{
    if(pkt->end + len >= sizeof(pkt->buf))
    {
        perror("Packet buffer overflow");
        exit(EXIT_FAILURE);
    }
    memcpy(&pkt->buf[pkt->end], buf, len);
    pkt->end += len;
}

void BufClear(struct Buf *pkt)
{
    pkt->end = 0;
}

int CheckSum(char *buf, uint32_t len)
{
    int checksum = 0;
    while(len--)
        checksum += *buf++;
    return checksum;
}

void ReadDataOnce()
{
    uint8_t buf[4096];
    int nread = read(client_sock_fd, buf, sizeof(buf));
    if(nread <= 0)
    {
        perror("Connection closed");
        exit(EXIT_FAILURE);
    }
    BufWrite(&input, buf, nread);
}

void WriteData()
{
    int nwritten = write(client_sock_fd, output.buf, sizeof(output.buf));
    if(nwritten < 0)
    {
        perror("Write error");
        exit(EXIT_FAILURE);
    }
    BufClear(&output);
}