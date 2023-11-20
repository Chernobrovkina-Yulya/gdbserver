#ifndef PACKETS_H
#define PACKETS_H

#define PACKET_BUF_SIZE 0x8000
#define GDB_EOF (-1)  

static const char kInterruptChar = '\x03';

struct Buf
{
    uint8_t buf[PACKET_BUF_SIZE];
    int end;
};

// buffer function

void PrintInput();
int BufRead(struct Buf *buf);
void BufWrite(struct Buf *pkt, const uint8_t *buf, size_t len);
void BufClear(struct Buf *pkt);

// packet functions

int CheckSum(char *buf, uint32_t len);

void ReadDataOnce();
void WriteData();

#endif