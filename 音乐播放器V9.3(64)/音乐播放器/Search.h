/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�궨�弰���������������ں���
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _Search_H
#define _Search_H

#include <windows.h>

//����������ر���Ϣ
#define PRO_CLOSE 10005//
//�����������ڴ���Ϣ
#define SEA_OPEN 10006//
//�����������ڹر���Ϣ
#define SEA_CLOSE 10008//
//���մ�����������Ϣ
#define SEA_OPENED 10010//

BOOL WINAPI Search_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL Search_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
void Search_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void Search_OnClose(HWND hwnd);

#endif