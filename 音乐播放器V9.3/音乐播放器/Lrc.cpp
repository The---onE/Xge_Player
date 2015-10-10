/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//歌词窗口文件
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

//歌词窗口消息处理函数
BOOL WINAPI Lrc_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static TRACKMOUSEEVENT leavemessage;
	switch(uMsg)
	{
        HANDLE_MSG(hWnd, WM_INITDIALOG, Lrc_OnInitDialog);
        HANDLE_MSG(hWnd, WM_COMMAND, Lrc_OnCommand);
		HANDLE_MSG(hWnd, WM_CLOSE, Lrc_OnClose);

		//窗口移动消息处理
		case WM_MOVE:
		{
			int x,y;
			//32位整数，高16位为窗口纵坐标，低16位为窗口横坐标
			x = lParam % 65536;
			y = lParam / 65536;
			//若纵坐标小于窗口吸附边缘最小距离
			if (y < NEAR_DISTANCE)
			{
				//设置鼠标离开窗口时应收缩
				shouldHide = TRUE;//
				RECT position;
				//获得窗口边缘坐标
				GetWindowRect(hWnd,&position);//
				SetWindowPos(hWnd,HWND_TOPMOST,position.left,0,0,0,SWP_NOSIZE);
			}//
			//若纵坐标不小于窗口吸附边缘最小距离
			else
			{
				//设置鼠标离开窗口时不应收缩
				shouldHide = FALSE;//
			}//
		}
		break;//

		//左击非客户区消息处理
		case WM_NCLBUTTONDOWN:
		{
			//若窗口已收缩则展开
			if (TRUE == isHide)
			{
				SetWindowPos(hWnd,HWND_TOPMOST,0,0,ORIGIN_WIDTH,ORIGIN_HEIGHT,SWP_NOMOVE);
				isHide = FALSE;
			}//
		}
		break;//

		//非客户区内鼠标移动消息处理
		case WM_NCMOUSEMOVE:
		{
			//若窗口已收缩则展开
			if (TRUE == isHide)
			{
				SetWindowPos(hWnd,HWND_TOPMOST,0,0,ORIGIN_WIDTH,ORIGIN_HEIGHT,SWP_NOMOVE);
				isHide = FALSE;
			}//	
		}
		break;//

		//客户区内鼠标移动消息处理
		case WM_MOUSEMOVE:
		{
			//允许接受WM_MOUSELEAVE鼠标离开客户区消息
			leavemessage.cbSize = sizeof(TRACKMOUSEEVENT);
			leavemessage.dwFlags = TME_LEAVE;
			leavemessage.hwndTrack = hWnd;
			if(!_TrackMouseEvent(&leavemessage))   
				return FALSE;//		
		}
		break;//

		//鼠标离开客户区消息处理
		case WM_MOUSELEAVE:
		{
			POINT pt;
			//获得鼠标坐标
			GetCursorPos(&pt);//
			RECT position;
			//获得窗口边缘坐标
			GetWindowRect(hWnd,&position);//
			//若鼠标已离开窗口
			if(pt.x>=position.right-10 || pt.x<=position.left+10 || pt.y>=position.bottom-10)
				//若窗口应隐藏则隐藏窗口
				if (TRUE == shouldHide)
				{
					SetWindowPos(hWnd,HWND_TOPMOST,0,0,ORIGIN_WIDTH,NEAR_HEIGHT,SWP_NOMOVE);
					isHide = TRUE;
				}////
		}
		break;//

		//主窗口定时器发送时间消息处理
		case LRC_TIME:
		{
			//若为跟随状态
			if (isFollow)
			{
				int musictime = wParam*60 + lParam;
				//将歌曲进度与歌词时间比较若相同则选中相应歌词
				select(hWnd,musictime);//
			}
		}
		break;//

		//接受打开歌词窗口消息
		case LRC_OPENED:
		{
			//若歌词窗口已打开则使其前端显示，若已收缩则展开
			SetWindowPos(hWnd,HWND_TOPMOST,0,0,ORIGIN_WIDTH,ORIGIN_HEIGHT,SWP_NOMOVE);
			isHide = FALSE;//
		}
		break;//

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
BOOL Lrc_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	//窗口透明
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
	//获取主窗口句柄
	lrcphwnd = (HWND)lParam;//
	//向主窗口发送窗口句柄
	PostMessage(lrcphwnd,LRC_OPEN,0,(LPARAM)hwnd);//
	//获取窗口坐标
	GetWindowRect(hwnd,&size);//
	//设置窗口总在最前
	SetWindowPos(hwnd,HWND_TOPMOST,0,0,ORIGIN_WIDTH,ORIGIN_HEIGHT,SWP_NOMOVE);//
	//窗口状态为未隐藏
	isHide = FALSE;//
	//窗口为未吸附边缘状态
	shouldHide = FALSE;//
	//初始状态为跟随歌曲状态
	isFollow = TRUE;//
	//初始化歌词列表
	ListBox_InsertString(GetDlgItem(hwnd,IDC_LRCList),-1,"歌词不存在");//
	//设置检测吸附窗口及刷新歌词定时器
	SetTimer(hwnd,TIMER_TEST,TIMER_TEST_TIME,LRCTimer);//
	return TRUE;
}//

//控件消息响应函数
void Lrc_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch(id)
	{
		//列表内双击响应
		case IDC_LRCList:
		{
			if(LBN_DBLCLK==codeNotify)
			{
				int i = ListBox_GetCurSel(GetDlgItem(hwnd,IDC_LRCList));
				//若选择项为歌词项
				if (i >= 3)
				{
					//向主窗口发送消息是歌曲从歌词时间开始播放
					PostMessage(lrcphwnd,LRC_CHANGE,0,orderlytime[i-3]);//
				}
			}
		}
		break;//
		
		//刷新按钮响应
		case IDC_REFRESH:
		{
			//刷新歌词
			GetSetLrc(hwnd,lrcphwnd,TRUE);//
		}
		break;//

		//跟随按钮响应
		case IDC_FOLLOW:
		{
			//若为正在跟随状态
			if (TRUE == isFollow)
			{
				//改为停止跟随状态
				isFollow = FALSE;//
				Button_SetText(GetDlgItem(hwnd,IDC_FOLLOW),TEXT("继续跟随"));
			}//
			//若为停止跟随状态
			else
			{
				//改为正在跟随状态
				isFollow = TRUE;//
				Button_SetText(GetDlgItem(hwnd,IDC_FOLLOW),TEXT("停止跟随"));
			}//
		}
		break;//
		
		default:
			break;
	}
}//

//退出函数
void Lrc_OnClose(HWND hwnd)
{
	KillTimer(hwnd, TIMER_TEST);
	//向主窗口发送关闭消息
	PostMessage(lrcphwnd,LRC_CLOSE,0,0);//
	EndDialog(hwnd, 0);
}//