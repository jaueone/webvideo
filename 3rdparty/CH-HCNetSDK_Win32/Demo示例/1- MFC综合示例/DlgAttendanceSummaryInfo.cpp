// DlgAttendanceSummaryInfo.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ClientDemo.h"
#include "DlgAttendanceSummaryInfo.h"
#include "afxdialogex.h"


// CDlgAttendanceSummaryInfo �Ի���

IMPLEMENT_DYNAMIC(CDlgAttendanceSummaryInfo, CDialog)

CDlgAttendanceSummaryInfo::CDlgAttendanceSummaryInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAttendanceSummaryInfo::IDD, pParent)
{
    memset(&m_struSearchInfoCond, 0, sizeof(m_struSearchInfoCond));
    memset(&m_struAttendanceSummaryCfg, 0, sizeof(m_struAttendanceSummaryCfg));
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

CDlgAttendanceSummaryInfo::~CDlgAttendanceSummaryInfo()
{
    char szLan[128] = { 0 };

    if (m_lHandle >= 0)
    {
        if (!NET_DVR_StopRemoteConfig(m_lHandle))
        {
            g_StringLanType(szLan, "���ڻ�����Ϣ��ѯֹͣʧ��", "get attendance summary info stop failed");
            AfxMessageBox(szLan);
            return;
        }
    }
}

void CDlgAttendanceSummaryInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_COMMAND, m_cmbCommand);
    DDX_Text(pDX, IDC_EDIT_EMPLOYEENO, m_iEmployeeNo);
    DDX_Text(pDX, IDC_EDIT_NAME, m_csName);
    DDX_Control(pDX, IDC_LIST_ATTENDANCE_SUMMARY_INFO, m_lstAttendanceSummaryInfo);
    DDX_DateTimeCtrl(pDX, IDC_START_DATE, m_startDate);
    DDX_DateTimeCtrl(pDX, IDC_START_TIME, m_startTime);
    DDX_DateTimeCtrl(pDX, IDC_STOP_DATE, m_stopDate);
    DDX_DateTimeCtrl(pDX, IDC_STOP_TIME, m_stopTime);
}

BOOL CDlgAttendanceSummaryInfo::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_cmbCommand.SetCurSel(0);
    char szLanTemp[128] = { 0 };
    m_lstAttendanceSummaryInfo.SetExtendedStyle(m_lstAttendanceSummaryInfo.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES);
    g_StringLanType(szLanTemp, "����", "Employee No");
    m_lstAttendanceSummaryInfo.InsertColumn(0, szLanTemp, LVCFMT_LEFT, 40, -1);
    g_StringLanType(szLanTemp, "����", "Name");
    m_lstAttendanceSummaryInfo.InsertColumn(1, szLanTemp, LVCFMT_LEFT, 40, -1);
    g_StringLanType(szLanTemp, "��������", "Department Name");
    m_lstAttendanceSummaryInfo.InsertColumn(2, szLanTemp, LVCFMT_LEFT, 80, -1);
    g_StringLanType(szLanTemp, "��׼����ʱ��", "Work Standard");
    m_lstAttendanceSummaryInfo.InsertColumn(3, szLanTemp, LVCFMT_LEFT, 100, -1);
    g_StringLanType(szLanTemp, "ʵ�ʹ���ʱ��", "Work Actual");
    m_lstAttendanceSummaryInfo.InsertColumn(4, szLanTemp, LVCFMT_LEFT, 100, -1);
    g_StringLanType(szLanTemp, "�ٵ�����", "Late Times");
    m_lstAttendanceSummaryInfo.InsertColumn(5, szLanTemp, LVCFMT_LEFT, 80, -1);
    g_StringLanType(szLanTemp, "�ٵ��ۼ�ʱ��", "Late Minutes");
    m_lstAttendanceSummaryInfo.InsertColumn(6, szLanTemp, LVCFMT_LEFT, 100, -1);
    g_StringLanType(szLanTemp, "���˴���", "Leave Early Times");
    m_lstAttendanceSummaryInfo.InsertColumn(7, szLanTemp, LVCFMT_LEFT, 80, -1);
    g_StringLanType(szLanTemp, "�����ۼ�ʱ��", "Leave Early Minutes");
    m_lstAttendanceSummaryInfo.InsertColumn(8, szLanTemp, LVCFMT_LEFT, 100, -1);
    g_StringLanType(szLanTemp, "��׼�Ӱ�ʱ��", "Overtime Standard");
    m_lstAttendanceSummaryInfo.InsertColumn(9, szLanTemp, LVCFMT_LEFT, 100, -1);
    g_StringLanType(szLanTemp, "ʵ�ʼӰ�ʱ��", "Overtime Actual");
    m_lstAttendanceSummaryInfo.InsertColumn(10, szLanTemp, LVCFMT_LEFT, 100, -1);
    g_StringLanType(szLanTemp, "��׼��������", "Attendance Standard");
    m_lstAttendanceSummaryInfo.InsertColumn(11, szLanTemp, LVCFMT_LEFT, 100, -1);
    g_StringLanType(szLanTemp, "ʵ�ʳ�������", "Attendance Actual");
    m_lstAttendanceSummaryInfo.InsertColumn(12, szLanTemp, LVCFMT_LEFT, 100, -1);
    g_StringLanType(szLanTemp, "��������", "Absent Days");
    m_lstAttendanceSummaryInfo.InsertColumn(13, szLanTemp, LVCFMT_LEFT, 80, -1);

    return TRUE;
}

