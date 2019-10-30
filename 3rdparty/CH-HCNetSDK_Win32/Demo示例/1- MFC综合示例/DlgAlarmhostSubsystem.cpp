// DlgAlarmhostSubsystem.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgAlarmhostSubsystem.h"
#include "DlgSubsysNetCfg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAlarmhostSubsystem dialog
const int g_struAutoArm[8] =
{
	IDC_DATETIMEPICKER_START1,
	IDC_DATETIMEPICKER_STOP1,
	IDC_DATETIMEPICKER_START2,
	IDC_DATETIMEPICKER_STOP2,
	IDC_DATETIMEPICKER_START3,
	IDC_DATETIMEPICKER_STOP3,
	IDC_DATETIMEPICKER_START4,
	IDC_DATETIMEPICKER_STOP4,
};

CDlgAlarmhostSubsystem::CDlgAlarmhostSubsystem(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAlarmhostSubsystem::IDD, pParent)
, m_iDeviceIndex(-1)
, m_lUserID(-1)
, m_iCurWeekdayIndex(0)
, m_chKeyZoneAlarmEnable(FALSE)
, m_chKeyZoneReportEnable(FALSE)
, m_chOneKeyAlarmEnable(FALSE)
, m_chSingleZoneArmEnable(FALSE)
, m_chMandatoryAlarm(FALSE)
{
	//{{AFX_DATA_INIT(CDlgAlarmhostSubsystem)
	m_wEnterDelay = 0;
	m_wExitDelay = 0;
    m_dwZoneIndex = 0;
	m_chHostageReport = FALSE;
	m_chPublicSubSystem = FALSE;
	m_chSubSystem = FALSE;
	m_chToneOfArm = FALSE;
	m_chToneOfSendReport = FALSE;
	m_timeStart1 = COleDateTime::GetCurrentTime();
	m_timeStart2 = COleDateTime::GetCurrentTime();
	m_timeStart3 = COleDateTime::GetCurrentTime();
	m_timeStart4 = COleDateTime::GetCurrentTime();
	m_timeStop1 = COleDateTime::GetCurrentTime();
	m_timeStop2 = COleDateTime::GetCurrentTime();
	m_timeStop3 = COleDateTime::GetCurrentTime();
	m_timeStop4 = COleDateTime::GetCurrentTime();
	m_timeAlarmIn = COleDateTime::GetCurrentTime();
	m_byAlarmInAdvance = 0;
	m_dwDelayTime = 0;
	m_chKeyZoneArm = FALSE;
	m_chKeyZoneDisarm = FALSE;
	m_chSendArmReport = FALSE;
	m_chSendDisarmReport = FALSE;
	m_bFri = FALSE;
	m_bMon = FALSE;
	m_bSta = FALSE;
	m_bSun = FALSE;
	m_bWen = FALSE;
	m_bEnableRemind = FALSE;
	m_bTus = FALSE;
	m_bThu = FALSE;
	m_bEnableAlarmInDelay = FALSE;
	m_iCurWeekdayIndex = 0;
	m_csSubSystemID = _T("");
    m_csOperatorCode = _T("");
	//}}AFX_DATA_INIT
	memset(&m_struAlarmHostAbility, 0, sizeof(m_struAlarmHostAbility));
	memset(&m_struSubSystem, 0, sizeof(m_struSubSystem));
	memset(&m_struSubSystemEx, 0, sizeof(m_struSubSystemEx));
}


void CDlgAlarmhostSubsystem::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAlarmhostSubsystem)
	DDX_Control(pDX, IDC_CMB_REMIND_ID, m_cmbRemindID);
	DDX_Control(pDX, IDC_COMBO_ARM_WEEK_COPY, m_cmArmWeekdayCopy);
	DDX_Control(pDX, IDC_COMBO_ARM_WEEK, m_cmArmWeekday);
	DDX_Control(pDX, IDC_LIST_SUPPORT_KEYBOARD_USER, m_listKeyBoardUser);
	DDX_Control(pDX, IDC_LIST_JOINT_SUBSYSTEM, m_listJointSubsystem);
	DDX_Control(pDX, IDC_LIST_BEJOINED_SUBSYSTEM, m_listBeJoinedSubSystem);
	DDX_Control(pDX, IDC_LIST_SUPPORT_KEYBOARD, m_listSupportKeyboard);
	DDX_Control(pDX, IDC_LIST_SUPPORT_KEYBOARD_ADDR, m_listSupportKeyboardAddr);
	DDX_Control(pDX, IDC_LIST_ASSOCIATE_ALARM_IN, m_listAssociateAlarmIn);
	DDX_Control(pDX, IDC_COMBO_SUBSYSTEMINDEX, m_cmSubSystem);
	DDX_Text(pDX, IDC_EDIT_ENTER_DELAY, m_wEnterDelay);
	DDX_Text(pDX, IDC_EDIT_EXITDELAY, m_wExitDelay);
	DDX_Check(pDX, IDC_CHECK_HOSTAGE_REPORT, m_chHostageReport);
	DDX_Check(pDX, IDC_CHECK_PUBLIC_SUBSYSTEM, m_chPublicSubSystem);
	DDX_Check(pDX, IDC_CHECK_SUBSYSTEM, m_chSubSystem);
	DDX_Check(pDX, IDC_CHECK_TONE_OF_ARM_AND_DISARM, m_chToneOfArm);
	DDX_Check(pDX, IDC_CHECK_TONE_OF_SEND_REPORT, m_chToneOfSendReport);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_START1, m_timeStart1);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_START2, m_timeStart2);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_START3, m_timeStart3);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_START4, m_timeStart4);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_STOP1, m_timeStop1);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_STOP2, m_timeStop2);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_STOP3, m_timeStop3);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_STOP4, m_timeStop4);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ALARMIN, m_timeAlarmIn);
	DDX_Text(pDX, IDC_EDIT_ALARM_IN_ADVANCE, m_byAlarmInAdvance);
	DDX_Text(pDX, IDC_EDIT_DELAY_TIME, m_dwDelayTime);
	DDV_MinMaxDWord(pDX, m_dwDelayTime, 0, 5999);
	DDX_Check(pDX, IDC_CHECK_KEYZOOM_ARM, m_chKeyZoneArm);
	DDX_Check(pDX, IDC_CHECK_KEYZOOM_DISARM, m_chKeyZoneDisarm);
	DDX_Check(pDX, IDC_CHECK_SEND_ARM_REPORT, m_chSendArmReport);
	DDX_Check(pDX, IDC_CHECK_SEND_DISARM_REPORT, m_chSendDisarmReport);
	DDX_Check(pDX, IDC_CHK_FRI, m_bFri);
	DDX_Check(pDX, IDC_CHK_MON, m_bMon);
	DDX_Check(pDX, IDC_CHK_STA, m_bSta);
	DDX_Check(pDX, IDC_CHK_SUN, m_bSun);
	DDX_Check(pDX, IDC_CHK_WEN, m_bWen);
	DDX_Check(pDX, IDC_CHK_ENABLE_REMIND, m_bEnableRemind);
	DDX_Check(pDX, IDC_CHK_TUS, m_bTus);
	DDX_Check(pDX, IDC_CHK_THU, m_bThu);
	DDX_Check(pDX, IDC_CHK_ENABLE_ALARMIN_DELAY, m_bEnableAlarmInDelay);
	DDX_Text(pDX, IDC_EDIT_SUBSYSTEM_ID, m_csSubSystemID);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_CHECK_KEYZONE_ARM_ENABLE, m_chKeyZoneAlarmEnable);
	DDX_Check(pDX, IDC_CHECK_KEYZONE_REPORT_ENABLE, m_chKeyZoneReportEnable);
	DDX_Check(pDX, IDC_CHECK_ONEKEY_SETUP_ARM_ENABLE, m_chOneKeyAlarmEnable);
	DDX_Check(pDX, IDC_CHECK_SINGLE_ZONE_ARM_ENABLE, m_chSingleZoneArmEnable);
	DDX_Check(pDX, IDC_MANDATORY_ALARM, m_chMandatoryAlarm);
    DDX_Text(pDX, IDC_EDIT_OPERATOR_CODE, m_csOperatorCode);
    DDX_Text(pDX, IDC_EDIT_ZONE_INDEX, m_dwZoneIndex);
}


