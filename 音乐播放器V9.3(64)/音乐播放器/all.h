/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//全局变量定义
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef all_h
#define all_h

//定义总长秒、总长分、当前秒、当前分、列表总数、当前歌曲在列表中位置、彩蛋
int Tsec,Tmin,Nsec,Nmin,max = 0,now = 0,cd = 0;//
//定义歌曲当前/总时间字符串、可播放短文件名字符串
TCHAR Time[15],ShortName1[MAX_PATH];//
//定义托盘消息
NOTIFYICONDATA noti;//
//定义系统时间
SYSTEMTIME STime;//
//定义由文件打开程序时文件名指针
TCHAR *pfo;//
//定义睡眠模式状态参数、静音状态参数、搜索打开状态参数、设置打开状态参数、歌词打开状态参数、迷你窗口状态、迷你歌词状态
BOOL sleep = FALSE,onoff = TRUE,sea = FALSE,set = FALSE,lrc = FALSE,min = FALSE,mil = FALSE;//
//定义搜索窗口、设置窗口、歌词窗口、迷你窗口句柄、迷你歌词句柄
HWND seahwnd = NULL,sethwnd = NULL,lrchwnd = NULL,minhwnd = NULL,milhwnd = NULL;//

#endif