// PlayLocalFile.cpp : implementation file
//

#include "stdafx.h"
#include "PlayBack.h"
#include "PlayLocalFile.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlayLocalFile property page

IMPLEMENT_DYNCREATE(CPlayLocalFile, CPropertyPage)

CPlayLocalFile::CPlayLocalFile() : CPropertyPage(CPlayLocalFile::IDD)
{
	//{{AFX_DATA_INIT(CPlayLocalFile)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_iRecType = 0xff;
	m_bIsPlaying = FALSE;
	m_bIsPause = FALSE;
	m_bChangeSpeed = FALSE;
	m_bStep = FALSE;
	m_lPlayPort = -1000;
	m_TotalTime = 0;
}

CPlayLocalFile::~CPlayLocalFile()
{
}

void CPlayLocalFile::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlayLocalFile)
	DDX_Control(pDX, IDC_FILE_LIST, m_ctrlFileList);
	DDX_Control(pDX, IDC_SLIDER_PLAY_LOCAL, m_sliderPlay);
	DDX_Control(pDX, IDC_TIME_START_LOCAL, m_StartTime);
	DDX_Control(pDX, IDC_TIME_END_LOCAL, m_StopTime);
	DDX_Control(pDX, IDC_DATE_START_LOCAL, m_StartDate);
	DDX_Control(pDX, IDC_DATE_END_LOCAL, m_StopDate);
	DDX_Control(pDX, IDC_COMBO_TYPE_LOCAL, m_coRecType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPlayLocalFile, CPropertyPage)
	//{{AFX_MSG_MAP(CPlayLocalFile)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH_LOCAL, OnBtnSearchLocal)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE_LOCAL, OnSelchangeComboTypeLocal)
	ON_NOTIFY(NM_DBLCLK, IDC_FILE_LIST, OnDblclkFileList)
	ON_BN_CLICKED(IDC_BUTTON_PLAY_LOCAL, OnBtnPlayLocal)
	ON_BN_CLICKED(IDC_BUTTON_STOP_LOCAL, OnBtnStopLocal)
	ON_BN_CLICKED(IDC_BUTTON_FAST_LOCAL, OnBtnFastLocal)
	ON_BN_CLICKED(IDC_BUTTON_SLOW_LOCAL, OnBtnSlowLocal)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_PLAY_LOCAL, OnReleasedcaptureSliderPlayLocal)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_DEL_ALL, OnButtonDelAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlayLocalFile message handlers

BOOL CPlayLocalFile::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	//��ʼ���ļ��б�
	m_ctrlFileList.InsertColumn(0,"�ļ�����",LVCFMT_LEFT,100);
	m_ctrlFileList.InsertColumn(1,"��С",LVCFMT_LEFT,50);
	m_ctrlFileList.InsertColumn(2,"��ʼʱ��",LVCFMT_LEFT,90);
	m_ctrlFileList.InsertColumn(3,"����ʱ��",LVCFMT_LEFT,90);
	m_ctrlFileList.InsertColumn(4,"����",LVCFMT_LEFT,50);
	
    //�ļ����ͣ�����
	m_coRecType.SetCurSel(0);

	//���Ž���
	m_sliderPlay.SetRange(0,100);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPlayLocalFile::OnBtnSearchLocal() 
{

	//��ǰѡ���豸��ͨ��
//	int iCurDevice = g_pMainDlg->m_iCurDeviceIndex;
	int iCurChannel = g_pMainDlg->m_iCurChanIndex;
// 	if(-1 == iCurDevice)
// 	{
// 		MessageBox("��ѡ��һ���豸");
// 		return;
// 	}
	//������б�
	m_ctrlFileList.DeleteAllItems();
	//��ȡ���ҵ�ʱ���	
	CTime tStartTime,tStopTime;
	GetStartStopTime(tStartTime,tStopTime);
	//�ļ�����
	int iType = m_coRecType.GetCurSel();
	//�����ļ�
	int num = SearchLocalFile(iCurDevice,iCurChannel,tStartTime,tStopTime,m_iRecType);

}



