/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//定时器回调函数定义
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef timer_h
#define timer_h

//播放时间定时器回调函数
void CALLBACK Change(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime)
{
	TCHAR st[256],Choose[MAX_PATH],ShortName[MAX_PATH],play[MAX_PATH+10];
	GetDlgItemText(hwnd,IDC_EDITNOW,Choose,sizeof(Choose));
	GetShortPathName(Choose,ShortName,sizeof(ShortName));
	wsprintf(play,"status %s mode",ShortName);
	//或曲正在播放歌曲状态
	mciSendString(play, st, sizeof(st), NULL);//
	//若正在播放歌曲已播完
	if(!strcmp("stopped",st))
	{
		HWND CBSX = GetDlgItem(hwnd,IDC_COMBOSX);
		//根据播放顺序组合框选中选项
		int i = ComboBox_GetCurSel(CBSX);
		switch (i)
		{
			//若为列表循环
			case LISTLOOP:
			{
				//播放正在播放歌曲下一首
				Next(hwnd);
				Play(hwnd);//
			}
			break;//
			
			//若为单曲循环
			case SELFLOOP:
			{
				//重新播放正在播放歌曲
				MSelf(hwnd);//
			}
			break;
			
			//若为随机播放
			case RANDOM:
			{
				//调用随机播放函数随机确定将播放歌曲
				Rand(hwnd);//  
				int i = ListBox_GetCurSel(GetDlgItem(hwnd,IDC_MList));
				TCHAR Choose[MAX_PATH];
				ListBox_GetText(GetDlgItem(hwnd,IDC_MList),i,Choose);
				TCHAR Now[MAX_PATH];
				GetDlgItemText(hwnd,IDC_EDITNOW,Now,sizeof(Now));
				//若将要播放歌曲与正在播放歌曲相同，则播放下一首
				if (!strcmp(Choose,Now))
				{
					Next(hwnd);
				}
				Play(hwnd);
			}
		}//
	}//
	//若正在播放歌曲未播完
	else
	{
		TimeNGet(ShortName,hwnd);
		wsprintf(Time,"%02d:%02d/%02d:%02d",Nmin,Nsec,Tmin,Tsec);
		//设置播放时间到编辑框
		SetDlgItemText(hwnd,IDC_EDITTIME,Time);//
		//若歌词窗口打开，向歌词窗口发送消息更新歌词时间
		if (TRUE == lrc)
			PostMessage(lrchwnd,LRC_TIME,Nmin,Nsec);//
		//若迷你歌词窗口打开，向迷你歌词窗口发送消息更新歌词时间
		if (TRUE == mil)
			PostMessage(milhwnd,MINILRC_TIME,Nmin,Nsec);//
		unsigned long NT = TimeNGet(ShortName,hwnd);
		//移动播放时间滚动条到相应位置
		SendMessage(GetDlgItem(hwnd,IDC_MUSIC_SLIDER),TBM_SETPOS,(unsigned long)TRUE,NT);//
	}//
}

//获取系统时间定时器回调函数
void CALLBACK STimeF(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime)
{
	//获取系统时间存入结构体
	GetLocalTime(&STime);//
	TCHAR time[6];
	wsprintf(time,"%02d:%02d",STime.wHour,STime.wMinute);
	//设置系统时间到编辑框
	SetDlgItemText(hwnd,IDC_EDITSTIME,time);//
}//

#endif