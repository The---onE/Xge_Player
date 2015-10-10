/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//列表中文件管理（添加、删除、移动）函数定义
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef file_h
#define file_h

//打开文件函数
void openf(HWND hwnd)
{
	HWND MList = GetDlgItem(hwnd,IDC_MList);  
	TCHAR * p;
	char szFile[MAX_PATH * 200];
	//初始化打开文件窗口结构体,详细信息搜索OPENFILENAME
	OPENFILENAME ofn;
	ZeroMemory(&ofn,sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = TEXT('\0');
	ofn.nMaxFile = sizeof(szFile);
	//设置可打开文件类型
	ofn.lpstrFilter = TEXT("Music(*.mp3;*.wma)\0*.mp3;*.wma\0MP3(*.mp3)\0*.mp3\0WMA(*.wma)\0*.wma\0ALL(*.*)\0*.*\0");//
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.hwndOwner = hwnd;
	//设置窗口风格
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT ;////
	//若有选中文件
	if (GetOpenFileName(&ofn))
	{
		p = szFile + ofn.nFileOffset;
		BOOL j = FALSE;
		//若选中多个文件则不断读取
		while (*p)//
		{
			//若选中单个文件
			if('\0' != *(szFile + ofn.nFileOffset - 1))
			{
				ListBox_InsertString(MList,-1,szFile);
				for (int i = 0 ; i < ListBox_GetCount(MList)-1 ; i++)
				{
					TCHAR List[MAX_PATH];
					ListBox_GetText(MList,i,List);
					//若含有同名文件则删除新文件
					if (0 == strcmp(List,szFile))
					{
						ListBox_DeleteString(MList,(ListBox_GetCount(MList)-1));//
						//设置同名文件为选中
						ListBox_SetCurSel(MList,i);
						j = TRUE;
						break;
					}//
				}
				if (!j)
				{
					//若无同名文件则设置新文件为选中
					ListBox_SetCurSel(MList,ListBox_GetCount(MList)-1);//
				}//
				//若选中单个文件则不再读取
				break;//
			}//

			//若选中多个文件
			//字符串格式: 文件目录\0文件名\0文件名\0……\0文件名\0\0 //
			else
			{
				TCHAR lj[MAX_PATH];
				//跳过以\替换\0连接文件目录与文件名
				wsprintf(lj,"%s\\%s",szFile,p);//
				//添加文件
				ListBox_InsertString(MList,-1,lj);//
				for (int i = 0 ; i < ListBox_GetCount(MList)-1 ; i++)
				{
					TCHAR List[MAX_PATH];
					ListBox_GetText(MList,i,List);
					//若含有同名文件则删除新文件
					if (0 == strcmp(List,lj))
					{
						ListBox_DeleteString(MList,(ListBox_GetCount(MList)-1));//
						//设置同名文件为选中
						ListBox_SetCurSel(MList,i);//
						j = TRUE;
						break;
					}
				}
				if (!j)
				{
					//若无同名文件则设置新文件为选中
					ListBox_SetCurSel(MList,ListBox_GetCount(MList)-1);//
				}
				p += lstrlen(p) +1;
			}//
		}
	}//
}//

//删除函数
void Del(HWND hwnd)
{
	TCHAR Choose1[MAX_PATH];
	GetDlgItemText(hwnd,IDC_EDITNOW,Choose1,sizeof(Choose1));
	HWND MList = GetDlgItem(hwnd,IDC_MList);
	int i = ListBox_GetCurSel(MList);
	TCHAR Choose2[MAX_PATH];
	ListBox_GetText(MList,i,Choose2);
	//若删除歌曲为正在播放歌曲，先停止再删除
	if (0 == strcmp(Choose1,Choose2))
	{
		Stop(hwnd);
		ListBox_DeleteString(MList,i);
	}//
	//若删除歌曲非正在播放歌曲，直接删除
	else
		ListBox_DeleteString(MList,i);//
}//

//上移函数
void Up(HWND hwnd)
{
	HWND MList = GetDlgItem(hwnd,IDC_MList);
	int i = ListBox_GetCurSel(MList);
	int max = ListBox_GetCount(MList);
	TCHAR Choose1[MAX_PATH],Choose2[MAX_PATH];
	ListBox_GetText(MList,i,Choose1);
	if(i-1 >= 0)
		ListBox_GetText(MList,i-1,Choose2);
	//互换选中歌曲与选中歌曲的上一首歌曲
	ListBox_DeleteString(MList,i);
	if(i-1 >= 0)
		ListBox_DeleteString(MList,i-1);
	//若选中歌曲非列表第一首歌曲，将选中歌曲与上一首歌曲互换
	if(i-1 >= 0)
	{
		ListBox_InsertString(MList,i-1,Choose2);
		ListBox_InsertString(MList,i-1,Choose1);
	}//
	//若选中歌曲为列表第一首歌曲，将选中歌曲移至列表最后一首之后
	else
		ListBox_InsertString(MList,max-1,Choose1);////
	//设置原选中歌曲仍为选中
	if(i-1 >= 0)
		ListBox_SetCurSel(MList,i-1);
	else
		ListBox_SetCurSel(MList,max-1);//
	
	//重新计算当前播放歌曲位置
	ListNow(hwnd);//
}//

//下移函数
void Down(HWND hwnd)
{
	HWND MList = GetDlgItem(hwnd,IDC_MList);
	int i = ListBox_GetCurSel(MList);
	int max = ListBox_GetCount(MList);
	TCHAR Choose1[MAX_PATH],Choose2[MAX_PATH];
	ListBox_GetText(MList,i,Choose1);
	if(i <= max-2)
		ListBox_GetText(MList,i+1,Choose2);
	//互换选中歌曲与选中歌曲的上一首歌曲
	ListBox_DeleteString(MList,i);
	if(i <= max-2)
		ListBox_DeleteString(MList,i);
	//若选中歌曲非列表最后一首歌曲，将选中歌曲与下一首歌曲互换
	if(i <= max-2)
	{
		ListBox_InsertString(MList,i,Choose1);
		ListBox_InsertString(MList,i,Choose2);
	}//
	//若选中歌曲为列表最后一首歌曲，将选中歌曲移至列表第一首歌曲之前
	else
		ListBox_InsertString(MList,0,Choose1);////
	//设置原选中歌曲仍为选中
	if(i <= max-2)
		ListBox_SetCurSel(MList,i+1);
	else
		ListBox_SetCurSel(MList,0);//

	//重新计算当前播放歌曲位置
	ListNow(hwnd);//
}//

//返回可写列表文件函数
FILE * SList(int i)
{
	FILE * FList;
	TCHAR file[MAX_PATH];
	wsprintf(file,"%sX哥音乐播放器列表%d(删除后列表将丢失).dat",GetWay(file),i);
	//以读写方式打开或创建列表文件
	fopen_s(&FList,file,"w+");//
	return FList;
}//

//返回可读列表文件函数
FILE * OFile(int i)
{
	FILE * FList;
	TCHAR file[MAX_PATH];
	wsprintf(file,"%sX哥音乐播放器列表%d(删除后列表将丢失).dat",GetWay(file),i);
	//以读写方式打开列表文件，若列表文件不存在则返回NULL
	fopen_s(&FList,file,"r+");//
	return FList;
}//

//保存列表函数
void saveList(HWND hwnd)
{
	FILE * FList;
	int i = ComboBox_GetCurSel(GetDlgItem(hwnd,IDC_CSL))+1;
	TCHAR asksave[20];
	wsprintf(asksave,"确定要保存至列表%d吗？",i);
	//弹出确认对话框，若确认则保存列表
	int yn = MessageBox(hwnd,asksave,TEXT("保存列表"),MB_YESNO|MB_ICONQUESTION);
	if (IDYES == yn)
	{
		FList = SList(i);
		HWND MList = GetDlgItem(hwnd,IDC_MList);
		TCHAR Choose[MAX_PATH];
		//以：文件路径\文件名|文件路径\文件名|……|文件路径\文件名| 形式保存至列表文件
		for (int t = 0;t < ListBox_GetCount(MList);t++)
		{
			ListBox_GetText(MList,t,Choose);
			fprintf(FList,"%s|",Choose);
		}//
		fclose(FList);
		MessageBox(hwnd,"保存列表成功！","成功",MB_OK|MB_ICONINFORMATION);
	}//
	else
		return;
}//

//打开列表函数
void openList(HWND hwnd)
{
	FILE *FList;
	HWND MList = GetDlgItem(hwnd,IDC_MList);
	int i = ComboBox_GetCurSel(GetDlgItem(hwnd,IDC_COL))+1;
	TCHAR askopen[20];
	wsprintf(askopen,"确定要打开列表%d吗？",i);
	//弹出确认对话框，若确认则读取列表
	int yn = MessageBox(hwnd,askopen,TEXT("打开列表"),MB_YESNO|MB_ICONQUESTION);
	if (IDYES == yn)
	{
		//若列表不存在，提示不存在
		if (NULL == (FList = OFile(i)))
			MessageBox(hwnd,TEXT("列表不存在"),"",MB_OK|MB_ICONSTOP);//
		else
		{
			TCHAR Fc[MAX_PATH*500];
			ZeroMemory(Fc,sizeof(Fc));
			fgets(Fc,MAX_PATH*500,FList);
			TCHAR *p = Fc;
			TCHAR *q = Fc;
			//以|分隔不断读取文件路径
			for (int i = 0;i<MAX_PATH*500;i++)
			{
				p++;
				if('\0' == *p)
					break;
				else if ('|' == *p)
				{
					*p = '\0';
					ListBox_InsertString(GetDlgItem(hwnd,IDC_MList),-1,q);
					for (int i = 0 ; i < ListBox_GetCount(MList)-1 ; i++)
					{
						TCHAR List[MAX_PATH];
						ListBox_GetText(MList,i,List);
						//若含有同名文件则删除新文件
						if (0 == strcmp(List,q))
						{
							ListBox_DeleteString(MList,(ListBox_GetCount(MList)-1));//
							break;
						}
					}
					q = p+1;
				}
			}//
			fclose(FList);
			//重新计算列表总数
			ListTotal(hwnd);
			ListNow(hwnd);//
		}
	}
	else
		return;		
}//

#endif