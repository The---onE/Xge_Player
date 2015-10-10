/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//����ȫ�������ں�������
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//ȫ�ֱ�������
#include "all.h"//
//�б����ļ�����������
#include "file.h"//
//��ȡMP3�ļ�����Ϣ��������
#include "information.h"//
//�����б�������ȡ��������
#include "list.h"//
//�����ڶ�ȡ�����ļ���������
#include "mainset.h"//
//����˳��������
#include "order.h"//
//����ʱ�亯������
#include "time_h.h"//
//��ʱ���ص���������
#include "timer.h"//

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//���ſ��ƣ����š���ͣ��ֹͣ�ȣ���������
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef control_h
#define control_h

//���ź���
void Play(HWND hwnd)
{
	//���ٲ���ʱ�䶨ʱ�����Ա��������ö�ʱ��
	KillTimer(hwnd,TIMER_PLAY); //
	HWND MList = GetDlgItem(hwnd,IDC_MList);
	int i = ListBox_GetCurSel(MList);
	TCHAR Choose[MAX_PATH];
	//����ѡ���ļ�
	if(LB_ERR != i)
	{
		ListBox_GetText(MList,i,Choose);
		TCHAR ShortName[MAX_PATH],play[MAX_PATH+10];
		GetShortPathName(Choose,ShortName,sizeof(ShortName));
		wsprintf(play,"play %s",ShortName);
		//���Ÿ�������ȡ������Ϣ
		int info = mciSendString(play,"",0,NULL);
		//������ʧ�ܣ��򲥷���һ�ײ�ɾ�������ļ�
		if (info)
		{
			HWND MList = GetDlgItem(hwnd,IDC_MList);
			int j = ListBox_GetCurSel(MList);
			//ɾ�������ļ�
			ListBox_DeleteString(MList,j);//
			int t = ListBox_GetCount(MList);
			//��λ����һ��
			if (t == j)
				ListBox_SetCurSel(MList,0);
			else
				ListBox_SetCurSel(MList,j);//
			//�ݹ����µ��ò��ź���
			Play(hwnd);//
			//�������ļ������򲻼���ִ��
			return;//
		}//
		//�����ųɹ������ļ���д�����ڲ��ű༭��
		SetDlgItemText(hwnd,IDC_EDITNOW,Choose);//
		//���ò���ʱ�䶨ʱ��
		SetTimer(hwnd,TIMER_PLAY,TIMER_PLAY_TIME,(TIMERPROC)Change);//
		TimeTGet(ShortName,hwnd);
		wsprintf(Time,"00:00/%02d:%02d",Tmin,Tsec);
		//ˢ�²���ʱ��
		SetDlgItemText(hwnd,IDC_EDITTIME,Time);//
		//��������
		NVoice(hwnd);
		Voice(hwnd);//
		//��ʼ������ʱ��
		beginTimeSet(ShortName,hwnd);//
		//���¼����б�����
		ListNow(hwnd);//
		//��ȡ������Ϣ
		fID3V1(hwnd);
	}//
	else
		return;
}//

void PlayorContinue(HWND hwnd)
{
	TCHAR Choose1[MAX_PATH];
	GetDlgItemText(hwnd,IDC_EDITNOW,Choose1,sizeof(Choose1));
	HWND MList = GetDlgItem(hwnd,IDC_MList);
	int i = ListBox_GetCurSel(MList);
	TCHAR Choose2[MAX_PATH];
	ZeroMemory(&Choose2,sizeof(Choose2));
	ListBox_GetText(MList,i,Choose2);
	//��ѡ�и��������ڲ��Ÿ�����ͬ���������
	if (!strcmp(Choose1,Choose2))
	{	
		TCHAR * ShortName = GSNTE(hwnd);
		TCHAR play[MAX_PATH+10];
		wsprintf(play,"play %s",ShortName);
		//���Ÿ���
		mciSendString(play,"",0,NULL);//
		//���ò���ʱ�䶨ʱ��
		SetTimer(hwnd,TIMER_PLAY,TIMER_PLAY_TIME,(TIMERPROC)Change);//
	}//
	//����ѡ�и���������������ڲ��Ÿ���
	else if (-1 == i)
	{
		TCHAR * ShortName = GSNTE(hwnd);
		TCHAR play[MAX_PATH+10];
		wsprintf(play,"play %s",ShortName);
		mciSendString(play,"",0,NULL);
	}//
	//��ѡ�и��������ڲ��Ÿ�����ͬ�򲥷�ѡ�и���
	else
	{
		Stop(hwnd);
		Play(hwnd);
	}//

}

