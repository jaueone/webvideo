// DlgPdcResult.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgPdcResult.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPdcResult dialog


CDlgPdcResult::CDlgPdcResult(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPdcResult::IDD, pParent)
    , m_bTriggerPeopleCountingData(FALSE)
    , m_bMultiChannelSearch(FALSE)
{
	//{{AFX_DATA_INIT(CDlgPdcResult)
	m_startDate = COleDateTime::GetCurrentTime();
	m_startTime = COleDateTime::GetCurrentTime();
	m_stopDate = COleDateTime::GetCurrentTime();
	m_stopTime = COleDateTime::GetCurrentTime();
	m_dwTerminalNo = 0;
	m_bEnableProgram = FALSE;
	m_dwScheduleNo = 0;
	//}}AFX_DATA_INIT

	m_lServerID = -1; // �û�ID
    m_iDevIndex = -1; // �豸����
	m_lChannel = -1;

	m_lHandle = -1;
	m_bGetNext = FALSE;
//	m_pStruPdcResult = NULL;
	m_hGetInfoThread = NULL;
	m_iStruCount = 0;
}

CDlgPdcResult::~CDlgPdcResult()
{
	char szLan[128] = {0};
	
	if (m_lHandle >= 0)
	{
		if (!NET_DVR_StopRemoteConfig(m_lHandle))
		{
			g_StringLanType(szLan, "���������ݲ�ѯֹͣʧ��", "Get Pdc Result List Stop Failed");
			AfxMessageBox(szLan);
			return;
		}
	} 
	
// 	if (m_pStruPdcResult != NULL)
// 	{
// 		delete [] m_pStruPdcResult;
// 		m_pStruPdcResult = NULL;
// 	}
}

void CDlgPdcResult::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPdcResult)
	DDX_Control(pDX, IDC_COMBO_CHANNEL, m_cmbChannel);
	DDX_Control(pDX, IDC_COMBO_REPORT_TYPE, m_comReportType);
	DDX_Control(pDX, IDC_LIST_PDC_RESULT, m_lstPdcResult);
	DDX_DateTimeCtrl(pDX, IDC_START_DATE, m_startDate);
	DDX_DateTimeCtrl(pDX, IDC_START_TIME, m_startTime);
	DDX_DateTimeCtrl(pDX, IDC_STOP_DATE, m_stopDate);
	DDX_DateTimeCtrl(pDX, IDC_STOP_TIME, m_stopTime);
	DDX_Text(pDX, IDC_EDIT_TERMINAL_NO, m_dwTerminalNo);
	DDX_Check(pDX, IDC_CHK_PROGRAM, m_bEnableProgram);
	DDX_Text(pDX, IDC_EDIT_SCHEDULE, m_dwScheduleNo);
	//}}AFX_DATA_MAP
    DDX_Check(pDX, IDC_CHECK_TRIGGER_PEOPLE_COUNTING_DATA, m_bTriggerPeopleCountingData);
    DDX_Check(pDX, IDC_CHECK_MULTI_CHANNEL, m_bMultiChannelSearch);
    DDX_Control(pDX, IDC_LIST_CHAN, m_listChan);
}


BEGIN_MESSAGE_MAP(CDlgPdcResult, CDialog)
	//{{AFX_MSG_MAP(CDlgPdcResult)
	ON_BN_CLICKED(IDC_BTN_GET_PDC_RESULT, OnBtnGetPdcResult)
	ON_BN_CLICKED(IDC_BTN_CLEAN_PDC_RESULT, OnBtnCleanPdcResult)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPdcResult message handlers

