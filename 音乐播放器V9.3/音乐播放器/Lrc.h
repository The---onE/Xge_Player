/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�궨�弰������ʴ��ں���
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _Lrc_H
#define _Lrc_H

#include <windows.h>

//���͸�ʴ��ڹر���Ϣ
#define LRC_CLOSE 10001//
//���͸�ʴ��ڴ���Ϣ
#define LRC_OPEN 10002//
//����ʱ����Ϣ
#define LRC_TIME 10003//
//���ܴ򿪸�ʴ�����Ϣ
#define LRC_OPENED 10004//
//����������ر���Ϣ
#define PRO_CLOSE 10005//
//���͸�ʸı���Ϣ
#define LRC_CHANGE 10012//

//����������Ե��С����
#define NEAR_DISTANCE 45//
//��������״̬�߶�
#define NEAR_HEIGHT 3//
//����չ��״̬�߶�
#define ORIGIN_HEIGHT size.bottom-size.top//
//����չ��״̬���
#define ORIGIN_WIDTH size.right-size.left//

//�������������ڼ�ˢ�¸�ʶ�ʱ�����뼰ʱ��
#define TIMER_TEST 5
#define TIMER_TEST_TIME 1000//

//����ʱ���ǩ���ֵ 
#define MAX_TIME 6000001//
//�������ͬ�����
#define LINE 150//
//���������ͬ�����//
#define SAME 5//

//��"lrcmanage.h"��
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