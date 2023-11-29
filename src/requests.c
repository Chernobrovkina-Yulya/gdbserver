#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

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
    case '!':
    // Pequest for extended remote mode
    PackStr("OK", 2);
    printf("request '!' handle\n");
    PutPkt();
    return;

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