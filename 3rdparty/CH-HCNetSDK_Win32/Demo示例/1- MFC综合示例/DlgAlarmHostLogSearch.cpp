// DlgAlarmHostLogSearch.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgAlarmHostLogSearch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAlarmHostLogSearch dialog
CDlgAlarmHostLogSearch * g_pLogSearch = NULL;

UINT GetAlarmHostLogThread(LPVOID pParam)
{
    CString csTmp;
    char szLan[128] = {0};
    LONG lRet = -1;
    NET_DVR_ALARMHOST_LOG_RET struAlarmHostLog = {0};
    
    while (1) 
    {
        lRet = NET_DVR_FindNextAlarmHostLog(g_pLogSearch->m_lSearchHandle, &struAlarmHostLog);
        if (lRet == NET_DVR_FILE_SUCCESS)
        {
            int nIndex = 0;
            csTmp.Format("%d", g_pLogSearch->m_lLogNum+1);
            g_pLogSearch->m_listAlarmHostLog.InsertItem(g_pLogSearch->m_lLogNum, csTmp,nIndex);
            nIndex++;
            csTmp.Format("%04d-%02d-%02d %02d:%02d:%02d", struAlarmHostLog.struLogTime.dwYear, struAlarmHostLog.struLogTime.dwMonth, struAlarmHostLog.struLogTime.dwDay, \
                struAlarmHostLog.struLogTime.dwHour,struAlarmHostLog.struLogTime.dwMinute, struAlarmHostLog.struLogTime.dwSecond);
            g_pLogSearch->m_listAlarmHostLog.SetItemText(g_pLogSearch->m_lLogNum, nIndex, csTmp);	
            nIndex++;
            
            char szTmp[40] = {0};
            memcpy(szTmp, struAlarmHostLog.sUserName, sizeof(struAlarmHostLog.sUserName));
            csTmp.Format("%s", szTmp);
            
            g_pLogSearch->m_listAlarmHostLog.SetItemText(g_pLogSearch->m_lLogNum, nIndex, csTmp);	
            nIndex++;
            
            g_pLogSearch->InfoMajorTypeMap(struAlarmHostLog.wMajorType, csTmp);
            g_pLogSearch->m_listAlarmHostLog.SetItemText(g_pLogSearch->m_lLogNum, nIndex, csTmp);
            nIndex++;

            if (MAJOR_ALARMHOST_ALARM == struAlarmHostLog.wMajorType)
            {
                g_pLogSearch->InfoMinorAlarmMap(struAlarmHostLog.wMinorType, csTmp);
                g_pLogSearch->m_listAlarmHostLog.SetItemText(g_pLogSearch->m_lLogNum, nIndex, csTmp);
                nIndex++;
            }
            else if (MAJOR_ALARMHOST_EXCEPTION == struAlarmHostLog.wMajorType)
            {
                g_pLogSearch->InfoMinorExceptionMap(struAlarmHostLog.wMinorType, csTmp);
                g_pLogSearch->m_listAlarmHostLog.SetItemText(g_pLogSearch->m_lLogNum, nIndex, csTmp);
                nIndex++;
            }
            else if (MAJOR_ALARMHOST_OPERATION == struAlarmHostLog.wMajorType)
            {
                g_pLogSearch->InfoMinorOperationMap(struAlarmHostLog.wMinorType, csTmp);
                g_pLogSearch->m_listAlarmHostLog.SetItemText(g_pLogSearch->m_lLogNum, nIndex, csTmp);
                nIndex++;
            }
            else if (MAJ0R_ALARMHOST_EVENT == struAlarmHostLog.wMajorType)
            {
                g_pLogSearch->InfoMinorEventMap(struAlarmHostLog.wMinorType, csTmp);
                g_pLogSearch->m_listAlarmHostLog.SetItemText(g_pLogSearch->m_lLogNum, nIndex, csTmp);
                nIndex++;
            }

            csTmp.Format("%d", struAlarmHostLog.wParam);
            g_pLogSearch->m_listAlarmHostLog.SetItemText(g_pLogSearch->m_lLogNum, nIndex, csTmp);
            nIndex++;
            
            csTmp.Format("%s",struAlarmHostLog.struIPAddr.sIpV4);
            g_pLogSearch->m_listAlarmHostLog.SetItemText(g_pLogSearch->m_lLogNum, nIndex, csTmp);
            nIndex++;
             
            if (struAlarmHostLog.dwInfoLen > 0)
            {
                g_pLogSearch->m_listAlarmHostLog.SetItemText(g_pLogSearch->m_lLogNum, nIndex, struAlarmHostLog.sInfo);
                nIndex++;
            }
            
            g_pLogSearch->m_lLogNum++;
        }
        else
        {
            if (lRet == NET_DVR_ISFINDING)
            {
                g_pLogSearch->GetDlgItem(IDC_STATIC_SEARCH)->ShowWindow(SW_SHOW);
				g_StringLanType(szLan, "��־������......", "Searching...");
                g_pLogSearch->GetDlgItem(IDC_STATIC_SEARCH)->SetWindowText(szLan);
                Sleep(5);
                continue;
            }
            if ((lRet == NET_DVR_NOMOREFILE) || (lRet == NET_DVR_FILE_NOFIND))
            {
                g_StringLanType(szLan, "������־", "Search Log");
                g_pLogSearch->GetDlgItem(IDC_BTN_SEARCH)->SetWindowText(szLan);
                g_pLogSearch->m_bSearch = FALSE;
                g_pLogSearch->GetDlgItem(IDC_STATIC_SEARCH)->ShowWindow(SW_HIDE);
                g_StringLanType(szLan, "������־����!", "Search log Ending");
                AfxMessageBox(szLan);
                break;
            }
            else
            {
                g_StringLanType(szLan, "������־", "Search Log");
                g_pLogSearch->GetDlgItem(IDC_BTN_SEARCH)->SetWindowText(szLan);
                g_pLogSearch->m_bSearch = FALSE;
                g_StringLanType(szLan, "���ڷ�����æ,���������,������־�쳣��ֹ!",\
                    "Since the server is busy, or network failure, search the log abnormal termination");
                AfxMessageBox(szLan);
                break;
            }
        }
    }
    CloseHandle(g_pLogSearch->m_hFileThread);
    g_pLogSearch->m_hFileThread = NULL;
    NET_DVR_FindAlarmHostLogClose(g_pLogSearch->m_lSearchHandle);
    return 0;
}

CDlgAlarmHostLogSearch::CDlgAlarmHostLogSearch(CWnd* pParent /*=NULL*/)
: CDialog(CDlgAlarmHostLogSearch::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgAlarmHostLogSearch)
    m_ctTimeStart = 0;
    m_ctTimeStop = 0;
    m_oleDataStart = COleDateTime::GetCurrentTime();
	m_oleDataStop = COleDateTime::GetCurrentTime();
    //}}AFX_DATA_INIT
    m_lSearchHandle = -1;
    m_iDevIndex = -1;
    memset(&m_struSearchParam, 0, sizeof(m_struSearchParam));
    m_hFileThread = NULL;
    m_lLogNum = 0;
    m_bSearch = FALSE;
}


void CDlgAlarmHostLogSearch::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgAlarmHostLogSearch)
    DDX_Control(pDX, IDC_COMBO_MODE, m_comboMode);
    DDX_Control(pDX, IDC_LIST_ALARM_HOST_LOG, m_listAlarmHostLog);
    DDX_Control(pDX, IDC_COMBO_MINOR_TYPE, m_comboMinorType);
    DDX_Control(pDX, IDC_COMBO_MAJOR_TYPE, m_comboMajorType);
    DDX_DateTimeCtrl(pDX, IDC_TIME_LOG_START, m_ctTimeStart);
    DDX_DateTimeCtrl(pDX, IDC_TIME_LOG_STOP, m_ctTimeStop);
	DDX_DateTimeCtrl(pDX, IDC_DATE_LOG_START, m_oleDataStart);
	DDX_DateTimeCtrl(pDX, IDC_DATE_LOG_STOP, m_oleDataStop);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAlarmHostLogSearch, CDialog)
