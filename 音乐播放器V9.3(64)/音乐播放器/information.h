/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//读取MP3文件中信息（ID3V1、ID3V2）函数定义
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef information_h
#define information_h

//定义ID3V1结构体存储歌曲信息
struct ID3v1
{
	//通过文件倒数第128字节后是否为“TAG”判断是否含ID3V1信息
	//详细信息搜索ID3
	char Header[4];//
	//歌曲标题、歌手、专辑信息固定长度为30
	char Title[31];
	char Artist[31];
	char Album[31];////
};// 

//读取ID3V2信息函数
void ID3V2(HWND hwnd,FILE * fp,ID3v1 ID3V1)
{
	TCHAR temp[5];
	int i = 0; 
	fseek(fp,10,SEEK_SET);
	//不断向后寻找“TIT2”标签
	do
	{
		fread(temp,4,1,fp);
		temp[4] = '\0';
		fseek(fp,-3,SEEK_CUR);
		i++;
	}while(0 != strcmp("TIT2",temp) && i<500);//
	//若未找到“TIT2”标签，则置歌曲名编辑框为空
	if(500 == i)
		SetDlgItemText(hwnd,IDC_Title,"");
	//若找到“TIT2”标签，则将标题加入歌曲名编辑框
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
	//不断向后寻找“TPE1”标签
	do
	{
		fread(temp,4,1,fp);
		temp[4] = '\0';
		fseek(fp,-3,SEEK_CUR);
		i++;
	}while(0 != strcmp("TPE1",temp) && i<500);//
	//若未找到“TPE1”标签，则置歌手编辑框为空
	if(500 == i)
		SetDlgItemText(hwnd,IDC_Artist,"");//
	//若找到“TPE1”标签，则将歌手名加入歌手编辑框
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
	//不断向后寻找“TALB”标签
	do
	{
		fread(temp,4,1,fp);
		temp[4] = '\0';
		fseek(fp,-3,SEEK_CUR);
		i++;
	}while(0 != strcmp("TALB",temp) && i<500);//
	//若未找到“TALB”标签，则置专辑编辑框为空
	if(500 == i)
		SetDlgItemText(hwnd,IDC_Album,"");//
	//若找到“TALB”标签，则将专辑名加入专辑编辑框
	else
	{
		ZeroMemory(ID3V1.Album,sizeof(ID3V1.Album));
		int Album = GetID3Size(fp);
		fseek(fp,3,SEEK_CUR);
		fread(ID3V1.Album,Album-1,1,fp);
		SetDlgItemText(hwnd,IDC_Album,ID3V1.Album);
	}//		
}//

//计算标签帧大小函数
int GetID3Size(FILE * fp)
{
	//格式为xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx 
	//得到一个32位的二进制数，就是标签帧大小
	char c[4];
	fseek(fp,3,SEEK_CUR);
	fread(c,1,4,fp);
	int FSize = (int)(c[0]*0x100000000 + c[1]*0x10000+ c[2]*0x100 + c[3]);//
	return FSize;
}//

//读取ID3V1信息函数
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
	//将文件位置指针移至倒数第128字节前
	fseek(fp,-128L,SEEK_END);//
	fread(ID3V1.Header,3,1,fp);
	//通过文件倒数第128字节后是否为“TAG”判断是否含ID3V1信息
	if (0 == memcmp(ID3V1.Header,"TAG",3))
	{
		ID3V1.Header[3] = '\0';
		//分别读取歌曲标题、歌手、专辑信息
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
		//通过文件开头后是否为“ID3”判断是否含ID3V2信息
		if (0 == memcmp(ID3V1.Header,"ID3",3))//
		{
			//若含ID3V2信息，调用读取ID3V2信息函数读取信息
			ID3V2(hwnd,fp,ID3V1);//
		}
		//若不含ID3V2信息（已确定不含ID3V1信息），则信息编辑框置空
		else
		{
			SetDlgItemText(hwnd,IDC_Title,"");
			SetDlgItemText(hwnd,IDC_Artist,"");
			SetDlgItemText(hwnd,IDC_Album,"");
		}//
	}

	//若标题框为空，则设置标题框为歌曲名
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
