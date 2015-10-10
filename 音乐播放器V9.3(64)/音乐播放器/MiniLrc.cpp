/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�����ʴ����ļ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <windows.h>
#include <windowsx.h>
#include <shellapi.h>
#include <Winuser.h>
#include <commctrl.h>
#include <stdio.h>
#include <stdlib.h>

#include "resource.h"
#include "MiniLrc.h"
#include "mlmanage.h"


//��������Ϣ������
BOOL WINAPI MiniLrc_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static TRACKMOUSEEVENT _leavemessage;
	switch(uMsg)
	{
        HANDLE_MSG(hWnd, WM_INITDIALOG, MiniLrc_OnInitDialog);
        HANDLE_MSG(hWnd, WM_COMMAND, MiniLrc_OnCommand);
		HANDLE_MSG(hWnd, WM_CLOSE, MiniLrc_OnClose);

		//�����ڹرպ�ر������Ӵ���
		case PRO_CLOSE:
		{
			EndDialog(hWnd, 0);
		}
		break;//

		//�����ڶ�ʱ������ʱ����Ϣ����
		case MINILRC_TIME:
		{
			int musictime = (int)wParam*60 + (int)lParam;
			//��������������ʱ��Ƚ�����ͬ��ѡ����Ӧ���
			_select(hWnd,musictime);//
		}
		break;//

		case WM_LBUTTONDOWN:
		{
			POINT point;
			GetCursorPos(&point);
			PostMessage(hWnd,WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
		}
		break;
		
		//�ı�������ɫ
		case WM_CTLCOLORSTATIC:
		{
			HBRUSH hbrushEditBox = CreateSolidBrush(RGB(240,240,240));
			HDC hdcEdit = (HDC)wParam;
			int r,g,b;
			r = 127;g = 127;b = 127;
			GetsetColor(&r,&g,&b);
			SetTextColor(hdcEdit, RGB(r,g,b));
			SetBkMode(hdcEdit,TRANSPARENT);
			SetBkColor(hdcEdit,(LONG)hbrushEditBox);
			return (LONG)hbrushEditBox;
		}
		break;//
	}
	
    return FALSE;
}//

//��ʼ�����ں���
BOOL MiniLrc_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	//����͸�����
	COLORREF color = RGB(240,240,240);
	SetWindowLong(hwnd,GWL_EXSTYLE,GetWindowLong(hwnd, GWL_EXSTYLE)|WS_EX_LAYERED);
	SetLayeredWindowAttributes(hwnd,color,255,LWA_COLORKEY);//
	
	//��ȡ�����ھ��
	milphwnd = (HWND)lParam;//
	//�������ڷ��;��
	PostMessage(milphwnd,MINILRC_OPEN,0,(LPARAM)hwnd);//

	RECT position;
	//��ô��ڱ�Ե����
	GetWindowRect(hwnd,&position);//
	//��ȡ��ʾ���ֱ���
	int height= GetSystemMetrics (SM_CYSCREEN);//
	//���ô���������ǰ
	SetWindowPos(hwnd,HWND_TOPMOST,position.left,height-(position.bottom-position.top)-DISTANCE,0,0,SWP_NOSIZE);//

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED);

	Initialization();
	strcpy_s(name,"");
	//����ˢ�¸�ʶ�ʱ��
	SetTimer(hwnd,TIMER_MLTEST,TIMER_MLTEST_TIME,(TIMERPROC)MLTimer);//

	return TRUE;
}//

//�ؼ���Ϣ��Ӧ����
void MiniLrc_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch(id)
	{
		case 0xffff:
			break;

		default:
		break;
	}
}//

//�˳�����
void MiniLrc_OnClose(HWND hwnd)
{
	KillTimer(hwnd, TIMER_MLTEST);
	Initialization();
	strcpy_s(name,"");
	//�������ڷ��͹ر���Ϣ
	PostMessage(milphwnd,MINILRC_CLOSE,0,0);//
	EndDialog(hwnd, 0);
}//