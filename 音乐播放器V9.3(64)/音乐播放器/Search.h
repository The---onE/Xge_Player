/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//宏定义及声明搜索歌曲窗口函数
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _Search_H
#define _Search_H

#include <windows.h>

//接收主程序关闭消息
#define PRO_CLOSE 10005//
//发送搜索窗口打开消息
#define SEA_OPEN 10006//
//发送搜索窗口关闭消息
#define SEA_CLOSE 10008//
//接收打开搜索窗口消息
#define SEA_OPENED 10010//

BOOL WINAPI Search_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL Search_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
void Search_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void Search_OnClose(HWND hwnd);

#endif