// PlayRemoteFile.cpp : implementation file
//

#include "stdafx.h"
#include "PlayBack.h"
#include "PlayRemoteFile.h"
#include "HCNetSDK.h"
#include "PlayBack.h"
#include <process.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




void RemoteFileSearchThread(void* pParam)
{
	CPlayRemoteFile *pDlg = (CPlayRemoteFile*)pParam;

	NET_DVR_FILECOND FileCond;
	memcpy(&FileCond,&pDlg->m_FileCond,sizeof(NET_DVR_FILECOND));

//	LONG hFindHandle = NET_DVR_FindFile_V30(g_struDevInfo[g_iCurDevIndex].lLoginID,&FileCond);
	LONG hFindHandle = NET_DVR_FindFile_V30(g_pMainDlg->m_struDeviceInfo.lLoginID,&FileCond);
	if(-1 == hFindHandle)
	{
		pDlg->MessageBox("����ʧ��");
		return;
	}
	else
	{
		NET_DVR_FINDDATA_V30 FindData;  //���ҵ����ļ���Ϣ
		int ret = NET_DVR_FindNextFile_V30(hFindHandle,&FindData);
		pDlg->GetDlgItem(IDC_STATIC_SEARCH)->SetWindowText("���ڲ����ļ�...");
		pDlg->GetDlgItem(IDC_STATIC_SEARCH)->Invalidate();
		int ItemIndex = 0;
		while(ret>0)
		{
			
			if(NET_DVR_FILE_EXCEPTION == ret)
			{
				pDlg->GetDlgItem(IDC_STATIC_SEARCH)->SetWindowText("�����ļ�ʱ�쳣!");
				pDlg->MessageBox("�����ļ�ʱ�쳣");
				break;
			}
			else if(NET_DVR_FILE_NOFIND == ret)
			{
			    pDlg->GetDlgItem(IDC_STATIC_SEARCH)->SetWindowText("û��¼���ļ�!");
				pDlg->MessageBox("û��¼���ļ�");
				break;
				
			}
			else if(NET_DVR_NOMOREFILE == ret)   //���ҽ���
			{
				pDlg->GetDlgItem(IDC_STATIC_SEARCH)->SetWindowText("���ҽ���!");
				break;
			}
			else if(NET_DVR_ISFINDING == ret)  //���ڲ���
			{
				//GetDlgItem(IDC_STATIC_SEARCH)->ShowWindow(SW_SHOW);
				ret = NET_DVR_FindNextFile_V30(hFindHandle,&FindData);
				Sleep(5);
			}
			else if(NET_DVR_FILE_SUCCESS == ret)  //��ȡ�ļ���Ϣ�ɹ�
			{
				//�����ļ���Ϣ
				
				CString FileName,FileSize,StartTime,StopTime,Property;
				FileName.Format("%s",FindData.sFileName);
				FileSize.Format("%0.2fM",(double)FindData.dwFileSize/1024/1024);
				StartTime.Format("%04d%02d%02d%02d%02d%02d",FindData.struStartTime.dwYear,FindData.struStartTime.dwMonth,FindData.struStartTime.dwDay,\
					FindData.struStartTime.dwHour,FindData.struStartTime.dwMinute,FindData.struStartTime.dwSecond);
				StopTime.Format("%d%d%d%d%d%d",FindData.struStopTime.dwYear,FindData.struStopTime.dwMonth,FindData.struStopTime.dwDay,\
					FindData.struStopTime.dwHour,FindData.struStopTime.dwMinute,FindData.struStopTime.dwSecond);
				Property.Format("%s",FindData.byLocked == 1 ? "�ļ�������" : "�����ļ�");
				
				pDlg->m_ctrlFileList.InsertItem(ItemIndex,FileName);
				pDlg->m_ctrlFileList.SetItemText(ItemIndex,1,FileSize);
				pDlg->m_ctrlFileList.SetItemText(ItemIndex,2,StartTime);
				pDlg->m_ctrlFileList.SetItemText(ItemIndex,3,StopTime);
				pDlg->m_ctrlFileList.SetItemText(ItemIndex,4,Property);
				ItemIndex++;
				g_vecFileInfo.push_back(FindData);
				ret = NET_DVR_FindNextFile_V30(hFindHandle,&FindData);
				
			}
		}
		//�رղ��ң��ͷž��
		NET_DVR_FindClose_V30(hFindHandle);
	}

}

/////////////////////////////////////////////////////////////////////////////
// CPlayRemoteFile property page

IMPLEMENT_DYNCREATE(CPlayRemoteFile, CPropertyPage)

