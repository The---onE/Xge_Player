/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//设置窗口文件
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <windows.h>
#include <windowsx.h>
#include "resource.h"
#include <stdio.h> 
#include <stdlib.h>
#include "SetDlg.h"
#include "setmanage.h"

//设置窗口消息处理函数
BOOL WINAPI Set_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
	{
        HANDLE_MSG(hWnd, WM_INITDIALOG, Set_OnInitDialog);
        HANDLE_MSG(hWnd, WM_COMMAND, Set_OnCommand);
		HANDLE_MSG(hWnd, WM_CLOSE, Set_OnClose);

		//主窗口关闭后关闭其他子窗口
		case PRO_CLOSE:
		{
			EndDialog(hWnd, 0);
		}
		break;//

	}
	
    return FALSE;
}//

//初始化窗口函数
BOOL Set_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	//获取主窗口句柄
	setphwnd = (HWND)lParam;//
	getSourceSet();
	int st = 30,r = 128,g = 128,b = 128;
	getLable("_st",&st);
	getLable("_cr",&r);
	getLable("_cg",&g);
	getLable("_cb",&b);
	TCHAR lable[10];
	//若已存在设置文件，打开设置文件作为原设置
	_itoa_s(st,lable,10);
	SetDlgItemText(hwnd,IDC_SLEEPTIME,lable);
	_itoa_s(r,lable,10);
	SetDlgItemText(hwnd,IDC_R,lable);
	_itoa_s(g,lable,10);
	SetDlgItemText(hwnd,IDC_G,lable);
	_itoa_s(b,lable,10);
	SetDlgItemText(hwnd,IDC_B,lable);//
	//设置窗口总在最前
	SetWindowPos(hwnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);//
	//向主窗口发送句柄
	PostMessage(setphwnd,SET_OPEN,0,(LPARAM)hwnd);//
	return TRUE;
}//

//控件消息响应函数
void Set_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch(id)
	{
		//确定按钮响应
		case IDC_OK:
		{
			CreateSet(hwnd);
			//保存新设置
			setFile();//
			//向主窗口发送关闭消息
			PostMessage(setphwnd,SET_CLOSE,0,0);//
			//直接退出
			EndDialog(hwnd, 0);//
		}
		break;//

		//取消按钮响应
		case IDC_CANCEL:
		{
			//调用退出函数
			Set_OnClose(hwnd);//
		}
		break;//

		default:
			break;
	}
}//

//退出函数
void Set_OnClose(HWND hwnd)
{
	CreateSet(hwnd);
	if (strcmp(SourceSet,NewSet))
	{
		//弹出确认对话框，若确认则关闭设置窗口
		int yn = MessageBox(hwnd,TEXT("不保存修改吗？"),TEXT("退出"),MB_YESNO|MB_ICONQUESTION);
		if (IDYES == yn)
		{
			//向主窗口发送关闭消息
			PostMessage(setphwnd,SET_CLOSE,0,0);//
			EndDialog(hwnd, 0);
		}//
		else
			return;
	}
	else
	{
		//向主窗口发送关闭消息
		PostMessage(setphwnd,SET_CLOSE,0,0);//
		EndDialog(hwnd, 0);
	}
}//