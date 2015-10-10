/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//迷你歌词窗口文件
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <windows.h>
#include <windowsx.h>
#include <shellapi.h>
#include <Winuser.h>
#include <commctrl.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include "SkinH.h"

#include "resource.h"
#include "MiniLrc.h"
#include "mlmanage.h"

#pragma comment(lib,"SkinH.lib")

//迷你歌词消息处理函数
BOOL WINAPI MiniLrc_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static TRACKMOUSEEVENT _leavemessage;
	switch(uMsg)
	{
        HANDLE_MSG(hWnd, WM_INITDIALOG, MiniLrc_OnInitDialog);
        HANDLE_MSG(hWnd, WM_COMMAND, MiniLrc_OnCommand);
		HANDLE_MSG(hWnd, WM_CLOSE, MiniLrc_OnClose);

		//主窗口关闭后关闭其他子窗口
		case PRO_CLOSE:
		{
			EndDialog(hWnd, 0);
		}
		break;//

		//主窗口定时器发送时间消息处理
		case MINILRC_TIME:
		{
			int musictime = (int)wParam*60 + (int)lParam;
			//将歌曲进度与歌词时间比较若相同则选中相应歌词
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
		
		//改变字体颜色
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

//初始化窗口函数
BOOL MiniLrc_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	SkinH_DetachEx(hwnd);
	SetWindowLong(hwnd, GWL_EXSTYLE,
	GetWindowLong(hwnd, GWL_EXSTYLE)^0x80000);
	HINSTANCE hInst = LoadLibrary(_T("User32.DLL"));
	if(hInst)
	{
		typedef BOOL (WINAPI *MYFUNC) (HWND, COLORREF, BYTE, DWORD);
		MYFUNC fun = NULL;
		COLORREF color = RGB(240,240,240);
		fun = (MYFUNC) GetProcAddress(hInst, _T("SetLayeredWindowAttributes"));
		if(fun) fun(hwnd,color,255,1);
		FreeLibrary(hInst);
	}
	
	//获取主窗口句柄
	milphwnd = (HWND)lParam;//
	//向主窗口发送句柄
	PostMessage(milphwnd,MINILRC_OPEN,0,(LPARAM)hwnd);//

	RECT position;
	//获得窗口边缘坐标
	GetWindowRect(hwnd,&position);//
	//获取显示器分辨率
	int height= GetSystemMetrics (SM_CYSCREEN);//
	//设置窗口总在最前
	SetWindowPos(hwnd,HWND_TOPMOST,position.left,height-(position.bottom-position.top)-DISTANCE,0,0,SWP_NOSIZE);//

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED);

	Initialization();
	strcpy(name,"");
	//设置刷新歌词定时器
	SetTimer(hwnd,TIMER_MLTEST,TIMER_MLTEST_TIME,(TIMERPROC)MLTimer);//

	return TRUE;
}//

//控件消息响应函数
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

//退出函数
void MiniLrc_OnClose(HWND hwnd)
{
	KillTimer(hwnd, TIMER_MLTEST);
	Initialization();
	strcpy(name,"");
	//向主窗口发送关闭消息
	PostMessage(milphwnd,MINILRC_CLOSE,0,0);//
	EndDialog(hwnd, 0);
}//