CPlayRemoteFile::CPlayRemoteFile() : CPropertyPage(CPlayRemoteFile::IDD)
{
	//{{AFX_DATA_INIT(CPlayRemoteFile)
	//}}AFX_DATA_INIT
	m_hPlayHandle = -1;
    m_IsDownloading = FALSE;
	m_lDownloadHandle = -1;
	m_IsPlaying = FALSE;
	m_IsPause = FALSE;
	m_bSound = FALSE;
	m_CurPlayPos = 0;
	m_bChangeSpeed = FALSE;
	m_bPlayStep = FALSE;
	m_bRecord = FALSE;
	m_RecType = 0xff;
	m_FileProperty = 0xff;
	m_csCurFile = "";
	memset(&m_FileCond,0,sizeof(NET_DVR_FILECOND));

	m_hPlayEnableIcon =			AfxGetApp()->LoadIcon(IDI_PLAY_ENABLE);
	m_hPlayDisableIcon =		AfxGetApp()->LoadIcon(IDI_PLAY_DISABLE);
	m_hPauseEnableIcon =		AfxGetApp()->LoadIcon(IDI_PAUSE_ENABLE);
	m_hPauseDisableIcon =		AfxGetApp()->LoadIcon(IDI_PAUSE_DISABLE);
	m_hStopEnableIcon =			AfxGetApp()->LoadIcon(IDI_STOP_ENABLE);
	m_hStopDisableIcon =		AfxGetApp()->LoadIcon(IDI_STOP_DISABLE);
	m_hSoundStartIcon=			AfxGetApp()->LoadIcon(IDI_SOUND_ENABLE);
	m_hSoundStopIcon=			AfxGetApp()->LoadIcon(IDI_SOUND_DISABLE);
	m_hGotoHeadDisableIcon =	AfxGetApp()->LoadIcon(IDI_GOTOSTART_DISABLE);
	m_hGotoTailDisableIcon =	AfxGetApp()->LoadIcon(IDI_GOTOEND_DISABLE);
	m_hFastDisableIcon =		AfxGetApp()->LoadIcon(IDI_FASTFORWARD_DISABLE);
	m_hFastEnableIcon =			AfxGetApp()->LoadIcon(IDI_FASTFORWARD_ENABLE);
	m_hSlowDisableIcon=			AfxGetApp()->LoadIcon(IDI_FASTBACKWARD_DISABLE);
	m_hSlowEnableIcon =			AfxGetApp()->LoadIcon(IDI_FASTBACKWARD_ENABLE);
	m_hGotoHeadEnableIcon =		AfxGetApp()->LoadIcon(IDI_GOTOSTART_ENABLE);
	m_hGotoTailEnableIcon =		AfxGetApp()->LoadIcon(IDI_GOTOEND_ENABLE);
	m_hStepEnableIcon=			AfxGetApp()->LoadIcon(IDI_STEP_ENABLE);
	m_hStepDisableIcon =		AfxGetApp()->LoadIcon(IDI_STEP_DISABLE);
	m_hStepBackEnableIcon =		AfxGetApp()->LoadIcon(IDI_STEPBACK_ENABLE);
	m_hStepBackDisableIcon =	AfxGetApp()->LoadIcon(IDI_STEPBACK_DISABLE);
	m_hCaptureIcon	=			AfxGetApp()->LoadIcon(IDI_PIC);
}

CPlayRemoteFile::~CPlayRemoteFile()
{
}

void CPlayRemoteFile::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlayRemoteFile)
	DDX_Control(pDX, IDC_SLIDER_VOLUME, m_sliderVolume);
	DDX_Control(pDX, IDC_SLIDER_PLAY, m_sliderPlay);
	DDX_Control(pDX, IDC_PROGRESS_DOWNLOAD, m_progressDownload);
	DDX_Control(pDX, IDC_STATIC_PLAY, m_staticPlay);
	DDX_Control(pDX, IDC_FILE_LIST, m_ctrlFileList);
	DDX_Control(pDX, IDC_TIME_START, m_TimeStart);
	DDX_Control(pDX, IDC_TIME_END, m_TimeEnd);
	DDX_Control(pDX, IDC_DATE_END, m_DateEnd);
	DDX_Control(pDX, IDC_DATE_START, m_DateStart);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_coRecType);
	DDX_Control(pDX, IDC_COMBO_FILE_PROPERTY, m_coFileProperty);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPlayRemoteFile, CPropertyPage)
	//{{AFX_MSG_MAP(CPlayRemoteFile)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, OnBtnSearch)
	ON_NOTIFY(NM_DBLCLK, IDC_FILE_LIST, OnDblclkFileList)
	ON_BN_CLICKED(IDC_BUTTON_REMOTE_PLAY, OnButtonPlay)
	ON_BN_CLICKED(IDC_BUTTON_REMOTE_STOP, OnButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_DOWNLOAD, OnButtonDownload)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_REMOTE_BACK, OnButtonBack)
	ON_BN_CLICKED(IDC_BUTTON_REMOTE_STEP, OnButtonStep)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, OnSelchangeComboType)
	ON_CBN_SELCHANGE(IDC_COMBO_FILE_PROPERTY, OnSelchangeComboFileProperty)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_PLAY, OnReleasedcaptureSliderPlay)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_REMOTE_SLOW, OnButtonRemoteSlow)
	ON_BN_CLICKED(IDC_BUTTON_REMOTE_FAST, OnButtonRemoteFast)
	ON_BN_CLICKED(IDC_BUTTON_REMOTE_PIC, OnButtonRemotePic)
	ON_BN_CLICKED(IDC_BUTTON_REMOTE_SOUND, OnButtonRemoteSound)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_VOLUME, OnReleasedcaptureSliderVolume)
	ON_BN_CLICKED(IDC_BUTTON_REMOTE_STOP_SAVE, OnButtonRemoteStopSave)
	ON_BN_CLICKED(IDC_BUTTON_REMOTE_SAVE, OnButtonRemoteSave)
	ON_NOTIFY(NM_CLICK, IDC_FILE_LIST, OnClickFileList)
	ON_BN_CLICKED(IDC_BUTTON_UNLOCK, OnButtonLockUnlock)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlayRemoteFile message handlers

