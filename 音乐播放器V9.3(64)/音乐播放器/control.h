/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//包含全部主窗口函数定义
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//全局变量定义
#include "all.h"//
//列表中文件管理函数定义
#include "file.h"//
//读取MP3文件中信息函数定义
#include "information.h"//
//播放列表总数读取函数定义
#include "list.h"//
//主窗口读取设置文件函数定义
#include "mainset.h"//
//播放顺序函数定义
#include "order.h"//
//歌曲时间函数定义
#include "time_h.h"//
//定时器回调函数定义
#include "timer.h"//

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//播放控制（播放、暂停、停止等）函数定义
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef control_h
#define control_h

//播放函数
void Play(HWND hwnd)
{
	//销毁播放时间定时器，以便重新设置定时器
	KillTimer(hwnd,TIMER_PLAY); //
	HWND MList = GetDlgItem(hwnd,IDC_MList);
	int i = ListBox_GetCurSel(MList);
	TCHAR Choose[MAX_PATH];
	//若有选中文件
	if(LB_ERR != i)
	{
		ListBox_GetText(MList,i,Choose);
		TCHAR ShortName[MAX_PATH],play[MAX_PATH+10];
		GetShortPathName(Choose,ShortName,sizeof(ShortName));
		wsprintf(play,"play %s",ShortName);
		//播放歌曲并获取错误信息
		int info = mciSendString(play,"",0,NULL);
		//若播放失败，则播放下一首并删除错误文件
		if (info)
		{
			HWND MList = GetDlgItem(hwnd,IDC_MList);
			int j = ListBox_GetCurSel(MList);
			//删除错误文件
			ListBox_DeleteString(MList,j);//
			int t = ListBox_GetCount(MList);
			//定位到下一首
			if (t == j)
				ListBox_SetCurSel(MList,0);
			else
				ListBox_SetCurSel(MList,j);//
			//递归重新调用播放函数
			Play(hwnd);//
			//若播放文件错误则不继续执行
			return;//
		}//
		//若播放成功，将文件名写入正在播放编辑框
		SetDlgItemText(hwnd,IDC_EDITNOW,Choose);//
		//设置播放时间定时器
		SetTimer(hwnd,TIMER_PLAY,TIMER_PLAY_TIME,(TIMERPROC)Change);//
		TimeTGet(ShortName,hwnd);
		wsprintf(Time,"00:00/%02d:%02d",Tmin,Tsec);
		//刷新播放时间
		SetDlgItemText(hwnd,IDC_EDITTIME,Time);//
		//调整音量
		NVoice(hwnd);
		Voice(hwnd);//
		//初始化播放时间
		beginTimeSet(ShortName,hwnd);//
		//重新计算列表数量
		ListNow(hwnd);//
		//读取歌曲信息
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
	//若选中歌曲与正在播放歌曲相同则继续播放
	if (!strcmp(Choose1,Choose2))
	{	
		TCHAR * ShortName = GSNTE(hwnd);
		TCHAR play[MAX_PATH+10];
		wsprintf(play,"play %s",ShortName);
		//播放歌曲
		mciSendString(play,"",0,NULL);//
		//设置播放时间定时器
		SetTimer(hwnd,TIMER_PLAY,TIMER_PLAY_TIME,(TIMERPROC)Change);//
	}//
	//若无选中歌曲则继续播放正在播放歌曲
	else if (-1 == i)
	{
		TCHAR * ShortName = GSNTE(hwnd);
		TCHAR play[MAX_PATH+10];
		wsprintf(play,"play %s",ShortName);
		mciSendString(play,"",0,NULL);
	}//
	//若选中歌曲与正在播放歌曲不同则播放选中歌曲
	else
	{
		Stop(hwnd);
		Play(hwnd);
	}//

}

//停止函数
void Stop(HWND hwnd)
{
	TCHAR * ShortName = GSNTE(hwnd);
	TCHAR play[MAX_PATH+10];
	wsprintf(play,"stop %s",ShortName);
	//停止歌曲
	mciSendString(play,"",0,NULL);//
	//清空正在播放编辑框
	SetDlgItemText(hwnd,IDC_EDITNOW,"");//
	wsprintf(Time,"00:00/00:00");
	//播放时间清零
	SetDlgItemText(hwnd,IDC_EDITTIME,Time);//
	//播放进度滚动条归零
	SendMessage(GetDlgItem(hwnd,IDC_MUSIC_SLIDER),TBM_SETPOS,(WPARAM)TRUE,0);//
	//歌曲信息清空
	SetDlgItemText(hwnd,IDC_Title,"");
	SetDlgItemText(hwnd,IDC_Artist,"");
	SetDlgItemText(hwnd,IDC_Album,"");//
	//销毁播放时间定时器
	KillTimer(hwnd,TIMER_PLAY);//
}//

//暂停函数
void Pause(HWND hwnd)
{
	TCHAR * ShortName = GSNTE(hwnd);
	TCHAR play[MAX_PATH+10];
	wsprintf(play,"pause %s",ShortName);
	//暂停歌曲
	mciSendString(play,"",0,NULL);//
	//销毁播放时间定时器
	KillTimer(hwnd,TIMER_PLAY);//
}//

//上一曲函数（只选中不播放）
void Last(HWND hwnd)
{
	HWND MList = GetDlgItem(hwnd,IDC_MList);
	TCHAR Choose[MAX_PATH];
	GetDlgItemText(hwnd,IDC_EDITNOW,Choose,sizeof(Choose));
	//在列表中查找与正在播放编辑框中内容相同的选项
	int i = ListBox_FindString(MList,0,Choose);
	int max = ListBox_GetCount(MList);
	//若选中歌曲非列表第一首歌曲，选中上一首歌曲
	if(i-1 >= 0)
		ListBox_SetCurSel(MList,i-1);//
	//若选中歌曲为列表第一首歌曲，选中最后一首歌曲
	else
		ListBox_SetCurSel(MList,max-1);//
}//

//下一曲函数（只选中不播放）
void Next(HWND hwnd)
{
	HWND MList = GetDlgItem(hwnd,IDC_MList);
	TCHAR Choose[MAX_PATH];
	GetDlgItemText(hwnd,IDC_EDITNOW,Choose,sizeof(Choose));
	//在列表中查找与正在播放编辑框中内容相同的选项
	int i = ListBox_FindString(MList,0,Choose);
	int max = ListBox_GetCount(MList);
	//若选中歌曲非列表最后一首歌曲，选中下一首歌曲
	if(i <= max-2)
		ListBox_SetCurSel(MList,i+1);//
	//若选中歌曲为列表最后一首歌曲，选中第一首歌曲
	else
		ListBox_SetCurSel(MList,0);
}

//调整音量函数
void Voice(HWND hwnd)
{
	//获得音量滚动条的值
	int volume = (int)SendMessage(GetDlgItem(hwnd,IDC_SOUND_SLIDER),TBM_GETPOS,0,0);//
	TCHAR vol[MAX_PATH];
	//音量滚动条最大值为640，换算为100
	_itoa_s((int)(volume * 0.15625),vol,10);
	SetDlgItemText(hwnd,IDC_EDITVOICE,vol);
	TCHAR * ShortName = GSNTE(hwnd);
	TCHAR szVolume[MAX_PATH+20];
	wsprintf(szVolume,"setaudio %s volume to %d",ShortName,volume);
	//设置音量
    mciSendString(szVolume,"",0,NULL);//
}//

//静音函数
BOOL Sof(HWND hwnd,int onoff,BOOL min,HWND minhwnd)
{
	TCHAR * ShortName = GSNTE(hwnd);
	TCHAR play[MAX_PATH+10];
	//若未静音
	if(onoff)
	{
		wsprintf(play,"setaudio %s off",ShortName);
		//设置静音
		mciSendString(play,"", 0, NULL);//
		//设置复选框为对应状态
		CheckDlgButton(hwnd,IDC_SOF,BST_CHECKED);
		if (min)
			CheckDlgButton(minhwnd,IDC__SOF,BST_CHECKED);
		//改变静音状态参数
		onoff = FALSE;//
	}//
	//若已静音
	else
	{
		wsprintf(play,"setaudio %s on",ShortName);
		//取消静音
		mciSendString(play,"", 0, NULL);//
		//设置复选框为对应状态
		CheckDlgButton(hwnd,IDC_SOF,BST_UNCHECKED);
		if (min)
			CheckDlgButton(minhwnd,IDC__SOF,BST_UNCHECKED);
		//改变静音状态参数
		onoff = TRUE;//
	}//
	return onoff;
}//

//静音状态检测函数
void NVoice(HWND hwnd)
{
	//若为静音状态
	if (!onoff)
	{
		TCHAR * ShortName = GSNTE(hwnd);
		TCHAR play[MAX_PATH+10];
		wsprintf(play,"setaudio %s off",ShortName);
		//静音播放
		mciSendString(play, NULL, 0, NULL);//
	}//
}//

//获取文件短路径函数
TCHAR * GSNTE(HWND hwnd)
{
	TCHAR Choose[MAX_PATH];
	//获取正在播放编辑框中文件路径
	GetDlgItemText(hwnd,IDC_EDITNOW,Choose,sizeof(Choose));//
	//返回文件短路径
	GetShortPathName(Choose,ShortName1,sizeof(ShortName1));
	return ShortName1;//
}

//根据歌词从相应位置播放
void playfrom(HWND hwnd,int Time)
{
	TCHAR Choose[MAX_PATH],STime[20];
	GetDlgItemText(hwnd,IDC_EDITNOW,Choose,sizeof(Choose));
	//若有正在播放歌曲
	if ('\0' != *Choose)
	{
		if (Time < 0)
			Time = 0;
		int stemp = Time/1000 + 1;
		int TTmin = stemp/60;
		int TTsec = stemp%60;//			
		wsprintf(STime,"%02d:%02d/%02d:%02d",TTmin,TTsec,Tmin,Tsec);
		//改变播放时间编辑框时间
		SetDlgItemText(hwnd,IDC_EDITTIME,STime);//
		KillTimer(hwnd,TIMER_PLAY);
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
		unsigned long NT = TimeNGet(ShortName,hwnd);
		//移动播放时间滚动条到相应位置
		SendMessage(GetDlgItem(hwnd,IDC_MUSIC_SLIDER),TBM_SETPOS,(unsigned long)TRUE,NT);//
		SetTimer(hwnd,TIMER_PLAY,TIMER_PLAY_TIME,(TIMERPROC)Change);
	}//
}//

#endif