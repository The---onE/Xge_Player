/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//宏定义及声明迷你窗口函数
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _Mini_H
#define _Mini_H

#include <windows.h>

//接收主程序关闭消息
#define PRO_CLOSE 10005//
//发送迷你窗口打开消息
#define MINI_OPEN 10020//
//接收打开迷你窗口消息
#define MINI_OPENED 10021//
//发送迷你窗口打开消息
#define MINI_CLOSE 10022//
//发送控制主窗口消息
#define MINI_CONTROL 10025//
//播放
#define MINI_PLAY 1//
//暂停
#define MINI_PAUSE 2//
//停止
#define MINI_STOP 3//
//上曲
#define MINI_LAST 4//
//下曲
#define MINI_NEXT 5//
//静音
#define MINI_SOF 6//
//音量滚动条
#define MINI_SOUND_SLIDER 7//
//进度滚动条
#define MINI_MUSIC_SLIDER 8//

//窗口吸附边缘最小距离
#define NEAR_DISTANCE 45//
//窗口收缩状态高度
#define NEAR_HEIGHT 3//
//窗口展开状态高度
#define _ORIGIN_HEIGHT _size.bottom-_size.top//
//窗口展开状态宽度
#define _ORIGIN_WIDTH _size.right-_size.left//

//定义检测吸附窗口及刷新进度滚动条位置定时器及时间
#define TIMER_MTEST 6
#define TIMER_MTEST_TIME 497//

void CALLBACK MINITimer(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime);

BOOL WINAPI Mini_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL Mini_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
void Mini_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void Mini_OnHScroll(HWND hwnd, HWND hwndCtl, UINT code, int pos);
void Mini_OnClose(HWND hwnd);

#endif