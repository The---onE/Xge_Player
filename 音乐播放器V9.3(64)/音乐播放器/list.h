/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�����б�������ȡ��������
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef list_h
#define list_h

//�����б���������
void ListTotal(HWND hwnd)
{
	HWND MList = GetDlgItem(hwnd,IDC_MList);
	//��ȡ�б�����
	max = ListBox_GetCount(MList);//
	TCHAR Lmax[5];
	wsprintf(Lmax,"%03d/%03d",now,max);
	//�����б�༭�����б�����
	SetDlgItemText(hwnd,IDC_EDITLIST,Lmax);//
}

//���㵱ǰ����λ�ú���
void ListNow(HWND hwnd)
{
	HWND MList = GetDlgItem(hwnd,IDC_MList);
	TCHAR Choose[MAX_PATH];
	GetDlgItemText(hwnd,IDC_EDITNOW,Choose,sizeof(Choose));
	//���б��в��������ڲ��ű༭����������ͬ��ѡ��
	now = ListBox_FindString(MList,0,Choose)+1;//
	TCHAR Lmax[5];
	wsprintf(Lmax,"%03d/%03d",now,max);
	//�����б�༭�������ڲ��Ÿ���λ��
	SetDlgItemText(hwnd,IDC_EDITLIST,Lmax);//
}//
#endif