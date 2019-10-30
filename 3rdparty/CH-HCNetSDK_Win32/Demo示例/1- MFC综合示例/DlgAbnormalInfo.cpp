// DlgAbnormalInfo.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ClientDemo.h"
#include "DlgAbnormalInfo.h"
#include "afxdialogex.h"


// CDlgAbnormalInfo �Ի���

IMPLEMENT_DYNAMIC(CDlgAbnormalInfo, CDialog)

CDlgAbnormalInfo::CDlgAbnormalInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAbnormalInfo::IDD, pParent)
{
    memset(&m_struSearchInfoCond, 0, sizeof(m_struSearchInfoCond));
    memset(&m_struAbnormalCfg, 0, sizeof(m_struAbnormalCfg));
    m_iEmployeeNo = 0;
    m_csName = _T("");
    m_startDate = COleDateTime::GetCurrentTime();
    m_startTime = COleDateTime::GetCurrentTime();
    m_stopDate = COleDateTime::GetCurrentTime();
    m_stopTime = COleDateTime::GetCurrentTime();
    m_lHandle = -1;
    m_bGetNext = FALSE;
    m_hGetInfoThread = NULL;
}

CDlgAbnormalInfo::~CDlgAbnormalInfo()
{
    char szLan[128] = { 0 };

    if (m_lHandle >= 0)
    {
        if (!NET_DVR_StopRemoteConfig(m_lHandle))
        {
            g_StringLanType(szLan, "�쳣ͳ����Ϣ��ѯֹͣʧ��", "get abnormal info stop failed");
            AfxMessageBox(szLan);
            return;
        }
    }
}

void CDlgAbnormalInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_COMMAND, m_cmbCommand);
    DDX_Text(pDX, IDC_EDIT_EMPLOYEENO, m_iEmployeeNo);
    DDX_Text(pDX, IDC_EDIT_NAME, m_csName);
    DDX_Control(pDX, IDC_LIST_ABNORMAL_INFO, m_lstAbnormalInfo);
    DDX_DateTimeCtrl(pDX, IDC_START_DATE, m_startDate);
    DDX_DateTimeCtrl(pDX, IDC_START_TIME, m_startTime);
    DDX_DateTimeCtrl(pDX, IDC_STOP_DATE, m_stopDate);
    DDX_DateTimeCtrl(pDX, IDC_STOP_TIME, m_stopTime);
}