//���ұ����ļ���iChanIndexΪ-1ʱĬ��Ϊ�����豸������ͨ��
int CPlayLocalFile::SearchLocalFile(int iDevIndex, int iChanIndex, const CTime &StartTime, const CTime &StopTime, int iFileType)
{
	int FileNum = 0;   //�ļ���

	switch(iFileType)
	{
	case 0xff:    //ȫ������ʱֻ����¼���ļ�
			FileNum = SearchRecordFile(iDevIndex,iChanIndex,StartTime,StopTime);
			break;
	case 0:       //����
		break;
	case 1:       //����
		break;
	case 2:       //����
		break;
	default:
		break;
	}
	return FileNum;
}


//��ȡ��ʼ�����ʱ��
void CPlayLocalFile::GetStartStopTime(CTime &tStartTime, CTime &tStopTime)
{
	CTime StartDay,StartTime;
	CTime StopDay,StopTime;
	
    m_StartDate.GetTime(StartDay);
	m_StartTime.GetTime(StartTime);
	m_StopDate.GetTime(StopDay);
	m_StopTime.GetTime(StopTime);
	
	tStartTime = CTime(StartDay.GetYear(),StartDay.GetMonth(),StartDay.GetDay(),\
		                StartTime.GetHour(),StartTime.GetMinute(),StartTime.GetSecond());
	tStopTime = CTime(StopDay.GetYear(),StopDay.GetMonth(),StopDay.GetDay(),\
		                StopTime.GetHour(),StopTime.GetMinute(),StopTime.GetSecond());
/*	StartDayTime.dwYear = StartDay.GetYear();
	StartDayTime.dwMonth = StartDay.GetMonth();
    StartDayTime.dwDay = StartDay.GetDay();
    StartDayTime.dwHour = StartTime.GetHour();
	StartDayTime.dwMinute = StartTime.GetMinute();
	StartDayTime.dwSecond = StartTime.GetSecond();
	
	StopDayTime.dwYear = StopDay.GetYear();
	StopDayTime.dwMonth = StopDay.GetMonth();
    StopDayTime.dwDay = StopDay.GetDay();
    StopDayTime.dwHour = StopTime.GetHour();
	StopDayTime.dwMinute = StopTime.GetMinute();
	StopDayTime.dwSecond = StopTime.GetSecond();*/
}

void CPlayLocalFile::OnSelchangeComboTypeLocal() 
{
	// TODO: Add your control notification handler code here
	int CurSel = m_coRecType.GetCurSel();
	if(0 == CurSel)
		m_iRecType = 0xff;
	else
		m_iRecType = CurSel - 1;
}


//�˶��ļ���ʱ���Ƿ��ڲ���ʱ�����
BOOL CPlayLocalFile::CheckDate(const CTime &StartTime, const CTime &StopTime, const CTime &DirName)
{
	if(StartTime<=DirName && DirName<=StopTime)
	    return TRUE;
	else
		return FALSE; 

}


//���ұ���¼���ļ�,iChanIndex == -1ʱ��Ĭ�ϲ���ȫ��ͨ����¼��
int CPlayLocalFile::SearchRecordFile(int iDevIndex, int iChanIndex, const CTime &StartTime, const CTime &StopTime)
{

	int FileNum = 0;
	CFileFind finderDir;
	CString RecordDir("C:\\mpeg4record\\*.*");
	BOOL bWorking = finderDir.FindFile(RecordDir);
	while(bWorking)
	{
		bWorking = finderDir.FindNextFile();
		if (finderDir.IsDots())
			continue;
		if (finderDir.IsDirectory())   //Ŀ¼��Ŀ¼��������
		{
			CString DirName = finderDir.GetFileName();
			CTime DirTime;
			finderDir.GetCreationTime(DirTime);     //��ȡ����ʱ��
			//�ж������Ƿ��ڲ��ҵ�ʱ�����
			BOOL IsIn = CheckDate(StartTime,StopTime,DirTime);
			if(IsIn)     //�������ļ���
			{
				CString DirPath = finderDir.GetFilePath();
				DirPath += "\\";
				DirPath += g_struDevInfo[iDevIndex].chDeviceIP;
				if(iChanIndex >= 1)     //ѡ��ĳһͨ��������ͨ����
				{
					CString Chan;
					Chan.Format("_%02d",iChanIndex);
				    DirPath += Chan;
				}
				DirPath += "*.mp4";
				CFileFind finderFile;
				BOOL bFindFile = finderFile.FindFile(DirPath);
				while(bFindFile)
				{
					bFindFile = finderFile.FindNextFile();
					if(finderFile.IsDots())
						continue;
					if(finderFile.IsDirectory())
						continue;

					//����ʱ��
					CTime CreateTime;
					finderFile.GetCreationTime(CreateTime);
					BOOL IsFileWanted = CheckDate(StartTime,StopTime,CreateTime);
					if(IsFileWanted)
					{
						CTime LastWriteTime;
						finderFile.GetLastWriteTime(LastWriteTime);
						LOCAL_FILE_INFO FileInfo;
						strcpy(FileInfo.chName,finderFile.GetFilePath());
						FileInfo.FileSize = finderFile.GetLength();
						FileInfo.iType = 0;     //�ļ�����
						FileInfo.StartTime = CreateTime;
						FileInfo.StopTime = LastWriteTime;
						m_vLocalFile.push_back(FileInfo);   //���浽�ļ��б�

						//���뵽�б��
						
						CString csSize,csStartTime,csStopTime,csFileType;
						csSize.Format("%0.2fM",(double)FileInfo.FileSize/1024/1024);
						csStartTime = CreateTime.Format("%Y.%m.%d %H:%M:%S");
						csStopTime = LastWriteTime.Format("%Y.%m.%d %H:%M:%S");
						csFileType = "����";
                        m_ctrlFileList.InsertItem(FileNum,finderFile.GetFileTitle());
                        m_ctrlFileList.SetItemText(FileNum,1,csSize);
						m_ctrlFileList.SetItemText(FileNum,2,csStartTime);
						m_ctrlFileList.SetItemText(FileNum,3,csStopTime);
						m_ctrlFileList.SetItemText(FileNum,4,csFileType);

						//�����ļ���
						FileNum++;
					}
				}
			}
		}
	}
	if(FileNum == 0)
		MessageBox("���ҽ�����û���ļ�");
	return FileNum;
}


