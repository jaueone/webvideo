// DlgGetAllSensorJoint.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgGetAllSensorJoint.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgGetAllSensorJoint dialog
DWORD WINAPI CDlgGetAllSensorJoint::GetConfigThread(LPVOID lpArg)
{
	CDlgGetAllSensorJoint* pThis = reinterpret_cast<CDlgGetAllSensorJoint*>(lpArg);
	int bRet = 0;
	char szLan[128] = {0};
	memset(&(pThis->m_struAlarmHostSensorJointCfg),0,sizeof(&(pThis->m_struAlarmHostSensorJointCfg)));
	while(pThis->m_bGetNext)
	{
		bRet = NET_DVR_GetNextRemoteConfig(pThis->m_lHandle, &pThis->m_struAlarmHostSensorJointCfg, sizeof(pThis->m_struAlarmHostSensorJointCfg));
		if (bRet == NET_SDK_GET_NEXT_STATUS_SUCCESS)
		{
			pThis->AddInfoToDlg();
		}
		else
		{
			if (bRet == NET_SDK_GET_NETX_STATUS_NEED_WAIT)
			{
				Sleep(5);
				continue;
			}
			if (bRet == NET_SDK_GET_NEXT_STATUS_FINISH)
			{
				g_StringLanType(szLan, "��ȡ���ý���!", "Get route info Ending");
				g_pMainDlg->AddLog(pThis->m_iDeviceIndex, OPERATION_SUCC_T, szLan);
				//AfxMessageBox(szLan);
				break;
			}
			else if(bRet == NET_SDK_GET_NEXT_STATUS_FAILED)
			{
				g_StringLanType(szLan, "��ȡ����ʧ��!", "Get route info failed");
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
	if (-1 != pThis->m_lHandle)
	{
		if (!NET_DVR_StopRemoteConfig(pThis->m_lHandle))
		{
			g_pMainDlg->AddLog(pThis->m_iDeviceIndex, OPERATION_FAIL_T, "Stop Remote Config Failed");
			pThis->m_bGetNext = FALSE;
		}
		else
		{
			g_pMainDlg->AddLog(pThis->m_iDeviceIndex, OPERATION_SUCC_T, "Stop Remote Config Successful");
			pThis->m_bGetNext = FALSE;
			pThis->m_lHandle = -1;
		}
	}
	return 0 ;
}

void CDlgGetAllSensorJoint::AddInfoToDlg() 
{
	//���
	if(0xff == m_struAlarmRs485Cfg.byChannel)
	{
		sprintf(m_sTemp, "%s", "--");
	}
	else
	{
		sprintf(m_sTemp, "%d", m_struAlarmRs485Cfg.byChannel);
	}
	m_listRs485.InsertItem(m_iRowCount,m_sTemp);
	
	//ǰ���豸����
	sprintf(m_sTemp, "%s", (char*)m_struAlarmRs485Cfg.sDeviceName, NAME_LEN);
	m_listRs485.SetItemText(m_iRowCount, 1, m_sTemp);
	
	sprintf(m_sTemp, "%d", m_struAlarmRs485Cfg.wDeviceType);
	m_listRs485.SetItemText(m_iRowCount, 2, m_sTemp);
	
	sprintf(m_sTemp, "%d", m_struAlarmRs485Cfg.wDeviceProtocol);
	m_listRs485.SetItemText(m_iRowCount, 3, m_sTemp);

	
	

	switch (m_struAlarmRs485Cfg.dwBaudRate)
	{
	case 0:
		sprintf(m_sTemp, "%s", "50");
		break;
	case 1:
		sprintf(m_sTemp, "%s", "75");
		break;
	case 2:
		sprintf(m_sTemp, "%s", "110");
		break;
	case 3:
		sprintf(m_sTemp, "%s", "150");
		break;
	case 4:
		sprintf(m_sTemp, "%s", "300");
		break;
	case 5:
		sprintf(m_sTemp, "%s", "600");
		break;
	case 6:
		sprintf(m_sTemp, "%s", "1200");
		break;
	case 7:
		sprintf(m_sTemp, "%s", "2400");
		break;
	case 8:
		sprintf(m_sTemp, "%s", "4800");
		break;
	case 9:
		sprintf(m_sTemp, "%s", "9600");
		break;
	case 10:
		sprintf(m_sTemp, "%s", "19200");
		break;
	case 11:
		sprintf(m_sTemp, "%s", "38400");
		break;
	case 12:
		sprintf(m_sTemp, "%s", "57600");
		break;
	case 13:
		sprintf(m_sTemp, "%s", "76800");
		break;
	case 14:
		sprintf(m_sTemp, "%s", "115.2k");
		break;
	default:
		sprintf(m_sTemp, "--");
		break;
	}
	m_listRs485.SetItemText(m_iRowCount, 4, m_sTemp);

	sprintf(m_sTemp, "%dλ", m_struAlarmRs485Cfg.byDataBit+5);
	m_listRs485.SetItemText(m_iRowCount, 5, m_sTemp);

	sprintf(m_sTemp, "%dλ", m_struAlarmRs485Cfg.byStopBit+1);
	m_listRs485.SetItemText(m_iRowCount, 6, m_sTemp);

	if (m_struAlarmRs485Cfg.byParity == 0)
	{
		sprintf(m_sTemp, "��У��");
	}
	else if (m_struAlarmRs485Cfg.byParity == 1)
	{
		sprintf(m_sTemp, "��У��");
	}
	else if (m_struAlarmRs485Cfg.byParity == 2)
	{
		sprintf(m_sTemp, "żУ��");
	}
	else
	{
		sprintf(m_sTemp, "������");
	}
	m_listRs485.SetItemText(m_iRowCount, 7, m_sTemp);

	if (m_struAlarmRs485Cfg.byFlowcontrol == 0)
	{
		sprintf(m_sTemp, "��");
	}
	else if (m_struAlarmRs485Cfg.byFlowcontrol == 1)
	{
		sprintf(m_sTemp, "������");
	}
	else if (m_struAlarmRs485Cfg.byFlowcontrol == 2)
	{
		sprintf(m_sTemp, "Ӳ����");
	}
	else
	{
		sprintf(m_sTemp, "������");
	}
	m_listRs485.SetItemText(m_iRowCount, 8, m_sTemp);

	if (m_struAlarmRs485Cfg.byDuplex == 0)
	{
		sprintf(m_sTemp, "��˫��");
	}
	else if (m_struAlarmRs485Cfg.byDuplex == 1)
	{
		sprintf(m_sTemp, "ȫ˫��");
	}
	else
	{
		sprintf(m_sTemp, "������");
	}
	m_listRs485.SetItemText(m_iRowCount, 9, m_sTemp);

	if (m_struAlarmRs485Cfg.byDuplex == 0)
	{
		sprintf(m_sTemp, "����̨");
	}
	else if (m_struAlarmRs485Cfg.byDuplex == 1)
	{
		sprintf(m_sTemp, "͸��ͨ��");
	}
	else
	{
		sprintf(m_sTemp, "������");
	}
	m_listRs485.SetItemText(m_iRowCount, 10, m_sTemp);
	
	m_iRowCount++;
}


CDlgGetAllSensorJoint::CDlgGetAllSensorJoint(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGetAllSensorJoint::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgGetAllSensorJoint)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgGetAllSensorJoint::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgGetAllSensorJoint)
	DDX_Control(pDX, IDC_LIST_SENSOR_JOINT, m_listSensorJoint);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgGetAllSensorJoint, CDialog)
	//{{AFX_MSG_MAP(CDlgGetAllSensorJoint)
	ON_BN_CLICKED(IDC_BTN_GET, OnBtnGet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgGetAllSensorJoint message handlers

BOOL CDlgGetAllSensorJoint::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
	m_lUserID = g_struDeviceInfo[m_iDeviceIndex].lLoginID;
	m_listRs485.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_listRs485.InsertColumn(0, "485ͨ����", LVCFMT_LEFT, 80, -1);
	m_listRs485.InsertColumn(1,"ǰ���豸����", LVCFMT_LEFT, 150, -1);
	m_listRs485.InsertColumn(2,"ǰ���豸����", LVCFMT_LEFT, 80, -1);
	m_listRs485.InsertColumn(3,"ǰ���豸Э��", LVCFMT_LEFT, 80, -1);
	m_listRs485.InsertColumn(4,"������", LVCFMT_LEFT, 80, -1);
	m_listRs485.InsertColumn(5,"����λ", LVCFMT_LEFT, 80, -1);
	m_listRs485.InsertColumn(6,"ֹͣλ", LVCFMT_LEFT, 80, -1);
	m_listRs485.InsertColumn(7,"У������", LVCFMT_LEFT, 80, -1);
	m_listRs485.InsertColumn(8,"��������", LVCFMT_LEFT, 80, -1);
	m_listRs485.InsertColumn(9,"˫��ģʽ", LVCFMT_LEFT, 80, -1);
	m_listRs485.InsertColumn(10,"����ģʽ", LVCFMT_LEFT, 80, -1);
	return TRUE;
}

void CDlgGetAllSensorJoint::OnBtnGet() 
{
	// TODO: Add your control notification handler code here
	m_listSensorJoint.DeleteAllItems();
	m_iRowCount = 0;
	if (m_lHandle>=0)
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "It is getting data, wait for a moment");
	}
	else
	{
		m_lHandle = NET_DVR_StartRemoteConfig(m_lUserID, NET_DVR_GET_ALL_ALARM_SENSOR_JOINT, NULL, 0, NULL, this);
		if (m_lHandle>=0)
		{
			m_bGetNext = TRUE;
			g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "Start Remote Config successfully");
			DWORD dwThreadId;
			m_hGetInfoThread = CreateThread(NULL,0,LPTHREAD_START_ROUTINE(GetConfigThread), this, 0, &dwThreadId);
		}
		else
		{
			g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "Start Remote Config failed");
			return;
		}
	}
}
