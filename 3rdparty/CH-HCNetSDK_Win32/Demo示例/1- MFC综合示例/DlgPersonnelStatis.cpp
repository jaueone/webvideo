// DlgPersonnelStatis.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgPersonnelStatis.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPersonnelStatis dialog


CDlgPersonnelStatis::CDlgPersonnelStatis(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPersonnelStatis::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPersonnelStatis)
	m_startDate = COleDateTime::GetCurrentTime();
	m_startTime = COleDateTime::GetCurrentTime();
	m_bAgeUserful = FALSE;
	m_bPersonNumUseful = FALSE;
	m_bSexUseful = FALSE;
	m_dwTerminalNo = 0;
	m_bEnableProgram = FALSE;
	m_dwScheduleNo = 0;
	//}}AFX_DATA_INIT

	m_lServerID = -1; // �û�ID
    m_iDevIndex = -1; // �豸����
	m_lChannel  = -1;	
	m_lHandle   = -1;
	m_bGetNext = FALSE;
	m_hGetInfoThread = NULL;

	memset(&m_struStatisticsResult, 0, sizeof(NET_DVR_FACECAPTURE_STATISTICS_RESULT));
	memset(&m_struStatisticsCond, 0, sizeof(NET_DVR_FACECAPTURE_STATISTICS_COND));
}


void CDlgPersonnelStatis::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPersonnelStatis)
	DDX_Control(pDX, IDC_COMBO_REPORT_TYPE, m_comPerortType);
	DDX_Control(pDX, IDC_LIST_PERSTATISTICS, m_lPerStatistics);
	DDX_DateTimeCtrl(pDX, IDC_START_DATE, m_startDate);
	DDX_DateTimeCtrl(pDX, IDC_START_TIME, m_startTime);
	DDX_Check(pDX, IDC_CHECK_AGE, m_bAgeUserful);
	DDX_Check(pDX, IDC_CHECK_NUM, m_bPersonNumUseful);
	DDX_Check(pDX, IDC_CHECK_SEX, m_bSexUseful);
	DDX_Text(pDX, IDC_EDIT_TERMINAL_NO, m_dwTerminalNo);
	DDX_Check(pDX, IDC_CHK_PROGRAM, m_bEnableProgram);
	DDX_Text(pDX, IDC_EDIT_SCHEDULE, m_dwScheduleNo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPersonnelStatis, CDialog)
	//{{AFX_MSG_MAP(CDlgPersonnelStatis)
	ON_BN_CLICKED(IDC_BTN_GET_PERSONNEL_STATISITCS, OnBtnGetPersonnelStatisitcs)
	ON_BN_CLICKED(IDC_BTN_CLEAN_HEATMAP_INFO, OnBtnCleanHeatmapInfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPersonnelStatis message handlers

BOOL CDlgPersonnelStatis::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (g_struDeviceInfo[m_iDevIndex].iDeviceType == DS_D60S) //��Ϣ����������
	{
		GetDlgItem(IDC_EDIT_TERMINAL_NO)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_TERMINAL_NO)->EnableWindow(FALSE);
	}
	
	char szLanTemp[128] = {0};
	m_lPerStatistics.SetExtendedStyle(m_lPerStatistics.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_SUBITEMIMAGES);
	g_StringLanType(szLanTemp, "����ͳ��", "dwPeopleNum");
    m_lPerStatistics.InsertColumn(0, szLanTemp, LVCFMT_LEFT, 80, -1);
	g_StringLanType(szLanTemp, "����", "dwTeenage");
	m_lPerStatistics.InsertColumn(1, szLanTemp, LVCFMT_LEFT, 80, -1);
	g_StringLanType(szLanTemp, "����", "dwYouth");
    m_lPerStatistics.InsertColumn(2, szLanTemp, LVCFMT_LEFT, 80, -1);
	g_StringLanType(szLanTemp, "����", "dwMidLife");
    m_lPerStatistics.InsertColumn(3, szLanTemp, LVCFMT_LEFT, 80, -1);
	g_StringLanType(szLanTemp, "����", "dwElderly");
	m_lPerStatistics.InsertColumn(4, szLanTemp, LVCFMT_LEFT, 80, -1);
    g_StringLanType(szLanTemp, "��ͯ", "Child");
    m_lPerStatistics.InsertColumn(5, szLanTemp, LVCFMT_LEFT, 80, -1);
    g_StringLanType(szLanTemp, "������", "Adolescent");
    m_lPerStatistics.InsertColumn(6, szLanTemp, LVCFMT_LEFT, 80, -1);
    g_StringLanType(szLanTemp, "׳��", "Prime");
    m_lPerStatistics.InsertColumn(7, szLanTemp, LVCFMT_LEFT, 80, -1);
    g_StringLanType(szLanTemp, "������", "Midage");
    m_lPerStatistics.InsertColumn(8, szLanTemp, LVCFMT_LEFT, 80, -1);
	g_StringLanType(szLanTemp, "��", "dwMale");
    m_lPerStatistics.InsertColumn(9, szLanTemp, LVCFMT_LEFT, 80, -1);
	g_StringLanType(szLanTemp, "Ů", "dwFemale");
    m_lPerStatistics.InsertColumn(10, szLanTemp, LVCFMT_LEFT, 80, -1);
    g_StringLanType(szLanTemp, "��������", "stat type");
    m_lPerStatistics.InsertColumn(11, szLanTemp, LVCFMT_LEFT, 80, -1);
	g_StringLanType(szLanTemp, "��Ŀ���", "Program No.");
    m_lPerStatistics.InsertColumn(12, szLanTemp, LVCFMT_LEFT, 80, -1);
	g_StringLanType(szLanTemp, "��Ŀ����", "Program Name");
    m_lPerStatistics.InsertColumn(13, szLanTemp, LVCFMT_LEFT, 120, -1);
	
	return TRUE;
}