BOOL CDlgAbnormalInfo::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_cmbCommand.SetCurSel(0);
    char szLanTemp[128] = { 0 };
    m_lstAbnormalInfo.SetExtendedStyle(m_lstAbnormalInfo.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES);
    g_StringLanType(szLanTemp, "����", "Employee No");
    m_lstAbnormalInfo.InsertColumn(0, szLanTemp, LVCFMT_LEFT, 40, -1);
    g_StringLanType(szLanTemp, "����", "Name");
    m_lstAbnormalInfo.InsertColumn(1, szLanTemp, LVCFMT_LEFT, 40, -1);
    g_StringLanType(szLanTemp, "��������", "Department Name");
    m_lstAbnormalInfo.InsertColumn(2, szLanTemp, LVCFMT_LEFT, 80, -1);
    g_StringLanType(szLanTemp, "�ϰ�ʱ��1", "On Duty Time 1");
    m_lstAbnormalInfo.InsertColumn(3, szLanTemp, LVCFMT_LEFT, 100, -1);
    g_StringLanType(szLanTemp, "�°�ʱ��1", "Off Duty Time 1");
    m_lstAbnormalInfo.InsertColumn(4, szLanTemp, LVCFMT_LEFT, 100, -1);
    g_StringLanType(szLanTemp, "�ϰ�ʱ��2", "On Duty Time 2");
    m_lstAbnormalInfo.InsertColumn(5, szLanTemp, LVCFMT_LEFT, 100, -1);
    g_StringLanType(szLanTemp, "�°�ʱ��2", "Off Duty Time 2");
    m_lstAbnormalInfo.InsertColumn(6, szLanTemp, LVCFMT_LEFT, 100, -1);
    g_StringLanType(szLanTemp, "�ϰ�ʱ��3", "On Duty Time 3");
    m_lstAbnormalInfo.InsertColumn(7, szLanTemp, LVCFMT_LEFT, 100, -1);
    g_StringLanType(szLanTemp, "�°�ʱ��3", "Off Duty Time 3");
    m_lstAbnormalInfo.InsertColumn(8, szLanTemp, LVCFMT_LEFT, 100, -1);
    g_StringLanType(szLanTemp, "�ϰ�ʱ��4", "On Duty Time 4");
    m_lstAbnormalInfo.InsertColumn(9, szLanTemp, LVCFMT_LEFT, 100, -1);
    g_StringLanType(szLanTemp, "�°�ʱ��4", "Off Duty Time 4");
    m_lstAbnormalInfo.InsertColumn(10, szLanTemp, LVCFMT_LEFT, 100, -1);
    g_StringLanType(szLanTemp, "�ٵ�ʱ��", "Late Minutes");
    m_lstAbnormalInfo.InsertColumn(11, szLanTemp, LVCFMT_LEFT, 80, -1);
    g_StringLanType(szLanTemp, "����ʱ��", "Leave Early Minutes");
    m_lstAbnormalInfo.InsertColumn(12, szLanTemp, LVCFMT_LEFT, 80, -1);
    g_StringLanType(szLanTemp, "ȱ��ʱ��", "Absence Minutes");
    m_lstAbnormalInfo.InsertColumn(13, szLanTemp, LVCFMT_LEFT, 80, -1);
    g_StringLanType(szLanTemp, "�ϼ�ʱ��", "Total Minutes");
    m_lstAbnormalInfo.InsertColumn(14, szLanTemp, LVCFMT_LEFT, 80, -1);

    return TRUE;
}

DWORD WINAPI CDlgAbnormalInfo::GetAbnormalInfoThread(LPVOID lpAbnormalInfo)
{
    CDlgAbnormalInfo* pThis = reinterpret_cast<CDlgAbnormalInfo*>(lpAbnormalInfo);
    int iRet = 0;
    char szLan[128] = { 0 };
    char szInfoBuf[128] = { 0 };
    while (pThis->m_bGetNext)
    {
        iRet = NET_DVR_GetNextRemoteConfig(pThis->m_lHandle, &pThis->m_struAbnormalCfg, sizeof(NET_DVR_ABNORMAL_CFG));
        if (iRet == NET_SDK_GET_NEXT_STATUS_SUCCESS)
        {
            //�ɹ�������
            pThis->AddAbnormalInfoCfg(&pThis->m_struAbnormalCfg);
        }
        else
        {
            if (iRet == NET_SDK_GET_NETX_STATUS_NEED_WAIT)
            {
                g_pMainDlg->AddLog(0, OPERATION_SUCC_T, "���ڲ�ѯ!");
                Sleep(200);
                continue;
            }
            else if (iRet == NET_SDK_GET_NEXT_STATUS_FINISH)
            {
                g_StringLanType(szLan, "�쳣ͳ����Ϣ��ѯ����!", "get abnormal info finish");
                sprintf(szInfoBuf, "%s[Info Count:%d]", szLan, pThis->m_lstAbnormalInfo.GetItemCount());
                AfxMessageBox(szInfoBuf);
                break;
            }
            else if (iRet == NET_SDK_GET_NEXT_STATUS_FAILED)
            {
                g_StringLanType(szLan, "�������쳣ͳ����Ϣ��ѯʧ��", "get abnormal info failed");
                AfxMessageBox(szLan);
                break;
            }
            else
            {
                g_StringLanType(szLan, "δ֪״̬", "unknown status");
                AfxMessageBox(szLan);
                break;
            }
        }
    }

    return 0;
}

