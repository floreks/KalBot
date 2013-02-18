#include "stdafx.h"
#include "KalTools.h"
#include "Recv.h"

string recvPacket;

BYTE ignoreRecv[] = {0x38,0x23,0x24,0x25,0x33};

DWORD WINAPI monitorRecv(void *pParams)
{
	recvPacket.clear();
	while(1)
	{
		if(!recvPacket.empty())
		{
			string copy = recvPacket;
			BYTE *p = find(ignoreRecv,ignoreRecv + 7,copy[2]);
			if(*p == 0)
			{
				if(logPacket &&  copy.length() < 68)
				{
					KalTools::LogTextBoxPacket((char*)copy.c_str(),"[RECV] ");
					KalTools::LogTextBox("[RECV] ");
					for(int i=0;i<int(*(PWORD(copy.c_str())));i++)
					{
						KalTools::LogTextBoxNl(" %X ",(BYTE)copy[i]);
						if(i%20==0 && i != 0)
							KalTools::LogTextBox(" ");
					}
				}
			}
			recvPacket.clear();
		}
		Sleep(1);
	}
	return 0;
}

int fRecvIAT(SOCKET s, char *buf, int len, int flags)
{
	if (ASyncPos==FinalSize && FinalSize>0)
	{
		recvPacket.assign(buf,buf+ASyncPos);
		KalTools::recvInterpreter(buf);
		ASyncPos = 0;
	}
	int ret = oldRecv(s,buf,len,flags);
	if (ret<0)
	{
		return ret;
	}
	if (ASyncPos==0)
		FinalSize = *((short int*) buf);
	ASyncPos+=ret;
	return ret;
}