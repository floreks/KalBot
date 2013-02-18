#include "stdafx.h"
#include "KalTools.h"
#include "Memory.h"

// ------------------- REAL_FUNCTIONS ------------------- //
typedef int (__cdecl* OpenDat_org)(char * , int , int , int , int);
typedef void (__cdecl* Notice_org)(char*, int);
typedef int (__cdecl * Chat_org)(char, char*, int);
typedef int (__cdecl *MiniChat_org)(char*,int,int);
typedef int (__stdcall *myRecv)(SOCKET s, char *buf, int len, int flags);
typedef int (__stdcall *mySend)(SOCKET s, char *buf, int len, int flags);
typedef float (__cdecl * Height_org)(float X, float Y); 
typedef int (__cdecl *OnMapClick_org)(int id, float x, float z, float y, int unk);

// Extern data from other files //
extern HWND textEdit;
extern DWORD dwJMPbackRecv;
extern bool logPacket;

extern int fRecvIAT(SOCKET s, char *buf, int len, int flags);
extern int fSendIAT(SOCKET s, char *buf, int len, int flags);

extern unsigned char* DecryptTableData;
// --------------------------- //

myRecv oldRecv = NULL;
mySend oldSend = NULL;

FARPROC KalTools::recvAddress = GetProcAddress(GetModuleHandle("ws2_32.dll"),"recv");
FARPROC KalTools::sendAddress = GetProcAddress(GetModuleHandle("ws2_32.dll"),"send");

DWORD KalTools::chatAdd = 0;
DWORD KalTools::noticeAdd = 0;
DWORD KalTools::sendAdd = 0;
BYTE* KalTools::sendKey = 0;
BYTE* KalTools::tableKey = 0;
DWORD KalTools::syncClient = 0;
DWORD KalTools::charID = 0;
WORD KalTools::curHp = 0;
Player KalTools::myPlayer = {};
DWORD KalTools::heightAdd = 0;
DWORD KalTools::miniChatAdd = 0;
DWORD KalTools::onMapClickAdd = 0;
bool attacking;

void KalTools::Log(string str)
{
	cout << str << '\n';
}

void KalTools::Log(const char* mFormat, ... )
{
	char* mText = new char[255];
	va_list args;
	va_start(args, mFormat);
	vsprintf_s(mText,255,mFormat,args);
	va_end(args);

	printf(mFormat,mText);
}

void KalTools::LogTextBoxBytes(char *str, int len)
{
	stringstream result;
	char *oldText;

	int txtlen=GetWindowTextLength(textEdit);
	if(txtlen > 2500)
		txtlen = 0;
	oldText = new char[txtlen+1];
	if(txtlen == 0)
		oldText[0] = '\0';

	GetWindowText(textEdit,oldText,txtlen);

	result << oldText;
	if(txtlen != 0)
		result << "\r\n";
	for(int i=0;i<len;i++)
	{
		result << hex << static_cast<WORD>(str[i]) << " ";
	}
	result << "\r\n";

	SendMessage(textEdit,WM_SETTEXT,0,(LPARAM)result.str().c_str());
	SendMessage(textEdit, LOWORD(WM_VSCROLL), SB_BOTTOM, 0);

	delete[]oldText;
}

void KalTools::LogTextBoxPacket(char *str, LPCSTR type)
{
	stringstream result;
	char *oldText;

	int txtlen=GetWindowTextLength(textEdit);
	if(txtlen > 2500)
		txtlen = 0;
	oldText = new char[txtlen+1];
	if(txtlen == 0)
		oldText[0] = '\0';

	GetWindowText(textEdit,oldText,txtlen);

	WORD size;
	memcpy(&size,str,2);
	result << oldText;
	if(txtlen != 0)
		result << "\r\n";
	result<< type << "Type[0x" << hex << static_cast<WORD>(str[2])
		<< dec	<< "] Size[" << size << "]\r\n";

	SendMessage(textEdit,WM_SETTEXT,0,(LPARAM)result.str().c_str());
	SendMessage(textEdit, LOWORD(WM_VSCROLL), SB_BOTTOM, 0);

	delete[]oldText;
}

