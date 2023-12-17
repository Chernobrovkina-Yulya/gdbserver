#include <iostream>
#include <string.h>

#include "server.hpp"

void GDBServer::ProcessRequests()
{
    while(true)
    {
        HandleRequest();
    }
}

void GDBServer::HandleRequest()
{
    if(GetPkt())
    { 
        perror("Communication failure");
        exit(EXIT_FAILURE);
    }

    std::cout << "packet from GDB: "<< pack.data << '\n';

    switch (pack.data[0])
    {
    case 'H':
        // request for setting thread for opertaions ('m', 'g', etc)
        //  на дпнный момент говорим GDB выбрать любой поток
        PackStr("");
        PutPkt();
        std::cout << "'H' command handle\n";
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
        PackStr("OK");
        std::cout << "request '!' handle\n";
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
        std::cout << "Warning: unknown RSP request: " << pack.data << '\n';
        break;
    }
}

void GDBServer::KillProcess()
{
    std::cout << "kill request recieved: " << pack.data << '\n';
}

void GDBServer::Continue()
{
    std::cout << "'c' request recieved: " << pack.data << '\n';
}

void GDBServer::ReportException()
{
    std::cout << "'?' request recieved: " << pack.data << '\n';
}

// handle query packets, recieved from GDB

void GDBServer::QueryPacket()
{
    if(strstr(pack.data, "qSupported"))
        qSupported();
    else
    {
        std::cout << "unknown query packet recieved: " << pack.data << '\n';
        EmptyResp();
    }
}

//handle v packets, recieved from GDB

void GDBServer::vPacket()
{
    if(strstr(pack.data, "vMustReplyEmpty"))
        vMustReplyEmpty();
    else
    {
        std::cout << "unknown 'v' packet recieved:" << pack.data << '\n';
        EmptyResp();
    }
}

// recieve a set of supported features from GDB and send server's set in response

void GDBServer::qSupported()
{
    if (strstr(pack.data, "hwbreak+;"))
    {
        PackStr("hwbreak+;");
        PutPkt();
        std::cout << "qSupported command handle\n";
    }
}

// tells GDB that the response to an unknown 'v' packet is empty string

void GDBServer::vMustReplyEmpty()
{
    EmptyResp();
    std::cout << "vMustReplyEmpty command handle\n";
}

void GDBServer::EmptyResp()
{
    PackStr("");
    PutPkt();
}
