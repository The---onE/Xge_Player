/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�б����ļ�������ӡ�ɾ�����ƶ�����������
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef file_h
#define file_h

//���ļ�����
void openf(HWND hwnd)
{
	HWND MList = GetDlgItem(hwnd,IDC_MList);  
	TCHAR * p;
	char szFile[MAX_PATH * 200];
	//��ʼ�����ļ����ڽṹ��,��ϸ��Ϣ����OPENFILENAME
	OPENFILENAME ofn;
	ZeroMemory(&ofn,sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = TEXT('\0');
	ofn.nMaxFile = sizeof(szFile);
	//���ÿɴ��ļ�����
	ofn.lpstrFilter = TEXT("Music(*.mp3;*.wma)\0*.mp3;*.wma\0MP3(*.mp3)\0*.mp3\0WMA(*.wma)\0*.wma\0ALL(*.*)\0*.*\0");//
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.hwndOwner = hwnd;
	//���ô��ڷ��
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT ;////
	//����ѡ���ļ�
	if (GetOpenFileName(&ofn))
	{
		p = szFile + ofn.nFileOffset;
		BOOL j = FALSE;
		//��ѡ�ж���ļ��򲻶϶�ȡ
		while (*p)//
		{
			//��ѡ�е����ļ�
			if('\0' != *(szFile + ofn.nFileOffset - 1))
			{
				ListBox_InsertString(MList,-1,szFile);
				for (int i = 0 ; i < ListBox_GetCount(MList)-1 ; i++)
				{
					TCHAR List[MAX_PATH];
					ListBox_GetText(MList,i,List);
					//������ͬ���ļ���ɾ�����ļ�
					if (0 == strcmp(List,szFile))
					{
						ListBox_DeleteString(MList,(ListBox_GetCount(MList)-1));//
						//����ͬ���ļ�Ϊѡ��
						ListBox_SetCurSel(MList,i);
						j = TRUE;
						break;
					}//
				}
				if (!j)
				{
					//����ͬ���ļ����������ļ�Ϊѡ��
					ListBox_SetCurSel(MList,ListBox_GetCount(MList)-1);//
				}//
				//��ѡ�е����ļ����ٶ�ȡ
				break;//
			}//

			//��ѡ�ж���ļ�
			//�ַ�����ʽ: �ļ�Ŀ¼\0�ļ���\0�ļ���\0����\0�ļ���\0\0 //
			else
			{
				TCHAR lj[MAX_PATH];
				//������\�滻\0�����ļ�Ŀ¼���ļ���
				wsprintf(lj,"%s\\%s",szFile,p);//
				//����ļ�
				ListBox_InsertString(MList,-1,lj);//
				for (int i = 0 ; i < ListBox_GetCount(MList)-1 ; i++)
				{
					TCHAR List[MAX_PATH];
					ListBox_GetText(MList,i,List);
					//������ͬ���ļ���ɾ�����ļ�
					if (0 == strcmp(List,lj))
					{
						ListBox_DeleteString(MList,(ListBox_GetCount(MList)-1));//
						//����ͬ���ļ�Ϊѡ��
						ListBox_SetCurSel(MList,i);//
						j = TRUE;
						break;
					}
				}
				if (!j)
				{
					//����ͬ���ļ����������ļ�Ϊѡ��
					ListBox_SetCurSel(MList,ListBox_GetCount(MList)-1);//
				}
				p += lstrlen(p) +1;
			}//
		}
	}//
}//

//ɾ������
void Del(HWND hwnd)
{
	TCHAR Choose1[MAX_PATH];
	GetDlgItemText(hwnd,IDC_EDITNOW,Choose1,sizeof(Choose1));
	HWND MList = GetDlgItem(hwnd,IDC_MList);
	int i = ListBox_GetCurSel(MList);
	TCHAR Choose2[MAX_PATH];
	ListBox_GetText(MList,i,Choose2);
	//��ɾ������Ϊ���ڲ��Ÿ�������ֹͣ��ɾ��
	if (0 == strcmp(Choose1,Choose2))
	{
		Stop(hwnd);
		ListBox_DeleteString(MList,i);
	}//
	//��ɾ�����������ڲ��Ÿ�����ֱ��ɾ��
	else
		ListBox_DeleteString(MList,i);//
}//

//���ƺ���
void Up(HWND hwnd)
{
	HWND MList = GetDlgItem(hwnd,IDC_MList);
	int i = ListBox_GetCurSel(MList);
	int max = ListBox_GetCount(MList);
	TCHAR Choose1[MAX_PATH],Choose2[MAX_PATH];
	ListBox_GetText(MList,i,Choose1);
	if(i-1 >= 0)
		ListBox_GetText(MList,i-1,Choose2);
	//����ѡ�и�����ѡ�и�������һ�׸���
	ListBox_DeleteString(MList,i);
	if(i-1 >= 0)
		ListBox_DeleteString(MList,i-1);
	//��ѡ�и������б��һ�׸�������ѡ�и�������һ�׸�������
	if(i-1 >= 0)
	{
		ListBox_InsertString(MList,i-1,Choose2);
		ListBox_InsertString(MList,i-1,Choose1);
	}//
	//��ѡ�и���Ϊ�б��һ�׸�������ѡ�и��������б����һ��֮��
	else
		ListBox_InsertString(MList,max-1,Choose1);////
	//����ԭѡ�и�����Ϊѡ��
	if(i-1 >= 0)
		ListBox_SetCurSel(MList,i-1);
	else
		ListBox_SetCurSel(MList,max-1);//
	
	//���¼��㵱ǰ���Ÿ���λ��
	ListNow(hwnd);//
}//

//���ƺ���
void Down(HWND hwnd)
{
	HWND MList = GetDlgItem(hwnd,IDC_MList);
	int i = ListBox_GetCurSel(MList);
	int max = ListBox_GetCount(MList);
	TCHAR Choose1[MAX_PATH],Choose2[MAX_PATH];
	ListBox_GetText(MList,i,Choose1);
	if(i <= max-2)
		ListBox_GetText(MList,i+1,Choose2);
	//����ѡ�и�����ѡ�и�������һ�׸���
	ListBox_DeleteString(MList,i);
	if(i <= max-2)
		ListBox_DeleteString(MList,i);
	//��ѡ�и������б����һ�׸�������ѡ�и�������һ�׸�������
	if(i <= max-2)
	{
		ListBox_InsertString(MList,i,Choose1);
		ListBox_InsertString(MList,i,Choose2);
	}//
	//��ѡ�и���Ϊ�б����һ�׸�������ѡ�и��������б��һ�׸���֮ǰ
	else
		ListBox_InsertString(MList,0,Choose1);////
	//����ԭѡ�и�����Ϊѡ��
	if(i <= max-2)
		ListBox_SetCurSel(MList,i+1);
	else
		ListBox_SetCurSel(MList,0);//

	//���¼��㵱ǰ���Ÿ���λ��
	ListNow(hwnd);//
}//

//���ؿ�д�б��ļ�����
FILE * SList(int i)
{
	FILE * FList;
	TCHAR file[MAX_PATH];
	wsprintf(file,"%sX�����ֲ������б�%d(ɾ�����б���ʧ).dat",GetWay(file),i);
	//�Զ�д��ʽ�򿪻򴴽��б��ļ�
	fopen_s(&FList,file,"w+");//
	return FList;
}//

//���ؿɶ��б��ļ�����
FILE * OFile(int i)
{
	FILE * FList;
	TCHAR file[MAX_PATH];
	wsprintf(file,"%sX�����ֲ������б�%d(ɾ�����б���ʧ).dat",GetWay(file),i);
	//�Զ�д��ʽ���б��ļ������б��ļ��������򷵻�NULL
	fopen_s(&FList,file,"r+");//
	return FList;
}//

//�����б���
void saveList(HWND hwnd)
{
	FILE * FList;
	int i = ComboBox_GetCurSel(GetDlgItem(hwnd,IDC_CSL))+1;
	TCHAR asksave[20];
	wsprintf(asksave,"ȷ��Ҫ�������б�%d��",i);
	//����ȷ�϶Ի�����ȷ���򱣴��б�
	int yn = MessageBox(hwnd,asksave,TEXT("�����б�"),MB_YESNO|MB_ICONQUESTION);
	if (IDYES == yn)
	{
		FList = SList(i);
		HWND MList = GetDlgItem(hwnd,IDC_MList);
		TCHAR Choose[MAX_PATH];
		//�ԣ��ļ�·��\�ļ���|�ļ�·��\�ļ���|����|�ļ�·��\�ļ���| ��ʽ�������б��ļ�
		for (int t = 0;t < ListBox_GetCount(MList);t++)
		{
			ListBox_GetText(MList,t,Choose);
			fprintf(FList,"%s|",Choose);
		}//
		fclose(FList);
		MessageBox(hwnd,"�����б�ɹ���","�ɹ�",MB_OK|MB_ICONINFORMATION);
	}//
	else
		return;
}//

//���б���
void openList(HWND hwnd)
{
	FILE *FList;
	HWND MList = GetDlgItem(hwnd,IDC_MList);
	int i = ComboBox_GetCurSel(GetDlgItem(hwnd,IDC_COL))+1;
	TCHAR askopen[20];
	wsprintf(askopen,"ȷ��Ҫ���б�%d��",i);
	//����ȷ�϶Ի�����ȷ�����ȡ�б�
	int yn = MessageBox(hwnd,askopen,TEXT("���б�"),MB_YESNO|MB_ICONQUESTION);
	if (IDYES == yn)
	{
		//���б����ڣ���ʾ������
		if (NULL == (FList = OFile(i)))
			MessageBox(hwnd,TEXT("�б�����"),"",MB_OK|MB_ICONSTOP);//
		else
		{
			TCHAR Fc[MAX_PATH*500];
			ZeroMemory(Fc,sizeof(Fc));
			fgets(Fc,MAX_PATH*500,FList);
			TCHAR *p = Fc;
			TCHAR *q = Fc;
			//��|�ָ����϶�ȡ�ļ�·��
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
						//������ͬ���ļ���ɾ�����ļ�
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
			//���¼����б�����
			ListTotal(hwnd);
			ListNow(hwnd);//
		}
	}
	else
		return;		
}//

#endif