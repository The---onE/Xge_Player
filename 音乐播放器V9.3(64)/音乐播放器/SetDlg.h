/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�궨�弰�������ô��ں���
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SetDlg_H
#define _SetDlg_H

#include <windows.h>
#include <shellapi.h>

//����������ر���Ϣ
#define PRO_CLOSE 10005//
//�������ô��ڴ���Ϣ
#define SET_OPEN 10007//
//�������ô��ڹر���Ϣ
#define SET_CLOSE 10009//

BOOL WINAPI Set_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL Set_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
void Set_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void Set_OnClose(HWND hwnd);

#endif