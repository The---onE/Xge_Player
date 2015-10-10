/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ȫ�ֱ�������ʴ��ں�������
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _mlmanage_H
#define _mlmanage_H

//���������ھ��ȫ�ֱ���
HWND milphwnd;//

char name[MAX_PATH] = "";
//����ʱ���ǩʱ��ȫ�ֱ���
int _time[LINE][SAME];//
//�������ʱ������ĸ��˳��ȫ�ֱ���
int _index[LINE*SAME];//
//�������ź����ʱ��ȫ�ֱ���
int _orderlytime[LINE*SAME];//
//����ʱ�䲹��ʱֵȫ�ֱ���
int _offset = 0;//

//�����ʽṹ��
struct
{	
	char *s;
}_text[LINE*SAME];//

//�����ʽṹ��
struct
{	
	char s[MAX_LENGTH];
}_entity[LINE*SAME];//

//��ʼ����������
void Initialization()
{
	//��ʼ��ʱ���ǩʱ��ȫ�ֱ�������ֹδ��ֵ����Ӱ������
	for (int x=0 ; x<LINE ; x++)
	{
		for (int y=0 ; y<SAME ; y++)
		{
			_time[x][y] = MAX_TIME+1;	
		}
	}//
	//��ʼ��ȫ�ֱ������Ա�ͨ��-1ȷ���߽�
	for (int z=0 ; z<LINE*SAME ; z++)
	{
		_index[z] = -1;
		_orderlytime[z] = -MAX_TIME;
	}//
}
//
//��ǩ������
BOOL _manageTag(HWND phwnd,char *q,int j,int k)
{
	//��Ϊʱ���ǩ
	if (*q>='0' && *q<='9')
	{
		//���������ַ�ֹͣ��ȡ����ȡ������
		int mm = atoi(q);//
		//ʹָ��ָ��:��֮��
		for (int x=0 ; x<2 ; x++)
		{
			q++;
			if (':' == *q)
			{
				q++;
				break;
			}
		}//
		//���������ַ�ֹͣ��ȡ����ȡ����
		int ss = atoi(q);//
		//ʹָ��ָ��:����.��֮��
		for (int y=0 ; y<2 ; y++)
		{
			q++;
			if ('.'==*q || ':'==*q)
			{
				q++;
				break;
			}
		}//
		//���������ַ�ֹͣ��ȡ����ȡ������
		int ff = atoi(q);//
		//��δԽ��
		if(k<SAME)
		{
			//��ʱ���ǩ�Ժ���Ϊ��λ��������
			_time[j][k] = ff*10 + ss*1000 + mm*60000 + 1;//
		}//
		//��ʱ���ǩ
		return TRUE;//
	}//
	//��Ϊoffset����ʱֵ��ǩ
	else if (('o'==q[0]||'O'==q[0]) && ('f'==q[1]||'F'==q[1]) && ('f' == q[2]||'F'==q[2]) && \
			 ('s'==q[3]||'S'==q[3]) && ('e'==q[4]||'E'==q[4]) && ('t'==q[5]||'T'==q[5]) && (':' == q[6]))
	{
		//ָ��ָ��:��֮��
		q += 7;
		//����:��֮��Ϊ���ֻ�������
		if ('-'==*q || '+'==*q || (*q>='0' && *q<='9'))
		{
			//���������ַ�ֹͣ��ȡ����ȡ����ʱֵ������ȫ�ֱ���
			_offset = -atoi(q);
		}//
		//��ʱ���ǩ
		return FALSE;//
	}//
	//��Ϊti�����ǩ
	else if (('t'==q[0]||'T'==q[0]) && ('i'==q[1]||'I'==q[1]) && (':' == q[2]))
	{
		//ָ��ָ��:��֮��
		q += 3;
		TCHAR ti[MAX_PATH];
		GetDlgItemText(phwnd,IDC_Title,ti,sizeof(ti));
		//�������ļ���ID3������Ϣ�򽫱�����������ڸ������༭��
		if ('\0' == *ti)
			SetDlgItemText(phwnd,IDC_Title,q);//
		//��ʱ���ǩ
		return FALSE;//
	}//
	//��Ϊar���ֱ�ǩ
	else if (('a'==q[0]||'A'==q[0]) && ('r'==q[1]||'R'==q[1]) && (':' == q[2]))
	{
		//ָ��ָ��:��֮��
		q += 3;
		TCHAR ar[MAX_PATH];
		GetDlgItemText(phwnd,IDC_Artist,ar,sizeof(ar));
		//�������ļ���ID3������Ϣ�򽫸��������������ڸ��ֱ༭��
		if ('\0' == *ar)
			SetDlgItemText(phwnd,IDC_Artist,q);//
		//��ʱ���ǩ
		return FALSE;//
	}//
	//��Ϊalר����ǩ
	else if (('a'==q[0]||'A'==q[0]) && ('l'==q[1]||'L'==q[1]) && (':' == q[2]))
	{
		//ָ��ָ��:��֮��
		q += 3;
		TCHAR al[MAX_PATH];
		GetDlgItemText(phwnd,IDC_Album,al,sizeof(al));
		//�������ļ���ID3ר����Ϣ��ר�������������ڸ��ֱ༭��
		if ('\0' == *al)
			SetDlgItemText(phwnd,IDC_Album,q);//
		//��ʱ���ǩ
		return FALSE;//
	}//
	else
		return FALSE;
}//

