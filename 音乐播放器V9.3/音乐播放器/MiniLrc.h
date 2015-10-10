/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//宏定义及声明迷你歌词函数
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MiniLrc_H
#define _MiniLrc_H

#include <windows.h>

//接收主程序关闭消息
#define PRO_CLOSE 10005//
//发送迷你窗口打开消息
#define MINILRC_OPEN 10030//
//发送迷你窗口打开消息
#define MINILRC_CLOSE 10032//
//接收时间信息
#define MINILRC_TIME 10033//

//定义检测吸附窗口及刷新歌词定时器代码及时间
#define TIMER_MLTEST 8
#define TIMER_MLTEST_TIME 1000//

//定义时间标签最大值 
#define MAX_TIME 6000001//
//定义每句最大长度
#define MAX_LENGTH 80//
//定义最大不同歌词数
#define LINE 150//
//定义最大相同歌词数//
#define SAME 5//
//定义窗口距桌面底部距离
#define DISTANCE 41//

//在"mlmanage.h"中
void Initialization();
BOOL _manageTag(HWND phwnd,char *q,int j,int k);
void _sort();
void _manageLrc(HWND hwnd,HWND phwnd,char * c);
void _GetSetLrc(HWND hwnd,HWND phwnd);
void _select(HWND hwnd,int musictime);
void CALLBACK MLTimer(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime);//

BOOL WINAPI MiniLrc_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL MiniLrc_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
void MiniLrc_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void MiniLrc_OnClose(HWND hwnd);

#endif