void CDlgAbnormalInfo::AddAbnormalInfoCfg(LPNET_DVR_ABNORMAL_CFG lpInter)
{
    int iItemCount = m_lstAbnormalInfo.GetItemCount();
    m_lstAbnormalInfo.InsertItem(iItemCount, "");

    char szStr[128] = { 0 };
    int count = 0;
    //����
    memset(szStr, 0, sizeof(szStr));
    sprintf(szStr, "%d", lpInter->dwEmployeeNo);
    m_lstAbnormalInfo.SetItemText(iItemCount, count, szStr);
    count += 1;
    //����
    memset(szStr, 0, sizeof(szStr));
    sprintf(szStr, "%s", lpInter->byName);
    m_lstAbnormalInfo.SetItemText(iItemCount, count, szStr);
    count += 1;
    //��������
    memset(szStr, 0, sizeof(szStr));
    sprintf(szStr, "%s", lpInter->byDepartmentName);
    m_lstAbnormalInfo.SetItemText(iItemCount, count, szStr);
    count += 1;
    //�쳣ʱ���
    for (int i = 0; i < ABNORMAL_INFO_NUM; ++i)
    {
        memset(szStr, 0, sizeof(szStr));
        sprintf(szStr, "%d-%d-%d %d:%d:%d", lpInter->struAttendanceTime[i].struOnDutyTime.wYear, lpInter->struAttendanceTime[i].struOnDutyTime.byMonth, \
            lpInter->struAttendanceTime[i].struOnDutyTime.byDay, lpInter->struAttendanceTime[i].struOnDutyTime.byHour, lpInter->struAttendanceTime[i].struOnDutyTime.byMinute, \
            lpInter->struAttendanceTime[i].struOnDutyTime.bySecond);
        m_lstAbnormalInfo.SetItemText(iItemCount, count, szStr);
        count += 1;
        memset(szStr, 0, sizeof(szStr));
        sprintf(szStr, "%d-%d-%d %d:%d:%d", lpInter->struAttendanceTime[i].struOffDutyTime.wYear, lpInter->struAttendanceTime[i].struOffDutyTime.byMonth, \
            lpInter->struAttendanceTime[i].struOffDutyTime.byDay, lpInter->struAttendanceTime[i].struOffDutyTime.byHour, lpInter->struAttendanceTime[i].struOffDutyTime.byMinute, \
            lpInter->struAttendanceTime[i].struOffDutyTime.bySecond);
        m_lstAbnormalInfo.SetItemText(iItemCount, count, szStr);
        count += 1;
    }
    //�ٵ�ʱ��
    memset(szStr, 0, sizeof(szStr));
    sprintf(szStr, "%d", lpInter->dwLateMinutes);
    m_lstAbnormalInfo.SetItemText(iItemCount, count, szStr);
    count += 1;
    //����ʱ��
    memset(szStr, 0, sizeof(szStr));
    sprintf(szStr, "%d", lpInter->dwLeaveEarlyMinutes);
    m_lstAbnormalInfo.SetItemText(iItemCount, count, szStr);
    count += 1;
    //ȱ��ʱ��
    memset(szStr, 0, sizeof(szStr));
    sprintf(szStr, "%d", lpInter->dwAbsenceMinutes);
    m_lstAbnormalInfo.SetItemText(iItemCount, count, szStr);
    count += 1;
    //�ϼ�ʱ��
    memset(szStr, 0, sizeof(szStr));
    sprintf(szStr, "%d", lpInter->dwTotalMinutes);
    m_lstAbnormalInfo.SetItemText(iItemCount, count, szStr);
    count += 1;
}

BEGIN_MESSAGE_MAP(CDlgAbnormalInfo, CDialog)
    ON_BN_CLICKED(IDC_BTN_GET_ABNORMAL_INFO, &CDlgAbnormalInfo::OnBnClickedBtnGetAbnormalInfo)
    ON_BN_CLICKED(IDC_BTN_CLEAN_ABNORMAL_INFO, &CDlgAbnormalInfo::OnBnClickedBtnCleanAbnormalInfo)
    ON_CBN_SELCHANGE(IDC_COMBO_COMMAND, &CDlgAbnormalInfo::OnCbnSelchangeComboCommand)
END_MESSAGE_MAP()


// CDlgAbnormalInfo ��Ϣ�������


