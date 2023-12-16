#pragma once

// functins for processing GDB queries

void ListenRequests();
void ClientRequest();
void KillProcess();
void Continue();
void ReportException();
void qSupported();
void QueryPacket();
void vPacket();
void vMustReplyEmpty();
void EmptyResp();