void KalTools::LogTextBoxNl(const char* mFormat, ...)
{
	char* mText = new char[255];
	va_list args;
	va_start(args, mFormat);
	vsprintf_s(mText,255,mFormat,args);
	va_end(args);

	string result;
	char *oldText;
	int txtlen=GetWindowTextLength(textEdit);
	if(txtlen > 5000)
		txtlen = 0;
	oldText = new char[txtlen+1];
	if(txtlen == 0)
		oldText[0] = '\0';
	GetWindowText(textEdit,oldText, txtlen);
	result += oldText;
	result += mText;

	SendMessage(textEdit,WM_SETTEXT,0,(LPARAM)result.c_str());
	SendMessage(textEdit, LOWORD(WM_VSCROLL), SB_BOTTOM, 0);

	delete[]mText;
}

void KalTools::LogTextBox(const char *mFormat, ...)
{
	char* mText = new char[255];
	va_list args;
	va_start(args, mFormat);
	vsprintf_s(mText,255,mFormat,args);
	va_end(args);

	string result;
	char *oldText;
	int txtlen=GetWindowTextLength(textEdit);
	if(txtlen > 5000)
		txtlen = 0;
	oldText = new char[txtlen+1];
	if(txtlen == 0)
		oldText[0] = '\0';
	GetWindowText(textEdit,oldText, txtlen);
	result += oldText;
	if(txtlen != 0)
		result += "\r\n";
	result += mText;

	SendMessage(textEdit,WM_SETTEXT,0,(LPARAM)result.c_str());
	SendMessage(textEdit, LOWORD(WM_VSCROLL), SB_BOTTOM, 0);

	delete[]mText;
}

void KalTools::LogPacket(char *str, LPCSTR type)
{
	string result = "&";
	stringstream sbuf;
	result += type;
	sbuf << result << ' ';
	for(int i=0;i<*(WORD*)str;i++)
	{
		sbuf << hex << (WORD)(BYTE)str[i] << ' ';
	}
	result = sbuf.str();
	result = result.substr(0,50);
	Chat(Color::green,(char*)result.c_str());
}

void KalTools::Chat(int color,char* mFormat,...)
{ 
	char* mText = new char[255];
	va_list args;
	va_start(args, mFormat);
	vsprintf_s(mText,255,mFormat,args);
	va_end(args);

	((Chat_org)chatAdd)(0,mText,color);
	delete[]mText;
}

void KalTools::MiniChat(int color, char *mFormat,...)
{
	char* mText = new char[255];
	va_list args;
	va_start(args, mFormat);
	vsprintf_s(mText,255,mFormat,args);
	va_end(args);

	((MiniChat_org)miniChatAdd)(mText,color,1);
	delete[]mText;
}

void KalTools::Notice(int color,char* mFormat,...)
{
	char* mText = new char[255];
	va_list args;
	va_start(args, mFormat);
	vsprintf_s(mText,255,mFormat,args);
	va_end(args);

	((Notice_org)noticeAdd)(mText,color);
	delete[]mText;
}

void KalTools::OnMapClick(float x, float y, float z)
{
	((OnMapClick_org)onMapClickAdd)(0,x-262143,z,y-262143,0);
}

int KalTools::getHeight(float x, float y)
{
	return (int)(((Height_org)heightAdd)(myPlayer.x-262143,myPlayer.y-262143)*10);
}

