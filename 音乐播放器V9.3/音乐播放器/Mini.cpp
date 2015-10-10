/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//搜索歌曲窗口文件
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

//定义主窗口句柄全局变量
HWND minphwnd;//
//定义是否鼠标离开窗口时应收缩参数
BOOL _shouldHide = FALSE;//
//定义是否已收缩参数
BOOL _isHide = FALSE;//
//定义窗口坐标全局变量
RECT _size;//
//定义是否检测鼠标离开窗口变量
BOOL _isTime = FALSE;//
//定义播放顺序组合框是否展开、是否拖动滚动条变量
BOOL _isOpen = FALSE,_isMusicMove = FALSE,_isSoundMove = FALSE;//

//搜索窗口消息处理函数
BOOL WINAPI Mini_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static TRACKMOUSEEVENT _leavemessage;
	switch(uMsg)
	{
        HANDLE_MSG(hWnd, WM_INITDIALOG, Mini_OnInitDialog);
        HANDLE_MSG(hWnd, WM_COMMAND, Mini_OnCommand);
		HANDLE_MSG(hWnd, WM_CLOSE, Mini_OnClose);
		//允许滚动条消息响应
		HANDLE_MSG(hWnd,WM_HSCROLL, Mini_OnHScroll);//

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
				_shouldHide = TRUE;//
				RECT position;
				//获得窗口边缘坐标
				GetWindowRect(hWnd,&position);//
				SetWindowPos(hWnd,HWND_TOPMOST,position.left,0,0,0,SWP_NOSIZE);
			}//
			//若纵坐标不小于窗口吸附边缘最小距离
			else
			{
				//设置鼠标离开窗口时不应收缩
				_shouldHide = FALSE;//
			}//
		}
		break;//

		//左击非客户区消息处理
		case WM_NCLBUTTONDOWN:
		{
			//若窗口已收缩则展开
			if (TRUE == _isHide)
			{
				SetWindowPos(hWnd,HWND_TOPMOST,0,0,_ORIGIN_WIDTH,_ORIGIN_HEIGHT,SWP_NOMOVE);
				_isHide = FALSE;
			}//
		}
		break;//

		//非客户区内鼠标移动消息处理
		case WM_NCMOUSEMOVE:
		{
			//若窗口已收缩则展开
			if (TRUE == _isHide)
			{
				SetWindowPos(hWnd,HWND_TOPMOST,0,0,_ORIGIN_WIDTH,_ORIGIN_HEIGHT,SWP_NOMOVE);
				_isHide = FALSE;
			}//	
		}
		break;//

		//客户区内鼠标移动消息处理
		case WM_MOUSEMOVE:
		{
			//允许接受WM_MOUSELEAVE鼠标离开客户区消息
			_leavemessage.cbSize = sizeof(TRACKMOUSEEVENT);
			_leavemessage.dwFlags = TME_LEAVE;
			_leavemessage.hwndTrack = hWnd;
			if(!_TrackMouseEvent(&_leavemessage))   
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
				if (TRUE == _shouldHide)
				{
					SetWindowPos(hWnd,HWND_TOPMOST,0,0,_ORIGIN_WIDTH,NEAR_HEIGHT,SWP_NOMOVE);
					_isHide = TRUE;
				}////
		}
		break;//

		//主窗口关闭后关闭其他子窗口
		case PRO_CLOSE:
		{
			EndDialog(hWnd, 0);
		}
		break;//

		//打开搜索窗口消息处理
		case MINI_OPENED:
		{
			//若迷你窗口已打开则使其前端显示，若已收缩则展开
			SetWindowPos(hWnd,HWND_TOPMOST,0,0,_ORIGIN_WIDTH,_ORIGIN_HEIGHT,SWP_NOMOVE);
			_isHide = FALSE;//
		}
		break;//
	}
	
    return FALSE;
}//

