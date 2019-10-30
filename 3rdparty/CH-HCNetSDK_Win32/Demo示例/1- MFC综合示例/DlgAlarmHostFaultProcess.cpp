//lgAlarmHostFaultProcess.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgAlarmHostFaultProcess.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAlarmHostFaultProcess dialog


CDlgAlarmHostFaultProcess::CDlgAlarmHostFaultProcess(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAlarmHostFaultProcess::IDD, pParent)
	,m_iCurSubSystemIndex(1)
	,m_iDeviceIndex(-1)
	,m_lUserID(-1)
{
	//{{AFX_DATA_INIT(CDlgAlarmHostFaultProcess)
	//}}AFX_DATA_INIT
}


void CDlgAlarmHostFaultProcess::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAlarmHostFaultProcess)
	DDX_Control(pDX, IDC_LIST_FAULT_LED_LINKAGE, m_listFaultLedLinkage);
	DDX_Control(pDX, IDC_LIST_OVERALL_FAULT_JOINT_SOUND, m_listOverallFaultJointSound);
	DDX_Control(pDX, IDC_LIST_OVERALL_FAULT_JOINT_LED, m_listOverallFaultJointLED);
	DDX_Control(pDX, IDC_COMBO_SUBSYSTEM, m_cmSubsystem);
	DDX_Control(pDX, IDC_LIST_SUBSYSTEM_FAULT_JOINT_SOUND, m_listSubsystemFaultJointSound);
	DDX_Control(pDX, IDC_LIST_SUBSYSTEM_FAULT_JOINT_LED, m_listSubsystemFaultJointLED);
	DDX_Control(pDX, IDC_LIST_CHECK_FAULT, m_listCheckFault);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAlarmHostFaultProcess, CDialog)
	//{{AFX_MSG_MAP(CDlgAlarmHostFaultProcess)
	ON_BN_CLICKED(IDC_BTN_GET, OnBtnGet)
	ON_BN_CLICKED(IDC_BTN_SET, OnBtnSet)
	ON_CBN_SELCHANGE(IDC_COMBO_SUBSYSTEM, OnSelchangeComboSubsystem)
	ON_BN_CLICKED(IDC_BTN_CLOSE_OVERALL_FAULT_SOUND, OnBtnCloseOverallFaultSound)
	ON_BN_CLICKED(IDC_BTN_CLOSE_SUBSYSTEM_FAULT_SOUND, OnBtnCloseSubsystemFaultSound)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAlarmHostFaultProcess message handlers

BOOL CDlgAlarmHostFaultProcess::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
	m_lUserID = g_struDeviceInfo[m_iDeviceIndex].lLoginID;
	DWORD dwReturn = 0;
    if (NET_DVR_GetDeviceAbility(m_lUserID, ALARMHOST_ABILITY, NULL, 0, (char*)&m_struAlarmHostAbility, sizeof(m_struAlarmHostAbility)))
    {
        char szLan[128] = {0};
        g_pMainDlg->AddLog(m_lUserID, OPERATION_SUCC_T, "ALARMHOST_ABILITY");
    }
    else
    {
        g_pMainDlg->AddLog(m_lUserID, OPERATION_FAIL_T, "ALARMHOST_ABILITY");
        return FALSE;
    }
	memset(&m_struFaultCfg, 0, sizeof(m_struFaultCfg));
	InitListWnd();
	InitComboBox();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAlarmHostFaultProcess::InitComboBox() 
{
	char szLan[128] = {0};
	CString csStr;
	m_cmSubsystem.ResetContent();
	for (int i=0; i<m_struAlarmHostAbility.wSubSystemNum; i++)
	{
		csStr.Format("%d", i+1);
		m_cmSubsystem.AddString(csStr);
	}
	g_StringLanType(szLan, "������ϵͳ", "All subsystem");
	csStr.Format("%s", szLan);
	m_cmSubsystem.AddString(csStr);
}