void CDlgAbnormalInfo::OnBnClickedBtnGetAbnormalInfo()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);

    char szLan[128] = { 0 };
    memset(&m_struSearchInfoCond, 0, sizeof(m_struSearchInfoCond));
    memset(&m_struAbnormalCfg, 0, sizeof(m_struAbnormalCfg));
    m_struSearchInfoCond.byCommand = m_cmbCommand.GetCurSel();
    if (m_struSearchInfoCond.byCommand == 0)
    {
        m_struSearchInfoCond.dwEmployeeNo = m_iEmployeeNo;
        memcpy(m_struSearchInfoCond.byName, (LPCSTR)m_csName, m_csName.GetAllocLength());
    }
    m_struSearchInfoCond.struStartTime.wYear = m_startDate.GetYear();
    m_struSearchInfoCond.struStartTime.byMonth = m_startDate.GetMonth();
    m_struSearchInfoCond.struStartTime.byDay = m_startDate.GetDay();
    m_struSearchInfoCond.struStartTime.byHour = m_startTime.GetHour();
    m_struSearchInfoCond.struStartTime.byMinute = m_startTime.GetMinute();
    m_struSearchInfoCond.struStartTime.bySecond = m_startTime.GetSecond();
    m_struSearchInfoCond.struEndTime.wYear = m_stopDate.GetYear();
    m_struSearchInfoCond.struEndTime.byMonth = m_stopDate.GetMonth();
    m_struSearchInfoCond.struEndTime.byDay = m_stopDate.GetDay();
    m_struSearchInfoCond.struEndTime.byHour = m_stopTime.GetHour();
    m_struSearchInfoCond.struEndTime.byMinute = m_stopTime.GetMinute();
    m_struSearchInfoCond.struEndTime.bySecond = m_stopTime.GetSecond();

    if (m_lHandle >= 0)
    {
        if (!NET_DVR_StopRemoteConfig(m_lHandle))
        {
            g_StringLanType(szLan, "�쳣ͳ����Ϣ��ѯֹͣʧ��", "get abnormal info stop failed");
            AfxMessageBox(szLan);
            return;
        }
    }

    m_lHandle = NET_DVR_StartRemoteConfig(m_lServerID, NET_DVR_GET_ABNORMAL_INFO, &m_struSearchInfoCond, sizeof(m_struSearchInfoCond), NULL, NULL);
    if (m_lHandle >= 0)
    {
        m_bGetNext = TRUE;
        DWORD dwThreadId;
        m_lstAbnormalInfo.DeleteAllItems();
        m_hGetInfoThread = CreateThread(NULL, 0, LPTHREAD_START_ROUTINE(GetAbnormalInfoThread), this, 0, &dwThreadId);
        g_StringLanType(szLan, "�쳣ͳ����Ϣ��ѯ�ɹ�", "get abnormal info succeed");
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, szLan);
    }
    else
    {
        m_bGetNext = FALSE;
        g_StringLanType(szLan, "�쳣ͳ����Ϣ��ѯʧ��", "get abnormal info failed");
        AfxMessageBox(szLan);
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_GET_ABNORMAL_INFO");
        return;
    }

    UpdateData(FALSE);
}


void CDlgAbnormalInfo::OnBnClickedBtnCleanAbnormalInfo()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    m_lstAbnormalInfo.DeleteAllItems();
}


void CDlgAbnormalInfo::OnCbnSelchangeComboCommand()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    if (0 == m_cmbCommand.GetCurSel())
    {
        GetDlgItem(IDC_STATIC_EMPLOYEENO)->ShowWindow(true);
        GetDlgItem(IDC_STATIC_NAME)->ShowWindow(true);
        GetDlgItem(IDC_EDIT_EMPLOYEENO)->ShowWindow(true);
        GetDlgItem(IDC_EDIT_NAME)->ShowWindow(true);
    }
    else
    {
        GetDlgItem(IDC_STATIC_EMPLOYEENO)->ShowWindow(false);
        GetDlgItem(IDC_STATIC_NAME)->ShowWindow(false);
        GetDlgItem(IDC_EDIT_EMPLOYEENO)->ShowWindow(false);
        GetDlgItem(IDC_EDIT_NAME)->ShowWindow(false);
    }
}