BOOL CPlayRemoteFile::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	
 //   GetDlgItem(IDC_STATIC_SEARCH)->ShowWindow(SW_HIDE);
	//��ʼ���ļ��б�
	m_ctrlFileList.InsertColumn(0,"�ļ�����",LVCFMT_LEFT,90);
	m_ctrlFileList.InsertColumn(1,"��С",LVCFMT_LEFT,50);
	m_ctrlFileList.InsertColumn(2,"��ʼʱ��",LVCFMT_LEFT,85);
	m_ctrlFileList.InsertColumn(3,"����ʱ��",LVCFMT_LEFT,85);
	m_ctrlFileList.InsertColumn(4,"����",LVCFMT_LEFT,50);

    //�ļ����ͣ�����
	m_coRecType.SetCurSel(0);
	m_coFileProperty.SetCurSel(0);

	//���ؽ�����
	m_progressDownload.SetRange(0,100);
	m_progressDownload.SetPos(0);
	m_progressDownload.ShowWindow(SW_HIDE);

	//���Ž���
	m_sliderPlay.SetRange(0,100,TRUE);

	//����
	m_sliderVolume.SetRange(0,0xffff);
	//��ʼ������
	InitUI();


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPlayRemoteFile::OnBtnSearch() 
{
	//������б�
	m_ctrlFileList.DeleteAllItems();
	//�������
	g_vecFileInfo.clear();
	//��ȡ���ҵ�ʱ���
	NET_DVR_TIME StartSearchTime, StopSearchTime;
	CTime StartDay,StartTime;
	CTime StopDay,StopTime;

	m_DateStart.GetTime(StartDay);
	m_TimeStart.GetTime(StartTime);
	m_DateEnd.GetTime(StopDay);
	m_TimeEnd.GetTime(StopTime);

	
	StartSearchTime.dwYear = StartDay.GetYear();
	StartSearchTime.dwMonth = StartDay.GetMonth();
    StartSearchTime.dwDay = StartDay.GetDay();
    StartSearchTime.dwHour = StartTime.GetHour();
	StartSearchTime.dwMinute = StartTime.GetMinute();
	StartSearchTime.dwSecond = StartTime.GetSecond();

	StopSearchTime.dwYear = StopDay.GetYear();
	StopSearchTime.dwMonth = StopDay.GetMonth();
    StopSearchTime.dwDay = StopDay.GetDay();
    StopSearchTime.dwHour = StopTime.GetHour();
	StopSearchTime.dwMinute = StopTime.GetMinute();
	StopSearchTime.dwSecond = StopTime.GetSecond();
	
	//��ǰѡ���豸��ͨ��
//	int iCurDevice = g_pMainDlg->m_iCurDeviceIndex;
	int iCurChanIndex = g_pMainDlg->m_iCurChanIndex;
	int iCurChannel = g_pMainDlg->m_struDeviceInfo.struChanInfo[iCurChanIndex].iChanIndex;
	//Login ID
//	LONG LoginID = g_struDevInfo[iCurDevice].lLoginID;

	LONG LoginID = g_pMainDlg->m_struDeviceInfo.lLoginID;
	//�ļ����ҽṹ��
	NET_DVR_FILECOND   FileCond;
	FileCond.dwFileType = m_RecType;
	FileCond.dwIsLocked = m_FileProperty;
	FileCond.dwUseCardNo = 0;
	if(iCurChanIndex == -1)   //δѡ��ͨ��
	{
		MessageBox("��ѡ��һ��ͨ��");
		return;
	}
	FileCond.lChannel = iCurChannel;
	
	memcpy(&FileCond.struStartTime,&StartSearchTime,sizeof(NET_DVR_TIME));
	memcpy(&FileCond.struStopTime,&StopSearchTime,sizeof(NET_DVR_TIME));
	
	//���Ƹ�m_FileCond
	memcpy(&m_FileCond,&FileCond,sizeof(NET_DVR_FILECOND));

	//���������ļ��߳�
    HANDLE ThreadHandle = (HANDLE)_beginthread(RemoteFileSearchThread,0,(void*)this);
	CloseHandle(ThreadHandle);

}


