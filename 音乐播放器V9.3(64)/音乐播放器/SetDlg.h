/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//宏定义及声明设置窗口函数
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SetDlg_H
#define _SetDlg_H

#include <windows.h>
#include <shellapi.h>

//接收主程序关闭消息
#define PRO_CLOSE 10005//
//发送设置窗口打开消息
#define SET_OPEN 10007//
//发送设置窗口关闭消息
#define SET_CLOSE 10009//

BOOL WINAPI Set_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL Set_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
void Set_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void Set_OnClose(HWND hwnd);

#endif