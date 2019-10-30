// DlgAudioControlFile.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ClientDemo.h"
#include "DlgAudioControlFile.h"
#include "afxdialogex.h"


// CDlgAudioControlFile �Ի���

IMPLEMENT_DYNAMIC(CDlgAudioControlFile, CDialog)

CDlgAudioControlFile::CDlgAudioControlFile(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAudioControlFile::IDD, pParent)
    , m_szAudioFilePath(_T(""))
{
    m_lUpdownloadHandle = -1;
    m_bUploading = FALSE;
}

CDlgAudioControlFile::~CDlgAudioControlFile()
{
}

void CDlgAudioControlFile::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);

    DDX_Text(pDX, IDC_EDIT_AUDIO_CONTROL_FILE_PATH, m_szAudioFilePath);
}


BEGIN_MESSAGE_MAP(CDlgAudioControlFile, CDialog)
    ON_BN_CLICKED(IDC_BTN_OPEN_AUDIO_FILE, &CDlgAudioControlFile::OnBnClickedBtnOpenAudioFile)
    ON_BN_CLICKED(IDC_BTN_AUDIO_CONTROL_FILE_IMPORT, &CDlgAudioControlFile::OnBnClickedBtnAudioControlFileImport)
    ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgAudioControlFile ��Ϣ�������


BOOL CDlgAudioControlFile::OnInitDialog()
{
    CDialog::OnInitDialog();

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAudioControlFile::OnBnClickedBtnOpenAudioFile()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CString strFilter = _T("wav(*.wav)|*.xml|All Files(*.*)|*.*||)");
    CFileDialog fileChose(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter);
    if (fileChose.DoModal() != IDOK)
    {
        return;
    }
    m_szAudioFilePath = fileChose.GetPathName();
    GetDlgItem(IDC_EDIT_AUDIO_CONTROL_FILE_PATH)->SetWindowText(m_szAudioFilePath);
}


void CDlgAudioControlFile::OnBnClickedBtnAudioControlFileImport()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������

    UpdateData(TRUE);

    if (m_bUploading)
    {
        return;
    }
    if (strlen((LPCTSTR)m_szAudioFilePath) == 0)
    {
        return;
    }
    char szFilePath[260] = { 0 };
    memcpy(szFilePath, (LPCTSTR)m_szAudioFilePath, strlen(m_szAudioFilePath));

    NET_DVR_TRANS_AUDIO_INFO struTransAudioInfo = { 0 };
    struTransAudioInfo.dwSize = sizeof(NET_DVR_TRANS_AUDIO_INFO);
    char* pFileName = NULL;
    pFileName = strrchr(szFilePath, '\\') + 1;
    memcpy(struTransAudioInfo.sAudioName, pFileName, strlen(pFileName));
    struTransAudioInfo.byAudioFormat = 0;


    m_lUpdownloadHandle = NET_DVR_UploadFile_V40(m_iUserID, UPLOAD_ASR_CONTROL_FILE, &struTransAudioInfo, sizeof(NET_DVR_TRANS_AUDIO_INFO), m_szAudioFilePath.GetBuffer(0), NULL, 0);
    if (m_lUpdownloadHandle == -1)
    {
        DWORD dwErr = NET_DVR_GetLastError();
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_UploadFile_V40 UPLOAD_ASR_CONTROL_FILE, err[%d]", dwErr);
        return;
    }
    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_UploadFile_V40 UPLOAD_ASR_CONTROL_FILE");

    m_bUploading = TRUE;
    SetTimer(TIME_AUDIO_CONTROL_FILE_PROGRESS, 100, NULL);
}


void CDlgAudioControlFile::OnTimer(UINT_PTR nIDEvent)
{
    // TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

    if (nIDEvent == TIME_AUDIO_CONTROL_FILE_PROGRESS)
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
        
        switch (lStatus)
        {
            case 1: //�ϴ������سɹ�
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
                   
                m_lUpdownloadHandle = -1;
                KillTimer(TIME_AUDIO_CONTROL_FILE_PROGRESS);
                MessageBox("��Ƶ�����ļ�����ɹ�", "����", MB_OK | MB_ICONWARNING);
                break;
            }
            case 2: //�����ϴ�������
            {
                break;
            }
            case 3: //�ϴ�������ʧ��
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
                    
                m_lUpdownloadHandle = -1;
                KillTimer(TIME_AUDIO_CONTROL_FILE_PROGRESS);
                MessageBox("��Ƶ�����ļ�����ʧ��", "����", MB_OK | MB_ICONWARNING);
                break;
            }
            case 4: //����Ͽ�
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
                    
                m_lUpdownloadHandle = -1;
                KillTimer(TIME_AUDIO_CONTROL_FILE_PROGRESS);
                MessageBox("�����쳣", "����", MB_OK | MB_ICONWARNING);
                break;
            }
        }
    }

    CDialog::OnTimer(nIDEvent);
}
