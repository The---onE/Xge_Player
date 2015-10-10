/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//宏定义及声明主窗口函数
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MAIN_H
#define _MAIN_H

#include <windows.h>
#include <shellapi.h>
struct ID3v1;

#define WS_EX_LAYERED 0x00080000
//允许托盘消息响应
#define WM_TRAY	10000//
//接收歌词窗口关闭消息
#define LRC_CLOSE 10001//
//接收歌词窗口打开消息
#define LRC_OPEN 10002//
//向歌词窗口发送时间信息
#define LRC_TIME 10003//
//发送打开歌词窗口消息
#define LRC_OPENED 10004//
//发送主程序关闭消息
#define PRO_CLOSE 10005//
//接收搜索窗口打开消息
#define SEA_OPEN 10006//
//接收设置窗口打开消息
#define SET_OPEN 10007//
//接收搜索窗口关闭消息
#define SEA_CLOSE 10008//
//接收设置窗口关闭消息
#define SET_CLOSE 10009//
//发送打开搜索窗口消息
#define SEA_OPENED 10010//
//接收歌词改变消息
#define LRC_CHANGE 10012//
//接收迷你窗口打开消息
#define MINI_OPEN 10020//
//发送打开迷你窗口消息
#define MINI_OPENED 10021//
//接收迷你窗口打开消息
#define MINI_CLOSE 10022//
//接收控制消息
#define MINI_CONTROL 10025//
//接收迷你窗口打开消息
#define MINILRC_OPEN 10030//
//接收迷你窗口打开消息
#define MINILRC_CLOSE 10032//
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

//定义单曲循环代码
#define SELFLOOP 0//
//定义列表循环代码
#define LISTLOOP 1//
//定义随机播放代码
#define RANDOM 2//

//定义播放时间定时器代码及时间
#define TIMER_PLAY 1
#define TIMER_PLAY_TIME 994//
//定义系统时间获取定时器代码及时间
#define TIMER_STIME 2
#define TIMER_STIME_TIME 59998//

//定义快捷键代码
#define HK_Z 9001
#define HK_X 9002
#define HK_S 9003
#define HK_A 9004
#define HK_Q 9005
#define HK_W 9006
#define HK_C 9007
#define HK_D 9008
#define HK_E 9009
#define HK_R 9010
#define HK_G 9011
#define HK_B 9012
#define HK_F 9013
#define HK_V 9014//

//在"control.h"中
void Play(HWND hwnd);
void PlayorContinue();
void Stop(HWND hwnd);
void Pause(HWND hwnd);
void Last(HWND hwnd);
void Next(HWND hwnd);
void Voice(HWND hwnd);
BOOL Sof(HWND hwnd,int onoff,BOOL min,HWND minhwnd);
void NVoice(HWND hwnd);
TCHAR * GSNTE(HWND hwnd);
void playfrom(HWND hwnd,int Time);//

//在"file.h"中
void openf(HWND hwnd);
void Del(HWND hwnd);
void Up(HWND hwnd);
void Down(HWND hwnd);
FILE * SList(int i);
FILE * OFile(int i);
void saveList(HWND hwnd);
void openList(HWND hwnd);//

//在"information.h"中
void ID3V2(HWND hwnd,FILE * fp,ID3v1 ID3V1);
int GetID3Size(FILE * fp);
void fID3V1(HWND hwnd);//

//在"list.h"中
void ListTotal(HWND hwnd);
void ListNow(HWND hwnd);//

//在"mainset.h"中
TCHAR * GetWay(TCHAR * appPath);
FILE * openSetFile();
FILE * saveSetFile();
int SleepTime();//

//在"order.h"中
void MSelf(HWND hwnd);
void Rand(HWND hwnd);//

//在"time_h.h"中
void beginTimeSet(TCHAR ShortName[],HWND hwnd);
int TimeTGet(TCHAR ShortName[],HWND hwnd);
int TimeNGet(TCHAR ShortName[],HWND hwnd);
void NTime(HWND hwnd,int code);//

//在"timer.h"中
void CALLBACK Change(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime);
void CALLBACK STimeF(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime);//

BOOL WINAPI Main_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL Main_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
void Main_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void Main_OnHScroll(HWND hwnd, HWND hwndCtl, UINT code, int pos);
void Main_OnClose(HWND hwnd);

#endif