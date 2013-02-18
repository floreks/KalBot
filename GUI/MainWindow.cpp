#include "stdafx.h"
#include "MainWindow.h"

HINSTANCE	inj_hModule;	//Injected Modules Handle
HWND	prnt_hWnd;	//Parent Window Handle
HWND	textEdit;
bool logPacket = 0;

LRESULT CALLBACK DLLWindowProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
		switch(wParam)
		{
		case MYMENU_EXIT:
			SendMessage(hwnd, WM_CLOSE, 0, 0);
			break;
		case START_LOG:
			logPacket = 1;
			break;
		case STOP_LOG:
			logPacket = 0;
			break;
		case HACK1:		
			MessageBox(hwnd, "Dummy", "", MB_ICONINFORMATION | MB_OK);
			break;
		case HACK2:		
			MessageBox(hwnd, "Dummy", "", MB_ICONINFORMATION | MB_OK);
			break;
		case HACK3:			
			MessageBox(hwnd, "Dummy", "", MB_ICONINFORMATION | MB_OK);
			break;
		case HACK4:
			MessageBox(hwnd, "Dummy", "", MB_ICONINFORMATION | MB_OK);
			break;
		case HACK5:
			MessageBox(hwnd, "Dummy", "", MB_ICONINFORMATION | MB_OK);
			break;
		case ABOUT:
			MessageBox(hwnd, "Created by : SeaM", "", MB_ICONINFORMATION | MB_OK);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage (0);
		break;
	default:
		return DefWindowProc (hwnd, message, wParam, lParam);
	}
	return 0;
}

BOOL RegisterDLLWindowClass(char szClassName[])
{
	WNDCLASSEX wc;
	wc.hInstance =  inj_hModule;
	wc.lpszClassName = szClassName;
	wc.lpfnWndProc = DLLWindowProc;
	wc.style = CS_DBLCLKS;
	wc.cbSize = sizeof (WNDCLASSEX);
	wc.hIcon = LoadIcon (NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor (NULL, IDC_ARROW);
	wc.lpszMenuName = NULL;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH) COLOR_BACKGROUND;
	if (!RegisterClassEx (&wc))
		return 0;
	return 1;
}

HMENU CreateDLLWindowMenu()
{
	HMENU hMenu;
	hMenu = CreateMenu();
	HMENU hMenuPopup;
	if(hMenu==NULL)
		return FALSE;
	hMenuPopup = CreatePopupMenu();
	AppendMenu (hMenuPopup, MF_STRING, MYMENU_EXIT, TEXT("Exit"));
	AppendMenu (hMenuPopup, MF_STRING, START_LOG, TEXT("Start packet log"));
	AppendMenu (hMenuPopup, MF_STRING, STOP_LOG, TEXT("Stop packet log"));
	AppendMenu (hMenu, MF_POPUP, (UINT_PTR) hMenuPopup, TEXT("File")); 

	hMenuPopup = CreatePopupMenu();
	AppendMenu (hMenuPopup, MF_STRING,HACK1, TEXT("Dummy"));
	AppendMenu (hMenuPopup, MF_STRING,HACK2, TEXT("Dummy"));
	AppendMenu (hMenuPopup, MF_STRING,HACK3, TEXT("Dummy"));
	AppendMenu (hMenuPopup, MF_STRING,HACK4, TEXT("Dummy"));
	AppendMenu (hMenuPopup, MF_STRING,HACK5, TEXT("Dummy"));
	AppendMenu (hMenu, MF_POPUP, (UINT_PTR) hMenuPopup, TEXT("Dummy")); 

	hMenuPopup = CreatePopupMenu();
	AppendMenu (hMenuPopup, MF_STRING,ABOUT, TEXT("Credits"));
	AppendMenu (hMenu, MF_POPUP, (UINT_PTR) hMenuPopup, TEXT("About")); 

	return hMenu;
}

HWND createTextBox(HWND hwnd)
{
	RECT rect;
	GetWindowRect(hwnd,&rect);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	HWND hText = CreateWindowEx( 0, "EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | ES_MULTILINE | ES_WANTRETURN | ES_READONLY | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
		0, 0, width-10, height-30, hwnd, NULL, (HINSTANCE)GetWindowLongA(hwnd,GWL_HINSTANCE), NULL );
	return hText;
}