BEGIN_MESSAGE_MAP(CDlgAlarmhostSubsystem, CDialog)
	//{{AFX_MSG_MAP(CDlgAlarmhostSubsystem)
	ON_BN_CLICKED(IDC_BTN_GET, OnBtnGet)
	ON_BN_CLICKED(IDC_BTN_SET, OnBtnSet)
	ON_BN_CLICKED(IDC_BTN_SUBSYSTEM_SETUP, OnBtnSubsystemSetup)
	ON_BN_CLICKED(IDC_BTN_STOPSETUP, OnBtnStopsetup)
	ON_BN_CLICKED(IDC_BTN_CLEARALARM, OnBtnClearalarm)
	ON_BN_CLICKED(IDC_BTN_BYPASS, OnBtnBypass)
	ON_BN_CLICKED(IDC_BTN_RESUME_BYPASS, OnBtnResumeBypass)
	ON_BN_CLICKED(IDC_CHECK_PUBLIC_SUBSYSTEM, OnCheckPublicSubsystem)
	ON_CBN_SELCHANGE(IDC_COMBO_ARM_WEEK, OnSelchangeComboArmWeek)
	ON_BN_CLICKED(IDC_BUTTON_COPY, OnButtonCopy)
	ON_BN_CLICKED(IDC_BTN_SUBSYSTEM_INMMEDIATE_ARM, OnBtnSubsystemInmmediateArm)
	ON_BN_CLICKED(IDC_BTN_SUBSYSTEM_STAY_ARM, OnBtnSubsystemStayArm)
	ON_CBN_SELCHANGE(IDC_CMB_REMIND_ID, OnSelchangeCmbRemindId)
	ON_BN_CLICKED(IDC_BTN_SAVE_REMIND, OnBtnSaveRemind)
    ON_BN_CLICKED(IDC_BTN_NETCARD_CFG, OnBnClickedBtnNetcardCfg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAlarmhostSubsystem message handlers

BOOL CDlgAlarmhostSubsystem::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
	m_lUserID = g_struDeviceInfo[m_iDeviceIndex].lLoginID;
	if (!NET_DVR_GetDeviceAbility(m_lUserID, ALARMHOST_ABILITY, NULL, 0, (char*)&m_struAlarmHostAbility, sizeof(m_struAlarmHostAbility)))
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_GetDeviceAbility FAILED");
	}
	else
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_GetDeviceAbility SUCC");
	}
	
	//�ṹ�������֧��8��ʱ��Σ�������ֻ����4��
// 	for (int i=0; i<4; i++)
// 	{
// 		GetDlgItem(IDC_DATETIMEPICKER_START1)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_DATETIMEPICKER_STOP1)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_DATETIMEPICKER_START2)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_DATETIMEPICKER_STOP2)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_DATETIMEPICKER_START3)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_DATETIMEPICKER_STOP3)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_DATETIMEPICKER_START4)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_DATETIMEPICKER_STOP4)->EnableWindow(FALSE);
// 	}
// 	m_cmArmWeekday.SetCurSel(0);
// 	m_cmArmWeekdayCopy.SetCurSel(0);
// 	m_cmArmWeekday.EnableWindow(FALSE);
// 	m_cmArmWeekdayCopy.EnableWindow(FALSE);
// 	GetDlgItem(IDC_BUTTON_COPY)->EnableWindow(FALSE);

