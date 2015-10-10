/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//全局变量及歌词窗口函数定义
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _mlmanage_H
#define _mlmanage_H

//定义主窗口句柄全局变量
HWND milphwnd;//

char name[MAX_PATH] = "";
//定义时间标签时间全局变量
int _time[LINE][SAME];//
//定义根据时间排序的歌词顺序全局变量
int _index[LINE*SAME];//
//定义已排好序的时间全局变量
int _orderlytime[LINE*SAME];//
//定义时间补偿时值全局变量
int _offset = 0;//

//定义歌词结构体
struct
{	
	char *s;
}_text[LINE*SAME];//

//定义歌词结构体
struct
{	
	char s[MAX_LENGTH];
}_entity[LINE*SAME];//

//初始化变量函数
void Initialization()
{
	//初始化时间标签时间全局变量，防止未赋值数据影响排序
	for (int x=0 ; x<LINE ; x++)
	{
		for (int y=0 ; y<SAME ; y++)
		{
			_time[x][y] = MAX_TIME+1;	
		}
	}//
	//初始化全局变量，以便通过-1确定边界
	for (int z=0 ; z<LINE*SAME ; z++)
	{
		_index[z] = -1;
		_orderlytime[z] = -MAX_TIME;
	}//
}
//
//标签处理函数
BOOL _manageTag(HWND phwnd,char *q,int j,int k)
{
	//若为时间标签
	if (*q>='0' && *q<='9')
	{
		//至非数字字符停止读取，读取分钟数
		int mm = atoi(q);//
		//使指针指向“:”之后
		for (int x=0 ; x<2 ; x++)
		{
			q++;
			if (':' == *q)
			{
				q++;
				break;
			}
		}//
		//至非数字字符停止读取，读取秒数
		int ss = atoi(q);//
		//使指针指向“:”或“.”之后
		for (int y=0 ; y<2 ; y++)
		{
			q++;
			if ('.'==*q || ':'==*q)
			{
				q++;
				break;
			}
		}//
		//至非数字字符停止读取，读取豪秒数
		int ff = atoi(q);//
		//若未越界
		if(k<SAME)
		{
			//将时间标签以毫秒为单位存入数组
			_time[j][k] = ff*10 + ss*1000 + mm*60000 + 1;//
		}//
		//是时间标签
		return TRUE;//
	}//
	//若为offset补偿时值标签
	else if (('o'==q[0]||'O'==q[0]) && ('f'==q[1]||'F'==q[1]) && ('f' == q[2]||'F'==q[2]) && \
			 ('s'==q[3]||'S'==q[3]) && ('e'==q[4]||'E'==q[4]) && ('t'==q[5]||'T'==q[5]) && (':' == q[6]))
	{
		//指针指向“:”之后
		q += 7;
		//若“:”之后为数字或正负号
		if ('-'==*q || '+'==*q || (*q>='0' && *q<='9'))
		{
			//至非数字字符停止读取，读取补偿时值，存入全局变量
			_offset = -atoi(q);
		}//
		//非时间标签
		return FALSE;//
	}//
	//若为ti标题标签
	else if (('t'==q[0]||'T'==q[0]) && ('i'==q[1]||'I'==q[1]) && (':' == q[2]))
	{
		//指针指向“:”之后
		q += 3;
		TCHAR ti[MAX_PATH];
		GetDlgItemText(phwnd,IDC_Title,ti,sizeof(ti));
		//若歌曲文件无ID3标题信息则将标题加入主窗口歌曲名编辑框
		if ('\0' == *ti)
			SetDlgItemText(phwnd,IDC_Title,q);//
		//非时间标签
		return FALSE;//
	}//
	//若为ar歌手标签
	else if (('a'==q[0]||'A'==q[0]) && ('r'==q[1]||'R'==q[1]) && (':' == q[2]))
	{
		//指针指向“:”之后
		q += 3;
		TCHAR ar[MAX_PATH];
		GetDlgItemText(phwnd,IDC_Artist,ar,sizeof(ar));
		//若歌曲文件无ID3歌手信息则将歌手名加入主窗口歌手编辑框
		if ('\0' == *ar)
			SetDlgItemText(phwnd,IDC_Artist,q);//
		//非时间标签
		return FALSE;//
	}//
	//若为al专辑标签
	else if (('a'==q[0]||'A'==q[0]) && ('l'==q[1]||'L'==q[1]) && (':' == q[2]))
	{
		//指针指向“:”之后
		q += 3;
		TCHAR al[MAX_PATH];
		GetDlgItemText(phwnd,IDC_Album,al,sizeof(al));
		//若歌曲文件无ID3专辑信息则将专辑名加入主窗口歌手编辑框
		if ('\0' == *al)
			SetDlgItemText(phwnd,IDC_Album,q);//
		//非时间标签
		return FALSE;//
	}//
	else
		return FALSE;
}//

