/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�궨�弰�������㴰�ں���
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _Mini_H
#define _Mini_H

#include <windows.h>

//����������ر���Ϣ
#define PRO_CLOSE 10005//
//�������㴰�ڴ���Ϣ
#define MINI_OPEN 10020//
//���մ����㴰����Ϣ
#define MINI_OPENED 10021//
//�������㴰�ڴ���Ϣ
#define MINI_CLOSE 10022//
//���Ϳ�����������Ϣ
#define MINI_CONTROL 10025//
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

//����������Ե��С����
#define NEAR_DISTANCE 45//
//��������״̬�߶�
#define NEAR_HEIGHT 3//
//����չ��״̬�߶�
#define _ORIGIN_HEIGHT _size.bottom-_size.top//
//����չ��״̬���
#define _ORIGIN_WIDTH _size.right-_size.left//

//�������������ڼ�ˢ�½��ȹ�����λ�ö�ʱ����ʱ��
#define TIMER_MTEST 6
#define TIMER_MTEST_TIME 497//

void CALLBACK MINITimer(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime);

BOOL WINAPI Mini_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL Mini_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
void Mini_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void Mini_OnHScroll(HWND hwnd, HWND hwndCtl, UINT code, int pos);
void Mini_OnClose(HWND hwnd);

#endif