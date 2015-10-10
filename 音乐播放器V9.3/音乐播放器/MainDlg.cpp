/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//程序主窗口文件
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include <windowsx.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mmsystem.h>
#include <commdlg.h>
#include <commctrl.h>
#include "SkinH.h"
#include <tchar.h>
//声明主窗口函数
#include "MainDlg.h"
//允许弹出搜索歌曲窗口
#include "Search.h"//
//允许弹出设置窗口
#include "SetDlg.h"//
//允许弹出歌词窗口
#include "Lrc.h"//
//允许迷你窗口
#include "Mini.h"//
//允许迷你歌词
#include "MiniLrc.h"//
//内含全部主窗口函数定义
#include "control.h"//

#pragma comment(lib,"SkinH.lib")
#pragma comment(lib,"Winmm.lib")


//调试用对话框：//TCHAR test[MAX_PATH];
				//itoa(inttest,test,10);
				//MessageBox(hwnd,test,"1",MB_OK);
				//MessageBox(hwnd,"OK","1",MB_OK);

//程序入口函数
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	//注册并初始化窗口类
	InitCommonControls();//
	//加载skinh.she皮肤
	SkinH_Attach();//
	//为皮肤设置特效
	SkinH_SetAero(1);
	SkinH_AdjustAero(127,0,0,4,0,0,-1,-1,-1);
	SkinH_SetMenuAlpha(191);//
	//若由文件拖入打开，获取文件路径
	pfo = lpCmdLine;//
	//弹出主窗口
	DialogBox(hInstance,MAKEINTRESOURCE(IDD_MAIN),NULL,(DLGPROC)Main_Proc); //
	return 0;
}//

