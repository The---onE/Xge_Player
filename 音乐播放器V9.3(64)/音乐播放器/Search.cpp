/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//搜索歌曲窗口文件
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <windows.h>
#include <windowsx.h>
#include <shellapi.h>

#include <stdio.h>
#include <stdlib.h>

#include "resource.h"
#include "Search.h"

HWND seaphwnd;
//定义列表搜索起始位置
int startindex = 0;//
//定义上次搜索内容
TCHAR search[MAX_PATH] = "";//


//搜索窗口消息处理函数
BOOL WINAPI Search_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
	{
        HANDLE_MSG(hWnd, WM_INITDIALOG, Search_OnInitDialog);
        HANDLE_MSG(hWnd, WM_COMMAND, Search_OnCommand);
		HANDLE_MSG(hWnd, WM_CLOSE, Search_OnClose);

		//主窗口关闭后关闭其他子窗口
		case PRO_CLOSE:
		{
			EndDialog(hWnd, 0);
		}
		break;//

		//打开搜索窗口消息处理
		case SEA_OPENED:
		{
			//若搜索窗口已打开则使其前端显示
			SetWindowPos(hWnd,HWND_NOTOPMOST,0,0,0,0, SWP_NOMOVE|SWP_NOSIZE);//
		}
		break;//
	}
	
    return FALSE;
}//

//初始化窗口函数
BOOL Search_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	//获取主窗口句柄
	seaphwnd = (HWND)lParam;//
	//向主窗口发送句柄
	PostMessage(seaphwnd,SEA_OPEN,0,(LPARAM)hwnd);//
	return TRUE;
}//

//控件消息响应函数
void Search_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch(id)
	{
		//列表按钮响应
		case IDC_LIST:
		{
			TCHAR Name[MAX_PATH];
			GetDlgItemText(hwnd,IDC_MUSICNAME,Name,sizeof(Name));
			//搜索含搜索字符串（从头开始匹配，必须含文件路径）的选项
			int foundindex = ListBox_SelectString(GetDlgItem(seaphwnd,IDC_MList),startindex,Name);//
			//若未找到弹出提示信息
			if (-1 == foundindex)
				MessageBox(hwnd,"未找到搜索项，请检查是否为全路径且字符为半角字符，或检查列表","搜索失败",MB_OK);//
			//若重复搜索从找到项之后搜索
			if (!strcmp(Name,search))
				startindex = foundindex;//
			//若重新搜索从列表起始位置搜索
			else
				startindex = 0;//
			strcpy_s(search,Name);
			
			//彩蛋
			if (!strcmp(Name,"148550528491"))
			{
				SetDlgItemText(hwnd,IDC_MUSICNAME,"telnet towel.blinkenlights.nl");
				MessageBox(hwnd,"                            感谢您的支持！\n\n复制文字到开始-运行中确定即可观看\n若错误请打开控制面板-程序-打开或关闭Windows功能\n勾选Telnet客户端，且必须连接网络","感谢",MB_OK);//
			}
		}
		break;//
	
		//百度按钮响应
		case IDC_BAIDU:
		{
			TCHAR Name[MAX_PATH];
			TCHAR Search[MAX_PATH+10];
			GetDlgItemText(hwnd,IDC_MUSICNAME,Name,sizeof(Name));
			//百度MP3搜索格式
			wsprintf(Search,"http://mp3.baidu.com/m?word=%s&lm=-1&f=ms&tn=baidump3&ct=134217728&lf=&rn=",Name);//
			//打开网址函数
			ShellExecute(NULL,"open",Search,NULL,NULL,SW_SHOW);//
		}
		break;//

		//搜搜按钮响应
		case IDC_SOSO:
		{
			TCHAR Name[MAX_PATH];
			TCHAR Search[MAX_PATH+10];
			GetDlgItemText(hwnd,IDC_MUSICNAME,Name,sizeof(Name));
			//搜搜音乐搜索格式
			wsprintf(Search,"http://cgi.music.soso.com/fcgi-bin/m.q?w=%s&source=1&t=0",Name);//
			//打开网址函数
			ShellExecute(NULL,"open",Search,NULL,NULL,SW_SHOW);//
		}
		break;//

		//搜狗
		case IDC_SOGOU:
		{
			TCHAR Name[MAX_PATH];
			TCHAR Search[MAX_PATH+10];
			GetDlgItemText(hwnd,IDC_MUSICNAME,Name,sizeof(Name));
			//搜狗音乐搜索格式
			wsprintf(Search,"http://mp3.sogou.com/music.so?query=%s&class=1&st=&ac=1&pf=&_asf=mp3.sogou.com&_ast=1335892391&p=&w=&w=02009900&sut=2277&sst0=1335892390859",Name);//
			//打开网址函数
			ShellExecute(NULL,"open",Search,NULL,NULL,SW_SHOW);//
		}
		break;//


		default:
		break;
	}


}//

//退出函数
void Search_OnClose(HWND hwnd)
{
	//向主窗口发送关闭消息
	PostMessage(seaphwnd,SEA_CLOSE,0,0);//
	EndDialog(hwnd, 0);
}//