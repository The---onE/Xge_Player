/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//播放顺序函数定义
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef order_h
#define order_h

//单曲循环函数
void MSelf(HWND hwnd)
{
	KillTimer(hwnd,TIMER_PLAY);
	TCHAR * ShortName = GSNTE(hwnd);
	TCHAR play[MAX_PATH+10];
	wsprintf(play,"play %s",ShortName);
	//播放正在播放编辑框中歌曲
	mciSendString(play,"",0,NULL);//
	//初始化播放系统（详细见"control.h"中void Play(HWND hwnd)函数）
	SetTimer(hwnd,TIMER_PLAY,TIMER_PLAY_TIME,(TIMERPROC)Change);
	TimeTGet(ShortName,hwnd);
	wsprintf(Time,"00:00/%02d:%02d",Tmin,Tsec);
	SetDlgItemText(hwnd,IDC_EDITTIME,Time);
	NVoice(hwnd);
	Voice(hwnd);
	beginTimeSet(ShortName,hwnd);
	ListNow(hwnd);//
}//

//随机播放函数
void Rand(HWND hwnd)
{
	HWND MList = GetDlgItem(hwnd,IDC_MList);
	int max = ListBox_GetCount(MList);
	//通过初始化主窗口时的随机数种子产生的随机数再产生随机数种子
	srand((unsigned)time(NULL)*rand()/rand());
	//设置随机数最大值
	int i = rand()%max;//
	ListBox_SetCurSel(MList,i);
}//
#endif