//{{AFX_MSG_MAP(CDlgAlarmHostLogSearch)
ON_BN_CLICKED(IDC_BTN_SEARCH, OnBtnSearch)
ON_BN_CLICKED(IDC_BTN_EXIT, OnBtnExit)
ON_CBN_SELCHANGE(IDC_COMBO_MAJOR_TYPE, OnSelchangeComboMajorType)
	ON_BN_CLICKED(IDC_BTN_EXPORT_LOG, OnBtnExportLog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAlarmHostLogSearch message handlers

BOOL CDlgAlarmHostLogSearch::OnInitDialog() 
{
    CDialog::OnInitDialog();
    //todo!!!
    CTime ctCurTime = CTime::GetCurrentTime();
    CTime time(ctCurTime.GetYear(),ctCurTime.GetMonth(),ctCurTime.GetDay(),0,0,0);
    CTime time0(ctCurTime.GetYear(),ctCurTime.GetMonth(),ctCurTime.GetDay(),23,59,59);
    m_oleDataStart = COleDateTime::GetCurrentTime();
    m_ctTimeStart = time;
    m_oleDataStop = COleDateTime::GetCurrentTime();
    m_ctTimeStop = time0;
    
    
    g_pLogSearch = this;
    
    InitWnd();
    
    m_comboMode.SetCurSel(2); // Ĭ�ϰ�ʱ�����
    m_comboMajorType.SetCurSel(0);
    OnSelchangeComboMajorType();
    
    UpdateData(FALSE);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAlarmHostLogSearch::InitWnd()
{
    InitModeCombo();
    InitMajorTypeCombo();
    InitAlarmHostLogList();
}

void CDlgAlarmHostLogSearch::InitAlarmHostLogList()
{
    char szLan[128] = {0};
    m_listAlarmHostLog.SetExtendedStyle(m_listAlarmHostLog.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_SUBITEMIMAGES);
    g_StringLanType(szLan, "���", "No.");
    m_listAlarmHostLog.InsertColumn(0, szLan,LVCFMT_RIGHT, 40, -1);
    g_StringLanType(szLan, "��־ʱ��", "Log Time");
    m_listAlarmHostLog.InsertColumn(1, szLan, LVCFMT_LEFT, 100, -1);
    g_StringLanType(szLan, "�����û�", "Operator user");
    m_listAlarmHostLog.InsertColumn(2, szLan, LVCFMT_LEFT, 100, -1);
    g_StringLanType(szLan, "������", "Major Type");
    m_listAlarmHostLog.InsertColumn(3, szLan, LVCFMT_LEFT,80, -1);
    g_StringLanType(szLan, "������", "Minor Type");
    m_listAlarmHostLog.InsertColumn(4, szLan, LVCFMT_LEFT, 100, -1);
    g_StringLanType(szLan, "����", "Param");
    m_listAlarmHostLog.InsertColumn(5, szLan, LVCFMT_LEFT, 50, -1);
    g_StringLanType(szLan, "Զ������", "Remote Host");
    m_listAlarmHostLog.InsertColumn(6, szLan, LVCFMT_LEFT, 80, -1);
    g_StringLanType(szLan, "����", "Description");
    m_listAlarmHostLog.InsertColumn(7, szLan, LVCFMT_LEFT, 251, -1);
    
    
    //     m_listDeviceLog.SetExtendedStyle(m_listDeviceLog.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_SUBITEMIMAGES);
    //     g_StringLanType(szLan, "���", "No.");
    //     m_listDeviceLog.InsertColumn(0, szLan, LVCFMT_RIGHT, 60, -1);
    //     g_StringLanType(szLan, "��־ʱ��", "Log Time");
    //     m_listDeviceLog.InsertColumn(1, szLan, LVCFMT_LEFT, 140, -1);
    //     g_StringLanType(szLan, "������", "Major Type");
    //     m_listDeviceLog.InsertColumn(2, szLan, LVCFMT_LEFT,80, -1);
    //     g_StringLanType(szLan, "������", "Minor Type");
    //     m_listDeviceLog.InsertColumn(3, szLan, LVCFMT_LEFT, 150, -1);
    //     g_StringLanType(szLan, "Զ������", "Remote Host");
    //     m_listDeviceLog.InsertColumn(4, szLan, LVCFMT_LEFT, 100, -1);
    //     g_StringLanType(szLan, "����", "Description");
    //     m_listDeviceLog.InsertColumn(5, szLan, LVCFMT_LEFT, 251, -1);
    
}


void CDlgAlarmHostLogSearch::InitModeCombo()
{
    char szLan[128] = {0};
    m_comboMode.ResetContent();
    g_StringLanType(szLan, "ȫ��", "All");
    m_comboMode.AddString(szLan);
    g_StringLanType(szLan, "������", "By type");
    m_comboMode.AddString(szLan);
    g_StringLanType(szLan, "��ʱ��", "By time");
    m_comboMode.AddString(szLan);
    g_StringLanType(szLan, "��ʱ�������", "By type & time");
    m_comboMode.AddString(szLan);
}

void CDlgAlarmHostLogSearch::InitMajorTypeCombo()
{
    char szLan[128] = {0};
    m_comboMajorType.ResetContent();
    g_StringLanType(szLan, "ȫ��", "All");
    m_comboMajorType.AddString(szLan);
    g_StringLanType(szLan, "����", "Alarm");
    m_comboMajorType.AddString(szLan);
    g_StringLanType(szLan, "�쳣", "Abnormal");
    m_comboMajorType.AddString(szLan);
    g_StringLanType(szLan, "����", "Operation");
    m_comboMajorType.AddString(szLan);
    g_StringLanType(szLan, "�¼�", "Event");
    m_comboMajorType.AddString(szLan);       
}

void CDlgAlarmHostLogSearch::InitMinorTypeCombo()
{
    int iMajorType = m_comboMajorType.GetCurSel();
    char szLan[128] = {0};
    if (0 == iMajorType)
    {
        int nIndex = 0;
        m_comboMinorType.ResetContent();
        m_comboMinorType.InsertString(nIndex, "All");
        m_comboMinorType.SetItemData(nIndex, 0);
        nIndex++;
    }
    else if (1 == iMajorType)
    {
        InitMinorAlarmCombo();
        
    }
    else if (2 == iMajorType)
    {
        InitMinorExcepCombo();
    }
    else if (3 == iMajorType)
    {
        InitMinorOperatorCombo();
    }
    else if (4 == iMajorType)
    {
        InitMinorEventCombo();
    }
}

void CDlgAlarmHostLogSearch::InitMinorAlarmCombo()
{
    int nIndex = 0;
    m_comboMinorType.ResetContent();
    
    m_comboMinorType.InsertString(nIndex, "All");
    m_comboMinorType.SetItemData(nIndex, 0);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_SHORT_CIRCUIT");
    m_comboMinorType.SetItemData(nIndex, MINOR_SHORT_CIRCUIT);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_BROKEN_CIRCUIT");
    m_comboMinorType.SetItemData(nIndex, MINOR_BROKEN_CIRCUIT);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARM_RESET");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARM_RESET);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARM_NORMAL");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARM_NORMAL);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_PASSWORD_ERROR");
    m_comboMinorType.SetItemData(nIndex, MINOR_PASSWORD_ERROR);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ID_CARD_ILLEGALLY");
    m_comboMinorType.SetItemData(nIndex, MINOR_ID_CARD_ILLEGALLY);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_KEYPAD_REMOVE");
    m_comboMinorType.SetItemData(nIndex, MINOR_KEYPAD_REMOVE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_KEYPAD_REMOVE_RESTORE");
    m_comboMinorType.SetItemData(nIndex, MINOR_KEYPAD_REMOVE_RESTORE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_DEV_REMOVE");
    m_comboMinorType.SetItemData(nIndex, MINOR_DEV_REMOVE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_DEV_REMOVE_RESTORE");
    m_comboMinorType.SetItemData(nIndex, MINOR_DEV_REMOVE_RESTORE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_BELOW_ALARM_LIMIT1");
    m_comboMinorType.SetItemData(nIndex, MINOR_BELOW_ALARM_LIMIT1);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_BELOW_ALARM_LIMIT2");
    m_comboMinorType.SetItemData(nIndex, MINOR_BELOW_ALARM_LIMIT2);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_BELOW_ALARM_LIMIT3");
    m_comboMinorType.SetItemData(nIndex, MINOR_BELOW_ALARM_LIMIT3);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_BELOW_ALARM_LIMIT4");
    m_comboMinorType.SetItemData(nIndex, MINOR_BELOW_ALARM_LIMIT4);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ABOVE_ALARM_LIMIT1");
    m_comboMinorType.SetItemData(nIndex, MINOR_ABOVE_ALARM_LIMIT1);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ABOVE_ALARM_LIMIT2");
    m_comboMinorType.SetItemData(nIndex, MINOR_ABOVE_ALARM_LIMIT2);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ABOVE_ALARM_LIMIT3");
    m_comboMinorType.SetItemData(nIndex, MINOR_ABOVE_ALARM_LIMIT3);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ABOVE_ALARM_LIMIT4");
    m_comboMinorType.SetItemData(nIndex, MINOR_ABOVE_ALARM_LIMIT3);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_URGENCYBTN_ON");
    m_comboMinorType.SetItemData(nIndex, MINOR_URGENCYBTN_ON);
    nIndex++;    
	m_comboMinorType.InsertString(nIndex, "MINOR_URGENCYBTN_OFF");
    m_comboMinorType.SetItemData(nIndex, MINOR_URGENCYBTN_OFF);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_VIRTUAL_DEFENCE_BANDIT");
    m_comboMinorType.SetItemData(nIndex, MINOR_VIRTUAL_DEFENCE_BANDIT);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_VIRTUAL_DEFENCE_FIRE");
    m_comboMinorType.SetItemData(nIndex, MINOR_VIRTUAL_DEFENCE_FIRE);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_VIRTUAL_DEFENCE_URGENT");
    m_comboMinorType.SetItemData(nIndex, MINOR_VIRTUAL_DEFENCE_URGENT);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_MOTDET_START");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_MOTDET_START);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_MOTDET_STOP");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_MOTDET_STOP);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_HIDE_ALARM_START");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_HIDE_ALARM_START);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_HIDE_ALARM_STOP");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_HIDE_ALARM_STOP);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_UPS_ALARM");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_UPS_ALARM);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_ELECTRICITY_METER_ALARM");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_ELECTRICITY_METER_ALARM);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_SWITCH_POWER_ALARM");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_SWITCH_POWER_ALARM);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_GAS_DETECT_SYS_ALARM");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_GAS_DETECT_SYS_ALARM);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_TRANSFORMER_TEMPRATURE_ALARM");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_TRANSFORMER_TEMPRATURE_ALARM);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_TEMP_HUMI_ALARM");
	m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_TEMP_HUMI_ALARM);
	nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_UPS_ALARM_RESTORE");
	m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_UPS_ALARM_RESTORE);
	nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_ELECTRICITY_METER_ALARM_RESTORE");
	m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_ELECTRICITY_METER_ALARM_RESTORE);
	nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_SWITCH_POWER_ALARM_RESTORE");
	m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_SWITCH_POWER_ALARM_RESTORE);
	nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_GAS_DETECT_SYS_ALARM_RESTORE");
	m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_GAS_DETECT_SYS_ALARM_RESTORE);
	nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_TRANSFORMER_TEMPRATURE_ALARM_RESTORE");
	m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_TRANSFORMER_TEMPRATURE_ALARM_RESTORE);
	nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_TEMP_HUMI_ALARM_RESTORE");
	m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_TEMP_HUMI_ALARM_RESTORE);
	nIndex++;

	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_WATER_LEVEL_SENSOR_ALARM");
	m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_WATER_LEVEL_SENSOR_ALARM);
	nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_WATER_LEVEL_SENSOR_ALARM_RESTORE");
	m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_WATER_LEVEL_SENSOR_ALARM_RESTORE);
	nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_DUST_NOISE_ALARM");
	m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_DUST_NOISE_ALARM);
	nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_DUST_NOISE_ALARM_RESTORE");
	m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_DUST_NOISE_ALARM_RESTORE);
	nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_ENVIRONMENTAL_LOGGER_ALARM");
	m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_ENVIRONMENTAL_LOGGER_ALARM);
	nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_ENVIRONMENTAL_LOGGER_ALARM_RESTORE");
	m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_ENVIRONMENTAL_LOGGER_ALARM_RESTORE);
	nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_TRIGGER_TAMPER");
	m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_TRIGGER_TAMPER);
	nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_TRIGGER_TAMPER_RESTORE");
	m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_TRIGGER_TAMPER_RESTORE);
	nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_EMERGENCY_CALL_HELP_ALARM");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_EMERGENCY_CALL_HELP_ALARM);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_EMERGENCY_CALL_HELP_ALARM_RESTORE");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_EMERGENCY_CALL_HELP_ALARM_RESTORE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_CONSULTING_ALARM");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_CONSULTING_ALARM);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_CONSULTING_ALARM_RESTORE");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_CONSULTING_ALARM_RESTORE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ZONE_MODULE_REMOVE");
    m_comboMinorType.SetItemData(nIndex, MINOR_ZONE_MODULE_REMOVE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ZONE_MODULE_RESET");
    m_comboMinorType.SetItemData(nIndex, MINOR_ZONE_MODULE_RESET);
    nIndex++;

    m_comboMinorType.InsertString(nIndex, "MINOR_ALARM_WIND_SPEED_ALARM");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARM_WIND_SPEED_ALARM);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARM_WIND_SPEED_ALARM_RESTORE");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARM_WIND_SPEED_ALARM_RESTORE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARM_GENERATE_OUTPUT_ALARM");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARM_GENERATE_OUTPUT_ALARM);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARM_GENERATE_OUTPUT_RESTORE");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARM_GENERATE_OUTPUT_RESTORE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARM_SOAK_ALARM");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARM_SOAK_ALARM);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARM_SOAK_ALARM_RESTORE");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARM_SOAK_ALARM_RESTORE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARM_SOLAR_POWER_ALARM");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARM_SOLAR_POWER_ALARM);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARM_SOLAR_POWER_ALARM_RESTORE");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARM_SOLAR_POWER_ALARM_RESTORE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARM_SF6_ALARM");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARM_SF6_ALARM);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARM_SF6_ALARM_RESTORE");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARM_SF6_ALARM_RESTORE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARM_WEIGHT_ALARM");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARM_WEIGHT_ALARM);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARM_WEIGHT_ALARM_RESTORE");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARM_WEIGHT_ALARM_RESTORE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARM_WEATHER_ALARM");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARM_WEATHER_ALARM);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARM_WEATHER_ALARM_RESTORE");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARM_WEATHER_ALARM_RESTORE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARM_FUEL_GAS_ALARM");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARM_FUEL_GAS_ALARM);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARM_FUEL_GAS_ALARM_RESTORE");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARM_FUEL_GAS_ALARM_RESTORE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARM_FIRE_ALARM");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARM_FIRE_ALARM);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARM_FIRE_ALARM_RESTORE");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARM_FIRE_ALARM_RESTORE);
    nIndex++;

    m_comboMinorType.InsertString(nIndex, "MINOR_WIRELESS_OUTPUT_MODULE_REMOVE");
    m_comboMinorType.SetItemData(nIndex, MINOR_WIRELESS_OUTPUT_MODULE_REMOVE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_WIRELESS_OUTPUT_MODULE_RESET");
    m_comboMinorType.SetItemData(nIndex, MINOR_WIRELESS_OUTPUT_MODULE_RESET);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_WIRELESS_REPEATER_MODULE_REMOVE");
    m_comboMinorType.SetItemData(nIndex, MINOR_WIRELESS_REPEATER_MODULE_REMOVE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_WIRELESS_REPEATER_MODULE_RESET");
    m_comboMinorType.SetItemData(nIndex, MINOR_WIRELESS_REPEATER_MODULE_RESET);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_WIRELESS_SIREN_MODULE_REMOVE");
    m_comboMinorType.SetItemData(nIndex, MINOR_WIRELESS_SIREN_MODULE_REMOVE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_WIRELESS_SIREN_MODULE_RESET");
    m_comboMinorType.SetItemData(nIndex, MINOR_WIRELESS_SIREN_MODULE_RESET);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_SOUND_INTENSITY_RISE");
    m_comboMinorType.SetItemData(nIndex, MINOR_SOUND_INTENSITY_RISE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_SOUND_INTENSITY_RISE_RESET");
    m_comboMinorType.SetItemData(nIndex, MINOR_SOUND_INTENSITY_RISE_RESET);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_SOUND_INTENSITY_DROP");
    m_comboMinorType.SetItemData(nIndex, MINOR_SOUND_INTENSITY_DROP);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_SOUND_INTENSITY_DROP_RESET");
    m_comboMinorType.SetItemData(nIndex, MINOR_SOUND_INTENSITY_DROP_RESET);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_AUDIO_INPUT_EXCEPTION");
    m_comboMinorType.SetItemData(nIndex, MINOR_AUDIO_INPUT_EXCEPTION);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_AUDIO_INPUT_EXCEPTION_RESET");
    m_comboMinorType.SetItemData(nIndex, MINOR_AUDIO_INPUT_EXCEPTION_RESET);
    nIndex++;
}

void CDlgAlarmHostLogSearch::InitMinorOperatorCombo()
{
    int nIndex = 0;
    m_comboMinorType.ResetContent();
    
    m_comboMinorType.InsertString(nIndex, "All");
    m_comboMinorType.SetItemData(nIndex, 0);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_GUARD");
    m_comboMinorType.SetItemData(nIndex, MINOR_GUARD);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_UNGUARD");
    m_comboMinorType.SetItemData(nIndex, MINOR_UNGUARD);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_BYPASS");
    m_comboMinorType.SetItemData(nIndex, MINOR_BYPASS);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_DURESS_ACCESS");
    m_comboMinorType.SetItemData(nIndex, MINOR_DURESS_ACCESS);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_LOCAL_REBOOT");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_LOCAL_REBOOT);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_REMOTE_REBOOT");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_REMOTE_REBOOT);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_LOCAL_UPGRADE");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_LOCAL_UPGRADE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_REMOTE_UPGRADE");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_REMOTE_UPGRADE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_RECOVERY_DEFAULT_PARAM");
    m_comboMinorType.SetItemData(nIndex, MINOR_RECOVERY_DEFAULT_PARAM);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARM_OUTPUT");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARM_OUTPUT);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ACCESS_OPEN");
    m_comboMinorType.SetItemData(nIndex, MINOR_ACCESS_OPEN);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ACCESS_CLOSE");
    m_comboMinorType.SetItemData(nIndex, MINOR_ACCESS_CLOSE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_SIREN_OPEN");
    m_comboMinorType.SetItemData(nIndex, MINOR_SIREN_OPEN);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_SIREN_CLOSE");
    m_comboMinorType.SetItemData(nIndex, MINOR_SIREN_CLOSE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_MOD_ZONE_CONFIG");
    m_comboMinorType.SetItemData(nIndex, MINOR_MOD_ZONE_CONFIG);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_MOD_ALARMOUT_CONIFG");
    m_comboMinorType.SetItemData(nIndex, MINOR_MOD_ALARMOUT_CONIFG);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_MOD_ANALOG_CONFIG");
    m_comboMinorType.SetItemData(nIndex, MINOR_MOD_ANALOG_CONFIG);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_RS485_CONFIG");
    m_comboMinorType.SetItemData(nIndex, MINOR_RS485_CONFIG);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_PHONE_CONFIG");
    m_comboMinorType.SetItemData(nIndex, MINOR_PHONE_CONFIG);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ADD_ADMIN");
    m_comboMinorType.SetItemData(nIndex, MINOR_ADD_ADMIN);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_MOD_NETUSER_PARAM");
    m_comboMinorType.SetItemData(nIndex, MINOR_MOD_NETUSER_PARAM);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_DEL_ADMIN");
    m_comboMinorType.SetItemData(nIndex, MINOR_DEL_ADMIN);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ADD_NETUSER");
    m_comboMinorType.SetItemData(nIndex, MINOR_ADD_NETUSER);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_MOD_NETUSER_PARAM");
    m_comboMinorType.SetItemData(nIndex, MINOR_MOD_NETUSER_PARAM);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_DEL_NETUSER");
    m_comboMinorType.SetItemData(nIndex, MINOR_DEL_NETUSER);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ADD_OPERATORUSER");
    m_comboMinorType.SetItemData(nIndex, MINOR_ADD_OPERATORUSER);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_MOD_OPERATORUSER_PW");
    m_comboMinorType.SetItemData(nIndex, MINOR_MOD_OPERATORUSER_PW);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_DEL_OPERATORUSER");
    m_comboMinorType.SetItemData(nIndex, MINOR_DEL_OPERATORUSER);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ADD_KEYPADUSER");
    m_comboMinorType.SetItemData(nIndex, MINOR_ADD_KEYPADUSER);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_DEL_KEYPADUSER");
    m_comboMinorType.SetItemData(nIndex, MINOR_DEL_KEYPADUSER);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_REMOTEUSER_LOGIN");
    m_comboMinorType.SetItemData(nIndex, MINOR_REMOTEUSER_LOGIN);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_REMOTEUSER_LOGOUT");
    m_comboMinorType.SetItemData(nIndex, MINOR_REMOTEUSER_LOGOUT);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_REMOTE_GUARD");
    m_comboMinorType.SetItemData(nIndex, MINOR_REMOTE_GUARD);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_REMOTE_UNGUARD");
    m_comboMinorType.SetItemData(nIndex, MINOR_REMOTE_UNGUARD);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_MOD_HOST_CONFIG");
    m_comboMinorType.SetItemData(nIndex, MINOR_MOD_HOST_CONFIG);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_RESTORE_BYPASS");
    m_comboMinorType.SetItemData(nIndex, MINOR_RESTORE_BYPASS);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMOUT_OPEN");// �����������
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMOUT_OPEN);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMOUT_CLOSE");// �����������
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMOUT_CLOSE);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_MOD_SUBSYSTEM_PARAM");// �޸���ϵͳ��������
    m_comboMinorType.SetItemData(nIndex, MINOR_MOD_SUBSYSTEM_PARAM);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_GROUP_BYPASS");// ����·
    m_comboMinorType.SetItemData(nIndex, MINOR_GROUP_BYPASS);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_RESTORE_GROUP_BYPASS");// ����·�ָ�
    m_comboMinorType.SetItemData(nIndex, MINOR_RESTORE_GROUP_BYPASS);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_MOD_GRPS_PARAM");// �޸�GPRS����
    m_comboMinorType.SetItemData(nIndex, MINOR_MOD_GRPS_PARAM);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_MOD_NET_REPORT_PARAM");// �޸������ϱ���������
    m_comboMinorType.SetItemData(nIndex, MINOR_MOD_NET_REPORT_PARAM);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_MOD_REPORT_MOD");// �޸��ϴ���ʽ����
    m_comboMinorType.SetItemData(nIndex, MINOR_MOD_REPORT_MOD);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_MOD_GATEWAY_PARAM");// �޸��Ž���������
    m_comboMinorType.SetItemData(nIndex, MINOR_MOD_GATEWAY_PARAM);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_REMOTE_START_REC");// Զ�̿�ʼ¼��
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_REMOTE_START_REC);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_REMOTE_STOP_REC");// Զ��ֹͣ¼��
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_REMOTE_STOP_REC);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_START_TRANS_CHAN");// ��ʼ͸������
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_START_TRANS_CHAN);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_STOP_TRANS_CHAN");// ֹͣ͸������
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_STOP_TRANS_CHAN);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_START_VT");// ��ʼ�����Խ�
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_START_VT);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_STOP_VTM");// ֹͣ�����Խ�
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_STOP_VTM);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_REMOTE_PLAYBYFILE");// Զ�̰��ļ��ط�
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_REMOTE_PLAYBYFILE);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_REMOTE_PLAYBYTIME");// Զ�̰�ʱ��ط�
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_REMOTE_PLAYBYTIME);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_REMOTE_PTZCTRL");// Զ����̨����
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_REMOTE_PTZCTRL);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_REMOTE_FORMAT_HDD");// Զ�̸�ʽ��Ӳ��
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_REMOTE_FORMAT_HDD);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_REMOTE_LOCKFILE");// Զ�������ļ�
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_REMOTE_LOCKFILE);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_REMOTE_UNLOCKFILE");// Զ�̽����ļ�
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_REMOTE_UNLOCKFILE);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_REMOTE_CFGFILE_OUTPUT");// Զ�̵��������ļ�
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_REMOTE_CFGFILE_OUTPUT);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_REMOTE_CFGFILE_INTPUT");// Զ�̵��������ļ�
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_REMOTE_CFGFILE_INTPUT);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_REMOTE_RECFILE_OUTPUT");// Զ�̵���¼���ļ�
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_REMOTE_RECFILE_OUTPUT);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_STAY_ARM");// ���ز���
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_STAY_ARM);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_QUICK_ARM");// ��ʱ����
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_QUICK_ARM);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_AUTOMATIC_ARM");// �Զ�����
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_AUTOMATIC_ARM);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_AUTOMATIC_DISARM");// �Զ�����
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_AUTOMATIC_DISARM);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_KEYSWITCH_ARM");// Կ�׷�������
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_KEYSWITCH_ARM);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_KEYSWITCH_DISARM");// Կ�׷�������
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_KEYSWITCH_DISARM);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_CLEAR_ALARM");// ����
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_CLEAR_ALARM);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_MOD_FAULT_CFG");// �޸�ϵͳ��������
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_MOD_FAULT_CFG);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_MOD_EVENT_TRIGGER_ALARMOUT_CFG");// �޸��¼����������������
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_MOD_EVENT_TRIGGER_ALARMOUT_CFG);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_SEARCH_EXTERNAL_MODULE");// �������ģ��
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_SEARCH_EXTERNAL_MODULE);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_REGISTER_EXTERNAL_MODULE");// ����ע�����ģ��
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_REGISTER_EXTERNAL_MODULE);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_CLOSE_KEYBOARD_ALARM");// �رռ��̹�����ʾ��
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_CLOSE_KEYBOARD_ALARM);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_MOD_3G_PARAM");// �޸�3G����
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_MOD_3G_PARAM);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_MOD_PRINT_PARAM");// �޸�3G����
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_MOD_PRINT_PARAM);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_SD_CARD_FORMAT");// SD�� ��ʽ��
    m_comboMinorType.SetItemData(nIndex, MINOR_SD_CARD_FORMAT);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_SUBSYSTEM_UPGRADE");// �Ӱ�̼�����
    m_comboMinorType.SetItemData(nIndex, MINOR_SUBSYSTEM_UPGRADE);
	nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_PLAN_ARM_CFG");//�ƻ���������������
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_PLAN_ARM_CFG);
	nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_PHONE_ARM");//�ֻ�����
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_PHONE_ARM);
	nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_PHONE_STAY_ARM");//�ֻ����ز���
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_PHONE_STAY_ARM);
	nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_PHONE_QUICK_ARM");//�ֻ���ʱ����
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_PHONE_QUICK_ARM);
	nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_PHONE_DISARM");//�ֻ�����
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_PHONE_DISARM);
	nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_PHONE_CLEAR_ALARM");//�ֻ�����
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_PHONE_CLEAR_ALARM);
	nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_WHITELIST_CFG");//����������
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_WHITELIST_CFG);
	nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_TIME_TRIGGER_CFG");//��ʱ���ش���������
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_TIME_TRIGGER_CFG);
	nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_CAPTRUE_CFG");//ץͼ��������
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_CAPTRUE_CFG);
	nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_TAMPER_CFG");//���������������
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_TAMPER_CFG);
	nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_REMOTE_KEYPAD_UPGRADE");//Զ����������
	m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_REMOTE_KEYPAD_UPGRADE);
	nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_ONETOUCH_AWAY_ARMING");//һ���������
	m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_ONETOUCH_AWAY_ARMING);
	nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_ONETOUCH_STAY_ARMING");//һ�����ز���
	m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_ONETOUCH_STAY_ARMING);
	nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_SINGLE_PARTITION_ARMING_OR_DISARMING");//������������
	m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_SINGLE_PARTITION_ARMING_OR_DISARMING);
	nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_CARD_CONFIGURATION");//����������
	m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_CARD_CONFIGURATION);
	nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_CARD_ARMING_OR_DISARMING");//ˢ��������
	m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_CARD_ARMING_OR_DISARMING);
	nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_EXPENDING_NETCENTER_CONFIGURATION");//��չ������������
	m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_EXPENDING_NETCENTER_CONFIGURATION);
	nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_NETCARD_CONFIGURATION");//��������
	m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_NETCARD_CONFIGURATION);
	nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_DDNS_CONFIGURATION");//DDNS����
	m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_DDNS_CONFIGURATION);
	nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_RS485BUS_CONFIGURATION");//485���߲�������
	m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_RS485BUS_CONFIGURATION);
	nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_RS485BUS_RE_REGISTRATION");//485��������ע��
	m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_RS485BUS_RE_REGISTRATION);
	nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_LOCAL_SET_DEVICE_ACTIVE");//���ؼ����豸
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_LOCAL_SET_DEVICE_ACTIVE);
	nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_REMOTE_SET_DEVICE_ACTIVE");//Զ�̼����豸
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_REMOTE_SET_DEVICE_ACTIVE);
	nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_LOCAL_PARA_FACTORY_DEFAULT");//���ػظ���������
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_LOCAL_PARA_FACTORY_DEFAULT);
	nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_REMOTE_PARA_FACTORY_DEFAULT");//Զ�ָ̻���������
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_REMOTE_PARA_FACTORY_DEFAULT);
	nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_REMOTE_OPEN_ELECTRIC_LOCK");//Զ�̴򿪵���
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_REMOTE_OPEN_ELECTRIC_LOCK);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_REMOTE_CLOSE_ELECTRIC_LOCK");//Զ�̹رյ���
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_REMOTE_CLOSE_ELECTRIC_LOCK);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_LOCAL_OPEN_ELECTRIC_LOCK");//���ش򿪵���
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_LOCAL_OPEN_ELECTRIC_LOCK);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_LOCAL_CLOSE_ELECTRIC_LOCK");//���عرյ���
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_LOCAL_CLOSE_ELECTRIC_LOCK);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_OPEN_ALARM_LAMP");//�򿪾���(Զ��)
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_OPEN_ALARM_LAMP);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_CLOSE_ALARM_LAMP");//�رվ���(Զ��)
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_CLOSE_ALARM_LAMP);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_TEMPORARY_PASSWORD");//��ʱ���������¼
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_TEMPORARY_PASSWORD);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_ONEKEY_AWAY_ARM");//һ���������
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_ONEKEY_AWAY_ARM);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_ONEKEY_STAY_ARM");//һ�����ز���
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_ONEKEY_STAY_ARM);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_SINGLE_ZONE_ARM");//����������
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_SINGLE_ZONE_ARM);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_SINGLE_ZONE_DISARM");//����������
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_SINGLE_ZONE_DISARM);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_HIDDNS_CONFIG");//HIDDNS����
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_HIDDNS_CONFIG);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_REMOTE_KEYBOARD_UPDATA");//Զ�̼���������־
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_REMOTE_KEYBOARD_UPDATA);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_ZONE_ADD_DETECTOR");//�������̽����
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_ZONE_ADD_DETECTOR);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_ZONE_DELETE_DETECTOR");//����ɾ��̽����
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_ZONE_DELETE_DETECTOR);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_QUERY_DETECTOR_SIGNAL");//������ѯ̽�����ź�ǿ��
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_QUERY_DETECTOR_SIGNAL);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_QUERY_DETECTOR_BATTERY");//������ѯ̽��������
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_QUERY_DETECTOR_BATTERY);
    nIndex++;

    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_SET_DETECTOR_GUARD");//̽��������
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_SET_DETECTOR_GUARD);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_SET_DETECTOR_UNGUARD");//̽��������
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_SET_DETECTOR_UNGUARD);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_SET_WIFI_PARAMETER");//����WIFI���ò���
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_SET_WIFI_PARAMETER);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_OPEN_VOICE");//������
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_OPEN_VOICE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_CLOSE_VOICE");//�ر�����
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_CLOSE_VOICE);
    nIndex++;

    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_ENABLE_FUNCTION_KEY");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_ENABLE_FUNCTION_KEY);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_DISABLE_FUNCTION_KEY");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_DISABLE_FUNCTION_KEY);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_READ_CARD");//Ѳ��ˢ��
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_READ_CARD);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_START_BROADCAST");//�������㲥
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_START_BROADCAST);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_STOP_BROADCAST");//�ر������㲥
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_STOP_BROADCAST);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_REMOTE_ZONE_MODULE_UPGRADE");//Զ����������ģ��
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_REMOTE_ZONE_MODULE_UPGRADE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_NETWORK_MODULE_EXTEND");//����ģ���������
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_NETWORK_MODULE_EXTEND);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_ADD_CONTROLLER");//���ң�����û�
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_ADD_CONTROLLER);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_DELETE_CONTORLLER");//ɾ��ң�����û�
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_DELETE_CONTORLLER);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_REMOTE_NETWORKMODULE_UPGRADE");//Զ����������ģ��
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_REMOTE_NETWORKMODULE_UPGRADE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_WIRELESS_OUTPUT_ADD");//ע���������ģ��
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_WIRELESS_OUTPUT_ADD);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_WIRELESS_OUTPUT_DEL");//ɾ���������ģ��
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_WIRELESS_OUTPUT_DEL);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_WIRELESS_REPEATER_ADD");//ע�������м���
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_WIRELESS_REPEATER_ADD);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_WIRELESS_REPEATER_DEL");//ɾ�������м���
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_WIRELESS_REPEATER_DEL);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_PHONELIST_CFG");//�绰������������
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_PHONELIST_CFG);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_RF_SIGNAL_CHECK");//RF�źŲ�ѯ
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_RF_SIGNAL_CHECK);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_USB_UPGRADE");//USB����
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_USB_UPGRADE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_DOOR_TIME_REMINDER_CFG");//�ŴŶ�ʱ���Ѳ�������
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_DOOR_TIME_REMINDER_CFG);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_WIRELESS_SIREN_ADD");//ע�����߾���
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_WIRELESS_SIREN_ADD);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_WIRELESS_SIREN_DEL");//ɾ�����߾���
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_WIRELESS_SIREN_DEL);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_OUT_SCALE_OPEN");//���翪��
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_OUT_SCALE_OPEN);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_OUT_SCALE_CLOSE");//����ر�
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_OUT_SCALE_CLOSE);
    nIndex++;
}