//ֹͣ����
void Stop(HWND hwnd)
{
	TCHAR * ShortName = GSNTE(hwnd);
	TCHAR play[MAX_PATH+10];
	wsprintf(play,"stop %s",ShortName);
	//ֹͣ����
	mciSendString(play,"",0,NULL);//
	//������ڲ��ű༭��
	SetDlgItemText(hwnd,IDC_EDITNOW,"");//
	wsprintf(Time,"00:00/00:00");
	//����ʱ������
	SetDlgItemText(hwnd,IDC_EDITTIME,Time);//
	//���Ž��ȹ���������
	SendMessage(GetDlgItem(hwnd,IDC_MUSIC_SLIDER),TBM_SETPOS,(WPARAM)TRUE,0);//
	//������Ϣ���
	SetDlgItemText(hwnd,IDC_Title,"");
	SetDlgItemText(hwnd,IDC_Artist,"");
	SetDlgItemText(hwnd,IDC_Album,"");//
	//���ٲ���ʱ�䶨ʱ��
	KillTimer(hwnd,TIMER_PLAY);//
}//

//��ͣ����
void Pause(HWND hwnd)
{
	TCHAR * ShortName = GSNTE(hwnd);
	TCHAR play[MAX_PATH+10];
	wsprintf(play,"pause %s",ShortName);
	//��ͣ����
	mciSendString(play,"",0,NULL);//
	//���ٲ���ʱ�䶨ʱ��
	KillTimer(hwnd,TIMER_PLAY);//
}//

//��һ��������ֻѡ�в����ţ�
void Last(HWND hwnd)
{
	HWND MList = GetDlgItem(hwnd,IDC_MList);
	TCHAR Choose[MAX_PATH];
	GetDlgItemText(hwnd,IDC_EDITNOW,Choose,sizeof(Choose));
	//���б��в��������ڲ��ű༭����������ͬ��ѡ��
	int i = ListBox_FindString(MList,0,Choose);
	int max = ListBox_GetCount(MList);
	//��ѡ�и������б��һ�׸�����ѡ����һ�׸���
	if(i-1 >= 0)
		ListBox_SetCurSel(MList,i-1);//
	//��ѡ�и���Ϊ�б��һ�׸�����ѡ�����һ�׸���
	else
		ListBox_SetCurSel(MList,max-1);//
}//

//��һ��������ֻѡ�в����ţ�
void Next(HWND hwnd)
{
	HWND MList = GetDlgItem(hwnd,IDC_MList);
	TCHAR Choose[MAX_PATH];
	GetDlgItemText(hwnd,IDC_EDITNOW,Choose,sizeof(Choose));
	//���б��в��������ڲ��ű༭����������ͬ��ѡ��
	int i = ListBox_FindString(MList,0,Choose);
	int max = ListBox_GetCount(MList);
	//��ѡ�и������б����һ�׸�����ѡ����һ�׸���
	if(i <= max-2)
		ListBox_SetCurSel(MList,i+1);//
	//��ѡ�и���Ϊ�б����һ�׸�����ѡ�е�һ�׸���
	else
		ListBox_SetCurSel(MList,0);
}

//������������
void Voice(HWND hwnd)
{
	//���������������ֵ
	int volume = (int)SendMessage(GetDlgItem(hwnd,IDC_SOUND_SLIDER),TBM_GETPOS,0,0);//
	TCHAR vol[MAX_PATH];
	//�������������ֵΪ640������Ϊ100
	_itoa_s((int)(volume * 0.15625),vol,10);
	SetDlgItemText(hwnd,IDC_EDITVOICE,vol);
	TCHAR * ShortName = GSNTE(hwnd);
	TCHAR szVolume[MAX_PATH+20];
	wsprintf(szVolume,"setaudio %s volume to %d",ShortName,volume);
	//��������
    mciSendString(szVolume,"",0,NULL);//
}//

