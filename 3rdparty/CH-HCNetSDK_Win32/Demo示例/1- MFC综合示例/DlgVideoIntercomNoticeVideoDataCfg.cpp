// DlgVideoIntercomNoticeVideoDataCfg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ClientDemo.h"
#include "DlgVideoIntercomNoticeVideoDataCfg.h"
#include "afxdialogex.h"

#define WM_MSG_GET_VIDEO_DATA_FINISH 1002

// CDlgVideoIntercomNoticeVideoDataCfg �Ի���

void CALLBACK g_fGetNoticeVideoDataCallback(DWORD dwType, void* lpBuffer, DWORD dwBufLen, void* pUserData);

IMPLEMENT_DYNAMIC(CDlgVideoIntercomNoticeVideoDataCfg, CDialog)

CDlgVideoIntercomNoticeVideoDataCfg::CDlgVideoIntercomNoticeVideoDataCfg(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgVideoIntercomNoticeVideoDataCfg::IDD, pParent)
{
    m_szUpload = _T("");
    m_dwFileLength = 0;
    memset(&m_struNoticeVideoData, 0, sizeof(m_struNoticeVideoData));
    m_struNoticeVideoData.dwSize = sizeof(m_struNoticeVideoData);
    m_bUploading = FALSE;
    m_lUpdownloadHandle = -1;
    m_lGetHandle = -1;
    m_dwVideoDataLen = 0;
    m_dwVideoDataReturn = 0;
    m_strNoticeNumber = _T("");
}

CDlgVideoIntercomNoticeVideoDataCfg::~CDlgVideoIntercomNoticeVideoDataCfg()
{
}

BOOL CDlgVideoIntercomNoticeVideoDataCfg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO: Add extra initialization here
    m_ctrlProgress.SetRange(0, 100);
    m_ctrlProgress.SetStep(1);
    m_ctrlProgress.SetPos(0);
    m_statProgress.SetWindowText(_T("0%"));

    return TRUE;
}

void CDlgVideoIntercomNoticeVideoDataCfg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_UPLOAD, m_szUpload);
    DDX_Control(pDX, IDC_PROGRESS, m_ctrlProgress);
    DDX_Control(pDX, IDC_STATIC_PROGRESS, m_statProgress);
    DDX_Text(pDX, IDC_EDIT_NOTICE_NUMBER, m_strNoticeNumber);
}


BEGIN_MESSAGE_MAP(CDlgVideoIntercomNoticeVideoDataCfg, CDialog)
    ON_BN_CLICKED(IDC_BTN_SCAN, &CDlgVideoIntercomNoticeVideoDataCfg::OnBnClickedBtnScan)
    ON_BN_CLICKED(IDC_BTN_UPLOAD, &CDlgVideoIntercomNoticeVideoDataCfg::OnBnClickedBtnUpload)
    ON_BN_CLICKED(IDC_BTN_START, &CDlgVideoIntercomNoticeVideoDataCfg::OnBnClickedBtnStart)
    ON_BN_CLICKED(IDC_BTN_STOP, &CDlgVideoIntercomNoticeVideoDataCfg::OnBnClickedBtnStop)
    ON_MESSAGE(WM_MSG_GET_VIDEO_DATA_FINISH, OnMsgGetVideoDataFinish)
END_MESSAGE_MAP()


// CDlgVideoIntercomNoticeVideoDataCfg ��Ϣ�������

LRESULT CDlgVideoIntercomNoticeVideoDataCfg::OnMsgGetVideoDataFinish(WPARAM wParam, LPARAM lParam)
{
    if (m_lGetHandle != -1)
    {
        NET_DVR_StopRemoteConfig(m_lGetHandle);
    }
    m_lGetHandle = -1;
    g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_GET_NOTICE_VIDEO_DATA Get finish");
    return 0;
}

