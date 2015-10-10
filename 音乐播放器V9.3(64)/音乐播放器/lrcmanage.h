/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//全局变量及歌词窗口函数定义
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _lrcmanage_H
#define _lrcmanage_H

//定义主窗口句柄全局变量
HWND lrcphwnd;//
//定义是否鼠标离开窗口时应收缩参数
BOOL shouldHide = FALSE;//
//定义是否已收缩参数
BOOL isHide = FALSE;//
//定时是否跟随歌曲更新歌词参数
BOOL isFollow = TRUE;//

//定义时间标签时间全局变量
int time[LINE][SAME];//
//定义根据时间排序的歌词顺序全局变量
int index[LINE*SAME];//
//定义已排好序的时间全局变量
int orderlytime[LINE*SAME];//
//定义时间补偿时值全局变量
int offset = 0;//

//定义窗口坐标全局变量
RECT size;//

//定义歌词结构体
struct
{	
	char *s;
}text[LINE*SAME];//

//标签处理函数
BOOL manageTag(HWND phwnd,char *q,int j,int k)
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
			time[j][k] = ff*10 + ss*1000 + mm*60000 + 1;//
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
			offset = -atoi(q);
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
void sort()
{
    //找出所有时间标签的最小值
	//最小值、次小值初始化为时间标签最大值+1
	int min = MAX_TIME,smin = MAX_TIME;
	//不断寻找，若比假定值小则替换，直到确定最小值
    for (int x=0 ; x<LINE ; x++)
	{
		for (int y=0 ; y<SAME ; y++)
		{
			if (time[x][y] < min)
			{
				//使原标签+1后自减以便使相同时间标签不被略过
				min = --time[x][y];//
				//将歌词序号存入全局变量
				index[0] = x;//
				//相应时间存入全局变量，orderlytime[i]的时间对应第index[i]（的值）段歌词
				orderlytime[0] = min;//
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
				if (time[x][y]>min && time[x][y]<=smin)
				{
					//使原标签+1后自减以便使相同时间表情不被略过
					smin = --time[x][y];//
					//将歌词序号存入全局变量
					index[in] = x;//
					//相应时间存入全局变量，orderlytime[i]的时间对应第index[i]（的值）段歌词
					orderlytime[in] = smin;//
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
BOOL manageLrc(HWND phwnd,char * c)
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
					if (!manageTag(phwnd,q,j,k))//
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
									if (!manageTag(phwnd,q,j,k))//
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
							text[j].s = q;//
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
void GetSetLrc(HWND hwnd,HWND phwnd,BOOL isrefresh)
{
	TCHAR mp3[MAX_PATH],lrc[MAX_PATH],oldname[MAX_PATH],*s,*t;
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
		//打开歌曲同名lrc文件
		wsprintf(lrc,"%s.lrc",t);//
		//指针指向最近“\”之后字符，以便得到歌曲名
		while (*s != '\\')
		{
			s--;
		}
		s++;//

		//若歌曲列表中有内容
		if (ListBox_GetCount(GetDlgItem(hwnd,IDC_LRCList)))//
			//读取第一项作为原歌曲名
			ListBox_GetText(GetDlgItem(hwnd,IDC_LRCList),0,oldname);//
		//若原歌曲名与新歌曲名相同则不再继续执行
		if (!isrefresh && !strcmp(s,oldname))
			return;//
		//清空列表
		SendDlgItemMessage(hwnd,IDC_LRCList,LB_RESETCONTENT,0,0);//
		//将新歌曲名加入歌词列表
		ListBox_InsertString(GetDlgItem(hwnd,IDC_LRCList),-1,s);//
	}
	//若无正在播放歌曲
	else
	{
		//若歌曲列表中有内容
		if (ListBox_GetCount(GetDlgItem(hwnd,IDC_LRCList)))//
			//读取第一项作为原歌曲名
			ListBox_GetText(GetDlgItem(hwnd,IDC_LRCList),0,oldname);//
		//若原正在播放歌曲也不存在则不再继续执行
		if (!isrefresh && !strcmp("歌词不存在",oldname))
			return;//
		//清空列表		
		SendDlgItemMessage(hwnd,IDC_LRCList,LB_RESETCONTENT,0,0);//
	}//

	FILE *f;
	//尝试打开lrc文件，若打开成功
	if(!fopen_s(&f,lrc,"r"))
	{
		//初始化时间标签时间全局变量，防止未赋值数据影响排序
		for (int x=0 ; x<LINE ; x++)
		{
			for (int y=0 ; y<SAME ; y++)
			{
				time[x][y] = MAX_TIME+1;	
			}
		}//
		//初始化全局变量，以便通过-1确定边界
		for (int z=0 ; z<LINE*SAME ; z++)
		{
			index[z] = -1;
			orderlytime[z] = -MAX_TIME;
		}//
		
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
			ListBox_InsertString(GetDlgItem(hwnd,IDC_LRCList),-1,"歌词过长或内存不足，请按【刷新】重试");
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
		BOOL enough = manageLrc(phwnd,c);//

		//排序时间标签
		sort();//

		//若有补偿时值，为所有时间标签时间添加补偿时值
		if (offset)
		{
			for (int u=0 ; u<LINE*SAME ; u++)
			{
				if (orderlytime[u] != -MAX_TIME)
					orderlytime[u] += offset;
				if (orderlytime[u] < 0)
					orderlytime[u] = 0;
			}
		}//
		//设置补偿时值为0以便下一首歌使用
		offset = 0;//
		
		TCHAR ti_ar[MAX_PATH],ti[MAX_PATH],ar[MAX_PATH],al[MAX_PATH];
		//读取主窗口信息编辑框
		GetDlgItemText(phwnd,IDC_Title,ti,sizeof(ti));
		GetDlgItemText(phwnd,IDC_Artist,ar,sizeof(ar));
		GetDlgItemText(phwnd,IDC_Album,al,sizeof(al));//
		//若存在歌手信息
		if (*ar!='\0')//
		{
			wsprintf(ti_ar,"%s-%s",ti,ar);
			//若存在专辑信息
			if (*al != '\0')//
			{
				wsprintf(ti_ar,"%s-%s",ti_ar,al);
			}
			//将歌曲名（-歌手-专辑）添加到歌曲列表
			ListBox_InsertString(GetDlgItem(hwnd,IDC_LRCList),-1,ti_ar);//
		}
		//若不存在歌手信息
		else
			//添加一空行
			ListBox_InsertString(GetDlgItem(hwnd,IDC_LRCList),-1,"");////
		//添加一空行
		ListBox_InsertString(GetDlgItem(hwnd,IDC_LRCList),-1,"");//

		//按时间标签时间顺序将歌词加入歌词列表
		for (int w=0 ; w<LINE*SAME ; w++)
		{
			if (index[w] != -1)
				ListBox_InsertString(GetDlgItem(hwnd,IDC_LRCList),-1,text[index[w]].s);
		}//
		
		if (!enough)
			ListBox_InsertString(GetDlgItem(hwnd,IDC_LRCList),-1,"歌词过长，无法显示超长部分");//

		//选中第4选项（第1句歌词）
		ListBox_SetCurSel(GetDlgItem(hwnd,IDC_LRCList),3);//

		//关闭lrc文件
		fclose(f);//
		//回收lrc文档空间
		free(c);//
	}//
	//若lrc文件不存在或打开失败
	else
	{
		//向歌词列表添加不存在信息
		ListBox_InsertString(GetDlgItem(hwnd,IDC_LRCList),-1,"歌词不存在");//
	}
}//

//歌词跟随歌曲进度函数
void select(HWND hwnd,int musictime)
{
	for (int v=0 ; v<LINE*SAME ; v++)
	{
		//若循环至边界则退出循环
		if (-MAX_TIME == orderlytime[v])
			break;//
		//将时间标签时间转为秒为单位
		int i = orderlytime[v]/1000;//
		//四舍五入
		if (orderlytime[v]%1000 >= 500)
			i++;//
		//若时间标签时间与歌曲当前时间相等则选中相应歌词
		if (musictime == i)
			ListBox_SetCurSel(GetDlgItem(hwnd,IDC_LRCList),v+3);//
	}
}//

//检测吸附窗口及刷新歌词定时器回调函数
void CALLBACK LRCTimer(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime)
{
	POINT pt;
	//获得鼠标坐标
	GetCursorPos(&pt);//
	RECT position;
	//获得窗口边缘坐标
	GetWindowRect(hwnd,&position);//
	//若鼠标不在窗口内
	if(pt.x>=position.right || pt.x<=position.left || pt.y>=position.bottom)//
		//若窗口已吸附边缘
		if (TRUE == shouldHide)//
			//若窗口未收缩
			if(FALSE == isHide)//
			{
				//收缩窗口
				SetWindowPos(hwnd,HWND_TOPMOST,0,0,565,NEAR_HEIGHT,SWP_NOMOVE);//
				isHide = TRUE;
			}
	//更新歌词
	GetSetLrc(hwnd,lrcphwnd,FALSE);//
}//

#endif