//��������
BOOL Sof(HWND hwnd,int onoff,BOOL min,HWND minhwnd)
{
	TCHAR * ShortName = GSNTE(hwnd);
	TCHAR play[MAX_PATH+10];
	//��δ����
	if(onoff)
	{
		wsprintf(play,"setaudio %s off",ShortName);
		//���þ���
		mciSendString(play,"", 0, NULL);//
		//���ø�ѡ��Ϊ��Ӧ״̬
		CheckDlgButton(hwnd,IDC_SOF,BST_CHECKED);
		if (min)
			CheckDlgButton(minhwnd,IDC__SOF,BST_CHECKED);
		//�ı侲��״̬����
		onoff = FALSE;//
	}//
	//���Ѿ���
	else
	{
		wsprintf(play,"setaudio %s on",ShortName);
		//ȡ������
		mciSendString(play,"", 0, NULL);//
		//���ø�ѡ��Ϊ��Ӧ״̬
		CheckDlgButton(hwnd,IDC_SOF,BST_UNCHECKED);
		if (min)
			CheckDlgButton(minhwnd,IDC__SOF,BST_UNCHECKED);
		//�ı侲��״̬����
		onoff = TRUE;//
	}//
	return onoff;
}//

//����״̬��⺯��
void NVoice(HWND hwnd)
{
	//��Ϊ����״̬
	if (!onoff)
	{
		TCHAR * ShortName = GSNTE(hwnd);
		TCHAR play[MAX_PATH+10];
		wsprintf(play,"setaudio %s off",ShortName);
		//��������
		mciSendString(play, NULL, 0, NULL);//
	}//
}//

//��ȡ�ļ���·������
TCHAR * GSNTE(HWND hwnd)
{
	TCHAR Choose[MAX_PATH];
	//��ȡ���ڲ��ű༭�����ļ�·��
	GetDlgItemText(hwnd,IDC_EDITNOW,Choose,sizeof(Choose));//
	//�����ļ���·��
	GetShortPathName(Choose,ShortName1,sizeof(ShortName1));
	return ShortName1;//
}

//���ݸ�ʴ���Ӧλ�ò���
void playfrom(HWND hwnd,int Time)
{
	TCHAR Choose[MAX_PATH],STime[20];
	GetDlgItemText(hwnd,IDC_EDITNOW,Choose,sizeof(Choose));
	//�������ڲ��Ÿ���
	if ('\0' != *Choose)
	{
		if (Time < 0)
			Time = 0;
		int stemp = Time/1000 + 1;
		int TTmin = stemp/60;
		int TTsec = stemp%60;//			
		wsprintf(STime,"%02d:%02d/%02d:%02d",TTmin,TTsec,Tmin,Tsec);
		//�ı䲥��ʱ��༭��ʱ��
		SetDlgItemText(hwnd,IDC_EDITTIME,STime);//
		KillTimer(hwnd,TIMER_PLAY);
		TCHAR play[MAX_PATH+10];
		TCHAR * ShortName = GSNTE(hwnd);
		wsprintf(play,"stop %s",ShortName);
		//ֹͣ���ڲ��Ÿ���
		mciSendString(play,"",0,NULL);//
		wsprintf(play,"play %s from %d",ShortName,Time);
		//����Ӧλ�ò������ڲ��Ÿ���
		mciSendString(play,"",0,NULL);//
		NVoice(hwnd);
		Voice(hwnd);
		unsigned long NT = TimeNGet(ShortName,hwnd);
		//�ƶ�����ʱ�����������Ӧλ��
		SendMessage(GetDlgItem(hwnd,IDC_MUSIC_SLIDER),TBM_SETPOS,(unsigned long)TRUE,NT);//
		SetTimer(hwnd,TIMER_PLAY,TIMER_PLAY_TIME,(TIMERPROC)Change);
	}//
}//

#endif