void CDlgVideoIntercomNoticeVideoDataCfg::OnBnClickedBtnScan()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
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
        char szLan1[512] = { 0 };
        char szLan2[512] = { 0 };
        g_StringLanType(szLan1, "���ļ�ʧ��", "Open file failed.");
        g_StringLanType(szLan2, "��ϵ������", "Material Management");
        MessageBox(szLan1, szLan2, MB_ICONWARNING);
        return;
    }
    m_dwFileLength = file.GetLength();
    m_struNoticeVideoData.dwFileSize = m_dwFileLength;
    file.Close();
    GetDlgItem(IDC_EDIT_UPLOAD)->SetWindowText(m_szUpload);
}


void CDlgVideoIntercomNoticeVideoDataCfg::OnBnClickedBtnUpload()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);
    memset(&m_struNoticeVideoData, 0, sizeof(m_struNoticeVideoData));
    m_struNoticeVideoData.dwSize = sizeof(m_struNoticeVideoData);
    m_struNoticeVideoData.dwFileSize = m_dwFileLength;
    strncpy((char*)m_struNoticeVideoData.byNoticeNumber, m_strNoticeNumber, MAX_NOTICE_NUMBER_LEN);
    if (m_bUploading)
    {
        return;
    }
    DWORD dwIndex = 0;
    m_lUpdownloadHandle = NET_DVR_UploadFile_V40(m_lServerID, UPLOAD_NOTICE_VIDEO_DATA, &m_struNoticeVideoData, sizeof(m_struNoticeVideoData), m_szUpload.GetBuffer(0), &dwIndex, sizeof(dwIndex));
    if (m_lUpdownloadHandle == -1)
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_UploadFile_V40 UPLOAD_NOTICE_VIDEO_DATA");
        return;
    }
    g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_UploadFile_V40 UPLOAD_NOTICE_VIDEO_DATA");

    m_bUploading = TRUE;
    m_ctrlProgress.SetPos(0);
    m_statProgress.SetWindowText(_T("0%"));
    SetTimer(TIMER_UPDOWNLOAD_CONFIG_PROGRESS, 100, NULL);
    UpdateData(FALSE);
}

