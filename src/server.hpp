#include <cstdint>
#include <string>
#define MAX_BUF_SIZE 0x8000

class CPU;

struct Packet
{
    char data[MAX_BUF_SIZE];
    size_t len;
    uint8_t CheckSum();
};

class GDBServer
{
public:
    void StartServer(int port);
    void StopServer();

    void ProcessRequests();
    void HandleRequest();
    void KillProcess();
    void Continue();
    void ReportException();
    void QueryPacket();
    void vPacket();
    void qSupported();
    void vMustReplyEmpty();
    void EmptyResp();

    void PackStr(std::string str);
    int GetChar();
    void PutChar(char ch);
    void PutPkt();
    int GetPkt();

private:
    int server_sock_fd;
    int client_sock_fd;
    //CPU *cpu;
    Packet pack;
};

// utility functions
char HexToChar(uint8_t d);
uint8_t CharToHex(int ch);