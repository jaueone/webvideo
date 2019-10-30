// DlgDetectorRegister.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ClientDemo.h"
#include "DlgDetectorRegister.h"
#include "afxdialogex.h"

// CDlgDetectorRegister �Ի���

IMPLEMENT_DYNAMIC(CDlgDetectorRegister, CDialog)

DWORD WINAPI CDlgDetectorRegister::GetConfigThread(LPVOID lpArg)
{
    CDlgDetectorRegister* pThis = reinterpret_cast<CDlgDetectorRegister*>(lpArg);
    int bRet = 0;
    char szLan[128] = { 0 };
    while (pThis->m_bGetNext)
    {
        bRet = NET_DVR_GetNextRemoteConfig(pThis->m_lTranHandle, &pThis->m_struInfo, sizeof(pThis->m_struInfo));
        if (bRet == NET_SDK_GET_NEXT_STATUS_SUCCESS)
        {
            pThis->AddInfoToDlg();
        }
        else
        {
            if (bRet == NET_SDK_GET_NETX_STATUS_NEED_WAIT)
            {
                Sleep(5);
                continue;
            }
            if (bRet == NET_SDK_GET_NEXT_STATUS_FINISH)
            {
                g_StringLanType(szLan, "ע�����!", "Register Ending");
                g_pMainDlg->AddLog(pThis->m_iDevIndex, OPERATION_SUCC_T, szLan);
                //AfxMessageBox(szLan);
                break;
            }
            else if (bRet == NET_SDK_GET_NEXT_STATUS_FAILED)
            {
                g_StringLanType(szLan, "ע��ʧ��!", "Register failed");
                AfxMessageBox(szLan);
                break;
            }
            else
            {
                g_StringLanType(szLan, "δ֪״̬", "Unknown status");
                AfxMessageBox(szLan);
                break;
            }
        }
    }
    if (-1 != pThis->m_lTranHandle)
    {
        if (!NET_DVR_StopRemoteConfig(pThis->m_lTranHandle))
        {
            g_pMainDlg->AddLog(pThis->m_iDevIndex, OPERATION_FAIL_T, "Stop Remote Config Failed");
            pThis->m_bGetNext = FALSE;
        }
        else
        {
            g_pMainDlg->AddLog(pThis->m_iDevIndex, OPERATION_SUCC_T, "Stop Remote Config Successful");
            pThis->m_bGetNext = FALSE;
            pThis->m_lTranHandle = -1;
        }
    }
    return 0;
}

void CDlgDetectorRegister::AddInfoToDlg()
{
    //ʱ���
    if (m_struInfo.byDetectorSerialNo[0] == '\0')
    {
        return;
    }
    char sTemp[256];
    memcpy(sTemp, m_struInfo.byDetectorSerialNo,sizeof(sTemp));
    m_listData.InsertItem(m_iRowCount,sTemp);
    sprintf(sTemp, "%d", m_struInfo.dwAlarmIn);
    m_listData.SetItemText(m_iRowCount, 1, sTemp);
    if (m_struInfo.wDetectorType==0)
    {
        g_StringLanType(sTemp, "������ť", "Panic Button");
        m_listData.SetItemText(m_iRowCount, 2, sTemp);
    }
    else if (m_struInfo.wDetectorType == 1)
    {
        g_StringLanType(sTemp, "�Ŵ�", "Magnetic Contact");
        m_listData.SetItemText(m_iRowCount, 2, sTemp);
    }
    else if (m_struInfo.wDetectorType == 2)
    {
        g_StringLanType(sTemp, "�̸�̽����", "Smoke Detector");
        m_listData.SetItemText(m_iRowCount, 2, sTemp);
    }
    else if (m_struInfo.wDetectorType == 3)
    {
        g_StringLanType(sTemp, "��������̽����", "Active Infrared Detector");
        m_listData.SetItemText(m_iRowCount, 2, sTemp);
    }
    else if (m_struInfo.wDetectorType == 4)
    {
        g_StringLanType(sTemp, "��������̽����", "Passive Infrared Detector");
        m_listData.SetItemText(m_iRowCount, 2, sTemp);
    }
    else if (m_struInfo.wDetectorType == 5)
    {
        g_StringLanType(sTemp, "��������̽����", "Glass Break Detector");
        m_listData.SetItemText(m_iRowCount, 2, sTemp);
    }
    else if (m_struInfo.wDetectorType == 6)
    {
        g_StringLanType(sTemp, "��̽����", "Vibration detector");
        m_listData.SetItemText(m_iRowCount, 2, sTemp);
    }
    else if (m_struInfo.wDetectorType == 7)
    {
        g_StringLanType(sTemp, "˫��̽����", "Dual Technology PIR detector");
        m_listData.SetItemText(m_iRowCount, 2, sTemp);
    }
    else if (m_struInfo.wDetectorType == 8)
    {
        g_StringLanType(sTemp, "������̽����", "Triple Technology PIR detector");
        m_listData.SetItemText(m_iRowCount, 2, sTemp);
    }
    else if (m_struInfo.wDetectorType == 9)
    {
        g_StringLanType(sTemp, "ʪ��̽����", "Humidity Dectector");
        m_listData.SetItemText(m_iRowCount, 2, sTemp);
    }
    else if (m_struInfo.wDetectorType == 10)
    {
        g_StringLanType(sTemp, "�¸�̽����", "Temperature Dectector");
        m_listData.SetItemText(m_iRowCount, 2, sTemp);

    }
    else if (m_struInfo.wDetectorType == 11)
    {
        g_StringLanType(sTemp, "��ȼ����̽����", "Combustible Gas Detector");
        m_listData.SetItemText(m_iRowCount, 2, sTemp);

    }
    else if (m_struInfo.wDetectorType == 12)
    {
        g_StringLanType(sTemp, "�涯����", "dynamic switch");
        m_listData.SetItemText(m_iRowCount, 2, sTemp);

    }
    else if (m_struInfo.wDetectorType == 13)
    {
        g_StringLanType(sTemp, "���ƿ���", "control switch");
        m_listData.SetItemText(m_iRowCount, 2, sTemp);
    }
    else if (m_struInfo.wDetectorType == 14)
    {
        g_StringLanType(sTemp, "������", "smart lock");
        m_listData.SetItemText(m_iRowCount, 2, sTemp);
    }
    else if (m_struInfo.wDetectorType == 15)
    {
        g_StringLanType(sTemp, "ˮ��̽����", "water detector");
        m_listData.SetItemText(m_iRowCount, 2, sTemp);
    }
    else if (m_struInfo.wDetectorType == 16)
    {
        g_StringLanType(sTemp, "λ��̽����", "displacement detector");
        m_listData.SetItemText(m_iRowCount, 2, sTemp);
    }
    else if (m_struInfo.wDetectorType == 17)
    {
        g_StringLanType(sTemp, "�����Ŵ�̽����", "single infrared detector");
        m_listData.SetItemText(m_iRowCount, 2, sTemp);
    }
    else if (m_struInfo.wDetectorType == 18)
    {
        g_StringLanType(sTemp, "���ߵ�����ģ��", "wireless single zone module");
        m_listData.SetItemText(m_iRowCount, 2, sTemp);
    }
    else if (m_struInfo.wDetectorType == 19)
    {
        g_StringLanType(sTemp, "����̽����", "Other Detector");
        m_listData.SetItemText(m_iRowCount, 2, sTemp);
    }
    //UpdateData(FALSE);
}