void KalTools::HookIt()
{
	// Chat BYTE pattern , char * mask //
	BYTE pChat[] = {0x55,0x8B,0xEC,0x83,0x3D,0x48,0x2B,0x86,0x00,0x00,0x74,0x17,0x8B,0x45,0x10,0x50}; // pattern //
	char * mChat = "xxx????????xxxx"; // mask //

	chatAdd = CMemory::dwFindPattern( 0x00400000,0x00700000,pChat,mChat);
	LogTextBox("[Chat Address]: 0x%x ",chatAdd);

	BYTE pHeight[] = {0x55,0x8B,0xEC,0x81,0xEC,0x94,0x00,0x00,0x00,0xD9,0x45,0x08,0xD8,0x35,0x00,0x00,0x00,0x00,0xD9,0x5D,0x08};
	char *mHeight = "xxxxxxxxxxxxxx????xxx";

	heightAdd = CMemory::dwFindPattern(0x00400000,0x00700000,pHeight,mHeight); 
	LogTextBox("[GetHeight Address]: 0x%x ", pHeight);

	BYTE pMiniChat[] = {0x55,0x8b,0xec,0x51,0x83,0x7d,0x10,0x64,0x75,0x00,0x68,0x00,0x00,0x00,0x00,0xe8,0x00,0x00,0x00,0x00};
	char *mMiniChat = "xxxxxxxxx?x????x????";

	miniChatAdd = CMemory::dwFindPattern(engineStart,engineEnd,pMiniChat,mMiniChat);
	LogTextBox("[MiniChat Address]: 0x%x ",miniChatAdd);

	BYTE pOnMapClick[] = {0x55,0x8b,0xec,0x81,0xec,0x00,0x00,0x00,0x00,0x83, 0x3d,0x00,0x00,0x00,0x00,0x00,0x0f,0x85};
	char * mOnMapClick = "xxxxx????xx?????xx";

	onMapClickAdd = CMemory::dwFindPattern(0x00400000,0x00700000,pOnMapClick,mOnMapClick);
	LogTextBox("[OnMapClick Address]: 0x%x ",onMapClickAdd);

	// Notice BYTE pattern , char * mask //
	BYTE pNotice[] = {0x55,0x8B,0xEC,0x83,0x3D,0x5C,0x2B,0x86,0x00,0x00,0x74,0x34,0x8B,0x45,0x0C,0x50}; // pattern //
	char * mNotice = "xxx???????xxxxxx"; // mask //

	noticeAdd = CMemory::dwFindPattern( 0x00400000,0x00700000,pNotice,mNotice);  // obtain address //
	LogTextBox("[Notice Address]: 0x%x ",noticeAdd);

	DWORD dwEngineSendA = CMemory::dwFindPattern(0x401000,0x2bc000,(BYTE*)"\x55\x8B\xEC\x83\xEC\x18\x83\x3D\x00\x00\x00\x00\x00\x00\x00\x33\xC0","xxxxxxxx???????xx");
	sendAdd = CMemory::dwFindPattern(dwEngineSendA+1,0x2bc000,(BYTE*)"\x55\x8B\xEC\x83\xEC\x18\x83\x3D\x00\x00\x00\x00\x00\x00\x00\x33\xC0","xxxxxxxx???????xx");
	LogTextBox("[Send Address]: 0x%x ",sendAdd);

	tableKey = (BYTE*)*((DWORD*)(sendAdd+0xCA)); 
	LogTextBox("[Table Key]: 0x%X ",*tableKey);

	CMemory::placeJMP((BYTE*)&KalTools::SendEngine,sendAdd,sizeof(&KalTools::SendEngine));
}

// ------------- Hooking recv --------------- //

__declspec(naked) void fRecv()
{
	char *buf;

	__asm PUSHAD
	__asm PUSHFD

	// Getting data from func
	__asm mov eax, DWORD PTR [ebp+0xC] // buf pointer
	__asm MOV buf, eax

	if(logPacket)
	{
		KalTools::LogTextBoxPacket(buf,"Server->Client: ");
		KalTools::recvInterpreter(buf);
	}

	__asm POPFD
	__asm POPAD

	__asm SUB ESP, 0x18
	__asm PUSH EBX
	__asm JMP dwJMPbackRecv
} 

void KalTools::hookRecv()
{
	CMemory::placeJMP((BYTE*)(DWORD)recvAddress+0xA, (DWORD)fRecv, 5);
}

void KalTools::hookIATRecv()
{
	PDWORD address;
	address = reinterpret_cast<PDWORD>(recvIAT);
	oldRecv = (myRecv)*address;
	DWORD oldprot, oldprot2;

	VirtualProtect(address, sizeof(DWORD), PAGE_READWRITE, (DWORD *)&oldprot);
	*address = (DWORD)fRecvIAT;
	VirtualProtect(address, sizeof(DWORD), oldprot, (DWORD *)&oldprot2);
}
// ------ End of recv hooks ------ //

// ------ Send hooks ------- //

void KalTools::hookIATSend()
{
	PDWORD address;
	address = reinterpret_cast<PDWORD>(sendIAT);
	oldSend = (mySend)*address;
	DWORD oldprot, oldprot2;

	VirtualProtect(address, sizeof(DWORD), PAGE_READWRITE, (DWORD *)&oldprot);
	*address = (DWORD)fSendIAT;
	VirtualProtect(address, sizeof(DWORD), oldprot, (DWORD *)&oldprot2);
}

// ------ End of send hooks ----- //

