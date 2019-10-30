// DlgScheduleInfo.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ClientDemo.h"
#include "DlgScheduleInfo.h"
#include "afxdialogex.h"


// CDlgScheduleInfo �Ի���

IMPLEMENT_DYNAMIC(CDlgScheduleInfo, CDialog)

CDlgScheduleInfo::CDlgScheduleInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgScheduleInfo::IDD, pParent)
{
    memset(&m_struSearchInfoCond, 0, sizeof(m_struSearchInfoCond));
    memset(&m_struScheduleInfoCfg, 0, sizeof(m_struScheduleInfoCfg));
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

CDlgScheduleInfo::~CDlgScheduleInfo()
{
    char szLan[128] = { 0 };

    if (m_lHandle >= 0)
    {
        if (!NET_DVR_StopRemoteConfig(m_lHandle))
        {
            g_StringLanType(szLan, "�Ű���Ϣ��ѯֹͣʧ��", "get schedule info stop failed");
            AfxMessageBox(szLan);
            return;
        }
    }
}

void CDlgScheduleInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_COMMAND, m_cmbCommand);
    DDX_Text(pDX, IDC_EDIT_EMPLOYEENO, m_iEmployeeNo);
    DDX_Text(pDX, IDC_EDIT_NAME, m_csName);
    DDX_Control(pDX, IDC_LIST_SCHEDULE_INFO, m_lstScheduleInfo);
    DDX_DateTimeCtrl(pDX, IDC_START_DATE, m_startDate);
    DDX_DateTimeCtrl(pDX, IDC_START_TIME, m_startTime);
    DDX_DateTimeCtrl(pDX, IDC_STOP_DATE, m_stopDate);
    DDX_DateTimeCtrl(pDX, IDC_STOP_TIME, m_stopTime);
}

BOOL CDlgScheduleInfo::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_cmbCommand.SetCurSel(0);
    char szLanTemp[128] = { 0 };
    m_lstScheduleInfo.SetExtendedStyle(m_lstScheduleInfo.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES);
    g_StringLanType(szLanTemp, "����", "Employee No");
    m_lstScheduleInfo.InsertColumn(0, szLanTemp, LVCFMT_LEFT, 40, -1);
    g_StringLanType(szLanTemp, "����", "Name");
    m_lstScheduleInfo.InsertColumn(1, szLanTemp, LVCFMT_LEFT, 40, -1);
    g_StringLanType(szLanTemp, "��������", "Department Name");
    m_lstScheduleInfo.InsertColumn(2, szLanTemp, LVCFMT_LEFT, 80, -1);
    g_StringLanType(szLanTemp, "�Ű�ƻ����", "Schedule Plan No");
    m_lstScheduleInfo.InsertColumn(3, szLanTemp, LVCFMT_LEFT, 100, -1);
    g_StringLanType(szLanTemp, "�Ű�ƻ�����", "Schedule Plan Type");
    m_lstScheduleInfo.InsertColumn(4, szLanTemp, LVCFMT_LEFT, 100, -1);
    g_StringLanType(szLanTemp, "ʹ��", "Enable");
    m_lstScheduleInfo.InsertColumn(5, szLanTemp, LVCFMT_LEFT, 40, -1);
    g_StringLanType(szLanTemp, "�������(��һ)", "Schedule Type");
    m_lstScheduleInfo.InsertColumn(6, szLanTemp, LVCFMT_LEFT, 100, -1);
    g_StringLanType(szLanTemp, "��α��(��һ)", "Schedule No");
    m_lstScheduleInfo.InsertColumn(7, szLanTemp, LVCFMT_LEFT, 100, -1);
    g_StringLanType(szLanTemp, "�������(�ܶ�)", "Schedule Type");
    m_lstScheduleInfo.InsertColumn(8, szLanTemp, LVCFMT_LEFT, 100, -1);
    g_StringLanType(szLanTemp, "��α��(�ܶ�)", "Schedule No");
    m_lstScheduleInfo.InsertColumn(9, szLanTemp, LVCFMT_LEFT, 100, -1);
    g_StringLanType(szLanTemp, "�������(����)", "Schedule Type");
    m_lstScheduleInfo.InsertColumn(10, szLanTemp, LVCFMT_LEFT, 100, -1);
    g_StringLanType(szLanTemp, "��α��(����)", "Schedule No");
    m_lstScheduleInfo.InsertColumn(11, szLanTemp, LVCFMT_LEFT, 100, -1);
    g_StringLanType(szLanTemp, "�������(����)", "Schedule Type");
    m_lstScheduleInfo.InsertColumn(12, szLanTemp, LVCFMT_LEFT, 100, -1);
    g_StringLanType(szLanTemp, "��α��(����)", "Schedule No");
    m_lstScheduleInfo.InsertColumn(13, szLanTemp, LVCFMT_LEFT, 100, -1);
    g_StringLanType(szLanTemp, "�������(����)", "Schedule Type");
    m_lstScheduleInfo.InsertColumn(14, szLanTemp, LVCFMT_LEFT, 100, -1);
    g_StringLanType(szLanTemp, "��α��(����)", "Schedule No");
    m_lstScheduleInfo.InsertColumn(15, szLanTemp, LVCFMT_LEFT, 100, -1);
    g_StringLanType(szLanTemp, "�������(����)", "Schedule Type");
    m_lstScheduleInfo.InsertColumn(16, szLanTemp, LVCFMT_LEFT, 100, -1);
    g_StringLanType(szLanTemp, "��α��(����)", "Schedule No");
    m_lstScheduleInfo.InsertColumn(17, szLanTemp, LVCFMT_LEFT, 100, -1);
    g_StringLanType(szLanTemp, "�������(����)", "Schedule Type");
    m_lstScheduleInfo.InsertColumn(18, szLanTemp, LVCFMT_LEFT, 100, -1);
    g_StringLanType(szLanTemp, "��α��(����)", "Schedule No");
    m_lstScheduleInfo.InsertColumn(19, szLanTemp, LVCFMT_LEFT, 100, -1);
    g_StringLanType(szLanTemp, "��ʼʱ��", "Start Time");
    m_lstScheduleInfo.InsertColumn(20, szLanTemp, LVCFMT_LEFT, 80, -1);
    g_StringLanType(szLanTemp, "����ʱ��", "End Time");
    m_lstScheduleInfo.InsertColumn(21, szLanTemp, LVCFMT_LEFT, 80, -1);
    g_StringLanType(szLanTemp, "��������", "Holiday No");
    m_lstScheduleInfo.InsertColumn(22, szLanTemp, LVCFMT_LEFT, 80, -1);

    return TRUE;
}