// 	for (i=0; i<m_struAlarmHostAbility.byAutoArmTimeNumEveryDay*2; i++)
// 	{
// 		((CComboBox*)GetDlgItem(g_struAutoArm[i]))->EnableWindow(TRUE);
// 	}

	//��ʼ��list�ؼ�
	InitListCtrl();
	//��ʼ��ComboBox�ؼ�
	InitComboBox();

	//ʱ��
	m_timeStart1.SetTime(0, 0, 0);
	m_timeStop1.SetTime(0, 0, 0);
	m_timeStart2.SetTime(0, 0, 0);
	m_timeStop2.SetTime(0, 0, 0);
	m_timeStart3.SetTime(0, 0, 0);
	m_timeStop3.SetTime(0, 0, 0);
	m_timeStart4.SetTime(0, 0, 0);
	m_timeStop4.SetTime(0, 0, 0);
	m_timeAlarmIn.SetTime(0, 0, 0);

	//������ϵͳ����
	m_listBeJoinedSubSystem.EnableWindow(FALSE);

	memset(&m_struSubSystem, 0, sizeof(m_struSubSystem));
	memset(&m_struSubSystemEx, 0, sizeof(m_struSubSystemEx));
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAlarmhostSubsystem::InitListCtrl() 
{
	int i=0;
	char szLan[128] = {0};
	//������ϵͳ
	m_listJointSubsystem.SetExtendedStyle(LVS_EX_CHECKBOXES); 
	for (i=0; i<m_struAlarmHostAbility.wSubSystemNum; i++)
	{
		sprintf(szLan, "SubSystem %d", (i+1));
        m_listJointSubsystem.InsertItem(i, szLan);
	}
	//��������ϵͳ����
	m_listBeJoinedSubSystem.SetExtendedStyle(LVS_EX_CHECKBOXES); 
	for (i=0; i<m_struAlarmHostAbility.wSubSystemNum; i++)
	{
		sprintf(szLan, "SubSystem %d", (i+1));
        m_listBeJoinedSubSystem.InsertItem(i, szLan);
	}
	//������������
	m_listAssociateAlarmIn.SetExtendedStyle(LVS_EX_CHECKBOXES); 
	for (i=0; i<m_struAlarmHostAbility.wLocalAlarmInNum + m_struAlarmHostAbility.wExpandAlarmInNum; i++)
	{
		sprintf(szLan, "Alarm In %d", (i+1));
        m_listAssociateAlarmIn.InsertItem(i, szLan);
	}
	//��ϵͳ���������û�
	m_listKeyBoardUser.SetExtendedStyle(LVS_EX_CHECKBOXES); 
	for (i=0; i<m_struAlarmHostAbility.wOperatorUserNum; i++)
	{
		sprintf(szLan, "User %d", (i+1));
        m_listKeyBoardUser.InsertItem(i, szLan);
	}

	//��ϵͳ�������̺�
	m_listSupportKeyboard.SetExtendedStyle(LVS_EX_CHECKBOXES);
	if ( m_struAlarmHostAbility.wKeyboardAddrNum <= 0 )
	{
		for (i=0; i<m_struAlarmHostAbility.wKeyboardNum; i++)
		{
			sprintf(szLan, "Keyboard %d", (i+1));
			m_listSupportKeyboard.InsertItem(i, szLan);
		}
	}
	else
	{
		for (i=0; i<m_struAlarmHostAbility.wKeyboardAddrNum; i++)
		{
			sprintf(szLan, "Keyboard %d", (i+1));
			m_listSupportKeyboard.InsertItem(i, szLan);
	 	}
	}
	
// 	//��ϵͳ�������̵�ַ
// 	m_listSupportKeyboardAddr.SetExtendedStyle(LVS_EX_CHECKBOXES); 
// 	for (i=0; i<m_struAlarmHostAbility.wKeyboardAddrNum; i++)
// 	{
// 		sprintf(szLan, "Keyboard addr %d", (i+1));
//         m_listSupportKeyboardAddr.InsertItem(i, szLan);
// 	}
// 	if ( m_struAlarmHostAbility.wKeyboardAddrNum <= 0 )
// 	{
// 		GetDlgItem(IDC_LIST_SUPPORT_KEYBOARD_ADDR)->EnableWindow(FALSE);
// 	}
// 	else
// 	{
// 		GetDlgItem(IDC_LIST_SUPPORT_KEYBOARD_ADDR)->EnableWindow(TRUE);
// 	}
// 	if ( m_struAlarmHostAbility.wKeyboardNum <= 0 )
// 	{
// 		GetDlgItem(IDC_LIST_SUPPORT_KEYBOARD)->EnableWindow(FALSE);
// 	}
// 	else
// 	{
// 		GetDlgItem(IDC_LIST_SUPPORT_KEYBOARD)->EnableWindow(TRUE);
// 	}
}

void CDlgAlarmhostSubsystem::InitComboBox() 
{
	CString csStr;
	int i = 0;
	char szLan[128] = {0};
	//��ϵͳ��
	for (i=0; i<m_struAlarmHostAbility.wSubSystemNum; i++)
	{
		csStr.Format("%d", i+1);
		m_cmSubSystem.AddString(csStr);
	}
	g_StringLanType(szLan, "������ϵͳ", "All the subsystem");
	m_cmSubSystem.AddString(szLan);
	m_cmSubSystem.SetCurSel(0);
	//����
	g_StringLanType(szLan, "����һ", "Monday");
	m_cmArmWeekday.AddString(szLan);
	m_cmArmWeekdayCopy.AddString(szLan);
	g_StringLanType(szLan, "���ڶ�", "Tuesday");
	m_cmArmWeekday.AddString(szLan);
	m_cmArmWeekdayCopy.AddString(szLan);
	g_StringLanType(szLan, "������", "Wednesday");
	m_cmArmWeekday.AddString(szLan);
	m_cmArmWeekdayCopy.AddString(szLan);
	g_StringLanType(szLan, "������", "Thursday");
	m_cmArmWeekday.AddString(szLan);
	m_cmArmWeekdayCopy.AddString(szLan);
	g_StringLanType(szLan, "������", "Friday");
	m_cmArmWeekday.AddString(szLan);
	m_cmArmWeekdayCopy.AddString(szLan);
	g_StringLanType(szLan, "������", "Saturday");
	m_cmArmWeekday.AddString(szLan);
	m_cmArmWeekdayCopy.AddString(szLan);
	g_StringLanType(szLan, "������", "Sunday");
	m_cmArmWeekday.AddString(szLan);
	m_cmArmWeekdayCopy.AddString(szLan);
	g_StringLanType(szLan, "��������", "Whole Week");
	m_cmArmWeekdayCopy.AddString(szLan);
}

