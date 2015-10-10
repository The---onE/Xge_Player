/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�������������ļ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <windowsx.h>
#include "tchar.h"
#include <shellapi.h>
#include <Winuser.h>
#include <commctrl.h>
#include <stdio.h>
#include <stdlib.h>

#include "resource.h"
#include "Mini.h"

//���������ھ��ȫ�ֱ���
HWND minphwnd;//
//�����Ƿ�����뿪����ʱӦ��������
BOOL _shouldHide = FALSE;//
//�����Ƿ�����������
BOOL _isHide = FALSE;//
//���崰������ȫ�ֱ���
RECT _size;//
//�����Ƿ�������뿪���ڱ���
BOOL _isTime = FALSE;//
//���岥��˳����Ͽ��Ƿ�չ�����Ƿ��϶�����������
BOOL _isOpen = FALSE,_isMusicMove = FALSE,_isSoundMove = FALSE;//

//����������Ϣ������
BOOL WINAPI Mini_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static TRACKMOUSEEVENT _leavemessage;
	switch(uMsg)
	{
        HANDLE_MSG(hWnd, WM_INITDIALOG, Mini_OnInitDialog);
        HANDLE_MSG(hWnd, WM_COMMAND, Mini_OnCommand);
		HANDLE_MSG(hWnd, WM_CLOSE, Mini_OnClose);
		//�����������Ϣ��Ӧ
		HANDLE_MSG(hWnd,WM_HSCROLL, Mini_OnHScroll);//

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
				_shouldHide = TRUE;//
				RECT position;
				//��ô��ڱ�Ե����
				GetWindowRect(hWnd,&position);//
				SetWindowPos(hWnd,HWND_TOPMOST,position.left,0,0,0,SWP_NOSIZE);
			}//
			//�������겻С�ڴ���������Ե��С����
			else
			{
				//��������뿪����ʱ��Ӧ����
				_shouldHide = FALSE;//
			}//
		}
		break;//

		//����ǿͻ�����Ϣ����
		case WM_NCLBUTTONDOWN:
		{
			//��������������չ��
			if (TRUE == _isHide)
			{
				SetWindowPos(hWnd,HWND_TOPMOST,0,0,_ORIGIN_WIDTH,_ORIGIN_HEIGHT,SWP_NOMOVE);
				_isHide = FALSE;
			}//
		}
		break;//

		//�ǿͻ���������ƶ���Ϣ����
		case WM_NCMOUSEMOVE:
		{
			//��������������չ��
			if (TRUE == _isHide)
			{
				SetWindowPos(hWnd,HWND_TOPMOST,0,0,_ORIGIN_WIDTH,_ORIGIN_HEIGHT,SWP_NOMOVE);
				_isHide = FALSE;
			}//	
		}
		break;//

		//�ͻ���������ƶ���Ϣ����
		case WM_MOUSEMOVE:
		{
			//�������WM_MOUSELEAVE����뿪�ͻ�����Ϣ
			_leavemessage.cbSize = sizeof(TRACKMOUSEEVENT);
			_leavemessage.dwFlags = TME_LEAVE;
			_leavemessage.hwndTrack = hWnd;
			if(!_TrackMouseEvent(&_leavemessage))   
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
				if (TRUE == _shouldHide)
				{
					SetWindowPos(hWnd,HWND_TOPMOST,0,0,_ORIGIN_WIDTH,NEAR_HEIGHT,SWP_NOMOVE);
					_isHide = TRUE;
				}////
		}
		break;//

		//�����ڹرպ�ر������Ӵ���
		case PRO_CLOSE:
		{
			EndDialog(hWnd, 0);
		}
		break;//

		//������������Ϣ����
		case MINI_OPENED:
		{
			//�����㴰���Ѵ���ʹ��ǰ����ʾ������������չ��
			SetWindowPos(hWnd,HWND_TOPMOST,0,0,_ORIGIN_WIDTH,_ORIGIN_HEIGHT,SWP_NOMOVE);
			_isHide = FALSE;//
		}
		break;//
	}
	
    return FALSE;
}//