void KalTools::recvInterpreter(char *packet)
{
	Player player;
	Monster monster;
	int namelen;
	BYTE header = packet[2];
	DWORD monsterID;
	switch(header)
	{
	case RPacket::welcome:
		sendKey = (BYTE*)*((DWORD*)(sendAdd+0xA5));
		LogTextBox("Send key captured: 0x%X ",*sendKey);
		Login("test","test","00000000");
		/*for(int i=0;i<540;i++)
		{
		LogTextBoxNl(" 0x%X, ",*(sendKey+i));  // dumping AES key table
		}
		LogTextBox(" ");*/
		break;
	case RPacket::playerAppear:
		memcpy((void*)&player.id,(void*)((DWORD)packet+3),4); // player id
		memcpy((void*)&player.name,(void*)((DWORD)packet+7),16); // player name
		namelen = strlen(player.name);
		LogTextBox("Found new player: %s ",player.name);
		memcpy(&player.classe,(packet+8+namelen),1); // player class
		memcpy(&player.x,(packet+9+namelen),4); // player x
		memcpy(&player.y,(void*)((DWORD)packet+13+namelen),4); // player y
		memcpy(&player.z,(packet+17+namelen),2); // player z
		if(!strcmp(player.name,"SeaM"))
		{
			myPlayer.id = player.id;
			myPlayer.x = player.x;
			myPlayer.y = player.y;
			myPlayer.z = player.z;
			LogTextBox("Assigned main player id: %d ",myPlayer.id);
		}
		if(namelen>2)
		{
			char playerClass[7];
			switch(player.classe)
			{
			case 0:
				strcpy_s(playerClass,"Knight");
				break;
			case 1:
				strcpy_s(playerClass,"Mage");
				break;
			case 2:
				strcpy_s(playerClass,"Archer");
				break;
			case 3:
				strcpy_s(playerClass,"Thief");
			default:
				strcpy_s(playerClass,"????");
			}
			Chat(Color::pink," Player: %s ID: %d",player.name,player.id);
			Chat(Color::pink," Class: %s X: %d Y: %d Z: %d",playerClass,player.x,player.y,player.z);
		}
		break;
	case RPacket::mobAppear:
		memcpy(&(monster.classe),(packet+3),2);
		memcpy(&(monster.id),(packet+5),4);
		memcpy(&(monster.x),(packet+9),4);
		memcpy(&(monster.y),(packet+13),4);
		memcpy(&(monster.z),(packet+17),2);
		memcpy(&(monster.HP),(packet+19),2);
		Chat(Color::violett," Mob: %d(%d) X: %d Y: %d HP: %d",monster.classe,monster.id,monster.x,monster.y,monster.HP);
		if(!attacking)
		{
			OnMapClick(monster.x,monster.y,0);
			attacking = 1;
		}
		break;
	case RPacket::charInfo:
		charID = *(DWORD*)&packet[9];
		LogTextBox("Char found id: %d ",charID);
		//SendEngine(0x0a,"dd",charID,0);
		//SendEngine(0x0b,"b",1);
		break;
	case RPacket::stateChange:
		if(packet[3] == 0x07)
			curHp    = *(WORD *)&packet[4];
		break;
	case RPacket::mobAnimation:
		monsterID = *(DWORD*)&packet[7];
		if(*(DWORD*)&packet[7] == myPlayer.id)
		{
			LogTextBox("Attacking monster id: %d ", *(DWORD*)&packet[3]);
			SendEngine(0x0D,"b",0xD);
			SendEngine(0x0C,"bdd",1,*(DWORD*)&packet[3],0);
			attacking = 0;
		}
		SendEngine(0x0D,"bbd",1,1,monsterID);
	}
}

void KalTools::sendInterpreter(char *packet)
{
	BYTE header = packet[2];
	string message;
	switch(header)
	{
	case SPacket::checkVer:
		memcpy(&syncClient,packet+3,2);
		LogTextBox("Sync client: 0x%X ",syncClient);
		break;
	case SPacket::chat:
		message.assign(packet+7,packet + *(PWORD)(packet));
		if(message[0] == '/')
		{
			if(message.compare(0,6,"/bh on")==0)
				MiniChat(Color::red,"Behead on.");
			if(message.compare(0,7,"/bh off")==0)
				MiniChat(Color::red,"Behead off.");
			if(message.compare(0,7,"/sh set")==0)
				MiniChat(Color::red,"Speed has been set on ...");
			if(message.compare(0,8,"/pick on")==0)
				MiniChat(Color::red,"Pick hack on.");
			if(message.compare(0,9,"/pick off")==0)
				MiniChat(Color::red,"Pick hack off.");
		}
	}
}

void KalTools::Login(string id, string pass, string secPass)
{
	SendEngine(0x02,"ss",id.c_str(),pass.c_str());
	Sleep(2000);
	SendEngine(0x75,"bs",0x0,secPass.c_str());
}