//主窗口消息处理函数
BOOL WINAPI Main_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		HANDLE_MSG(hWnd,WM_INITDIALOG, Main_OnInitDialog);
		HANDLE_MSG(hWnd,WM_COMMAND, Main_OnCommand);
		HANDLE_MSG(hWnd,WM_CLOSE, Main_OnClose);
		//允许滚动条消息响应
		HANDLE_MSG(hWnd,WM_HSCROLL, Main_OnHScroll);//

		//托盘消息处理
		case WM_TRAY:
		{
			//左击托盘图标
			if (WM_LBUTTONDOWN == lParam)
			{
				//恢复窗口显示
				ShowWindow(hWnd, SW_NORMAL);//
				//激活窗口
				SetForegroundWindow(hWnd);//
			}//
			//右击托盘图标
			else if (WM_RBUTTONDOWN == lParam)
			{
				POINT point;
				//初始化菜单
				HMENU hMenu=LoadMenu((HINSTANCE)GetWindowLong(hWnd,-6),MAKEINTRESOURCE(IDR_MENU2));//
				hMenu=GetSubMenu(hMenu,0);
				//获得鼠标位置
				GetCursorPos(&point);//
				//弹出菜单
				TrackPopupMenu(hMenu,TPM_RIGHTBUTTON,point.x,point.y,0,hWnd,NULL);//
				DestroyMenu(hMenu);
			}//
		}
		break;//

		//拖入文件消息处理
		case WM_DROPFILES:
		{	
			TCHAR FName[MAX_PATH];
			//获取文件数量
			UINT nFiles = DragQueryFile((HDROP)wParam,0xFFFFFFFF,NULL,0);
			for(UINT i=0; i<nFiles; i++)
			{
				//获取文件路径
				DragQueryFile((HDROP)wParam,i,FName,sizeof(FName));//
				HWND MList = GetDlgItem(hWnd,IDC_MList);
				ListBox_InsertString(MList,-1,FName);
				//检测列表中是否包含同名文件
				BOOL j = FALSE;
				for (int i = 0 ; i < ListBox_GetCount(MList)-1 ; i++)
				{
					TCHAR List[MAX_PATH];
					ListBox_GetText(MList,i,List);
					//若含有同名文件则删除新文件
					if (0 == strcmp(List,FName))
					{
						ListBox_DeleteString(MList,(ListBox_GetCount(MList)-1));//
						//设置同名文件为选中
						ListBox_SetCurSel(MList,i);//
						j = TRUE;
						break;
					}
				}
				if (!j)
				{
					//若无同名文件则设置新文件为选中
					ListBox_SetCurSel(MList,ListBox_GetCount(MList)-1);//
				}//
			}
			//重新计算列表总数
			ListTotal(hWnd);
			ListNow(hWnd);//
			//释放文件指针
			DragFinish((HDROP)wParam);//
		}
		break;//

		//热键消息处理
		case WM_HOTKEY:
		{
			//Alt+Shift+Z热键
			if (HK_Z == wParam)
			{
				//上一曲
				Last(hWnd);
				Stop(hWnd);
				Play(hWnd);//
			}//
			//Alt+Shift+X热键
			if (HK_X == wParam)
			{
				//下一曲
				Next(hWnd);
				Stop(hWnd);
				Play(hWnd);//
			}//
			//Alt+Shift+S热键
			if (HK_S == wParam)
			{
				//暂停
				Pause(hWnd);//
			}//
			//Alt+Shift+A热键
			if (HK_A == wParam)
			{
				//播放/继续
				TCHAR Choose1[MAX_PATH];
				GetDlgItemText(hWnd,IDC_EDITNOW,Choose1,sizeof(Choose1));
				HWND MList = GetDlgItem(hWnd,IDC_MList);
				int i = ListBox_GetCurSel(MList);
				TCHAR Choose2[MAX_PATH];
				ZeroMemory(&Choose2,sizeof(Choose2));
				ListBox_GetText(MList,i,Choose2);
				//若选中歌曲与正在播放歌曲相同则继续播放
				if (0 == strcmp(Choose1,Choose2))
				{	
					Play(hWnd);
				}//
				//若无选中歌曲则继续播放正在播放歌曲
				else if (-1 == i)
				{
					TCHAR * ShortName = GSNTE(hWnd);
					TCHAR play[MAX_PATH+10];
					wsprintf(play,"play %s",ShortName);
					mciSendString(play,"",0,NULL);
					fID3V1(hWnd);
				}//
				//若选中歌曲与正在播放歌曲不同则播放选中歌曲
				else
				{
					Stop(hWnd);
					Play(hWnd);
				}////
			}//
			//Alt+Shift+Q热键
			if (HK_Q == wParam)
			{
				//恢复窗口
				//恢复窗口显示
				ShowWindow(hWnd, SW_NORMAL);//
				//激活窗口
				SetForegroundWindow(hWnd);////	
			}//
			//Alt+Shift+W热键
			if (HK_W == wParam)
			{
				//隐藏窗口
				ShowWindow(hWnd, SW_HIDE);//	
			}//
			//Alt+Shift+C热键
			if (HK_C == wParam)
			{
				//音量下降5
				HWND VHK = GetDlgItem(hWnd,IDC_SOUND_SLIDER);
				//得到原音量
				int volume = (int)SendMessage(VHK,TBM_GETPOS,0,0);//
				//新音量为原音量减5 *6.4
				int Nvolume = volume - 32;//
				SendMessage(VHK,TBM_SETPOS,(WPARAM)TRUE,Nvolume);
				Voice(hWnd);//
			}//
			//Alt+Shift+D热键
			if (HK_D == wParam)
			{
				//音量上升5
				HWND VHK = GetDlgItem(hWnd,IDC_SOUND_SLIDER);
				//得到原音量
				int volume = (int)SendMessage(VHK,TBM_GETPOS,0,0);//
				//新音量为原音量加5 *6.4
				int Nvolume = volume + 32;
				SendMessage(VHK,TBM_SETPOS,(WPARAM)TRUE,Nvolume);
				Voice(hWnd);//	
			}//
			//Alt+Shift+E热键
			if (HK_E == wParam)
			{
				//停止
				Stop(hWnd);//	
			}//
			//Alt+Shift+R热键
			if (HK_R == wParam)
			{
				//退出程序
				Main_OnClose(hWnd);//	
			}//
			//Alt+Shift+G热键
			if (HK_G == wParam)
			{
				//静音
				onoff = Sof(hWnd,onoff,min,minhwnd);//	
			}//
			//Alt+Shift+B热键
			if (HK_B == wParam)
			{
				//若歌词窗口未打开
				if (!lrc)
				{
					lrc = TRUE;
					//弹出歌词窗口
					DialogBoxParam((HINSTANCE)GetWindowLong(hWnd,-6),MAKEINTRESOURCE(IDD_LRC),NULL,(DLGPROC)Lrc_Proc,(LPARAM)hWnd);//
				}
				//若歌词窗口已打开
				else
				{
					//向歌词窗口发送消息，使窗口显示
					PostMessage(lrchwnd,LRC_OPENED,0,0);//
				}//
	
			}//
			//Alt+Shift+F热键
			if (HK_F == wParam)
			{
				//上移
				Up(hWnd);//
			}//
			//Alt+Shift+V热键
			if (HK_V == wParam)
			{
				//下移
				Down(hWnd);//
			}//
		}
		break;//

		//歌词窗口关闭消息处理
		case LRC_CLOSE:
		{
			//改变歌词打开状态参数
			lrc = FALSE;//
			//歌词窗口句柄设为空
			lrchwnd = NULL;//
		}
		break;//

		//歌词窗口打开消息处理
		case LRC_OPEN:
		{
			//获取歌词窗口句柄
			lrchwnd = (HWND)lParam;//
		}
		break;//

		//搜索窗口打开消息处理
		case SEA_OPEN:
		{
			//获取搜索窗口句柄
			seahwnd = (HWND)lParam;//
		}
		break;//

		//设置窗口打开消息处理
		case SET_OPEN:
		{
			//获取设置窗口句柄
			sethwnd = (HWND)lParam;//
		}
		break;//

		//搜索窗口关闭消息处理
		case SEA_CLOSE:
		{
			//改变搜索打开状态参数
			sea = FALSE;//
			//搜索窗口句柄设为空
			seahwnd = NULL;//
		}
		break;//

		//设置窗口关闭消息处理
		case SET_CLOSE:
		{
			//改变设置打开状态参数
			set = FALSE;//
			//设置窗口句柄设为空
			sethwnd = NULL;//
		}
		break;//

		//歌词改变消息处理
		case LRC_CHANGE:
		{
			//根据选中歌词时间从相应位置播放
			playfrom(hWnd,(int)lParam);//
			if (TRUE == mil)
			{
				int stemp = (int)lParam/1000 + 1;
				int TTmin = stemp/60;
				int TTsec = stemp%60;
				PostMessage(milhwnd,MINILRC_TIME,TTmin,TTsec-1);
				PostMessage(milhwnd,MINILRC_TIME,TTmin,TTsec);
			}
		}
		break;//

		//迷你窗口打开消息处理
		case MINI_OPEN:
		{
			//获取歌词窗口句柄
			minhwnd = (HWND)lParam;//
		}
		break;//

		//迷你窗口关闭消息处理
		case MINI_CLOSE:
		{
			//改变迷你打开状态参数
			min = FALSE;//
			//迷你窗口句柄设为空
			minhwnd = NULL;//
			//恢复窗口显示
			ShowWindow(hWnd, SW_NORMAL);//
			//激活窗口
			SetForegroundWindow(hWnd);
			SetWindowPos(hWnd,HWND_NOTOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);//
		}
		break;//

		//迷你窗口控制消息处理
		case MINI_CONTROL:
		{
			switch(wParam)
			{
				//播放消息
				case MINI_PLAY:
				{
					//开始或继续播放
					PlayorContinue(hWnd);//
				}
				break;//

				//暂停消息
				case MINI_PAUSE:
				{
					//暂停正在播放歌曲
					Pause(hWnd);//
				}
				break;//

				//停止消息
				case MINI_STOP:
				{
					//停止正在播放歌曲
					Stop(hWnd);//
				}
				break;//

				//上一曲消息
				case MINI_LAST:
				{
					//停止正在播放歌曲，播放列表中选中歌曲的下一首，若未选中则播放最后一首歌曲
					Last(hWnd);
					Stop(hWnd);
					Play(hWnd);//
				}
				break;//

				//下一曲消息
				case MINI_NEXT:
				{
					//停止正在播放歌曲，播放列表中选中歌曲的下一首，若未选中则播放第一首歌曲
					Next(hWnd);
					Stop(hWnd);
					Play(hWnd);//
				}
				break;//

				//静音消息
				case MINI_SOF:
				{
					//若当前未静音则静音，若当前为静音则取消静音
					onoff = Sof(hWnd,onoff,min,minhwnd);//
				}
				break;//

				//音量滚动条消息
				case MINI_SOUND_SLIDER:
				{
					SendMessage(GetDlgItem(hWnd,IDC_SOUND_SLIDER),TBM_SETPOS,(unsigned long)TRUE,lParam);//
					Voice(hWnd);
				}
				break;//

				//进度滚动条消息
				case MINI_MUSIC_SLIDER:
				{
					SendMessage(GetDlgItem(hWnd,IDC_MUSIC_SLIDER),TBM_SETPOS,(unsigned long)TRUE,lParam);//
					NTime(hWnd,SB_ENDSCROLL);
				}
				break;//

			}
		}
		break;//

		//歌词窗口关闭消息处理
		case MINILRC_CLOSE:
		{
			//改变歌词打开状态参数
			mil = FALSE;//
			//歌词窗口句柄设为空
			milhwnd = NULL;//
		}
		break;//

		//歌词窗口打开消息处理
		case MINILRC_OPEN:
		{
			//获取歌词窗口句柄
			milhwnd = (HWND)lParam;//
		}
		break;//

	}

	return FALSE;
}//

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//初始化窗口函数
BOOL Main_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	//设置透明风格
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
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//允许接受文件
	DragAcceptFiles(hwnd,TRUE);//
	//初始化随机数（用于随机播放）
	srand((unsigned)time(NULL));//
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//注册热键
	RegisterHotKey(hwnd,HK_Z,MOD_ALT|MOD_SHIFT,'Z');
	RegisterHotKey(hwnd,HK_X,MOD_ALT|MOD_SHIFT,'X');
	RegisterHotKey(hwnd,HK_S,MOD_ALT|MOD_SHIFT,'S');
	RegisterHotKey(hwnd,HK_A,MOD_ALT|MOD_SHIFT,'A');
	RegisterHotKey(hwnd,HK_Q,MOD_ALT|MOD_SHIFT,'Q');
	RegisterHotKey(hwnd,HK_W,MOD_ALT|MOD_SHIFT,'W');
	RegisterHotKey(hwnd,HK_C,MOD_ALT|MOD_SHIFT,'C');
	RegisterHotKey(hwnd,HK_D,MOD_ALT|MOD_SHIFT,'D');
	RegisterHotKey(hwnd,HK_E,MOD_ALT|MOD_SHIFT,'E');
	RegisterHotKey(hwnd,HK_R,MOD_ALT|MOD_SHIFT,'R');
	RegisterHotKey(hwnd,HK_G,MOD_ALT|MOD_SHIFT,'G');
	RegisterHotKey(hwnd,HK_B,MOD_ALT|MOD_SHIFT,'B');
	RegisterHotKey(hwnd,HK_F,MOD_ALT|MOD_SHIFT,'F');
	RegisterHotKey(hwnd,HK_V,MOD_ALT|MOD_SHIFT,'V');//
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//产生托盘图标，允许托盘消息
	noti.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	noti.hIcon = (HICON)LoadImage((HINSTANCE)GetWindowLong(hwnd, -6), "ID_ICON.ico",IMAGE_ICON,0,0,LR_LOADFROMFILE);	
	noti.hWnd = hwnd;
	strcpy(noti.szTip,TEXT("X哥音乐播放器"));
	noti.uCallbackMessage = WM_TRAY;
	noti.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP;
	noti.uID = ID_ICON;
	Shell_NotifyIcon(NIM_ADD,&noti);//
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//初始化时间/列表显示
	SetDlgItemText(hwnd,IDC_EDITTIME,TEXT("00:00/00:00"));
	SetDlgItemText(hwnd,IDC_EDITLIST,TEXT("000/000"));
	GetLocalTime(&STime);
	TCHAR time[6];
	wsprintf(time,"%02d:%02d",STime.wHour,STime.wMinute);
	SetDlgItemText(hwnd,IDC_EDITSTIME,time);//
	//设置系统时间获取定时器
	SetTimer(hwnd,TIMER_STIME,TIMER_STIME_TIME,(TIMERPROC)STimeF);//
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//初始化组合框显示
	//播放顺序组合框初始化
	HWND CBSX = GetDlgItem(hwnd,IDC_COMBOSX);
	ComboBox_InsertString(CBSX,-1,TEXT("单曲循环"));
	ComboBox_InsertString(CBSX,-1,TEXT("列表循环"));
	ComboBox_InsertString(CBSX,-1,TEXT("随机播放"));
	ComboBox_SetCurSel(CBSX,RANDOM);//

	//保存列表组合框初始化
	HWND CSL = GetDlgItem(hwnd,IDC_CSL);
	ComboBox_InsertString(CSL,-1,TEXT("列表1"));
	ComboBox_InsertString(CSL,-1,TEXT("列表2"));
	ComboBox_InsertString(CSL,-1,TEXT("列表3"));
	ComboBox_InsertString(CSL,-1,TEXT("列表4"));
	ComboBox_SetCurSel(CSL,0);//

	//打开列表组合框初始化
	HWND COL = GetDlgItem(hwnd,IDC_COL);
	ComboBox_InsertString(COL,-1,TEXT("列表1"));
	ComboBox_InsertString(COL,-1,TEXT("列表2"));
	ComboBox_InsertString(COL,-1,TEXT("列表3"));
	ComboBox_InsertString(COL,-1,TEXT("列表4"));
	ComboBox_SetCurSel(COL,0);////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//音量滚动条初始化
	HWND VHK = GetDlgItem(hwnd,IDC_SOUND_SLIDER);
	//设置滚动条最小值为0
	SendMessage(VHK,TBM_SETRANGEMIN,(WPARAM)TRUE,0);//
	//设置滚动条最大值为640 /6.4
	SendMessage(VHK,TBM_SETRANGEMAX,(WPARAM)TRUE,640);//
	//设置初始位置为384	/6.4						   
	SendMessage(VHK,TBM_SETPOS,(WPARAM)TRUE,384);//
	//设置按←键时，减小32 /6.4
	SendMessage(VHK,TBM_SETPAGESIZE,0, (LPARAM)32);//
	//设置按→键时，增大32 /6.4
	SendMessage(VHK,TBM_SETLINESIZE,0, (LPARAM)32);//
	SetDlgItemText(hwnd,IDC_EDITVOICE,TEXT(" 60"));//
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 	//播放进度滚动条初始化
	HWND MHK = GetDlgItem(hwnd,IDC_MUSIC_SLIDER);
	//设置滚动条最小值为0
	SendMessage(MHK,TBM_SETRANGEMIN,(WPARAM)TRUE,0);//
	//设置滚动条最大值为1000
	SendMessage(MHK,TBM_SETRANGEMAX,(WPARAM)TRUE,0);//
	//设置初始位置为0
	SendMessage(MHK,TBM_SETPOS,(WPARAM)TRUE,0);//
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//若由文件拖入打开，处理文件
	if (strcmp("",pfo))
	{
		TCHAR of[MAX_PATH];
		//若由多个文件拖入打开
		if('"' == pfo[0])
		{
			int m = 1;
			//按"截断，将文件目录复制出来
			while('"' != pfo[m])
			{
				of[m-1] = pfo[m];
				++m;
			}
			of[m-1] = '\0';//
			HWND MList = GetDlgItem(hwnd,IDC_MList);
			//添加文件
			ListBox_InsertString(MList,-1,of);
			//若列表中有同名文件则删除新文件
			for (int i = 0 ; i < ListBox_GetCount(MList)-1 ; i++)
			{
				TCHAR List[MAX_PATH];
				ListBox_GetText(MList,i,List);
				if (!strcmp(List,of))
				{
					ListBox_DeleteString(MList,(ListBox_GetCount(MList)-1));
					break;
				}
			}////
		}//
		//若由单个文件拖入打开
		else
		{
			HWND MList = GetDlgItem(hwnd,IDC_MList);
			ListBox_InsertString(MList,-1,pfo);
			//若列表中有同名文件则删除新文件
			for (int i = 0 ; i < ListBox_GetCount(MList)-1 ; i++)
			{
				TCHAR List[MAX_PATH];
				ListBox_GetText(MList,i,List);
				if (!strcmp(List,pfo))
				{
					ListBox_DeleteString(MList,(ListBox_GetCount(MList)-1));
					break;
				}
			}//
		}//
		//重新计算列表总数
		ListTotal(hwnd);
		ListNow(hwnd);//
	}//
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	return TRUE;
}//

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//控件消息响应函数
void Main_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	HWND MList = GetDlgItem(hwnd,IDC_MList);
	switch(id)
	{
		//上曲按钮响应
		case ID_traLAST: case ID_menLAST: case IDC_btnLAST:
		{
			//停止正在播放歌曲，播放列表中选中歌曲的下一首，若未选中则播放最后一首歌曲
			Last(hwnd);
			Stop(hwnd);
			Play(hwnd);//
		}
		break;//

		//下曲按钮响应
		case ID_traNEXT: case ID_menNEXT: case IDC_btnNEXT:
		{
			//停止正在播放歌曲，播放列表中选中歌曲的下一首，若未选中则播放第一首歌曲
			Next(hwnd);
			Stop(hwnd);
			Play(hwnd);//
		}
		break;//
		
		//在歌曲列表中双击响应
		case IDC_MList:
		{
			if(LBN_DBLCLK==codeNotify)
			{
				//停止正在播放歌曲，播放选中歌曲
				Stop(hwnd);
				Play(hwnd);//
			}
		}
		break;//
		
		//播放/继续按钮响应
		case ID_traPLAY: case ID_menPLAY: case IDC_btnPLAY:
		{
			//开始或继续播放
			PlayorContinue(hwnd);//
		}
		break;//


		//停止按钮响应
		case ID_traSTOP: case ID_menSTOP: case IDC_btnSTOP:
		{
			//停止正在播放歌曲
			Stop(hwnd);//
		}
		break;//

		//暂停按钮响应
		case ID_traPAUSE: case ID_menPAUSE: case IDC_btnPAUSE:
		{
			//暂停正在播放歌曲
			Pause(hwnd);//
		}
		break;//
		
		//添加按钮响应
		case IDC_btnADD: case ID_menADD:
		{
			//打开添加歌曲窗口
			openf(hwnd);//
			//重新计算列表总数
			ListTotal(hwnd);
			ListNow(hwnd);//
		}
		break;//

		//删除按钮响应
		case ID_menDEL: case IDC_btnDEL:
		{
			//删除选中文件
			Del(hwnd);//
			//重新计算列表总数
			ListTotal(hwnd);
			ListNow(hwnd);//
			
		}
		break;//

		//上移按钮响应
		case IDC_btnUP:
		{
			//将选中文件上移
			Up(hwnd);//
		}
		break;//

		//下移按钮响应
		case IDC_btnDOWN:
		{
			//将选中文件下移
			Down(hwnd);//
		}
		break;//

		//静音按钮响应
		case IDC_SOF: case ID_menSOF: case ID_traSOF:
		{
			//若当前未静音则静音，若当前为静音则取消静音
			onoff = Sof(hwnd,onoff,min,minhwnd);//
		}
		break;//

		//退出按钮响应
		case ID_traEXIT: case ID_menEXIT:
		{
			//调用退出函数
			Main_OnClose(hwnd);//
		}
		break;//
		
		//播放顺序选中单曲循环选项响应
		case ID_traSELFLOOP: case ID_menSELFLOOP:
		{
			//将播放顺序组合框设为单曲循环选中
			ComboBox_SetCurSel(GetDlgItem(hwnd,IDC_COMBOSX),SELFLOOP);//
		}
		break;//

		//播放顺序选中列表循环选项响应
		case ID_traLISTLOOP: case ID_menLISTLOOP:
		{
			ComboBox_SetCurSel(GetDlgItem(hwnd,IDC_COMBOSX),LISTLOOP);//
		}
		break;//

		//播放顺序选中随机播放选项响应
		case ID_traRANDOM: case ID_menRANDOM:
		{
			//将播放顺序组合框设为随机播放选中
			ComboBox_SetCurSel(GetDlgItem(hwnd,IDC_COMBOSX),RANDOM);//
		}
		break;//

		//清空按钮响应
		case ID_menCLEAN: case IDC_btnCLEAN:
		{
			//弹出确认对话框，若确定则清空列表
			int yn = MessageBox(hwnd,TEXT("确定要清空列表吗？"),TEXT("清空列表"),MB_YESNO|MB_ICONQUESTION);
			if (IDYES == yn)
			{
				Stop(hwnd);
				SendDlgItemMessage(hwnd,IDC_MList,LB_RESETCONTENT,0,0);
				ListTotal(hwnd);
				ListNow(hwnd);
			}
			else
				return;//
		}
		break;//

		//隐藏窗口按钮响应
		case ID_menHIDE: case IDC_btnHIDE:
		{ 
			//隐藏窗口
			ShowWindow(hwnd, SW_HIDE);//
		}
		break;//

		//关于按钮响应
		case ID_menABOUT:
		{
			//弹出关于对话框
			MessageBox(hwnd,TEXT("X哥音乐播放器V7.0\n感谢您的支持!\n作者:徐梦翔\nQQ:834489218"),TEXT("关于"),MB_OK);//
			//彩蛋
			cd++;
			if (3 == cd)
				MessageBox(hwnd,TEXT("      ***             ***\n  **      **       **      **\n*              *              *\n*                             *\n   **                     **\n      **               **\n          **       **\n             *   *\n               *"),TEXT("感谢"),MB_OK);//
		}
		break;//
		
		//恢复窗口按钮响应
		case ID_traRESUME:
		{
			//恢复窗口显示
			ShowWindow(hwnd, SW_NORMAL);//
			//激活窗口
			SetForegroundWindow(hwnd);//
		}
		break;//

		//迷你按钮响应
		case IDC_btnMINI:
		{
			//若迷你窗口未打开
			if (!min)
			{
				min = TRUE;
				//隐藏窗口
				ShowWindow(hwnd, SW_HIDE);//
				//弹出迷你窗口
				DialogBoxParam((HINSTANCE)GetWindowLong(hwnd,-6),MAKEINTRESOURCE(IDD_MINI),NULL,(DLGPROC)Mini_Proc,(LPARAM)hwnd);//
			}
			//若迷你窗口已打开
			else
			{
				//隐藏窗口
				ShowWindow(hwnd, SW_HIDE);//
				//向迷你窗口发送消息，使窗口显示
				PostMessage(minhwnd,MINI_OPENED,0,0);//
			}//
		}
		break;//

		//打开列表按钮响应
		case ID_menOLIST: case IDC_btnOLIST:
		{
			//弹出确认对话框，若确定则打开选中列表
			openList(hwnd);//
		}
		break;//

		//保存列表按钮响应
		case ID_menSLIST: case IDC_btnSLIST:
		{
			//弹出确认对话框，若确定则保存至选中列表
			saveList(hwnd);//
		}
		break;//

		//音量升高按钮响应
		case ID_menVUP: case ID_traVUP:
		{
			HWND VHK = GetDlgItem(hwnd,IDC_SOUND_SLIDER);
			//音量升高5 *6.4
			int volume = (int)SendMessage(VHK,TBM_GETPOS,0,0);
			int Nvolume = volume + 32;
			SendMessage(VHK,TBM_SETPOS,(WPARAM)TRUE,Nvolume);
			Voice(hwnd);//
		}
		break;//

		//音量降低按钮响应
		case ID_menVDOWN: case ID_traVDOWN:
		{
			HWND VHK = GetDlgItem(hwnd,IDC_SOUND_SLIDER);
			//音量降低5 *6.4
			int volume = (int)SendMessage(VHK,TBM_GETPOS,0,0);
			int Nvolume = volume - 32;
			SendMessage(VHK,TBM_SETPOS,(WPARAM)TRUE,Nvolume);
			Voice(hwnd);//
		}
		break;//

		//搜索歌曲按钮响应
		case IDC_btnSEARCH: case ID_traSEARCH:
		{
			//若搜索歌曲窗口未打开
			if (!sea)
			{
				sea = TRUE;
				//弹出搜索歌曲窗口
				DialogBoxParam((HINSTANCE)GetWindowLong(hwnd,-6),MAKEINTRESOURCE(IDD_SEARCH),NULL,(DLGPROC)Search_Proc,(LPARAM)hwnd);//
			}
			//若搜索歌曲窗口已打开
			else
			{
				//向搜索歌曲窗口发送消息，使窗口显示
				PostMessage(seahwnd,SEA_OPENED,0,0);//
			}//
		}
		break;//

		//设置按钮响应
		case ID_menSET: case ID_traSET:
		{
			//若设置窗口未打开
			if (!set)
			{
				set = TRUE;
				//弹出设置窗口
				DialogBoxParam((HINSTANCE)GetWindowLong(hwnd,-6),MAKEINTRESOURCE(IDD_SET),hwnd,(DLGPROC)Set_Proc,(LPARAM)hwnd);//
			}
		}
		break;//

		//打开歌词按钮响应
		case IDC_btnLRC: case ID_menLRC: case ID_traLRC:
		{
			//若歌词窗口未打开
			if (!lrc)
			{
				lrc = TRUE;
				//弹出歌词窗口
				DialogBoxParam((HINSTANCE)GetWindowLong(hwnd,-6),MAKEINTRESOURCE(IDD_LRC),NULL,(DLGPROC)Lrc_Proc,(LPARAM)hwnd);//
			}
			//若歌词窗口已打开
			else
			{
				//向歌词窗口发送消息，使窗口显示
				PostMessage(lrchwnd,LRC_OPENED,0,0);//
			}//
		}
		break;//

		//睡眠模式按钮响应
		case IDC_btnSLEEP:
		{
			HWND SleepBTN = GetDlgItem(hwnd,IDC_btnSLEEP);
			//若未进入睡眠模式，确认是否进入
			if (!sleep)
			{
				//弹出确认对话框，若确认则进入睡眠模式
				int yn = MessageBox(hwnd,TEXT("需要保存列表吗？"),TEXT("退出"),MB_YESNO|MB_ICONQUESTION);
				if (IDYES == yn)
					return;
				int i = MessageBox(hwnd,TEXT("确定要进入睡眠模式吗？"),TEXT("睡眠模式"),MB_YESNO|MB_ICONQUESTION);
				if (IDYES == i)
				{
					//读取设置文件中设置的睡眠等待时间
					int st = SleepTime();//
					TCHAR temp[256];
					//在睡眠等待时间后关机
					wsprintf(temp,"shutdown -s -t %d",st+15);
					system(temp);//
					HWND COL = GetDlgItem(hwnd,IDC_COMBOSX);
					//设置播放顺序为列表循环
					ComboBox_SetCurSel(COL,LISTLOOP);//
					wsprintf(temp,"已进入睡眠模式，计算机将在%d分钟后关闭！",st/60);
					MessageBox(hwnd,TEXT(temp),TEXT("睡眠模式"),MB_OK|MB_ICONINFORMATION);
					//将按钮标题更改
					Button_SetText(SleepBTN,TEXT("退出睡眠模式"));//
					//将睡眠模式变量更改为进入睡眠模式
					sleep = TRUE;//
				}
				else
					return;
				//
			}//
			//若已进入睡眠模式，确认是否退出
			else
			{
				//弹出确认对话框，若确认则退出睡眠模式
				int i = MessageBox(hwnd,TEXT("确定要退出睡眠模式吗？"),TEXT("睡眠模式"),MB_YESNO|MB_ICONQUESTION);
				if (IDYES == i)
				{	
					//取消自动关机
					system("shutdown -a");
					HWND COL = GetDlgItem(hwnd,IDC_COMBOSX);
					//设置播放顺序为随机播放
					ComboBox_SetCurSel(COL,RANDOM);//
					MessageBox(hwnd,TEXT("已退出睡眠模式！"),TEXT("睡眠模式"),MB_OK|MB_ICONINFORMATION);
					//将按钮标题更改
					Button_SetText(SleepBTN,TEXT("睡眠模式(&J)"));
					//将睡眠模式变量更改为未进入睡眠模式
					sleep = FALSE;
				}
				else
					return;
			}
		}
		break;//

		case IDC_btnMINILRC:
		{
			//若歌词窗口未打开
			if (!mil)
			{
				mil = TRUE;
				Button_SetText(GetDlgItem(hwnd,IDC_btnMINILRC),TEXT("关闭桌面歌词"));
				//弹出歌词窗口
				DialogBoxParam((HINSTANCE)GetWindowLong(hwnd,-6),MAKEINTRESOURCE(IDD_MINILRC),NULL,(DLGPROC)MiniLrc_Proc,(LPARAM)hwnd);//
			}
			//若歌词窗口已打开
			else
			{
				mil = FALSE;
				Button_SetText(GetDlgItem(hwnd,IDC_btnMINILRC),TEXT("桌面歌词(&P)"));
				//向歌词窗口发送消息，使窗口关闭
				PostMessage(milhwnd,PRO_CLOSE,0,0);//
			}//
		}
		break;

		default:
		break;
	}
}//

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//滚动条消息响应函数
void Main_OnHScroll(HWND hwnd, HWND hwndCtl, UINT code, int pos)
{
	switch(GetDlgCtrlID(hwndCtl))
	{
		//音量滚动条消息响应
		case IDC_SOUND_SLIDER:
		{
			//根据音量滚动条位置更改音量
			Voice(hwnd);//
		}
		break;//

		//播放进度滚动条消息响应
		case IDC_MUSIC_SLIDER:
		{
			//根据进度滚动条位置从相应位置播放
			NTime(hwnd,code);//
			if (lrc)
				PostMessage(lrchwnd,LRC_TIME,Nmin,Nsec);
		}
		break;//

		default:
		break;
	}
}//

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//退出函数
void Main_OnClose(HWND hwnd)
{
	if (IDYES == MessageBox(hwnd,TEXT("需要保存列表吗？"),TEXT("退出"),MB_YESNO|MB_ICONQUESTION))
		return;
	int i = MessageBox(hwnd,TEXT("确定要退出吗？"),TEXT("退出"),MB_YESNO|MB_ICONQUESTION);
	if (IDYES == i)
	{
		//取消托盘图标
		Shell_NotifyIcon(NIM_DELETE,&noti);//
		Stop(hwnd);
		//若打开子窗口，发送关闭消息
		if (seahwnd != NULL)
			PostMessage(seahwnd,PRO_CLOSE,0,0);
		if (sethwnd != NULL)
			PostMessage(sethwnd,PRO_CLOSE,0,0);
		if (lrchwnd != NULL)
			PostMessage(lrchwnd,PRO_CLOSE,0,0);
		if (minhwnd != NULL)
			PostMessage(minhwnd,PRO_CLOSE,0,0);
		if (milhwnd != NULL)
			PostMessage(milhwnd,PRO_CLOSE,0,0);//
		EndDialog(hwnd, 0);
	}
	else
		return;//
}//