// DlgAlarmHostDong.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ClientDemo.h"
#include "DlgAlarmHostDong.h"
#include "afxdialogex.h"


// CDlgAlarmHostDong �Ի���

IMPLEMENT_DYNAMIC(CDlgAlarmHostDong, CDialogEx)

CDlgAlarmHostDong::CDlgAlarmHostDong(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgAlarmHostDong::IDD, pParent)
    , m_sFilePath(_T(""))
    , m_sSaveFilePath(_T(""))
    , m_dwChanNo(0)
    , m_dwSlotNo(0)
    , m_sOutput1(_T(""))
    , m_sOutput2(_T(""))
    , m_sOutput3(_T(""))
    , m_sOutput4(_T(""))
    , m_sOutput5(_T(""))
    , m_sOutput6(_T(""))
    , m_bSwitch(FALSE)
{
    m_iUserID = -1;
    m_iDeviceIndex = -1;
    m_lUpdownloadHandle = -1;
    m_bUploading = FALSE;
    m_bDownloading = FALSE;
}

CDlgAlarmHostDong::~CDlgAlarmHostDong()
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
    m_iUserID = -1;
    m_iDeviceIndex = -1;
    m_bUploading = FALSE;
    m_bDownloading = FALSE;
}

void CDlgAlarmHostDong::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_FILE_PATH, m_sFilePath);
    DDX_Text(pDX, IDC_EDIT_FILE_SAVE_PATH, m_sSaveFilePath);
    DDX_Text(pDX, IDC_EDIT_CHANNEL_NO, m_dwChanNo);
    DDX_Text(pDX, IDC_EDIT_SLOT_NO, m_dwSlotNo);
    DDX_Text(pDX, IDC_EDIT_OUTPUT1, m_sOutput1);
    DDX_Text(pDX, IDC_EDIT_OUTPUT2, m_sOutput2);
    DDX_Text(pDX, IDC_EDIT_OUTPUT3, m_sOutput3);
    DDX_Text(pDX, IDC_EDIT_OUTPUT4, m_sOutput4);
    DDX_Text(pDX, IDC_EDIT_OUTPUT5, m_sOutput5);
    DDX_Text(pDX, IDC_EDIT_OUTPUT6, m_sOutput6);
    DDX_Check(pDX, IDC_CHK_SWITCH, m_bSwitch);
}


BEGIN_MESSAGE_MAP(CDlgAlarmHostDong, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_CHOOSE_FILE, &CDlgAlarmHostDong::OnBnClickedBtnChooseFile)
    ON_BN_CLICKED(IDC_BTN_SAVE_FILE, &CDlgAlarmHostDong::OnBnClickedBtnSaveFile)
    ON_BN_CLICKED(IDC_BTN_IMPORT, &CDlgAlarmHostDong::OnBnClickedBtnImport)
    ON_BN_CLICKED(IDC_BTN_EXPORT, &CDlgAlarmHostDong::OnBnClickedBtnExport)
    ON_WM_TIMER()
    ON_BN_CLICKED(IDC_BTN_CONTROL, &CDlgAlarmHostDong::OnBnClickedBtnControl)
END_MESSAGE_MAP()


// CDlgAlarmHostDong ��Ϣ�������


void CDlgAlarmHostDong::OnBnClickedBtnChooseFile()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CString strFilter = _T("XML(*.xml)|*.xml|All Files(*.*)|*.*||)");
    CFileDialog fileChose(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter);
    if (fileChose.DoModal() != IDOK)
    {
        return;
    }
    m_sFilePath = fileChose.GetPathName();
    GetDlgItem(IDC_EDIT_FILE_PATH)->SetWindowText(m_sFilePath);
}


void CDlgAlarmHostDong::OnBnClickedBtnSaveFile()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CString strFilter = _T("XML(*.xml)|*.xml|All Files(*.*)|*.*||)");
    CFileDialog fileChose(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter);
    if (fileChose.DoModal() != IDOK)
    {
        return;
    }
    m_sSaveFilePath = fileChose.GetPathName();
    GetDlgItem(IDC_EDIT_FILE_SAVE_PATH)->SetWindowText(m_sSaveFilePath);
}


void CDlgAlarmHostDong::OnBnClickedBtnImport()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);

    if (m_bUploading || m_bDownloading)
    {
        return;
    }

    m_lUpdownloadHandle = NET_DVR_UploadFile_V40(m_iUserID, UPLOAD_MODBUS_CFG_FILE, NULL, 0, m_sFilePath.GetBuffer(0), NULL, 0);
    if (m_lUpdownloadHandle == -1)
    {
        DWORD dwErr = NET_DVR_GetLastError();
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_UploadFile_V40 UPLOAD_MODBUS_CFG_FILE, err[%d]", dwErr);
        return;
    }
    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_UploadFile_V40 UPLOAD_MODBUS_CFG_FILE");

    m_bUploading = TRUE;
    SetTimer(TIME_MODBUS_CFG_FILE_PROGRESS, 100, NULL);
}


void CDlgAlarmHostDong::OnBnClickedBtnExport()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);

    if (m_bUploading || m_bDownloading)
    {
        return;
    }
    CFile fileInfo;
    if (PathFileExists(m_sSaveFilePath.GetBuffer(0)))
    {
        fileInfo.Remove(m_sSaveFilePath.GetBuffer(0));
    }

    m_lUpdownloadHandle = NET_DVR_StartDownload(m_iUserID, NET_SDK_DOWNLOAD_MODBUS_CFG_FILE, NULL, 0, m_sSaveFilePath.GetBuffer(0));
    if (m_lUpdownloadHandle == -1)
    {
        DWORD dwErr = NET_DVR_GetLastError();
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_StartDownload NET_SDK_DOWNLOAD_MODBUS_CFG_FILE, err[%d]", dwErr);
        return;
    }
    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_StartDownload NET_SDK_DOWNLOAD_MODBUS_CFG_FILE");

    m_bDownloading = TRUE;
    SetTimer(TIME_MODBUS_CFG_FILE_PROGRESS, 100, NULL);
}


