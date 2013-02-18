#pragma once

#define MYMENU_EXIT         (WM_APP + 101)
#define HACK1				(WM_APP + 102)
#define HACK2				(WM_APP + 103)
#define HACK3				(WM_APP + 104)
#define HACK4				(WM_APP + 105) 
#define HACK5				(WM_APP + 106) 
#define ABOUT				(WM_APP + 107)
#define START_LOG			(WM_APP + 108)
#define STOP_LOG			(WM_APP + 109)

LRESULT CALLBACK DLLWindowProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL RegisterDLLWindowClass(char szClassName[]);
HMENU CreateDLLWindowMenu();
HWND createTextBox(HWND hwnd);