/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//主窗口读取设置文件函数定义
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef mainset_h
#define mainset_h

//获取程序路径函数
TCHAR * GetWay(TCHAR * appPath)
{
	//获取程序路径与文件名
	int appl=GetModuleFileName(NULL,appPath,256);
	//从后向前查找\替换为\0，去掉文件名
	for(int i=appl-1;i>=0;i--)
		if(appPath[i]!='\\')
 			appPath[i]='\0';//
 		else
 			break;
 	return appPath;
}//	

//返回可读设置文件函数
FILE * openSetFile()
{
	FILE * FList;
	TCHAR file[MAX_PATH];
	wsprintf(file,"%s配置文件(请勿删除或更改).dat",GetWay(file));
	//以读写方式打开设置文件，若设置文件不存在则返回NULL
	fopen_s(&FList,file,"r+");//
	return FList;
}

//读取睡眠等待时间函数
int SleepTime()
{
	FILE *FList;
	//若不存在设置文件，默认时间为30分钟
	int t = 30;//
	//若存在设置文件
	if (NULL != (FList=openSetFile()))
	{
		TCHAR Fc[MAX_PATH*2];
		ZeroMemory(Fc,sizeof(Fc));
		fgets(Fc,MAX_PATH*2,FList);
		TCHAR *p = Fc;
		TCHAR *q = Fc;
		//不断向前搜索|并检测标签是否为“_st”
		while (*p)
		{
			p++;
			if ('|' == *p)
			{
				*p = '\0';
				//若找到“_st”标签
				if(!strcmp("_st",q))
				{
					q = p+1;
					t = atoi(q);
				}//
				p++;
				q = p;
			}
		}
		fclose(FList);
	}//
	//返回以秒为单位的睡眠等待时间
	return t*60;//
}

#endif