void CDlgAlarmHostFaultProcess::InitListWnd() 
{
	char szLan[128] = {0};
	int iIndex = 0;
	int iBitIndex = 0;
	BOOL bBitState = 0;
    m_listCheckFault.SetExtendedStyle(LVS_EX_CHECKBOXES);
	m_listOverallFaultJointLED.SetExtendedStyle(LVS_EX_CHECKBOXES);
	m_listOverallFaultJointSound.SetExtendedStyle(LVS_EX_CHECKBOXES);
	m_listSubsystemFaultJointLED.SetExtendedStyle(LVS_EX_CHECKBOXES);
	m_listSubsystemFaultJointSound.SetExtendedStyle(LVS_EX_CHECKBOXES);
	m_listFaultLedLinkage.SetExtendedStyle(LVS_EX_CHECKBOXES);

	while (iBitIndex<14)
	{
		switch (iBitIndex)
		{
		case 0:
			g_StringLanType(szLan, "������ϵ�", "AC outage");
			break;
		case 1:
			g_StringLanType(szLan, "����Ƿѹ", "low voltage of battery");
			break;
		case 2:
			g_StringLanType(szLan, "��������", "device prevent disassemble");
			break;
		case 3:
			g_StringLanType(szLan, "�绰�ߵ���", "telephone off_line");
			break;
		case 4:
			g_StringLanType(szLan, "485�����쳣", "485 bus abnormal");
			break;
		case 5:
			g_StringLanType(szLan, "�������", "network abnormal");
			break;
		case 6:
			g_StringLanType(szLan, "�����쳣", "wireless abnormal");
			break;
		case 7:
			g_StringLanType(szLan, "��չ�����쳣", "expand bus abnormal");
			break;
		case 8:
			g_StringLanType(szLan, "Ӳ���쳣", "hard disk abnormal");
			break;
		case 9:
			g_StringLanType(szLan, "FPGA����", "FPGA fault");
			break;
		case 10:
			g_StringLanType(szLan, "ģ��������", "Sensor fault");
			break;
		case 11:
			g_StringLanType(szLan, "������չ���쳣", "Zone expand board abnormal");
			break;
        case 12:
            g_StringLanType(szLan, "WIFIͨ�Ź���", "Wifi connect failed");
            break;
        case 13:
            g_StringLanType(szLan, "RF�źŸ��Ź���", "Rf signal fault");
            break;
		default:
			break;
		}
		bBitState = (m_struAlarmHostAbility.dwFaultType >> iBitIndex) & 0x01;
		if (1 == bBitState)
		{
			m_listCheckFault.InsertItem(iIndex, szLan);
			m_listSubsystemFaultJointLED.InsertItem(iIndex, szLan);
			m_listSubsystemFaultJointSound.InsertItem(iIndex, szLan);
			m_listOverallFaultJointLED.InsertItem(iIndex, szLan);
			m_listOverallFaultJointSound.InsertItem(iIndex, szLan);
			m_listFaultLedLinkage.InsertItem(iIndex, szLan);
			m_listCheckFault.SetItemData(iIndex, iBitIndex);
			m_listOverallFaultJointLED.SetItemData(iIndex, iBitIndex);
			m_listOverallFaultJointSound.SetItemData(iIndex, iBitIndex);
			m_listSubsystemFaultJointLED.SetItemData(iIndex, iBitIndex);
			m_listSubsystemFaultJointSound.SetItemData(iIndex, iBitIndex);
			m_listFaultLedLinkage.SetItemData(iIndex++, iBitIndex);
		}
		iBitIndex++;
	}

	if (0 == m_struAlarmHostAbility.byOverallKeyboard)
	{
		GetDlgItem(IDC_LIST_OVERALL_FAULT_JOINT_LED)->EnableWindow(FALSE);
		GetDlgItem(IDC_LIST_OVERALL_FAULT_JOINT_SOUND)->EnableWindow(FALSE);
	}
}