void CDlgAlarmHostLogSearch::InitMinorExcepCombo()
{
    int nIndex = 0;
    m_comboMinorType.ResetContent();
    
	m_comboMinorType.InsertString(nIndex, "All");
    m_comboMinorType.SetItemData(nIndex, 0);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_POWER_ON");
    m_comboMinorType.SetItemData(nIndex, MINOR_POWER_ON);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_POWER_OFF");
    m_comboMinorType.SetItemData(nIndex, MINOR_POWER_OFF);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_WDT_RESET");
    m_comboMinorType.SetItemData(nIndex, MINOR_WDT_RESET);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_LOW_BATTERY_VOLTAGE");
    m_comboMinorType.SetItemData(nIndex, MINOR_LOW_BATTERY_VOLTAGE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_AC_LOSS");
    m_comboMinorType.SetItemData(nIndex, MINOR_AC_LOSS);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_AC_RESTORE");
    m_comboMinorType.SetItemData(nIndex, MINOR_AC_RESTORE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_RTC_EXCEPTION");
    m_comboMinorType.SetItemData(nIndex, MINOR_RTC_EXCEPTION);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_NETWORK_CONNECT_FAILURE");
    m_comboMinorType.SetItemData(nIndex, MINOR_NETWORK_CONNECT_FAILURE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_NETWORK_CONNECT_RESTORE");
    m_comboMinorType.SetItemData(nIndex, MINOR_NETWORK_CONNECT_RESTORE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_TEL_LINE_CONNECT_FAILURE");
    m_comboMinorType.SetItemData(nIndex, MINOR_TEL_LINE_CONNECT_FAILURE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_TEL_LINE_CONNECT_RESTORE");
    m_comboMinorType.SetItemData(nIndex, MINOR_TEL_LINE_CONNECT_RESTORE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_EXPANDER_BUS_LOSS");
    m_comboMinorType.SetItemData(nIndex, MINOR_EXPANDER_BUS_LOSS);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_EXPANDER_BUS_RESTORE");
    m_comboMinorType.SetItemData(nIndex, MINOR_EXPANDER_BUS_RESTORE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_KEYPAD_BUS_LOSS");
    m_comboMinorType.SetItemData(nIndex, MINOR_KEYPAD_BUS_LOSS);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_KEYPAD_BUS_RESTORE");
    m_comboMinorType.SetItemData(nIndex, MINOR_KEYPAD_BUS_RESTORE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_SENSOR_FAILURE");
    m_comboMinorType.SetItemData(nIndex, MINOR_SENSOR_FAILURE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_SENSOR_RESTORE");
    m_comboMinorType.SetItemData(nIndex, MINOR_SENSOR_RESTORE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_RS485_CONNECT_FAILURE");
    m_comboMinorType.SetItemData(nIndex, MINOR_RS485_CONNECT_FAILURE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_RS485_CONNECT_RESTORE");
    m_comboMinorType.SetItemData(nIndex, MINOR_RS485_CONNECT_RESTORE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_BATTERT_VOLTAGE_RESTORE");
    m_comboMinorType.SetItemData(nIndex, MINOR_BATTERT_VOLTAGE_RESTORE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_WIRED_NETWORK_ABNORMAL");
    m_comboMinorType.SetItemData(nIndex, MINOR_WIRED_NETWORK_ABNORMAL);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_WIRED_NETWORK_RESTORE");
    m_comboMinorType.SetItemData(nIndex, MINOR_WIRED_NETWORK_RESTORE);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_GPRS_ABNORMAL");
    m_comboMinorType.SetItemData(nIndex, MINOR_GPRS_ABNORMAL);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_GPRS_RESTORE");
    m_comboMinorType.SetItemData(nIndex, MINOR_GPRS_RESTORE);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_3G_ABNORMAL");
    m_comboMinorType.SetItemData(nIndex, MINOR_3G_ABNORMAL);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_3G_RESTORE");
    m_comboMinorType.SetItemData(nIndex, MINOR_3G_RESTORE);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_SIM_CARD_ABNORMAL");
    m_comboMinorType.SetItemData(nIndex, MINOR_SIM_CARD_ABNORMAL);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_SIM_CARD_RESTORE");
    m_comboMinorType.SetItemData(nIndex, MINOR_SIM_CARD_RESTORE);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_VI_LOST");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_VI_LOST);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_ILLEGAL_ACCESS");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_ILLEGAL_ACCESS);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_HD_FULL");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_HD_FULL);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_HD_ERROR");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_HD_ERROR);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_DCD_LOST");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_DCD_LOST);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_IP_CONFLICT");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_IP_CONFLICT);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_NET_BROKEN");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_NET_BROKEN);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_REC_ERROR");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_REC_ERROR);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_VI_EXCEPTION");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_VI_EXCEPTION);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_FORMAT_HDD_ERROR");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_FORMAT_HDD_ERROR);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_USB_ERROR");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_USB_ERROR);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_USB_RESTORE");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_USB_RESTORE);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_PRINT_ERROR");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_PRINT_ERROR);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_PRINT_RESTORE");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_PRINT_RESTORE);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_SUBSYSTEM_COMMUNICATION_ERROR");
    m_comboMinorType.SetItemData(nIndex, MINOR_SUBSYSTEM_COMMUNICATION_ERROR);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_IPC_NO_LINK");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_IPC_NO_LINK);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_IPC_IP_CONFLICT");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_IPC_IP_CONFLICT);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_VI_MISMATCH");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_VI_MISMATCH);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_GPRS_MODULE_FAULT");
	m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_GPRS_MODULE_FAULT);
	nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_TELEPHONE_MODULE_FAULT");
	m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_TELEPHONE_MODULE_FAULT);
	nIndex++;
}

