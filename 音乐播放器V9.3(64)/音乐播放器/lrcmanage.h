/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ȫ�ֱ�������ʴ��ں�������
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _lrcmanage_H
#define _lrcmanage_H

//���������ھ��ȫ�ֱ���
HWND lrcphwnd;//
//�����Ƿ�����뿪����ʱӦ��������
BOOL shouldHide = FALSE;//
//�����Ƿ�����������
BOOL isHide = FALSE;//
//��ʱ�Ƿ����������¸�ʲ���
BOOL isFollow = TRUE;//

//����ʱ���ǩʱ��ȫ�ֱ���
int time[LINE][SAME];//
//�������ʱ������ĸ��˳��ȫ�ֱ���
int index[LINE*SAME];//
//�������ź����ʱ��ȫ�ֱ���
int orderlytime[LINE*SAME];//
//����ʱ�䲹��ʱֵȫ�ֱ���
int offset = 0;//

//���崰������ȫ�ֱ���
RECT size;//

//�����ʽṹ��
struct
{	
	char *s;
}text[LINE*SAME];//

//��ǩ������
BOOL manageTag(HWND phwnd,char *q,int j,int k)
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
			time[j][k] = ff*10 + ss*1000 + mm*60000 + 1;//
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
			offset = -atoi(q);
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
void sort()
{
    //�ҳ�����ʱ���ǩ����Сֵ
	//��Сֵ����Сֵ��ʼ��Ϊʱ���ǩ���ֵ+1
	int min = MAX_TIME,smin = MAX_TIME;
	//����Ѱ�ң����ȼٶ�ֵС���滻��ֱ��ȷ����Сֵ
    for (int x=0 ; x<LINE ; x++)
	{
		for (int y=0 ; y<SAME ; y++)
		{
			if (time[x][y] < min)
			{
				//ʹԭ��ǩ+1���Լ��Ա�ʹ��ͬʱ���ǩ�����Թ�
				min = --time[x][y];//
				//�������Ŵ���ȫ�ֱ���
				index[0] = x;//
				//��Ӧʱ�����ȫ�ֱ�����orderlytime[i]��ʱ���Ӧ��index[i]����ֵ���θ��
				orderlytime[0] = min;//
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
				if (time[x][y]>min && time[x][y]<=smin)
				{
					//ʹԭ��ǩ+1���Լ��Ա�ʹ��ͬʱ����鲻���Թ�
					smin = --time[x][y];//
					//�������Ŵ���ȫ�ֱ���
					index[in] = x;//
					//��Ӧʱ�����ȫ�ֱ�����orderlytime[i]��ʱ���Ӧ��index[i]����ֵ���θ��
					orderlytime[in] = smin;//
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
BOOL manageLrc(HWND phwnd,char * c)
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
					if (!manageTag(phwnd,q,j,k))//
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
									if (!manageTag(phwnd,q,j,k))//
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
							text[j].s = q;//
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
void GetSetLrc(HWND hwnd,HWND phwnd,BOOL isrefresh)
{
	TCHAR mp3[MAX_PATH],lrc[MAX_PATH],oldname[MAX_PATH],*s,*t;
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
		//�򿪸���ͬ��lrc�ļ�
		wsprintf(lrc,"%s.lrc",t);//
		//ָ��ָ�������\��֮���ַ����Ա�õ�������
		while (*s != '\\')
		{
			s--;
		}
		s++;//

		//�������б���������
		if (ListBox_GetCount(GetDlgItem(hwnd,IDC_LRCList)))//
			//��ȡ��һ����Ϊԭ������
			ListBox_GetText(GetDlgItem(hwnd,IDC_LRCList),0,oldname);//
		//��ԭ���������¸�������ͬ���ټ���ִ��
		if (!isrefresh && !strcmp(s,oldname))
			return;//
		//����б�
		SendDlgItemMessage(hwnd,IDC_LRCList,LB_RESETCONTENT,0,0);//
		//���¸������������б�
		ListBox_InsertString(GetDlgItem(hwnd,IDC_LRCList),-1,s);//
	}
	//�������ڲ��Ÿ���
	else
	{
		//�������б���������
		if (ListBox_GetCount(GetDlgItem(hwnd,IDC_LRCList)))//
			//��ȡ��һ����Ϊԭ������
			ListBox_GetText(GetDlgItem(hwnd,IDC_LRCList),0,oldname);//
		//��ԭ���ڲ��Ÿ���Ҳ���������ټ���ִ��
		if (!isrefresh && !strcmp("��ʲ�����",oldname))
			return;//
		//����б�		
		SendDlgItemMessage(hwnd,IDC_LRCList,LB_RESETCONTENT,0,0);//
	}//

	FILE *f;
	//���Դ�lrc�ļ������򿪳ɹ�
	if(!fopen_s(&f,lrc,"r"))
	{
		//��ʼ��ʱ���ǩʱ��ȫ�ֱ�������ֹδ��ֵ����Ӱ������
		for (int x=0 ; x<LINE ; x++)
		{
			for (int y=0 ; y<SAME ; y++)
			{
				time[x][y] = MAX_TIME+1;	
			}
		}//
		//��ʼ��ȫ�ֱ������Ա�ͨ��-1ȷ���߽�
		for (int z=0 ; z<LINE*SAME ; z++)
		{
			index[z] = -1;
			orderlytime[z] = -MAX_TIME;
		}//
		
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
			ListBox_InsertString(GetDlgItem(hwnd,IDC_LRCList),-1,"��ʹ������ڴ治�㣬�밴��ˢ�¡�����");
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
		BOOL enough = manageLrc(phwnd,c);//

		//����ʱ���ǩ
		sort();//

		//���в���ʱֵ��Ϊ����ʱ���ǩʱ����Ӳ���ʱֵ
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
		//���ò���ʱֵΪ0�Ա���һ�׸�ʹ��
		offset = 0;//
		
		TCHAR ti_ar[MAX_PATH],ti[MAX_PATH],ar[MAX_PATH],al[MAX_PATH];
		//��ȡ��������Ϣ�༭��
		GetDlgItemText(phwnd,IDC_Title,ti,sizeof(ti));
		GetDlgItemText(phwnd,IDC_Artist,ar,sizeof(ar));
		GetDlgItemText(phwnd,IDC_Album,al,sizeof(al));//
		//�����ڸ�����Ϣ
		if (*ar!='\0')//
		{
			wsprintf(ti_ar,"%s-%s",ti,ar);
			//������ר����Ϣ
			if (*al != '\0')//
			{
				wsprintf(ti_ar,"%s-%s",ti_ar,al);
			}
			//����������-����-ר������ӵ������б�
			ListBox_InsertString(GetDlgItem(hwnd,IDC_LRCList),-1,ti_ar);//
		}
		//�������ڸ�����Ϣ
		else
			//���һ����
			ListBox_InsertString(GetDlgItem(hwnd,IDC_LRCList),-1,"");////
		//���һ����
		ListBox_InsertString(GetDlgItem(hwnd,IDC_LRCList),-1,"");//

		//��ʱ���ǩʱ��˳�򽫸�ʼ������б�
		for (int w=0 ; w<LINE*SAME ; w++)
		{
			if (index[w] != -1)
				ListBox_InsertString(GetDlgItem(hwnd,IDC_LRCList),-1,text[index[w]].s);
		}//
		
		if (!enough)
			ListBox_InsertString(GetDlgItem(hwnd,IDC_LRCList),-1,"��ʹ������޷���ʾ��������");//

		//ѡ�е�4ѡ���1���ʣ�
		ListBox_SetCurSel(GetDlgItem(hwnd,IDC_LRCList),3);//

		//�ر�lrc�ļ�
		fclose(f);//
		//����lrc�ĵ��ռ�
		free(c);//
	}//
	//��lrc�ļ������ڻ��ʧ��
	else
	{
		//�����б���Ӳ�������Ϣ
		ListBox_InsertString(GetDlgItem(hwnd,IDC_LRCList),-1,"��ʲ�����");//
	}
}//

//��ʸ���������Ⱥ���
void select(HWND hwnd,int musictime)
{
	for (int v=0 ; v<LINE*SAME ; v++)
	{
		//��ѭ�����߽����˳�ѭ��
		if (-MAX_TIME == orderlytime[v])
			break;//
		//��ʱ���ǩʱ��תΪ��Ϊ��λ
		int i = orderlytime[v]/1000;//
		//��������
		if (orderlytime[v]%1000 >= 500)
			i++;//
		//��ʱ���ǩʱ���������ǰʱ�������ѡ����Ӧ���
		if (musictime == i)
			ListBox_SetCurSel(GetDlgItem(hwnd,IDC_LRCList),v+3);//
	}
}//

//����������ڼ�ˢ�¸�ʶ�ʱ���ص�����
void CALLBACK LRCTimer(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime)
{
	POINT pt;
	//����������
	GetCursorPos(&pt);//
	RECT position;
	//��ô��ڱ�Ե����
	GetWindowRect(hwnd,&position);//
	//����겻�ڴ�����
	if(pt.x>=position.right || pt.x<=position.left || pt.y>=position.bottom)//
		//��������������Ե
		if (TRUE == shouldHide)//
			//������δ����
			if(FALSE == isHide)//
			{
				//��������
				SetWindowPos(hwnd,HWND_TOPMOST,0,0,565,NEAR_HEIGHT,SWP_NOMOVE);//
				isHide = TRUE;
			}
	//���¸��
	GetSetLrc(hwnd,lrcphwnd,FALSE);//
}//

#endif