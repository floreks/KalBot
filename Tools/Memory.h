#pragma once

using namespace std;

class CMemory
{
private:
	int g_hCrtIn;
	int g_hCrtOut;
public:
	CMemory(){}
	~CMemory(){}

	static void myAllocConsole(wstring consoleName);
	static bool bDataCompare(const BYTE* pData, const BYTE* bMask, const char* szMask);
	static DWORD dwFindPattern(DWORD dwAddress,DWORD dwLen,BYTE *bMask,char * szMask);
	static LPVOID MemcpyEx(DWORD lpDest, DWORD lpSource, int len);
	static void placeJMP(BYTE *pAddress, DWORD dwJumpTo, DWORD dwLen);
};