void CDlgAlarmhostSubsystem::OnBtnGet() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	DWORD dwSubSystemIndex = m_cmSubSystem.GetCurSel() + 1;
	DWORD dwReturn = -1;
	m_struSubSystem.dwSize = sizeof(m_struSubSystem);
	char szLan[128] = {0};
	if (!NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_ALARMHOSTSUBSYSTEM_CFG, dwSubSystemIndex, &m_struSubSystem, sizeof(m_struSubSystem), &dwReturn))
	{
		g_StringLanType(szLan, "��ȡ��ϵͳ����ʧ��", "Get subsystem failed!");
		//MessageBox(szLan);
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, szLan);
		return;
	}
	else
	{
		g_StringLanType(szLan, "��ȡ��ϵͳ���óɹ�", "Get subsystem Successful!");
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, szLan);
	}

	m_wEnterDelay = m_struSubSystem.wEnterDelay;
	m_wExitDelay = m_struSubSystem.wExitDelay;
	
	m_chHostageReport = m_struSubSystem.byHostageReport;
	//	m_chFaultAlarm = m_struSubSystem.byFaultAlarmEnable;
	
	m_chSubSystem = m_struSubSystem.bySubsystemEnable;
	m_chToneOfArm = m_struSubSystem.byKeyToneOfArmOrDisarm;
	m_chToneOfSendReport = m_struSubSystem.byKeyToneOfManualTestReport;
	m_dwDelayTime = m_struSubSystem.wDelayTime;
	m_chPublicSubSystem = m_struSubSystem.byPublicAttributeEnable;
	m_chKeyZoneArm = m_struSubSystem.byKeyZoneArm;
	m_chKeyZoneDisarm = m_struSubSystem.byKeyZoneDisarm;
	m_chSendArmReport = m_struSubSystem.byKeyZoneArmReport;
	m_chSendDisarmReport = m_struSubSystem.byKeyZoneDisarmReport;
	m_bEnableAlarmInDelay = m_struSubSystem.byEnableAlarmInDelay;
	m_chKeyZoneReportEnable = m_struSubSystem.byKeyZoneArmReportEnable;
	m_chKeyZoneAlarmEnable = m_struSubSystem.byKeyZoneArmEnable;
	m_chOneKeyAlarmEnable = m_struSubSystem.byOneKeySetupAlarmEnable;
	m_chSingleZoneArmEnable = m_struSubSystem.bySingleZoneSetupAlarmEnable;
	char szSubSystemID[MAX_SUBSYSTEM_ID_LEN+1] = {0};
	strncpy((char*)szSubSystemID, (char*)m_struSubSystem.bySubSystemID, MAX_SUBSYSTEM_ID_LEN);
	m_csSubSystemID = szSubSystemID;
	UpdateData(FALSE);
	OnCheckPublicSubsystem();


	if (!NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_ALARMHOST_SUBSYSTEM_CFG_EX, dwSubSystemIndex, &m_struSubSystemEx, sizeof(m_struSubSystemEx), &dwReturn))
	{
		g_StringLanType(szLan, "��ȡ��չ����ʧ��", "Get subsystem External Config Failed");
		//MessageBox(szLan);
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, szLan);
		return;
	}
	else
	{
		g_StringLanType(szLan, "��ȡ��չ���óɹ�", "Get subsystem External Config Successful!");
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, szLan);
	}

	//��ϵͳ��չ����
	int i = 0; 
	for (i=0; i<m_listAssociateAlarmIn.GetItemCount(); i++)
	{
		m_listAssociateAlarmIn.SetCheck(i, (BOOL)((m_struSubSystemEx.byJointAlarmIn[i/8]>>(i%8))&0x01));
	}

	for (i=0; i<m_listSupportKeyboard.GetItemCount(); i++)
	{
		m_listSupportKeyboard.SetCheck(i, (BOOL)((m_struSubSystemEx.byJointKeyboard[i/8]>>(i%8))&0x01));
	}

	for (i=0; i<m_listKeyBoardUser.GetItemCount(); i++)
	{
		m_listKeyBoardUser.SetCheck(i, (BOOL)((m_struSubSystemEx.byJointOpetaterUser[i/8]>>(i%8))&0x01));
	}
	m_byAlarmInAdvance = m_struSubSystemEx.byAlarmInAdvance;

	m_cmArmWeekday.SetCurSel(0);
	m_cmbRemindID.SetCurSel(0);
	UpdateData(FALSE);
	SetArmTimeToDlg();
	UpdateData(FALSE);
}

void CDlgAlarmhostSubsystem::OnBtnSet() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	char szLan[1024] = {0};
	DWORD dwSubsystemIndex = m_cmSubSystem.GetCurSel() + 1;
	m_struSubSystem.dwSize = sizeof(m_struSubSystem);
	m_struSubSystem.wEnterDelay = m_wEnterDelay;
	m_struSubSystem.wExitDelay = m_wExitDelay;
	m_struSubSystem.byHostageReport = m_chHostageReport;
//	m_struSubSystem.byFaultAlarmEnable = m_chFaultAlarm;

	m_struSubSystem.bySubsystemEnable = m_chSubSystem;
	m_struSubSystem.byKeyToneOfArmOrDisarm = m_chToneOfArm;
	m_struSubSystem.byKeyToneOfManualTestReport = m_chToneOfSendReport;
	m_struSubSystem.wDelayTime = m_dwDelayTime;
	m_struSubSystem.byPublicAttributeEnable = m_chPublicSubSystem;
	m_struSubSystem.byKeyZoneArm = m_chKeyZoneArm;
	m_struSubSystem.byKeyZoneArmReport = m_chSendArmReport;
	m_struSubSystem.byKeyZoneDisarmReport = m_chSendDisarmReport;
	m_struSubSystem.byKeyZoneDisarm = m_chKeyZoneDisarm;
	m_struSubSystem.byEnableAlarmInDelay = m_bEnableAlarmInDelay;
	strncpy((char*)m_struSubSystem.bySubSystemID, m_csSubSystemID.GetBuffer(0), MAX_SUBSYSTEM_ID_LEN);
	m_struSubSystem.byKeyZoneArmReportEnable = m_chKeyZoneAlarmEnable;
	m_struSubSystem.byKeyZoneArmEnable = m_chKeyZoneReportEnable;
	m_struSubSystem.byOneKeySetupAlarmEnable = m_chOneKeyAlarmEnable;
	m_struSubSystem.bySingleZoneSetupAlarmEnable = m_chSingleZoneArmEnable;
	//������ϵͳ