void CDlgVideoIntercomNoticeVideoDataCfg::OnTimer(UINT nIDEvent)
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
                g_pMainDlg->AddLog(m_lServerID, OPERATION_FAIL_T, "NET_DVR_GetUploadState");
				if (!NET_DVR_UploadClose(m_lUpdownloadHandle))
				{
                    g_pMainDlg->AddLog(m_lServerID, OPERATION_FAIL_T, "NET_DVR_UploadClose");
				}
				else
				{
                    g_pMainDlg->AddLog(m_lServerID, OPERATION_SUCC_T, "NET_DVR_UploadClose");
				}
			}
			else
			{
                g_pMainDlg->AddLog(m_lServerID, OPERATION_SUCC_T, "NET_DVR_GetUploadState");
			}
		}
		switch (lStatus)
		{
		    case 1: //�ϴ��ɹ�
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
                        g_pMainDlg->AddLog(m_lServerID, OPERATION_FAIL_T, "NET_DVR_UploadClose");
					}
					else
					{
                        g_pMainDlg->AddLog(m_lServerID, OPERATION_SUCC_T, "NET_DVR_UploadClose");
					}
					m_bUploading = FALSE;
				}
				m_lUpdownloadHandle = -1;
				KillTimer(TIMER_UPDOWNLOAD_CONFIG_PROGRESS);
				char sTitle[64] = {0};
				char sMsg[64] = {0};
				g_StringLanType(sTitle, "��ϵ����Ϣ����", "Contents info config");
				g_StringLanType(sMsg, "�ϴ�/�������", "Upload/Download finished.");
				MessageBox(sMsg, sTitle, MB_OK|MB_ICONWARNING);
				break;
			}
		    case 2: //�����ϴ�
			{
				m_ctrlProgress.SetPos(dwProgress);
				CString str;
				str.Format("%d", dwProgress);
				str += _T("%");
				m_statProgress.SetWindowText(str);
				break;
			}
		    case 3: //�ϴ�ʧ��
			{
				if (m_bUploading)
				{
					if (!NET_DVR_UploadClose(m_lUpdownloadHandle))
					{
                        g_pMainDlg->AddLog(m_lServerID, OPERATION_FAIL_T, "NET_DVR_UploadClose");
					}
					else
					{
                        g_pMainDlg->AddLog(m_lServerID, OPERATION_SUCC_T, "NET_DVR_UploadClose");
					}
					m_bUploading = FALSE;
				}
				m_lUpdownloadHandle = -1;
				KillTimer(TIMER_UPDOWNLOAD_CONFIG_PROGRESS);
				char sTitle[64] = {0};
				char sMsg[64] = {0};
				g_StringLanType(sTitle, "��ϵ����Ϣ����", "Contents info config");
				g_StringLanType(sMsg, "�ϴ�/����ʧ��", "Upload/Download failed.");
				MessageBox(sMsg, sTitle, MB_OK|MB_ICONWARNING);
				break;
			}
		    case 4: //����Ͽ�
			{
				if (m_bUploading)
				{
					if (!NET_DVR_UploadClose(m_lUpdownloadHandle))
					{
                        g_pMainDlg->AddLog(m_lServerID, OPERATION_FAIL_T, "NET_DVR_UploadClose");
					}
					else
					{
                        g_pMainDlg->AddLog(m_lServerID, OPERATION_SUCC_T, "NET_DVR_UploadClose");
					}
					m_bUploading = FALSE;
				}
				m_lUpdownloadHandle = -1;
				KillTimer(TIMER_UPDOWNLOAD_CONFIG_PROGRESS);
				char sTitle[64] = {0};
				char sMsg[64] = {0};
				g_StringLanType(sTitle, "��ϵ����Ϣ����", "Contents info config");
				g_StringLanType(sMsg, "����Ͽ�", "Network disconnection.");
				MessageBox(sMsg, sTitle, MB_OK|MB_ICONWARNING);
				break;
			}
		}
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CDlgVideoIntercomNoticeVideoDataCfg::OnBnClickedBtnStart()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    if (m_lGetHandle != -1)
    {
        NET_DVR_StopRemoteConfig(m_lGetHandle);
    }
    m_lGetHandle = -1;
    NET_DVR_NOTICE_VIDEO_DATA_COND struCond = { 0 };
    struCond.dwSize = sizeof(struCond);
    m_lGetHandle = NET_DVR_StartRemoteConfig(m_lServerID, NET_DVR_GET_NOTICE_VIDEO_DATA, &struCond, sizeof(struCond), g_fGetNoticeVideoDataCallback, this);
    if (m_lGetHandle == -1)
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_GET_NOTICE_VIDEO_DATA failed");
        return;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_GET_NOTICE_VIDEO_DATA success");
    }
}


void CDlgVideoIntercomNoticeVideoDataCfg::OnBnClickedBtnStop()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    if (m_lGetHandle != -1)
    {
        NET_DVR_StopRemoteConfig(m_lGetHandle);
    }
    m_lGetHandle = -1;
    g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_GET_NOTICE_VIDEO_DATA Get finish");
    return;
}

void CALLBACK g_fGetNoticeVideoDataCallback(DWORD dwType, void* lpBuffer, DWORD dwBufLen, void* pUserData)
{
    CDlgVideoIntercomNoticeVideoDataCfg* pDlg = (CDlgVideoIntercomNoticeVideoDataCfg*)pUserData;
    if (pDlg == NULL)
    {
        return;
    }
    pDlg->ProcessGetNoticeVideoDataCallbackData(dwType, lpBuffer, dwBufLen);
}

