// DlgAcsWorkStatus.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgAcsWorkStatus.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAcsWorkStatus dialog


CDlgAcsWorkStatus::CDlgAcsWorkStatus(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAcsWorkStatus::IDD, pParent)
    , m_dwWhiteFaceNum(0)
    , m_dwBlackFaceNum(0)
{
	//{{AFX_DATA_INIT(CDlgAcsWorkStatus)
	m_bAntiSneak = FALSE;
	m_bHostAntiDismantle = FALSE;
	m_bLowBattery = FALSE;
	m_bMultiDoorInterlock = FALSE;
	m_fBatteryVoltage = 0.0f;
	m_dwCardNum = 0;
	//}}AFX_DATA_INIT
}


void CDlgAcsWorkStatus::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgAcsWorkStatus)
    DDX_Control(pDX, IDC_COMBO_INLIGHTSTATUS, m_comboIndicatorLightStatus);
    DDX_Control(pDX, IDC_LIST_ALARM_ARMSTATUS, m_lstArmStatus);
    DDX_Control(pDX, IDC_LIST_DOOR, m_listDoor);
    DDX_Control(pDX, IDC_LIST_CRAD_READER, m_listCardReader);
    DDX_Control(pDX, IDC_LIST_CASE_SENSOR, m_listCaseSensor);
    DDX_Control(pDX, IDC_LIST_ALARM_OUT, m_listAlarmOut);
    DDX_Control(pDX, IDC_LIST_ALARM_IN, m_listAlarmIn);
    DDX_Control(pDX, IDC_COMBO_POWER_SUPPLY_MODE, m_comboPowerSupplyMode);
    DDX_Check(pDX, IDC_CHECK_ANTI_SNEAK, m_bAntiSneak);
    DDX_Check(pDX, IDC_CHECK_HOST_ANTI_DISMANTLE, m_bHostAntiDismantle);
    DDX_Check(pDX, IDC_CHECK_LOW_BATTERY, m_bLowBattery);
    DDX_Check(pDX, IDC_CHECK_MULTIDOOR_INTER_LOCK, m_bMultiDoorInterlock);
    DDX_Text(pDX, IDC_EDIT_BATTERY_VOLTAGE, m_fBatteryVoltage);
    DDX_Text(pDX, IDC_EDIT_CARD_NUM, m_dwCardNum);
    //}}AFX_DATA_MAP
    DDX_Control(pDX, IDC_COMBO_FIRE_ALARM_STATUS, m_cmbFireAlarmStatus);
    DDX_Control(pDX, IDC_CMB_BATTERY_CHARGE_STATUS, m_cmbBatteryChargeStatus);
    DDX_Control(pDX, IDC_CMB_ANTI_SNEAK_SERVER_STATUS, m_cmbAntiSneakServerStatus);
    DDX_Control(pDX, IDC_CMB_MASTER_CHANNEL_CONTROLLER_STATUS, m_cmbMasterChannelControllerStatus);
    DDX_Control(pDX, IDC_CMB_SLAVE_CHANNEL_CONTROLLER_STATUS, m_cmbSlaveChannelControllerStatus);
    DDX_Text(pDX, IDC_EDIT_WFACE_NUM, m_dwWhiteFaceNum);
    DDX_Text(pDX, IDC_EDIT_BFACE_NUM, m_dwBlackFaceNum);
}


