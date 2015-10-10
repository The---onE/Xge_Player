/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��ʴ����ļ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <windowsx.h>
#include "tchar.h"
#include "resource.h"
#include <stdio.h> 
#include <stdlib.h>
#include "SkinH.h"
#include <commctrl.h>
#include "Lrc.h"
#include "lrcmanage.h"

//��ʴ�����Ϣ������
BOOL WINAPI Lrc_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static TRACKMOUSEEVENT leavemessage;
	switch(uMsg)
	{
        HANDLE_MSG(hWnd, WM_INITDIALOG, Lrc_OnInitDialog);
        HANDLE_MSG(hWnd, WM_COMMAND, Lrc_OnCommand);
		HANDLE_MSG(hWnd, WM_CLOSE, Lrc_OnClose);

		//�����ƶ���Ϣ����
		case WM_MOVE:
		{
			int x,y;
			//32λ��������16λΪ���������꣬��16λΪ���ں�����
			x = lParam % 65536;
			y = lParam / 65536;
			//��������С�ڴ���������Ե��С����
			if (y < NEAR_DISTANCE)
			{
				//��������뿪����ʱӦ����
				shouldHide = TRUE;//
				RECT position;
				//��ô��ڱ�Ե����
				GetWindowRect(hWnd,&position);//
				SetWindowPos(hWnd,HWND_TOPMOST,position.left,0,0,0,SWP_NOSIZE);
			}//
			//�������겻С�ڴ���������Ե��С����
			else
			{
				//��������뿪����ʱ��Ӧ����
				shouldHide = FALSE;//
			}//
		}
		break;//

		//����ǿͻ�����Ϣ����
		case WM_NCLBUTTONDOWN:
		{
			//��������������չ��
			if (TRUE == isHide)
			{
				SetWindowPos(hWnd,HWND_TOPMOST,0,0,ORIGIN_WIDTH,ORIGIN_HEIGHT,SWP_NOMOVE);
				isHide = FALSE;
			}//
		}
		break;//

		//�ǿͻ���������ƶ���Ϣ����
		case WM_NCMOUSEMOVE:
		{
			//��������������չ��
			if (TRUE == isHide)
			{
				SetWindowPos(hWnd,HWND_TOPMOST,0,0,ORIGIN_WIDTH,ORIGIN_HEIGHT,SWP_NOMOVE);
				isHide = FALSE;
			}//	
		}
		break;//

		//�ͻ���������ƶ���Ϣ����
		case WM_MOUSEMOVE:
		{
			//�������WM_MOUSELEAVE����뿪�ͻ�����Ϣ
			leavemessage.cbSize = sizeof(TRACKMOUSEEVENT);
			leavemessage.dwFlags = TME_LEAVE;
			leavemessage.hwndTrack = hWnd;
			if(!_TrackMouseEvent(&leavemessage))   
				return FALSE;//		
		}
		break;//

		//����뿪�ͻ�����Ϣ����
		case WM_MOUSELEAVE:
		{
			POINT pt;
			//����������
			GetCursorPos(&pt);//
			RECT position;
			//��ô��ڱ�Ե����
			GetWindowRect(hWnd,&position);//
			//��������뿪����
			if(pt.x>=position.right-10 || pt.x<=position.left+10 || pt.y>=position.bottom-10)
				//������Ӧ���������ش���
				if (TRUE == shouldHide)
				{
					SetWindowPos(hWnd,HWND_TOPMOST,0,0,ORIGIN_WIDTH,NEAR_HEIGHT,SWP_NOMOVE);
					isHide = TRUE;
				}////
		}
		break;//

		//�����ڶ�ʱ������ʱ����Ϣ����
		case LRC_TIME:
		{
			//��Ϊ����״̬
			if (isFollow)
			{
				int musictime = wParam*60 + lParam;
				//��������������ʱ��Ƚ�����ͬ��ѡ����Ӧ���
				select(hWnd,musictime);//
			}
		}
		break;//

		//���ܴ򿪸�ʴ�����Ϣ
		case LRC_OPENED:
		{
			//����ʴ����Ѵ���ʹ��ǰ����ʾ������������չ��
			SetWindowPos(hWnd,HWND_TOPMOST,0,0,ORIGIN_WIDTH,ORIGIN_HEIGHT,SWP_NOMOVE);
			isHide = FALSE;//
		}
		break;//

		//�����ڹرպ�ر������Ӵ���
		case PRO_CLOSE:
		{
			EndDialog(hWnd, 0);
		}
		break;//

	}
	
    return FALSE;
}//