void CDlgAlarmHostLogSearch::InitMinorEventCombo()
{
    int nIndex = 0;
    m_comboMinorType.ResetContent();
    
    m_comboMinorType.InsertString(nIndex, "All");
    m_comboMinorType.SetItemData(nIndex, 0);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_SCHOOLTIME_IRGI_B");
    m_comboMinorType.SetItemData(nIndex, MINOR_SCHOOLTIME_IRGI_B);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_SCHOOLTIME_SDK");
    m_comboMinorType.SetItemData(nIndex, MINOR_SCHOOLTIME_SDK);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_SCHOOLTIME_SELFTEST");
    m_comboMinorType.SetItemData(nIndex, MINOR_SCHOOLTIME_SELFTEST);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_SUBSYSTEM_ABNORMALINSERT");
    m_comboMinorType.SetItemData(nIndex, MINOR_SUBSYSTEM_ABNORMALINSERT);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_SUBSYSTEM_ABNORMALPULLOUT");
    m_comboMinorType.SetItemData(nIndex, MINOR_SUBSYSTEM_ABNORMALPULLOUT);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_AUTO_ARM");
    m_comboMinorType.SetItemData(nIndex, MINOR_AUTO_ARM);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_AUTO_DISARM");
    m_comboMinorType.SetItemData(nIndex, MINOR_AUTO_DISARM);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_TIME_TIGGER_ON");
    m_comboMinorType.SetItemData(nIndex, MINOR_TIME_TIGGER_ON);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_TIME_TIGGER_OFF");
    m_comboMinorType.SetItemData(nIndex, MINOR_TIME_TIGGER_OFF);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_AUTO_ARM_FAILD");
    m_comboMinorType.SetItemData(nIndex, MINOR_AUTO_ARM_FAILD);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_AUTO_DISARM_FAILD");
    m_comboMinorType.SetItemData(nIndex, MINOR_AUTO_DISARM_FAILD);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_TIME_TIGGER_ON_FAILD");
    m_comboMinorType.SetItemData(nIndex, MINOR_TIME_TIGGER_ON_FAILD);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_TIME_TIGGER_OFF_FAILD");
    m_comboMinorType.SetItemData(nIndex, MINOR_TIME_TIGGER_OFF_FAILD);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_MANDATORY_ALARM");
    m_comboMinorType.SetItemData(nIndex, MINOR_MANDATORY_ALARM);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_KEYPAD_LOCKED");
	m_comboMinorType.SetItemData(nIndex, MINOR_KEYPAD_LOCKED);
	nIndex++;

    m_comboMinorType.InsertString(nIndex, "MINOR_USB_INSERT");
    m_comboMinorType.SetItemData(nIndex, MINOR_USB_INSERT);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_USB_PULLOUT");
    m_comboMinorType.SetItemData(nIndex, MINOR_USB_PULLOUT);
    nIndex++;
}

void CDlgAlarmHostLogSearch::OnBtnSearch() 
{
	char szLan[128] = {0};

	if (!m_bSearch)
	{
		LONG lSelectMode = m_comboMode.GetCurSel();
		UpdateData(TRUE);
		GetSearchParamFromWnd(m_struSearchParam);
		m_lSearchHandle = NET_DVR_FindAlarmHostLog(m_lServerID, lSelectMode, &m_struSearchParam);
		if (m_lSearchHandle < 0)
		{
			g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_FindAlarmHostLog");
			AfxMessageBox("Fail to find log");
			return;
		}
		else
		{
			g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_FindAlarmHostLog");
		}
		
		DWORD dwThreadId;
		m_listAlarmHostLog.DeleteAllItems();
		m_bSearch = TRUE;
		m_lLogNum = 0;
		if (m_hFileThread == NULL)
		{
			m_hFileThread = CreateThread(NULL,0,LPTHREAD_START_ROUTINE(GetAlarmHostLogThread),this,0,&dwThreadId);		
		}
		if (m_hFileThread == NULL)
		{
			g_StringLanType(szLan, "���߳�ʧ��", "Open thread failed");
			AfxMessageBox(szLan);
			return;
		}
		g_StringLanType(szLan, "ֹͣ����", "Stop Search");
		GetDlgItem(IDC_BTN_SEARCH)->SetWindowText(szLan);
		m_bSearch = TRUE;
		GetDlgItem(IDC_STATIC_SEARCH)->ShowWindow(SW_SHOW);
	}
	else
    {
        if (m_hFileThread)
        {
            TerminateThread(m_hFileThread, 0);
        }
        CloseHandle(m_hFileThread);
        m_hFileThread = NULL;
        NET_DVR_FindAlarmHostLogClose(m_lSearchHandle);
        g_StringLanType(szLan, "������־", "Search");
        GetDlgItem(IDC_BTN_SEARCH)->SetWindowText(szLan);
        m_bSearch = FALSE;
        GetDlgItem(IDC_STATIC_SEARCH)->ShowWindow(SW_HIDE);
        //    m_iDirectoryNum = 0;
    }
    
}

void CDlgAlarmHostLogSearch::OnCancel() 
{
	OnBtnExit();
}

void CDlgAlarmHostLogSearch::OnBtnExit() 
{
    if (m_hFileThread)
    {
        TerminateThread(m_hFileThread, 0);
        CloseHandle(m_hFileThread);
        m_hFileThread = NULL;
        NET_DVR_FindAlarmHostLogClose(m_lSearchHandle);
    }
    CDialog::OnCancel();
}


void CDlgAlarmHostLogSearch::GetSearchParamFromWnd(NET_DVR_ALARMHOST_SEARCH_LOG_PARAM &struSearchParam)
{
    struSearchParam.wMajorType = m_comboMajorType.GetCurSel();
	struSearchParam.wMinorType = m_comboMinorType.GetItemData(m_comboMinorType.GetCurSel());
    struSearchParam.struStartTime.dwYear = m_oleDataStart.GetYear();
    struSearchParam.struStartTime.dwMonth = m_oleDataStart.GetMonth();
    struSearchParam.struStartTime.dwDay = m_oleDataStart.GetDay();
    struSearchParam.struStartTime.dwHour = m_ctTimeStart.GetHour();
    struSearchParam.struStartTime.dwMinute = m_ctTimeStart.GetMinute();
    struSearchParam.struStartTime.dwSecond = m_ctTimeStart.GetSecond();
    
    struSearchParam.struEndTime.dwYear = m_oleDataStop.GetYear();
    struSearchParam.struEndTime.dwMonth = m_oleDataStop.GetMonth();
    struSearchParam.struEndTime.dwDay = m_oleDataStop.GetDay();
    struSearchParam.struEndTime.dwHour = m_ctTimeStop.GetHour();
    struSearchParam.struEndTime.dwMinute = m_ctTimeStop.GetMinute();
    struSearchParam.struEndTime.dwSecond = m_ctTimeStop.GetSecond();
}

void CDlgAlarmHostLogSearch::OnSelchangeComboMajorType() 
{
    InitMinorTypeCombo();
    m_comboMinorType.SetCurSel(0);
}

