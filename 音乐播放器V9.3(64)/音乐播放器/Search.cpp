/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�������������ļ�
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
//�����б�������ʼλ��
int startindex = 0;//
//�����ϴ���������
TCHAR search[MAX_PATH] = "";//


//����������Ϣ������
BOOL WINAPI Search_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
	{
        HANDLE_MSG(hWnd, WM_INITDIALOG, Search_OnInitDialog);
        HANDLE_MSG(hWnd, WM_COMMAND, Search_OnCommand);
		HANDLE_MSG(hWnd, WM_CLOSE, Search_OnClose);

		//�����ڹرպ�ر������Ӵ���
		case PRO_CLOSE:
		{
			EndDialog(hWnd, 0);
		}
		break;//

		//������������Ϣ����
		case SEA_OPENED:
		{
			//�����������Ѵ���ʹ��ǰ����ʾ
			SetWindowPos(hWnd,HWND_NOTOPMOST,0,0,0,0, SWP_NOMOVE|SWP_NOSIZE);//
		}
		break;//
	}
	
    return FALSE;
}//

//��ʼ�����ں���
BOOL Search_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	//��ȡ�����ھ��
	seaphwnd = (HWND)lParam;//
	//�������ڷ��;��
	PostMessage(seaphwnd,SEA_OPEN,0,(LPARAM)hwnd);//
	return TRUE;
}//

//�ؼ���Ϣ��Ӧ����
void Search_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch(id)
	{
		//�б�ť��Ӧ
		case IDC_LIST:
		{
			TCHAR Name[MAX_PATH];
			GetDlgItemText(hwnd,IDC_MUSICNAME,Name,sizeof(Name));
			//�����������ַ�������ͷ��ʼƥ�䣬���뺬�ļ�·������ѡ��
			int foundindex = ListBox_SelectString(GetDlgItem(seaphwnd,IDC_MList),startindex,Name);//
			//��δ�ҵ�������ʾ��Ϣ
			if (-1 == foundindex)
				MessageBox(hwnd,"δ�ҵ�����������Ƿ�Ϊȫ·�����ַ�Ϊ����ַ��������б�","����ʧ��",MB_OK);//
			//���ظ��������ҵ���֮������
			if (!strcmp(Name,search))
				startindex = foundindex;//
			//�������������б���ʼλ������
			else
				startindex = 0;//
			strcpy_s(search,Name);
			
			//�ʵ�
			if (!strcmp(Name,"148550528491"))
			{
				SetDlgItemText(hwnd,IDC_MUSICNAME,"telnet towel.blinkenlights.nl");
				MessageBox(hwnd,"                            ��л����֧�֣�\n\n�������ֵ���ʼ-������ȷ�����ɹۿ�\n��������򿪿������-����-�򿪻�ر�Windows����\n��ѡTelnet�ͻ��ˣ��ұ�����������","��л",MB_OK);//
			}
		}
		break;//
	
		//�ٶȰ�ť��Ӧ
		case IDC_BAIDU:
		{
			TCHAR Name[MAX_PATH];
			TCHAR Search[MAX_PATH+10];
			GetDlgItemText(hwnd,IDC_MUSICNAME,Name,sizeof(Name));
			//�ٶ�MP3������ʽ
			wsprintf(Search,"http://mp3.baidu.com/m?word=%s&lm=-1&f=ms&tn=baidump3&ct=134217728&lf=&rn=",Name);//
			//����ַ����
			ShellExecute(NULL,"open",Search,NULL,NULL,SW_SHOW);//
		}
		break;//

		//���Ѱ�ť��Ӧ
		case IDC_SOSO:
		{
			TCHAR Name[MAX_PATH];
			TCHAR Search[MAX_PATH+10];
			GetDlgItemText(hwnd,IDC_MUSICNAME,Name,sizeof(Name));
			//��������������ʽ
			wsprintf(Search,"http://cgi.music.soso.com/fcgi-bin/m.q?w=%s&source=1&t=0",Name);//
			//����ַ����
			ShellExecute(NULL,"open",Search,NULL,NULL,SW_SHOW);//
		}
		break;//

		//�ѹ�
		case IDC_SOGOU:
		{
			TCHAR Name[MAX_PATH];
			TCHAR Search[MAX_PATH+10];
			GetDlgItemText(hwnd,IDC_MUSICNAME,Name,sizeof(Name));
			//�ѹ�����������ʽ
			wsprintf(Search,"http://mp3.sogou.com/music.so?query=%s&class=1&st=&ac=1&pf=&_asf=mp3.sogou.com&_ast=1335892391&p=&w=&w=02009900&sut=2277&sst0=1335892390859",Name);//
			//����ַ����
			ShellExecute(NULL,"open",Search,NULL,NULL,SW_SHOW);//
		}
		break;//


		default:
		break;
	}


}//

//�˳�����
void Search_OnClose(HWND hwnd)
{
	//�������ڷ��͹ر���Ϣ
	PostMessage(seaphwnd,SEA_CLOSE,0,0);//
	EndDialog(hwnd, 0);
}//