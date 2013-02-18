#include "stdafx.h"
#include "Memory.h"

using namespace std;

void CMemory::myAllocConsole(wstring consoleName)
{
	AllocConsole();

	int HandleIn = _open_osfhandle((long)GetStdHandle(STD_INPUT_HANDLE), _O_TEXT);
	int HandleOut = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);

	FILE *In = _fdopen(HandleIn, "r");
	FILE *Out = _fdopen(HandleOut, "w");

	*stdin = *In;
	*stdout = *Out;

	SetConsoleTitleW(consoleName.c_str());
}

bool CMemory::bDataCompare(const BYTE* pData, const BYTE* bMask, const char* szMask)
{
	for(;*szMask;++szMask,++pData,++bMask)
		if(*szMask=='x' && *pData!=*bMask ) 
			return false;
	return (*szMask) == NULL;
}

DWORD CMemory::dwFindPattern(DWORD dwAddress,DWORD dwLen,BYTE *bMask,char * szMask)
{
	for(DWORD i=0; i < dwLen; i++)
		if( CMemory::bDataCompare( (BYTE*)( dwAddress+i ),bMask,szMask) )
			return (DWORD)(dwAddress+i);

	return 0;
}


LPVOID CMemory::MemcpyEx(DWORD lpDest, DWORD lpSource, int len)
{
	DWORD oldSourceProt,oldDestProt=0;
	VirtualProtect((LPVOID)lpSource,len,PAGE_EXECUTE_READWRITE,&oldSourceProt);
	VirtualProtect((LPVOID)lpDest,len,PAGE_EXECUTE_READWRITE,&oldDestProt);
	memcpy((void*)lpDest,(void*)lpSource,len);
	VirtualProtect((LPVOID)lpDest,len,oldDestProt,&oldDestProt);
	VirtualProtect((LPVOID)lpSource,len,oldSourceProt,&oldSourceProt);
	return (LPVOID)lpDest;
}

void CMemory::placeJMP(BYTE *pAddress, DWORD dwJumpTo, DWORD dwLen){
	DWORD dwOldProtect, dwBkup, dwRelAddr;

	VirtualProtect(pAddress, dwLen, PAGE_EXECUTE_READWRITE, &dwOldProtect);

	dwRelAddr = (DWORD) (dwJumpTo - (DWORD) pAddress) - 5;   
	*pAddress = 0xE9; // placing JMP opcode at dest address
	*((DWORD *)(pAddress + 0x1)) = dwRelAddr; // placing JMP address to our hooked func
	// Overwrite the rest of the bytes with NOPs
	for(DWORD x = 0x5; x < dwLen; x++)
		*(pAddress + x) = 0x90; 

	VirtualProtect(pAddress, dwLen, dwOldProtect, &dwBkup);
}