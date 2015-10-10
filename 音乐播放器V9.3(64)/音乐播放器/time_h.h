/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//歌曲时间函数定义
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef time_h
#define time_h

//初始化时间函数
void beginTimeSet(TCHAR ShortName[],HWND hwnd)
{
	//刚开始播放时，设置播放时间为0
	wsprintf(Time,"00:00/%02d:%02d",Tmin,Tsec);
	SetDlgItemText(hwnd,IDC_EDITTIME,Time);//
	//初始化播放进度滚动条
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

//获取歌曲总长时间函数
int TimeTGet(TCHAR ShortName[],HWND hwnd)
{
	TCHAR play1[MAX_PATH+10];
	TCHAR st1[MAX_PATH];
	wsprintf(play1,"status %s length",ShortName);
	//获取以毫秒为单位的总长
	mciSendString(play1,st1,sizeof(st1), NULL);//
	int i = atoi(st1);
	//设置总长分、总长秒全局变量
	int stemp = i/1000 + 1;
	Tsec = stemp%60;
	Tmin = stemp/60;//
	//返回以毫秒为单位的总长
	return i;//
}//

//获取当前播放时间函数
int TimeNGet(TCHAR ShortName[],HWND hwnd)
{
	TCHAR play1[MAX_PATH+10];
	TCHAR st1[MAX_PATH];
	wsprintf(play1,"status %s position",ShortName);
	//获取以毫秒为单位的当前播放时间
	mciSendString(play1,st1,sizeof(st1), NULL);
	int i = atoi(st1);
	//设置当前秒、当前分全局变量
	int stemp = i/1000 + 1;
	Nsec = stemp%60;
	Nmin = stemp/60;//
	//返回以毫秒为单位的总长
	return i;//
}//

//根据播放进度滚动条从相应位置播放函数
void NTime(HWND hwnd,int code)
{
	TCHAR Choose[MAX_PATH],STime[20];
	GetDlgItemText(hwnd,IDC_EDITNOW,Choose,sizeof(Choose));
	//得到播放进度滚动条位置
	int Time = (int)SendMessage(GetDlgItem(hwnd,IDC_MUSIC_SLIDER),TBM_GETPOS,0,0);//
	//根据播放进度滚动条位置计算相应时间
	int stemp = Time/1000 + 1;
	int TTmin = stemp/60;
	int TTsec = stemp%60;//			
	wsprintf(STime,"%02d:%02d/%02d:%02d",TTmin,TTsec,Tmin,Tsec);
	//改变播放时间编辑框时间
	SetDlgItemText(hwnd,IDC_EDITTIME,STime);//
	//若有正在播放歌曲
	if ('\0' != *Choose)
	{
		KillTimer(hwnd,TIMER_PLAY);
		//在停止拖动时
		if (SB_ENDSCROLL==code)
		{
			TCHAR play[MAX_PATH+10];
			TCHAR * ShortName = GSNTE(hwnd);
			wsprintf(play,"stop %s",ShortName);
			//停止正在播放歌曲
			mciSendString(play,"",0,NULL);//
			wsprintf(play,"play %s from %d",ShortName,Time);
			//从相应位置播放正在播放歌曲
			mciSendString(play,"",0,NULL);//
			NVoice(hwnd);
			Voice(hwnd);
			SetTimer(hwnd,TIMER_PLAY,TIMER_PLAY_TIME,(TIMERPROC)Change);
		}//
	}//
}//

#endif