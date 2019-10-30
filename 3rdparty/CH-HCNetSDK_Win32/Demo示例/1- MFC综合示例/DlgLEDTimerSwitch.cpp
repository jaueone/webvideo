// DlgLEDTimerSwitch.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgLEDTimerSwitch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgLEDTimerSwitch dialog


CDlgLEDTimerSwitch::CDlgLEDTimerSwitch(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLEDTimerSwitch::IDD, pParent)
	,m_iDeviceIndex(-1)
	, m_lUserID(-1)
{
	//{{AFX_DATA_INIT(CDlgLEDTimerSwitch)
		// NOTE: the ClassWizard will add member initialization here
	m_chTime1 = FALSE;
	m_chTime2 = FALSE;
	m_chTime3 = FALSE;
	m_time1Off = COleDateTime::GetCurrentTime();
	m_time1On = COleDateTime::GetCurrentTime();
	m_time2Off = COleDateTime::GetCurrentTime();
	m_time2On = COleDateTime::GetCurrentTime();
	m_time3Off = COleDateTime::GetCurrentTime();
	m_time3On = COleDateTime::GetCurrentTime();
	//}}AFX_DATA_INIT
}


void CDlgLEDTimerSwitch::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgLEDTimerSwitch)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Check(pDX, IDC_CHECK_TIME1, m_chTime1);
	DDX_Check(pDX, IDC_CHECK_TIME2, m_chTime2);
	DDX_Check(pDX, IDC_CHECK_TIME3, m_chTime3);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_TIME1_OFF, m_time1Off);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_TIME1_ON, m_time1On);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_TIME2_OFF, m_time2Off);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_TIME2_ON, m_time2On);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_TIME3_OFF, m_time3Off);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_TIME3_ON, m_time3On);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgLEDTimerSwitch, CDialog)
	//{{AFX_MSG_MAP(CDlgLEDTimerSwitch)
	ON_BN_CLICKED(IDC_CHECK_TIME1, OnCheckTime1)
	ON_BN_CLICKED(IDC_CHECK_TIME2, OnCheckTime2)
	ON_BN_CLICKED(IDC_CHECK_TIME3, OnCheckTime3)
	ON_BN_CLICKED(IDC_BTN_GET, OnBtnGet)
	ON_BN_CLICKED(IDC_BTN_SET, OnBtnSet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLEDTimerSwitch message handlers

BOOL CDlgLEDTimerSwitch::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	memset(&m_struSwitchTime, 0, sizeof(m_struSwitchTime));
	m_iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
	m_lUserID = g_struDeviceInfo[m_iDeviceIndex].lLoginID;

	OnCheckTime1();
	OnCheckTime2();
	OnCheckTime3();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgLEDTimerSwitch::OnCheckTime1() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	if (m_chTime1 == 0)//��ʱ�����Ч
	{
		m_struSwitchTime.struTimer[0].byValid = 0;
		GetDlgItem(IDC_DATETIMEPICKER_TIME1_ON)->EnableWindow(FALSE);
		GetDlgItem(IDC_DATETIMEPICKER_TIME1_OFF)->EnableWindow(FALSE);
	}
	else //��ʱ�����Ч
	{
		m_struSwitchTime.struTimer[0].byValid = 1;
		m_time1On.SetTime(m_struSwitchTime.struTimer[0].struTimeOn.byHour, m_struSwitchTime.struTimer[0].struTimeOn.byMinute, 0);
		m_time1Off.SetTime(m_struSwitchTime.struTimer[0].struTimeOff.byHour, m_struSwitchTime.struTimer[0].struTimeOff.byMinute, 0);
		GetDlgItem(IDC_DATETIMEPICKER_TIME1_ON)->EnableWindow(TRUE);
		GetDlgItem(IDC_DATETIMEPICKER_TIME1_OFF)->EnableWindow(TRUE);
	}
	UpdateData(FALSE);	
}

