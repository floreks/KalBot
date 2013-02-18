#pragma once

// ---------- Data ----------- //
DWORD dwJMPbackRecv = (DWORD)KalTools::getRecvAddress() + 0xF; //The Jump Back address
int ASyncPos=0;
int FinalSize=0;

typedef int (__stdcall *myRecv)(SOCKET s, char *buf, int len, int flags);

extern myRecv oldRecv;
extern bool logPacket;

// ---------- Functions ------------ //
int fRecvIAT(SOCKET s, char *buf, int len, int flags);