// 	if (m_chPublicSubSystem == 0)
// 	{
// 		for (int i=0; i<m_listBeJoinedSubSystem.GetItemCount(); i++)
// 		{
// 			m_struSubSystem.struJointSubSystem.struNormalSubSystem.dwBeJoinedSubSystem = (m_struSubSystem.struJointSubSystem.struNormalSubSystem.dwBeJoinedSubSystem<<1) | m_listJointSubsystem.GetCheck(i);
// 		}
// 	}
	if (m_chPublicSubSystem == 1)
	{
		m_struSubSystem.struJointSubSystem.struPublicSubSystem.dwJointSubSystem = 0;
		for (int i=0; i<m_listJointSubsystem.GetItemCount(); i++)
		{
			m_struSubSystem.struJointSubSystem.struPublicSubSystem.dwJointSubSystem |= (BOOL)(m_listJointSubsystem.GetCheck(i)<<i);
		}
	}
	if (!NET_DVR_SetDVRConfig(m_lUserID, NET_DVR_SET_ALARMHOSTSUBSYSTEM_CFG, dwSubsystemIndex, &m_struSubSystem, sizeof(m_struSubSystem)))
	{

		g_StringLanType(szLan, "������ϵͳ����ʧ��", "Set Subsystem Config Failed");
		MessageBox(szLan);
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, szLan);
	}
	else
	{
		g_StringLanType(szLan, "������ϵͳ���óɹ�", "Set Subsystem Config Successful");
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, szLan);
	}

	memset(&m_struSubSystemEx.byJointAlarmIn, 0, sizeof(m_struSubSystemEx.byJointAlarmIn));
	memset(&m_struSubSystemEx.byJointKeyboard, 0, sizeof(m_struSubSystemEx.byJointKeyboard));
	memset(&m_struSubSystemEx.byJointOpetaterUser, 0, sizeof(m_struSubSystemEx.byJointOpetaterUser));
	int i = 0; 
	for (i=0; i<m_listAssociateAlarmIn.GetItemCount(); i++)
	{
		m_struSubSystemEx.byJointAlarmIn[i/8] |= (BOOL)(m_listAssociateAlarmIn.GetCheck(i)<<(i%8));
	}
	
	for (i=0; i<m_listSupportKeyboard.GetItemCount(); i++)
	{
		m_struSubSystemEx.byJointKeyboard[i/8] |= (BOOL)(m_listSupportKeyboard.GetCheck(i)<<(i%8));
	}

	for (i=0; i<m_listKeyBoardUser.GetItemCount(); i++)
	{
		m_struSubSystemEx.byJointOpetaterUser[i/8] |= (BOOL)(m_listKeyBoardUser.GetCheck(i)<<(i%8));
	}
	OnSelchangeComboArmWeek();
	m_struSubSystemEx.byAlarmInAdvance = m_byAlarmInAdvance;

	m_struSubSystemEx.dwSize = sizeof(NET_DVR_SUBSYSTEM_PARAM_EX);
	if (!NET_DVR_SetDVRConfig(m_lUserID, NET_DVR_SET_ALARMHOST_SUBSYSTEM_CFG_EX, dwSubsystemIndex, &m_struSubSystemEx, sizeof(m_struSubSystemEx)))
	{
		
		g_StringLanType(szLan, "������ϵͳ��չ����ʧ��", "Set External Config Failed");
		MessageBox(szLan);
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, szLan);
	}
	else
	{
		g_StringLanType(szLan, "������ϵͳ��չ���óɹ�", "Set External Config successful");
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, szLan);
	}
}

void CDlgAlarmhostSubsystem::OnBtnSubsystemSetup() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	DWORD dwSubSystemNum = m_cmSubSystem.GetCurSel() + 1;
    if (m_cmSubSystem.GetCurSel() == m_struAlarmHostAbility.wSubSystemNum)
    {
        DWORD dwSystemID = atoi(m_csSubSystemID);
        if (dwSystemID == 0)
        {
            dwSubSystemNum = 0xffffffff;
        }
        else
        {
            dwSubSystemNum = dwSystemID;
        }
    }
	memset(&m_struCtrlParam, 0, sizeof(m_struCtrlParam));
	m_struCtrlParam.dwSize = sizeof(m_struCtrlParam);
	m_struCtrlParam.byIndex = dwSubSystemNum;
	m_struCtrlParam.dwControlParam = 1;
	m_struCtrlParam.byMandatoryAlarm = m_chMandatoryAlarm;
    m_struCtrlParam.wZoneIndex = m_dwZoneIndex;
    memcpy(m_struCtrlParam.byOperatorCode, m_csOperatorCode, 16);

	char szLan[128] = {0};
	if (!NET_DVR_RemoteControl(m_lUserID, NET_DVR_ARM_ALARMHOST_SUBSYSTEM, &m_struCtrlParam, sizeof(m_struCtrlParam)))
	{
		g_StringLanType(szLan, "�������ʧ��", "arm failed");
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, szLan);
		return;
	}
	else
	{
		g_StringLanType(szLan, "��������ɹ�", "arm successful");
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, szLan);
	}
	UpdateData(FALSE);
}

void CDlgAlarmhostSubsystem::OnBtnStopsetup() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	char szLan[1024] = {0};
	DWORD dwSubSystemNum = m_cmSubSystem.GetCurSel() + 1;
    if (m_cmSubSystem.GetCurSel() == m_struAlarmHostAbility.wSubSystemNum)
    {
        DWORD dwSystemID = atoi(m_csSubSystemID);
        if (dwSystemID == 0)
        {
            dwSubSystemNum = 0xffffffff;
        }
        else
        {
            dwSubSystemNum = dwSystemID;
        }
    }
    memset(&m_struCtrlParam, 0, sizeof(m_struCtrlParam));
    m_struCtrlParam.dwSize = sizeof(m_struCtrlParam);
    m_struCtrlParam.byIndex = dwSubSystemNum;
    m_struCtrlParam.wZoneIndex = m_dwZoneIndex;
    memcpy(m_struCtrlParam.byOperatorCode, m_csOperatorCode, 16);
    if (!NET_DVR_RemoteControl(m_lUserID, NET_DVR_ALARMHOST_CLOSE_SUBSYSTEM, &m_struCtrlParam, sizeof(m_struCtrlParam)))
	{
		g_StringLanType(szLan, "����ʧ��", "disarm failed");
		MessageBox(szLan);
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, szLan);
		return;
	}
	else
	{
		g_StringLanType(szLan, "�����ɹ�", "disarm successful");
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, szLan);
	}
}

