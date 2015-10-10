/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�����������ļ�
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
//���������ں���
#include "MainDlg.h"
//������������������
#include "Search.h"//
//���������ô���
#include "SetDlg.h"//
//��������ʴ���
#include "Lrc.h"//
//�������㴰��
#include "Mini.h"//
//����������
#include "MiniLrc.h"//
//�ں�ȫ�������ں�������
#include "control.h"//

#pragma comment(lib,"SkinH.lib")
#pragma comment(lib,"Winmm.lib")


//�����öԻ���//TCHAR test[MAX_PATH];
				//itoa(inttest,test,10);
				//MessageBox(hwnd,test,"1",MB_OK);
				//MessageBox(hwnd,"OK","1",MB_OK);

//������ں���
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	//ע�Ტ��ʼ��������
	InitCommonControls();//
	//����skinh.sheƤ��
	SkinH_Attach();//
	//ΪƤ��������Ч
	SkinH_SetAero(1);
	SkinH_AdjustAero(127,0,0,4,0,0,-1,-1,-1);
	SkinH_SetMenuAlpha(191);//
	//�����ļ�����򿪣���ȡ�ļ�·��
	pfo = lpCmdLine;//
	//����������
	DialogBox(hInstance,MAKEINTRESOURCE(IDD_MAIN),NULL,(DLGPROC)Main_Proc); //
	return 0;
}//