void CPlayRemoteFile::InsertFileList()
{
	int FileNum = g_vecFileInfo.size();
	CString FileName,FileSize,StartTime,StopTime,Property;
	NET_DVR_FINDDATA_V30 FileInfo;
	for(int i=0; i<FileNum; i++)
	{
		memcpy(&FileInfo,&g_vecFileInfo[i],sizeof(NET_DVR_FINDDATA_V30));
		FileName.Format("%s",FileInfo.sFileName);
		FileSize.Format("%0.2fM",(double)FileInfo.dwFileSize/1024/1024);
		StartTime.Format("%d%d%d%d%d%d",FileInfo.struStartTime.dwYear,FileInfo.struStartTime.dwMonth,FileInfo.struStartTime.dwDay,\
			FileInfo.struStartTime.dwHour,FileInfo.struStartTime.dwMinute,FileInfo.struStartTime.dwSecond);
		StopTime.Format("%d%d%d%d%d%d",FileInfo.struStopTime.dwYear,FileInfo.struStopTime.dwMonth,FileInfo.struStopTime.dwDay,\
			FileInfo.struStopTime.dwHour,FileInfo.struStopTime.dwMinute,FileInfo.struStopTime.dwSecond);
        Property.Format("%s",FileInfo.byLocked == 1 ? "�ļ�������" : "�����ļ�");

		m_ctrlFileList.InsertItem(i,FileName);
		m_ctrlFileList.SetItemText(i,1,FileSize);
		m_ctrlFileList.SetItemText(i,2,StartTime);
		m_ctrlFileList.SetItemText(i,3,StopTime);
		m_ctrlFileList.SetItemText(i,4,Property);
		//m_ctrlFileList.SetItemData(i,i);
	}
}

void CPlayRemoteFile::OnDblclkFileList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

	if(FALSE == StopPlayback())
		return;
	StartPlayback();
	*pResult = 0;
}

//��ʼ�ط�
BOOL CPlayRemoteFile::StartPlayback()
{
	POSITION pos = m_ctrlFileList.GetFirstSelectedItemPosition();
	if(pos == NULL)
	{
		MessageBox("��ѡ��һ���ļ�");
		return FALSE;
	}
	if(FALSE == StopPlayback())
		return FALSE;

	int index = m_ctrlFileList.GetNextSelectedItem(pos);
	HWND hPlayWnd = m_staticPlay.GetSafeHwnd();
	char *pFileName = g_vecFileInfo[index].sFileName;
	
//	m_hPlayHandle = NET_DVR_PlayBackByName(g_struDevInfo[g_iCurDevIndex].lLoginID,pFileName,hPlayWnd);
	m_hPlayHandle = NET_DVR_PlayBackByName(g_pMainDlg->m_struDeviceInfo.lLoginID,pFileName,hPlayWnd);
	if(-1 == m_hPlayHandle)
	{
		int err = NET_DVR_GetLastError();
		CString tmp;
		tmp.Format("�ط�ʧ�ܣ��������%d",err);
		MessageBox(tmp);
		return FALSE;
	}

	//���µ�ǰ�����ļ�
	m_csCurFile.Format("%s",pFileName);
	//��ȡ��֡������ʱ��
	NET_DVR_PlayBackControl(m_hPlayHandle,NET_DVR_GETTOTALFRAMES,0,&m_TotalFrames);
    NET_DVR_PlayBackControl(m_hPlayHandle,NET_DVR_GETTOTALTIME,0,&m_TotalTime);
	//���ò��ź�������Ҫ�������������������
	NET_DVR_PlayBackControl(m_hPlayHandle,NET_DVR_PLAYSTART,0,NULL);

    //����ʱ��
	SetTimer(REMOTE_PLAY_STATE_TIMER,500,NULL);

	m_IsPlaying = TRUE;
	m_IsPause = FALSE;
	
	//������
	if(NET_DVR_PlayBackControl(m_hPlayHandle,NET_DVR_PLAYSTARTAUDIO,0,NULL))
	{
		CButton *pButton = NULL;
		pButton = (CButton *)GetDlgItem(IDC_BUTTON_REMOTE_SOUND);
		pButton->SetIcon(m_hSoundStartIcon);
		GetDlgItem(IDC_BUTTON_REMOTE_SOUND)->EnableWindow(TRUE);
		m_bSound=TRUE;
	}

	//��������ť
	SetPlayState(PLAY_NORMAL);
	
	return TRUE;
}