void CDlgAlarmHostLogSearch::InfoMinorAlarmMap(DWORD dwMinorType, CString &szTemp)
{
    char szLan[256] = {0};
    switch (dwMinorType)
    {
    case MINOR_SHORT_CIRCUIT:
        g_StringLanType(szLan, "��·����", "SHORT_CIRCUIT");
        break;
    case MINOR_BROKEN_CIRCUIT:           // ��·����
        g_StringLanType(szLan, "��·����", "BROKEN_CIRCUIT");
        break;
    case MINOR_ALARM_RESET:              // ������λ
        g_StringLanType(szLan, "������λ", "ALARM_RESET");
        break;
    case MINOR_ALARM_NORMAL:				// �����ָ�����
        g_StringLanType(szLan, "�����ָ�����", "ALARM_NORMAL");
        break;
    case MINOR_PASSWORD_ERROR:			// �����������3�������������
        g_StringLanType(szLan, "�����������", "PASSWORD_ERROR");
        break;
    case MINOR_ID_CARD_ILLEGALLY:		// �Ƿ���Ӧ��ID
        g_StringLanType(szLan, "�Ƿ���Ӧ��ID", "ID_CARD_ILLEGALLY");
        break;
    case MINOR_KEYPAD_REMOVE:			// ���̷���
        g_StringLanType(szLan, "���̷���", "KEYPAD_REMOVE");
        break;
    case MINOR_KEYPAD_REMOVE_RESTORE:	// ���̷���λ
        g_StringLanType(szLan, "���̷���λ", "KEYPAD_REMOVE_RESTORE");
        break;
    case MINOR_DEV_REMOVE:				// �豸����
        g_StringLanType(szLan, "�豸����", "DEV_REMOVE");
        break;
    case MINOR_DEV_REMOVE_RESTORE:		// �豸����λ
        g_StringLanType(szLan, "�豸����λ", "DEV_REMOVE_RESTORE");
        break;
    case MINOR_BELOW_ALARM_LIMIT1:		// ģ�������ڱ�����1
        g_StringLanType(szLan, "ģ�������ڱ�����1", "BELOW_ALARM_LIMIT1");
        break;
    case MINOR_BELOW_ALARM_LIMIT2:		// ģ�������ڱ�����2
        g_StringLanType(szLan, "ģ�������ڱ�����2", "BELOW_ALARM_LIMIT2");
        break;
    case MINOR_BELOW_ALARM_LIMIT3:		// ģ�������ڱ�����3
        g_StringLanType(szLan, "ģ�������ڱ�����3", "BELOW_ALARM_LIMIT3");
        break;
    case MINOR_BELOW_ALARM_LIMIT4:		// ģ�������ڱ�����4
        g_StringLanType(szLan, "ģ�������ڱ�����4", "BELOW_ALARM_LIMIT4");
        break;
    case MINOR_ABOVE_ALARM_LIMIT1:		// ģ�������ڱ�����1
        g_StringLanType(szLan, "ģ�������ڱ�����1", "ABOVE_ALARM_LIMIT1");
        break;
    case MINOR_ABOVE_ALARM_LIMIT2:		// ģ�������ڱ�����2
        g_StringLanType(szLan, "ģ�������ڱ�����2", "ABOVE_ALARM_LIMIT2");
        break;
    case MINOR_ABOVE_ALARM_LIMIT3:		// ģ�������ڱ�����3
        g_StringLanType(szLan, "ģ�������ڱ�����3", "ABOVE_ALARM_LIMIT3");
        break;
    case MINOR_ABOVE_ALARM_LIMIT4:		// ģ�������ڱ�����4
        g_StringLanType(szLan, "ģ�������ڱ�����4", "ABOVE_ALARM_LIMIT4");
        break;
	case MINOR_URGENCYBTN_ON:			//������ť����
		g_StringLanType(szLan, "������ť����", "URGENCYBTN_ON");
		break;
	case MINOR_URGENCYBTN_OFF:			//������ť��λ
		g_StringLanType(szLan, "������ť��λ", "URGENCYBTN_OFF");
		break;
	case MINOR_VIRTUAL_DEFENCE_BANDIT:			//������˾�
		g_StringLanType(szLan, "������˾�", "VIRTUAL_DEFENCE_BANDIT");
		break;
	case MINOR_VIRTUAL_DEFENCE_FIRE:			//�������
		g_StringLanType(szLan, "�������", "VIRTUAL_DEFENCE_FIRE");
		break;
	case MINOR_VIRTUAL_DEFENCE_URGENT:			//���������
		g_StringLanType(szLan, "���������", "VIRTUAL_DEFENCE_URGENT");
		break;
	case MINOR_ALARMHOST_MOTDET_START:			//�ƶ���ⱨ����ʼ
		g_StringLanType(szLan, "�ƶ���ⱨ����ʼ", "ALARMHOST_MOTDET_START");
		break;
	case MINOR_ALARMHOST_MOTDET_STOP:			//�ƶ���ⱨ������
		g_StringLanType(szLan, "�ƶ���ⱨ������", "ALARMHOST_MOTDET_STOP");
		break;
	case MINOR_ALARMHOST_HIDE_ALARM_START:			//�ڵ�������ʼ
		g_StringLanType(szLan, "�ڵ�������ʼ", "ALARMHOST_HIDE_ALARM_START");
		break;
	case MINOR_ALARMHOST_HIDE_ALARM_STOP:			//�ڵ���������
		g_StringLanType(szLan, "�ڵ���������", "ALARMHOST_HIDE_ALARM_STOP");
		break;
	case MINOR_ALARMHOST_UPS_ALARM:					//UPS����
		g_StringLanType(szLan, "UPS����", "ALARMHOST_UPS_ALARM");
		break;
	case MINOR_ALARMHOST_ELECTRICITY_METER_ALARM:	//��������
		g_StringLanType(szLan, "��������", "ALARMHOST_ELECTRICITY_METER_ALARM");
		break;
	case MINOR_ALARMHOST_SWITCH_POWER_ALARM:		//���ص�Դ����
		g_StringLanType(szLan, "���ص�Դ����", "ALARMHOST_SWITCH_POWER_ALARM");
		break;
	case MINOR_ALARMHOST_GAS_DETECT_SYS_ALARM:		//������ϵͳ����
		g_StringLanType(szLan, "������ϵͳ����", "ALARMHOST_GAS_DETECT_SYS_ALARM");
		break;
	case MINOR_ALARMHOST_TRANSFORMER_TEMPRATURE_ALARM: //��ѹ�����Ա���
		g_StringLanType(szLan, "��ѹ�����Ա���", "ALARMHOST_TRANSFORMER_TEMPRATURE_ALARM");
		break;
	case MINOR_ALARMHOST_TEMP_HUMI_ALARM: //��ʪ�ȴ���������
		g_StringLanType(szLan, "��ʪ�ȴ���������", "ALARMHOST_TEMP_HUMI_ALARM");
		break;
	case MINOR_ALARMHOST_UPS_ALARM_RESTORE: //UPS�����ָ�
		g_StringLanType(szLan, "UPS�����ָ�", "ALARMHOST_UPS_ALARM_RESTORE");
		break;
	case MINOR_ALARMHOST_ELECTRICITY_METER_ALARM_RESTORE: //���������ָ�
		g_StringLanType(szLan, "���������ָ�", "ALARMHOST_ELECTRICITY_METER_ALARM_RESTORE");
		break;
	case MINOR_ALARMHOST_SWITCH_POWER_ALARM_RESTORE: //�����������ָ�
		g_StringLanType(szLan, "�����������ָ�", "ALARMHOST_SWITCH_POWER_ALARM_RESTORE");
		break;
	case MINOR_ALARMHOST_GAS_DETECT_SYS_ALARM_RESTORE: //�����ⱨ���ָ�
		g_StringLanType(szLan, "�����ⱨ���ָ�", "ALARMHOST_GAS_DETECT_SYS_ALARM_RESTORE");
		break;
	case MINOR_ALARMHOST_TRANSFORMER_TEMPRATURE_ALARM_RESTORE: //��������Ա����ָ�
		g_StringLanType(szLan, "��������Ա����ָ�", "ALARMHOST_TRANSFORMER_TEMPRATURE_ALARM_RESTORE");
		break;
	case MINOR_ALARMHOST_TEMP_HUMI_ALARM_RESTORE: //��ʪ�ȴ����������ָ�
		g_StringLanType(szLan, "��ʪ�ȴ����������ָ�", "ALARMHOST_TEMP_HUMI_ALARM_RESTORE");
		break;

	case MINOR_ALARMHOST_WATER_LEVEL_SENSOR_ALARM: //ˮλ����������
		g_StringLanType(szLan, "ˮλ����������", "ALARMHOST_WATER_LEVEL_SENSOR_ALARM");
		break;
	case MINOR_ALARMHOST_WATER_LEVEL_SENSOR_ALARM_RESTORE: //ˮλ�����������ָ�
		g_StringLanType(szLan, "ˮλ�����������ָ�", "ALARMHOST_WATER_LEVEL_SENSOR_ALARM_RESTORE");
		break;
	case MINOR_ALARMHOST_DUST_NOISE_ALARM: //�ﳾ��������������
		g_StringLanType(szLan, "�ﳾ��������������", "ALARMHOST_DUST_NOISE_ALARM");
		break;
	case MINOR_ALARMHOST_DUST_NOISE_ALARM_RESTORE: //�ﳾ���������������ָ�
		g_StringLanType(szLan, "�ﳾ���������������ָ�", "ALARMHOST_DUST_NOISE_ALARM_RESTORE");
		break;
	case MINOR_ALARMHOST_ENVIRONMENTAL_LOGGER_ALARM: //�����ɼ��Ǳ���
		g_StringLanType(szLan, "�����ɼ��Ǳ���", "ALARMHOST_ENVIRONMENTAL_LOGGER_ALARM");
		break;
	case MINOR_ALARMHOST_ENVIRONMENTAL_LOGGER_ALARM_RESTORE: //�����ɼ��Ǳ����ָ�
		g_StringLanType(szLan, "�����ɼ��Ǳ����ָ�", "ALARMHOST_ENVIRONMENTAL_LOGGER_ALARM_RESTORE");
		break;
	case MINOR_ALARMHOST_TRIGGER_TAMPER: //̽��������
		g_StringLanType(szLan, "̽��������", "ALARMHOST_TRIGGER_TAMPER");
		break;	
	case MINOR_ALARMHOST_TRIGGER_TAMPER_RESTORE: //̽��������ָ�
		g_StringLanType(szLan, "̽��������ָ�", "ALARMHOST_TRIGGER_TAMPER_RESTORE");
		break;
    case MINOR_ALARMHOST_EMERGENCY_CALL_HELP_ALARM: //����������������
        g_StringLanType(szLan, "����������������", "MINOR_ALARMHOST_EMERGENCY_CALL_HELP_ALARM");
        break;
    case MINOR_ALARMHOST_EMERGENCY_CALL_HELP_ALARM_RESTORE: //�����������������ָ�
        g_StringLanType(szLan, "�����������������ָ�", "MINOR_ALARMHOST_EMERGENCY_CALL_HELP_ALARM_RESTORE");
        break;
    case MINOR_ALARMHOST_CONSULTING_ALARM: //ҵ����ѯ����
        g_StringLanType(szLan, "ҵ����ѯ����", "MINOR_ALARMHOST_CONSULTING_ALARM");
        break;
    case MINOR_ALARMHOST_CONSULTING_ALARM_RESTORE: //ҵ����ѯ�����ָ�
        g_StringLanType(szLan, "ҵ����ѯ�����ָ�", "MINOR_ALARMHOST_CONSULTING_ALARM_RESTORE");
        break;
    case MINOR_ZONE_MODULE_REMOVE:// ����ģ�����
        g_StringLanType(szLan, "����ģ�����", "MINOR_ZONE_MODULE_REMOVE");
        break;
    case MINOR_ZONE_MODULE_RESET: // ����ģ�����λ
        g_StringLanType(szLan, "����ģ�����λ", "MINOR_ZONE_MODULE_RESET");
        break;

    case MINOR_ALARM_WIND_SPEED_ALARM:// ���ٴ������澯
        g_StringLanType(szLan, "���ٴ������澯", "MINOR_ALARM_WIND_SPEED_ALARM");
        break;
    case MINOR_ALARM_WIND_SPEED_ALARM_RESTORE: // ���ٴ������澯�ָ�
        g_StringLanType(szLan, "���ٴ������澯�ָ�", "MINOR_ALARM_WIND_SPEED_ALARM_RESTORE");
        break;
    case MINOR_ALARM_GENERATE_OUTPUT_ALARM:// ͨ�����ģ��澯
        g_StringLanType(szLan, "ͨ�����ģ��澯", "MINOR_ALARM_GENERATE_OUTPUT_ALARM");
        break;
    case MINOR_ALARM_GENERATE_OUTPUT_RESTORE: // ͨ�����ģ��澯�ָ�
        g_StringLanType(szLan, "ͨ�����ģ��澯�ָ�", "MINOR_ALARM_GENERATE_OUTPUT_RESTORE");
        break;
    case MINOR_ALARM_SOAK_ALARM:// ��ˮ�������澯
        g_StringLanType(szLan, "��ˮ�������澯", "MINOR_ALARM_SOAK_ALARM");
        break;
    case MINOR_ALARM_SOAK_ALARM_RESTORE: // ��ˮ�������澯�ָ�
        g_StringLanType(szLan, "��ˮ�������澯�ָ�", "MINOR_ALARM_SOAK_ALARM_RESTORE");
        break;
    case MINOR_ALARM_SOLAR_POWER_ALARM:// ̫���ܴ������澯
        g_StringLanType(szLan, "̫���ܴ������澯", "MINOR_ALARM_SOLAR_POWER_ALARM");
        break;
    case MINOR_ALARM_SOLAR_POWER_ALARM_RESTORE: // ̫���ܴ������澯�ָ�
        g_StringLanType(szLan, "̫���ܴ������澯�ָ�", "MINOR_ALARM_SOLAR_POWER_ALARM_RESTORE");
        break;
    case MINOR_ALARM_SF6_ALARM:// SF6���������澯
        g_StringLanType(szLan, "SF6���������澯", "MINOR_ALARM_SF6_ALARM");
        break;
    case MINOR_ALARM_SF6_ALARM_RESTORE: // SF6���������澯�ָ�
        g_StringLanType(szLan, "SF6���������澯�ָ�", "MINOR_ALARM_SF6_ALARM_RESTORE");
        break;
    case MINOR_ALARM_WEIGHT_ALARM:// �����Ǹ澯
        g_StringLanType(szLan, "�����Ǹ澯", "MINOR_ALARM_WEIGHT_ALARM");
        break;
    case MINOR_ALARM_WEIGHT_ALARM_RESTORE: // �����Ǹ澯�ָ�
        g_StringLanType(szLan, "�����Ǹ澯�ָ�", "MINOR_ALARM_WEIGHT_ALARM_RESTORE");
        break;
    case MINOR_ALARM_WEATHER_ALARM:// ����ɼ�ϵͳ�澯
        g_StringLanType(szLan, "����ɼ�ϵͳ�澯", "MINOR_ALARM_WEATHER_ALARM");
        break;
    case MINOR_ALARM_WEATHER_ALARM_RESTORE: // ����ɼ�ϵͳ�澯�ָ�
        g_StringLanType(szLan, "����ɼ�ϵͳ�澯�ָ�", "MINOR_ALARM_WEATHER_ALARM_RESTORE");
        break;
    case MINOR_ALARM_FUEL_GAS_ALARM:// ȼ�����ϵͳ�澯
        g_StringLanType(szLan, "ȼ�����ϵͳ�澯", "MINOR_ALARM_FUEL_GAS_ALARM");
        break;
    case MINOR_ALARM_FUEL_GAS_ALARM_RESTORE: // ȼ�����ϵͳ�澯�ָ�
        g_StringLanType(szLan, "ȼ�����ϵͳ�澯�ָ�", "MINOR_ALARM_FUEL_GAS_ALARM_RESTORE");
        break;
    case MINOR_ALARM_FIRE_ALARM:// ���ֱ���ϵͳ�澯
        g_StringLanType(szLan, "���ֱ���ϵͳ�澯", "MINOR_ALARM_FIRE_ALARM");
        break;
    case MINOR_ALARM_FIRE_ALARM_RESTORE: // ���ֱ���ϵͳ�澯�ָ�
        g_StringLanType(szLan, "���ֱ���ϵͳ�澯�ָ�", "MINOR_ALARM_FIRE_ALARM_RESTORE");
        break;
    case MINOR_WIRELESS_OUTPUT_MODULE_REMOVE:// �������ģ�����
        g_StringLanType(szLan, "�������ģ�����", "MINOR_WIRELESS_OUTPUT_MODULE_REMOVE");
        break;
    case MINOR_WIRELESS_OUTPUT_MODULE_RESET : // �������ģ�����λ
        g_StringLanType(szLan, "�������ģ�����λ", "MINOR_WIRELESS_OUTPUT_MODULE_RESET");
        break;
    case MINOR_WIRELESS_REPEATER_MODULE_REMOVE :// �����м�������
        g_StringLanType(szLan, "�����м�������", "MINOR_WIRELESS_REPEATER_MODULE_REMOVE");
        break;
    case MINOR_WIRELESS_REPEATER_MODULE_RESET: // �����м�������λ
        g_StringLanType(szLan, "�����м�������λ", "MINOR_WIRELESS_REPEATER_MODULE_RESET");
        break;
    case MINOR_WIRELESS_SIREN_MODULE_REMOVE :// ���߾��ŷ���
        g_StringLanType(szLan, "���߾��ŷ���", "MINOR_WIRELESS_SIREN_MODULE_REMOVE");
        break;
    case MINOR_WIRELESS_SIREN_MODULE_RESET: // ���߾��ŷ���λ
        g_StringLanType(szLan, "���߾��ŷ���λ", "MINOR_WIRELESS_SIREN_MODULE_RESET");
        break;
    case MINOR_SOUND_INTENSITY_RISE: // ��ǿ��������
        g_StringLanType(szLan, "��ǿ��������", "MINOR_SOUND_INTENSITY_RISE");
        break;
    case MINOR_SOUND_INTENSITY_RISE_RESET: // ��ǿ���������ָ�
        g_StringLanType(szLan, "��ǿ���������ָ�", "MINOR_SOUND_INTENSITY_RISE_RESET");
        break;
    case MINOR_SOUND_INTENSITY_DROP: // ��ǿ��������
        g_StringLanType(szLan, "��ǿ��������", "MINOR_SOUND_INTENSITY_DROP");
        break;
    case MINOR_SOUND_INTENSITY_DROP_RESET: // ��ǿ���������ָ�
        g_StringLanType(szLan, "��ǿ���������ָ�", "MINOR_SOUND_INTENSITY_DROP_RESET");
        break;
    case MINOR_AUDIO_INPUT_EXCEPTION: // ��Ƶ�����쳣����
        g_StringLanType(szLan, "��Ƶ�����쳣����", "MINOR_AUDIO_INPUT_EXCEPTION");
        break;
    case MINOR_AUDIO_INPUT_EXCEPTION_RESET: // ��Ƶ�����쳣�����ָ�
        g_StringLanType(szLan, "��Ƶ�����쳣�����ָ�", "MINOR_AUDIO_INPUT_EXCEPTION_RESET");
        break;
    default:
        sprintf(szLan, "%d", dwMinorType);  
        break;
    }
    szTemp.Format("%s", szLan);
}

