/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//宏定义及声明歌词窗口函数
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _Lrc_H
#define _Lrc_H

#include <windows.h>

//发送歌词窗口关闭消息
#define LRC_CLOSE 10001//
//发送歌词窗口打开消息
#define LRC_OPEN 10002//
//接收时间信息
#define LRC_TIME 10003//
//接受打开歌词窗口消息
#define LRC_OPENED 10004//
//接收主程序关闭消息
#define PRO_CLOSE 10005//
//发送歌词改变消息
#define LRC_CHANGE 10012//

//窗口吸附边缘最小距离
#define NEAR_DISTANCE 45//
//窗口收缩状态高度
#define NEAR_HEIGHT 3//
//窗口展开状态高度
#define ORIGIN_HEIGHT size.bottom-size.top//
//窗口展开状态宽度
#define ORIGIN_WIDTH size.right-size.left//

//定义检测吸附窗口及刷新歌词定时器代码及时间
#define TIMER_TEST 5
#define TIMER_TEST_TIME 1000//

//定义时间标签最大值 
#define MAX_TIME 6000001//
//定义最大不同歌词数
#define LINE 150//
//定义最大相同歌词数//
#define SAME 5//

//在"lrcmanage.h"中
BOOL manageTag(HWND phwnd,char *q,int j,int k);
void sort();
void manageLrc(HWND hwnd,HWND phwnd,char * c);
void GetSetLrc(HWND hwnd,HWND phwnd,BOOL isrefresh);
void select(HWND hwnd,int musictime);
void CALLBACK LRCTimer(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime);//

BOOL WINAPI Lrc_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL Lrc_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
void Lrc_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void Lrc_OnClose(HWND hwnd);

#endif