DWORD WINAPI CDlgAttendanceSummaryInfo::GetAttendanceSummaryThread(LPVOID lpAttendanceSummaryInfo)
{
    CDlgAttendanceSummaryInfo* pThis = reinterpret_cast<CDlgAttendanceSummaryInfo*>(lpAttendanceSummaryInfo);
    int iRet = 0;
    char szLan[128] = { 0 };
    char szInfoBuf[128] = { 0 };
    while (pThis->m_bGetNext)
    {
        iRet = NET_DVR_GetNextRemoteConfig(pThis->m_lHandle, &pThis->m_struAttendanceSummaryCfg, sizeof(NET_DVR_ATTENDANCE_SUMMARY_CFG));
        if (iRet == NET_SDK_GET_NEXT_STATUS_SUCCESS)
        {
            //�ɹ�������
            pThis->AddAttendanceSummaryCfg(&pThis->m_struAttendanceSummaryCfg);
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
                g_StringLanType(szLan, "���ڻ�����Ϣ��ѯ����!", "get attendance summary info finish");
                sprintf(szInfoBuf, "%s[Info Count:%d]", szLan, pThis->m_lstAttendanceSummaryInfo.GetItemCount());
                AfxMessageBox(szInfoBuf);
                break;
            }
            else if (iRet == NET_SDK_GET_NEXT_STATUS_FAILED)
            {
                g_StringLanType(szLan, "�����ӿ��ڻ�����Ϣ��ѯʧ��", "get attendance summary info failed");
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

void CDlgAttendanceSummaryInfo::AddAttendanceSummaryCfg(LPNET_DVR_ATTENDANCE_SUMMARY_CFG lpInter)
{
    int iItemCount = m_lstAttendanceSummaryInfo.GetItemCount();
    m_lstAttendanceSummaryInfo.InsertItem(iItemCount, "");

    char szStr[128] = { 0 };
    int count = 0;
    //����
    memset(szStr, 0, sizeof(szStr));
    sprintf(szStr, "%d", lpInter->dwEmployeeNo);
    m_lstAttendanceSummaryInfo.SetItemText(iItemCount, count, szStr);
    count += 1;
    //����
    memset(szStr, 0, sizeof(szStr));
    sprintf(szStr, "%s", lpInter->byName);
    m_lstAttendanceSummaryInfo.SetItemText(iItemCount, count, szStr);
    count += 1;
    //��������
    memset(szStr, 0, sizeof(szStr));
    sprintf(szStr, "%s", lpInter->byDepartmentName);
    m_lstAttendanceSummaryInfo.SetItemText(iItemCount, count, szStr);
    count += 1;
    //��׼����ʱ�䣨���ӣ�
    memset(szStr, 0, sizeof(szStr));
    sprintf(szStr, "%d", lpInter->dwWorkStandard);
    m_lstAttendanceSummaryInfo.SetItemText(iItemCount, count, szStr);
    count += 1;
    //ʵ�ʹ���ʱ�䣨���ӣ�
    memset(szStr, 0, sizeof(szStr));
    sprintf(szStr, "%d", lpInter->dwWorkActual);
    m_lstAttendanceSummaryInfo.SetItemText(iItemCount, count, szStr);
    count += 1;
    //�ٵ�����
    memset(szStr, 0, sizeof(szStr));
    sprintf(szStr, "%d", lpInter->dwLateTimes);
    m_lstAttendanceSummaryInfo.SetItemText(iItemCount, count, szStr);
    count += 1;
    //�ٵ��ۼ�ʱ�䣨���ӣ�
    memset(szStr, 0, sizeof(szStr));
    sprintf(szStr, "%d", lpInter->dwLateMinutes);
    m_lstAttendanceSummaryInfo.SetItemText(iItemCount, count, szStr);
    count += 1;
    //���˴���
    memset(szStr, 0, sizeof(szStr));
    sprintf(szStr, "%d", lpInter->dwLeaveEarlyTimes);
    m_lstAttendanceSummaryInfo.SetItemText(iItemCount, count, szStr);
    count += 1;
    //�����ۼ�ʱ�䣨���ӣ�
    memset(szStr, 0, sizeof(szStr));
    sprintf(szStr, "%d", lpInter->dwLeaveEarlyMinutes);
    m_lstAttendanceSummaryInfo.SetItemText(iItemCount, count, szStr);
    count += 1;
    //��׼�Ӱ�ʱ�䣨���ӣ�
    memset(szStr, 0, sizeof(szStr));
    sprintf(szStr, "%d", lpInter->dwOvertimeStandard);
    m_lstAttendanceSummaryInfo.SetItemText(iItemCount, count, szStr);
    count += 1;
    //ʵ�ʼӰ�ʱ�䣨���ӣ�
    memset(szStr, 0, sizeof(szStr));
    sprintf(szStr, "%d", lpInter->dwOvertimeActual);
    m_lstAttendanceSummaryInfo.SetItemText(iItemCount, count, szStr);
    count += 1;
    //��׼�����������죩
    memset(szStr, 0, sizeof(szStr));
    sprintf(szStr, "%d", lpInter->dwAttendanceStandard);
    m_lstAttendanceSummaryInfo.SetItemText(iItemCount, count, szStr);
    count += 1;
    //ʵ�ʳ����������죩
    memset(szStr, 0, sizeof(szStr));
    sprintf(szStr, "%d", lpInter->dwAttendanceActual);
    m_lstAttendanceSummaryInfo.SetItemText(iItemCount, count, szStr);
    count += 1;
    //�����������죩
    memset(szStr, 0, sizeof(szStr));
    sprintf(szStr, "%d", lpInter->dwAbsentDays);
    m_lstAttendanceSummaryInfo.SetItemText(iItemCount, count, szStr);
    count += 1;
}

BEGIN_MESSAGE_MAP(CDlgAttendanceSummaryInfo, CDialog)
    ON_BN_CLICKED(IDC_BTN_GET_ATTENDANCE_SUMMARY_INFO, &CDlgAttendanceSummaryInfo::OnBnClickedBtnGetAttendanceSummaryInfo)
    ON_BN_CLICKED(IDC_BTN_CLEAN_ATTENDANCE_SUMMARY_INFO, &CDlgAttendanceSummaryInfo::OnBnClickedBtnCleanAttendanceSummaryInfo)
    ON_CBN_SELCHANGE(IDC_COMBO_COMMAND, &CDlgAttendanceSummaryInfo::OnCbnSelchangeComboCommand)
END_MESSAGE_MAP()


// CDlgAttendanceSummaryInfo ��Ϣ�������


void CDlgAttendanceSummaryInfo::OnBnClickedBtnGetAttendanceSummaryInfo()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);

    char szLan[128] = { 0 };
    memset(&m_struSearchInfoCond, 0, sizeof(m_struSearchInfoCond));
    memset(&m_struAttendanceSummaryCfg, 0, sizeof(m_struAttendanceSummaryCfg));
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
            g_StringLanType(szLan, "���ڻ�����Ϣ��ѯֹͣʧ��", "get attendance summary info stop failed");
            AfxMessageBox(szLan);
            return;
        }
    }

    m_lHandle = NET_DVR_StartRemoteConfig(m_lServerID, NET_DVR_GET_ATTENDANCE_SUMMARY_INFO, &m_struSearchInfoCond, sizeof(m_struSearchInfoCond), NULL, NULL);
    if (m_lHandle >= 0)
    {
        m_bGetNext = TRUE;
        DWORD dwThreadId;
        m_lstAttendanceSummaryInfo.DeleteAllItems();
        m_hGetInfoThread = CreateThread(NULL, 0, LPTHREAD_START_ROUTINE(GetAttendanceSummaryThread), this, 0, &dwThreadId);
        g_StringLanType(szLan, "���ڻ�����Ϣ��ѯ�ɹ�", "get attendance summary info succeed");
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, szLan);
    }
    else
    {
        m_bGetNext = FALSE;
        g_StringLanType(szLan, "���ڻ�����Ϣ��ѯʧ��", "get attendance summary info failed");
        AfxMessageBox(szLan);
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_GET_ATTENDANCE_SUMMARY_INFO");
        return;
    }

    UpdateData(FALSE);
}


void CDlgAttendanceSummaryInfo::OnBnClickedBtnCleanAttendanceSummaryInfo()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    m_lstAttendanceSummaryInfo.DeleteAllItems();
}


void CDlgAttendanceSummaryInfo::OnCbnSelchangeComboCommand()
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