//��ʼ�����ں���
BOOL Mini_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
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
	minphwnd = (HWND)lParam;//
	//�������ڷ��;��
	PostMessage(minphwnd,MINI_OPEN,0,(LPARAM)hwnd);//

	//��ȡ��������
	GetWindowRect(hwnd,&_size);//
	//���ô���������ǰ
	SetWindowPos(hwnd,HWND_TOPMOST,0,0,_ORIGIN_WIDTH,_ORIGIN_HEIGHT,SWP_NOMOVE);//
	//����״̬Ϊδ����
	_isHide = FALSE;//
	//����Ϊδ������Ե״̬
	_shouldHide = FALSE;//

	//��ʼ����Ͽ���ʾ
	//����˳����Ͽ��ʼ��
	HWND CBSX = GetDlgItem(hwnd,IDC__COMBOSX);
	ComboBox_InsertString(CBSX,-1,TEXT("����ѭ��"));
	ComboBox_InsertString(CBSX,-1,TEXT("�б�ѭ��"));
	ComboBox_InsertString(CBSX,-1,TEXT("�������"));
	//��ȡ�����ڲ���˳����Ͽ�ѡ��ѡ�����õ����㴰����
	int i = ComboBox_GetCurSel(GetDlgItem(minphwnd,IDC_COMBOSX));
	ComboBox_SetCurSel(CBSX,i);//

	//������ѡ���ʼ��
	CheckDlgButton(hwnd,IDC__SOF,IsDlgButtonChecked(minphwnd,IDC_SOF));//

	//������������ʼ��
	HWND VHK = GetDlgItem(hwnd,IDC__SOUND_SLIDER);
	//���ù�������СֵΪ0
	SendMessage(VHK,TBM_SETRANGEMIN,(WPARAM)TRUE,0);//
	//���ù��������ֵΪ640 /6.4
	SendMessage(VHK,TBM_SETRANGEMAX,(WPARAM)TRUE,640);//
	//���ð�����ʱ����С32 /6.4
	SendMessage(VHK,TBM_SETPAGESIZE,0, (LPARAM)32);//
	//���ð�����ʱ������32 /6.4
	SendMessage(VHK,TBM_SETLINESIZE,0, (LPARAM)32);//
	//��ȡ����������������λ�����õ����㴰��������������
	int Voice=SendMessage(GetDlgItem(minphwnd,IDC_SOUND_SLIDER),TBM_GETPOS,0,0);
	SendMessage(GetDlgItem(hwnd,IDC__SOUND_SLIDER),TBM_SETPOS,(unsigned long)TRUE,Voice);//

	TCHAR TT[15],*p;
	//��ȡ������ʱ��༭��������
	GetDlgItemText(minphwnd,IDC_EDITTIME,TT,sizeof(TT));//
	p = TT;
	while (*p)
		p++;
	while (*p != '/')
		p--;
	p++;
	//��ȡ�ܳ�����
	int time = atoi(p);//
	p += 3;
	//��ȡ�ܳ������������һ��ת��Ϊ����
	time = time*60000 + (atoi(p)-1)*1000;//

	//���Ž��ȹ�������ʼ��
	HWND MHK = GetDlgItem(hwnd,IDC__MUSIC_SLIDER);
	//���ù�������СֵΪ0
	SendMessage(MHK,TBM_SETRANGEMIN,(WPARAM)TRUE,0);//
	//���ù��������ֵΪ1000
	SendMessage(MHK,TBM_SETRANGEMAX,(WPARAM)TRUE,time);//
	//���ð�����ʱ����С25
	SendMessage(MHK,TBM_SETPAGESIZE,0, (LPARAM)(time/40));//
	//���ð�����ʱ������25 
	SendMessage(MHK,TBM_SETLINESIZE,0, (LPARAM)(time/40));//
	//��ȡ�����ڽ��ȹ�����λ�����õ����㴰�ڽ��ȹ�������
	int Time=SendMessage(GetDlgItem(minphwnd,IDC_MUSIC_SLIDER),TBM_GETPOS,0,0);
	SendMessage(GetDlgItem(hwnd,IDC__MUSIC_SLIDER),TBM_SETPOS,(unsigned long)TRUE,Time);//

	TCHAR Name[MAX_PATH];
	//��ȡ���������ڲ��ű༭�����ļ�·��
	GetDlgItemText(minphwnd,IDC_EDITNOW,Name,sizeof(Name));//
	//�����������ڲ��ű༭�����ļ�·�����õ����㴰����
	SetDlgItemText(hwnd,IDC__EDITNOW,Name);//

	//���ü���������ڼ�ˢ�½��ȹ�����λ�ö�ʱ��
	SetTimer(hwnd,TIMER_MTEST,TIMER_MTEST_TIME,MINITimer);//

	return TRUE;
}//

//�ؼ���Ϣ��Ӧ����
void Mini_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch(id)
	{
		//����/������ť��Ӧ
		case IDC__btnPLAY:
		{
			//�������ڷ��Ͳ�����Ϣ
			PostMessage(minphwnd,MINI_CONTROL,MINI_PLAY,0);//
		}
		break;//

		//��ͣ��ť��Ӧ
		case IDC__btnPAUSE:
		{
			//�������ڷ�����ͣ��Ϣ
			PostMessage(minphwnd,MINI_CONTROL,MINI_PAUSE,0);//
		}
		break;//

		//ֹͣ��ť��Ӧ
		case IDC__btnSTOP:
		{
			//�������ڷ���ֹͣ��Ϣ
			PostMessage(minphwnd,MINI_CONTROL,MINI_STOP,0);//
		}
		break;//

		//������ť��Ӧ
		case IDC__btnLAST:
		{
			//�������ڷ�����һ����Ϣ
			PostMessage(minphwnd,MINI_CONTROL,MINI_LAST,0);//
		}
		break;//

		//������ť��Ӧ
		case IDC__btnNEXT:
		{
			//�������ڷ�����һ����Ϣ
			PostMessage(minphwnd,MINI_CONTROL,MINI_NEXT,0);//
		}
		break;//

		//������ѡ����Ӧ
		case IDC__SOF:
		{
			//�������ڷ��;�����Ϣ
			PostMessage(minphwnd,MINI_CONTROL,MINI_SOF,0);//
		}
		break;//

		//���Ĳ���˳����Ͽ�ѡ��ѡ����Ӧ
		case IDC__COMBOSX:
		{
			if (CBN_SELCHANGE==codeNotify)
			{
				//��ȡ�����ڲ���˳����Ͽ�ѡ��ѡ�����õ����㴰����
				int i = ComboBox_GetCurSel(GetDlgItem(hwnd,IDC__COMBOSX));
				ComboBox_SetCurSel(GetDlgItem(minphwnd,IDC_COMBOSX),i);// 
			}

			if (CBN_DROPDOWN==codeNotify)
				_isOpen = TRUE; 

			if (CBN_CLOSEUP==codeNotify)
				_isOpen = FALSE; 
		}
		break;//

		default:
		break;
	}
}//