BOOL CPlayRemoteFile::StopPlayback()
{
    if(m_hPlayHandle >= 0)
	{
		 
		 //NET_DVR_PlayBackControl(m_hPlayHandle,NET_DVR_PLAYSTOP,0,NULL);
		 if(!NET_DVR_StopPlayBack(m_hPlayHandle))
		 {
			 int err = NET_DVR_GetLastError();
			 CString tmp;
			 tmp.Format("�رջط�ʧ�ܣ��������%d",err);
		     MessageBox(tmp);
			 return FALSE;
		 }
		 //״̬��������
		 m_hPlayHandle = -1;
		 m_IsPlaying = FALSE;
		 m_IsPause = FALSE;
		 m_bChangeSpeed = FALSE;
		 m_bPlayStep = FALSE;
		 m_sliderPlay.SetPos(0);
		 m_csCurFile = "";
		 KillTimer(REMOTE_PLAY_STATE_TIMER);
		 
		 //�ر�����
		 if(NET_DVR_PlayBackControl(m_hPlayHandle,NET_DVR_PLAYSTOPAUDIO,0,NULL))
		 {
			 CButton *pButton = NULL;
			 pButton = (CButton *)GetDlgItem(IDC_BUTTON_REMOTE_SOUND);
			 pButton->SetIcon(m_hSoundStopIcon);
			 GetDlgItem(IDC_BUTTON_REMOTE_SOUND)->EnableWindow(TRUE);
			 m_bSound=FALSE;
		}

		 //��������ť
	     SetPlayState(PLAY_STOP);
		 
		 GetDlgItem(IDC_STATIC_FRAME)->SetWindowText("");

	}
	GetDlgItem(IDC_STATIC_PLAY)->Invalidate();
	return TRUE;
}


//����/��ͣ��
void CPlayRemoteFile::OnButtonPlay() 
{
	// TODO: Add your control notification handler code here

	//�ٶȵ���Ϊ����״̬
    if(m_bChangeSpeed || m_bPlayStep)
	{
		NET_DVR_PlayBackControl(m_hPlayHandle,NET_DVR_PLAYNORMAL,0,NULL);
		m_bChangeSpeed = FALSE;
		m_bPlayStep = FALSE;
	}

	if(m_IsPause)   //��ͣ״̬����������
	{
		PausePlayBack(FALSE);
		
		//��������
	}
	else            //����״̬����ͣ���򲥷��ļ�
	{
		if(m_hPlayHandle >= 0)
		{
		     PausePlayBack(TRUE);
		}
		else
		{
			StartPlayback();
		}
		
	}
}

void CPlayRemoteFile::OnButtonStop() 
{
	// TODO: Add your control notification handler code here
	StopPlayback();
}

void CPlayRemoteFile::OnButtonDownload() 
{
	
    if(m_lDownloadHandle < 0)
	{
		POSITION pos = m_ctrlFileList.GetFirstSelectedItemPosition();
		if(pos == NULL)
		{
			MessageBox("��ѡ��һ���ļ�");
			return;
		}
		int index = m_ctrlFileList.GetNextSelectedItem(pos);
		char *pFileName = g_vecFileInfo[index].sFileName;
		char SavedFileName[256]={0};
		
		strcat(SavedFileName,pFileName);
		strcat(SavedFileName,".mp4");
		
		m_lDownloadHandle = NET_DVR_GetFileByName (g_pMainDlg->m_struDeviceInfo.lLoginID,pFileName,SavedFileName);
		
		if (m_lDownloadHandle >= 0)
		{
			NET_DVR_PlayBackControl(m_lDownloadHandle, NET_DVR_PLAYSTART, 0, NULL);
			m_IsDownloading = TRUE;
			m_progressDownload.SetPos(0);
			m_progressDownload.ShowWindow(SW_SHOW);
			SetTimer(DOWNSTATE_TIMER,100,NULL);
			GetDlgItem(IDC_BUTTON_DOWNLOAD)->SetWindowText("ֹͣ");
		}
		else
		{
			MessageBox("�����ļ�ʧ��");
			return;
		}
	}
	else
	{
	    if(!NET_DVR_StopGetFile(m_lDownloadHandle))
		{
            MessageBox("ֹͣ����ʧ��");
			return;
		}
        GetDlgItem(IDC_BUTTON_DOWNLOAD)->SetWindowText("����");

	}


}