//��������Ϣ������
BOOL WINAPI Main_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		HANDLE_MSG(hWnd,WM_INITDIALOG, Main_OnInitDialog);
		HANDLE_MSG(hWnd,WM_COMMAND, Main_OnCommand);
		HANDLE_MSG(hWnd,WM_CLOSE, Main_OnClose);
		//�����������Ϣ��Ӧ
		HANDLE_MSG(hWnd,WM_HSCROLL, Main_OnHScroll);//

		//������Ϣ����
		case WM_TRAY:
		{
			//�������ͼ��
			if (WM_LBUTTONDOWN == lParam)
			{
				//�ָ�������ʾ
				ShowWindow(hWnd, SW_NORMAL);//
				//�����
				SetForegroundWindow(hWnd);//
			}//
			//�һ�����ͼ��
			else if (WM_RBUTTONDOWN == lParam)
			{
				POINT point;
				//��ʼ���˵�
				HMENU hMenu=LoadMenu((HINSTANCE)GetWindowLong(hWnd,-6),MAKEINTRESOURCE(IDR_MENU2));//
				hMenu=GetSubMenu(hMenu,0);
				//������λ��
				GetCursorPos(&point);//
				//�����˵�
				TrackPopupMenu(hMenu,TPM_RIGHTBUTTON,point.x,point.y,0,hWnd,NULL);//
				DestroyMenu(hMenu);
			}//
		}
		break;//

		//�����ļ���Ϣ����
		case WM_DROPFILES:
		{	
			TCHAR FName[MAX_PATH];
			//��ȡ�ļ�����
			UINT nFiles = DragQueryFile((HDROP)wParam,0xFFFFFFFF,NULL,0);
			for(UINT i=0; i<nFiles; i++)
			{
				//��ȡ�ļ�·��
				DragQueryFile((HDROP)wParam,i,FName,sizeof(FName));//
				HWND MList = GetDlgItem(hWnd,IDC_MList);
				ListBox_InsertString(MList,-1,FName);
				//����б����Ƿ����ͬ���ļ�
				BOOL j = FALSE;
				for (int i = 0 ; i < ListBox_GetCount(MList)-1 ; i++)
				{
					TCHAR List[MAX_PATH];
					ListBox_GetText(MList,i,List);
					//������ͬ���ļ���ɾ�����ļ�
					if (0 == strcmp(List,FName))
					{
						ListBox_DeleteString(MList,(ListBox_GetCount(MList)-1));//
						//����ͬ���ļ�Ϊѡ��
						ListBox_SetCurSel(MList,i);//
						j = TRUE;
						break;
					}
				}
				if (!j)
				{
					//����ͬ���ļ����������ļ�Ϊѡ��
					ListBox_SetCurSel(MList,ListBox_GetCount(MList)-1);//
				}//
			}
			//���¼����б�����
			ListTotal(hWnd);
			ListNow(hWnd);//
			//�ͷ��ļ�ָ��
			DragFinish((HDROP)wParam);//
		}
		break;//

		//�ȼ���Ϣ����
		case WM_HOTKEY:
		{
			//Alt+Shift+Z�ȼ�
			if (HK_Z == wParam)
			{
				//��һ��
				Last(hWnd);
				Stop(hWnd);
				Play(hWnd);//
			}//
			//Alt+Shift+X�ȼ�
			if (HK_X == wParam)
			{
				//��һ��
				Next(hWnd);
				Stop(hWnd);
				Play(hWnd);//
			}//
			//Alt+Shift+S�ȼ�
			if (HK_S == wParam)
			{
				//��ͣ
				Pause(hWnd);//
			}//
			//Alt+Shift+A�ȼ�
			if (HK_A == wParam)
			{
				//����/����
				TCHAR Choose1[MAX_PATH];
				GetDlgItemText(hWnd,IDC_EDITNOW,Choose1,sizeof(Choose1));
				HWND MList = GetDlgItem(hWnd,IDC_MList);
				int i = ListBox_GetCurSel(MList);
				TCHAR Choose2[MAX_PATH];
				ZeroMemory(&Choose2,sizeof(Choose2));
				ListBox_GetText(MList,i,Choose2);
				//��ѡ�и��������ڲ��Ÿ�����ͬ���������
				if (0 == strcmp(Choose1,Choose2))
				{	
					Play(hWnd);
				}//
				//����ѡ�и���������������ڲ��Ÿ���
				else if (-1 == i)
				{
					TCHAR * ShortName = GSNTE(hWnd);
					TCHAR play[MAX_PATH+10];
					wsprintf(play,"play %s",ShortName);
					mciSendString(play,"",0,NULL);
					fID3V1(hWnd);
				}//
				//��ѡ�и��������ڲ��Ÿ�����ͬ�򲥷�ѡ�и���
				else
				{
					Stop(hWnd);
					Play(hWnd);
				}////
			}//
			//Alt+Shift+Q�ȼ�
			if (HK_Q == wParam)
			{
				//�ָ�����
				//�ָ�������ʾ
				ShowWindow(hWnd, SW_NORMAL);//
				//�����
				SetForegroundWindow(hWnd);////	
			}//
			//Alt+Shift+W�ȼ�
			if (HK_W == wParam)
			{
				//���ش���
				ShowWindow(hWnd, SW_HIDE);//	
			}//
			//Alt+Shift+C�ȼ�
			if (HK_C == wParam)
			{
				//�����½�5
				HWND VHK = GetDlgItem(hWnd,IDC_SOUND_SLIDER);
				//�õ�ԭ����
				int volume = (int)SendMessage(VHK,TBM_GETPOS,0,0);//
				//������Ϊԭ������5 *6.4
				int Nvolume = volume - 32;//
				SendMessage(VHK,TBM_SETPOS,(WPARAM)TRUE,Nvolume);
				Voice(hWnd);//
			}//
			//Alt+Shift+D�ȼ�
			if (HK_D == wParam)
			{
				//��������5
				HWND VHK = GetDlgItem(hWnd,IDC_SOUND_SLIDER);
				//�õ�ԭ����
				int volume = (int)SendMessage(VHK,TBM_GETPOS,0,0);//
				//������Ϊԭ������5 *6.4
				int Nvolume = volume + 32;
				SendMessage(VHK,TBM_SETPOS,(WPARAM)TRUE,Nvolume);
				Voice(hWnd);//	
			}//
			//Alt+Shift+E�ȼ�
			if (HK_E == wParam)
			{
				//ֹͣ
				Stop(hWnd);//	
			}//
			//Alt+Shift+R�ȼ�
			if (HK_R == wParam)
			{
				//�˳�����
				Main_OnClose(hWnd);//	
			}//
			//Alt+Shift+G�ȼ�
			if (HK_G == wParam)
			{
				//����
				onoff = Sof(hWnd,onoff,min,minhwnd);//	
			}//
			//Alt+Shift+B�ȼ�
			if (HK_B == wParam)
			{
				//����ʴ���δ��
				if (!lrc)
				{
					lrc = TRUE;
					//������ʴ���
					DialogBoxParam((HINSTANCE)GetWindowLong(hWnd,-6),MAKEINTRESOURCE(IDD_LRC),NULL,(DLGPROC)Lrc_Proc,(LPARAM)hWnd);//
				}
				//����ʴ����Ѵ�
				else
				{
					//���ʴ��ڷ�����Ϣ��ʹ������ʾ
					PostMessage(lrchwnd,LRC_OPENED,0,0);//
				}//
	
			}//
			//Alt+Shift+F�ȼ�
			if (HK_F == wParam)
			{
				//����
				Up(hWnd);//
			}//
			//Alt+Shift+V�ȼ�
			if (HK_V == wParam)
			{
				//����
				Down(hWnd);//
			}//
		}
		break;//

		//��ʴ��ڹر���Ϣ����
		case LRC_CLOSE:
		{
			//�ı��ʴ�״̬����
			lrc = FALSE;//
			//��ʴ��ھ����Ϊ��
			lrchwnd = NULL;//
		}
		break;//

		//��ʴ��ڴ���Ϣ����
		case LRC_OPEN:
		{
			//��ȡ��ʴ��ھ��
			lrchwnd = (HWND)lParam;//
		}
		break;//

		//�������ڴ���Ϣ����
		case SEA_OPEN:
		{
			//��ȡ�������ھ��
			seahwnd = (HWND)lParam;//
		}
		break;//

		//���ô��ڴ���Ϣ����
		case SET_OPEN:
		{
			//��ȡ���ô��ھ��
			sethwnd = (HWND)lParam;//
		}
		break;//

		//�������ڹر���Ϣ����
		case SEA_CLOSE:
		{
			//�ı�������״̬����
			sea = FALSE;//
			//�������ھ����Ϊ��
			seahwnd = NULL;//
		}
		break;//

		//���ô��ڹر���Ϣ����
		case SET_CLOSE:
		{
			//�ı����ô�״̬����
			set = FALSE;//
			//���ô��ھ����Ϊ��
			sethwnd = NULL;//
		}
		break;//

		//��ʸı���Ϣ����
		case LRC_CHANGE:
		{
			//����ѡ�и��ʱ�����Ӧλ�ò���
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

		//���㴰�ڴ���Ϣ����
		case MINI_OPEN:
		{
			//��ȡ��ʴ��ھ��
			minhwnd = (HWND)lParam;//
		}
		break;//

		//���㴰�ڹر���Ϣ����
		case MINI_CLOSE:
		{
			//�ı������״̬����
			min = FALSE;//
			//���㴰�ھ����Ϊ��
			minhwnd = NULL;//
			//�ָ�������ʾ
			ShowWindow(hWnd, SW_NORMAL);//
			//�����
			SetForegroundWindow(hWnd);
			SetWindowPos(hWnd,HWND_NOTOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);//
		}
		break;//

		//���㴰�ڿ�����Ϣ����
		case MINI_CONTROL:
		{
			switch(wParam)
			{
				//������Ϣ
				case MINI_PLAY:
				{
					//��ʼ���������
					PlayorContinue(hWnd);//
				}
				break;//

				//��ͣ��Ϣ
				case MINI_PAUSE:
				{
					//��ͣ���ڲ��Ÿ���
					Pause(hWnd);//
				}
				break;//

				//ֹͣ��Ϣ
				case MINI_STOP:
				{
					//ֹͣ���ڲ��Ÿ���
					Stop(hWnd);//
				}
				break;//

				//��һ����Ϣ
				case MINI_LAST:
				{
					//ֹͣ���ڲ��Ÿ����������б���ѡ�и�������һ�ף���δѡ���򲥷����һ�׸���
					Last(hWnd);
					Stop(hWnd);
					Play(hWnd);//
				}
				break;//

				//��һ����Ϣ
				case MINI_NEXT:
				{
					//ֹͣ���ڲ��Ÿ����������б���ѡ�и�������һ�ף���δѡ���򲥷ŵ�һ�׸���
					Next(hWnd);
					Stop(hWnd);
					Play(hWnd);//
				}
				break;//

				//������Ϣ
				case MINI_SOF:
				{
					//����ǰδ��������������ǰΪ������ȡ������
					onoff = Sof(hWnd,onoff,min,minhwnd);//
				}
				break;//

				//������������Ϣ
				case MINI_SOUND_SLIDER:
				{
					SendMessage(GetDlgItem(hWnd,IDC_SOUND_SLIDER),TBM_SETPOS,(unsigned long)TRUE,lParam);//
					Voice(hWnd);
				}
				break;//

				//���ȹ�������Ϣ
				case MINI_MUSIC_SLIDER:
				{
					SendMessage(GetDlgItem(hWnd,IDC_MUSIC_SLIDER),TBM_SETPOS,(unsigned long)TRUE,lParam);//
					NTime(hWnd,SB_ENDSCROLL);
				}
				break;//

			}
		}
		break;//

		//��ʴ��ڹر���Ϣ����
		case MINILRC_CLOSE:
		{
			//�ı��ʴ�״̬����
			mil = FALSE;//
			//��ʴ��ھ����Ϊ��
			milhwnd = NULL;//
		}
		break;//

		//��ʴ��ڴ���Ϣ����
		case MINILRC_OPEN:
		{
			//��ȡ��ʴ��ھ��
			milhwnd = (HWND)lParam;//
		}
		break;//

	}

	return FALSE;
}//

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//��ʼ�����ں���
BOOL Main_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	//����͸�����
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
	//��������ļ�
	DragAcceptFiles(hwnd,TRUE);//
	//��ʼ�������������������ţ�
	srand((unsigned)time(NULL));//
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//ע���ȼ�
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
	//��������ͼ�꣬����������Ϣ
	noti.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	noti.hIcon = (HICON)LoadImage((HINSTANCE)GetWindowLong(hwnd, -6), "ID_ICON.ico",IMAGE_ICON,0,0,LR_LOADFROMFILE);	
	noti.hWnd = hwnd;
	strcpy(noti.szTip,TEXT("X�����ֲ�����"));
	noti.uCallbackMessage = WM_TRAY;
	noti.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP;
	noti.uID = ID_ICON;
	Shell_NotifyIcon(NIM_ADD,&noti);//
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//��ʼ��ʱ��/�б���ʾ
	SetDlgItemText(hwnd,IDC_EDITTIME,TEXT("00:00/00:00"));
	SetDlgItemText(hwnd,IDC_EDITLIST,TEXT("000/000"));
	GetLocalTime(&STime);
	TCHAR time[6];
	wsprintf(time,"%02d:%02d",STime.wHour,STime.wMinute);
	SetDlgItemText(hwnd,IDC_EDITSTIME,time);//
	//����ϵͳʱ���ȡ��ʱ��
	SetTimer(hwnd,TIMER_STIME,TIMER_STIME_TIME,(TIMERPROC)STimeF);//
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//��ʼ����Ͽ���ʾ
	//����˳����Ͽ��ʼ��
	HWND CBSX = GetDlgItem(hwnd,IDC_COMBOSX);
	ComboBox_InsertString(CBSX,-1,TEXT("����ѭ��"));
	ComboBox_InsertString(CBSX,-1,TEXT("�б�ѭ��"));
	ComboBox_InsertString(CBSX,-1,TEXT("�������"));
	ComboBox_SetCurSel(CBSX,RANDOM);//

	//�����б���Ͽ��ʼ��
	HWND CSL = GetDlgItem(hwnd,IDC_CSL);
	ComboBox_InsertString(CSL,-1,TEXT("�б�1"));
	ComboBox_InsertString(CSL,-1,TEXT("�б�2"));
	ComboBox_InsertString(CSL,-1,TEXT("�б�3"));
	ComboBox_InsertString(CSL,-1,TEXT("�б�4"));
	ComboBox_SetCurSel(CSL,0);//

	//���б���Ͽ��ʼ��
	HWND COL = GetDlgItem(hwnd,IDC_COL);
	ComboBox_InsertString(COL,-1,TEXT("�б�1"));
	ComboBox_InsertString(COL,-1,TEXT("�б�2"));
	ComboBox_InsertString(COL,-1,TEXT("�б�3"));
	ComboBox_InsertString(COL,-1,TEXT("�б�4"));
	ComboBox_SetCurSel(COL,0);////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//������������ʼ��
	HWND VHK = GetDlgItem(hwnd,IDC_SOUND_SLIDER);
	//���ù�������СֵΪ0
	SendMessage(VHK,TBM_SETRANGEMIN,(WPARAM)TRUE,0);//
	//���ù��������ֵΪ640 /6.4
	SendMessage(VHK,TBM_SETRANGEMAX,(WPARAM)TRUE,640);//
	//���ó�ʼλ��Ϊ384	/6.4						   
	SendMessage(VHK,TBM_SETPOS,(WPARAM)TRUE,384);//
	//���ð�����ʱ����С32 /6.4
	SendMessage(VHK,TBM_SETPAGESIZE,0, (LPARAM)32);//
	//���ð�����ʱ������32 /6.4
	SendMessage(VHK,TBM_SETLINESIZE,0, (LPARAM)32);//
	SetDlgItemText(hwnd,IDC_EDITVOICE,TEXT(" 60"));//
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 	//���Ž��ȹ�������ʼ��
	HWND MHK = GetDlgItem(hwnd,IDC_MUSIC_SLIDER);
	//���ù�������СֵΪ0
	SendMessage(MHK,TBM_SETRANGEMIN,(WPARAM)TRUE,0);//
	//���ù��������ֵΪ1000
	SendMessage(MHK,TBM_SETRANGEMAX,(WPARAM)TRUE,0);//
	//���ó�ʼλ��Ϊ0
	SendMessage(MHK,TBM_SETPOS,(WPARAM)TRUE,0);//
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//�����ļ�����򿪣������ļ�
	if (strcmp("",pfo))
	{
		TCHAR of[MAX_PATH];
		//���ɶ���ļ������
		if('"' == pfo[0])
		{
			int m = 1;
			//��"�ضϣ����ļ�Ŀ¼���Ƴ���
			while('"' != pfo[m])
			{
				of[m-1] = pfo[m];
				++m;
			}
			of[m-1] = '\0';//
			HWND MList = GetDlgItem(hwnd,IDC_MList);
			//����ļ�
			ListBox_InsertString(MList,-1,of);
			//���б�����ͬ���ļ���ɾ�����ļ�
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
		//���ɵ����ļ������
		else
		{
			HWND MList = GetDlgItem(hwnd,IDC_MList);
			ListBox_InsertString(MList,-1,pfo);
			//���б�����ͬ���ļ���ɾ�����ļ�
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
		//���¼����б�����
		ListTotal(hwnd);
		ListNow(hwnd);//
	}//
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	return TRUE;
}//

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//�ؼ���Ϣ��Ӧ����
void Main_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	HWND MList = GetDlgItem(hwnd,IDC_MList);
	switch(id)
	{
		//������ť��Ӧ
		case ID_traLAST: case ID_menLAST: case IDC_btnLAST:
		{
			//ֹͣ���ڲ��Ÿ����������б���ѡ�и�������һ�ף���δѡ���򲥷����һ�׸���
			Last(hwnd);
			Stop(hwnd);
			Play(hwnd);//
		}
		break;//

		//������ť��Ӧ
		case ID_traNEXT: case ID_menNEXT: case IDC_btnNEXT:
		{
			//ֹͣ���ڲ��Ÿ����������б���ѡ�и�������һ�ף���δѡ���򲥷ŵ�һ�׸���
			Next(hwnd);
			Stop(hwnd);
			Play(hwnd);//
		}
		break;//
		
		//�ڸ����б���˫����Ӧ
		case IDC_MList:
		{
			if(LBN_DBLCLK==codeNotify)
			{
				//ֹͣ���ڲ��Ÿ���������ѡ�и���
				Stop(hwnd);
				Play(hwnd);//
			}
		}
		break;//
		
		//����/������ť��Ӧ
		case ID_traPLAY: case ID_menPLAY: case IDC_btnPLAY:
		{
			//��ʼ���������
			PlayorContinue(hwnd);//
		}
		break;//


		//ֹͣ��ť��Ӧ
		case ID_traSTOP: case ID_menSTOP: case IDC_btnSTOP:
		{
			//ֹͣ���ڲ��Ÿ���
			Stop(hwnd);//
		}
		break;//

		//��ͣ��ť��Ӧ
		case ID_traPAUSE: case ID_menPAUSE: case IDC_btnPAUSE:
		{
			//��ͣ���ڲ��Ÿ���
			Pause(hwnd);//
		}
		break;//
		
		//��Ӱ�ť��Ӧ
		case IDC_btnADD: case ID_menADD:
		{
			//����Ӹ�������
			openf(hwnd);//
			//���¼����б�����
			ListTotal(hwnd);
			ListNow(hwnd);//
		}
		break;//

		//ɾ����ť��Ӧ
		case ID_menDEL: case IDC_btnDEL:
		{
			//ɾ��ѡ���ļ�
			Del(hwnd);//
			//���¼����б�����
			ListTotal(hwnd);
			ListNow(hwnd);//
			
		}
		break;//

		//���ư�ť��Ӧ
		case IDC_btnUP:
		{
			//��ѡ���ļ�����
			Up(hwnd);//
		}
		break;//

		//���ư�ť��Ӧ
		case IDC_btnDOWN:
		{
			//��ѡ���ļ�����
			Down(hwnd);//
		}
		break;//

		//������ť��Ӧ
		case IDC_SOF: case ID_menSOF: case ID_traSOF:
		{
			//����ǰδ��������������ǰΪ������ȡ������
			onoff = Sof(hwnd,onoff,min,minhwnd);//
		}
		break;//

		//�˳���ť��Ӧ
		case ID_traEXIT: case ID_menEXIT:
		{
			//�����˳�����
			Main_OnClose(hwnd);//
		}
		break;//
		
		//����˳��ѡ�е���ѭ��ѡ����Ӧ
		case ID_traSELFLOOP: case ID_menSELFLOOP:
		{
			//������˳����Ͽ���Ϊ����ѭ��ѡ��
			ComboBox_SetCurSel(GetDlgItem(hwnd,IDC_COMBOSX),SELFLOOP);//
		}
		break;//

		//����˳��ѡ���б�ѭ��ѡ����Ӧ
		case ID_traLISTLOOP: case ID_menLISTLOOP:
		{
			ComboBox_SetCurSel(GetDlgItem(hwnd,IDC_COMBOSX),LISTLOOP);//
		}
		break;//

		//����˳��ѡ���������ѡ����Ӧ
		case ID_traRANDOM: case ID_menRANDOM:
		{
			//������˳����Ͽ���Ϊ�������ѡ��
			ComboBox_SetCurSel(GetDlgItem(hwnd,IDC_COMBOSX),RANDOM);//
		}
		break;//

		//��հ�ť��Ӧ
		case ID_menCLEAN: case IDC_btnCLEAN:
		{
			//����ȷ�϶Ի�����ȷ��������б�
			int yn = MessageBox(hwnd,TEXT("ȷ��Ҫ����б���"),TEXT("����б�"),MB_YESNO|MB_ICONQUESTION);
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

		//���ش��ڰ�ť��Ӧ
		case ID_menHIDE: case IDC_btnHIDE:
		{ 
			//���ش���
			ShowWindow(hwnd, SW_HIDE);//
		}
		break;//

		//���ڰ�ť��Ӧ
		case ID_menABOUT:
		{
			//�������ڶԻ���
			MessageBox(hwnd,TEXT("X�����ֲ�����V7.0\n��л����֧��!\n����:������\nQQ:834489218"),TEXT("����"),MB_OK);//
			//�ʵ�
			cd++;
			if (3 == cd)
				MessageBox(hwnd,TEXT("      ***             ***\n  **      **       **      **\n*              *              *\n*                             *\n   **                     **\n      **               **\n          **       **\n             *   *\n               *"),TEXT("��л"),MB_OK);//
		}
		break;//
		
		//�ָ����ڰ�ť��Ӧ
		case ID_traRESUME:
		{
			//�ָ�������ʾ
			ShowWindow(hwnd, SW_NORMAL);//
			//�����
			SetForegroundWindow(hwnd);//
		}
		break;//

		//���㰴ť��Ӧ
		case IDC_btnMINI:
		{
			//�����㴰��δ��
			if (!min)
			{
				min = TRUE;
				//���ش���
				ShowWindow(hwnd, SW_HIDE);//
				//�������㴰��
				DialogBoxParam((HINSTANCE)GetWindowLong(hwnd,-6),MAKEINTRESOURCE(IDD_MINI),NULL,(DLGPROC)Mini_Proc,(LPARAM)hwnd);//
			}
			//�����㴰���Ѵ�
			else
			{
				//���ش���
				ShowWindow(hwnd, SW_HIDE);//
				//�����㴰�ڷ�����Ϣ��ʹ������ʾ
				PostMessage(minhwnd,MINI_OPENED,0,0);//
			}//
		}
		break;//

		//���б�ť��Ӧ
		case ID_menOLIST: case IDC_btnOLIST:
		{
			//����ȷ�϶Ի�����ȷ�����ѡ���б�
			openList(hwnd);//
		}
		break;//

		//�����б�ť��Ӧ
		case ID_menSLIST: case IDC_btnSLIST:
		{
			//����ȷ�϶Ի�����ȷ���򱣴���ѡ���б�
			saveList(hwnd);//
		}
		break;//

		//�������߰�ť��Ӧ
		case ID_menVUP: case ID_traVUP:
		{
			HWND VHK = GetDlgItem(hwnd,IDC_SOUND_SLIDER);
			//��������5 *6.4
			int volume = (int)SendMessage(VHK,TBM_GETPOS,0,0);
			int Nvolume = volume + 32;
			SendMessage(VHK,TBM_SETPOS,(WPARAM)TRUE,Nvolume);
			Voice(hwnd);//
		}
		break;//

		//�������Ͱ�ť��Ӧ
		case ID_menVDOWN: case ID_traVDOWN:
		{
			HWND VHK = GetDlgItem(hwnd,IDC_SOUND_SLIDER);
			//��������5 *6.4
			int volume = (int)SendMessage(VHK,TBM_GETPOS,0,0);
			int Nvolume = volume - 32;
			SendMessage(VHK,TBM_SETPOS,(WPARAM)TRUE,Nvolume);
			Voice(hwnd);//
		}
		break;//

		//����������ť��Ӧ
		case IDC_btnSEARCH: case ID_traSEARCH:
		{
			//��������������δ��
			if (!sea)
			{
				sea = TRUE;
				//����������������
				DialogBoxParam((HINSTANCE)GetWindowLong(hwnd,-6),MAKEINTRESOURCE(IDD_SEARCH),NULL,(DLGPROC)Search_Proc,(LPARAM)hwnd);//
			}
			//���������������Ѵ�
			else
			{
				//�������������ڷ�����Ϣ��ʹ������ʾ
				PostMessage(seahwnd,SEA_OPENED,0,0);//
			}//
		}
		break;//

		//���ð�ť��Ӧ
		case ID_menSET: case ID_traSET:
		{
			//�����ô���δ��
			if (!set)
			{
				set = TRUE;
				//�������ô���
				DialogBoxParam((HINSTANCE)GetWindowLong(hwnd,-6),MAKEINTRESOURCE(IDD_SET),hwnd,(DLGPROC)Set_Proc,(LPARAM)hwnd);//
			}
		}
		break;//

		//�򿪸�ʰ�ť��Ӧ
		case IDC_btnLRC: case ID_menLRC: case ID_traLRC:
		{
			//����ʴ���δ��
			if (!lrc)
			{
				lrc = TRUE;
				//������ʴ���
				DialogBoxParam((HINSTANCE)GetWindowLong(hwnd,-6),MAKEINTRESOURCE(IDD_LRC),NULL,(DLGPROC)Lrc_Proc,(LPARAM)hwnd);//
			}
			//����ʴ����Ѵ�
			else
			{
				//���ʴ��ڷ�����Ϣ��ʹ������ʾ
				PostMessage(lrchwnd,LRC_OPENED,0,0);//
			}//
		}
		break;//

		//˯��ģʽ��ť��Ӧ
		case IDC_btnSLEEP:
		{
			HWND SleepBTN = GetDlgItem(hwnd,IDC_btnSLEEP);
			//��δ����˯��ģʽ��ȷ���Ƿ����
			if (!sleep)
			{
				//����ȷ�϶Ի�����ȷ�������˯��ģʽ
				int yn = MessageBox(hwnd,TEXT("��Ҫ�����б���"),TEXT("�˳�"),MB_YESNO|MB_ICONQUESTION);
				if (IDYES == yn)
					return;
				int i = MessageBox(hwnd,TEXT("ȷ��Ҫ����˯��ģʽ��"),TEXT("˯��ģʽ"),MB_YESNO|MB_ICONQUESTION);
				if (IDYES == i)
				{
					//��ȡ�����ļ������õ�˯�ߵȴ�ʱ��
					int st = SleepTime();//
					TCHAR temp[256];
					//��˯�ߵȴ�ʱ���ػ�
					wsprintf(temp,"shutdown -s -t %d",st+15);
					system(temp);//
					HWND COL = GetDlgItem(hwnd,IDC_COMBOSX);
					//���ò���˳��Ϊ�б�ѭ��
					ComboBox_SetCurSel(COL,LISTLOOP);//
					wsprintf(temp,"�ѽ���˯��ģʽ�����������%d���Ӻ�رգ�",st/60);
					MessageBox(hwnd,TEXT(temp),TEXT("˯��ģʽ"),MB_OK|MB_ICONINFORMATION);
					//����ť�������
					Button_SetText(SleepBTN,TEXT("�˳�˯��ģʽ"));//
					//��˯��ģʽ��������Ϊ����˯��ģʽ
					sleep = TRUE;//
				}
				else
					return;
				//
			}//
			//���ѽ���˯��ģʽ��ȷ���Ƿ��˳�
			else
			{
				//����ȷ�϶Ի�����ȷ�����˳�˯��ģʽ
				int i = MessageBox(hwnd,TEXT("ȷ��Ҫ�˳�˯��ģʽ��"),TEXT("˯��ģʽ"),MB_YESNO|MB_ICONQUESTION);
				if (IDYES == i)
				{	
					//ȡ���Զ��ػ�
					system("shutdown -a");
					HWND COL = GetDlgItem(hwnd,IDC_COMBOSX);
					//���ò���˳��Ϊ�������
					ComboBox_SetCurSel(COL,RANDOM);//
					MessageBox(hwnd,TEXT("���˳�˯��ģʽ��"),TEXT("˯��ģʽ"),MB_OK|MB_ICONINFORMATION);
					//����ť�������
					Button_SetText(SleepBTN,TEXT("˯��ģʽ(&J)"));
					//��˯��ģʽ��������Ϊδ����˯��ģʽ
					sleep = FALSE;
				}
				else
					return;
			}
		}
		break;//

		case IDC_btnMINILRC:
		{
			//����ʴ���δ��
			if (!mil)
			{
				mil = TRUE;
				Button_SetText(GetDlgItem(hwnd,IDC_btnMINILRC),TEXT("�ر�������"));
				//������ʴ���
				DialogBoxParam((HINSTANCE)GetWindowLong(hwnd,-6),MAKEINTRESOURCE(IDD_MINILRC),NULL,(DLGPROC)MiniLrc_Proc,(LPARAM)hwnd);//
			}
			//����ʴ����Ѵ�
			else
			{
				mil = FALSE;
				Button_SetText(GetDlgItem(hwnd,IDC_btnMINILRC),TEXT("������(&P)"));
				//���ʴ��ڷ�����Ϣ��ʹ���ڹر�
				PostMessage(milhwnd,PRO_CLOSE,0,0);//
			}//
		}
		break;

		default:
		break;
	}
}//

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//��������Ϣ��Ӧ����
void Main_OnHScroll(HWND hwnd, HWND hwndCtl, UINT code, int pos)
{
	switch(GetDlgCtrlID(hwndCtl))
	{
		//������������Ϣ��Ӧ
		case IDC_SOUND_SLIDER:
		{
			//��������������λ�ø�������
			Voice(hwnd);//
		}
		break;//

		//���Ž��ȹ�������Ϣ��Ӧ
		case IDC_MUSIC_SLIDER:
		{
			//���ݽ��ȹ�����λ�ô���Ӧλ�ò���
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

//�˳�����
void Main_OnClose(HWND hwnd)
{
	if (IDYES == MessageBox(hwnd,TEXT("��Ҫ�����б���"),TEXT("�˳�"),MB_YESNO|MB_ICONQUESTION))
		return;
	int i = MessageBox(hwnd,TEXT("ȷ��Ҫ�˳���"),TEXT("�˳�"),MB_YESNO|MB_ICONQUESTION);
	if (IDYES == i)
	{
		//ȡ������ͼ��
		Shell_NotifyIcon(NIM_DELETE,&noti);//
		Stop(hwnd);
		//�����Ӵ��ڣ����͹ر���Ϣ
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