//��ʼ�����ں���
BOOL Lrc_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	//����͸��
	SetWindowLong(hwnd, GWL_EXSTYLE,
	GetWindowLong(hwnd, GWL_EXSTYLE)^0x80000);
	HINSTANCE hInst = LoadLibrary(_T("User32.DLL"));
	if(hInst)
	{
		typedef BOOL (WINAPI *MYFUNC) (HWND, COLORREF, BYTE, DWORD);
		MYFUNC fun = NULL;
		fun = (MYFUNC) GetProcAddress(hInst, _T("SetLayeredWindowAttributes"));
		if(fun) fun(hwnd, 0, 255*90/100, 2);
		FreeLibrary(hInst);
	}//
	//��ȡ�����ھ��
	lrcphwnd = (HWND)lParam;//
	//�������ڷ��ʹ��ھ��
	PostMessage(lrcphwnd,LRC_OPEN,0,(LPARAM)hwnd);//
	//��ȡ��������
	GetWindowRect(hwnd,&size);//
	//���ô���������ǰ
	SetWindowPos(hwnd,HWND_TOPMOST,0,0,ORIGIN_WIDTH,ORIGIN_HEIGHT,SWP_NOMOVE);//
	//����״̬Ϊδ����
	isHide = FALSE;//
	//����Ϊδ������Ե״̬
	shouldHide = FALSE;//
	//��ʼ״̬Ϊ�������״̬
	isFollow = TRUE;//
	//��ʼ������б�
	ListBox_InsertString(GetDlgItem(hwnd,IDC_LRCList),-1,"��ʲ�����");//
	//���ü���������ڼ�ˢ�¸�ʶ�ʱ��
	SetTimer(hwnd,TIMER_TEST,TIMER_TEST_TIME,LRCTimer);//
	return TRUE;
}//

//�ؼ���Ϣ��Ӧ����
void Lrc_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch(id)
	{
		//�б���˫����Ӧ
		case IDC_LRCList:
		{
			if(LBN_DBLCLK==codeNotify)
			{
				int i = ListBox_GetCurSel(GetDlgItem(hwnd,IDC_LRCList));
				//��ѡ����Ϊ�����
				if (i >= 3)
				{
					//�������ڷ�����Ϣ�Ǹ����Ӹ��ʱ�俪ʼ����
					PostMessage(lrcphwnd,LRC_CHANGE,0,orderlytime[i-3]);//
				}
			}
		}
		break;//
		
		//ˢ�°�ť��Ӧ
		case IDC_REFRESH:
		{
			//ˢ�¸��
			GetSetLrc(hwnd,lrcphwnd,TRUE);//
		}
		break;//

		//���水ť��Ӧ
		case IDC_FOLLOW:
		{
			//��Ϊ���ڸ���״̬
			if (TRUE == isFollow)
			{
				//��Ϊֹͣ����״̬
				isFollow = FALSE;//
				Button_SetText(GetDlgItem(hwnd,IDC_FOLLOW),TEXT("��������"));
			}//
			//��Ϊֹͣ����״̬
			else
			{
				//��Ϊ���ڸ���״̬
				isFollow = TRUE;//
				Button_SetText(GetDlgItem(hwnd,IDC_FOLLOW),TEXT("ֹͣ����"));
			}//
		}
		break;//
		
		default:
			break;
	}
}//

//�˳�����
void Lrc_OnClose(HWND hwnd)
{
	KillTimer(hwnd, TIMER_TEST);
	//�������ڷ��͹ر���Ϣ
	PostMessage(lrcphwnd,LRC_CLOSE,0,0);//
	EndDialog(hwnd, 0);
}//