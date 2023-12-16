#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "requests.h"
#include "packets.h"

extern struct Buf input, output;


// main function - request handler
void ClientRequest()
{
    if(GetPkt(&input))
    { 
        perror("Communication failure");
        exit(EXIT_FAILURE);
    }
    switch (input.data[0])
    {
    case 'H':
        // request for setting thread for opertaions ('m', 'g', etc)
        //  на дпнный момент говорим GDB выбрать любой поток
        PackStr("", 0);
        PutPkt();
        printf("'H' command handle\n");
        break;

    case 'v':
        // v packet recieved
        vPacket();
        break;
         
    case 'q':
        // general query packet recieved
        QueryPacket();
        break;

    case '!':
        // Pequest for extended remote mode
        PackStr("OK", 2);
        printf("request '!' handle\n");
        PutPkt();
        break;

    case '?':
        //return last signal id
        ReportException();
        break;
    case 'k':
        // kill request
        KillProcess();
        break;
    case 'c':
        // continue
        Continue();
        break;
    default:
        // unknown requests are ignored
        printf("Warning: unknown RSP request: %s", input.data);
        break;
    }
    PrintInput();
}

// loop to listen for GDB client requests
void ListenRequests()
{
    while(1)
    {
        ClientRequest();
    }
}

void KillProcess()
{
    printf("kill request recieved: %s\n", input.data);
}

void Continue()
{
    printf("'c' request recieved: %s\n", input.data);
}

void ReportException()
{
    printf("'?' request recieved: %s\n", input.data);
}

// handle query packets, recieved from GDB

void QueryPacket()
{
    if(strstr(input.data, "qSupported"))
        qSupported();
    else
    {
        printf("unknown query packet recieved\n");
        EmptyResp();
    }
}

//handle v packets, recieved from GDB

void vPacket()
{
    if(strstr(input.data, "vMustReplyEmpty"))
        vMustReplyEmpty();
    else
    {
        printf("unknown 'v' packet\n");
        EmptyResp();
    }
}

// recieve a set of supported features from GDB and send server's set in response

void qSupported()
{
    if (strstr(input.data, "hwbreak+;"))
    {
        PackStr("hwbreak+;", 9);
        PutPkt();
        printf("qSupported command handle\n");
    }
}

// tells GDB that the response to an unknown 'v' packet is empty string

void vMustReplyEmpty()
{
    EmptyResp();
    printf("vMustReplyEmpty command handle\n");
}

void EmptyResp()
{
    PackStr("", 0);
    PutPkt();
}