void CDlgLEDTimerSwitch::OnCheckTime2() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_chTime2 == 0)//��ʱ�����Ч
	{
		m_struSwitchTime.struTimer[1].byValid = 0;
		GetDlgItem(IDC_DATETIMEPICKER_TIME2_ON)->EnableWindow(FALSE);
		GetDlgItem(IDC_DATETIMEPICKER_TIME2_OFF)->EnableWindow(FALSE);
	}
	else //��ʱ�����Ч
	{
		m_struSwitchTime.struTimer[1].byValid = 1;
		m_time2On.SetTime(m_struSwitchTime.struTimer[1].struTimeOn.byHour, m_struSwitchTime.struTimer[1].struTimeOn.byMinute, 0);
		m_time2Off.SetTime(m_struSwitchTime.struTimer[1].struTimeOff.byHour, m_struSwitchTime.struTimer[1].struTimeOff.byMinute, 0);
		GetDlgItem(IDC_DATETIMEPICKER_TIME2_ON)->EnableWindow(TRUE);
		GetDlgItem(IDC_DATETIMEPICKER_TIME2_OFF)->EnableWindow(TRUE);
	}
	UpdateData(FALSE);
}

void CDlgLEDTimerSwitch::OnCheckTime3() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_chTime3 == 0)//��ʱ�����Ч
	{
		m_struSwitchTime.struTimer[2].byValid = 0;
		GetDlgItem(IDC_DATETIMEPICKER_TIME3_ON)->EnableWindow(FALSE);
		GetDlgItem(IDC_DATETIMEPICKER_TIME3_OFF)->EnableWindow(FALSE);
	}
	else //��ʱ�����Ч
	{
		m_struSwitchTime.struTimer[2].byValid = 1;
		m_time3On.SetTime(m_struSwitchTime.struTimer[2].struTimeOn.byHour, m_struSwitchTime.struTimer[2].struTimeOn.byMinute, 0);
		m_time3Off.SetTime(m_struSwitchTime.struTimer[2].struTimeOff.byHour, m_struSwitchTime.struTimer[2].struTimeOff.byMinute, 0);
		GetDlgItem(IDC_DATETIMEPICKER_TIME3_ON)->EnableWindow(TRUE);
		GetDlgItem(IDC_DATETIMEPICKER_TIME3_OFF)->EnableWindow(TRUE);
	}
	UpdateData(FALSE);
}

void CDlgLEDTimerSwitch::OnBtnGet() 
{
	// TODO: Add your control notification handler code here
	DWORD dwReturn = 0;
	m_struSwitchTime.dwSize = sizeof(m_struSwitchTime);
	if (!NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_LED_TIMER_SWITCH, 0, &m_struSwitchTime, sizeof(m_struSwitchTime), &dwReturn))
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_GET_LED_TIMER_SWITCH FAILED");
		return;
	}
	else
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_GET_LED_TIMER_SWITCH SUCC");
		m_chTime1 = m_struSwitchTime.struTimer[0].byValid;
		m_chTime2 = m_struSwitchTime.struTimer[1].byValid;
		m_chTime3 = m_struSwitchTime.struTimer[2].byValid;
		UpdateData(FALSE);
		OnCheckTime1();
		OnCheckTime2();
		OnCheckTime3();
	}
}

void CDlgLEDTimerSwitch::OnBtnSet() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (FALSE == CheckAndGetTime())
	{
		return;
	}
	m_struSwitchTime.dwSize = sizeof(m_struSwitchTime);
	if (!NET_DVR_SetDVRConfig(m_lUserID, NET_DVR_SET_LED_TIMER_SWITCH, 0, &m_struSwitchTime, sizeof(m_struSwitchTime)))
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_SET_LED_TIMER_SWITCH FAILED");
		return;
	}
	else
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_SET_LED_TIMER_SWITCH SUCC");
	}
}