void CDlgAlarmHostDong::OnTimer(UINT_PTR nIDEvent)
{
    // TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
    if (nIDEvent == TIME_MODBUS_CFG_FILE_PROGRESS)
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
            KillTimer(TIME_MODBUS_CFG_FILE_PROGRESS);
            MessageBox("ModbusЭ�������ļ����뵼���ɹ�", "���뵼��", MB_OK | MB_ICONWARNING);
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
            KillTimer(TIME_MODBUS_CFG_FILE_PROGRESS);
            MessageBox("ModbusЭ�������ļ����뵼��ʧ��", "���뵼��", MB_OK | MB_ICONWARNING);
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
            KillTimer(TIME_MODBUS_CFG_FILE_PROGRESS);
            MessageBox("�����쳣", "���뵼��", MB_OK | MB_ICONWARNING);
            break;
        }
        }
    }

    CDialogEx::OnTimer(nIDEvent);
}


void CDlgAlarmHostDong::OnBnClickedBtnControl()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);

    DWORD dwChan = (m_dwChanNo << 16) | m_dwSlotNo; //���ͨ���ţ���2�ֽ�485ͨ����+��2�ֽڲ�λ�ţ�

    NET_DVR_GENERATE_OUTPUT_CTRL struOutput = { 0 };
    struOutput.dwSize = sizeof(NET_DVR_GENERATE_OUTPUT_CTRL);

    //ȡ�ַ�����ǰ8���ַ�����ΪBYTE�������ݵ�ÿһλ��ֵ���ӵ͵��ߣ�
    //���ַ������ַ���������1ʱ����Ӧ�Ķ�����λΪ1������Ϊ0
    char sTemp[8] = { 0 };
    strncpy(sTemp, m_sOutput1.GetBuffer(0), 8);
    int nCnt = 0;
    BYTE byTemp = 0x00000001;
    for (nCnt = 0; nCnt < 8; nCnt++)
    {
        if (sTemp[nCnt] > '0')
        {
            struOutput.byOutPutState1 = struOutput.byOutPutState1 | byTemp;
        }
        byTemp = byTemp << 1;
    }

    ZeroMemory(sTemp, sizeof(sTemp));
    strncpy(sTemp, m_sOutput2.GetBuffer(0), 8);
    nCnt = 0;
    byTemp = 0x00000001;
    for (nCnt = 0; nCnt < 8; nCnt++)
    {
        if (sTemp[nCnt] > '0')
        {
            struOutput.byOutPutState2 = struOutput.byOutPutState2 | byTemp;
        }
        byTemp = byTemp << 1;
    }

    ZeroMemory(sTemp, sizeof(sTemp));
    strncpy(sTemp, m_sOutput3.GetBuffer(0), 8);
    nCnt = 0;
    byTemp = 0x00000001;
    for (nCnt = 0; nCnt < 8; nCnt++)
    {
        if (sTemp[nCnt] > '0')
        {
            struOutput.byOutPutState3 = struOutput.byOutPutState3 | byTemp;
        }
        byTemp = byTemp << 1;
    }

    ZeroMemory(sTemp, sizeof(sTemp));
    strncpy(sTemp, m_sOutput4.GetBuffer(0), 8);
    nCnt = 0;
    byTemp = 0x00000001;
    for (nCnt = 0; nCnt < 8; nCnt++)
    {
        if (sTemp[nCnt] > '0')
        {
            struOutput.byOutPutState4 = struOutput.byOutPutState4 | byTemp;
        }
        byTemp = byTemp << 1;
    }

    ZeroMemory(sTemp, sizeof(sTemp));
    strncpy(sTemp, m_sOutput5.GetBuffer(0), 8);
    nCnt = 0;
    byTemp = 0x00000001;
    for (nCnt = 0; nCnt < 8; nCnt++)
    {
        if (sTemp[nCnt] > '0')
        {
            struOutput.byOutPutState5 = struOutput.byOutPutState5 | byTemp;
        }
        byTemp = byTemp << 1;
    }

    ZeroMemory(sTemp, sizeof(sTemp));
    strncpy(sTemp, m_sOutput6.GetBuffer(0), 8);
    nCnt = 0;
    byTemp = 0x00000001;
    for (nCnt = 0; nCnt < 8; nCnt++)
    {
        if (sTemp[nCnt] > '0')
        {
            struOutput.byOutPutState6 = struOutput.byOutPutState6 | byTemp;
        }
        byTemp = byTemp << 1;
    }

    if (m_bSwitch == FALSE)
    {
        struOutput.byOperateType = 2; //��
    }
    else if (m_bSwitch == TRUE)
    {
        struOutput.byOperateType = 1; //��
    }
    
    //���ýӿڿ���
    if (!NET_DVR_SetDVRConfig(m_iUserID, NET_SDK_GENERATE_OUTPUT_CONTROL, dwChan, &struOutput, sizeof(struOutput)))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_SDK_GENERATE_OUTPUT_CONTROL err[%d]", NET_DVR_GetLastError());
        return;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_SDK_GENERATE_OUTPUT_CONTROL");
    }
}