//��������Ϣ��Ӧ����
void Mini_OnHScroll(HWND hwnd, HWND hwndCtl, UINT code, int pos)
{
	switch(GetDlgCtrlID(hwndCtl))
	{
		//������������Ϣ��Ӧ
		case IDC__SOUND_SLIDER:
		{
			_isSoundMove = TRUE;
			//�õ�����������λ��
			int Voice=SendMessage(GetDlgItem(hwnd,IDC__SOUND_SLIDER),TBM_GETPOS,0,0);//
			//��λ�÷��͵�������
			PostMessage(minphwnd,MINI_CONTROL,MINI_SOUND_SLIDER,Voice);//
			//ֹͣ�϶�ʱ
			if (SB_ENDSCROLL == code)
				_isSoundMove = FALSE;//
		}
		break;//
			
		//���Ž��ȹ�������Ϣ��Ӧ
		case IDC__MUSIC_SLIDER:
		{
			_isMusicMove = TRUE;
			//ֹͣ�϶�ʱ
			if (SB_ENDSCROLL == code)
			{
				//�õ����Ž��ȹ�����λ��
				int Time=SendMessage(GetDlgItem(hwnd,IDC__MUSIC_SLIDER),TBM_GETPOS,0,0);//
				//��λ�÷��͵�������
				PostMessage(minphwnd,MINI_CONTROL,MINI_MUSIC_SLIDER,Time);//
				_isMusicMove = FALSE;
			}//
		}
		break;//
			
		default:
			break;
	}
}//

//����������ڼ�ˢ�¸�ʶ�ʱ���ص�����
void CALLBACK MINITimer(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime)
{
	
	if (_isTime)
	{
		_isTime = FALSE;
		POINT pt;
		//����������
		GetCursorPos(&pt);//
		RECT position;
		//��ô��ڱ�Ե����
		GetWindowRect(hwnd,&position);//
		//����겻�ڴ�����
		if(pt.x>=position.right || pt.x<=position.left || pt.y>=position.bottom)//
			//��������������Ե
			if (TRUE == _shouldHide)//
				//������δ����
				if(FALSE == _isHide)//
				{
					//��������
					SetWindowPos(hwnd,HWND_TOPMOST,0,0,_ORIGIN_WIDTH,NEAR_HEIGHT,SWP_NOMOVE);//
					_isHide = TRUE;//
				}
	}
	else
		_isTime = TRUE;

	if (!_isOpen)
	{
		//��ȡ�����ڲ���˳����Ͽ�ѡ��ѡ�����õ����㴰����
		int i = ComboBox_GetCurSel(GetDlgItem(minphwnd,IDC_COMBOSX));
		ComboBox_SetCurSel(GetDlgItem(hwnd,IDC__COMBOSX),i);//
	}

	if (!_isSoundMove)
	{
		//��ȡ����������������λ�����õ����㴰��������������
		int Voice=SendMessage(GetDlgItem(minphwnd,IDC_SOUND_SLIDER),TBM_GETPOS,0,0);
		SendMessage(GetDlgItem(hwnd,IDC__SOUND_SLIDER),TBM_SETPOS,(unsigned long)TRUE,Voice);//
	}

	if (!_isMusicMove)
	{
		//��ȡ�����ڽ��ȹ�����λ�����õ����㴰�ڽ��ȹ�������
		int Time=SendMessage(GetDlgItem(minphwnd,IDC_MUSIC_SLIDER),TBM_GETPOS,0,0);
		SendMessage(GetDlgItem(hwnd,IDC__MUSIC_SLIDER),TBM_SETPOS,(unsigned long)TRUE,Time);//
	}

	TCHAR Name[MAX_PATH];
	//��ȡ���������ڲ��ű༭�����ļ�·��
	GetDlgItemText(minphwnd,IDC_EDITNOW,Name,sizeof(Name));//
	//�����������ڲ��ű༭�����ļ�·�����õ����㴰����
	SetDlgItemText(hwnd,IDC__EDITNOW,Name);//
}//

//�˳�����
void Mini_OnClose(HWND hwnd)
{
	//�������ڷ��͹ر���Ϣ
	PostMessage(minphwnd,MINI_CLOSE,0,0);//
	EndDialog(hwnd, 0);
}//