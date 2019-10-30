// UPLOAD_PANORAMIC_STITCH.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ClientDemo.h"
#include "UPLOAD_PANORAMIC_STITCH.h"
#include "afxdialogex.h"


// CUPLOAD_PANORAMIC_STITCH �Ի���

IMPLEMENT_DYNAMIC(CUPLOAD_PANORAMIC_STITCH, CDialogEx)

CUPLOAD_PANORAMIC_STITCH::CUPLOAD_PANORAMIC_STITCH(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUPLOAD_PANORAMIC_STITCH::IDD, pParent)
{
    m_lSetPANORAMICHandle = -1;
}

CUPLOAD_PANORAMIC_STITCH::~CUPLOAD_PANORAMIC_STITCH()
{
}

void CUPLOAD_PANORAMIC_STITCH::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);

}


BEGIN_MESSAGE_MAP(CUPLOAD_PANORAMIC_STITCH, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_GET_FILEPATH, &CUPLOAD_PANORAMIC_STITCH::OnBnClickedBtnGetFilepath)
    ON_BN_CLICKED(IDC_BTN_UPLAOD, &CUPLOAD_PANORAMIC_STITCH::OnBnClickedBtnUplaod)
    ON_WM_CLOSE()
END_MESSAGE_MAP()


// CUPLOAD_PANORAMIC_STITCH ��Ϣ�������

DWORD WINAPI CUPLOAD_PANORAMIC_STITCH::UpLoadSendThread(LPVOID pParam)
{
    CUPLOAD_PANORAMIC_STITCH *pThis = (CUPLOAD_PANORAMIC_STITCH*)pParam;

    DWORD dwState = 0;
    DWORD dwProgress = 0;
    char szLan[256] = { 0 };
    while (TRUE)
    {
        dwState = NET_DVR_GetUploadState(pThis->m_lSetPANORAMICHandle, &dwProgress);
        if (dwState == 1)
        {
            pThis->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText("Upload successfully");
            pThis->GetDlgItem(IDC_BTN_UPLAOD)->EnableWindow(TRUE);
            break;
        }
        else if (dwState == 2)
        {
            sprintf(szLan, "%s%d", "Is uploading,progress:", dwProgress);
            pThis->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(szLan);
        }
        else if (dwState == 3)
        {
            pThis->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText("Upload failed");
            pThis->GetDlgItem(IDC_BTN_UPLAOD)->EnableWindow(TRUE);
            break;
        }
        else if (dwState == 4)
        {
            if (dwProgress == 100)
            {
                pThis->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText("Upload successfully");
                pThis->GetDlgItem(IDC_BTN_UPLAOD)->EnableWindow(TRUE);
                break;
            }
            else
            {
                pThis->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText("Network disconnect, status unknown");
                pThis->GetDlgItem(IDC_BTN_UPLAOD)->EnableWindow(TRUE);
                break;
            }

        }
        if (dwState != 2 && dwState != 5)
        {
            NET_DVR_UploadClose(pThis->m_lSetPANORAMICHandle);
        }
    }

    return FALSE;

}

void CUPLOAD_PANORAMIC_STITCH::OnBnClickedBtnGetFilepath()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);

    char szLan[1024] = { 0 };
    static char szFilter[] = "All File(*.*)|*.*||";
    CFileDialog dlg(TRUE, "*.*", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
    if (dlg.DoModal() == IDOK)
    {
        strFilePath = dlg.GetPathName();
        SetDlgItemText(IDC_EDIT_PANORAMIC_DIR, strFilePath);
    }

    m_hFile = CreateFile(strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == m_hFile)
    {
        g_StringLanType(szLan, "���ļ�ʧ�ܻ��޴��ļ�", "Open file failed or no this file");
        AfxMessageBox(szLan);
        return;
    }
    BY_HANDLE_FILE_INFORMATION struFileInfo = { 0 };
    if (!GetFileInformationByHandle(m_hFile, &struFileInfo))
    {
        g_StringLanType(szLan, "��ȡ�ļ���Ϣʧ��", "Get file info failed ");
        AfxMessageBox(szLan);
        return;
    }

    m_iFileSize = (((INT64)struFileInfo.nFileSizeHigh << 32)) + (INT64)struFileInfo.nFileSizeLow;
    if (m_iFileSize == 0)
    {
        g_StringLanType(szLan, "�ļ�Ϊ��", "Configure file is empty");
        AfxMessageBox(szLan);
        return;
    }
    CloseHandle(m_hFile);
    GetDlgItem(IDC_BTN_UPLAOD)->EnableWindow(TRUE);
}


BOOL CUPLOAD_PANORAMIC_STITCH::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    GetDlgItem(IDC_BTN_UPLAOD)->EnableWindow(FALSE);
    // TODO:  �ڴ���Ӷ���ĳ�ʼ��
    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣:  OCX ����ҳӦ���� FALSE
}


void CUPLOAD_PANORAMIC_STITCH::OnBnClickedBtnUplaod()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);
    char szLan[128] = { 0 };
    if (m_lSetPANORAMICHandle != -1)
    {
        NET_DVR_UploadClose(m_lSetPANORAMICHandle);
        m_lSetPANORAMICHandle = -1;
    }

    //����ѡ����
    NET_DVR_PANORAMIC_STITCH_UPLOAD struSendData = { 0 };
    struSendData.dwSize = sizeof(NET_DVR_PANORAMIC_STITCH_UPLOAD);
    struSendData.dwChannel = m_lChannel;

    m_lSetPANORAMICHandle = NET_DVR_UploadFile_V40(m_lServerID, UPLOAD_PANORAMIC_STITCH, &struSendData, sizeof(NET_DVR_PANORAMIC_STITCH_UPLOAD), strFilePath, NULL, 0);
    if (m_lSetPANORAMICHandle == -1)
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "UPLOAD_PANORAMIC_STITCH");
        m_lSetPANORAMICHandle = -1;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "UPLOAD_PANORAMIC_STITCH");
    }
    m_hUpLoadThread = CreateThread(NULL, 0, LPTHREAD_START_ROUTINE(UpLoadSendThread), this, 0, &m_dwThreadId);
    if (m_hUpLoadThread == NULL)
    {
        g_StringLanType(szLan, "���ϴ��߳�ʧ��!", "Open UpLoad thread Fail!");
        AfxMessageBox(szLan);
        return;
    }
    GetDlgItem(IDC_BTN_UPLAOD)->EnableWindow(FALSE);
}


void CUPLOAD_PANORAMIC_STITCH::OnClose()
{
    // TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
    UpdateData(TRUE);

    if (m_lSetPANORAMICHandle != -1)
    {
        NET_DVR_UploadClose(m_lSetPANORAMICHandle);
        m_lSetPANORAMICHandle = -1;
    }
    CDialogEx::OnClose();
}