//�����б���ȡѡ�е��ļ���Ϣ
void CPlayRemoteFile::OnClickFileList(NMHDR* pNMHDR, LRESULT* pResult) 
{
// 	POSITION pos = m_ctrlFileList.GetFirstSelectedItemPosition();
// 	if(pos != NULL)
// 	{
// 	     SetPlayState(PLAY_STOP);	
// 	}
	
	*pResult = 0;
}

void CPlayRemoteFile::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{

	case DOWNSTATE_TIMER:
		{
		int DownloadPos = NET_DVR_GetDownloadPos(m_lDownloadHandle);
		if(DownloadPos>=0 && DownloadPos <100)
			m_progressDownload.SetPos(DownloadPos);
		else if (DownloadPos == 100)
		{	
			m_progressDownload.SetPos(0);
			m_IsDownloading = FALSE;
			m_progressDownload.ShowWindow(SW_HIDE);
			KillTimer(DOWNSTATE_TIMER);
			MessageBox("�������");
		}
		break;
		}
	case REMOTE_PLAY_STATE_TIMER:
		{
			DWORD PlayPos;
			NET_DVR_PlayBackControl(m_hPlayHandle,NET_DVR_PLAYGETPOS,0,&PlayPos);
			
			if(PlayPos>=0 && PlayPos<100)
			{
				m_sliderPlay.SetPos(PlayPos);
			}
			else if(PlayPos == 100)   //�������
			{
				StopPlayback();
				KillTimer(REMOTE_PLAY_STATE_TIMER);
				GetDlgItem(IDC_STATIC_FRAME)->SetWindowText("");

			}
			break;
		}
	default:
		break;
	}
	CPropertyPage::OnTimer(nIDEvent);
}


//����/��ͣ,bPlayOrPause==TURE,��ͣ,FALSE-����
BOOL CPlayRemoteFile::PausePlayBack(BOOL bPlayOrPause)
{
	if(bPlayOrPause)  //��ͣ
	{
		if(m_hPlayHandle>=0 && m_IsPlaying ==TRUE)
		{
			NET_DVR_PlayBackControl(m_hPlayHandle,NET_DVR_PLAYPAUSE,0,NULL);
			//m_CurPlayPos = 
			m_IsPause = TRUE;
			m_IsPlaying = FALSE;
			//��������ť
	        SetPlayState(PLAY_PAUSE);
		//	GetDlgItem(IDC_BUTTON_PLAY)->SetWindowText("PLAY");
		}
		
	}
	else     //����
	{
	    NET_DVR_PlayBackControl(m_hPlayHandle,NET_DVR_PLAYRESTART,0,NULL);
		m_IsPlaying = TRUE;
		m_IsPause = FALSE;
		//��������ť
	    SetPlayState(PLAY_NORMAL);
		//GetDlgItem(IDC_BUTTON_PLAY)->SetWindowText("||");
	}

	return TRUE;
}

void CPlayRemoteFile::OnButtonRemoteSlow() 
{
	if(!m_IsPlaying || m_IsPause)
		return;
	NET_DVR_PlayBackControl(m_hPlayHandle,NET_DVR_PLAYSLOW,0,NULL);
	m_bChangeSpeed = TRUE;
		
}

void CPlayRemoteFile::OnButtonRemoteFast() 
{
	if(!m_IsPlaying || m_IsPause)
		return;
	NET_DVR_PlayBackControl(m_hPlayHandle,NET_DVR_PLAYFAST,0,NULL);
	m_bChangeSpeed = TRUE;
	
}


void CPlayRemoteFile::OnButtonBack() 
{
	// TODO: Add your control notification handler code here
	if(m_hPlayHandle == -1)
		return;
	NET_DVR_PlayBackControl(m_hPlayHandle,NET_DVR_PLAYSETPOS,0,NULL);
}

void CPlayRemoteFile::OnButtonStep() 
{
	// TODO: Add your control notification handler code here
	if(!m_IsPause ||m_hPlayHandle == -1)
		return;

	NET_DVR_PlayBackControl(m_hPlayHandle,NET_DVR_PLAYFRAME,0,NULL);
	m_bPlayStep = TRUE;
}


// //¼������
// void CPlayRemoteFile::OnEditchangeComboType() 
// {
// 	// TODO: Add your control notification handler code here
// /*	int CurSel = m_coRecType.GetCurSel();
// 	if(0 == CurSel)
// 		m_RecType = 0xff;
// 	else
// 		m_RecType = CurSel - 1;*/
// }

// void CPlayRemoteFile::OnEditchangeComboFileProperty() 
// {
// 	// TODO: Add your control notification handler code here
// 	/*int CurSel = m_coFileProperty.GetCurSel();
// 	if(0 == CurSel)
// 		m_FileProperty = 0xff;
// 	else
// 		m_FileProperty = CurSel - 1;*/
// }

