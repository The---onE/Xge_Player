/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//播放列表总数读取函数定义
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef list_h
#define list_h

//计算列表总数函数
void ListTotal(HWND hwnd)
{
	HWND MList = GetDlgItem(hwnd,IDC_MList);
	//获取列表总数
	max = ListBox_GetCount(MList);//
	TCHAR Lmax[5];
	wsprintf(Lmax,"%03d/%03d",now,max);
	//设置列表编辑框中列表总数
	SetDlgItemText(hwnd,IDC_EDITLIST,Lmax);//
}

//计算当前歌曲位置函数
void ListNow(HWND hwnd)
{
	HWND MList = GetDlgItem(hwnd,IDC_MList);
	TCHAR Choose[MAX_PATH];
	GetDlgItemText(hwnd,IDC_EDITNOW,Choose,sizeof(Choose));
	//在列表中查找与正在播放编辑框中内容相同的选项
	now = ListBox_FindString(MList,0,Choose)+1;//
	TCHAR Lmax[5];
	wsprintf(Lmax,"%03d/%03d",now,max);
	//设置列表编辑框中正在播放歌曲位置
	SetDlgItemText(hwnd,IDC_EDITLIST,Lmax);//
}//
#endif