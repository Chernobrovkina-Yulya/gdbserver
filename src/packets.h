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
void BufClear(struct Buf *pkt);

// packet functions

uint8_t CheckSum(struct Buf* pkt);

int GetPkt(struct Buf *buf);
void PutPkt();
void PackStr(char *str, size_t size);

// function to get, put single char

void PutChar(char ch);
int GetChar();

// conversion functions
char HexToChar(uint8_t d);
uint8_t CharToHex(int ch);

#endif