// KalBot.cpp: Definiuje funkcje wyeksportowane dla aplikacji DLL.
//

#include "stdafx.h"
#include "MainWindow.h"
#include "KalTools.h"
#include "Memory.h"

extern HANDLE hThread;
extern HWND prnt_hWnd;
extern HMODULE inj_hModule;
extern HWND textEdit;

extern DWORD WINAPI monitorRecv(void *pParams);
extern DWORD WINAPI monitorSend(void *pParams);
extern DWORD WINAPI monitorStats(void *pParams);

DWORD WINAPI windowThread(LPVOID lpParam)
{
	MSG messages;
	HWND hwnd;
	char *pString = reinterpret_cast<char * > (lpParam);
	HMENU hMenu = CreateDLLWindowMenu();
	if(!(RegisterDLLWindowClass("myWindow")))
		MessageBox(0,"Error registering window class",0,0);
	if(!(prnt_hWnd = FindWindowA(0, "KalOnline")))
		MessageBox(0,"Error finding window",0,0);
	if(!(hwnd = CreateWindowExA (0, "myWindow", pString, WS_EX_PALETTEWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 500, 300, 0, hMenu, inj_hModule, NULL )))
		MessageBox(0,"Error creating window",0,0);
	textEdit = createTextBox(hwnd);
	ShowWindow (hwnd, SW_SHOWNORMAL);
	while (GetMessage (&messages, NULL, 0, 0))
	{
		TranslateMessage(&messages);
		DispatchMessage(&messages);
	}
	return 1;
}

DWORD WINAPI mainThread(void * pParams)
{
	Sleep(2000);
	KalTools::HookIt();
	KalTools::hookIATRecv();
	KalTools::hookIATSend();
	CreateThread(0,NULL,&monitorRecv,NULL,NULL,NULL);
	CreateThread(0,NULL,&monitorSend,NULL,NULL,NULL);
	CreateThread(0,NULL,&monitorStats,NULL,NULL,NULL);

	CloseHandle(hThread);
	_endthreadex(0);
	return NULL;
}