void CDlgAlarmHostFaultProcess::OnBtnGet() 
{
	// TODO: Add your control notification handler code here
	char szLan[128] = {0};
	m_cmSubsystem.SetCurSel(0);
	UpdateData(FALSE);
	memset(&m_struFaultCfg, 0, sizeof(m_struFaultCfg));
	m_struFaultCfg.dwSize = sizeof(NET_DVR_ALARMHOST_FAULT_CFG);
	DWORD dwReturn = 0;
	if (!NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_ALARMHOST_FAULT_CFG, 0, &m_struFaultCfg, sizeof(m_struFaultCfg), &dwReturn))
	{
		g_StringLanType(szLan, "��ȡ���ϴ�������ʧ��", "Get fault process config failed");
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, szLan);
		return;
	}
	else
	{
		g_StringLanType(szLan, "��ȡ���ϴ������óɹ�", "Get fault process config successful");
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, szLan);
	}
	SetFaultInfoToDlg();
	m_iCurSubSystemIndex = 1;
}

void CDlgAlarmHostFaultProcess::SetFaultInfoToDlg() 
{
	int iBitIndex = 0;
	int i=0; 
	for (i=0; i<m_listCheckFault.GetItemCount(); i++)
	{
		iBitIndex = m_listCheckFault.GetItemData(i);
		m_listCheckFault.SetCheck(i, (BOOL)(((m_struFaultCfg.dwCheckFault>>iBitIndex)&0x01)));
	}
	for (i=0; i<m_listOverallFaultJointLED.GetItemCount(); i++)
	{
		iBitIndex = m_listOverallFaultJointLED.GetItemData(i);
		m_listOverallFaultJointLED.SetCheck(i, (BOOL)(((m_struFaultCfg.dwOverallFaultJointLED>>iBitIndex)&0x01)));
	}
	for (i=0; i<m_listOverallFaultJointSound.GetItemCount(); i++)
	{
		iBitIndex = m_listOverallFaultJointSound.GetItemData(i);
		m_listOverallFaultJointSound.SetCheck(i, (BOOL)(((m_struFaultCfg.dwOverallFaultJointSound>>iBitIndex)&0x01)));
	}
	m_cmSubsystem.SetCurSel(0);
	for (i=0; i<m_listSubsystemFaultJointLED.GetItemCount(); i++)
	{
		iBitIndex = m_listSubsystemFaultJointLED.GetItemData(i);
		m_listSubsystemFaultJointLED.SetCheck(i, (BOOL)(m_struFaultCfg.dwSubSystemFaultJointLED[0]>>iBitIndex)&0x01);
		m_listSubsystemFaultJointSound.SetCheck(i, (BOOL)(m_struFaultCfg.dwSubSystemFaultJointSound[0]>>iBitIndex)&0x01);
	}

	for (i=0; i<m_listFaultLedLinkage.GetItemCount(); i++)
	{
		iBitIndex = m_listFaultLedLinkage.GetItemData(i);
		m_listFaultLedLinkage.SetCheck(i, (BOOL)(((m_struFaultCfg.dwFaultJointFaultLight>>iBitIndex)&0x01)));
	}
	UpdateData(FALSE);
}

void CDlgAlarmHostFaultProcess::GetFaultInfoFromDlg() 
{
	UpdateData(TRUE);
	OnSelchangeComboSubsystem();
	int iBitIndex = 0;
	m_struFaultCfg.dwCheckFault = 0;
	int i=0; 
	for (i=0; i<m_listCheckFault.GetItemCount(); i++)
	{
		iBitIndex = m_listCheckFault.GetItemData(i);
		m_struFaultCfg.dwCheckFault |= (m_listCheckFault.GetCheck(i) << iBitIndex);
	}

	m_struFaultCfg.dwOverallFaultJointLED = 0;
	for (i=0; i<m_listOverallFaultJointLED.GetItemCount(); i++)
	{
		iBitIndex = m_listOverallFaultJointLED.GetItemData(i);
		m_struFaultCfg.dwOverallFaultJointLED |= (m_listOverallFaultJointLED.GetCheck(i) << iBitIndex);
	}

	m_struFaultCfg.dwOverallFaultJointSound = 0;
	for (i=0; i<m_listOverallFaultJointSound.GetItemCount(); i++)
	{
		iBitIndex = m_listOverallFaultJointSound.GetItemData(i);
		m_struFaultCfg.dwOverallFaultJointSound |= (m_listOverallFaultJointSound.GetCheck(i) << iBitIndex);
	}

	m_struFaultCfg.dwFaultJointFaultLight = 0;
	for (i=0; i<m_listFaultLedLinkage.GetItemCount(); i++)
	{
		iBitIndex = m_listFaultLedLinkage.GetItemData(i);
		m_struFaultCfg.dwFaultJointFaultLight |= (m_listFaultLedLinkage.GetCheck(i) << iBitIndex);
	}
}