BOOL CDlgPdcResult::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (g_struDeviceInfo[m_iDevIndex].iDeviceType == DS_D60S) //��Ϣ����������
	{
		GetDlgItem(IDC_EDIT_TERMINAL_NO)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_TERMINAL_NO)->EnableWindow(FALSE);
	}

	char szLanTemp[128] = {0};
	m_lstPdcResult.SetExtendedStyle(m_lstPdcResult.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_SUBITEMIMAGES);
	g_StringLanType(szLanTemp, "��ʼʱ��", "Start Time");
    m_lstPdcResult.InsertColumn(0, szLanTemp, LVCFMT_LEFT, 80, -1);
	g_StringLanType(szLanTemp, "����ʱ��", "Stop Time");
	m_lstPdcResult.InsertColumn(1, szLanTemp, LVCFMT_LEFT, 80, -1);
	g_StringLanType(szLanTemp, "��������", "Enter Num");
    m_lstPdcResult.InsertColumn(2, szLanTemp, LVCFMT_LEFT, 80, -1);
	g_StringLanType(szLanTemp, "�뿪����", "Leave Num");
    m_lstPdcResult.InsertColumn(3, szLanTemp, LVCFMT_LEFT, 80, -1);
	g_StringLanType(szLanTemp, "��Ŀ���", "Program No.");
    m_lstPdcResult.InsertColumn(4, szLanTemp, LVCFMT_LEFT, 80, -1);
	g_StringLanType(szLanTemp, "��Ŀ����", "Program Name");
    m_lstPdcResult.InsertColumn(5, szLanTemp, LVCFMT_LEFT, 120, -1);
    g_StringLanType(szLanTemp, "��������", "Passing Num");
    m_lstPdcResult.InsertColumn(6, szLanTemp, LVCFMT_LEFT, 80, -1);

// 	m_pStruPdcResult = new NET_DVR_PDC_RESULT[MAX_ALARMHOST_ALARMOUT_NUM];
// 	memset(m_pStruPdcResult, 0, sizeof(NET_DVR_PDC_RESULT)*MAX_ALARMHOST_ALARMOUT_NUM);

    memset(&m_struPdcResult, 0, sizeof(NET_DVR_PDC_RESULT));
	memset(&m_struPdcResultCond, 0, sizeof(NET_DVR_PDC_QUERY_COND));

	g_AddChanInfoToComBox(m_cmbChannel, m_iDevIndex);
	m_comReportType.SetCurSel(0);

    m_listChan.SetExtendedStyle(m_listChan.GetExtendedStyle() | LVS_EX_CHECKBOXES);
    UpdateChanStatus();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgPdcResult::AddPdcResultLst(LPNET_DVR_PDC_RESULT lpInter)
{
	int iItemCount = m_lstPdcResult.GetItemCount();
	m_lstPdcResult.InsertItem(iItemCount, "");
	
	char szStr[128] = {0};
	
	//��ʼʱ��
	memset(szStr, 0, sizeof(szStr));
    if (lpInter->byISO8601)
    {
        sprintf(szStr, "%d-%d-%d %d:%d:%dT%2.2d%2.2d", lpInter->struStartTime.wYear, lpInter->struStartTime.byMonth, \
            lpInter->struStartTime.byDay, lpInter->struStartTime.byHour, lpInter->struStartTime.byMinute, \
            lpInter->struStartTime.bySecond, lpInter->cStartTimeDifferenceH, lpInter->cStartTimeDifferenceM);
    }
    else
    {
        sprintf(szStr, "%d-%d-%d %d:%d:%d", lpInter->struStartTime.wYear, lpInter->struStartTime.byMonth, \
            lpInter->struStartTime.byDay, lpInter->struStartTime.byHour, lpInter->struStartTime.byMinute, \
            lpInter->struStartTime.bySecond);
    }
	m_lstPdcResult.SetItemText(iItemCount, 0, szStr);
	
	//����ʱ��
	memset(szStr, 0, sizeof(szStr));
    if (lpInter->byISO8601)
    {
        sprintf(szStr, "%d-%d-%d %d:%d:%dT%2.2d%2.2d", lpInter->struEndTime.wYear, lpInter->struEndTime.byMonth, \
            lpInter->struEndTime.byDay, lpInter->struEndTime.byHour, lpInter->struEndTime.byMinute, \
            lpInter->struEndTime.bySecond, lpInter->cStopTimeDifferenceH, lpInter->cStopTimeDifferenceM);
    }
    else
    {
        sprintf(szStr, "%d-%d-%d %d:%d:%d", lpInter->struEndTime.wYear, lpInter->struEndTime.byMonth, \
            lpInter->struEndTime.byDay, lpInter->struEndTime.byHour, lpInter->struEndTime.byMinute, \
            lpInter->struEndTime.bySecond);
    }
	m_lstPdcResult.SetItemText(iItemCount, 1, szStr);
	
	//��������
	memset(szStr, 0, sizeof(szStr));
	sprintf(szStr, "%d", lpInter->dwEnterNum);
	m_lstPdcResult.SetItemText(iItemCount, 2, szStr);
	
	//�뿪����
	memset(szStr, 0, sizeof(szStr));
	sprintf(szStr, "%d", lpInter->dwLeaveNum);
	m_lstPdcResult.SetItemText(iItemCount, 3, szStr);

	//��Ŀ��Ϣ
	memset(szStr, 0, sizeof(szStr));
	sprintf(szStr, "%d", lpInter->struProgramInfo.dwProgramNo);
	m_lstPdcResult.SetItemText(iItemCount, 4, szStr);

	m_lstPdcResult.SetItemText(iItemCount, 5, (char*)lpInter->struProgramInfo.sProgramName);

    //��������
    memset(szStr, 0, sizeof(szStr));
    sprintf(szStr, "%d", lpInter->dwPeoplePassing);
    m_lstPdcResult.SetItemText(iItemCount, 6, szStr);
}

