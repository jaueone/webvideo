// DialogDoorContent.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DialogDoorContent.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogDoorContent dialog


CDialogDoorContent::CDialogDoorContent(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogDoorContent::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogDoorContent)
	m_szDownload = _T("");
	m_szUpload = _T("");
	//}}AFX_DATA_INIT

	m_iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
	m_lUserID = g_struDeviceInfo[m_iDeviceIndex].lLoginID;
	m_lUpdownloadHandle = -1;
	m_bUploading = FALSE;
	m_bDownloading = FALSE;
    m_dwFileLength=0;
    memset(&m_DoorFile,0,sizeof(m_DoorFile));
    m_DoorFile.dwSize = sizeof(m_DoorFile);
}


void CDialogDoorContent::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogDoorContent)
	DDX_Control(pDX, IDC_PROGRESS, m_ctrlProgress);
	DDX_Control(pDX, IDC_STATIC_PROGRESS, m_statProgress);
	DDX_Text(pDX, IDC_EDIT_DOWNLOAD, m_szDownload);
	DDX_Text(pDX, IDC_EDIT_UPLOAD, m_szUpload);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogDoorContent, CDialog)
	//{{AFX_MSG_MAP(CDialogDoorContent)
	ON_BN_CLICKED(IDC_BTN_SCAN, OnBtnScan)
	ON_BN_CLICKED(IDC_BTN_SCAN_D, OnBtnScanD)
	ON_BN_CLICKED(IDC_BTN_DOWNLOAD, OnBtnDownload)
	ON_BN_CLICKED(IDC_BTN_UPLOAD, OnBtnUpload)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BTN_CANCON, &CDialogDoorContent::OnBnClickedBtnCancon)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogDoorContent message handlers

void CDialogDoorContent::OnBtnScan() 
{
	// TODO: Add your control notification handler code here

	CString strFilter;
	CFileDialog fileChose(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter);
	if (fileChose.DoModal() != IDOK)
	{
		return;
	}
	m_szUpload = fileChose.GetPathName();
	CFile file;
	if (!file.Open(m_szUpload, CFile::modeRead))
	{
		char szLan1[512] = {0};
		char szLan2[512] = {0};
		g_StringLanType(szLan1, "打开文件失败", "Open file failed.");
		g_StringLanType(szLan2, "联系人配置", "Material Management");
		MessageBox(szLan1, szLan2, MB_ICONWARNING);
		return;
	}
    m_dwFileLength = file.GetLength();
    m_DoorFile.dwFileSize = m_dwFileLength;
	file.Close();
	GetDlgItem(IDC_EDIT_UPLOAD)->SetWindowText(m_szUpload);
	
}

void CDialogDoorContent::OnBtnScanD() 
{
	// TODO: Add your control notification handler code here
	CString strFilter = _T("All Files(*.*)|*.*||");
	CFileDialog fileChose(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter);
	if (fileChose.DoModal() != IDOK)
	{
		return;
	}
	m_szDownload = fileChose.GetPathName();
	GetDlgItem(IDC_EDIT_DOWNLOAD)->SetWindowText(m_szDownload);
}

void CDialogDoorContent::OnBtnDownload() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_bUploading || m_bDownloading)
	{
		return;
	}

	m_lUpdownloadHandle = NET_DVR_StartDownload(m_lUserID,NET_SDK_DOWNLOAD_DOOR_CONTENT,NULL,0,m_szDownload.GetBuffer(0));
	if(m_lUpdownloadHandle == -1)
	{
		g_pMainDlg->AddLog(m_iDeviceIndex,OPERATION_FAIL_T,"NET_DVR_StartDownload NET_SDK_DOWNLOAD_DOOR_CONTENT");
		return;
	}
	g_pMainDlg->AddLog(m_iDeviceIndex,OPERATION_SUCC_T,"NET_DVR_StartDownload NET_SDK_DOWNLOAD_DOOR_CONTENT");
	m_bDownloading = TRUE;
	m_ctrlProgress.SetPos(0);
	m_statProgress.SetWindowText(_T("0%"));
	SetTimer(TIMER_UPDOWNLOAD_CONFIG_PROGRESS, 100, NULL);
}

void CDialogDoorContent::OnBtnUpload() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
    memset(&m_DoorFile, 0, sizeof(m_DoorFile));
    m_DoorFile.dwSize = sizeof(m_DoorFile);
	strcpy((char*)m_DoorFile.byFileName, m_szUpload);
    m_DoorFile.dwFileSize = m_dwFileLength;
	if(m_bDownloading || m_bUploading)
	{
		return;
	}
	DWORD dwIndex = 0;
	m_lUpdownloadHandle = NET_DVR_UploadFile_V40(m_lUserID,UPLOAD_DOOR_CONTENT,&m_DoorFile,sizeof(m_DoorFile),m_szUpload.GetBuffer(0),&dwIndex,sizeof(dwIndex));
	if(m_lUpdownloadHandle == -1)
	{
		g_pMainDlg->AddLog(m_iDeviceIndex,OPERATION_FAIL_T,"NET_DVR_UploadFile_V40 UPLOAD_DOOR_CONTENT");
		return;
	}
	g_pMainDlg->AddLog(m_iDeviceIndex,OPERATION_SUCC_T,"NET_DVR_UploadFile_V40 UPLOAD_DOOR_CONTENT");

	m_bUploading = TRUE;
	m_ctrlProgress.SetPos(0);
	m_statProgress.SetWindowText(_T("0%"));
	SetTimer(TIMER_UPDOWNLOAD_CONFIG_PROGRESS,100,NULL);
	UpdateData(FALSE);
}