//初始化窗口函数
BOOL Mini_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
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
	minphwnd = (HWND)lParam;//
	//向主窗口发送句柄
	PostMessage(minphwnd,MINI_OPEN,0,(LPARAM)hwnd);//

	//获取窗口坐标
	GetWindowRect(hwnd,&_size);//
	//设置窗口总在最前
	SetWindowPos(hwnd,HWND_TOPMOST,0,0,_ORIGIN_WIDTH,_ORIGIN_HEIGHT,SWP_NOMOVE);//
	//窗口状态为未隐藏
	_isHide = FALSE;//
	//窗口为未吸附边缘状态
	_shouldHide = FALSE;//

	//初始化组合框显示
	//播放顺序组合框初始化
	HWND CBSX = GetDlgItem(hwnd,IDC__COMBOSX);
	ComboBox_InsertString(CBSX,-1,TEXT("单曲循环"));
	ComboBox_InsertString(CBSX,-1,TEXT("列表循环"));
	ComboBox_InsertString(CBSX,-1,TEXT("随机播放"));
	//获取主窗口播放顺序组合框选中选项设置到迷你窗口中
	int i = ComboBox_GetCurSel(GetDlgItem(minphwnd,IDC_COMBOSX));
	ComboBox_SetCurSel(CBSX,i);//

	//静音复选框初始化
	CheckDlgButton(hwnd,IDC__SOF,IsDlgButtonChecked(minphwnd,IDC_SOF));//

	//音量滚动条初始化
	HWND VHK = GetDlgItem(hwnd,IDC__SOUND_SLIDER);
	//设置滚动条最小值为0
	SendMessage(VHK,TBM_SETRANGEMIN,(WPARAM)TRUE,0);//
	//设置滚动条最大值为640 /6.4
	SendMessage(VHK,TBM_SETRANGEMAX,(WPARAM)TRUE,640);//
	//设置按←键时，减小32 /6.4
	SendMessage(VHK,TBM_SETPAGESIZE,0, (LPARAM)32);//
	//设置按→键时，增大32 /6.4
	SendMessage(VHK,TBM_SETLINESIZE,0, (LPARAM)32);//
	//获取主窗口音量滚动条位置设置到迷你窗口音量滚动条上
	int Voice=SendMessage(GetDlgItem(minphwnd,IDC_SOUND_SLIDER),TBM_GETPOS,0,0);
	SendMessage(GetDlgItem(hwnd,IDC__SOUND_SLIDER),TBM_SETPOS,(unsigned long)TRUE,Voice);//

	TCHAR TT[15],*p;
	//获取主窗口时间编辑框中内容
	GetDlgItemText(minphwnd,IDC_EDITTIME,TT,sizeof(TT));//
	p = TT;
	while (*p)
		p++;
	while (*p != '/')
		p--;
	p++;
	//获取总长分数
	int time = atoi(p);//
	p += 3;
	//获取总长秒数并与分数一起转化为毫秒
	time = time*60000 + (atoi(p)-1)*1000;//

	//播放进度滚动条初始化
	HWND MHK = GetDlgItem(hwnd,IDC__MUSIC_SLIDER);
	//设置滚动条最小值为0
	SendMessage(MHK,TBM_SETRANGEMIN,(WPARAM)TRUE,0);//
	//设置滚动条最大值为1000
	SendMessage(MHK,TBM_SETRANGEMAX,(WPARAM)TRUE,time);//
	//设置按←键时，减小25
	SendMessage(MHK,TBM_SETPAGESIZE,0, (LPARAM)(time/40));//
	//设置按→键时，增大25 
	SendMessage(MHK,TBM_SETLINESIZE,0, (LPARAM)(time/40));//
	//获取主窗口进度滚动条位置设置到迷你窗口进度滚动条上
	int Time=SendMessage(GetDlgItem(minphwnd,IDC_MUSIC_SLIDER),TBM_GETPOS,0,0);
	SendMessage(GetDlgItem(hwnd,IDC__MUSIC_SLIDER),TBM_SETPOS,(unsigned long)TRUE,Time);//

	TCHAR Name[MAX_PATH];
	//获取主窗口正在播放编辑框中文件路径
	GetDlgItemText(minphwnd,IDC_EDITNOW,Name,sizeof(Name));//
	//将主窗口正在播放编辑框中文件路径设置到迷你窗口中
	SetDlgItemText(hwnd,IDC__EDITNOW,Name);//

	//设置检测吸附窗口及刷新进度滚动条位置定时器
	SetTimer(hwnd,TIMER_MTEST,TIMER_MTEST_TIME,MINITimer);//

	return TRUE;
}//