void CDlgAlarmhostSubsystem::OnBtnClearalarm() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	char szLan[1024] = {0};
	DWORD dwSubSystemNum = m_cmSubSystem.GetCurSel() + 1;
    if (m_cmSubSystem.GetCurSel() == m_struAlarmHostAbility.wSubSystemNum)
    {
        DWORD dwSystemID = atoi(m_csSubSystemID);
        if (dwSystemID == 0)
        {
            dwSubSystemNum = 0xffffffff;
        }
        else
        {
            dwSubSystemNum = dwSystemID;
        }
    }
	if (!NET_DVR_AlarmHostClearAlarm(m_lUserID, dwSubSystemNum))
	{
		g_StringLanType(szLan, "����ʧ��", "Clear Alarm Failed");
		MessageBox(szLan);
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, szLan);
		return;
	}
	else
	{
		g_StringLanType(szLan, "�����ɹ�", "Clear Alarm successful");
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, szLan);
	}
}

void CDlgAlarmhostSubsystem::OnBtnBypass() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	char szLan[1024] = {0};
	DWORD dwSubSystemNum = m_cmSubSystem.GetCurSel() + 1;
    if (m_cmSubSystem.GetCurSel() == m_struAlarmHostAbility.wSubSystemNum)
    {
        DWORD dwSystemID = atoi(m_csSubSystemID);
        if (dwSystemID == 0)
        {
            dwSubSystemNum = 0xffffffff;
        }
        else
        {
            dwSubSystemNum = dwSystemID;
        }
    }
	memset(&m_struCtrlParam, 0, sizeof(m_struCtrlParam));
	m_struCtrlParam.dwSize = sizeof(m_struCtrlParam);
	m_struCtrlParam.byIndex = dwSubSystemNum;
    m_struCtrlParam.wZoneIndex = m_dwZoneIndex;
    memcpy(m_struCtrlParam.byOperatorCode, m_csOperatorCode, 16);
	if (!NET_DVR_RemoteControl(m_lUserID, NET_DVR_SET_SUBSYSTEM_BYPASS, &m_struCtrlParam, sizeof(m_struCtrlParam)))
	{
		g_StringLanType(szLan, "��ϵͳ��·ʧ��", "Subsystem Bypass Failed");
		MessageBox(szLan);
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, szLan);
		return;
	}
	else
	{
		g_StringLanType(szLan, "��ϵͳ��·�ɹ�", "Subsystem Bypass Successful");
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, szLan);
	}
}

void CDlgAlarmhostSubsystem::OnBtnResumeBypass() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	char szLan[1024] = {0};
	DWORD dwSubSystemNum = m_cmSubSystem.GetCurSel() + 1;
    if (m_cmSubSystem.GetCurSel() == m_struAlarmHostAbility.wSubSystemNum)
    {
        DWORD dwSystemID = atoi(m_csSubSystemID);
        if (dwSystemID == 0)
        {
            dwSubSystemNum = 0xffffffff;
        }
        else
        {
            dwSubSystemNum = dwSystemID;
        }
    }
	memset(&m_struCtrlParam, 0, sizeof(m_struCtrlParam));
	m_struCtrlParam.dwSize = sizeof(m_struCtrlParam);
	m_struCtrlParam.byIndex = dwSubSystemNum;
    m_struCtrlParam.wZoneIndex = m_dwZoneIndex;
    memcpy(m_struCtrlParam.byOperatorCode, m_csOperatorCode, 16);
	if (!NET_DVR_RemoteControl(m_lUserID, NET_DVR_CANCEL_SUBSYSTEM_BYPASS, &m_struCtrlParam, sizeof(m_struCtrlParam)))
	{
		g_StringLanType(szLan, "��ϵͳ��·�ָ�ʧ��", "Subsystem Resume Bypass Failed");
		MessageBox(szLan);
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, szLan);
		return;
	}
	else
	{
		g_StringLanType(szLan, "��ϵͳ��·�ָ��ɹ�", "Subsystem Resume Bypass Successful");
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, szLan);
	}
}


void CDlgAlarmhostSubsystem::OnCheckPublicSubsystem() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (1 == m_chPublicSubSystem)
	{
		m_listJointSubsystem.EnableWindow(TRUE);
		for (int i=0; i<m_listJointSubsystem.GetItemCount(); i++)
		{
			m_listBeJoinedSubSystem.SetCheck(i, FALSE);
			m_listJointSubsystem.SetCheck(i, (BOOL)((m_struSubSystem.struJointSubSystem.struPublicSubSystem.dwJointSubSystem>>i) & 0x01));
		}
	}
	else if (0 == m_chPublicSubSystem)
	{
		for (int i=0; i<m_listBeJoinedSubSystem.GetItemCount(); i++)
		{
			m_listJointSubsystem.SetCheck(i, FALSE);
			m_listBeJoinedSubSystem.SetCheck(i, (BOOL)((m_struSubSystem.struJointSubSystem.struNormalSubSystem.dwBeJoinedSubSystem>>i) & 0x01));
		} 
	}
	UpdateData(FALSE);
}

void CDlgAlarmhostSubsystem::SetArmTimeToDlg() 
{
	UpdateData(TRUE);
	int iIndex = m_cmArmWeekday.GetCurSel();
	m_timeStart1.SetTime(m_struSubSystemEx.struAlarmTime[iIndex][0].byStartHour, m_struSubSystemEx.struAlarmTime[iIndex][0].byStartMin, 0);
	m_timeStop1.SetTime(m_struSubSystemEx.struAlarmTime[iIndex][0].byStopHour, m_struSubSystemEx.struAlarmTime[iIndex][0].byStopMin, 0);
	m_timeStart2.SetTime(m_struSubSystemEx.struAlarmTime[iIndex][1].byStartHour, m_struSubSystemEx.struAlarmTime[iIndex][1].byStartMin, 0);
	m_timeStop2.SetTime(m_struSubSystemEx.struAlarmTime[iIndex][1].byStopHour, m_struSubSystemEx.struAlarmTime[iIndex][1].byStopMin, 0);
	m_timeStart3.SetTime(m_struSubSystemEx.struAlarmTime[iIndex][2].byStartHour, m_struSubSystemEx.struAlarmTime[iIndex][2].byStartMin, 0);
	m_timeStop3.SetTime(m_struSubSystemEx.struAlarmTime[iIndex][2].byStopHour, m_struSubSystemEx.struAlarmTime[iIndex][2].byStopMin, 0);
	m_timeStart4.SetTime(m_struSubSystemEx.struAlarmTime[iIndex][3].byStartHour, m_struSubSystemEx.struAlarmTime[iIndex][3].byStartMin, 0);
	m_timeStop4.SetTime(m_struSubSystemEx.struAlarmTime[iIndex][3].byStopHour, m_struSubSystemEx.struAlarmTime[iIndex][3].byStopMin, 0);

	OnSelchangeCmbRemindId();
	//m_timeAlarmIn.SetTime(m_struSubSystemEx.struAlarmRemindTime->, m_struSubSystemEx.byAlarmInMin, m_struSubSystemEx.byAlarmInSec);
	UpdateData(FALSE);
}

