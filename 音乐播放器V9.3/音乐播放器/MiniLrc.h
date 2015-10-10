/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�궨�弰���������ʺ���
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MiniLrc_H
#define _MiniLrc_H

#include <windows.h>

//����������ر���Ϣ
#define PRO_CLOSE 10005//
//�������㴰�ڴ���Ϣ
#define MINILRC_OPEN 10030//
//�������㴰�ڴ���Ϣ
#define MINILRC_CLOSE 10032//
//����ʱ����Ϣ
#define MINILRC_TIME 10033//

//�������������ڼ�ˢ�¸�ʶ�ʱ�����뼰ʱ��
#define TIMER_MLTEST 8
#define TIMER_MLTEST_TIME 1000//

//����ʱ���ǩ���ֵ 
#define MAX_TIME 6000001//
//����ÿ����󳤶�
#define MAX_LENGTH 80//
//�������ͬ�����
#define LINE 150//
//���������ͬ�����//
#define SAME 5//
//���崰�ھ�����ײ�����
#define DISTANCE 41//

//��"mlmanage.h"��
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