void CDlgAlarmHostLogSearch::InfoMinorExceptionMap(DWORD dwMinorType, CString &szTemp)
{
    char szLan[256] = {0};
    switch (dwMinorType)
    {
    case MINOR_POWER_ON:	// �ϵ�
        g_StringLanType(szLan, "�ϵ�", "POWER_ON");
        break;
    case MINOR_POWER_OFF:				// ����
        g_StringLanType(szLan, "����", "POWER_OFF");
        break;
    case MINOR_WDT_RESET:				// WDT ��λ
        g_StringLanType(szLan, "WDT��λ", "WDT_RESET");
        break;
    case MINOR_LOW_BATTERY_VOLTAGE:		// ���ص�ѹ��
        g_StringLanType(szLan, "���ص�ѹ��", "LOW_BATTERY_VOLTAGE");
        break;
    case MINOR_AC_LOSS:					// ������ϵ�
        g_StringLanType(szLan, "������ϵ�", "AC_LOSS");
        break;
    case MINOR_AC_RESTORE:				// ������ָ�
        g_StringLanType(szLan, "������ָ�", "AC_RESTORE");
        break;
    case MINOR_RTC_EXCEPTION:			// RTCʵʱʱ���쳣
        g_StringLanType(szLan, "RTCʵʱʱ���쳣", "RTC_EXCEPTION");
        break;
    case MINOR_NETWORK_CONNECT_FAILURE:	// �������Ӷ�
        g_StringLanType(szLan, "�������Ӷ�", "NETWORK_CONNECT_FAILURE");
        break;
    case MINOR_NETWORK_CONNECT_RESTORE:	// �������ӻָ�
        g_StringLanType(szLan, "�������ӻָ�", "NETWORK_CONNECT_RESTORE");
        break;
    case MINOR_TEL_LINE_CONNECT_FAILURE:	// �绰�����Ӷ�
        g_StringLanType(szLan, "�绰�����Ӷ�", "TEL_LINE_CONNECT_FAILURE");
        break;
    case MINOR_TEL_LINE_CONNECT_RESTORE:	// �绰�����ӻָ�
        g_StringLanType(szLan, "�绰�����ӻָ�", "TEL_LINE_CONNECT_RESTORE");
        break;
    case MINOR_EXPANDER_BUS_LOSS:		// ��չ����ģ�����
        g_StringLanType(szLan, "��չ����ģ�����", "EXPANDER_BUS_LOSS");
        break;
    case MINOR_EXPANDER_BUS_RESTORE:		// ��չ����ģ����߻ָ�
        g_StringLanType(szLan, "��չ����ģ����߻ָ�", "EXPANDER_BUS_RESTORE");
        break;
    case MINOR_KEYPAD_BUS_LOSS:			// ��������ģ�����
        g_StringLanType(szLan, "��������ģ�����", "KEYPAD_BUS_LOSS");
        break;
    case MINOR_KEYPAD_BUS_RESTORE:		// ��������ģ����߻ָ�
        g_StringLanType(szLan, "��������ģ����߻ָ�", "KEYPAD_BUS_RESTORE");
        break;
    case MINOR_SENSOR_FAILURE:			// ģ��������������
        g_StringLanType(szLan, "ģ��������������", "SENSOR_FAILURE");
        break;
    case MINOR_SENSOR_RESTORE:			// ģ�����������ָ�
        g_StringLanType(szLan, "ģ�������������ϻָ�", "SENSOR_RESTORE");
        break;
    case MINOR_RS485_CONNECT_FAILURE:	// RS485ͨ�����Ӷ�
        g_StringLanType(szLan, "485ͨ�����Ӷ�", "RS485_CONNECT_FAILURE");
        break;
    case MINOR_RS485_CONNECT_RESTORE:	// RS485ͨ�����Ӷϻָ�
        g_StringLanType(szLan, "485ͨ�����Ӷϻָ�", "RS485_CONNECT_RESTORE");
        break;
    case MINOR_BATTERT_VOLTAGE_RESTORE:
        g_StringLanType(szLan, "���ص�ѹ�ָ�����", "BATTERT_VOLTAGE_RESTORE");
        break;
	case MINOR_WIRED_NETWORK_ABNORMAL:			// ���������쳣
        g_StringLanType(szLan, "���������쳣", "WIRED_NETWORK_ABNORMAL");
        break;
    case MINOR_WIRED_NETWORK_RESTORE:	// ��������ָ�����
        g_StringLanType(szLan, "��������ָ�����", "WIRED_NETWORK_RESTORE");
        break;
    case MINOR_GPRS_ABNORMAL:	// GPRSͨ���쳣
        g_StringLanType(szLan, "GPRSͨ���쳣", "GPRS_ABNORMAL");
        break;
    case MINOR_GPRS_RESTORE: //GPRS�ָ�����
        g_StringLanType(szLan, "GPRS�ָ�����", "GPRS_RESTORE");
        break;
	case MINOR_3G_ABNORMAL: //3Gͨ���쳣
        g_StringLanType(szLan, "3Gͨ���쳣", "3G_ABNORMAL");
        break;
	case MINOR_3G_RESTORE: //3G�ָ�����
        g_StringLanType(szLan, "3G�ָ�����", "3G_RESTORE");
        break;
	case MINOR_SIM_CARD_ABNORMAL: //SIM���쳣
        g_StringLanType(szLan, "SIM���쳣", "SIM_CARD_ABNORMAL");
        break;
	case MINOR_SIM_CARD_RESTORE: //SIM���ָ�����
        g_StringLanType(szLan, "SIM���ָ�����", "SIM_CARD_RESTORE");
        break;
	case MINOR_ALARMHOST_VI_LOST: //��Ƶ�źŶ�ʧ
        g_StringLanType(szLan, "��Ƶ�źŶ�ʧ", "ALARMHOST_VI_LOST");
        break;
	case MINOR_ALARMHOST_ILLEGAL_ACCESS: //�Ƿ�����
        g_StringLanType(szLan, "�Ƿ�����", "ALARMHOST_ILLEGAL_ACCESS");
        break;
	case MINOR_ALARMHOST_HD_FULL: //Ӳ����
        g_StringLanType(szLan, "Ӳ����", "ALARMHOST_HD_FULL");
        break;
	case MINOR_ALARMHOST_HD_ERROR: //Ӳ�̴���
        g_StringLanType(szLan, "Ӳ�̴���", "ALARMHOST_HD_ERROR");
        break;
	case MINOR_ALARMHOST_DCD_LOST: //MODEM ����
        g_StringLanType(szLan, "MODEM ����", "ALARMHOST_DCD_LOST");
        break;
	case MINOR_ALARMHOST_IP_CONFLICT: //IP��ַ��ͻ
        g_StringLanType(szLan, "IP��ַ��ͻ", "ALARMHOST_IP_CONFLICT");
        break;
	case MINOR_ALARMHOST_NET_BROKEN: //����Ͽ�
        g_StringLanType(szLan, "����Ͽ�", "ALARMHOST_NET_BROKEN");
        break;
	case MINOR_ALARMHOST_REC_ERROR: //¼�����
        g_StringLanType(szLan, "¼�����", "ALARMHOST_REC_ERROR");
        break;
	case MINOR_ALARMHOST_VI_EXCEPTION: //��Ƶ�����쳣
        g_StringLanType(szLan, "��Ƶ�����쳣", "ALARMHOST_VI_EXCEPTION");
        break;
	case MINOR_ALARMHOST_FORMAT_HDD_ERROR: //Զ�̸�ʽ��Ӳ��ʧ��
		g_StringLanType(szLan, "Զ�̸�ʽ��Ӳ��ʧ��", "ALARMHOST_FORMAT_HDD_ERROR");
        break;
	case MINOR_ALARMHOST_USB_ERROR: //USBͨ�Ź���
		g_StringLanType(szLan, "USBͨ�Ź���", "ALARMHOST_USB_ERROR");
        break;
	case MINOR_ALARMHOST_USB_RESTORE: //USBͨ�Ź��ϻָ�
		g_StringLanType(szLan, "USBͨ�Ź��ϻָ�", "ALARMHOST_USB_RESTORE");
        break;
	case MINOR_ALARMHOST_PRINT_ERROR: //��ӡ������
		g_StringLanType(szLan, "��ӡ������", "ALARMHOST_PRINT_ERROR");
        break;
	case MINOR_ALARMHOST_PRINT_RESTORE: //��ӡ�����ϻָ�
		g_StringLanType(szLan, "��ӡ�����ϻָ�", "ALARMHOST_PRINT_RESTORE");
        break;
	case MINOR_SUBSYSTEM_COMMUNICATION_ERROR: //�Ӱ�ͨѶ����
		g_StringLanType(szLan, "�Ӱ�ͨѶ����", "SUBSYSTEM_COMMUNICATION_ERROR");
        break;
	case MINOR_ALARMHOST_IPC_NO_LINK: /* IPC���ӶϿ�  */
		g_StringLanType(szLan, "IPC���ӶϿ�", "IPC_NO_LINK");
        break;
	case MINOR_ALARMHOST_IPC_IP_CONFLICT: /*ipc ip ��ַ ��ͻ*/
		g_StringLanType(szLan, "ipc ip ��ַ ��ͻ", "IPC_IP_CONFLICT");
        break;
	case MINOR_ALARMHOST_VI_MISMATCH: /*��Ƶ��ʽ��ƥ��*/
		g_StringLanType(szLan, "��Ƶ��ʽ��ƥ��", "VI_MISMATCH");
        break;
	case MINOR_ALARMHOST_GPRS_MODULE_FAULT: /*GPRSģ�����*/
		g_StringLanType(szLan, "GPRSģ�����", "GPRS_MODULE_FAULT");
		break;
	case MINOR_ALARMHOST_TELEPHONE_MODULE_FAULT: /*�绰ģ�����*/
		g_StringLanType(szLan, "�绰ģ�����", "TELEPHONE_MODULE_FAULT");
		break;

    case MINOR_ALARMHOST_WIFI_ABNORMAL:
        g_StringLanType(szLan, "WIFIͨ���쳣", "MINOR_ALARMHOST_WIFI_ABNORMAL");
        break;
    case MINOR_ALARMHOST_WIFI_RESTORE:
        g_StringLanType(szLan, "WIFI�ָ�����", "MINOR_ALARMHOST_WIFI_RESTORE");
        break;
    case MINOR_ALARMHOST_RF_ABNORMAL:
        g_StringLanType(szLan, "RF�ź��쳣", "MINOR_ALARMHOST_RF_ABNORMAL");
        break;
    case MINOR_ALARMHOST_RF_RESTORE:
        g_StringLanType(szLan, "RF�źŻָ�����", "MINOR_ALARMHOST_RF_RESTORE");
        break;
    case MINOR_ALARMHOST_DETECTOR_ONLINE:
        g_StringLanType(szLan, "̽��������", "MINOR_ALARMHOST_DETECTOR_ONLINE");
        break;
    case MINOR_ALARMHOST_DETECTOR_OFFLINE:
        g_StringLanType(szLan, "̽��������", "MINOR_ALARMHOST_DETECTOR_OFFLINE");
        break;
    case MINOR_ALARMHOST_DETECTOR_BATTERY_NORMAL:
        g_StringLanType(szLan, "̽������������", "MINOR_ALARMHOST_DETECTOR_BATTERY_NORMAL");
        break;
    case MINOR_ALARMHOST_DETECTOR_BATTERY_LOW:
        g_StringLanType(szLan, "̽��������Ƿѹ", "MINOR_ALARMHOST_DETECTOR_BATTERY_LOW");
        break;
    case MINOR_ALARMHOST_DATA_TRAFFIC_OVERFLOW:
        g_StringLanType(szLan, "��������", "MINOR_ALARMHOST_DATA_TRAFFIC_OVERFLOW");
        break;
    case MINOR_ZONE_MODULE_LOSS:
        g_StringLanType(szLan, "����ģ�����", "MINOR_ZONE_MODULE_LOSS");
        break;
    case MINOR_ZONE_MODULE_RESTORE:
        g_StringLanType(szLan, "����ģ����߻ָ�", "MINOR_ZONE_MODULE_RESTORE");
        break;
    case MINOR_ALARMHOST_WIRELESS_OUTPUT_LOSS:
        g_StringLanType(szLan, "�������ģ������", "MINOR_ALARMHOST_WIRELESS_OUTPUT_LOSS");
        break;
    case MINOR_ALARMHOST_WIRELESS_OUTPUT_RESTORE:
        g_StringLanType(szLan, "�������ģ��ָ�����", "MINOR_ALARMHOST_WIRELESS_OUTPUT_RESTORE");
        break;
    case MINOR_ALARMHOST_WIRELESS_REPEATER_LOSS:
        g_StringLanType(szLan, "�����м�������", "MINOR_ALARMHOST_WIRELESS_REPEATER_LOSS");
        break; 
    case MINOR_ALARMHOST_WIRELESS_REPEATER_RESTORE:
        g_StringLanType(szLan, "�����м�������", "MINOR_ALARMHOST_WIRELESS_REPEATER_RESTORE");
        break;
    case MINOR_TRIGGER_MODULE_LOSS:
        g_StringLanType(szLan, "������ģ�����", "MINOR_TRIGGER_MODULE_LOSS");
        break;
    case MINOR_TRIGGER_MODULE_RESTORE:
        g_StringLanType(szLan, "������ģ����߻ָ�", "MINOR_TRIGGER_MODULE_RESTORE");
        break;
    case MINOR_WIRELESS_SIREN_LOSS:
        g_StringLanType(szLan, "���߾�������", "MINOR_WIRELESS_SIREN_LOSS");
        break;
    case MINOR_WIRELESS_SIREN_RESTORE:
        g_StringLanType(szLan, "���߾��Żָ�����", "MINOR_WIRELESS_SIREN_RESTORE");
        break;
    default:
        sprintf(szLan, "%d", dwMinorType);
        break;
    }
    szTemp.Format("%s", szLan);
}