//控件消息响应函数
void Mini_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch(id)
	{
		//播放/继续按钮响应
		case IDC__btnPLAY:
		{
			//向主窗口发送播放消息
			PostMessage(minphwnd,MINI_CONTROL,MINI_PLAY,0);//
		}
		break;//

		//暂停按钮响应
		case IDC__btnPAUSE:
		{
			//向主窗口发送暂停消息
			PostMessage(minphwnd,MINI_CONTROL,MINI_PAUSE,0);//
		}
		break;//

		//停止按钮响应
		case IDC__btnSTOP:
		{
			//向主窗口发送停止消息
			PostMessage(minphwnd,MINI_CONTROL,MINI_STOP,0);//
		}
		break;//

		//上曲按钮响应
		case IDC__btnLAST:
		{
			//向主窗口发送上一曲消息
			PostMessage(minphwnd,MINI_CONTROL,MINI_LAST,0);//
		}
		break;//

		//下曲按钮响应
		case IDC__btnNEXT:
		{
			//向主窗口发送下一曲消息
			PostMessage(minphwnd,MINI_CONTROL,MINI_NEXT,0);//
		}
		break;//

		//静音复选框响应
		case IDC__SOF:
		{
			//向主窗口发送静音消息
			PostMessage(minphwnd,MINI_CONTROL,MINI_SOF,0);//
		}
		break;//

		//更改播放顺序组合框选中选项响应
		case IDC__COMBOSX:
		{
			if (CBN_SELCHANGE==codeNotify)
			{
				//获取主窗口播放顺序组合框选中选项设置到迷你窗口中
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

//滚动条消息响应函数
void Mini_OnHScroll(HWND hwnd, HWND hwndCtl, UINT code, int pos)
{
	switch(GetDlgCtrlID(hwndCtl))
	{
		//音量滚动条消息响应
		case IDC__SOUND_SLIDER:
		{
			_isSoundMove = TRUE;
			//得到音量滚动条位置
			int Voice=SendMessage(GetDlgItem(hwnd,IDC__SOUND_SLIDER),TBM_GETPOS,0,0);//
			//将位置发送到主窗口
			PostMessage(minphwnd,MINI_CONTROL,MINI_SOUND_SLIDER,Voice);//
			//停止拖动时
			if (SB_ENDSCROLL == code)
				_isSoundMove = FALSE;//
		}
		break;//
			
		//播放进度滚动条消息响应
		case IDC__MUSIC_SLIDER:
		{
			_isMusicMove = TRUE;
			//停止拖动时
			if (SB_ENDSCROLL == code)
			{
				//得到播放进度滚动条位置
				int Time=SendMessage(GetDlgItem(hwnd,IDC__MUSIC_SLIDER),TBM_GETPOS,0,0);//
				//将位置发送到主窗口
				PostMessage(minphwnd,MINI_CONTROL,MINI_MUSIC_SLIDER,Time);//
				_isMusicMove = FALSE;
			}//
		}
		break;//
			
		default:
			break;
	}
}//

//检测吸附窗口及刷新歌词定时器回调函数
void CALLBACK MINITimer(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime)
{
	
	if (_isTime)
	{
		_isTime = FALSE;
		POINT pt;
		//获得鼠标坐标
		GetCursorPos(&pt);//
		RECT position;
		//获得窗口边缘坐标
		GetWindowRect(hwnd,&position);//
		//若鼠标不在窗口内
		if(pt.x>=position.right || pt.x<=position.left || pt.y>=position.bottom)//
			//若窗口已吸附边缘
			if (TRUE == _shouldHide)//
				//若窗口未收缩
				if(FALSE == _isHide)//
				{
					//收缩窗口
					SetWindowPos(hwnd,HWND_TOPMOST,0,0,_ORIGIN_WIDTH,NEAR_HEIGHT,SWP_NOMOVE);//
					_isHide = TRUE;//
				}
	}
	else
		_isTime = TRUE;

	if (!_isOpen)
	{
		//获取主窗口播放顺序组合框选中选项设置到迷你窗口中
		int i = ComboBox_GetCurSel(GetDlgItem(minphwnd,IDC_COMBOSX));
		ComboBox_SetCurSel(GetDlgItem(hwnd,IDC__COMBOSX),i);//
	}

	if (!_isSoundMove)
	{
		//获取主窗口音量滚动条位置设置到迷你窗口音量滚动条上
		int Voice=SendMessage(GetDlgItem(minphwnd,IDC_SOUND_SLIDER),TBM_GETPOS,0,0);
		SendMessage(GetDlgItem(hwnd,IDC__SOUND_SLIDER),TBM_SETPOS,(unsigned long)TRUE,Voice);//
	}

	if (!_isMusicMove)
	{
		//获取主窗口进度滚动条位置设置到迷你窗口进度滚动条上
		int Time=SendMessage(GetDlgItem(minphwnd,IDC_MUSIC_SLIDER),TBM_GETPOS,0,0);
		SendMessage(GetDlgItem(hwnd,IDC__MUSIC_SLIDER),TBM_SETPOS,(unsigned long)TRUE,Time);//
	}

	TCHAR Name[MAX_PATH];
	//获取主窗口正在播放编辑框中文件路径
	GetDlgItemText(minphwnd,IDC_EDITNOW,Name,sizeof(Name));//
	//将主窗口正在播放编辑框中文件路径设置到迷你窗口中
	SetDlgItemText(hwnd,IDC__EDITNOW,Name);//
}//

//退出函数
void Mini_OnClose(HWND hwnd)
{
	//向主窗口发送关闭消息
	PostMessage(minphwnd,MINI_CLOSE,0,0);//
	EndDialog(hwnd, 0);
}//