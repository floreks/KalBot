#include "stdafx.h"
#include "KalTools.h"
#include "Send.h"

typedef int (__stdcall *mySend)(SOCKET s, char *buf, int len, int flags);

extern bool logPacket;
extern mySend oldSend;
extern long DecryptTable(unsigned long index, unsigned char *buf, unsigned long len);
extern int DecryptPacket(char* buf);

string sendPacket;
BYTE ignoreSend[] = {0x03,0x11};

DWORD WINAPI monitorSend(void *pParams)
{
	sendPacket.clear();
	while(1)
	{
		if(!sendPacket.empty())
		{
			string copy = sendPacket;
			if(KalTools::getTableKey() != 0)
			{
				DWORD tKey = ((*KalTools::getTableKey())-1) & 0x3F;
				DecryptTable(tKey,(unsigned char*)copy.c_str()+2,copy.length()-2);
				DecryptPacket((char*)copy.c_str());
				BYTE *p = find(ignoreSend,ignoreSend + 3,copy[2]);
				if(*p == 0)
				{
					KalTools::sendInterpreter((char*)copy.c_str());
					if(logPacket && copy.length() < 50)
					{
						KalTools::LogTextBoxPacket((char*)copy.c_str(),"[SEND] ");
						KalTools::LogTextBox("[SEND] ");
						for(int i=0;i<int(*(PWORD(copy.c_str())));i++)
						{
							KalTools::LogTextBoxNl(" %X ",(BYTE)copy[i]);
							if(i%20==0 && i != 0)
								KalTools::LogTextBox(" ");
						}
					}
				}
			}
		}
		sendPacket.clear();
		Sleep(1);
	}
	return 0;
}

int fSendIAT(SOCKET s, char *buf, int len, int flags)
{
	sendPacket.assign(buf,buf+len);
	KalTools::LogPacket(buf,"[SEND]");
	return oldSend(s,buf,len,flags);
}