DWORD WINAPI CDlgScheduleInfo::GetScheduleInfoThread(LPVOID lpScheduleInfo)
{
    CDlgScheduleInfo* pThis = reinterpret_cast<CDlgScheduleInfo*>(lpScheduleInfo);
    int iRet = 0;
    char szLan[128] = {0};
    char szInfoBuf[128] = {0};
    while (pThis->m_bGetNext)
    {
        iRet = NET_DVR_GetNextRemoteConfig(pThis->m_lHandle, &pThis->m_struScheduleInfoCfg, sizeof(NET_DVR_SCHEDULE_INFO_CFG));
        if (iRet == NET_SDK_GET_NEXT_STATUS_SUCCESS)
        {
            //�ɹ�������
            pThis->AddScheduleInfoCfg(&pThis->m_struScheduleInfoCfg);
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
                g_StringLanType(szLan, "�Ű���Ϣ��ѯ����!", "get schedule info finish");
                sprintf(szInfoBuf, "%s[Info Count:%d]", szLan, pThis->m_lstScheduleInfo.GetItemCount());
                AfxMessageBox(szInfoBuf);
                break;
            }
            else if (iRet == NET_SDK_GET_NEXT_STATUS_FAILED)
            {
                g_StringLanType(szLan, "�������Ű���Ϣ��ѯʧ��", "get schedule info failed");
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

void CDlgScheduleInfo::AddScheduleInfoCfg(LPNET_DVR_SCHEDULE_INFO_CFG lpInter)
{
    int iItemCount = m_lstScheduleInfo.GetItemCount();
    m_lstScheduleInfo.InsertItem(iItemCount, "");

    char szStr[128] = { 0 };
    int count = 0;
    //����
    memset(szStr, 0, sizeof(szStr));
    sprintf(szStr, "%d", lpInter->dwEmployeeNo);
    m_lstScheduleInfo.SetItemText(iItemCount, count, szStr);
    count += 1;
    //����
    memset(szStr, 0, sizeof(szStr));
    sprintf(szStr, "%s", lpInter->byName);
    m_lstScheduleInfo.SetItemText(iItemCount, count, szStr);
    count += 1;
    //��������
    memset(szStr, 0, sizeof(szStr));
    sprintf(szStr, "%s", lpInter->byDepartmentName);
    m_lstScheduleInfo.SetItemText(iItemCount, count, szStr);
    count += 1;
    //�Ű�ƻ����
    memset(szStr, 0, sizeof(szStr));
    sprintf(szStr, "%d", lpInter->struSchedulePlan.bySchedulePlanNo);
    m_lstScheduleInfo.SetItemText(iItemCount, count, szStr);
    count += 1;
    //�Ű�ƻ�����
    if (lpInter->struSchedulePlan.bySchedulePlanType == 0)
    {
        m_lstScheduleInfo.SetItemText(iItemCount, count, "personal");
    }
    else if (lpInter->struSchedulePlan.bySchedulePlanType == 1)
    {
        m_lstScheduleInfo.SetItemText(iItemCount, count, "department");
    }
    else
    {
        m_lstScheduleInfo.SetItemText(iItemCount, count, "error");
    }
    count += 1;
    //ʹ��
    if (lpInter->struSchedulePlan.byEnable == 0)
    {
        m_lstScheduleInfo.SetItemText(iItemCount, count, "disable");
    }
    else
    {
        m_lstScheduleInfo.SetItemText(iItemCount, count, "enable");
    }
    count += 1;
    //���ѡ��
    for (int i = 0; i < DAYS_A_WEEK; ++i)
    {
        if (lpInter->struSchedulePlan.struScheduleChoice[i].byScheduleType == 0)
        {
            m_lstScheduleInfo.SetItemText(iItemCount, count, "no class");
        }
        else if (lpInter->struSchedulePlan.struScheduleChoice[i].byScheduleType == 1)
        {
            m_lstScheduleInfo.SetItemText(iItemCount, count, "ordinary class");
        }
        else if (lpInter->struSchedulePlan.struScheduleChoice[i].byScheduleType == 2)
        {
            m_lstScheduleInfo.SetItemText(iItemCount, count, "working class");
        }
        else
        {
            m_lstScheduleInfo.SetItemText(iItemCount, count, "error");
        }
        count += 1;
        memset(szStr, 0, sizeof(szStr));
        sprintf(szStr, "%d", lpInter->struSchedulePlan.struScheduleChoice[i].byScheduleNo);
        m_lstScheduleInfo.SetItemText(iItemCount, count, szStr);
        count += 1;
    }
    //��ʼʱ��
    memset(szStr, 0, sizeof(szStr));
    sprintf(szStr, "%d-%d-%d %d:%d:%d", lpInter->struSchedulePlan.struStartTime.wYear, lpInter->struSchedulePlan.struStartTime.byMonth, \
        lpInter->struSchedulePlan.struStartTime.byDay, lpInter->struSchedulePlan.struStartTime.byHour, lpInter->struSchedulePlan.struStartTime.byMinute, \
        lpInter->struSchedulePlan.struStartTime.bySecond);
    m_lstScheduleInfo.SetItemText(iItemCount, count, szStr);
    count += 1;
    //����ʱ��
    memset(szStr, 0, sizeof(szStr));
    sprintf(szStr, "%d-%d-%d %d:%d:%d", lpInter->struSchedulePlan.struEndTime.wYear, lpInter->struSchedulePlan.struEndTime.byMonth, \
        lpInter->struSchedulePlan.struEndTime.byDay, lpInter->struSchedulePlan.struEndTime.byHour, lpInter->struSchedulePlan.struEndTime.byMinute, \
        lpInter->struSchedulePlan.struEndTime.bySecond);
    m_lstScheduleInfo.SetItemText(iItemCount, count, szStr);
    count += 1;
    //��������
    memset(szStr, 0, sizeof(szStr));
    sprintf(szStr, "%d", lpInter->struSchedulePlan.byHolidayNo);
    m_lstScheduleInfo.SetItemText(iItemCount, count, szStr);
    count += 1;
}


BEGIN_MESSAGE_MAP(CDlgScheduleInfo, CDialog)
    ON_BN_CLICKED(IDC_BTN_GET_SCHEDULE_INFO, &CDlgScheduleInfo::OnBnClickedBtnGetScheduleInfo)
    ON_BN_CLICKED(IDC_BTN_CLEAN_SCHEDULE_INFO, &CDlgScheduleInfo::OnBnClickedBtnCleanScheduleInfo)
    ON_CBN_SELCHANGE(IDC_COMBO_COMMAND, &CDlgScheduleInfo::OnCbnSelchangeComboCommand)
END_MESSAGE_MAP()


// CDlgScheduleInfo ��Ϣ�������


void CDlgScheduleInfo::OnBnClickedBtnGetScheduleInfo()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);

    char szLan[128] = {0};
    memset(&m_struSearchInfoCond, 0, sizeof(m_struSearchInfoCond));
    memset(&m_struScheduleInfoCfg, 0, sizeof(m_struScheduleInfoCfg));
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
            g_StringLanType(szLan, "�Ű���Ϣ��ѯֹͣʧ��", "get schedule info stop failed");
            AfxMessageBox(szLan);
            return;
        }
    }

    m_lHandle = NET_DVR_StartRemoteConfig(m_lServerID, NET_DVR_GET_SCHEDULE_INFO, &m_struSearchInfoCond, sizeof(m_struSearchInfoCond), NULL, NULL);
    if (m_lHandle >= 0)
    {
        m_bGetNext = TRUE;
        DWORD dwThreadId;
        m_lstScheduleInfo.DeleteAllItems();
        m_hGetInfoThread = CreateThread(NULL, 0, LPTHREAD_START_ROUTINE(GetScheduleInfoThread), this, 0, &dwThreadId);
        g_StringLanType(szLan, "�Ű���Ϣ��ѯ�ɹ�", "get schedule info succeed");
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, szLan);
    }
    else
    {
        m_bGetNext = FALSE;
        g_StringLanType(szLan, "�Ű���Ϣ��ѯʧ��", "get schedule info failed");
        AfxMessageBox(szLan);
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_GET_SCHEDULE_INFO");
        return;
    }

    UpdateData(FALSE);
}


void CDlgScheduleInfo::OnBnClickedBtnCleanScheduleInfo()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    m_lstScheduleInfo.DeleteAllItems();
}


void CDlgScheduleInfo::OnCbnSelchangeComboCommand()
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