CDlgDetectorRegister::CDlgDetectorRegister(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDetectorRegister::IDD, pParent)
{
    m_iDevIndex = 0;
    m_lServerID = -1;
    m_iRowCount = 0;
    m_lTranHandle = -1;
}

CDlgDetectorRegister::~CDlgDetectorRegister()
{
}

void CDlgDetectorRegister::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_ALL_REGISTER_DETECTOR, m_listData);
}


BEGIN_MESSAGE_MAP(CDlgDetectorRegister, CDialog)
    ON_BN_CLICKED(IDC_BTN_DETECTOR_REGISTER, &CDlgDetectorRegister::OnBnClickedBtnDetectorRegister)
END_MESSAGE_MAP()


// CDlgDetectorRegister ��Ϣ�������
BOOL CDlgDetectorRegister::OnInitDialog()
{
    CDialog::OnInitDialog();
    char szLan[128] = { 0 };
    m_listData.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
    g_StringLanType(szLan, "̽�������к�", "Serial");
    m_listData.InsertColumn(0, szLan, LVCFMT_LEFT, 150, -1);
    g_StringLanType(szLan, "������", "zone number");
    m_listData.InsertColumn(1, szLan, LVCFMT_LEFT, 300, -1);
    g_StringLanType(szLan, "����", "type");
    m_listData.InsertColumn(2, szLan, LVCFMT_LEFT, 250, -1);
    UpdateData(FALSE);
    return TRUE;
}

void CDlgDetectorRegister::OnBnClickedBtnDetectorRegister()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    m_listData.DeleteAllItems();
    m_iRowCount = 0;
    if (m_lTranHandle >= 0)
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "It is getting data, wait for a moment");
    }
    else
    {
        m_lTranHandle = NET_DVR_StartRemoteConfig(m_lServerID, NET_DVR_ALARMHOST_REGISTER_DETECTOR, NULL, 0, NULL, this);
        if (m_lTranHandle >= 0)
        {
            m_bGetNext = TRUE;
            g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "Start Remote Config successfully");
            DWORD dwThreadId;
            m_hGetInfoThread = CreateThread(NULL, 0, LPTHREAD_START_ROUTINE(GetConfigThread), this, 0, &dwThreadId);
            if (m_hGetInfoThread == NULL)
            {
                char szLan[128] = { 0 };
                g_StringLanType(szLan, "��ע���߳�ʧ��!", "open register thread Fail!");
                AfxMessageBox(szLan);
                return;
            }
            CloseHandle(m_hGetInfoThread);
        }
        else
        {
            g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "Start Remote Config failed");
            return;
        }
    }
}