void CPlayRemoteFile::OnSelchangeComboType() 
{
	// TODO: Add your control notification handler code here
	int CurSel = m_coRecType.GetCurSel();
	if(0 == CurSel)
		m_RecType = 0xff;
	else
		m_RecType = CurSel - 1;
}

void CPlayRemoteFile::OnSelchangeComboFileProperty() 
{
	// TODO: Add your control notification handler code here
	int CurSel = m_coFileProperty.GetCurSel();
	if(0 == CurSel)
		m_FileProperty = 0xff;
	else
		m_FileProperty = CurSel - 1;
}

void CPlayRemoteFile::OnReleasedcaptureSliderPlay(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int pos = m_sliderPlay.GetPos();
	if(m_hPlayHandle == -1)
		return;
	NET_DVR_PlayBackControl(m_hPlayHandle,NET_DVR_PLAYSETPOS,pos,NULL);
	*pResult = 0;
}


//��ͣ�򵥲�ʱ�Ի���ˢ��
HBRUSH CPlayRemoteFile::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if(m_hPlayHandle >= 0)
	{
		
	    if(m_IsPause || m_bPlayStep)
		{
		    NET_DVR_RefreshPlay(m_hPlayHandle);
		}
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}


void CPlayRemoteFile::InitUI()
{
	// ��ʼ����ť
	CButton *pButton;

	pButton = (CButton *)GetDlgItem(IDC_BUTTON_REMOTE_PLAY);
	pButton->SetIcon(m_hPlayEnableIcon);
	pButton->EnableWindow(TRUE);
	
	pButton = (CButton *)GetDlgItem(IDC_BUTTON_REMOTE_STOP);
	pButton->SetIcon(m_hStopDisableIcon);
	pButton->EnableWindow(FALSE);
	
	pButton = (CButton *)GetDlgItem(IDC_BUTTON_REMOTE_FAST);
	pButton->SetIcon(m_hFastDisableIcon);
	pButton->EnableWindow(FALSE);
	
	pButton = (CButton *)GetDlgItem(IDC_BUTTON_REMOTE_SLOW);
	pButton->SetIcon(m_hSlowDisableIcon);
	pButton->EnableWindow(FALSE);
	
	pButton = (CButton *)GetDlgItem(IDC_BUTTON_REMOTE_STEP);
	pButton->SetIcon(m_hStepDisableIcon);
	pButton->EnableWindow(FALSE);
	
	pButton = (CButton *)GetDlgItem(IDC_BUTTON_REMOTE_BACK);
	pButton->SetIcon(m_hGotoHeadDisableIcon);
	pButton->EnableWindow(FALSE);
	
	pButton = (CButton *)GetDlgItem(IDC_BUTTON_REMOTE_PIC);
	pButton->SetIcon(m_hCaptureIcon);
	pButton->EnableWindow(FALSE);
	
	pButton = (CButton *)GetDlgItem(IDC_BUTTON_REMOTE_SOUND);
	pButton->SetIcon(m_hSoundStopIcon);
	pButton->EnableWindow(FALSE);
}