void CDlgAlarmhostSubsystem::OnSelchangeComboArmWeek() 
{
	// TODO: Add your control notification handler code here
// 	UpdateData(TRUE);
// 	int iIndex = m_cmArmWeekday.GetCurSel();
// 	m_struSubSystemEx.struAlarmTime[m_iCurWeekdayIndex][0].byStartHour = m_timeStart1.GetHour();
// 	m_struSubSystemEx.struAlarmTime[m_iCurWeekdayIndex][0].byStartMin = m_timeStart1.GetMinute();
// 	m_struSubSystemEx.struAlarmTime[m_iCurWeekdayIndex][0].byStopHour = m_timeStop1.GetHour();
// 	m_struSubSystemEx.struAlarmTime[m_iCurWeekdayIndex][0].byStopMin = m_timeStop1.GetMinute();
// 	m_struSubSystemEx.struAlarmTime[m_iCurWeekdayIndex][1].byStartHour = m_timeStart2.GetHour();
// 	m_struSubSystemEx.struAlarmTime[m_iCurWeekdayIndex][1].byStartMin = m_timeStart2.GetMinute();
// 	m_struSubSystemEx.struAlarmTime[m_iCurWeekdayIndex][1].byStopHour = m_timeStop2.GetHour();
// 	m_struSubSystemEx.struAlarmTime[m_iCurWeekdayIndex][1].byStopMin = m_timeStop2.GetMinute();
// 	m_struSubSystemEx.struAlarmTime[m_iCurWeekdayIndex][2].byStartHour = m_timeStart3.GetHour();
// 	m_struSubSystemEx.struAlarmTime[m_iCurWeekdayIndex][2].byStartMin = m_timeStart3.GetMinute();
// 	m_struSubSystemEx.struAlarmTime[m_iCurWeekdayIndex][2].byStopHour = m_timeStop3.GetHour();
// 	m_struSubSystemEx.struAlarmTime[m_iCurWeekdayIndex][2].byStopMin = m_timeStop3.GetMinute();
// 	m_struSubSystemEx.struAlarmTime[m_iCurWeekdayIndex][3].byStartHour = m_timeStart4.GetHour();
// 	m_struSubSystemEx.struAlarmTime[m_iCurWeekdayIndex][3].byStartMin = m_timeStart4.GetMinute();
// 	m_struSubSystemEx.struAlarmTime[m_iCurWeekdayIndex][3].byStopHour = m_timeStop4.GetHour();
// 	m_struSubSystemEx.struAlarmTime[m_iCurWeekdayIndex][3].byStopMin = m_timeStop4.GetMinute();
// 
// 	m_timeStart1.SetTime(m_struSubSystemEx.struAlarmTime[iIndex][0].byStartHour, m_struSubSystemEx.struAlarmTime[iIndex][0].byStartMin, 0);
// 	m_timeStop1.SetTime(m_struSubSystemEx.struAlarmTime[iIndex][0].byStopHour, m_struSubSystemEx.struAlarmTime[iIndex][0].byStopMin, 0);
// 	m_timeStart2.SetTime(m_struSubSystemEx.struAlarmTime[iIndex][1].byStartHour, m_struSubSystemEx.struAlarmTime[iIndex][1].byStartMin, 0);
// 	m_timeStop2.SetTime(m_struSubSystemEx.struAlarmTime[iIndex][1].byStopHour, m_struSubSystemEx.struAlarmTime[iIndex][1].byStopMin, 0);
// 	m_timeStart3.SetTime(m_struSubSystemEx.struAlarmTime[iIndex][2].byStartHour, m_struSubSystemEx.struAlarmTime[iIndex][2].byStartMin, 0);
// 	m_timeStop3.SetTime(m_struSubSystemEx.struAlarmTime[iIndex][2].byStopHour, m_struSubSystemEx.struAlarmTime[iIndex][2].byStopMin, 0);
// 	m_timeStart4.SetTime(m_struSubSystemEx.struAlarmTime[iIndex][3].byStartHour, m_struSubSystemEx.struAlarmTime[iIndex][3].byStartMin, 0);
// 	m_timeStop4.SetTime(m_struSubSystemEx.struAlarmTime[iIndex][3].byStopHour, m_struSubSystemEx.struAlarmTime[iIndex][3].byStopMin, 0);
// 	m_iCurWeekdayIndex = iIndex;
// 	UpdateData(FALSE);
}

void CDlgAlarmhostSubsystem::OnButtonCopy() 
{
	// TODO: Add your control notification handler code here
// 	OnSelchangeComboArmWeek();
// 	int iIndex = m_cmArmWeekday.GetCurSel();
// 	int iIndexCopy = m_cmArmWeekdayCopy.GetCurSel();
// 	//���Ƶ���������
// 	if (iIndexCopy==7)
// 	{
// 		for (int i=0; i<7; i++)
// 		{
// 			memcpy((char*)&m_struSubSystemEx.struAlarmTime[i], (char*)&m_struSubSystemEx.struAlarmTime[iIndex], sizeof(m_struSubSystemEx.struAlarmTime[0]));
// 		}
// 	}
// 	memcpy((char*)&m_struSubSystemEx.struAlarmTime[iIndexCopy], (char*)&m_struSubSystemEx.struAlarmTime[iIndex], sizeof(m_struSubSystemEx.struAlarmTime[0]));
}

