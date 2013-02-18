/*
Made by BakaBug - http://www.BakaBug.net
don't share it ! or it will get fixed xD
*/

#include <windows.h>
#include <stdio.h>

#ifndef DecryptHeader
#define DecryptHeader

#define _DWORD DWORD
#define _WORD WORD
#define _BYTE BYTE
int DecryptPacketAES(char* buf);
signed int __stdcall DecryptAES(char* source, int size);
int EncryptPacketAES(char* buf);
signed int __stdcall EncryptAES(char* source, int size);
long DecryptTable(unsigned long index, unsigned char *buf, unsigned long len);
long EncryptTable(unsigned long index, unsigned char *buf, unsigned long len);
#endif