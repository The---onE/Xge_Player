/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//����ʱ�亯������
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef time_h
#define time_h

//��ʼ��ʱ�亯��
void beginTimeSet(TCHAR ShortName[],HWND hwnd)
{
	//�տ�ʼ����ʱ�����ò���ʱ��Ϊ0
	wsprintf(Time,"00:00/%02d:%02d",Tmin,Tsec);
	SetDlgItemText(hwnd,IDC_EDITTIME,Time);//
	//��ʼ�����Ž��ȹ�����
	unsigned long NT = TimeNGet(ShortName,hwnd);
	unsigned long TT = TimeTGet(ShortName,hwnd);
	HWND MHK = GetDlgItem(hwnd,IDC_MUSIC_SLIDER);
	SendMessage(MHK,TBM_SETRANGEMIN,(WPARAM)TRUE,0);
	SendMessage(MHK,TBM_SETRANGEMAX,(unsigned long)TRUE,TT);
	SendMessage(MHK,TBM_SETPOS,(unsigned long)TRUE,NT);
	SendMessage(MHK,TBM_SETPAGESIZE,0, (unsigned long)(TT/40));  
	SendMessage(MHK,TBM_SETLINESIZE,0, (unsigned long)(TT/40));//
	if (min)
	{
		MHK = GetDlgItem(minhwnd,IDC__MUSIC_SLIDER);
		SendMessage(MHK,TBM_SETRANGEMIN,(WPARAM)TRUE,0);
		SendMessage(MHK,TBM_SETRANGEMAX,(unsigned long)TRUE,TT);
		SendMessage(MHK,TBM_SETPOS,(unsigned long)TRUE,NT);
		SendMessage(MHK,TBM_SETPAGESIZE,0, (unsigned long)(TT/40));  
		SendMessage(MHK,TBM_SETLINESIZE,0, (unsigned long)(TT/40));//
	}//
}//

//��ȡ�����ܳ�ʱ�亯��
int TimeTGet(TCHAR ShortName[],HWND hwnd)
{
	TCHAR play1[MAX_PATH+10];
	TCHAR st1[MAX_PATH];
	wsprintf(play1,"status %s length",ShortName);
	//��ȡ�Ժ���Ϊ��λ���ܳ�
	mciSendString(play1,st1,sizeof(st1), NULL);//
	int i = atoi(st1);
	//�����ܳ��֡��ܳ���ȫ�ֱ���
	int stemp = i/1000 + 1;
	Tsec = stemp%60;
	Tmin = stemp/60;//
	//�����Ժ���Ϊ��λ���ܳ�
	return i;//
}//

//��ȡ��ǰ����ʱ�亯��
int TimeNGet(TCHAR ShortName[],HWND hwnd)
{
	TCHAR play1[MAX_PATH+10];
	TCHAR st1[MAX_PATH];
	wsprintf(play1,"status %s position",ShortName);
	//��ȡ�Ժ���Ϊ��λ�ĵ�ǰ����ʱ��
	mciSendString(play1,st1,sizeof(st1), NULL);
	int i = atoi(st1);
	//���õ�ǰ�롢��ǰ��ȫ�ֱ���
	int stemp = i/1000 + 1;
	Nsec = stemp%60;
	Nmin = stemp/60;//
	//�����Ժ���Ϊ��λ���ܳ�
	return i;//
}//

//���ݲ��Ž��ȹ���������Ӧλ�ò��ź���
void NTime(HWND hwnd,int code)
{
	TCHAR Choose[MAX_PATH],STime[20];
	GetDlgItemText(hwnd,IDC_EDITNOW,Choose,sizeof(Choose));
	//�õ����Ž��ȹ�����λ��
	int Time = (int)SendMessage(GetDlgItem(hwnd,IDC_MUSIC_SLIDER),TBM_GETPOS,0,0);//
	//���ݲ��Ž��ȹ�����λ�ü�����Ӧʱ��
	int stemp = Time/1000 + 1;
	int TTmin = stemp/60;
	int TTsec = stemp%60;//			
	wsprintf(STime,"%02d:%02d/%02d:%02d",TTmin,TTsec,Tmin,Tsec);
	//�ı䲥��ʱ��༭��ʱ��
	SetDlgItemText(hwnd,IDC_EDITTIME,STime);//
	//�������ڲ��Ÿ���
	if ('\0' != *Choose)
	{
		KillTimer(hwnd,TIMER_PLAY);
		//��ֹͣ�϶�ʱ
		if (SB_ENDSCROLL==code)
		{
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
			SetTimer(hwnd,TIMER_PLAY,TIMER_PLAY_TIME,(TIMERPROC)Change);
		}//
	}//
}//

#endif