void CDlgAlarmHostLogSearch::InfoMinorOperationMap(DWORD dwMinorType, CString &szTemp)
{
    char szLan[256] = {0};
    switch (dwMinorType)
    {
    case MINOR_GUARD:	// ��ͨ����
        g_StringLanType(szLan, "��ͨ����", "GUARD");
        break;
    case MINOR_UNGUARD:					// ��ͨ����
        g_StringLanType(szLan, "��ͨ����", "UNGUARD");
        break;
    case MINOR_BYPASS:					// ��·
        g_StringLanType(szLan, "��·", "BYPASS");
        break;
    case MINOR_DURESS_ACCESS:			// Ю��
        g_StringLanType(szLan, "Ю��", "DURESS_ACCESS");
        break;
    case MINOR_ALARMHOST_LOCAL_REBOOT:	// ��������
        g_StringLanType(szLan, "��������", "ALARMHOST_LOCAL_REBOOT");
        break;
    case MINOR_ALARMHOST_REMOTE_REBOOT:	// Զ������
        g_StringLanType(szLan, "Զ������", "ALARMHOST_REMOTE_REBOOT");
        break;
    case MINOR_ALARMHOST_LOCAL_UPGRADE:	// ��������
        g_StringLanType(szLan, "��������", "ALARMHOST_LOCAL_UPGRADE");
        break;
    case MINOR_ALARMHOST_REMOTE_UPGRADE:	// Զ������
        g_StringLanType(szLan, "Զ������", "ALARMHOST_REMOTE_UPGRADE");
        break;
    case MINOR_RECOVERY_DEFAULT_PARAM:	// �ָ�Ĭ�ϲ���
        g_StringLanType(szLan, "�ָ�Ĭ�ϲ���", "RECOVERY_DEFAULT_PARAM");
        break;
    case MINOR_ALARM_OUTPUT:				// ���Ʊ������
        g_StringLanType(szLan, "���Ʊ������", "ALARM_OUTPUT");
        break;
    case MINOR_ACCESS_OPEN:				// �����Ž���
        g_StringLanType(szLan, "�����Ž���", "ACCESS_OPEN");
        break;
    case MINOR_ACCESS_CLOSE:				// �����Ž���
        g_StringLanType(szLan, "�����Ž���", "ACCESS_CLOSE");
        break;
    case MINOR_SIREN_OPEN:				// ���ƾ��ſ�
        g_StringLanType(szLan, "���ƾ��ſ�", "SIREN_OPEN");
        break;
    case MINOR_SIREN_CLOSE:				// ���ƾ��Ź�
        g_StringLanType(szLan, "���ƾ��Ź�", "SIREN_CLOSE");
        break;
    case MINOR_MOD_ZONE_CONFIG:		// �޸ķ�������
        g_StringLanType(szLan, "�޸ķ�������", "MOD_ZONE_CONFIG");
        break;
    case MINOR_MOD_ALARMOUT_CONIFG:	// ���Ʊ����������
        g_StringLanType(szLan, "�޸ı����������", "MOD_ALARMOUT_CONIFG");
        break;
    case MINOR_MOD_ANALOG_CONFIG:		// �޸�ģ��������
        g_StringLanType(szLan, "�޸�ģ��������", "MOD_ANALOG_CONFIG");
        break;
    case MINOR_RS485_CONFIG:				// �޸�485ͨ������
        g_StringLanType(szLan, "�޸�485ͨ������", "RS485_CONFIG");
        break;
    case MINOR_PHONE_CONFIG:				// �޸Ĳ�������
        g_StringLanType(szLan, "�޸Ĳ�������", "PHONE_CONFIG");
        break;
    case MINOR_ADD_ADMIN:        // ���ӹ���Ա
        g_StringLanType(szLan, "���ӹ���Ա", "ADD_ADMIN");
        break;
    case MINOR_MOD_ADMIN_PARAM:	// �޸Ĺ���Ա����
        g_StringLanType(szLan, "�޸Ĺ���Ա����", "MOD_ADMIN_PARAM");
        break;
    case MINOR_DEL_ADMIN:		// ɾ������Ա
        g_StringLanType(szLan, "ɾ������Ա", "DEL_ADMIN");
        break;
    case MINOR_ADD_NETUSER:		// ���Ӻ�˲���Ա
        g_StringLanType(szLan, "���Ӻ�˲���Ա", "ADD_NETUSER");
        break;
    case MINOR_MOD_NETUSER_PARAM:	// �޸ĺ�˲���Ա����
        g_StringLanType(szLan, "�޸ĺ�˲���Ա����", "MOD_NETUSER_PARAM");
        break;
    case MINOR_DEL_NETUSER:				// ɾ����˲���Ա
        g_StringLanType(szLan, "ɾ����˲���Ա", "DEL_NETUSER");
        break;
    case MINOR_ADD_OPERATORUSER:			// ����ǰ�˲���Ա
        g_StringLanType(szLan, "����ǰ�˲���Ա", "ADD_OPERATORUSER");
        break;
    case MINOR_MOD_OPERATORUSER_PW:		//�޸�ǰ�˲���Ա����
        g_StringLanType(szLan, "�޸�ǰ�˲���Ա����", "MOD_OPERATORUSER_PW");
        break;
    case MINOR_DEL_OPERATORUSER:			// ɾ��ǰ�˲���Ա
        g_StringLanType(szLan, "ɾ��ǰ�˲���Ա", "DEL_OPERATORUSER");
        break;
    case MINOR_ADD_KEYPADUSER:			// ���Ӽ���/�������û�	
        g_StringLanType(szLan, "���Ӽ���/�������û�	", "ADD_KEYPADUSER");
        break;
    case MINOR_DEL_KEYPADUSER:			// ɾ������/�������û�	
        g_StringLanType(szLan, "ɾ������/������", "DEL_KEYPADUSER");
        break;
    case MINOR_REMOTEUSER_LOGIN:		// Զ���û���½
        g_StringLanType(szLan, "Զ���û���½", "REMOTEUSER_LOGIN");
        break;
    case MINOR_REMOTEUSER_LOGOUT:		// Զ���û�ע��
        g_StringLanType(szLan, "Զ���û�ע��", "REMOTEUSER_LOGOUT");
        break;
    case MINOR_REMOTE_GUARD:				// Զ�̲���
        g_StringLanType(szLan, "Զ�̲���", "REMOTE_GUARD");
        break;
    case MINOR_REMOTE_UNGUARD:			// Զ�̳���
        g_StringLanType(szLan, "Զ�̳���", "REMOTE_UNGUARD");
        break;
    case MINOR_MOD_HOST_CONFIG:
        g_StringLanType(szLan, "�޸���������", "MOD_HOST_CONFIG");
        break;
    case MINOR_RESTORE_BYPASS:
        g_StringLanType(szLan, "��·�ָ�", "RESTORE_BYPASS");
        break;
    case MINOR_ALARMOUT_OPEN:	//�����������
        g_StringLanType(szLan, "�����������", "ALARMOUT_OPEN");
        break;
    case MINOR_ALARMOUT_CLOSE:	//��������ر�
        g_StringLanType(szLan, "��������ر�", "ALARMOUT_CLOSE");
        break;
    case MINOR_MOD_SUBSYSTEM_PARAM:	//�޸���ϵͳ��������
        g_StringLanType(szLan, "�޸���ϵͳ��������", "MOD_SUBSYSTEM_PARAM");
        break;
    case MINOR_GROUP_BYPASS:	//����·
        g_StringLanType(szLan, "����·", "GROUP_BYPASS");
        break;
    case MINOR_RESTORE_GROUP_BYPASS:	//����·�ָ�
        g_StringLanType(szLan, "����·�ָ�", "RESTORE_GROUP_BYPASS");
        break;
    case MINOR_MOD_GRPS_PARAM:	//�޸�GPRS����
        g_StringLanType(szLan, "�޸�GPRS����", "MOD_GRPS_PARAM");
        break;
    case MINOR_MOD_NET_REPORT_PARAM:	//�޸������ϱ���������
        g_StringLanType(szLan, "�޸������ϱ���������", "MOD_NET_REPORT_PARAM");
        break;
    case MINOR_MOD_REPORT_MOD:	//�޸��ϴ���ʽ����
        g_StringLanType(szLan, "�޸��ϴ���ʽ����", "MOD_REPORT_MOD");
        break;
    case MINOR_MOD_GATEWAY_PARAM:	//�޸��Ž���������
        g_StringLanType(szLan, "�޸��Ž���������", "MOD_GATEWAY_PARAM");
        break;
    case MINOR_ALARMHOST_REMOTE_START_REC:	//Զ�̿�ʼ¼��
        g_StringLanType(szLan, "Զ�̿�ʼ¼��", "ALARMHOST_REMOTE_START_REC");
        break;
    case MINOR_ALARMHOST_REMOTE_STOP_REC:	//Զ��ֹͣ¼��
        g_StringLanType(szLan, "Զ��ֹͣ¼��", "ALARMHOST_REMOTE_STOP_REC");
        break;
    case MINOR_ALARMHOST_START_TRANS_CHAN:	//��ʼ͸������
        g_StringLanType(szLan, "��ʼ͸������", "ALARMHOST_START_TRANS_CHAN");
        break;
    case MINOR_ALARMHOST_STOP_TRANS_CHAN:	//ֹͣ͸������
        g_StringLanType(szLan, "ֹͣ͸������", "ALARMHOST_STOP_TRANS_CHAN");
        break;
    case MINOR_ALARMHOST_START_VT:	//��ʼ�����Խ�
        g_StringLanType(szLan, "��ʼ�����Խ�", "ALARMHOST_START_VT");
        break;
	case MINOR_ALARMHOST_STOP_VTM:	//ֹͣ�����Խ�
        g_StringLanType(szLan, "ֹͣ�����Խ�", "ALARMHOST_STOP_VTM");
        break;
    case MINOR_ALARMHOST_REMOTE_PLAYBYFILE:	//Զ�̰��ļ��ط�
        g_StringLanType(szLan, "Զ�̰��ļ��ط�", "ALARMHOST_REMOTE_PLAYBYFILE");
        break;
	case MINOR_ALARMHOST_REMOTE_PLAYBYTIME:	//Զ�̰�ʱ��ط�
        g_StringLanType(szLan, "Զ�̰�ʱ��ط�", "ALARMHOST_REMOTE_PLAYBYTIME");
        break;
    case MINOR_ALARMHOST_REMOTE_PTZCTRL:	//Զ����̨����
        g_StringLanType(szLan, "Զ����̨����", "ALARMHOST_REMOTE_PTZCTRL");
        break;
	case MINOR_ALARMHOST_REMOTE_FORMAT_HDD:	//Զ�̸�ʽ��Ӳ��
        g_StringLanType(szLan, "Զ�̸�ʽ��Ӳ��", "ALARMHOST_REMOTE_FORMAT_HDD");
        break;
    case MINOR_ALARMHOST_REMOTE_LOCKFILE:	//Զ�������ļ�
        g_StringLanType(szLan, "Զ�������ļ�", "ALARMHOST_REMOTE_LOCKFILE");
        break;
	case MINOR_ALARMHOST_REMOTE_UNLOCKFILE:	//Զ�̽����ļ�
        g_StringLanType(szLan, "Զ�̽����ļ�", "ALARMHOST_REMOTE_UNLOCKFILE");
        break;
    case MINOR_ALARMHOST_REMOTE_CFGFILE_OUTPUT:	//Զ�̵��������ļ�
        g_StringLanType(szLan, "Զ�̵��������ļ�", "ALARMHOST_REMOTE_CFGFILE_OUTPUT");
        break;
	case MINOR_ALARMHOST_REMOTE_CFGFILE_INTPUT:	//Զ�̵��������ļ�
        g_StringLanType(szLan, "Զ�̵��������ļ�", "ALARMHOST_REMOTE_CFGFILE_INTPUT");
        break;
    case MINOR_ALARMHOST_STAY_ARM:	//���ز���
        g_StringLanType(szLan, "���ز���", "ALARMHOST_STAY_ARM");
        break;
    case MINOR_ALARMHOST_QUICK_ARM:	//��ʱ����
        g_StringLanType(szLan, "��ʱ����", "ALARMHOST_QUICK_ARM");
        break;
	case MINOR_ALARMHOST_AUTOMATIC_ARM:	//�Զ�����
        g_StringLanType(szLan, "�Զ�����", "ALARMHOST_AUTOMATIC_ARM");
        break;
	case MINOR_ALARMHOST_AUTOMATIC_DISARM:	//�Զ�����
        g_StringLanType(szLan, "�Զ�����", "ALARMHOST_AUTOMATIC_DISARM");
        break;
	case MINOR_ALARMHOST_KEYSWITCH_ARM:	//Կ�׷�������
        g_StringLanType(szLan, "Կ�׷�������", "ALARMHOST_KEYSWITCH_ARM");
        break;
	case MINOR_ALARMHOST_KEYSWITCH_DISARM:	//Կ�׷�������
        g_StringLanType(szLan, "Կ�׷�������", "ALARMHOST_KEYSWITCH_DISARM");
        break;

	case MINOR_ALARMHOST_CLEAR_ALARM:	//����
        g_StringLanType(szLan, "����", "ALARMHOST_CLEAR_ALARM");
        break;
	case MINOR_ALARMHOST_MOD_FAULT_CFG:	//�޸�ϵͳ��������
        g_StringLanType(szLan, "�޸�ϵͳ��������", "ALARMHOST_MOD_FAULT_CFG");
        break;
	case MINOR_ALARMHOST_MOD_EVENT_TRIGGER_ALARMOUT_CFG:	//�޸��¼����������������
        g_StringLanType(szLan, "�޸��¼����������������", "LARMHOST_MOD_EVENT_TRIGGER_ALARMOUT_CFG");
        break;
	case MINOR_ALARMHOST_SEARCH_EXTERNAL_MODULE:	//�������ģ��
        g_StringLanType(szLan, "�������ģ��", "ALARMHOST_SEARCH_EXTERNAL_MODULE");
        break;
	case MINOR_ALARMHOST_REGISTER_EXTERNAL_MODULE:	//����ע�����ģ��
        g_StringLanType(szLan, "����ע�����ģ��", "ALARMHOST_REGISTER_EXTERNAL_MODULE");
        break;
	case MINOR_ALARMHOST_CLOSE_KEYBOARD_ALARM:	//�رռ��̹�����ʾ��
        g_StringLanType(szLan, "�رռ��̹�����ʾ��", "ALARMHOST_CLOSE_KEYBOARD_ALARM");
        break;
	case MINOR_ALARMHOST_MOD_3G_PARAM:	//�޸�3G����
        g_StringLanType(szLan, "�޸�3G����", "ALARMHOST_MOD_3G_PARAM");
        break;
	case MINOR_ALARMHOST_MOD_PRINT_PARAM:
		g_StringLanType(szLan, "�޸Ĵ�ӡ������", "ALARMHOST_MOD_PRINT_PARAM");
        break;
	case MINOR_SD_CARD_FORMAT:
		g_StringLanType(szLan, "SD����ʽ��", "SD_CARD_FORMAT");
        break;
	case MINOR_SUBSYSTEM_UPGRADE:
		g_StringLanType(szLan, "�Ӱ�̼�����", "SUBSYSTEM_UPGRADE");
		break;		
	case MINOR_ALARMHOST_PLAN_ARM_CFG:
		g_StringLanType(szLan, "�ƻ���������������", "ALARMHOST_PLAN_ARM_CFG");
		break;
	case MINOR_ALARMHOST_PHONE_ARM:
		g_StringLanType(szLan, "�ֻ�����", "ALARMHOST_PHONE_ARM");
		break;
	case MINOR_ALARMHOST_PHONE_STAY_ARM:
		g_StringLanType(szLan, "�ֻ����ز���", "ALARMHOST_PHONE_STAY_ARM");
		break;
	case MINOR_ALARMHOST_PHONE_QUICK_ARM:
		g_StringLanType(szLan, "�ֻ���ʱ����", "ALARMHOST_PHONE_QUICK_ARM");
		break;
	case MINOR_ALARMHOST_PHONE_DISARM:
		g_StringLanType(szLan, "�ֻ�����", "ALARMHOST_PHONE_DISARM");
		break;
	case MINOR_ALARMHOST_PHONE_CLEAR_ALARM:
		g_StringLanType(szLan, "�ֻ�����", "ALARMHOST_PHONE_CLEAR_ALARM");
		break;
	case MINOR_ALARMHOST_WHITELIST_CFG:
		g_StringLanType(szLan, "����������", "ALARMHOST_WHITELIST_CFG");
		break;
	case MINOR_ALARMHOST_TIME_TRIGGER_CFG:
		g_StringLanType(szLan, "��ʱ���ش���������", "ALARMHOST_TIME_TRIGGER_CFG");
		break;
	case MINOR_ALARMHOST_CAPTRUE_CFG:
		g_StringLanType(szLan, "ץͼ��������", "ALARMHOST_CAPTRUE_CFG");
		break;
	case MINOR_ALARMHOST_TAMPER_CFG:
		g_StringLanType(szLan, "���������������", "ALARMHOST_TAMPER_CFG");
		break;
	case MINOR_ALARMHOST_REMOTE_KEYPAD_UPGRADE:
		g_StringLanType(szLan, "Զ����������", "REMOTE_KEYPAD_UPGRADE");
		break;
	case MINOR_ALARMHOST_ONETOUCH_AWAY_ARMING:
		g_StringLanType(szLan, "һ���������", "ONETOUCH_AWAY_ARMING");
		break;
	case MINOR_ALARMHOST_ONETOUCH_STAY_ARMING:
		g_StringLanType(szLan, "һ�����ز���", "ONETOUCH_STAY_ARMING");
		break;
	case MINOR_ALARMHOST_SINGLE_PARTITION_ARMING_OR_DISARMING:
		g_StringLanType(szLan, "������������", "SINGLE_PARTITION_ARMING_OR_DISARMING");
		break;
	case MINOR_ALARMHOST_CARD_CONFIGURATION:
		g_StringLanType(szLan, "����������", "CARD_CONFIGURATION");
		break;
	case MINOR_ALARMHOST_CARD_ARMING_OR_DISARMING:
		g_StringLanType(szLan, "ˢ��������", "CARD_ARMING_OR_DISARMING");
		break;
	case MINOR_ALARMHOST_EXPENDING_NETCENTER_CONFIGURATION:
		g_StringLanType(szLan, "��չ������������", "EXPENDING_NETCENTER_CONFIGURATION");
		break;
	case MINOR_ALARMHOST_NETCARD_CONFIGURATION:
		g_StringLanType(szLan, "��������", "NETCARD_CONFIGURATION");
		break;
	case MINOR_ALARMHOST_DDNS_CONFIGURATION:
		g_StringLanType(szLan, "DDNS����", "DDNS_CONFIGURATION");
		break;
	case MINOR_ALARMHOST_RS485BUS_CONFIGURATION:
		g_StringLanType(szLan, "485���߲�������", "RS485BUS_CONFIGURATION");
		break;
	case MINOR_ALARMHOST_RS485BUS_RE_REGISTRATION:
		g_StringLanType(szLan, "485��������ע��", "RS485BUS_RE_REGISTRATION");
		break;

    case MINOR_ALARMHOST_REMOTE_OPEN_ELECTRIC_LOCK:
        g_StringLanType(szLan, "Զ�̴򿪵���", "ALARMHOST_REMOTE_OPEN_ELECTRIC_LOCK");
        break;
    case MINOR_ALARMHOST_REMOTE_CLOSE_ELECTRIC_LOCK:
        g_StringLanType(szLan, "Զ�̹رյ���", "ALARMHOST_REMOTE_CLOSE_ELECTRIC_LOCK");
        break;
    case MINOR_ALARMHOST_LOCAL_OPEN_ELECTRIC_LOCK:
        g_StringLanType(szLan, "���ش򿪵���", "ALARMHOST_LOCAL_OPEN_ELECTRIC_LOCK");
        break;
    case MINOR_ALARMHOST_LOCAL_CLOSE_ELECTRIC_LOCK:
        g_StringLanType(szLan, "���عرյ���", "ALARMHOST_LOCAL_CLOSE_ELECTRIC_LOCK");
        break;
    case MINOR_ALARMHOST_OPEN_ALARM_LAMP:
        g_StringLanType(szLan, "�򿪾���(Զ��)", "ALARMHOST_OPEN_ALARM_LAMP");
        break;
    case MINOR_ALARMHOST_CLOSE_ALARM_LAMP:
        g_StringLanType(szLan, "�رվ���(Զ��)", "ALARMHOST_CLOSE_ALARM_LAMP");
        break;
    case MINOR_ALARMHOST_TEMPORARY_PASSWORD:
        g_StringLanType(szLan, "��ʱ���������¼", "ALARMHOST_TEMPORARY_PASSWORD");
        break;
    case MINOR_ALARMHOST_ONEKEY_AWAY_ARM:
        g_StringLanType(szLan, "һ���������", "ALARMHOST_ONEKEY_AWAY_ARM");
        break;
    case MINOR_ALARMHOST_ONEKEY_STAY_ARM:
        g_StringLanType(szLan, "һ�����ز���", "MINOR_ALARMHOST_ONEKEY_STAY_ARM");
        break;
    case MINOR_ALARMHOST_SINGLE_ZONE_ARM:
        g_StringLanType(szLan, "����������", "MINOR_ALARMHOST_SINGLE_ZONE_ARM");
        break;
    case MINOR_ALARMHOST_SINGLE_ZONE_DISARM:
        g_StringLanType(szLan, "����������", "MINOR_ALARMHOST_SINGLE_ZONE_DISARM");
        break;
    case MINOR_ALARMHOST_HIDDNS_CONFIG:
        g_StringLanType(szLan, "HIDDNS����", "MINOR_ALARMHOST_HIDDNS_CONFIG");
        break;
    case MINOR_ALARMHOST_REMOTE_KEYBOARD_UPDATA:
        g_StringLanType(szLan, "Զ�̼���������־", "MINOR_ALARMHOST_REMOTE_KEYBOARD_UPDATA");
        break;
    case MINOR_ALARMHOST_ZONE_ADD_DETECTOR:
        g_StringLanType(szLan, "�������̽����", "MINOR_ALARMHOST_ZONE_ADD_DETECTOR");
        break;
    case MINOR_ALARMHOST_ZONE_DELETE_DETECTOR:
        g_StringLanType(szLan, "����ɾ��̽����", "MINOR_ALARMHOST_ZONE_DELETE_DETECTOR");
        break;
    case MINOR_ALARMHOST_QUERY_DETECTOR_SIGNAL:
        g_StringLanType(szLan, "������ѯ̽�����ź�ǿ��", "MINOR_ALARMHOST_QUERY_DETECTOR_SIGNAL");
        break;
    case MINOR_ALARMHOST_QUERY_DETECTOR_BATTERY:
        g_StringLanType(szLan, "������ѯ̽��������", "MINOR_ALARMHOST_QUERY_DETECTOR_BATTERY");
        break;
    case MINOR_ALARMHOST_SET_DETECTOR_GUARD:
        g_StringLanType(szLan, "̽��������", "MINOR_ALARMHOST_SET_DETECTOR_GUARD");
        break;
    case MINOR_ALARMHOST_SET_DETECTOR_UNGUARD:
        g_StringLanType(szLan, "̽��������", "MINOR_ALARMHOST_SET_DETECTOR_UNGUARD");
        break;
    case MINOR_ALARMHOST_SET_WIFI_PARAMETER:
        g_StringLanType(szLan, "����WIFI���ò���", "MINOR_ALARMHOST_SET_WIFI_PARAMETER");
        break;
    case MINOR_ALARMHOST_OPEN_VOICE:
        g_StringLanType(szLan, "������", "MINOR_ALARMHOST_OPEN_VOICE");
        break;
    case MINOR_ALARMHOST_CLOSE_VOICE:
        g_StringLanType(szLan, "�ر�����", "MINOR_ALARMHOST_CLOSE_VOICE");
        break;
    case MINOR_ALARMHOST_ENABLE_FUNCTION_KEY:
        g_StringLanType(szLan, "���ù��ܼ�", "MINOR_ALARMHOST_ENABLE_FUNCTION_KEY");
        break;
    case MINOR_ALARMHOST_DISABLE_FUNCTION_KEY://121
        g_StringLanType(szLan, "�رչ��ܼ�", "MINOR_ALARMHOST_DISABLE_FUNCTION_KEY");
        break;
    case MINOR_ALARMHOST_READ_CARD:
        g_StringLanType(szLan, "Ѳ��ˢ��", "ALARMHOST_READ_CARD");
        break;
    case MINOR_ALARMHOST_START_BROADCAST:
        g_StringLanType(szLan, "�������㲥", "ALARMHOST_START_BROADCAST");
        break;
    case MINOR_ALARMHOST_STOP_BROADCAST:
        g_StringLanType(szLan, "�ر������㲥", "ALARMHOST_STOP_BROADCAST");
        break;
    case MINOR_ALARMHOST_REMOTE_ZONE_MODULE_UPGRADE:
        g_StringLanType(szLan, "Զ����������ģ��", "ALARMHOST_REMOTE_ZONE_MODULE_UPGRADE");
        break;
    case MINOR_ALARMHOST_NETWORK_MODULE_EXTEND:
        g_StringLanType(szLan, "����ģ���������", "ALARMHOST_NETWORK_MODULE_EXTEND");
        break;
    case MINOR_ALARMHOST_ADD_CONTROLLER:
        g_StringLanType(szLan, "���ң�����û�", "ALARMHOST_ADD_CONTROLLER");
        break;
    case MINOR_ALARMHOST_DELETE_CONTORLLER:
        g_StringLanType(szLan, "ɾ��ң�����û�", "ALARMHOST_DELETE_CONTORLLER");
        break;
    case MINOR_ALARMHOST_REMOTE_NETWORKMODULE_UPGRADE:
        g_StringLanType(szLan, "Զ����������ģ��", "ALARMHOST_REMOTE_NETWORKMODULE_UPGRADE");
        break;
    case MINOR_ALARMHOST_WIRELESS_OUTPUT_ADD:
        g_StringLanType(szLan, "ע���������ģ��", "ALARMHOST_WIRELESS_OUTPUT_ADD");
        break;
    case MINOR_ALARMHOST_WIRELESS_OUTPUT_DEL:
        g_StringLanType(szLan, "ɾ���������ģ��", "ALARMHOST_WIRELESS_OUTPUT_DEL");
        break;
    case MINOR_ALARMHOST_WIRELESS_REPEATER_ADD:
        g_StringLanType(szLan, "ע�������м���", "ALARMHOST_WIRELESS_REPEATER_ADD");
        break;
    case MINOR_ALARMHOST_WIRELESS_REPEATER_DEL:
        g_StringLanType(szLan, "ɾ�������м���", "ALARMHOST_WIRELESS_REPEATER_DEL");
        break;
    case MINOR_ALARMHOST_PHONELIST_CFG:
        g_StringLanType(szLan, "�绰������������", "ALARMHOST_PHONELIST_CFG");
        break;
    case MINOR_ALARMHOST_RF_SIGNAL_CHECK:
        g_StringLanType(szLan, "RF�źŲ�ѯ", "ALARMHOST_RF_SIGNAL_CHECK");
        break;
    case MINOR_ALARMHOST_USB_UPGRADE:
        g_StringLanType(szLan, "MINOR_ALARMHOST_USB_UPGRADE", "ALARMHOST_USB_UPGRADE");
        break;
    case MINOR_ALARMHOST_DOOR_TIME_REMINDER_CFG:
        g_StringLanType(szLan, "�ŴŶ�ʱ���Ѳ�������", "ALARMHOST_DOOR_TIME_REMINDER_CFG");
        break;
    case MINOR_ALARMHOST_WIRELESS_SIREN_ADD:
        g_StringLanType(szLan, "ע�����߾���", "ALARMHOST_WIRELESS_SIREN_ADD");
        break;
    case MINOR_ALARMHOST_WIRELESS_SIREN_DEL:
        g_StringLanType(szLan, "ɾ�����߾���", "ALARMHOST_WIRELESS_SIREN_DEL");
        break;
    case MINOR_ALARMHOST_OUT_SCALE_OPEN:
        g_StringLanType(szLan, "���翪��", "ALARMHOST_OUT_SCALE_OPEN");
        break;
    case MINOR_ALARMHOST_OUT_SCALE_CLOSE:
        g_StringLanType(szLan, "����ر�", "ALARMHOST_OUT_SCALE_CLOSE");
        break;
	case MINOR_ALARMHOST_LOCAL_SET_DEVICE_ACTIVE://0xf0=240
		g_StringLanType(szLan, "���ؼ����豸", "ALARMHOST_LOCAL_SET_DEVICE_ACTIVE");
		break;
	case MINOR_ALARMHOST_REMOTE_SET_DEVICE_ACTIVE:
		g_StringLanType(szLan, "Զ�̼����豸", "ALARMHOST_LOCAL_SET_DEVICE_ACTIVE");
		break;
	case MINOR_ALARMHOST_LOCAL_PARA_FACTORY_DEFAULT:
		g_StringLanType(szLan, "���ػָ���������", "ALARMHOST_LOCAL_PARA_FACTORY_DEFAULT");
		break;
	case MINOR_ALARMHOST_REMOTE_PARA_FACTORY_DEFAULT:
		g_StringLanType(szLan, "Զ�ָ̻���������", "ALARMHOST_LOCAL_PARA_FACTORY_DEFAULT");
		break;
    default:
        sprintf(szLan, "%d", dwMinorType);
        break;
    }
    szTemp.Format("%s", szLan);
}