//排序函数
void _sort()
{
    //找出所有时间标签的最小值
	//最小值、次小值初始化为时间标签最大值+1
	int min = MAX_TIME,smin = MAX_TIME;
	//不断寻找，若比假定值小则替换，直到确定最小值
    for (int x=0 ; x<LINE ; x++)
	{
		for (int y=0 ; y<SAME ; y++)
		{
			if (_time[x][y] < min)
			{
				//使原标签+1后自减以便使相同时间标签不被略过
				min = --_time[x][y];//
				//将歌词序号存入全局变量
				_index[0] = x;//
				//相应时间存入全局变量，orderlytime[i]的时间对应第index[i]（的值）段歌词
				_orderlytime[0] = min;//
			}
		}
	}////
	//为时间标签排序
	for (int in=1 ; in<LINE*SAME ; in++)
	{
		//不断寻找，以最小值为底线，若比假定值小且比最小值大则替换，直到确定次小值
		for (int x=0 ; x<LINE ; x++)
		{
			for (int y=0 ; y<SAME ; y++)
			{
				if (_time[x][y]>min && _time[x][y]<=smin)
				{
					//使原标签+1后自减以便使相同时间表情不被略过
					smin = --_time[x][y];//
					//将歌词序号存入全局变量
					_index[in] = x;//
					//相应时间存入全局变量，orderlytime[i]的时间对应第index[i]（的值）段歌词
					_orderlytime[in] = smin;//
				}
			}
		}//
		//将次小值作为底线，次小值设为最小值
		min = smin;//
		//假定值设为时间标签最大值+1
		smin = MAX_TIME;//
	}
}//

//处理lrc文档函数
BOOL _manageLrc(HWND phwnd,char * c)
{
	int j = -1,k = 0;
	char *p=c , *q=c , *r=c;
	//向后读取直到结束标志
	while (*p != EOF)
	{
		//若指针指向标签首
		if ('[' == *p)//
		{
			//不同歌词数加1
			j++;//
			if (j >= LINE)
				return FALSE;
			//指针指向标签内容
			q = p+1;//
			//向后读取直到换行
			while (*p != '\n')
			{
				p++;
				//若指针指向标签尾
				if (']' == *p)//
				{
					//相同歌词数重新置0
					k = 0;//
					//将标签尾变为\0
					*p = '\0';
					//处理标签
					//若非时间标签不同歌词数减1
					if (!_manageTag(phwnd,q,j,k))//
						j--;//
					//不断向后读取检测是否有相同歌词
					p++;
					while (1)
					{
						//若有相同歌词
						if('[' == *p)//
						{
							//指针指向标签内容
							q = p+1;//
							//向后读取直到换行
							while (*p != '\n')
							{	
								p++;
								//若指针指向标签尾
								if (']' == *p)
								{
									//相同歌词数加1
									k++;//
									//将标签尾变为\0
									*p = '\0';//
									if (k >= SAME)
										break;
									//处理标签
									//若非时间标签不同歌词数减1
									if (!_manageTag(phwnd,q,j,k))//
										j--;//
									p++;
									break;
								}
							}//
						}
						//若无相同歌词
						else//
						{
							//指针指向标签尾之后歌词内容
							r = (q = p);
							//歌词结构体中字符指针指向歌词内容
							_text[j].s = q;//
							//将换行符变为\0，将歌词内容截断
							while (*r != '\n')
								r++;
							*r = '\0';//
							break;
						}//if('[' == *p)/
					}////while(1)
					break;
				}//if (']' == *p)   
			}////while (*p != '\n')
		}//if ('[' == *p)
		p++;
	}////while (*p != EOF)
	return TRUE;
}//

