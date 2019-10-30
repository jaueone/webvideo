// DlgAlarmHostPrinter.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgAlarmHostPrinter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAlarmHostPrinter dialog


CDlgAlarmHostPrinter::CDlgAlarmHostPrinter(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAlarmHostPrinter::IDD, pParent)
	, m_iDeviceIndex(-1)
	, m_lUserID(-1)
{
	//{{AFX_DATA_INIT(CDlgAlarmHostPrinter)
	m_chEnable = FALSE;
	m_chPrintTime = FALSE;
	m_chFault = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgAlarmHostPrinter::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAlarmHostPrinter)
	DDX_Control(pDX, IDC_LIST_OPERATE_INFO, m_listOperateInfo);
	DDX_Control(pDX, IDC_LIST_DEVICE_INFO, m_listDeviceInfo);
	DDX_Control(pDX, IDC_LIST_ALARM_INFO, m_listAlarmInfo);
	DDX_Check(pDX, IDC_CHECK_ENABLE, m_chEnable);
	DDX_Check(pDX, IDC_CHECK_PRINT_TIME, m_chPrintTime);
	DDX_Check(pDX, IDC_CHECK_FAULT, m_chFault);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAlarmHostPrinter, CDialog)
	//{{AFX_MSG_MAP(CDlgAlarmHostPrinter)
	ON_BN_CLICKED(IDC_BTN_GET, OnBtnGet)
	ON_BN_CLICKED(IDC_BTN_SET, OnBtnSet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAlarmHostPrinter message handlers

BOOL CDlgAlarmHostPrinter::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
	m_lUserID = g_struDeviceInfo[m_iDeviceIndex].lLoginID;
	m_listAlarmInfo.SetExtendedStyle(LVS_EX_CHECKBOXES);
	m_listDeviceInfo.SetExtendedStyle(LVS_EX_CHECKBOXES);
	m_listOperateInfo.SetExtendedStyle(LVS_EX_CHECKBOXES);
	int iIndex = 0;
	//bit0-zone alarm��bit1-zone alarm restore��bit2-emergency Keypad Alarms��bit-3-duress alarm
	g_StringLanType(m_szLan, "��������", "zone Alarm");
	m_listAlarmInfo.InsertItem(iIndex++, m_szLan);
	g_StringLanType(m_szLan, "���������ָ�", "zone Alarm restore");
	m_listAlarmInfo.InsertItem(iIndex++, m_szLan);
	g_StringLanType(m_szLan, "��������", "emergency keypad Alarms");
	m_listAlarmInfo.InsertItem(iIndex++, m_szLan);
	g_StringLanType(m_szLan, "Ю�ֱ���", "duress alarm");
	m_listAlarmInfo.InsertItem(iIndex++, m_szLan);
	
	//bit17-BUS Fault��bit18-BUS restore
	iIndex = 0;
	g_StringLanType(m_szLan, "������ϵ�", "AC loss");
	m_listDeviceInfo.InsertItem(iIndex++, m_szLan);
	g_StringLanType(m_szLan, "������ϵ�ָ�", "AC loss restore");
	m_listDeviceInfo.InsertItem(iIndex++, m_szLan);
	g_StringLanType(m_szLan, "���ص�ѹ��", "low battery");
	m_listDeviceInfo.InsertItem(iIndex++, m_szLan);
	g_StringLanType(m_szLan, "���ص�ѹ�ͻָ�", "low battery restore");
	m_listDeviceInfo.InsertItem(iIndex++, m_szLan);
	g_StringLanType(m_szLan, "�绰�߹���", "PSTN fault");
	m_listDeviceInfo.InsertItem(iIndex++, m_szLan);
	g_StringLanType(m_szLan, "�绰�߹��ϻָ�", "PSTN fault restore");
	m_listDeviceInfo.InsertItem(iIndex++, m_szLan);
	g_StringLanType(m_szLan, "���Ա���", "Test Report");
	m_listDeviceInfo.InsertItem(iIndex++, m_szLan);

	g_StringLanType(m_szLan, "����", "tamper alarm");
	m_listDeviceInfo.InsertItem(iIndex++, m_szLan);
	g_StringLanType(m_szLan, "����ָ�", "tamper restore");
	m_listDeviceInfo.InsertItem(iIndex++, m_szLan);

	g_StringLanType(m_szLan, "485�豸����", "485 device fault");
	m_listDeviceInfo.InsertItem(iIndex++, m_szLan);
	g_StringLanType(m_szLan, "485�豸���߻ָ�", "485 device restore");
	m_listDeviceInfo.InsertItem(iIndex++, m_szLan);

	g_StringLanType(m_szLan, "GPRS/3G����", "GPRS/3G fault");
	m_listDeviceInfo.InsertItem(iIndex++, m_szLan);
	g_StringLanType(m_szLan, "GPRS/3G����", "GPRS/3G fault restore");
	m_listDeviceInfo.InsertItem(iIndex++, m_szLan);

	g_StringLanType(m_szLan, "���������쳣", "NetWork fault");
	m_listDeviceInfo.InsertItem(iIndex++, m_szLan);
	g_StringLanType(m_szLan, "���������쳣�ָ�", "NetWork fault restore");
	m_listDeviceInfo.InsertItem(iIndex++, m_szLan);

	g_StringLanType(m_szLan, "�����쳣", "BUS fault");
	m_listDeviceInfo.InsertItem(iIndex++, m_szLan);
	g_StringLanType(m_szLan, "�����쳣�ָ�", "BUS fault restore");
	m_listDeviceInfo.InsertItem(iIndex++, m_szLan);
	g_StringLanType(m_szLan, "Ӳ�̹���", "hard disk fault");
	m_listDeviceInfo.InsertItem(iIndex++, m_szLan);
	g_StringLanType(m_szLan, "Ӳ�̹��ϻָ�", "hard disk fault restore");
	m_listDeviceInfo.InsertItem(iIndex++, m_szLan);
	g_StringLanType(m_szLan, "��������", "keyboard locked");
	m_listDeviceInfo.InsertItem(iIndex++, m_szLan);
	
	iIndex = 0;
	g_StringLanType(m_szLan, "����", "Arm");
	m_listOperateInfo.InsertItem(iIndex++, m_szLan);
	g_StringLanType(m_szLan, "����", "Disarm");
	m_listOperateInfo.InsertItem(iIndex++, m_szLan);
	g_StringLanType(m_szLan, "����", "Clear Alarm");
	m_listOperateInfo.InsertItem(iIndex++, m_szLan);
	g_StringLanType(m_szLan, "��·", "Bypass");
	m_listOperateInfo.InsertItem(iIndex++, m_szLan);
	g_StringLanType(m_szLan, "��·�ָ�", "Bypass restore");
	m_listOperateInfo.InsertItem(iIndex++, m_szLan);
	g_StringLanType(m_szLan, "������", "Enter Code");
	m_listOperateInfo.InsertItem(iIndex++, m_szLan);
	g_StringLanType(m_szLan, "�˳����", "Exit Code");
	m_listOperateInfo.InsertItem(iIndex++, m_szLan);
	g_StringLanType(m_szLan, "������λ", "Device restore");
	m_listOperateInfo.InsertItem(iIndex++, m_szLan);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAlarmHostPrinter::OnBtnGet() 
{
	// TODO: Add your control notification handler code here
	memset(&m_struPrinter, 0, sizeof(m_struPrinter));
	m_struPrinter.dwSize = sizeof(m_struPrinter);
	DWORD dwReturn = 0;
	BOOL	bState = 0;
	if (!NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_ALARMHOST_PRINTER_CFG, 0, &m_struPrinter, sizeof(m_struPrinter), &dwReturn))
	{
		g_StringLanType(m_szLan, "��ȡ��ӡ������ʧ��", "Get Printer Config Failed");
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, m_szLan);
		return;
	}
	else
	{
		m_chEnable = m_struPrinter.byPrinterEnable;
		m_chPrintTime = m_struPrinter.byPrintTime;
		m_chFault = m_struPrinter.byFaultDetect;
		int i=0;
		for (i=0; i<m_listAlarmInfo.GetItemCount(); i++)
		{
			bState = (m_struPrinter.dwAlarmInfo>>i) & 0x01;
			m_listAlarmInfo.SetCheck(i, bState);
		}
		for (i=0; i<m_listDeviceInfo.GetItemCount(); i++)
		{
			bState = (m_struPrinter.dwDeviceInfo>>i) & 0x01;
			m_listDeviceInfo.SetCheck(i, bState);
		}
		for (i=0; i<m_listOperateInfo.GetItemCount(); i++)
		{
			bState = (m_struPrinter.dwOperateInfo>>i) & 0x01;
			m_listOperateInfo.SetCheck(i, bState);
		}
		g_StringLanType(m_szLan, "��ȡ��ӡ�����óɹ�", "Get Printer Config Successful");
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, m_szLan);
	}
	UpdateData(FALSE);
}

