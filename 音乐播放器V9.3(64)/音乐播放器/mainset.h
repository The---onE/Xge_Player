/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�����ڶ�ȡ�����ļ���������
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef mainset_h
#define mainset_h

//��ȡ����·������
TCHAR * GetWay(TCHAR * appPath)
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
FILE * openSetFile()
{
	FILE * FList;
	TCHAR file[MAX_PATH];
	wsprintf(file,"%s�����ļ�(����ɾ�������).dat",GetWay(file));
	//�Զ�д��ʽ�������ļ����������ļ��������򷵻�NULL
	fopen_s(&FList,file,"r+");//
	return FList;
}

//��ȡ˯�ߵȴ�ʱ�亯��
int SleepTime()
{
	FILE *FList;
	//�������������ļ���Ĭ��ʱ��Ϊ30����
	int t = 30;//
	//�����������ļ�
	if (NULL != (FList=openSetFile()))
	{
		TCHAR Fc[MAX_PATH*2];
		ZeroMemory(Fc,sizeof(Fc));
		fgets(Fc,MAX_PATH*2,FList);
		TCHAR *p = Fc;
		TCHAR *q = Fc;
		//������ǰ����|������ǩ�Ƿ�Ϊ��_st��
		while (*p)
		{
			p++;
			if ('|' == *p)
			{
				*p = '\0';
				//���ҵ���_st����ǩ
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
	//��������Ϊ��λ��˯�ߵȴ�ʱ��
	return t*60;//
}

#endif