BEGIN_MESSAGE_MAP(CDlgAcsWorkStatus, CDialog)
	//{{AFX_MSG_MAP(CDlgAcsWorkStatus)
	ON_BN_CLICKED(IDC_BTN_UPDATE, OnBtnUpdate)
	ON_BN_CLICKED(IDC_BTN_EXIT, OnDlgExit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAcsWorkStatus message handlers

BOOL CDlgAcsWorkStatus::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	memset(&m_struAcsWorkStatus, 0, sizeof(m_struAcsWorkStatus));

    char szLan[64] = {0};
    m_listDoor.SetExtendedStyle(m_listDoor.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
    m_listCardReader.SetExtendedStyle(m_listCardReader.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
    m_listCaseSensor.SetExtendedStyle(m_listCaseSensor.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
    m_listAlarmIn.SetExtendedStyle(m_listAlarmIn.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
    m_listAlarmOut.SetExtendedStyle(m_listAlarmOut.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
    m_lstArmStatus.SetExtendedStyle(m_lstArmStatus.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

    g_StringLanType(szLan, "�ű��", "Door No");
    m_listDoor.InsertColumn(0, szLan, LVCFMT_LEFT, 60, -1);
    g_StringLanType(szLan, "����״̬(�̵�������״̬)", "Door lock status");
    m_listDoor.InsertColumn(1, szLan, LVCFMT_LEFT, 80, -1);
    g_StringLanType(szLan, "�Ŵ�״̬", "Magnetic Status");
    m_listDoor.InsertColumn(2, szLan, LVCFMT_LEFT, 80, -1);
    g_StringLanType(szLan, "��״̬(¥��״̬)", "Door status"); 
    m_listDoor.InsertColumn(3, szLan, LVCFMT_LEFT, 80, -1);

    g_StringLanType(szLan, "���������", "Card reader No");
    m_listCardReader.InsertColumn(0, szLan, LVCFMT_LEFT, 80, -1);
    g_StringLanType(szLan, "����״̬", "Online status");
    m_listCardReader.InsertColumn(1, szLan, LVCFMT_LEFT, 80, -1);
    g_StringLanType(szLan, "����״̬", "Anti Dismantle");
    m_listCardReader.InsertColumn(2, szLan, LVCFMT_LEFT, 80, -1);
    g_StringLanType(szLan, "��֤��ʽ", "Verify Mode");
    m_listCardReader.InsertColumn(3, szLan, LVCFMT_LEFT, 80, -1);

    g_StringLanType(szLan, "�¼����������", "Case sensor No");
    m_listCaseSensor.InsertColumn(0, szLan, LVCFMT_LEFT, 120, -1);
    g_StringLanType(szLan, "״̬", "Status");
    m_listCaseSensor.InsertColumn(1, szLan, LVCFMT_LEFT, 50, -1);

    g_StringLanType(szLan, "���", "Alarmin Arm status");
    m_lstArmStatus.InsertColumn(0, szLan, LVCFMT_LEFT, 50, -1);
    g_StringLanType(szLan, "����״̬", "Status");
    m_lstArmStatus.InsertColumn(1, szLan, LVCFMT_LEFT, 80, -1);

    g_StringLanType(szLan, "���������", "Alarm in No");
    m_listAlarmIn.InsertColumn(0, szLan, LVCFMT_LEFT, 100, -1);
    g_StringLanType(szLan, "״̬", "Status");
    m_listAlarmIn.InsertColumn(1, szLan, LVCFMT_LEFT, 60, -1);

    g_StringLanType(szLan, "���������", "Alarm out No");
    m_listAlarmOut.InsertColumn(0, szLan, LVCFMT_LEFT, 100, -1);
    g_StringLanType(szLan, "״̬", "Status");
    m_listAlarmOut.InsertColumn(1, szLan, LVCFMT_LEFT, 60, -1);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAcsWorkStatus::OnBtnUpdate() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
    DWORD dwReturned = 0;
    char szLan[64] = {0};
    if (!NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_ACS_WORK_STATUS_V50, 0, &m_struAcsWorkStatus, sizeof(m_struAcsWorkStatus), &dwReturned))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_GET_ACS_WORK_STATUS_V50");
        g_StringLanType(szLan, "��ȡ�Ž���������״̬ʧ��", "Get Acs work status fail");
        AfxMessageBox(szLan);
    }
    else
    {
        UpdateControlData();
    }
}

void CDlgAcsWorkStatus::UpdateControlData()
{
    int i = 0;
    char szLan[64] = {0};

    //���б�
    m_listDoor.DeleteAllItems();
    for (i = 0; i < MAX_DOOR_NUM_256; i++)
    {
        sprintf(szLan, "%d", i + 1);
        m_listDoor.InsertItem(i, szLan);

        if (m_struAcsWorkStatus.byDoorLockStatus[i] == 0)
        {
            g_StringLanType(szLan, "��", "Close");
        }
        else if (m_struAcsWorkStatus.byDoorLockStatus[i] == 1)
        {
            g_StringLanType(szLan, "��", "Open");
        }
        else if (m_struAcsWorkStatus.byDoorLockStatus[i] == 2)
        {
            g_StringLanType(szLan, "��·����", "Damage short-circuit alarm");
        }
        else if (m_struAcsWorkStatus.byDoorLockStatus[i] == 3)
        {
            g_StringLanType(szLan, "��·����", "Damage breaking alarm");
        }
        else if (m_struAcsWorkStatus.byDoorLockStatus[i] == 4)
        {
            g_StringLanType(szLan, "�쳣����", "Abnormal alarm ");
        }

        m_listDoor.SetItemText(i, 1, szLan);

        if (m_struAcsWorkStatus.byMagneticStatus[i] == 0)
        {
            g_StringLanType(szLan, "��", "Close");
        }
        else if (m_struAcsWorkStatus.byMagneticStatus[i] == 1)
        {
            g_StringLanType(szLan, "��", "Open");
        }
        else if (m_struAcsWorkStatus.byMagneticStatus[i] == 2)
        {
            g_StringLanType(szLan, "��·����", "Damage short-circuit alarm");
        }
        else if (m_struAcsWorkStatus.byMagneticStatus[i] == 3)
        {
            g_StringLanType(szLan, "��·����", "Damage breaking alarm");
        }
        else if (m_struAcsWorkStatus.byMagneticStatus[i] == 4)
        {
            g_StringLanType(szLan, "�쳣����", "Abnormal alarm ");
        }

        m_listDoor.SetItemText(i, 2, szLan);

        if (m_struAcsWorkStatus.byDoorStatus[i] == 1)
        {
            g_StringLanType(szLan, "����", "dormant");
        }
        else if (m_struAcsWorkStatus.byDoorStatus[i] == 2)
        {
            g_StringLanType(szLan, "����(����)", "Always Open");
        }
        else if (m_struAcsWorkStatus.byDoorStatus[i] == 3)
        {
            g_StringLanType(szLan, "����(����)", "Always Close");
        }
        else if (m_struAcsWorkStatus.byDoorStatus[i] == 4)
        {
            g_StringLanType(szLan, "��ͨ(�ܿ�)", "Common");
        }
        m_listDoor.SetItemText(i, 3, szLan);
    }

    //������״̬
    m_listCardReader.DeleteAllItems();
    for (i = 0; i < MAX_CARD_READER_NUM_512; i++)
    {
        sprintf(szLan, "%d", i + 1);
        m_listCardReader.InsertItem(i, szLan);

        if (m_struAcsWorkStatus.byCardReaderOnlineStatus[i] == 0)
        {
            g_StringLanType(szLan, "����", "Offline");
        }
        else if (m_struAcsWorkStatus.byCardReaderOnlineStatus[i] == 1)
        {
            g_StringLanType(szLan, "����", "Online");
        }

        m_listCardReader.SetItemText(i, 1, szLan);

        if (m_struAcsWorkStatus.byCardReaderAntiDismantleStatus[i] == 0)
        {
            g_StringLanType(szLan, "�ر�", "Disabled");
        }
        else if (m_struAcsWorkStatus.byCardReaderAntiDismantleStatus[i] == 1)
        {
            g_StringLanType(szLan, "����", "Enabled");
        }

        m_listCardReader.SetItemText(i, 2, szLan);

        if (m_struAcsWorkStatus.byCardReaderVerifyMode[i] == 1)
        {
            g_StringLanType(szLan, "����", "Dormant");
        }
        else if (m_struAcsWorkStatus.byCardReaderVerifyMode[i] == 2)
        {
            g_StringLanType(szLan, "ˢ��+����", "Card & password");
        }
        else if (m_struAcsWorkStatus.byCardReaderVerifyMode[i] == 3)
        {
            g_StringLanType(szLan, "ˢ��", "Card");
        }
        else if (m_struAcsWorkStatus.byCardReaderVerifyMode[i] == 4)
        {
            g_StringLanType(szLan, "ˢ��������", "Card or password");
        }
        else if (m_struAcsWorkStatus.byCardReaderVerifyMode[i] == 5)
        {
            g_StringLanType(szLan, "ָ��", "Fingerprint");
        }
        else if (m_struAcsWorkStatus.byCardReaderVerifyMode[i] == 6)
        {
            g_StringLanType(szLan, "ָ��+����", "Fingerprint and password");
        }
        else if (m_struAcsWorkStatus.byCardReaderVerifyMode[i] == 7)
        {
            g_StringLanType(szLan, "ָ�ƻ�ˢ��", "Fingerprint or Card");
        }
        else if (m_struAcsWorkStatus.byCardReaderVerifyMode[i] == 8)
        {
            g_StringLanType(szLan, "ָ��+ˢ��", "Fingerprint and Card");
        }
        else if (m_struAcsWorkStatus.byCardReaderVerifyMode[i] == 9)
        {
            g_StringLanType(szLan, "ָ��+ˢ��+����", "Fingerprint and Card and password");
        }
        else if (m_struAcsWorkStatus.byCardReaderVerifyMode[i] == 10)
        {
            g_StringLanType(szLan, "������ָ�ƻ�ˢ��������", "Face or fingerprint or card or password");
        }
        else if (m_struAcsWorkStatus.byCardReaderVerifyMode[i] == 11)
        {
            g_StringLanType(szLan, "����+ָ��", "Face and fingerprint");
        }
        else if (m_struAcsWorkStatus.byCardReaderVerifyMode[i] == 12)
        {
            g_StringLanType(szLan, "����+����", "Face and password");
        }
        else if (m_struAcsWorkStatus.byCardReaderVerifyMode[i] == 13)
        {
            g_StringLanType(szLan, "����+ˢ��", "Face and card");
        }
        else if (m_struAcsWorkStatus.byCardReaderVerifyMode[i] == 14)
        {
            g_StringLanType(szLan, "����", "Face");
        }
        else if (m_struAcsWorkStatus.byCardReaderVerifyMode[i] == 15)
        {
            g_StringLanType(szLan, "����+����", "Employee no and password");
        }
        else if (m_struAcsWorkStatus.byCardReaderVerifyMode[i] == 16)
        {
            g_StringLanType(szLan, "ָ�ƻ�����", "Fingerprint or password");
        }
        else if (m_struAcsWorkStatus.byCardReaderVerifyMode[i] == 17)
        {
            g_StringLanType(szLan, "����+ָ��", "Employee no and fingerprint");
        }
        else if (m_struAcsWorkStatus.byCardReaderVerifyMode[i] == 18)
        {
            g_StringLanType(szLan, "����+ָ��+����", "Employee no and fingerprint and password");
        }
        else if (m_struAcsWorkStatus.byCardReaderVerifyMode[i] == 19)
        {
            g_StringLanType(szLan, "����+ָ��+ˢ��", "Face and fingerprint and card");
        }
        else if (m_struAcsWorkStatus.byCardReaderVerifyMode[i] == 20)
        {
            g_StringLanType(szLan, "����+����+ָ��", "Face and password and fingerprint");
        }
        else if (m_struAcsWorkStatus.byCardReaderVerifyMode[i] == 21)
        {
            g_StringLanType(szLan, "����+����", "Employee no and face");
        }
        else if (m_struAcsWorkStatus.byCardReaderVerifyMode[i] == 22)
        {
            g_StringLanType(szLan, "����������+ˢ��", "Face or face and card");
        }
        else if (m_struAcsWorkStatus.byCardReaderVerifyMode[i] == 23)
        {
            g_StringLanType(szLan, "ָ�ƻ�����", "fingerprint or face");
        }
        else if (m_struAcsWorkStatus.byCardReaderVerifyMode[i] == 24)
        {
            g_StringLanType(szLan, "ˢ��������������", "card or face or password");
        }

        m_listCardReader.SetItemText(i, 3, szLan);
    }

    //�¼�������״̬
    m_listCaseSensor.DeleteAllItems();
    for (i = 0; i < MAX_CASE_SENSOR_NUM; i++)
    {
        sprintf(szLan, "%d", i + 1);
        m_listCaseSensor.InsertItem(i, szLan);

        if (m_struAcsWorkStatus.byCaseStatus[i] == 0)
        {
            g_StringLanType(szLan, "������", "No input");
        }
        else// if (m_struAcsWorkStatus.byCaseStatus[i] == 1)
        {
            g_StringLanType(szLan, "������", "Input");
        }

        m_listCaseSensor.SetItemText(i, 1, szLan);
    }

    //��������״̬
    m_lstArmStatus.DeleteAllItems(); 
    m_listAlarmIn.DeleteAllItems();
    for (i = 0; i < MAX_ALARMHOST_ALARMIN_NUM; i++)
    {
        sprintf(szLan, "%d", i + 1);
        m_listAlarmIn.InsertItem(i, szLan);
        m_lstArmStatus.InsertItem(i, szLan); 
        if (m_struAcsWorkStatus.byAlarmInStatus[i] == 0)
        {
            g_StringLanType(szLan, "������", "No input");
        }
        else //if (m_struAcsWorkStatus.byAlarmOutStatus[i] == 1)
        {
            g_StringLanType(szLan, "������", "Input");
        }

        m_listAlarmIn.SetItemText(i, 1, szLan);

        if (m_struAcsWorkStatus.bySetupAlarmStatus[i] == 0)
        {
            g_StringLanType(szLan, "����", "Disarm");
        }
        else
        {
            g_StringLanType(szLan, "����", "Arm");
        }
        
        m_lstArmStatus.SetItemText(i, 1, szLan);
    }

    //�������״̬
    m_listAlarmOut.DeleteAllItems();
    for (i = 0; i < MAX_ALARMHOST_ALARMOUT_NUM; i++)
    {
        sprintf(szLan, "%d", i + 1);
        m_listAlarmOut.InsertItem(i, szLan);

        if (m_struAcsWorkStatus.byAlarmOutStatus[i] == 0)
        {
            g_StringLanType(szLan, "�����", "No output");
        }
        else// if (m_struAcsWorkStatus.byAlarmOutStatus[i] == 1)
        {
            g_StringLanType(szLan, "�����", "Output");
        }

        m_listAlarmOut.SetItemText(i, 1, szLan);
    }

    m_comboPowerSupplyMode.SetCurSel(m_struAcsWorkStatus.byPowerSupplyStatus - 1);
    m_fBatteryVoltage = (float)m_struAcsWorkStatus.wBatteryVoltage/10;
    m_bLowBattery = m_struAcsWorkStatus.byBatteryLowVoltage;
    m_dwCardNum = m_struAcsWorkStatus.dwCardNum;
    m_bMultiDoorInterlock = m_struAcsWorkStatus.byMultiDoorInterlockStatus;
    m_bAntiSneak = m_struAcsWorkStatus.byAntiSneakStatus;
    m_bHostAntiDismantle = m_struAcsWorkStatus.byHostAntiDismantleStatus;
    
    m_comboIndicatorLightStatus.SetCurSel(m_struAcsWorkStatus.byIndicatorLightStatus);
    m_cmbFireAlarmStatus.SetCurSel(m_struAcsWorkStatus.byFireAlarmStatus);
    m_cmbBatteryChargeStatus.SetCurSel(m_struAcsWorkStatus.byBatteryChargeStatus);
    m_cmbAntiSneakServerStatus.SetCurSel(m_struAcsWorkStatus.byAntiSneakServerStatus);
    m_cmbMasterChannelControllerStatus.SetCurSel(m_struAcsWorkStatus.byMasterChannelControllerStatus);
    m_cmbSlaveChannelControllerStatus.SetCurSel(m_struAcsWorkStatus.bySlaveChannelControllerStatus);

    m_dwBlackFaceNum = m_struAcsWorkStatus.dwBlackFaceNum;
    m_dwWhiteFaceNum = m_struAcsWorkStatus.dwWhiteFaceNum;
    UpdateData(FALSE);
}

void CDlgAcsWorkStatus::OnDlgExit() 
{
	// TODO: Add your control notification handler code here
    CDialog::OnCancel();
}