void CDlgVideoIntercomNoticeVideoDataCfg::ProcessGetNoticeVideoDataCallbackData(DWORD dwType, void* lpBuffer, DWORD dwBufLen)
{
    if (dwType == NET_SDK_CALLBACK_TYPE_DATA)
    {
        LPNET_DVR_NOTICE_VIDEO_DATA_CFG lpNoticeVideoDataCfg = new NET_DVR_NOTICE_VIDEO_DATA_CFG;
        memcpy(lpNoticeVideoDataCfg, lpBuffer, sizeof(*lpNoticeVideoDataCfg));

        if (lpNoticeVideoDataCfg->byDataType == 0)
        {
            g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "Data type is invalid!");
            return;
        }
        else if (lpNoticeVideoDataCfg->byDataType == 1)
        {
            if (lpNoticeVideoDataCfg->dwDataLen != sizeof(NET_DVR_NOTICE_VIDEO_DATA) || lpNoticeVideoDataCfg->pDataBuffer == NULL)
            {
                g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "Data len[%d] is not equal sizeof(NET_DVR_NOTICE_VIDEO_DATA)[%d]!", lpNoticeVideoDataCfg->dwDataLen, sizeof(NET_DVR_NOTICE_VIDEO_DATA));
                return;
            }
            g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "Start receive video data!");

            LPNET_DVR_NOTICE_VIDEO_DATA pNoticeVideoData = (LPNET_DVR_NOTICE_VIDEO_DATA)lpNoticeVideoDataCfg->pDataBuffer;
            if (pNoticeVideoData->dwSize != sizeof(NET_DVR_NOTICE_VIDEO_DATA))
            {
                g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "Size[%d] is not equal sizeof(NET_DVR_NOTICE_VIDEO_DATA)[%d]!", pNoticeVideoData->dwSize, sizeof(NET_DVR_NOTICE_VIDEO_DATA));
                return;
            }
            if (pNoticeVideoData->dwFileSize == 0)
            {
                g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "Video data size is 0!");
                return;
            }
            else
            {
                m_dwVideoDataLen = pNoticeVideoData->dwFileSize;
                //����������Ƶ�����ļ�
                char cFilename[256] = { 0 };

                SYSTEMTIME t;
                GetLocalTime(&t);
                char chTime[128];
                sprintf(chTime, "%4.4d%2.2d%2.2d%2.2d%2.2d%2.2d%3.3d", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);

                sprintf(cFilename, "%s\\VideoData", g_struLocalParam.chPictureSavePath);
                if (GetFileAttributes(cFilename) != FILE_ATTRIBUTE_DIRECTORY)
                {
                    CreateDirectory(cFilename, NULL);
                }

                sprintf(cFilename, "%s\\LocalTime[%s]Video.mp4", cFilename, chTime);
                m_hVideoDataFile = CreateFile(cFilename, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
                if (m_hVideoDataFile == INVALID_HANDLE_VALUE)
                {
                    g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "Create file failed!");
                    return;
                }
            }
        }
        else
        {
            if ((lpNoticeVideoDataCfg->dwDataLen == 0) || (lpNoticeVideoDataCfg->pDataBuffer == NULL))
            {
                g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "Video data is null!");
                return;
            }
            if (m_hVideoDataFile == INVALID_HANDLE_VALUE)
            {
                g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "Video data file handle is null!");
                return;
            }

            WriteFile(m_hVideoDataFile, lpNoticeVideoDataCfg->pDataBuffer, lpNoticeVideoDataCfg->dwDataLen, &m_dwVideoDataReturn, NULL);
            m_dwVideoDataLen -= lpNoticeVideoDataCfg->dwDataLen;
            if (m_dwVideoDataLen == 0)
            {
                g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "Receive video data success!");
                CloseHandle(m_hVideoDataFile);
                m_hVideoDataFile = INVALID_HANDLE_VALUE;
            }
        }
    }
    else if (dwType == NET_SDK_CALLBACK_TYPE_STATUS)
    {
        DWORD dwStatus = *(DWORD*)lpBuffer;
        if (dwStatus == NET_SDK_CALLBACK_STATUS_SUCCESS)
        {
            PostMessage(WM_MSG_GET_VIDEO_DATA_FINISH, 0, 0);
        }
        else if (dwStatus == NET_SDK_CALLBACK_STATUS_FAILED)
        {
            g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "Get notice video data failed!");
        }
    }
}