void CDlgAlarmHostLogSearch::InfoMinorEventMap(DWORD dwMinorType, CString &szTemp)
{
    char szLan[256] = {0};
    switch (dwMinorType)
    {
    case MINOR_SCHOOLTIME_IRGI_B:		// B��Уʱ
        g_StringLanType(szLan, "B��Уʱ", "SCHOOLTIME_IRGI_B");
        break;
    case MINOR_SCHOOLTIME_SDK:				// SDKУʱ
        g_StringLanType(szLan, "SDKУʱ", "SCHOOLTIME_SDK");
        break;
    case MINOR_SCHOOLTIME_SELFTEST:			// ��ʱ�Լ�Уʱ
        g_StringLanType(szLan, "��ʱ�Լ�Уʱ", "SCHOOLTIME_SELFTEST");
        break;
	case MINOR_SUBSYSTEM_ABNORMALINSERT:			// �Ӱ����
        g_StringLanType(szLan, "�Ӱ����", "SUBSYSTEM_ABNORMALINSERT");
        break;
	case MINOR_SUBSYSTEM_ABNORMALPULLOUT:			// �Ӱ�γ�
        g_StringLanType(szLan, "�Ӱ�γ�", "SUBSYSTEM_ABNORMALPULLOUT");
        break;

	case MINOR_AUTO_ARM:			//�Զ�����
        g_StringLanType(szLan, "�Զ�����", "AUTO_ARM");
        break;
	case MINOR_AUTO_DISARM:			//�Զ�����
        g_StringLanType(szLan, "�Զ�����", "AUTO_DISARM");
        break;
	case MINOR_TIME_TIGGER_ON:			// ��ʱ����������
        g_StringLanType(szLan, "��ʱ����������", "TIME_TIGGER_ON");
        break;
	case MINOR_TIME_TIGGER_OFF:			//��ʱ�رմ�����
        g_StringLanType(szLan, "��ʱ�رմ�����", "TIME_TIGGER_OFF");
        break;
	case MINOR_AUTO_ARM_FAILD:			//�Զ�����ʧ��
        g_StringLanType(szLan, "�Զ�����ʧ��", "AUTO_ARM_FAILD");
        break;
	case MINOR_AUTO_DISARM_FAILD:			//�Զ�����ʧ��
        g_StringLanType(szLan, "�Զ�����ʧ��", "AUTO_DISARM_FAILD");
        break;
	case MINOR_TIME_TIGGER_ON_FAILD:			//��ʱ����������ʧ��
        g_StringLanType(szLan, "��ʱ����������ʧ��", "TIME_TIGGER_ON_FAILD");
        break;
	case MINOR_TIME_TIGGER_OFF_FAILD:			//��ʱ�رմ�����ʧ��
        g_StringLanType(szLan, "��ʱ�رմ�����ʧ��", "TIME_TIGGER_OFF_FAILD");
        break;
	case MINOR_MANDATORY_ALARM:			//ǿ�Ʋ���
        g_StringLanType(szLan, "ǿ�Ʋ���", "MANDATORY_ALARM");
        break;
	case MINOR_KEYPAD_LOCKED:			//��������
		g_StringLanType(szLan, "��������", "KEYPAD_LOCKED");
		break;
    case MINOR_USB_INSERT:
        g_StringLanType(szLan, "USB����", "MINOR_USB_INSERT");
        break;
    case MINOR_USB_PULLOUT:
        g_StringLanType(szLan, "USB�γ�", "MINOR_USB_PULLOUT");
        break;
    default:
        sprintf(szLan, "%d", dwMinorType);
        break;
    }
    szTemp.Format("%s", szLan);
}

void CDlgAlarmHostLogSearch::InfoMajorTypeMap(DWORD dwMajorType, CString &szTemp)
{
    char szLan[256] = {0};
    switch (dwMajorType)
    {
    case MAJOR_ALARMHOST_ALARM:
        g_StringLanType(szLan, "����", "ALARM");
        break;
    case MAJOR_ALARMHOST_EXCEPTION:
        g_StringLanType(szLan, "�쳣", "EXCEPTION");
        break;
    case MAJOR_ALARMHOST_OPERATION:
        g_StringLanType(szLan, "����", "OPERATION");
        break;
    case MAJ0R_ALARMHOST_EVENT:
        g_StringLanType(szLan, "�¼�", "EVENT");
        break;
    default:
        break;
    }
    szTemp.Format("%s", szLan);
}

void CDlgAlarmHostLogSearch::OnBtnExportLog() 
{
    CString csFileName = "";
	/*
	CString csTmp = "";
	GetCurrentDirectory(1024, csTmp.GetBuffer(0));

    csFileName += csTmp;
    csFileName.Format("%s%s",csFileName.GetBuffer(0), "\\Alarm host log");
	*/
    
	CString csTmp = "";
	char strTemp1[1024];
	GetCurrentDirectory(1024, strTemp1);
	csTmp.Format("%s", strTemp1);
	sprintf(strTemp1 + strlen(strTemp1), "\\Alarm host log");

	csFileName.Format("%s", strTemp1);

    if (!CreateDirectory(csFileName, NULL))
    {
        TRACE("CreateDirectory Fail");
    }
    

    SYSTEMTIME t;
    GetLocalTime(&t);
    csTmp.Format("%4.4d%2.2d%2.2d%2.2d%2.2d%2.2d%3.3d", t.wYear,t.wMonth,t.wDay,t.wHour,t.wMinute,t.wSecond,t.wMilliseconds);
    
    csFileName = csFileName + "\\" + csTmp + ".txt";
    
    HANDLE hFile = INVALID_HANDLE_VALUE;

    hFile = CreateFile(csFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    
    int i = 0;
    char szLan[4096] = {0};
    CString strTmp = "";
    for (i = 0; i < m_listAlarmHostLog.GetItemCount(); i++)
    {
        memset(szLan, 0, sizeof(szLan));
        strTmp = m_listAlarmHostLog.GetItemText(i, 0);
        sprintf(szLan, "%s  %s", szLan, strTmp.GetBuffer(0));
        
        strTmp = m_listAlarmHostLog.GetItemText(i, 1);
        sprintf(szLan, "%s  %s", szLan, strTmp.GetBuffer(0));

        strTmp = m_listAlarmHostLog.GetItemText(i, 2);
        sprintf(szLan, "%s  %s", szLan, strTmp.GetBuffer(0));
           
        strTmp = m_listAlarmHostLog.GetItemText(i, 3);
        sprintf(szLan, "%s  %s", szLan, strTmp.GetBuffer(0));

        strTmp  = m_listAlarmHostLog.GetItemText(i, 4);
        sprintf(szLan, "%s  %s", szLan, strTmp.GetBuffer(0));

        strTmp  = m_listAlarmHostLog.GetItemText(i, 5);
        sprintf(szLan, "%s  %s", szLan, strTmp.GetBuffer(0));

        strTmp  = m_listAlarmHostLog.GetItemText(i, 6);
        sprintf(szLan, "%s  %s", szLan, strTmp.GetBuffer(0));

        strTmp  = m_listAlarmHostLog.GetItemText(i, 7);
        sprintf(szLan, "%s  %s\r\n", szLan, strTmp.GetBuffer(0));
        DWORD dwReturn = 0;
        WriteFile(hFile, szLan, strlen(szLan), &dwReturn, NULL);
    }

    if (hFile != INVALID_HANDLE_VALUE)
    {
        CloseHandle(hFile);
    }
}