void CDlgAlarmHostFaultProcess::OnBtnSet() 
{
	// TODO: Add your control notification handler code here
	char szLan[128] = {0};
	GetFaultInfoFromDlg();
	m_struFaultCfg.dwSize = sizeof(m_struFaultCfg);
	if (!NET_DVR_SetDVRConfig(m_lUserID, NET_DVR_SET_ALARMHOST_FAULT_CFG, 0, &m_struFaultCfg, sizeof(m_struFaultCfg)))
	{
		g_StringLanType(szLan, "���ù��ϴ�������ʧ��", "Set fault process config failed");
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, szLan);
		return;
	}
	else
	{
		g_StringLanType(szLan, "���ù��ϴ������óɹ�", "Set fault process config successful");
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, szLan);
	}
}

void CDlgAlarmHostFaultProcess::OnSelchangeComboSubsystem() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int iSubSystemIndex = m_cmSubsystem.GetCurSel()+1;
	int i=0;
	int iBitIndex = 0;
	if (m_struAlarmHostAbility.wSubSystemNum == (m_iCurSubSystemIndex -1))
	{
		g_StringLanType(m_szLan, "�Ƿ��޸�������ϵͳ���� ?", "modify all the sub system ?");
		if(IDOK != MessageBox(m_szLan, NULL, MB_OKCANCEL))
		{
			for (i=0; i<m_listSubsystemFaultJointLED.GetItemCount(); i++)
			{
				iBitIndex = m_listSubsystemFaultJointLED.GetItemData(i);
				m_listSubsystemFaultJointLED.SetCheck(i, (BOOL)(m_struFaultCfg.dwSubSystemFaultJointLED[iSubSystemIndex-1]>>iBitIndex)&0x01);
				m_listSubsystemFaultJointSound.SetCheck(i, (BOOL)(m_struFaultCfg.dwSubSystemFaultJointSound[iSubSystemIndex-1]>>iBitIndex)&0x01);
			}
			m_iCurSubSystemIndex = iSubSystemIndex;
			return;
		}
		m_struFaultCfg.dwSubSystemFaultJointLED[0] = 0;
		m_struFaultCfg.dwSubSystemFaultJointSound[0] = 0;
		for (i=0; i<m_listSubsystemFaultJointLED.GetItemCount(); i++)
		{
			iBitIndex = m_listSubsystemFaultJointLED.GetItemData(i);
			m_struFaultCfg.dwSubSystemFaultJointLED[0] |= (m_listSubsystemFaultJointLED.GetCheck(i) << iBitIndex);
			m_struFaultCfg.dwSubSystemFaultJointSound[0] |= (m_listSubsystemFaultJointSound.GetCheck(i) << iBitIndex);
		}
		//ȫ����ϵͳ
		for (int i=0; i<m_struAlarmHostAbility.wSubSystemNum; i++)
		{
			m_struFaultCfg.dwSubSystemFaultJointLED[i] = m_struFaultCfg.dwSubSystemFaultJointLED[0];
			m_struFaultCfg.dwSubSystemFaultJointSound[i] = m_struFaultCfg.dwSubSystemFaultJointSound[0];
		}
	}
	else
	{
		m_struFaultCfg.dwSubSystemFaultJointLED[m_iCurSubSystemIndex-1] = 0;
		m_struFaultCfg.dwSubSystemFaultJointSound[m_iCurSubSystemIndex-1] = 0;
		for (i=0; i<m_listSubsystemFaultJointLED.GetItemCount(); i++)
		{
			iBitIndex = m_listSubsystemFaultJointLED.GetItemData(i);
			m_struFaultCfg.dwSubSystemFaultJointLED[m_iCurSubSystemIndex-1] |= (m_listSubsystemFaultJointLED.GetCheck(i) << iBitIndex);
			m_struFaultCfg.dwSubSystemFaultJointSound[m_iCurSubSystemIndex-1] |= (m_listSubsystemFaultJointSound.GetCheck(i) << iBitIndex);
		}
	}

	
	if (m_struAlarmHostAbility.wSubSystemNum == (iSubSystemIndex-1))
	{
		//ѡ��������ϵͳʱ������ʾ�������
		for (i=0; i<m_listSubsystemFaultJointLED.GetItemCount(); i++)
		{
			m_listSubsystemFaultJointLED.SetCheck(i, FALSE);
			m_listSubsystemFaultJointSound.SetCheck(i, FALSE);
		}
	}
	else
	{
		for (i=0; i<m_listSubsystemFaultJointLED.GetItemCount(); i++)
		{
			iBitIndex = m_listSubsystemFaultJointLED.GetItemData(i);
			m_listSubsystemFaultJointLED.SetCheck(i, (BOOL)(m_struFaultCfg.dwSubSystemFaultJointLED[iSubSystemIndex-1]>>iBitIndex)&0x01);
			m_listSubsystemFaultJointSound.SetCheck(i, (BOOL)(m_struFaultCfg.dwSubSystemFaultJointSound[iSubSystemIndex-1]>>iBitIndex)&0x01);
		}
	}
	m_iCurSubSystemIndex = iSubSystemIndex;
	UpdateData(FALSE);
}