//˫���б�����
void CPlayLocalFile::OnDblclkFileList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if(m_bIsPlaying)    //���ڲ��ţ���ֹͣ
	{
		StopPlayBack();
	}

	PlayLoalFile();


	*pResult = 0;
}

void CPlayLocalFile::PlayLoalFile()
{

	CString FileName;
	HWND hPlayWnd;
	POSITION pos = m_ctrlFileList.GetFirstSelectedItemPosition();
	if(pos == NULL)
	{
		MessageBox("��ѡ��һ���ļ�");
		return;
	}
	int SelIndex = m_ctrlFileList.GetNextSelectedItem(pos);

	//��ȡ����ͨ����
    
	if(!PlayM4_GetPort(&m_lPlayPort))
	{
		MessageBox("��ȡͨ����ʧ��!");
		return;
	}

	//PlayM4_SetFileRefCallBack(...)

	//���ļ�
    if(!PlayM4_OpenFile(m_lPlayPort,m_vLocalFile[SelIndex].chName))
	{
		MessageBox("���ļ�ʧ��!");
		return;
	}

	
	//����
	hPlayWnd = GetDlgItem(IDC_STATIC_PLAY_LOCAL)->GetSafeHwnd();
	if(!PlayM4_Play(m_lPlayPort,hPlayWnd))
	{
		MessageBox("����ʧ��!");
		return;
	}
    m_TotalTime = PlayM4_GetFileTime(m_lPlayPort); 
	m_TotalFrames = PlayM4_GetFileTotalFrames(m_lPlayPort);
	SetTimer(LOCAL_PLAY_STATE_TIMER,500,NULL);
	m_bIsPlaying = TRUE;
	GetDlgItem(IDC_BUTTON_PLAY_LOCAL)->SetWindowText("��ͣ");
	

}

void CPlayLocalFile::StopPlayBack()
{
	if(m_bIsPlaying)
	{
		PlayM4_Stop(m_lPlayPort);
		PlayM4_CloseFile(m_lPlayPort);
		PlayM4_FreePort(m_lPlayPort);
		KillTimer(LOCAL_PLAY_STATE_TIMER);
		m_bIsPlaying = FALSE;
		m_bIsPause = FALSE;
		GetDlgItem(IDC_BUTTON_PLAY_LOCAL)->SetWindowText("����");
		m_sliderPlay.SetPos(0);
	    GetDlgItem(IDC_STATIC_PLAY_LOCAL)->Invalidate();
	}
}