#if (_MSC_VER >= 1500)	//vs2008
void CDialogDoorContent::OnTimer(UINT_PTR nIDEvent)
#else
void CDialogDoorContent:: OnTimer(UINT nIDEvent)
#endif
{
	// TODO: Add your message handler code here and/or call default
		if (nIDEvent == TIMER_UPDOWNLOAD_CONFIG_PROGRESS)
	{
		DWORD dwProgress = 0;
		LONG lStatus = -1;
		if (m_bUploading)
		{
			lStatus = NET_DVR_GetUploadState(m_lUpdownloadHandle, &dwProgress);
			if (lStatus == -1)
			{
				g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_GetUploadState");
				if (!NET_DVR_UploadClose(m_lUpdownloadHandle))
				{
					g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_UploadClose");
				}
				else
				{
					g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_UploadClose");
				}
			}
			else
			{
				g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_GetUploadState");
			}
		}
		else if (m_bDownloading)
		{
			lStatus = NET_DVR_GetDownloadState(m_lUpdownloadHandle, &dwProgress);
			if (lStatus == -1)
			{
				g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_GetDownloadState");
				if (!NET_DVR_StopDownload(m_lUpdownloadHandle))
				{
					g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_StopDownload");
				}
				else
				{
					g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_StopDownload");
				}
			}
			else
			{
				g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_GetDownloadState");
			}
		}
		switch (lStatus)
		{
		case 1: //上传或下载成功
			{
				m_ctrlProgress.SetPos(dwProgress);
				CString str;
				str.Format("%d", dwProgress);
				str += _T("%");
				m_statProgress.SetWindowText(str);
				if (m_bUploading)
				{
					if (!NET_DVR_UploadClose(m_lUpdownloadHandle))
					{
						g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_UploadClose");
					}
					else
					{
						g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_UploadClose");
					}
					m_bUploading = FALSE;
				}
				else if (m_bDownloading)
				{
					if (!NET_DVR_StopDownload(m_lUpdownloadHandle))
					{
						g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_StopDownload");
					}
					else
					{
						g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_StopDownload");
					}
					m_bDownloading = FALSE;
				}
				m_lUpdownloadHandle = -1;
				KillTimer(TIMER_UPDOWNLOAD_CONFIG_PROGRESS);
				char sTitle[64] = {0};
				char sMsg[64] = {0};
				g_StringLanType(sTitle, "联系人信息配置", "Contents info config");
				g_StringLanType(sMsg, "上传/下载完成", "Upload/Download finished.");
				MessageBox(sMsg, sTitle, MB_OK|MB_ICONWARNING);
				break;
			}
		case 2: //正在上传或下载
			{
				m_ctrlProgress.SetPos(dwProgress);
				CString str;
				str.Format("%d", dwProgress);
				str += _T("%");
				m_statProgress.SetWindowText(str);
				break;
			}
		case 3: //上传或下载失败
			{
				if (m_bUploading)
				{
					if (!NET_DVR_UploadClose(m_lUpdownloadHandle))
					{
						g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_UploadClose");
					}
					else
					{
						g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_UploadClose");
					}
					m_bUploading = FALSE;
				}
				else if (m_bDownloading)
				{
					if (!NET_DVR_StopDownload(m_lUpdownloadHandle))
					{
						g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_StopDownload");
					}
					else
					{
						g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_StopDownload");
					}
					m_bDownloading = FALSE;
				}
				m_lUpdownloadHandle = -1;
				KillTimer(TIMER_UPDOWNLOAD_CONFIG_PROGRESS);
				char sTitle[64] = {0};
				char sMsg[64] = {0};
				g_StringLanType(sTitle, "联系人信息配置", "Contents info config");
				g_StringLanType(sMsg, "上传/下载失败", "Upload/Download failed.");
				MessageBox(sMsg, sTitle, MB_OK|MB_ICONWARNING);
				break;
			}
		case 4: //网络断开
			{
				if (m_bUploading)
				{
					if (!NET_DVR_UploadClose(m_lUpdownloadHandle))
					{
						g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_UploadClose");
					}
					else
					{
						g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_UploadClose");
					}
					m_bUploading = FALSE;
				}
				else if (m_bDownloading)
				{
					if (!NET_DVR_StopDownload(m_lUpdownloadHandle))
					{
						g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_StopDownload");
					}
					else
					{
						g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_StopDownload");
					}
					m_bDownloading = FALSE;
				}
				m_lUpdownloadHandle = -1;
				KillTimer(TIMER_UPDOWNLOAD_CONFIG_PROGRESS);
				char sTitle[64] = {0};
				char sMsg[64] = {0};
				g_StringLanType(sTitle, "联系人信息配置", "Contents info config");
				g_StringLanType(sMsg, "网络断开", "Network disconnection.");
				MessageBox(sMsg, sTitle, MB_OK|MB_ICONWARNING);
				break;
			}
		}
	}
	
	CDialog::OnTimer(nIDEvent);
}

BOOL CDialogDoorContent::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_ctrlProgress.SetRange(0, 100);
	m_ctrlProgress.SetStep(1);
	m_ctrlProgress.SetPos(0);
	m_statProgress.SetWindowText(_T("0%"));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDialogDoorContent::OnBnClickedBtnCancon()
{
    // TODO:  在此添加控件通知处理程序代码
    CDialog::OnCancel();
}


