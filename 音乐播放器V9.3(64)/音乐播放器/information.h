/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��ȡMP3�ļ�����Ϣ��ID3V1��ID3V2����������
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef information_h
#define information_h

//����ID3V1�ṹ��洢������Ϣ
struct ID3v1
{
	//ͨ���ļ�������128�ֽں��Ƿ�Ϊ��TAG���ж��Ƿ�ID3V1��Ϣ
	//��ϸ��Ϣ����ID3
	char Header[4];//
	//�������⡢���֡�ר����Ϣ�̶�����Ϊ30
	char Title[31];
	char Artist[31];
	char Album[31];////
};// 

//��ȡID3V2��Ϣ����
void ID3V2(HWND hwnd,FILE * fp,ID3v1 ID3V1)
{
	TCHAR temp[5];
	int i = 0; 
	fseek(fp,10,SEEK_SET);
	//�������Ѱ�ҡ�TIT2����ǩ
	do
	{
		fread(temp,4,1,fp);
		temp[4] = '\0';
		fseek(fp,-3,SEEK_CUR);
		i++;
	}while(0 != strcmp("TIT2",temp) && i<500);//
	//��δ�ҵ���TIT2����ǩ�����ø������༭��Ϊ��
	if(500 == i)
		SetDlgItemText(hwnd,IDC_Title,"");
	//���ҵ���TIT2����ǩ���򽫱������������༭��
	else
	{
		ZeroMemory(ID3V1.Title,sizeof(ID3V1.Title));
		int Title = GetID3Size(fp);
		fseek(fp,3,SEEK_CUR);
		fread(ID3V1.Title,Title-1,1,fp);
		SetDlgItemText(hwnd,IDC_Title,ID3V1.Title);
	}//
		
	i = 0;
	fseek(fp,10,SEEK_SET);
	//�������Ѱ�ҡ�TPE1����ǩ
	do
	{
		fread(temp,4,1,fp);
		temp[4] = '\0';
		fseek(fp,-3,SEEK_CUR);
		i++;
	}while(0 != strcmp("TPE1",temp) && i<500);//
	//��δ�ҵ���TPE1����ǩ�����ø��ֱ༭��Ϊ��
	if(500 == i)
		SetDlgItemText(hwnd,IDC_Artist,"");//
	//���ҵ���TPE1����ǩ���򽫸�����������ֱ༭��
	else
	{
		ZeroMemory(ID3V1.Artist,sizeof(ID3V1.Artist));
		int Artist = GetID3Size(fp);
		fseek(fp,3,SEEK_CUR);
		fread(ID3V1.Artist,Artist-1,1,fp);
		SetDlgItemText(hwnd,IDC_Artist,ID3V1.Artist);
	}//
		
	i = 0;
	fseek(fp,10,SEEK_SET);
	//�������Ѱ�ҡ�TALB����ǩ
	do
	{
		fread(temp,4,1,fp);
		temp[4] = '\0';
		fseek(fp,-3,SEEK_CUR);
		i++;
	}while(0 != strcmp("TALB",temp) && i<500);//
	//��δ�ҵ���TALB����ǩ������ר���༭��Ϊ��
	if(500 == i)
		SetDlgItemText(hwnd,IDC_Album,"");//
	//���ҵ���TALB����ǩ����ר��������ר���༭��
	else
	{
		ZeroMemory(ID3V1.Album,sizeof(ID3V1.Album));
		int Album = GetID3Size(fp);
		fseek(fp,3,SEEK_CUR);
		fread(ID3V1.Album,Album-1,1,fp);
		SetDlgItemText(hwnd,IDC_Album,ID3V1.Album);
	}//		
}//

//�����ǩ֡��С����
int GetID3Size(FILE * fp)
{
	//��ʽΪxxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx 
	//�õ�һ��32λ�Ķ������������Ǳ�ǩ֡��С
	char c[4];
	fseek(fp,3,SEEK_CUR);
	fread(c,1,4,fp);
	int FSize = (int)(c[0]*0x100000000 + c[1]*0x10000+ c[2]*0x100 + c[3]);//
	return FSize;
}//

//��ȡID3V1��Ϣ����
void fID3V1(HWND hwnd)
{
	ID3v1 ID3V1;
	ZeroMemory(&ID3V1,sizeof(ID3V1));
	FILE *fp;
	TCHAR Choose[MAX_PATH];
	ZeroMemory(&Choose,sizeof(Choose));
	GetDlgItemText(hwnd,IDC_EDITNOW,Choose,sizeof(Choose));
	if (fopen_s(&fp,Choose,"r"))
		return;
	//���ļ�λ��ָ������������128�ֽ�ǰ
	fseek(fp,-128L,SEEK_END);//
	fread(ID3V1.Header,3,1,fp);
	//ͨ���ļ�������128�ֽں��Ƿ�Ϊ��TAG���ж��Ƿ�ID3V1��Ϣ
	if (0 == memcmp(ID3V1.Header,"TAG",3))
	{
		ID3V1.Header[3] = '\0';
		//�ֱ��ȡ�������⡢���֡�ר����Ϣ
		fread(ID3V1.Title,30,1,fp);
		ID3V1.Title[30] = '\0';
		fread(ID3V1.Artist,30,1,fp);
		ID3V1.Artist[30] = '\0';
		fread(ID3V1.Album,30,1,fp);
		ID3V1.Album[30] = '\0';
		SetDlgItemText(hwnd,IDC_Title,ID3V1.Title);
		SetDlgItemText(hwnd,IDC_Artist,ID3V1.Artist);
		SetDlgItemText(hwnd,IDC_Album,ID3V1.Album);
	}
	else
	{
		fseek(fp,0,SEEK_SET);
		fread(ID3V1.Header,3,1,fp);
		//ͨ���ļ���ͷ���Ƿ�Ϊ��ID3���ж��Ƿ�ID3V2��Ϣ
		if (0 == memcmp(ID3V1.Header,"ID3",3))//
		{
			//����ID3V2��Ϣ�����ö�ȡID3V2��Ϣ������ȡ��Ϣ
			ID3V2(hwnd,fp,ID3V1);//
		}
		//������ID3V2��Ϣ����ȷ������ID3V1��Ϣ��������Ϣ�༭���ÿ�
		else
		{
			SetDlgItemText(hwnd,IDC_Title,"");
			SetDlgItemText(hwnd,IDC_Artist,"");
			SetDlgItemText(hwnd,IDC_Album,"");
		}//
	}

	//�������Ϊ�գ������ñ����Ϊ������
	SetDlgItemText(hwnd,IDC_Title,ID3V1.Title);
	if (!*ID3V1.Title)
	{
		TCHAR mp3[MAX_PATH],*p;
		GetDlgItemText(hwnd,IDC_EDITNOW,mp3,sizeof(mp3));
		p = mp3;
		if (*mp3 != '\0')
		{
			while (*p != '\0')
			{
				p++;	
			}
			while (*p != ':')
			{
				p--;
				if ('.' == *p)
				{
					*p = '\0';
					break;
				}
			}
			while (*p != '\\')
			{
				p--;
			}
			p++;
				SetDlgItemText(hwnd,IDC_Title,p);
		}
	}//

	fclose(fp);
}//

#endif