void CDlgPersonnelStatis::AddPersonnelStatisticsLst(LPNET_DVR_FACECAPTURE_STATISTICS_RESULT lpInter)
{
	int iItemCount = m_lPerStatistics.GetItemCount();
	m_lPerStatistics.InsertItem(iItemCount, "");
	
	char szStr[128] = {0};
	
	//����ͳ��
	memset(szStr, 0, sizeof(szStr));
	sprintf(szStr, "%d", lpInter->dwPeopleNum);
	m_lPerStatistics.SetItemText(iItemCount, 0, szStr);

	//���������ͳ��
	memset(szStr, 0, sizeof(szStr));
	sprintf(szStr, "%d", lpInter->struAgeGroupParam.dwTeenage);
	m_lPerStatistics.SetItemText(iItemCount, 1, szStr);

	memset(szStr, 0, sizeof(szStr));
	sprintf(szStr, "%d", lpInter->struAgeGroupParam.dwYouth);
	m_lPerStatistics.SetItemText(iItemCount, 2, szStr);

	memset(szStr, 0, sizeof(szStr));
	sprintf(szStr, "%d", lpInter->struAgeGroupParam.dwMidLife);
	m_lPerStatistics.SetItemText(iItemCount, 3, szStr);

	memset(szStr, 0, sizeof(szStr));
	sprintf(szStr, "%d", lpInter->struAgeGroupParam.dwElderly);
	m_lPerStatistics.SetItemText(iItemCount, 4, szStr);
	
    memset(szStr, 0, sizeof(szStr));
    sprintf(szStr, "%d", lpInter->struAgeGroupParam.dwChild);
    m_lPerStatistics.SetItemText(iItemCount, 5, szStr);

    memset(szStr, 0, sizeof(szStr));
    sprintf(szStr, "%d", lpInter->struAgeGroupParam.dwAdolescent);
    m_lPerStatistics.SetItemText(iItemCount, 6, szStr);

    memset(szStr, 0, sizeof(szStr));
    sprintf(szStr, "%d", lpInter->struAgeGroupParam.dwPrime);
    m_lPerStatistics.SetItemText(iItemCount, 7, szStr);

    memset(szStr, 0, sizeof(szStr));
    sprintf(szStr, "%d", lpInter->struAgeGroupParam.dwMidage);
    m_lPerStatistics.SetItemText(iItemCount, 8, szStr);

	//�Ա�����ͳ��
	memset(szStr, 0, sizeof(szStr));
	sprintf(szStr, "%d", lpInter->struSexGroupParam.dwMale);
	m_lPerStatistics.SetItemText(iItemCount, 9, szStr);

	memset(szStr, 0, sizeof(szStr));
	sprintf(szStr, "%d", lpInter->struSexGroupParam.dwFemale);
	m_lPerStatistics.SetItemText(iItemCount, 10, szStr);

    memset(szStr, 0, sizeof(szStr));


    char szAge[100] = {0};
    char szSex[100] = {0};
    char szNum[100] = {0};

    g_StringLanType(szAge, "�����,", "Age,");
    g_StringLanType(szSex, "�Ա�,", "Sex,");
    g_StringLanType(szNum, "��Ա����,", "people num,");



    if (lpInter->byStatType&1)
    {
        strcat(szStr, szAge);
    }

    if ((lpInter->byStatType>>1)&1)
    {
        strcat(szStr, szSex);
    }

    if((lpInter->byStatType>>2)&1)
    {
        strcat(szStr, szNum);
    }

    m_lPerStatistics.SetItemText(iItemCount, 11, szStr);

	//��Ŀ��Ϣ
	memset(szStr, 0, sizeof(szStr));
	sprintf(szStr, "%d", lpInter->struProgramInfo.dwProgramNo);
	m_lPerStatistics.SetItemText(iItemCount, 12, szStr);

	m_lPerStatistics.SetItemText(iItemCount, 13, (char*)lpInter->struProgramInfo.sProgramName);
}