void CPlayRemoteFile::SetPlayState(int iState)
{
	CButton *pButton;
    switch(iState)
	{
	case PLAY_STOP:
		pButton = (CButton *)GetDlgItem(IDC_BUTTON_REMOTE_PLAY);
		pButton->SetIcon(m_hPlayEnableIcon);
		pButton->EnableWindow(TRUE);
		
		pButton = (CButton *)GetDlgItem(IDC_BUTTON_REMOTE_STOP);
		pButton->SetIcon(m_hStopDisableIcon);
		pButton->EnableWindow(FALSE);
		
		pButton = (CButton *)GetDlgItem(IDC_BUTTON_REMOTE_FAST);
		pButton->SetIcon(m_hFastDisableIcon);
		pButton->EnableWindow(FALSE);
		
		pButton = (CButton *)GetDlgItem(IDC_BUTTON_REMOTE_SLOW);
		pButton->SetIcon(m_hSlowDisableIcon);
		pButton->EnableWindow(FALSE);
		
		pButton = (CButton *)GetDlgItem(IDC_BUTTON_REMOTE_STEP);
		pButton->SetIcon(m_hStepDisableIcon);
		pButton->EnableWindow(FALSE);
		
		pButton = (CButton *)GetDlgItem(IDC_BUTTON_REMOTE_BACK);
		pButton->SetIcon(m_hGotoHeadDisableIcon);
		pButton->EnableWindow(FALSE);
		
		pButton = (CButton *)GetDlgItem(IDC_BUTTON_REMOTE_PIC);
		pButton->SetIcon(m_hCaptureIcon);
		pButton->EnableWindow(FALSE);
		
		pButton = (CButton *)GetDlgItem(IDC_BUTTON_REMOTE_SOUND);
		pButton->SetIcon(m_hSoundStopIcon);
	    pButton->EnableWindow(FALSE);

		break;
	case PLAY_NORMAL:
		pButton = (CButton *)GetDlgItem(IDC_BUTTON_REMOTE_PLAY);
		pButton->SetIcon(m_hPauseEnableIcon);
		pButton->EnableWindow(TRUE);
		
		pButton = (CButton *)GetDlgItem(IDC_BUTTON_REMOTE_STOP);
		pButton->SetIcon(m_hStopEnableIcon);
		pButton->EnableWindow(TRUE);
		
		pButton = (CButton *)GetDlgItem(IDC_BUTTON_REMOTE_FAST);
		pButton->SetIcon(m_hFastEnableIcon);
		pButton->EnableWindow(TRUE);
		
		pButton = (CButton *)GetDlgItem(IDC_BUTTON_REMOTE_SLOW);
		pButton->SetIcon(m_hSlowEnableIcon);
		pButton->EnableWindow(TRUE);
		
		pButton = (CButton *)GetDlgItem(IDC_BUTTON_REMOTE_STEP);
		pButton->SetIcon(m_hStepEnableIcon);
		pButton->EnableWindow(TRUE);
		
		pButton = (CButton *)GetDlgItem(IDC_BUTTON_REMOTE_BACK);
		pButton->SetIcon(m_hGotoHeadEnableIcon);
		pButton->EnableWindow(TRUE);
		
		pButton = (CButton *)GetDlgItem(IDC_BUTTON_REMOTE_PIC);
		pButton->SetIcon(m_hCaptureIcon);
		pButton->EnableWindow(TRUE);
		
		pButton = (CButton *)GetDlgItem(IDC_BUTTON_REMOTE_SOUND);
		pButton->SetIcon(m_hSoundStartIcon);
	    pButton->EnableWindow(TRUE);
		break;
	case PLAY_PAUSE:
		pButton = (CButton *)GetDlgItem(IDC_BUTTON_REMOTE_PLAY);
		pButton->SetIcon(m_hPlayEnableIcon);
		pButton->EnableWindow(TRUE);
		break;
	case PLAY_FAST:
		break;
	case PLAY_SLOW:
		break;
	default:
		break;
	}
}


void CPlayRemoteFile::OnButtonRemotePic() 
{
	if(!NET_DVR_PlayBackCaptureFile(m_hPlayHandle,"pic.bmp"))
	{
		MessageBox("ץͼʧ��!");
	}
	else
	{
		MessageBox("ץͼ�ɹ�!");
	}
	
}

void CPlayRemoteFile::OnButtonRemoteSound() 
{
	CButton *pButton = NULL; 
	if(!m_bSound)
	{
         if(NET_DVR_PlayBackControl(m_hPlayHandle,NET_DVR_PLAYSTARTAUDIO,0,NULL))
		 {
			 pButton = (CButton *)GetDlgItem(IDC_BUTTON_REMOTE_SOUND);
			 pButton->SetIcon(m_hSoundStartIcon);
			 GetDlgItem(IDC_BUTTON_REMOTE_SOUND)->EnableWindow(TRUE);
			 m_bSound=TRUE;
		 }
	}
	else
	{
		if(NET_DVR_PlayBackControl(m_hPlayHandle,NET_DVR_PLAYSTOPAUDIO,0,NULL))
		{
			pButton = (CButton *)GetDlgItem(IDC_BUTTON_REMOTE_SOUND);
			pButton->SetIcon(m_hSoundStopIcon);
			GetDlgItem(IDC_BUTTON_REMOTE_SOUND)->EnableWindow(TRUE);
			m_bSound=FALSE;
		}
	}
	
}

void CPlayRemoteFile::OnReleasedcaptureSliderVolume(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	int iPos = m_sliderVolume.GetPos();

	if(!NET_DVR_PlayBackControl(m_hPlayHandle,NET_DVR_PLAYAUDIOVOLUME,iPos,NULL))
	{
		MessageBox("��������ʧ��!");
		return;
	}
	*pResult = 0;
}

void CPlayRemoteFile::OnButtonRemoteStopSave() 
{
	if(!NET_DVR_StopPlayBackSave(m_hPlayHandle))
	{
		MessageBox("ֹͣʧ��");
		return;
	}
	
	m_bRecord = FALSE;
	
}

void CPlayRemoteFile::OnButtonRemoteSave() 
{
	if(m_hPlayHandle < 0)
	{
		MessageBox("���ȿ�ʼ�ط�!");
		return;
	}

	if(!NET_DVR_PlayBackSaveData(m_hPlayHandle,"1.mp4"))
	{
		MessageBox("����ʧ��");
		return;
	}

	m_bRecord = TRUE;

}


void CPlayRemoteFile::OnButtonLockUnlock() 
{
	// TODO: Add your control notification handler code here
	
}
