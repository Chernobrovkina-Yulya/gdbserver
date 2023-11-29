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

uint8_t CheckSum(struct Buf* pkt)
{
    int len = pkt->len;
    uint8_t checksum = 0;
    for (int i = 0; i < len; ++i)
        checksum += (uint8_t)pkt->data[i];
    return checksum;
}

// Get the next packet from gdb
// store result in struct Buf *buf
// returns 0 if success, 1 otherwise
int GetPkt(struct Buf *buf)
{
    while(1)
    {
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
                continue;
            }
            if(ch == '#')       // reach the end of packet
                break;

            buf->data[count] = (char)ch;
            ++count;
        }
        buf->data[count] = '\0';
        buf->len = count;

        if(ch == '#')
        {
            // validate the checksum
            uint8_t checksum = CheckSum(buf);
            uint8_t pkt_checksum;
            ch = GetChar();
            if(ch == -1)
                return 1;
            pkt_checksum = CharToHex(ch) << 4;

            ch = GetChar();
            if(ch == -1)
                return 1;
            pkt_checksum += CharToHex(ch);

            if(checksum != pkt_checksum)
            {
                perror("Bad checksum in RSP packet");
                PutChar('-');                           // put negative ack
            }
            else
            {
                PutChar('+');                           // put positive ack
                return 0;
            }
        }
        return 0;
    }
    return 0;
}

// put the packet out on the RSP connection
void PutPkt()
{
    uint8_t checksum = CheckSum(&output);
    int len = output.len;
    int ch;
    do
    {
        PutChar('$');                                   // strart of the packet
        for (int count = 0; count < len; ++count)
        {
            int c = output.data[count];
            PutChar(c);
        }
        PutChar('#');                                   // end of the packet
        PutChar(HexToChar(checksum >> 4));              // put the schecksum
        PutChar(HexToChar(checksum % 16));
        ch = GetChar();
    } while (ch != '+');
    BufClear(&output);                                  // clear output buffer
}

// write string in output buffer
void PackStr(char* str, size_t size)
{
    memcpy(output.data, str, size);
    output.data[size] = '\0';
    output.len = size;
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
        else
            return;
    }
}

// utility to mapping a value to hex character
// d - hexidecimal digit. Any non-hex digit returns a NULL char
char HexToChar(uint8_t d)
{
    const char map[] = "0123456789abcdef"
                       "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
                       "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
                       "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
                       "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
                       "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
                       "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
                       "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
                       "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
                       "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
                       "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
                       "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
                       "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
                       "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
                       "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
                       "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
    return map[d];
}

// give the value of a hex char
// return -1 if the char is invalid
uint8_t CharToHex(int ch)
{
    return ((ch >= 'a') && (ch <= 'f')) ? ch - 'a' + 10:
           ((ch >= '0') && (ch <= '9')) ? ch - '0':
           ((ch >= 'A') && (ch <= 'F')) ? ch - 'A' + 10 : -1;
}