void CDlgAlarmHostFaultProcess::OnBtnCloseOverallFaultSound() 
{
	// TODO: Add your control notification handler code here
	char szLan[128] = {0};
	if (!NET_DVR_RemoteControl(m_lUserID, NET_DVR_CLOSE_ALARMHOST_OVERALL_FAULT_ALARM, &m_struCtrlParam, sizeof(m_struCtrlParam)))
	{
		g_StringLanType(szLan, "�ر�ȫ�ֹ�����ʾ��ʧ��", "Close  Fault Alarm Failed");
		MessageBox(szLan);
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, szLan);
		return;
	}
	else
	{
		g_StringLanType(szLan, "�ر�ȫ�ֹ�����ʾ���ɹ�", "Close Overall Fault Alarm Successful");
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, szLan);
	}
}

void CDlgAlarmHostFaultProcess::OnBtnCloseSubsystemFaultSound() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	char szLan[1024] = {0};
	DWORD dwSubSystemNum = m_cmSubsystem.GetCurSel() + 1;
	if (m_cmSubsystem.GetCurSel() == m_struAlarmHostAbility.wSubSystemNum)
	{
		dwSubSystemNum = 0xff;
	}
	memset(&m_struCtrlParam, 0, sizeof(m_struCtrlParam));
	m_struCtrlParam.dwSize = sizeof(m_struCtrlParam);
	m_struCtrlParam.byIndex = dwSubSystemNum;
	if (!NET_DVR_RemoteControl(m_lUserID, NET_DVR_CLOSE_SUBSYSTEM_FAULT_ALARM, &m_struCtrlParam, sizeof(m_struCtrlParam)))
	{
		g_StringLanType(szLan, "�ر���ϵͳ������ʾ��ʧ��", "Close Subsystem Fault Alarm Failed");
		MessageBox(szLan);
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, szLan);
		return;
	}
	else
	{
		g_StringLanType(szLan, "�ر���ϵͳ������ʾ���ɹ�", "Close Subsystem Fault Alarm Successful");
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, szLan);
	}
}
