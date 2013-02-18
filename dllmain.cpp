// dllmain.cpp: Okreœla punkt wejœcia dla aplikacji DLL.
#include "stdafx.h"

extern DWORD WINAPI mainThread(void *pParams);
extern DWORD WINAPI windowThread(LPVOID lpParam);

extern HINSTANCE inj_hModule;
extern HWND prnt_hWnd;

HANDLE hThread = INVALID_HANDLE_VALUE;
DWORD threadID;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		hThread = CreateThread(NULL, 0, mainThread, NULL, NULL, &threadID); 
		inj_hModule = hModule;
		CreateThread(0, NULL, windowThread, (LPVOID)"Packet Sniffer", NULL, NULL);
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}