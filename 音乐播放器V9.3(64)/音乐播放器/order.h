/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//����˳��������
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef order_h
#define order_h

//����ѭ������
void MSelf(HWND hwnd)
{
	KillTimer(hwnd,TIMER_PLAY);
	TCHAR * ShortName = GSNTE(hwnd);
	TCHAR play[MAX_PATH+10];
	wsprintf(play,"play %s",ShortName);
	//�������ڲ��ű༭���и���
	mciSendString(play,"",0,NULL);//
	//��ʼ������ϵͳ����ϸ��"control.h"��void Play(HWND hwnd)������
	SetTimer(hwnd,TIMER_PLAY,TIMER_PLAY_TIME,(TIMERPROC)Change);
	TimeTGet(ShortName,hwnd);
	wsprintf(Time,"00:00/%02d:%02d",Tmin,Tsec);
	SetDlgItemText(hwnd,IDC_EDITTIME,Time);
	NVoice(hwnd);
	Voice(hwnd);
	beginTimeSet(ShortName,hwnd);
	ListNow(hwnd);//
}//

//������ź���
void Rand(HWND hwnd)
{
	HWND MList = GetDlgItem(hwnd,IDC_MList);
	int max = ListBox_GetCount(MList);
	//ͨ����ʼ��������ʱ����������Ӳ�����������ٲ������������
	srand((unsigned)time(NULL)*rand()/rand());
	//������������ֵ
	int i = rand()%max;//
	ListBox_SetCurSel(MList,i);
}//
#endif