void CDlgAlarmhostSubsystem::OnBtnSubsystemInmmediateArm() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	DWORD dwSubSystemNum = m_cmSubSystem.GetCurSel() + 1;
	if (m_cmSubSystem.GetCurSel() == m_struAlarmHostAbility.wSubSystemNum)
	{
        DWORD dwSystemID = atoi(m_csSubSystemID);
        if (dwSystemID == 0)
        {
            dwSubSystemNum = 0xff;
        }
        else
        {
            dwSubSystemNum = dwSystemID;
        }
	}
	memset(&m_struCtrlParam, 0, sizeof(m_struCtrlParam));
	m_struCtrlParam.dwSize = sizeof(m_struCtrlParam);
	m_struCtrlParam.byIndex = dwSubSystemNum;
	m_struCtrlParam.dwControlParam = 2;
	m_struCtrlParam.byMandatoryAlarm = m_chMandatoryAlarm;
    m_struCtrlParam.wZoneIndex = m_dwZoneIndex;
    memcpy(m_struCtrlParam.byOperatorCode, m_csOperatorCode, 16);
	char szLan[128] = {0};
	if (!NET_DVR_RemoteControl(m_lUserID, NET_DVR_ARM_ALARMHOST_SUBSYSTEM, &m_struCtrlParam, sizeof(m_struCtrlParam)))
	{
		g_StringLanType(szLan, "��ʱ����ʧ��", "immediate arm failed");
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, szLan);
		return;
	}
	else
	{
		g_StringLanType(szLan, "��ʱ�����ɹ�", "immediate arm successful");
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, szLan);
	}
	UpdateData(FALSE);
}

void CDlgAlarmhostSubsystem::OnBtnSubsystemStayArm() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	DWORD dwSubSystemNum = m_cmSubSystem.GetCurSel() + 1;
    if (m_cmSubSystem.GetCurSel() == m_struAlarmHostAbility.wSubSystemNum)
    {
        DWORD dwSystemID = atoi(m_csSubSystemID);
        if (dwSystemID == 0)
        {
            dwSubSystemNum = 0xff;
        }
        else
        {
            dwSubSystemNum = dwSystemID;
        }
    }
	memset(&m_struCtrlParam, 0, sizeof(m_struCtrlParam));
	m_struCtrlParam.dwSize = sizeof(m_struCtrlParam);
	m_struCtrlParam.byIndex = dwSubSystemNum;
	m_struCtrlParam.dwControlParam = 3;
	m_struCtrlParam.byMandatoryAlarm = m_chMandatoryAlarm;
    m_struCtrlParam.wZoneIndex = m_dwZoneIndex;
    memcpy(m_struCtrlParam.byOperatorCode, m_csOperatorCode, 16);

	char szLan[128] = {0};
	if (!NET_DVR_RemoteControl(m_lUserID, NET_DVR_ARM_ALARMHOST_SUBSYSTEM, &m_struCtrlParam, sizeof(m_struCtrlParam)))
	{
		g_StringLanType(szLan, "���ز���ʧ��", "stay arm failed");
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, szLan);
		return;
	}
	else
	{
		g_StringLanType(szLan, "���ز����ɹ�", "stay arm successful");
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, szLan);
	}
	UpdateData(FALSE);
}

void CDlgAlarmhostSubsystem::OnSelchangeCmbRemindId() 
{
	// TODO: Add your control notification handler code here
	int iRemindID = m_cmbRemindID.GetCurSel();

	//��������Э����֧��7*8����ʱ��demo�ϰ���֧��8�����ѣ�ÿ������һ��ʱ�䣬��Ӧһ���������Ƿ�ѡ��

	m_bMon = m_struSubSystemEx.struAlarmRemindTime[0][iRemindID].byEnable;
	m_bTus = m_struSubSystemEx.struAlarmRemindTime[1][iRemindID].byEnable;
	m_bWen = m_struSubSystemEx.struAlarmRemindTime[2][iRemindID].byEnable;
	m_bThu = m_struSubSystemEx.struAlarmRemindTime[3][iRemindID].byEnable;
	m_bFri = m_struSubSystemEx.struAlarmRemindTime[4][iRemindID].byEnable;
	m_bSta = m_struSubSystemEx.struAlarmRemindTime[5][iRemindID].byEnable;
	m_bSun = m_struSubSystemEx.struAlarmRemindTime[6][iRemindID].byEnable;

	int i = -1;
	BOOL bEnbale = FALSE;
	for (i = 0; i < MAX_DAYS; i++)
	{
		if (m_struSubSystemEx.struAlarmRemindTime[i][iRemindID].byEnable)
		{
			m_timeAlarmIn.SetTime(m_struSubSystemEx.struAlarmRemindTime[i][iRemindID].byHour, m_struSubSystemEx.struAlarmRemindTime[i][iRemindID].byMinute, m_struSubSystemEx.struAlarmRemindTime[i][iRemindID].bySecond);
			bEnbale = TRUE;
		}	
	}
	if (!bEnbale)
	{
		m_timeAlarmIn.SetTime(0, 0, 0);
	}
	UpdateData(FALSE);
}

void CDlgAlarmhostSubsystem::OnBtnSaveRemind() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int iRemindID = m_cmbRemindID.GetCurSel();
	int i = -1;

	m_struSubSystemEx.struAlarmRemindTime[0][iRemindID].byEnable = m_bMon;
	m_struSubSystemEx.struAlarmRemindTime[1][iRemindID].byEnable = m_bTus;
	m_struSubSystemEx.struAlarmRemindTime[2][iRemindID].byEnable = m_bWen;
	m_struSubSystemEx.struAlarmRemindTime[3][iRemindID].byEnable = m_bThu;
	m_struSubSystemEx.struAlarmRemindTime[4][iRemindID].byEnable = m_bFri;
	m_struSubSystemEx.struAlarmRemindTime[5][iRemindID].byEnable = m_bSta;
	m_struSubSystemEx.struAlarmRemindTime[6][iRemindID].byEnable = m_bSun;

	for (i = 0; i < MAX_DAYS; i++)
	{
		if (m_struSubSystemEx.struAlarmRemindTime[i][iRemindID].byEnable)
		{
			m_struSubSystemEx.struAlarmRemindTime[i][iRemindID].byHour = m_timeAlarmIn.GetHour();
			m_struSubSystemEx.struAlarmRemindTime[i][iRemindID].byMinute = m_timeAlarmIn.GetMinute();
			m_struSubSystemEx.struAlarmRemindTime[i][iRemindID].bySecond = m_timeAlarmIn.GetSecond();
		}	
	}	
	UpdateData(FALSE);
}

void CDlgAlarmhostSubsystem::OnBnClickedBtnNetcardCfg()
{
    // TODO: Add your control notification handler code here
    CDlgSubsysNetCfg dlg;
    int iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
    if (iDeviceIndex == -1)
    {
        return;
    }
    dlg.m_lUserID = g_struDeviceInfo[iDeviceIndex].lLoginID;
    dlg.m_iDeviceIndex = iDeviceIndex;
    dlg.DoModal();
}
