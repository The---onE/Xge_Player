/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�궨�弰���������ں���
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MAIN_H
#define _MAIN_H

#include <windows.h>
#include <shellapi.h>
struct ID3v1;

#define WS_EX_LAYERED 0x00080000
//����������Ϣ��Ӧ
#define WM_TRAY	10000//
//���ո�ʴ��ڹر���Ϣ
#define LRC_CLOSE 10001//
//���ո�ʴ��ڴ���Ϣ
#define LRC_OPEN 10002//
//���ʴ��ڷ���ʱ����Ϣ
#define LRC_TIME 10003//
//���ʹ򿪸�ʴ�����Ϣ
#define LRC_OPENED 10004//
//����������ر���Ϣ
#define PRO_CLOSE 10005//
//�����������ڴ���Ϣ
#define SEA_OPEN 10006//
//�������ô��ڴ���Ϣ
#define SET_OPEN 10007//
//�����������ڹر���Ϣ
#define SEA_CLOSE 10008//
//�������ô��ڹر���Ϣ
#define SET_CLOSE 10009//
//���ʹ�����������Ϣ
#define SEA_OPENED 10010//
//���ո�ʸı���Ϣ
#define LRC_CHANGE 10012//
//�������㴰�ڴ���Ϣ
#define MINI_OPEN 10020//
//���ʹ����㴰����Ϣ
#define MINI_OPENED 10021//
//�������㴰�ڴ���Ϣ
#define MINI_CLOSE 10022//
//���տ�����Ϣ
#define MINI_CONTROL 10025//
//�������㴰�ڴ���Ϣ
#define MINILRC_OPEN 10030//
//�������㴰�ڴ���Ϣ
#define MINILRC_CLOSE 10032//
//����
#define MINI_PLAY 1//
//��ͣ
#define MINI_PAUSE 2//
//ֹͣ
#define MINI_STOP 3//
//����
#define MINI_LAST 4//
//����
#define MINI_NEXT 5//
//����
#define MINI_SOF 6//
//����������
#define MINI_SOUND_SLIDER 7//
//���ȹ�����
#define MINI_MUSIC_SLIDER 8//

//���嵥��ѭ������
#define SELFLOOP 0//
//�����б�ѭ������
#define LISTLOOP 1//
//����������Ŵ���
#define RANDOM 2//

//���岥��ʱ�䶨ʱ�����뼰ʱ��
#define TIMER_PLAY 1
#define TIMER_PLAY_TIME 994//
//����ϵͳʱ���ȡ��ʱ�����뼰ʱ��
#define TIMER_STIME 2
#define TIMER_STIME_TIME 59998//

//�����ݼ�����
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

//��"control.h"��
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

//��"file.h"��
void openf(HWND hwnd);
void Del(HWND hwnd);
void Up(HWND hwnd);
void Down(HWND hwnd);
FILE * SList(int i);
FILE * OFile(int i);
void saveList(HWND hwnd);
void openList(HWND hwnd);//

//��"information.h"��
void ID3V2(HWND hwnd,FILE * fp,ID3v1 ID3V1);
int GetID3Size(FILE * fp);
void fID3V1(HWND hwnd);//

//��"list.h"��
void ListTotal(HWND hwnd);
void ListNow(HWND hwnd);//

//��"mainset.h"��
TCHAR * GetWay(TCHAR * appPath);
FILE * openSetFile();
FILE * saveSetFile();
int SleepTime();//

//��"order.h"��
void MSelf(HWND hwnd);
void Rand(HWND hwnd);//

//��"time_h.h"��
void beginTimeSet(TCHAR ShortName[],HWND hwnd);
int TimeTGet(TCHAR ShortName[],HWND hwnd);
int TimeNGet(TCHAR ShortName[],HWND hwnd);
void NTime(HWND hwnd,int code);//

//��"timer.h"��
void CALLBACK Change(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime);
void CALLBACK STimeF(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime);//

BOOL WINAPI Main_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL Main_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
void Main_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void Main_OnHScroll(HWND hwnd, HWND hwndCtl, UINT code, int pos);
void Main_OnClose(HWND hwnd);

#endif