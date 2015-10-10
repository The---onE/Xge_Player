/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//全局变量及设置窗口函数定义
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef setmanage_h
#define setmanage_h

HWND setphwnd;
TCHAR SourceSet[MAX_PATH*2];
TCHAR NewSet[MAX_PATH*2];

//获取程序路径函数
TCHAR * _GetWay(TCHAR * appPath)
{
	//获取程序路径与文件名
	int appl=GetModuleFileName(NULL,appPath,256);//
	//从后向前查找\替换为\0，去掉文件名
	for(int i=appl-1;i>=0;i--)
		if(appPath[i]!='\\')
			appPath[i]='\0';//
		else
			break;
		return appPath;
}//

//返回可读设置文件函数
FILE * _openSetFile()
{
	FILE * FList;
	TCHAR file[MAX_PATH];
	wsprintf(file,"%s配置文件(请勿删除或更改).dat",_GetWay(file));
	//以读写方式打开设置文件，若设置文件不存在则返回NULL
	fopen_s(&FList,file,"r+");//
	return FList;
}//

//返回可写设置文件函数
FILE * _saveSetFile()
{
	FILE * FList;
	TCHAR file[MAX_PATH];
	wsprintf(file,"%s配置文件(请勿删除或更改).dat",_GetWay(file));
	//以读写方式打开或创建列表文件
	fopen_s(&FList,file,"w+");//
	return FList;
}//

//获取标签对应数值函数
void getLable(char* lable,int* num)
{
	FILE *FList;
	//若存在设置文件
	if (NULL != (FList=_openSetFile()))
	{
		TCHAR Fc[MAX_PATH*2];
		ZeroMemory(Fc,sizeof(Fc));
		fgets(Fc,MAX_PATH*2,FList);
		TCHAR *p = Fc;
		TCHAR *q = Fc;
		//不断向前搜索|并检测标签
		while (*p)
		{
			p++;
			if ('|' == *p)
			{
				*p = '\0';
				//若找到对应标签
				if(!strcmp(lable,q))
				{
					q = p+1;
					*num = atoi(q);
					break;
				}//
				p++;
				q = p;
			}//
		}
		fclose(FList);
	}//
}//

void getSourceSet()
{
	FILE *FList;
	if (NULL != (FList=_openSetFile()))
	{
		fgets(SourceSet,MAX_PATH*2,FList);
		fclose(FList);
	}
	else
		strcpy_s(SourceSet,"");
}

//创建要保存的内容
void CreateSet(HWND hwnd)
{
	int st,r,g,b;
	TCHAR lable[5];
	GetDlgItemText(hwnd,IDC_SLEEPTIME,lable,sizeof(lable));
	st = atoi(lable);
	GetDlgItemText(hwnd,IDC_R,lable,sizeof(lable));
	r = atoi(lable);
	GetDlgItemText(hwnd,IDC_G,lable,sizeof(lable));
	g = atoi(lable);
	GetDlgItemText(hwnd,IDC_B,lable,sizeof(lable));
	b = atoi(lable);
	if (r >255) r = 255;
	if (g >255) g = 255;
	if (b >255) b = 255;
	wsprintf(NewSet,"_st|%d|_cr|%d|_cg|%d|_cb|%d|",st,r,g,b);
}//

//保存设置文件函数
void setFile()
{
	FILE *FList;
	//若睡眠等待时间编辑框不为空
	if (NULL != (FList=_saveSetFile()))
	{
		//按：_st|时间 格式保存至设置文件
		fprintf(FList,"%s",NewSet);//
		fclose(FList);
	}
}//

#endif