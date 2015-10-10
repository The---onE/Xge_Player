/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��ʱ���ص���������
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef timer_h
#define timer_h

//����ʱ�䶨ʱ���ص�����
void CALLBACK Change(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime)
{
	TCHAR st[256],Choose[MAX_PATH],ShortName[MAX_PATH],play[MAX_PATH+10];
	GetDlgItemText(hwnd,IDC_EDITNOW,Choose,sizeof(Choose));
	GetShortPathName(Choose,ShortName,sizeof(ShortName));
	wsprintf(play,"status %s mode",ShortName);
	//�������ڲ��Ÿ���״̬
	mciSendString(play, st, sizeof(st), NULL);//
	//�����ڲ��Ÿ����Ѳ���
	if(!strcmp("stopped",st))
	{
		HWND CBSX = GetDlgItem(hwnd,IDC_COMBOSX);
		//���ݲ���˳����Ͽ�ѡ��ѡ��
		int i = ComboBox_GetCurSel(CBSX);
		switch (i)
		{
			//��Ϊ�б�ѭ��
			case LISTLOOP:
			{
				//�������ڲ��Ÿ�����һ��
				Next(hwnd);
				Play(hwnd);//
			}
			break;//
			
			//��Ϊ����ѭ��
			case SELFLOOP:
			{
				//���²������ڲ��Ÿ���
				MSelf(hwnd);//
			}
			break;
			
			//��Ϊ�������
			case RANDOM:
			{
				//����������ź������ȷ�������Ÿ���
				Rand(hwnd);//  
				int i = ListBox_GetCurSel(GetDlgItem(hwnd,IDC_MList));
				TCHAR Choose[MAX_PATH];
				ListBox_GetText(GetDlgItem(hwnd,IDC_MList),i,Choose);
				TCHAR Now[MAX_PATH];
				GetDlgItemText(hwnd,IDC_EDITNOW,Now,sizeof(Now));
				//����Ҫ���Ÿ��������ڲ��Ÿ�����ͬ���򲥷���һ��
				if (!strcmp(Choose,Now))
				{
					Next(hwnd);
				}
				Play(hwnd);
			}
		}//
	}//
	//�����ڲ��Ÿ���δ����
	else
	{
		TimeNGet(ShortName,hwnd);
		wsprintf(Time,"%02d:%02d/%02d:%02d",Nmin,Nsec,Tmin,Tsec);
		//���ò���ʱ�䵽�༭��
		SetDlgItemText(hwnd,IDC_EDITTIME,Time);//
		//����ʴ��ڴ򿪣����ʴ��ڷ�����Ϣ���¸��ʱ��
		if (TRUE == lrc)
			PostMessage(lrchwnd,LRC_TIME,Nmin,Nsec);//
		//�������ʴ��ڴ򿪣��������ʴ��ڷ�����Ϣ���¸��ʱ��
		if (TRUE == mil)
			PostMessage(milhwnd,MINILRC_TIME,Nmin,Nsec);//
		unsigned long NT = TimeNGet(ShortName,hwnd);
		//�ƶ�����ʱ�����������Ӧλ��
		SendMessage(GetDlgItem(hwnd,IDC_MUSIC_SLIDER),TBM_SETPOS,(unsigned long)TRUE,NT);//
	}//
}

//��ȡϵͳʱ�䶨ʱ���ص�����
void CALLBACK STimeF(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime)
{
	//��ȡϵͳʱ�����ṹ��
	GetLocalTime(&STime);//
	TCHAR time[6];
	wsprintf(time,"%02d:%02d",STime.wHour,STime.wMinute);
	//����ϵͳʱ�䵽�༭��
	SetDlgItemText(hwnd,IDC_EDITSTIME,time);//
}//

#endif