/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//���ô����ļ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <windows.h>
#include <windowsx.h>
#include "resource.h"
#include <stdio.h> 
#include <stdlib.h>
#include "SetDlg.h"
#include "setmanage.h"

//���ô�����Ϣ������
BOOL WINAPI Set_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
	{
        HANDLE_MSG(hWnd, WM_INITDIALOG, Set_OnInitDialog);
        HANDLE_MSG(hWnd, WM_COMMAND, Set_OnCommand);
		HANDLE_MSG(hWnd, WM_CLOSE, Set_OnClose);

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
BOOL Set_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	//��ȡ�����ھ��
	setphwnd = (HWND)lParam;//
	getSourceSet();
	int st = 30,r = 128,g = 128,b = 128;
	getLable("_st",&st);
	getLable("_cr",&r);
	getLable("_cg",&g);
	getLable("_cb",&b);
	TCHAR lable[10];
	//���Ѵ��������ļ����������ļ���Ϊԭ����
	_itoa_s(st,lable,10);
	SetDlgItemText(hwnd,IDC_SLEEPTIME,lable);
	_itoa_s(r,lable,10);
	SetDlgItemText(hwnd,IDC_R,lable);
	_itoa_s(g,lable,10);
	SetDlgItemText(hwnd,IDC_G,lable);
	_itoa_s(b,lable,10);
	SetDlgItemText(hwnd,IDC_B,lable);//
	//���ô���������ǰ
	SetWindowPos(hwnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);//
	//�������ڷ��;��
	PostMessage(setphwnd,SET_OPEN,0,(LPARAM)hwnd);//
	return TRUE;
}//

//�ؼ���Ϣ��Ӧ����
void Set_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch(id)
	{
		//ȷ����ť��Ӧ
		case IDC_OK:
		{
			CreateSet(hwnd);
			//����������
			setFile();//
			//�������ڷ��͹ر���Ϣ
			PostMessage(setphwnd,SET_CLOSE,0,0);//
			//ֱ���˳�
			EndDialog(hwnd, 0);//
		}
		break;//

		//ȡ����ť��Ӧ
		case IDC_CANCEL:
		{
			//�����˳�����
			Set_OnClose(hwnd);//
		}
		break;//

		default:
			break;
	}
}//

//�˳�����
void Set_OnClose(HWND hwnd)
{
	CreateSet(hwnd);
	if (strcmp(SourceSet,NewSet))
	{
		//����ȷ�϶Ի�����ȷ����ر����ô���
		int yn = MessageBox(hwnd,TEXT("�������޸���"),TEXT("�˳�"),MB_YESNO|MB_ICONQUESTION);
		if (IDYES == yn)
		{
			//�������ڷ��͹ر���Ϣ
			PostMessage(setphwnd,SET_CLOSE,0,0);//
			EndDialog(hwnd, 0);
		}//
		else
			return;
	}
	else
	{
		//�������ڷ��͹ر���Ϣ
		PostMessage(setphwnd,SET_CLOSE,0,0);//
		EndDialog(hwnd, 0);
	}
}//