DWORD WINAPI CDlgPdcResult::GetPdcResultThread(LPVOID lpPdcResult)
{
	CDlgPdcResult* pThis = reinterpret_cast<CDlgPdcResult*>(lpPdcResult);
	int iRet = 0;
	char szLan[128] = {0};
	char szInfoBuf[128] = {0};
	while (pThis->m_bGetNext)
	{
        iRet = NET_DVR_GetNextRemoteConfig(pThis->m_lHandle, &pThis->m_struPdcResult, sizeof(NET_DVR_PDC_RESULT));
		if (iRet == NET_SDK_GET_NEXT_STATUS_SUCCESS)
		{
            pThis->AddPdcResultLst(&pThis->m_struPdcResult);
			pThis->m_iStruCount++;
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
				g_StringLanType(szLan, "���������ݲ�ѯ����!", "Get Pdc Result List Finish");
				sprintf(szInfoBuf, "%s[Info Count:%d]", szLan, pThis->m_lstPdcResult.GetItemCount());
				AfxMessageBox(szInfoBuf);
				break;
			}
			else if(iRet == NET_SDK_GET_NEXT_STATUS_FAILED)
			{
				g_StringLanType(szLan, "�����ӿ��������ݲ�ѯʧ��", "Get Pdc Result List failed");
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

void CDlgPdcResult::OnBtnGetPdcResult() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	char szLan[128] = {0};
    DWORD dwBufferLen = 0;
    char* pSearchChannel = NULL;
	m_lChannel = LONG(m_dwTerminalNo<<16 | m_cmbChannel.GetItemData(m_cmbChannel.GetCurSel()));
	m_struPdcResultCond.dwSize = sizeof(m_struPdcResultCond);
	m_struPdcResultCond.dwChannel = m_lChannel;

	m_struPdcResultCond.struStartTime.wYear = m_startDate.GetYear();
	m_struPdcResultCond.struStartTime.byMonth = m_startDate.GetMonth();
	m_struPdcResultCond.struStartTime.byDay = m_startDate.GetDay();
	m_struPdcResultCond.struStartTime.byHour = m_startTime.GetHour();
	m_struPdcResultCond.struStartTime.byMinute = m_startTime.GetMinute();
	m_struPdcResultCond.struStartTime.bySecond = m_startTime.GetSecond();
	
	m_struPdcResultCond.struEndTime.wYear = m_stopDate.GetYear();
	m_struPdcResultCond.struEndTime.byMonth = m_stopDate.GetMonth();
	m_struPdcResultCond.struEndTime.byDay = m_stopDate.GetDay();
	m_struPdcResultCond.struEndTime.byHour = m_stopTime.GetHour();
	m_struPdcResultCond.struEndTime.byMinute = m_stopTime.GetMinute();
	m_struPdcResultCond.struEndTime.bySecond = m_stopTime.GetSecond();
	
	m_struPdcResultCond.byReportType = m_comReportType.GetCurSel()+1;

	m_struPdcResultCond.byEnableProgramStatistics = m_bEnableProgram;
	m_struPdcResultCond.dwPlayScheduleNo = m_dwScheduleNo;
    m_struPdcResultCond.byTriggerPeopleCountingData = m_bTriggerPeopleCountingData;

    m_struPdcResultCond.byISO8601 = g_bISO8601;
    m_struPdcResultCond.cStartTimeDifferenceH = g_iHourDiffWithUTC;
    m_struPdcResultCond.cStartTimeDifferenceM = g_iMinDiffWithUTC;
    m_struPdcResultCond.cStopTimeDifferenceH = g_iHourDiffWithUTC;
    m_struPdcResultCond.cStopTimeDifferenceM = g_iMinDiffWithUTC;
    m_struPdcResultCond.byMultiChannelSearch = m_bMultiChannelSearch;

    if (m_struPdcResultCond.byMultiChannelSearch && m_listChan.GetItemCount() > 0)
    {
        dwBufferLen = m_listChan.GetItemCount() * sizeof(DWORD) + 1;
        pSearchChannel = new char[dwBufferLen];
        memset(pSearchChannel, 0, dwBufferLen);
        m_struPdcResultCond.dwSearchChannelNum = 0;
        for (int i = 0; i < m_listChan.GetItemCount(); i++)
        {
            if (m_listChan.GetCheck(i))
            {
                int iChannel = -1;
                iChannel = m_listChan.GetItemData(i);
                ((DWORD*)pSearchChannel)[m_struPdcResultCond.dwSearchChannelNum] = iChannel;
                m_struPdcResultCond.dwSearchChannelNum++;
            }
        }
        m_struPdcResultCond.pSearchChannel = pSearchChannel;
    }
	
	if (m_lHandle >= 0)
	{
		if (!NET_DVR_StopRemoteConfig(m_lHandle))
		{
			m_bGetNext = FALSE;
            if (pSearchChannel != NULL)
            {
                delete[] pSearchChannel;
                pSearchChannel = NULL;
            }
			g_StringLanType(szLan, "���������ݲ�ѯֹͣʧ��", "PDC Result List Stop Failed");
			AfxMessageBox(szLan);
			return;
		}
	} 
	
	m_lHandle = NET_DVR_StartRemoteConfig(m_lServerID, NET_DVR_GET_PDC_RESULT, &m_struPdcResultCond, sizeof(m_struPdcResultCond), NULL, NULL);
	if (m_lHandle >= 0)
	{
		m_bGetNext = TRUE;
		DWORD dwThreadId;
		m_lstPdcResult.DeleteAllItems();
		m_hGetInfoThread = CreateThread(NULL, 0, LPTHREAD_START_ROUTINE(GetPdcResultThread), this, 0, &dwThreadId);
        g_StringLanType(szLan, "���������ݲ�ѯ�ɹ�", "PDC Result List succ");
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, szLan);
	}
	else
	{
		m_bGetNext = FALSE;
        if (pSearchChannel != NULL)
        {
            delete[] pSearchChannel;
            pSearchChannel = NULL;
        }
		g_StringLanType(szLan, "���������ݲ�ѯʧ��", "PDC Result List failed");
		AfxMessageBox(szLan);
		g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_GET_PDC_RESULT");
		return;
	}
    if (pSearchChannel != NULL)
    {
        delete[] pSearchChannel;
        pSearchChannel = NULL;
    }
	
	UpdateData(FALSE);
}

void CDlgPdcResult::OnBtnCleanPdcResult() 
{
	// TODO: Add your control notification handler code here
	m_lstPdcResult.DeleteAllItems();
}


void CDlgPdcResult::UpdateChanStatus()
{
    int iIndex = 0;
    int i = 0;
    CString csTemp;
    m_listChan.DeleteAllItems();
    //get the whole state of all channels

    for (i = 0; i < g_struDeviceInfo[m_iDevIndex].iDeviceChanNum; i++)
    {
        if (i < g_struDeviceInfo[m_iDevIndex].iAnalogChanNum)
        {
            csTemp.Format(ANALOG_C_FORMAT, g_struDeviceInfo[m_iDevIndex].iStartChan + i);
            m_listChan.InsertItem(iIndex, csTemp);
            m_listChan.SetItemData(iIndex, i + g_struDeviceInfo[m_iDevIndex].iStartChan);
            iIndex++;
        }
        else
        {
            csTemp.Format(DIGITAL_C_FORMAT, i + 1 - g_struDeviceInfo[m_iDevIndex].iAnalogChanNum/*g_struDeviceInfo[m_iDeviceIndex].iStartChan-g_struDeviceInfo[m_iDeviceIndex].pStruIPParaCfgV40[0].dwStartDChan*/);
            m_listChan.InsertItem(iIndex, csTemp);
            m_listChan.SetItemData(iIndex, i - g_struDeviceInfo[m_iDevIndex].iAnalogChanNum + g_struDeviceInfo[m_iDevIndex].pStruIPParaCfgV40[0].dwStartDChan);
            iIndex++;
        }
    }
}