DWORD WINAPI CDlgPersonnelStatis::GetPersonnelStatisticsThread(LPVOID lpPersonnelStatistics)
{
	CDlgPersonnelStatis* pThis = reinterpret_cast<CDlgPersonnelStatis*>(lpPersonnelStatistics);
	int iRet = 0;
	char szLan[128] = {0};
	char szInfoBuf[128] = {0};
	while (pThis->m_bGetNext)
	{
		iRet = NET_DVR_GetNextRemoteConfig(pThis->m_lHandle, &pThis->m_struStatisticsResult, sizeof(NET_DVR_FACECAPTURE_STATISTICS_RESULT));
		if (iRet == NET_SDK_CALLBACK_STATUS_SUCCESS)
		{
			pThis->AddPersonnelStatisticsLst(&pThis->m_struStatisticsResult);
		}
		else
		{
			if (iRet == NET_SDK_GET_NETX_STATUS_NEED_WAIT)
			{
                //g_StringLanType(szLan, "���ڲ�ѯ!", "getting....");
                g_pMainDlg->AddLog(0, OPERATION_SUCC_T, "���ڲ�ѯ!");
				Sleep(200);
				continue;
			}
			if (iRet == NET_SDK_GET_NEXT_STATUS_FINISH)
			{
				g_StringLanType(szLan, "��Աͳ�����ݲ�ѯ����!", "Get Personnel Statistics List Finish");
				sprintf(szInfoBuf, "%s[Info Count:%d]", szLan, pThis->m_lPerStatistics.GetItemCount());
				AfxMessageBox(szInfoBuf);
				break;
			}
			else if(iRet == NET_SDK_GET_NEXT_STATUS_FAILED)
			{
				g_StringLanType(szLan, "��������Աͳ�����ݲ�ѯʧ��", "Get Personnel Statistics List failed");
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
	return 0;
}

void CDlgPersonnelStatis::OnBtnGetPersonnelStatisitcs() 
{
	UpdateData(TRUE);
	char szLan[128] = {0};
	m_struStatisticsCond.dwSize = sizeof(m_struStatisticsCond);
	m_struStatisticsCond.dwChannel = m_dwTerminalNo<<16 | (DWORD)m_lChannel;
	m_struStatisticsCond.struStartTime.wYear = m_startDate.GetYear();
	m_struStatisticsCond.struStartTime.byMonth = m_startDate.GetMonth();
	m_struStatisticsCond.struStartTime.byDay = m_startDate.GetDay();
	m_struStatisticsCond.struStartTime.byHour = m_startTime.GetHour();
	m_struStatisticsCond.struStartTime.byMinute = m_startTime.GetMinute();
	m_struStatisticsCond.struStartTime.bySecond = m_startTime.GetSecond();
	
	
	m_struStatisticsCond.byReportType = m_comPerortType.GetCurSel() + 1;
	m_struStatisticsCond.byStatType = 0;

    if (m_bAgeUserful)
    {
        m_struStatisticsCond.byStatType = m_struStatisticsCond.byStatType | 1;
    }

    if (m_bSexUseful)
    {
        m_struStatisticsCond.byStatType = m_struStatisticsCond.byStatType | 1<<1;
    }

    if (m_bPersonNumUseful)
    {
        m_struStatisticsCond.byStatType = m_struStatisticsCond.byStatType | 1<<2;

    }

	m_struStatisticsCond.byEnableProgramStatistics = m_bEnableProgram;
	m_struStatisticsCond.dwPlayScheduleNo = m_dwScheduleNo;

	if (m_lHandle >= 0)
	{
		if (!NET_DVR_StopRemoteConfig(m_lHandle))
		{
			m_bGetNext = FALSE;
			g_StringLanType(szLan, "��Աͳ�Ʋ�ѯֹͣʧ��", "Personnel Statistics List Stop Failed");
			AfxMessageBox(szLan);
			return;
		}
	} 
	
	m_lHandle = NET_DVR_StartRemoteConfig(m_lServerID, NET_DVR_FACECAPTURE_STATISTICS, &m_struStatisticsCond, sizeof(m_struStatisticsCond), NULL, NULL);
	if (m_lHandle >= 0)
	{
		m_bGetNext = TRUE;
		DWORD dwThreadId;
		m_lPerStatistics.DeleteAllItems();
		m_hGetInfoThread = CreateThread(NULL, 0, LPTHREAD_START_ROUTINE(GetPersonnelStatisticsThread), this, 0, &dwThreadId);
        g_StringLanType(szLan, "��Աͳ�����ݲ�ѯ�ɹ�", "Personnel Statistics List succ");
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, szLan);
	}
	else
	{
		m_bGetNext = FALSE;
		g_StringLanType(szLan, "��Աͳ�����ݲ�ѯʧ��", "Personnel Statistics List failed");
		AfxMessageBox(szLan);
		g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_FACECAPTURE_STATISTICS");
		return;
	}

	UpdateData(FALSE);
	
}

void CDlgPersonnelStatis::OnBtnCleanHeatmapInfo() 
{
	// TODO: Add your control notification handler code here

	m_lPerStatistics.DeleteAllItems();
}