void CDlgAlarmHostPrinter::OnBtnSet() 
{
	// TODO: Add your control notification handler code here
	memset((char*)&m_struPrinter, 0, sizeof(m_struPrinter));
	UpdateData(TRUE);
	m_struPrinter.byPrinterEnable = m_chEnable;
	m_struPrinter.byPrintTime = m_chPrintTime;
	m_struPrinter.byFaultDetect = m_chFault;
	int test =  m_listAlarmInfo.GetItemCount();
	int i=0;
	for (i=0; i<m_listAlarmInfo.GetItemCount(); i++)
	{
		m_struPrinter.dwAlarmInfo |= m_listAlarmInfo.GetCheck(i)<<i;
	}
	for (i=0; i<m_listDeviceInfo.GetItemCount(); i++)
	{
		m_struPrinter.dwDeviceInfo |= m_listDeviceInfo.GetCheck(i)<<i;
	}
	for (i=0; i<m_listOperateInfo.GetItemCount(); i++)
	{
		m_struPrinter.dwOperateInfo |= m_listOperateInfo.GetCheck(i)<<i;
	}

	m_struPrinter.dwSize = sizeof(m_struPrinter);
	DWORD dwReturn = 0;
	if (!NET_DVR_SetDVRConfig(m_lUserID, NET_DVR_SET_ALARMHOST_PRINTER_CFG, 0, &m_struPrinter, sizeof(m_struPrinter)))
	{
		g_StringLanType(m_szLan, "���ô�ӡ������ʧ��", "Set Printer Config Failed");
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, m_szLan);
		return;
	}
	else
	{
		g_StringLanType(m_szLan, "���ô�ӡ�����óɹ�", "Set Printer Config Successful");
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, m_szLan);
	}
}