BOOL CDlgLEDTimerSwitch::CheckAndGetTime()
{
	//ʱ���1��Ч���ж�ʱ���1�Ŀ����͹ػ�ʱ��
	if (m_struSwitchTime.struTimer[0].byValid == 1)
	{
		if (m_time1Off <= m_time1On)
		{
			MessageBox("Error: time1_off is early than time1_on");
			return FALSE;
		}
		GetTime(LED_TIME1);
	}

	//ʱ���2��Ч���ж�ʱ���2�Ŀ����ػ�ʱ�䣬��ʱ���2�Ŀ���ʱ����Ҫ����ʱ���1�Ĺػ�ʱ��
	if (m_struSwitchTime.struTimer[1].byValid == 1)
	{
		//����ʱ���1�����ж�
		if (m_struSwitchTime.struTimer[0].byValid == 1)
		{
			if (m_time2On <= m_time1Off)
			{
				MessageBox("Error: time2_on is early than time1_off");
				return FALSE;
			}
		}
		//ʱ���2�Ĺػ�ʱ��Ϳ���ʱ��Ƚ�
		if (m_time2Off <= m_time2On)
		{
			MessageBox("time2_off is early than time2_on");
			return FALSE;
		}
		GetTime(LED_TIME2);
	}

	//ʱ���3��Ч���ж�ʱ���3�Ŀ����ػ�ʱ�䣬��ʱ���3�Ŀ���ʱ����Ҫ����ʱ���2�Ĺػ�ʱ��
	if (m_struSwitchTime.struTimer[2].byValid == 1)
	{
		//����ʱ���2�����ж�
		if (m_struSwitchTime.struTimer[0].byValid == 1)
		{
			if (m_time3On <= m_time2Off)
			{
				MessageBox("Error: time3_on is early than time2_off");
				return FALSE;
			}
		}
		else	//ʱ���2��Ч������Ҫ��ʱ���1�Ƿ���Ч
		{
			if (m_struSwitchTime.struTimer[0].byValid == 1)
			{
				if (m_time3On <= m_time1Off)
				{
					MessageBox("Error: time3_on is early than time1_off");
					return FALSE;
				}
			}
		}
		//ʱ���3�Ĺػ�ʱ��Ϳ���ʱ��Ƚ�
		if (m_time3Off <= m_time3On)
		{
			MessageBox("time3_off is early than time3_on");
			return FALSE;
		}
		GetTime(LED_TIME3);
	}
	return TRUE;
} 

void CDlgLEDTimerSwitch::GetTime(INT iTimeIndex)
{
	//��ʱʱ���1
	memset(&m_struSwitchTime.struTimer[iTimeIndex].struTimeOn, 0, sizeof(NET_DVR_TIME_EX));
	if (iTimeIndex == LED_TIME1)
	{
		m_struSwitchTime.struTimer[iTimeIndex].struTimeOn.byHour = m_time1On.GetHour();
		m_struSwitchTime.struTimer[iTimeIndex].struTimeOn.byMinute = m_time1On.GetMinute();
		m_struSwitchTime.struTimer[iTimeIndex].struTimeOff.byHour = m_time1Off.GetHour();
		m_struSwitchTime.struTimer[iTimeIndex].struTimeOff.byMinute = m_time1Off.GetMinute();
	}
	//��ʱʱ���2
	else if(iTimeIndex == LED_TIME2)
	{
		m_struSwitchTime.struTimer[iTimeIndex].struTimeOn.byHour = m_time2On.GetHour();
		m_struSwitchTime.struTimer[iTimeIndex].struTimeOn.byMinute = m_time2On.GetMinute();	
		m_struSwitchTime.struTimer[iTimeIndex].struTimeOff.byHour = m_time2Off.GetHour();
		m_struSwitchTime.struTimer[iTimeIndex].struTimeOff.byMinute = m_time2Off.GetMinute();	
	}
	//��ʱʱ���3
	else if (iTimeIndex == LED_TIME3)
	{
		m_struSwitchTime.struTimer[iTimeIndex].struTimeOn.byHour = m_time3On.GetHour();
		m_struSwitchTime.struTimer[iTimeIndex].struTimeOn.byMinute = m_time3On.GetMinute();	
		m_struSwitchTime.struTimer[iTimeIndex].struTimeOff.byHour = m_time3Off.GetHour();
		m_struSwitchTime.struTimer[iTimeIndex].struTimeOff.byMinute = m_time3Off.GetMinute();
	}
}
