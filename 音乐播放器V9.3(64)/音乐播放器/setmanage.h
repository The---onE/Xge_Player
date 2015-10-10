/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ȫ�ֱ��������ô��ں�������
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef setmanage_h
#define setmanage_h

HWND setphwnd;
TCHAR SourceSet[MAX_PATH*2];
TCHAR NewSet[MAX_PATH*2];

//��ȡ����·������
TCHAR * _GetWay(TCHAR * appPath)
{
	//��ȡ����·�����ļ���
	int appl=GetModuleFileName(NULL,appPath,256);//
	//�Ӻ���ǰ����\�滻Ϊ\0��ȥ���ļ���
	for(int i=appl-1;i>=0;i--)
		if(appPath[i]!='\\')
			appPath[i]='\0';//
		else
			break;
		return appPath;
}//

//���ؿɶ������ļ�����
FILE * _openSetFile()
{
	FILE * FList;
	TCHAR file[MAX_PATH];
	wsprintf(file,"%s�����ļ�(����ɾ�������).dat",_GetWay(file));
	//�Զ�д��ʽ�������ļ����������ļ��������򷵻�NULL
	fopen_s(&FList,file,"r+");//
	return FList;
}//

//���ؿ�д�����ļ�����
FILE * _saveSetFile()
{
	FILE * FList;
	TCHAR file[MAX_PATH];
	wsprintf(file,"%s�����ļ�(����ɾ�������).dat",_GetWay(file));
	//�Զ�д��ʽ�򿪻򴴽��б��ļ�
	fopen_s(&FList,file,"w+");//
	return FList;
}//

//��ȡ��ǩ��Ӧ��ֵ����
void getLable(char* lable,int* num)
{
	FILE *FList;
	//�����������ļ�
	if (NULL != (FList=_openSetFile()))
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
				//���ҵ���Ӧ��ǩ
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

//����Ҫ���������
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

//���������ļ�����
void setFile()
{
	FILE *FList;
	//��˯�ߵȴ�ʱ��༭��Ϊ��
	if (NULL != (FList=_saveSetFile()))
	{
		//����_st|ʱ�� ��ʽ�����������ļ�
		fprintf(FList,"%s",NewSet);//
		fclose(FList);
	}
}//

#endif