//������
void _sort()
{
    //�ҳ�����ʱ���ǩ����Сֵ
	//��Сֵ����Сֵ��ʼ��Ϊʱ���ǩ���ֵ+1
	int min = MAX_TIME,smin = MAX_TIME;
	//����Ѱ�ң����ȼٶ�ֵС���滻��ֱ��ȷ����Сֵ
    for (int x=0 ; x<LINE ; x++)
	{
		for (int y=0 ; y<SAME ; y++)
		{
			if (_time[x][y] < min)
			{
				//ʹԭ��ǩ+1���Լ��Ա�ʹ��ͬʱ���ǩ�����Թ�
				min = --_time[x][y];//
				//�������Ŵ���ȫ�ֱ���
				_index[0] = x;//
				//��Ӧʱ�����ȫ�ֱ�����orderlytime[i]��ʱ���Ӧ��index[i]����ֵ���θ��
				_orderlytime[0] = min;//
			}
		}
	}////
	//Ϊʱ���ǩ����
	for (int in=1 ; in<LINE*SAME ; in++)
	{
		//����Ѱ�ң�����СֵΪ���ߣ����ȼٶ�ֵС�ұ���Сֵ�����滻��ֱ��ȷ����Сֵ
		for (int x=0 ; x<LINE ; x++)
		{
			for (int y=0 ; y<SAME ; y++)
			{
				if (_time[x][y]>min && _time[x][y]<=smin)
				{
					//ʹԭ��ǩ+1���Լ��Ա�ʹ��ͬʱ����鲻���Թ�
					smin = --_time[x][y];//
					//�������Ŵ���ȫ�ֱ���
					_index[in] = x;//
					//��Ӧʱ�����ȫ�ֱ�����orderlytime[i]��ʱ���Ӧ��index[i]����ֵ���θ��
					_orderlytime[in] = smin;//
				}
			}
		}//
		//����Сֵ��Ϊ���ߣ���Сֵ��Ϊ��Сֵ
		min = smin;//
		//�ٶ�ֵ��Ϊʱ���ǩ���ֵ+1
		smin = MAX_TIME;//
	}
}//

