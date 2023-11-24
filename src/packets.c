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
    printf("%s\n", input.data);
}

void BufClear(struct Buf *pkt)
{
    pkt->len = 0;
}

int CheckSum(char *buf, uint32_t len)
{
    int checksum = 0;
    while(len--)
        checksum += *buf++;
    return checksum;
}

// Get the next packet from gdb
//store result in struct Buf *buf
// returns 0 if success, 1 otherwise
int GetPkt(struct Buf *buf)
{
    while(1)
    {
        uint8_t checksum = 0;
        int     count = 0;
        int     ch;

        ch = GetChar();
        while(ch != '$')        // wait for the start of packet '$'
        {
            if(ch == -1)
                return 1;       // connection failed
            else
                ch = GetChar();
        }
        while(count < MAX_BUF_SIZE - 1)
        {
            ch = GetChar();
            if(ch == -1)
                return 1;

            if(ch == '$')
            {
                count = 0;
                checksum = 0;
                continue;
            }
            if(ch == '#')       // reach the end of packet
                break;

            checksum += (uint8_t)ch;
            buf->data[count] = (char)ch;
            ++count;
        }
        buf->data[count] = '\0';
        buf->len = count;

        /*if(ch == '#')
        {
            // validate the checksum
            int pkt_checksum;
            ch = GetChar();
            if(ch == -1)
                return 1;
            pkt_checksum = ch;

            ch = GetChar();
            if(ch == -1)
                return 1;
            pkt_checksum += ch;

            if(checksum != pkt_checksum)
                return 1;
        }*/
        return 0;
    }
    return 0;
}

// get a single char from rcp connection
// return the character recieved and -1 on failure
int GetChar()
{
    char buf[1];
    int numread = read(client_sock_fd, buf, sizeof(char));
    if(numread == -1)
        return -1;
    return buf[0] & 0xff;
}

// put a single char out on the rsp connection
void PutChar(char ch)
{
    while(1)
    {
        char buf[1] = {ch};
        int numwritten = write(client_sock_fd, buf, sizeof(char));
        if(numwritten == 0 || numwritten == -1)
            continue;
    }
}