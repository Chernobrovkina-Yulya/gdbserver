#ifndef PACKETS_H
#define PACKETS_H

#define MAX_BUF_SIZE 0x8000
#define GDB_EOF (-1)  

static const char kInterruptChar = '\x03';


// struct for packet representation
struct Buf
{
    char data[MAX_BUF_SIZE];
    int len;
};

// buffer function

void PrintInput();
int BufRead(struct Buf *buf);
void BufWrite(struct Buf *pkt, const uint8_t *buf, size_t len);
void BufClear(struct Buf *pkt);

// packet functions

int CheckSum(char *buf, uint32_t len);

int GetPkt(struct Buf *buf);
int PutPkt();

// function to get, put single char

void PutChar(char ch);
int GetChar();

#endif