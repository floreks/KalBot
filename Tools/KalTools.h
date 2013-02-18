#pragma once
#include "GameStruct.h"
using namespace std;

// ----------- End of prototypes -------------- //

#define engineStart 0x00400000
#define engineEnd 0x00700000

#define recvIAT 0x00725468
#define sendIAT 0x0072546C

class KalTools
{
private:
	static FARPROC recvAddress;
	static FARPROC sendAddress;

	static DWORD chatAdd;
	static DWORD noticeAdd;
	static DWORD heightAdd;
	static DWORD miniChatAdd;
	static DWORD onMapClickAdd;
	static DWORD sendAdd;
	static BYTE* sendKey;
	static BYTE* tableKey;
	static DWORD syncClient;
	static DWORD charID;
	static WORD curHp;
	static Player myPlayer;
public:

	KalTools(){}
	~KalTools(){}

	// Getters //
	static FARPROC getRecvAddress(){return recvAddress;}
	static DWORD getSendAddress(){return sendAdd;}
	static BYTE* getSendKey(){return sendKey;}
	static BYTE* getTableKey(){return tableKey;}
	static DWORD getSyncClient(){return syncClient;}
	static DWORD getCharID(){return charID;}
	static WORD getCurHP(){return curHp;}
	static Player getMyPlayer(){return myPlayer;}

	static void hookRecv();

	static void hookIATRecv();
	static void hookIATSend();

	static void Log(string str);
	static void Log(const char* format, ...);
	static void LogPacket(char *str, LPCSTR type);
	static void send(DWORD type, LPCSTR format...);
	static void LogTextBoxPacket(char* text, LPCSTR type);
	static void LogTextBox(const char* mFormat, ...);
	static void LogTextBoxBytes(char *text, int len);
	static void LogTextBoxNl(const char *mFormat, ...);

	static void OpenDat(char * dat,int x,int y,int w, int h);
	static void Chat(int color,char* mFormat,...);
	static void Notice(int color,char* mFormat,...);
	static void MiniChat(int color,char *mFormat,...);
	static void SendEngine(DWORD Header,LPCSTR szFormat,...){}
	static void Login(string id, string password, string secPass);
	static void OnMapClick(float x, float y, float z);
	static int getHeight(float x, float y);

	static void HookIt();
	static void sendInterpreter(char *buf);
	static void recvInterpreter(char *buf);
};