//����lrc�ĵ�����
BOOL _manageLrc(HWND phwnd,char * c)
{
	int j = -1,k = 0;
	char *p=c , *q=c , *r=c;
	//����ȡֱ��������־
	while (*p != EOF)
	{
		//��ָ��ָ���ǩ��
		if ('[' == *p)//
		{
			//��ͬ�������1
			j++;//
			if (j >= LINE)
				return FALSE;
			//ָ��ָ���ǩ����
			q = p+1;//
			//����ȡֱ������
			while (*p != '\n')
			{
				p++;
				//��ָ��ָ���ǩβ
				if (']' == *p)//
				{
					//��ͬ�����������0
					k = 0;//
					//����ǩβ��Ϊ\0
					*p = '\0';
					//�����ǩ
					//����ʱ���ǩ��ͬ�������1
					if (!_manageTag(phwnd,q,j,k))//
						j--;//
					//��������ȡ����Ƿ�����ͬ���
					p++;
					while (1)
					{
						//������ͬ���
						if('[' == *p)//
						{
							//ָ��ָ���ǩ����
							q = p+1;//
							//����ȡֱ������
							while (*p != '\n')
							{	
								p++;
								//��ָ��ָ���ǩβ
								if (']' == *p)
								{
									//��ͬ�������1
									k++;//
									//����ǩβ��Ϊ\0
									*p = '\0';//
									if (k >= SAME)
										break;
									//�����ǩ
									//����ʱ���ǩ��ͬ�������1
									if (!_manageTag(phwnd,q,j,k))//
										j--;//
									p++;
									break;
								}
							}//
						}
						//������ͬ���
						else//
						{
							//ָ��ָ���ǩβ֮��������
							r = (q = p);
							//��ʽṹ�����ַ�ָ��ָ��������
							_text[j].s = q;//
							//�����з���Ϊ\0����������ݽض�
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

//�򿪡��������ļ��������б��
void _GetSetLrc(HWND hwnd,HWND phwnd)
{
	TCHAR mp3[MAX_PATH],lrc[MAX_PATH],*s,*t;
	//��ȡ���������ڲ��Ÿ���
	GetDlgItemText(phwnd,IDC_EDITNOW,mp3,sizeof(mp3));//
	s = t = mp3;
	//�������ڲ��Ÿ���
	if (*mp3 != '\0')
	{
		//��ָ��ָ���ļ�����β
		while (*s != '\0')
		{
			s++;	
		}//
		//��ǰ��ȡֱ���̷�
		while (*s != ':')
		{
			s--;
			//����.����Ϊ\0�Ը�����չ��
			if ('.' == *s)
			{
				*s = '\0';
				break;
			}//
		
		}//

		//ָ��ָ�������\��֮���ַ����Ա�õ�������
		while (*s != '\\')
		{
			s--;
		}
		s++;//

		//��ԭ���������¸�������ͬ���ټ���ִ��
		if (!strcmp(s,name))
			return;//

		strcpy_s(name,s);
		SetDlgItemText(hwnd,IDC_MINILRC,name);
		//�򿪸���ͬ��lrc�ļ�
		wsprintf(lrc,"%s.lrc",t);//
	}
	//�������ڲ��Ÿ���
	else
	{
		if (!strcmp(name,""))
			return;
		Initialization();
		SetDlgItemText(hwnd,IDC_MINILRC,"");
		strcpy_s(name,"");
	}//

	FILE *f;
	//���Դ�lrc�ļ������򿪳ɹ�
	if(!fopen_s(&f,lrc,"r"))
	{
		Initialization();
		
		int i = 0;
		char temp;
		//���϶�ȡlrc�ļ�ֱ���ļ�����
		do
		{
			i++;
			temp = fgetc(f);
		}while (temp != EOF);//

		fseek(f,0L,SEEK_SET);
		char *c;
		//Ϊ�����ĵ�����ռ�
		int mc = 0;
		do
		{
			c = (char*)malloc(sizeof(char) * (i+5));
			mc++;
		}while (!c && mc<10);//
		if (10 == mc)
		{
			MessageBox(hwnd,"��ʹ������ڴ治�㣬�޷���ȡ���","����",MB_OK);
			return;
		}
		i = -1;
		//���϶�ȡlrc�ļ�ֱ���ļ�����
		do
		{
			i++;
			c[i] = fgetc(f);
		}while (c[i] != EOF);//

		//��ȡlrc�ĵ�
		BOOL enough = _manageLrc(phwnd,c);//

		//����ʱ���ǩ
		_sort();//

		//���в���ʱֵ��Ϊ����ʱ���ǩʱ����Ӳ���ʱֵ
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
		//���ò���ʱֵΪ0�Ա���һ�׸�ʹ��
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

		//�ر�lrc�ļ�
		fclose(f);//
		//����lrc�ĵ��ռ�
		free(c);//

	}//
	else
		Initialization();
}//

//��ʸ���������Ⱥ���
void _select(HWND hwnd,int musictime)
{
	for (int v=0 ; v<LINE*SAME ; v++)
	{
		//��ѭ�����߽����˳�ѭ��
		if (-MAX_TIME == _orderlytime[v])
			break;//
		//��ʱ���ǩʱ��תΪ��Ϊ��λ
		int i = _orderlytime[v]/1000;//
		//��������
		if (_orderlytime[v]%1000 >= 500)
			i++;//
		//��ʱ���ǩʱ���������ǰʱ�������ѡ����Ӧ���
		if (musictime == i)
			SetDlgItemText(hwnd,IDC_MINILRC,_entity[_index[v]].s);//
	}
}//

//����������ڼ�ˢ�¸�ʶ�ʱ���ص�����
void CALLBACK MLTimer(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime)
{
	//���¸��
	_GetSetLrc(hwnd,milphwnd);//
}//


//��ȡ����·������
TCHAR * mlGetWay(TCHAR * appPath)
{
	//��ȡ����·�����ļ���
	int appl=GetModuleFileName(NULL,appPath,256);
	//�Ӻ���ǰ����\�滻Ϊ\0��ȥ���ļ���
	for(int i=appl-1;i>=0;i--)
		if(appPath[i]!='\\')
 			appPath[i]='\0';//
 		else
 			break;
 	return appPath;
}//	

//���ؿɶ������ļ�����
FILE * mlopenSetFile()
{
	FILE * FList;
	TCHAR file[MAX_PATH];
	wsprintf(file,"%s�����ļ�(����ɾ�������).dat",mlGetWay(file));
	//�Զ�д��ʽ�������ļ����������ļ��������򷵻�NULL
	fopen_s(&FList,file,"r+");//
	return FList;
}

void GetIntFromLable(char* lable,int* num,char*p,char*q)
{
	//���ҵ���_cr����ǩ
	if(!strcmp(lable,q))
	{
		q = p+1;
		*num = atoi(q);
	}//
}

//��ȡ��ɫ����
void GetsetColor(int*r,int*g,int*b)
{
	FILE *FList;
	//�����������ļ�
	if (NULL != (FList=mlopenSetFile()))
	{
		TCHAR Fc[MAX_PATH*2];
		ZeroMemory(Fc,sizeof(Fc));
		fgets(Fc,MAX_PATH*2,FList);
		TCHAR *p = Fc;
		TCHAR *q = Fc;
		//������ǰ����|������ǩ
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