void CPlayLocalFile::OnBtnPlayLocal() 
{
	// TODO: Add your control notification handler code here
	if(m_bIsPlaying)   
	{
		if(m_bIsPause)  //resatrt
		{
            PlayM4_Pause(m_lPlayPort,FALSE);
            m_bIsPause = FALSE;
			GetDlgItem(IDC_BUTTON_PLAY_LOCAL)->SetWindowText("��ͣ");
		}
		else            //pause
		{
			PlayM4_Pause(m_lPlayPort,TRUE);
			m_bIsPause = TRUE;
			GetDlgItem(IDC_BUTTON_PLAY_LOCAL)->SetWindowText("����");
		}
	}
	else                //start play
	{
		PlayLoalFile();
		m_bIsPlaying = TRUE;
		GetDlgItem(IDC_BUTTON_PLAY_LOCAL)->SetWindowText("��ͣ");

	}
}

void CPlayLocalFile::OnBtnStopLocal() 
{
	// TODO: Add your control notification handler code here
	StopPlayBack();
	
}

void CPlayLocalFile::OnBtnFastLocal() 
{
	// TODO: Add your control notification handler code here
	if(m_bIsPlaying && !m_bIsPause)
	{
		PlayM4_Fast(m_lPlayPort);
	//	m_bChangeSpeed = TRUE;
	}
	
}

void CPlayLocalFile::OnBtnSlowLocal() 
{
	// TODO: Add your control notification handler code here
	if(m_bIsPlaying && !m_bIsPause)
	{
		PlayM4_Slow(m_lPlayPort);
	//	m_bChangeSpeed = TRUE;
	}
}

void CPlayLocalFile::OnReleasedcaptureSliderPlayLocal(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if(m_bIsPlaying)
	{
		int pos = m_sliderPlay.GetPos();
		if(pos >= 0)
		{
			float fRelativePos = (float)pos/100;
			PlayM4_SetPlayPos(m_lPlayPort,fRelativePos);
		}
	}
	
	*pResult = 0;
}

void CPlayLocalFile::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{
	case LOCAL_PLAY_STATE_TIMER:
		{
			DWORD CurTime = PlayM4_GetPlayedTime(m_lPlayPort);
			DWORD CurFrame = PlayM4_GetPlayedFrames(m_lPlayPort);
			
			float fPos = PlayM4_GetPlayPos(m_lPlayPort);
			if(fPos>=(1-0.001) && fPos<=(1+0.001))
			{
                StopPlayBack();
				break;
			}
			int iPos = fPos*100;
			m_sliderPlay.SetPos(iPos);		
            break;
		}
		
	default:
		break;

	}
	CPropertyPage::OnTimer(nIDEvent);
}

void CPlayLocalFile::OnButtonDel() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_ctrlFileList.GetFirstSelectedItemPosition();
	if(pos == NULL)
	{
		MessageBox("��ѡ��һ���ļ�");
		return;
	}
	int SelIndex = m_ctrlFileList.GetNextSelectedItem(pos);

	if(IDOK != MessageBox("ȷ��ɾ���ļ���","Warning!",IDOK))
		return;

	if(!DeleteFile(m_vLocalFile[SelIndex].chName))
	{
		MessageBox("ɾ���ļ�ʧ��!");
		return;
	}

	//��m_vLocalFileɾ��
//	m_vLocalFile.erase(SelIndex);
//	vector<LOCAL_FILE_INFO>::iterator itVect;
//	for(int i=0,itVect=m_vLocalFile.begin(); itVect!=m_vLocalFile.end();i++,itVect++)
/*	for(int i=0; i<m_vLocalFile.size();i++)
	{
		if(i == SelIndex)
		{
			m_vLocalFile.erase(i);
			break;
		}
	}
*/
	for(int i=SelIndex; i<m_vLocalFile.size();i++)
	{
		m_vLocalFile[i]=m_vLocalFile[i+1];
	}
	m_vLocalFile.pop_back();
	//���б���ɾ��
	m_ctrlFileList.DeleteItem(SelIndex);
	
}

void CPlayLocalFile::OnButtonDelAll() 
{
	// TODO: Add your control notification handler code here
	if(IDOK != MessageBox("ȷ��ɾ�������ļ���","Warning!",IDOK))
		return;
	for(int i=0; i<m_vLocalFile.size(); i++)
	{
		if(!DeleteFile(m_vLocalFile[i].chName))
		{
		//	MessageBox("ɾ���ļ�ʧ��!");
			continue;
		}
	}

	m_vLocalFile.clear();
    m_ctrlFileList.DeleteAllItems();

}