//打开、处理歌词文件并加入列表框
void _GetSetLrc(HWND hwnd,HWND phwnd)
{
	TCHAR mp3[MAX_PATH],lrc[MAX_PATH],*s,*t;
	//获取主窗口正在播放歌曲
	GetDlgItemText(phwnd,IDC_EDITNOW,mp3,sizeof(mp3));//
	s = t = mp3;
	//若有正在播放歌曲
	if (*mp3 != '\0')
	{
		//将指针指向文件名结尾
		while (*s != '\0')
		{
			s++;	
		}//
		//向前读取直到盘符
		while (*s != ':')
		{
			s--;
			//将“.”变为\0以更改拓展名
			if ('.' == *s)
			{
				*s = '\0';
				break;
			}//
		
		}//

		//指针指向最近“\”之后字符，以便得到歌曲名
		while (*s != '\\')
		{
			s--;
		}
		s++;//

		//若原歌曲名与新歌曲名相同则不再继续执行
		if (!strcmp(s,name))
			return;//

		strcpy_s(name,s);
		SetDlgItemText(hwnd,IDC_MINILRC,name);
		//打开歌曲同名lrc文件
		wsprintf(lrc,"%s.lrc",t);//
	}
	//若无正在播放歌曲
	else
	{
		if (!strcmp(name,""))
			return;
		Initialization();
		SetDlgItemText(hwnd,IDC_MINILRC,"");
		strcpy_s(name,"");
	}//

	FILE *f;
	//尝试打开lrc文件，若打开成功
	if(!fopen_s(&f,lrc,"r"))
	{
		Initialization();
		
		int i = 0;
		char temp;
		//不断读取lrc文件直到文件结束
		do
		{
			i++;
			temp = fgetc(f);
		}while (temp != EOF);//

		fseek(f,0L,SEEK_SET);
		char *c;
		//为整个文档分配空间
		int mc = 0;
		do
		{
			c = (char*)malloc(sizeof(char) * (i+5));
			mc++;
		}while (!c && mc<10);//
		if (10 == mc)
		{
			MessageBox(hwnd,"歌词过长或内存不足，无法获取歌词","错误",MB_OK);
			return;
		}
		i = -1;
		//不断读取lrc文件直到文件结束
		do
		{
			i++;
			c[i] = fgetc(f);
		}while (c[i] != EOF);//

		//读取lrc文档
		BOOL enough = _manageLrc(phwnd,c);//

		//排序时间标签
		_sort();//

		//若有补偿时值，为所有时间标签时间添加补偿时值
		if (_offset)
		{
			for (int u=0 ; u<LINE*SAME ; u++)
			{
				if (_orderlytime[u] != -MAX_TIME)
					_orderlytime[u] += _offset;
				if (_orderlytime[u] < 0)
					_orderlytime[u] = 0;
			}
		}//
		//设置补偿时值为0以便下一首歌使用
		_offset = 0;//

		for (int w=0 ; w<LINE*SAME ; w++)
		{
			if (_index[w] != -1)
				for (int a=0 ; a<MAX_LENGTH ; a++)
				{
					_entity[_index[w]].s[a] = _text[_index[w]].s[a];
					if (!_text[_index[w]].s[a])
						break;
				}
				_entity[_index[w]].s[MAX_LENGTH-1] = '\0';
		}//

		//关闭lrc文件
		fclose(f);//
		//回收lrc文档空间
		free(c);//

	}//
	else
		Initialization();
}//

//歌词跟随歌曲进度函数
void _select(HWND hwnd,int musictime)
{
	for (int v=0 ; v<LINE*SAME ; v++)
	{
		//若循环至边界则退出循环
		if (-MAX_TIME == _orderlytime[v])
			break;//
		//将时间标签时间转为秒为单位
		int i = _orderlytime[v]/1000;//
		//四舍五入
		if (_orderlytime[v]%1000 >= 500)
			i++;//
		//若时间标签时间与歌曲当前时间相等则选中相应歌词
		if (musictime == i)
			SetDlgItemText(hwnd,IDC_MINILRC,_entity[_index[v]].s);//
	}
}//

//检测吸附窗口及刷新歌词定时器回调函数
void CALLBACK MLTimer(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime)
{
	//更新歌词
	_GetSetLrc(hwnd,milphwnd);//
}//


//获取程序路径函数
TCHAR * mlGetWay(TCHAR * appPath)
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
FILE * mlopenSetFile()
{
	FILE * FList;
	TCHAR file[MAX_PATH];
	wsprintf(file,"%s配置文件(请勿删除或更改).dat",mlGetWay(file));
	//以读写方式打开设置文件，若设置文件不存在则返回NULL
	fopen_s(&FList,file,"r+");//
	return FList;
}

void GetIntFromLable(char* lable,int* num,char*p,char*q)
{
	//若找到“_cr”标签
	if(!strcmp(lable,q))
	{
		q = p+1;
		*num = atoi(q);
	}//
}

//读取颜色函数
void GetsetColor(int*r,int*g,int*b)
{
	FILE *FList;
	//若存在设置文件
	if (NULL != (FList=mlopenSetFile()))
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
				GetIntFromLable("_cr",r,p,q);
				GetIntFromLable("_cg",g,p,q);
				GetIntFromLable("_cb",b,p,q);
				p++;
				q = p;
			}
		}
		fclose(FList);
	}//
	return;
}

#endif