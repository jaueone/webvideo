// DlgGetAllRs485Cfg.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgGetAllRs485Cfg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgGetAllRs485Cfg dialog
DWORD WINAPI DlgGetAllRs485Cfg::GetConfigThread(LPVOID lpArg)
{
	DlgGetAllRs485Cfg* pThis = reinterpret_cast<DlgGetAllRs485Cfg*>(lpArg);
	int bRet = 0;
	char szLan[128] = {0};
	memset(&(pThis->m_struAlarmRs485Cfg),0,sizeof(&(pThis->m_struAlarmRs485Cfg)));
	while(pThis->m_bGetNext)
	{
		bRet = NET_DVR_GetNextRemoteConfig(pThis->m_lHandle, &pThis->m_struAlarmRs485Cfg, sizeof(pThis->m_struAlarmRs485Cfg));
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

void DlgGetAllRs485Cfg::AddInfoToDlg() 
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

    if (m_struAlarmRs485Cfg.byWorkMode == 0)
	{
		sprintf(m_sTemp, "����̨");
	}
    else if (m_struAlarmRs485Cfg.byWorkMode == 1)
	{
		sprintf(m_sTemp, "͸��ͨ��");
	}
    else if (m_struAlarmRs485Cfg.byWorkMode == 2)
    {
        sprintf(m_sTemp, "�ݿ�");
    }
    else if (m_struAlarmRs485Cfg.byWorkMode == 3)
    {
        sprintf(m_sTemp, "������");
    }
    else if (m_struAlarmRs485Cfg.byWorkMode == 4)
    {
        sprintf(m_sTemp, "�Զ���");
    }
    else if (m_struAlarmRs485Cfg.byWorkMode == 0xff)
    {
        sprintf(m_sTemp, "����");
    }
	else
	{
		sprintf(m_sTemp, "������");
	}
	m_listRs485.SetItemText(m_iRowCount, 10, m_sTemp);

    if (m_struAlarmRs485Cfg.bySerialType == 0)
    {
        sprintf(m_sTemp, "485");
    }
    else if (m_struAlarmRs485Cfg.bySerialType == 1)
    {
        sprintf(m_sTemp, "232");
    }
    else
    {
        sprintf(m_sTemp, "Unknown");
    }
    m_listRs485.SetItemText(m_iRowCount, 11, m_sTemp);
	
	m_iRowCount++;
}

DlgGetAllRs485Cfg::DlgGetAllRs485Cfg(CWnd* pParent /*=NULL*/)
	: CDialog(DlgGetAllRs485Cfg::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgGetAllRs485Cfg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
}


void DlgGetAllRs485Cfg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgGetAllRs485Cfg)
	DDX_Control(pDX, IDC_LIST_RS485, m_listRs485);
	//}}AFX_DATA_MAP
	m_lHandle = -1;
}


BEGIN_MESSAGE_MAP(DlgGetAllRs485Cfg, CDialog)
	//{{AFX_MSG_MAP(DlgGetAllRs485Cfg)
	ON_BN_CLICKED(IDC_BTN_GET, OnBtnGet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgGetAllRs485Cfg message handlers
BOOL DlgGetAllRs485Cfg::OnInitDialog() 
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
    m_listRs485.InsertColumn(11, "��������", LVCFMT_LEFT, 80, -1);
	GetDeviceTypeList(m_struDevTypeList);
	return TRUE;
}

BOOL DlgGetAllRs485Cfg::GetDeviceTypeList(NET_DVR_DEVICE_TYPE_LIST& struDevTypeList)
{
    if (NET_DVR_GetDeviceTypeList(m_lUserID, &struDevTypeList))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_GetDeviceTypeList");
        return TRUE;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_GetDeviceTypeList");
        return FALSE;
    }
}

void DlgGetAllRs485Cfg::OnBtnGet() 
{
	// TODO: Add your control notification handler code here
	NET_DVR_485LIST_INFO stru485ListInfo;
	memset(&stru485ListInfo,0,sizeof(NET_DVR_485LIST_INFO));
	stru485ListInfo.dwSize = sizeof(NET_DVR_485LIST_INFO);
	stru485ListInfo.byAll = 1;

	m_listRs485.DeleteAllItems();
	m_iRowCount = 0;
	if (m_lHandle>=0)
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "It is getting data, wait for a moment");
	}
	else
	{
		m_lHandle = NET_DVR_StartRemoteConfig(m_lUserID, NET_DVR_GET_ALL_ALARM_RS485CFG, &stru485ListInfo, sizeof(NET_DVR_485LIST_INFO), NULL, this);
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
