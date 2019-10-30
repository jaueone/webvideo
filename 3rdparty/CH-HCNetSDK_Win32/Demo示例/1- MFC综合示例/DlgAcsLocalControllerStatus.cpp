// DlgAcsLocalControllerStatus.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ClientDemo.h"
#include "DlgAcsLocalControllerStatus.h"
#include "afxdialogex.h"


// CDlgAcsLocalControllerStatus �Ի���

IMPLEMENT_DYNAMIC(CDlgAcsLocalControllerStatus, CDialogEx)

void CALLBACK g_fGetStatusCallback(DWORD dwType, void* lpBuffer, DWORD dwBufLen, void* pUserData);
#define WM_MSG_GETSTATUS_FINISH 1003
#define WM_MSG_ADD_STATUS_TOLIST 1004
#define WM_MSG_UPDATEDATA_INTERFACE   1005

CDlgAcsLocalControllerStatus::CDlgAcsLocalControllerStatus(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgAcsLocalControllerStatus::IDD, pParent)
	, m_iLocalController(0)
{
	m_iSelectItem = -1;
	m_lServerID = -1;
	m_iDevIndex = -1;
	m_lGetStatusHandle = -1;
	m_lSetStatusHandle = -1;
}

CDlgAcsLocalControllerStatus::~CDlgAcsLocalControllerStatus()
{
}

void CDlgAcsLocalControllerStatus::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LOCAL_CONTROLLER_NO, m_iLocalController);
	DDX_Control(pDX, IDC_LIST_LOCAL_CONTROLLER_STATUS, m_listLocalControllerStatus);
	DDX_Control(pDX, IDC_LIST_MAGNETIC_STATUS, m_listMagneticStatus);
	DDX_Control(pDX, IDC_LIST_DOOR_LOCKSTATUS, m_listDoorLockStatus);
	DDX_Control(pDX, IDC_LIST_READER_STATUS, m_listReaderStatus);
}


BEGIN_MESSAGE_MAP(CDlgAcsLocalControllerStatus, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CDlgAcsLocalControllerStatus::OnBnClickedButtonSearch)
	ON_NOTIFY(NM_CLICK, IDC_LIST_LOCAL_CONTROLLER_STATUS, &CDlgAcsLocalControllerStatus::OnNMClickListLocalControllerStatus)
	ON_MESSAGE(WM_MSG_GETSTATUS_FINISH, OnMsgGetStatusFinish)
	ON_MESSAGE(WM_MSG_ADD_STATUS_TOLIST, OnMsgAddStatusToList)
	ON_MESSAGE(WM_MSG_UPDATEDATA_INTERFACE, OnMsgUpdateData)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CDlgAcsLocalControllerStatus ��Ϣ�������

BOOL CDlgAcsLocalControllerStatus::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	char szLan[64] = { 0 };

	m_listLocalControllerStatus.DeleteAllItems();
	g_StringLanType(szLan, "���", "No");
	m_listLocalControllerStatus.InsertColumn(0, szLan, LVCFMT_LEFT, 40);
	g_StringLanType(szLan, "�͵ؿ��������", "LocalControllerNo");
	m_listLocalControllerStatus.InsertColumn(1, szLan, LVCFMT_LEFT, 60);
	g_StringLanType(szLan, "�͵ؿ���������״̬", "LocalAntiDismantleStatus");
	m_listLocalControllerStatus.InsertColumn(2, szLan, LVCFMT_LEFT, 100);
	g_StringLanType(szLan, "�豸����״̬", "PowerSupplyStatus");
	m_listLocalControllerStatus.InsertColumn(3, szLan, LVCFMT_LEFT, 80);
	g_StringLanType(szLan, "���ص�ѹֵ", "BatteryVoltage");
	m_listLocalControllerStatus.InsertColumn(4, szLan, LVCFMT_LEFT, 80);
	g_StringLanType(szLan, "�����Ƿ��ڵ�ѹ״̬", "BatteryLowVoltage");
	m_listLocalControllerStatus.InsertColumn(5, szLan, LVCFMT_LEFT, 120);
	g_StringLanType(szLan, "��������", "FireAlarm");
	m_listLocalControllerStatus.InsertColumn(6, szLan, LVCFMT_LEFT, 80);
	g_StringLanType(szLan, "�豸���к�", "SerialNumber");
	m_listLocalControllerStatus.InsertColumn(7, szLan, LVCFMT_LEFT, 120);
	g_StringLanType(szLan, "�͵ؿ���������״̬(�ֿ�������״̬)", "LocalControllerStatus");
	m_listLocalControllerStatus.InsertColumn(8, szLan, LVCFMT_LEFT, 120);
	m_listLocalControllerStatus.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_listMagneticStatus.DeleteAllItems();
	g_StringLanType(szLan, "�ű��", "DoorNo");
	m_listMagneticStatus.InsertColumn(0, szLan, LVCFMT_LEFT, 80);
	g_StringLanType(szLan, "�Ŵ�״̬", "MagneticStatus");
	m_listMagneticStatus.InsertColumn(1, szLan, LVCFMT_LEFT, 120);
	m_listMagneticStatus.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_listDoorLockStatus.DeleteAllItems();
	g_StringLanType(szLan, "�ű��", "DoorNo");
	m_listDoorLockStatus.InsertColumn(0, szLan, LVCFMT_LEFT, 80);
	g_StringLanType(szLan, "����״̬", "DoorLockStatus");
	m_listDoorLockStatus.InsertColumn(1, szLan, LVCFMT_LEFT, 120);
	m_listDoorLockStatus.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_listReaderStatus.DeleteAllItems();
	g_StringLanType(szLan, "���������", "DoorNo");
	m_listReaderStatus.InsertColumn(0, szLan, LVCFMT_LEFT, 80);
	g_StringLanType(szLan, "������״̬", "DoorReaderStatus");
	m_listReaderStatus.InsertColumn(1, szLan, LVCFMT_LEFT, 120);
	m_listReaderStatus.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CALLBACK g_fGetStatusCallback(DWORD dwType, void* lpBuffer, DWORD dwBufLen, void* pUserData)
{
	CDlgAcsLocalControllerStatus* pDlg = (CDlgAcsLocalControllerStatus*)pUserData;
	if (pDlg == NULL)
	{
		return;
	}
	pDlg->ProcessGetStatusCallbackData(dwType, lpBuffer, dwBufLen);
}

LRESULT CDlgAcsLocalControllerStatus::OnMsgGetStatusFinish(WPARAM wParam, LPARAM lParam)
{
	NET_DVR_StopRemoteConfig(m_lGetStatusHandle);
	m_lGetStatusHandle = -1;
	g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_GET_LOCAL_CONTROLLER_STATUS Get finish");
	return 0;
}

LRESULT CDlgAcsLocalControllerStatus::OnMsgAddStatusToList(WPARAM wParam, LPARAM lParam)
{
	LPNET_DVR_LOCAL_CONTROLLER_STATUS lpStatusCfg = (LPNET_DVR_LOCAL_CONTROLLER_STATUS)wParam;
	if (lpStatusCfg->dwSize == 0)
	{
		return 0;
	}
	AddToStatusList(*lpStatusCfg, lpStatusCfg->wLocalControllerID);
	delete lpStatusCfg;
	return 0;
}

LRESULT CDlgAcsLocalControllerStatus::OnMsgUpdateData(WPARAM wParam, LPARAM lParam)
{
	DWORD dwTrue = (DWORD)wParam;
	UpdateData(dwTrue);
	return 0;
}

void CDlgAcsLocalControllerStatus::AddToStatusList(const NET_DVR_LOCAL_CONTROLLER_STATUS& struStatusInfo, const WORD wLocalControllerNo)
{
	LPNET_DVR_LOCAL_CONTROLLER_STATUS pStatusCfg = NULL;
	CString strItem = "";
	int iIndex = -1;
	if (wLocalControllerNo != 0 && m_listLocalControllerStatus.GetItemCount() >= wLocalControllerNo)
	{
		iIndex = struStatusInfo.wLocalControllerID;
		pStatusCfg = (LPNET_DVR_LOCAL_CONTROLLER_STATUS)m_listLocalControllerStatus.GetItemData(iIndex);
	}
	else
	{
		pStatusCfg = new NET_DVR_LOCAL_CONTROLLER_STATUS;
		iIndex = m_listLocalControllerStatus.GetItemCount();
		strItem.Format("%d", iIndex + 1);
		m_listLocalControllerStatus.InsertItem(iIndex, strItem);
	}
	if (iIndex < 0)
	{
		return;
	}
	memcpy(pStatusCfg, &struStatusInfo, sizeof(struStatusInfo));
	m_listLocalControllerStatus.SetItemData(iIndex, (DWORD)pStatusCfg);
	strItem.Format("%d", struStatusInfo.wLocalControllerID);
	m_listLocalControllerStatus.SetItemText(iIndex, 1, strItem);
	if (struStatusInfo.byLocalAntiDismantleStatus == 0)
	{
		m_listLocalControllerStatus.SetItemText(iIndex, 2, "�ر�");
	}
	else if(struStatusInfo.byLocalAntiDismantleStatus == 1)
	{
		m_listLocalControllerStatus.SetItemText(iIndex, 2, "����");
	}
	if (struStatusInfo.byPowerSupplyStatus == 1)
	{
		m_listLocalControllerStatus.SetItemText(iIndex, 3, "�����繩��");
	}
	else if(struStatusInfo.byPowerSupplyStatus == 2)
	{
		m_listLocalControllerStatus.SetItemText(iIndex, 3, "���ع���");
	}
	strItem.Format("%f", ((float)struStatusInfo.wBatteryVoltage / 10));
	m_listLocalControllerStatus.SetItemText(iIndex, 4, strItem);
	if (struStatusInfo.byBatteryLowVoltage == 0)
	{
		m_listLocalControllerStatus.SetItemText(iIndex, 5, "��");
	}
	else if (struStatusInfo.byBatteryLowVoltage == 1)
	{
		m_listLocalControllerStatus.SetItemText(iIndex, 5, "��");
	}
	if (struStatusInfo.byFireAlarm == 0)
	{
		m_listLocalControllerStatus.SetItemText(iIndex, 6, "����");
	}
	else if (struStatusInfo.byFireAlarm == 1)
	{
		m_listLocalControllerStatus.SetItemText(iIndex, 6, "�̽ӱ���");
	}
	else if (struStatusInfo.byFireAlarm == 2)
	{
		m_listLocalControllerStatus.SetItemText(iIndex, 6, "�Ͽ�����");
	}
	m_listLocalControllerStatus.SetItemText(iIndex, 7, (char*)struStatusInfo.bySerialNumber);
	switch (struStatusInfo.wLocalControllerStatus)
	{
	case 0:
		m_listLocalControllerStatus.SetItemText(iIndex, 8, "����");
		break;
	case 1:
		m_listLocalControllerStatus.SetItemText(iIndex, 8, "��������");
		break;
	case 2:
		m_listLocalControllerStatus.SetItemText(iIndex, 8, "��·1�ϵ�RS485����1");
		break;
	case 3:
		m_listLocalControllerStatus.SetItemText(iIndex, 8, "��·1�ϵ�RS485����2");
		break;
	case 4:
		m_listLocalControllerStatus.SetItemText(iIndex, 8, "��·2�ϵ�RS485����1");
		break;
	case 5:
		m_listLocalControllerStatus.SetItemText(iIndex, 8, "��·2�ϵ�RS485����2");
		break;
	case 6:
		m_listLocalControllerStatus.SetItemText(iIndex, 8, "��·3�ϵ�RS485����1");
		break;
	case 7:
		m_listLocalControllerStatus.SetItemText(iIndex, 8, "��·3�ϵ�RS485����2");
		break;
	case 8:
		m_listLocalControllerStatus.SetItemText(iIndex, 8, "��·4�ϵ�RS485����1");
		break;
	case 9:
		m_listLocalControllerStatus.SetItemText(iIndex, 8, "��·4�ϵ�RS485����2");
		break;
	default:
		break;
	}
	//delete pBellCfg;
	UpdateData(FALSE);
}

void CDlgAcsLocalControllerStatus::ProcessGetStatusCallbackData(DWORD dwType, void* lpBuffer, DWORD dwBufLen)
{
	if (dwType == NET_SDK_CALLBACK_TYPE_DATA)
	{
		LPNET_DVR_LOCAL_CONTROLLER_STATUS lpStatusCfg = new NET_DVR_LOCAL_CONTROLLER_STATUS;
		memcpy(lpStatusCfg, lpBuffer, sizeof(*lpStatusCfg));
		g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "GetLocalController PROCESSING %d", lpStatusCfg->wLocalControllerID);
		PostMessage(WM_MSG_ADD_STATUS_TOLIST, (WPARAM)lpStatusCfg, 0);
	}
	else if (dwType == NET_SDK_CALLBACK_TYPE_STATUS)
	{
		DWORD dwStatus = *(DWORD*)lpBuffer;
		if (dwStatus == NET_SDK_CALLBACK_STATUS_SUCCESS)
		{
			PostMessage(WM_MSG_GETSTATUS_FINISH, 0, 0);
		}
		else if (dwStatus == NET_SDK_CALLBACK_STATUS_FAILED)
		{
			g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "GetLocalController STATUS_FAILED");
		}
	}
}

void CDlgAcsLocalControllerStatus::OnBnClickedButtonSearch()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_lGetStatusHandle != -1)
	{
		NET_DVR_StopRemoteConfig(m_lGetStatusHandle);
	}
	m_listLocalControllerStatus.DeleteAllItems();
	UpdateData(TRUE);
	NET_DVR_LOCAL_CONTROLLER_STATUS_COND struCond = { 0 };
	struCond.dwSize = sizeof(struCond);
	struCond.wLocalControllerID = m_iLocalController;

	m_lGetStatusHandle = NET_DVR_StartRemoteConfig(m_lServerID, NET_DVR_GET_LOCAL_CONTROLLER_STATUS, &struCond, sizeof(struCond), g_fGetStatusCallback, this);
	if (m_lGetStatusHandle == -1)
	{
		g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_GET_LOCAL_CONTROLLER_STATUS");
		return;
	}
	else
	{
		g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_GET_LOCAL_CONTROLLER_STATUS");
	}
}


void CDlgAcsLocalControllerStatus::OnNMClickListLocalControllerStatus(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	LPNET_DVR_LOCAL_CONTROLLER_STATUS lpStatus = GetSelItem();
	UpdateSubList(lpStatus);
	*pResult = 0;
}

LPNET_DVR_LOCAL_CONTROLLER_STATUS CDlgAcsLocalControllerStatus::GetSelItem()
{
	UpdateData(TRUE);
	POSITION  iPos = m_listLocalControllerStatus.GetFirstSelectedItemPosition();

	if (iPos == NULL)
	{
		return NULL;
	}
	int iCurSel = m_listLocalControllerStatus.GetNextSelectedItem(iPos);
	m_iSelectItem = iCurSel;
	m_listLocalControllerStatus.SetItemState(m_iSelectItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	LPNET_DVR_LOCAL_CONTROLLER_STATUS lpStatus = (LPNET_DVR_LOCAL_CONTROLLER_STATUS)m_listLocalControllerStatus.GetItemData(iCurSel);
	return lpStatus;
}

void CDlgAcsLocalControllerStatus::UpdateSubList(const NET_DVR_LOCAL_CONTROLLER_STATUS *lpStatus)
{
	if (!lpStatus)
	{
		return;
	}
	m_listMagneticStatus.DeleteAllItems();
	m_listDoorLockStatus.DeleteAllItems();
	m_listReaderStatus.DeleteAllItems();
	int i = 0;
	CString strItem = "";
	for (i = 0; i < MAX_DOOR_NUM; i++)
	{
		strItem.Format("%d", i + 1);
		m_listMagneticStatus.InsertItem(i, strItem);
		switch (lpStatus->byMagneticStatus[i])
		{
		case 0:
			m_listMagneticStatus.SetItemText(i, 1, "�����ر�");
			break;
		case 1:
			m_listMagneticStatus.SetItemText(i, 1, "��������");
			break;
		case 2:
			m_listMagneticStatus.SetItemText(i, 1, "�ƻ���·����");
			break;
		case 3:
			m_listMagneticStatus.SetItemText(i, 1, "�ƻ���·����");
			break;
		case 4:
			m_listMagneticStatus.SetItemText(i, 1, "�쳣����");
			break;
		default:
			break;
		}
	}
	for (i = 0; i < MAX_DOOR_NUM; i++)
	{
		strItem.Format("%d", i + 1);
		m_listDoorLockStatus.InsertItem(i, strItem);
		switch (lpStatus->byDoorLockStatus[i])
		{
		case 0:
			m_listDoorLockStatus.SetItemText(i, 1, "�����ر�");
			break;
		case 1:
			m_listDoorLockStatus.SetItemText(i, 1, "��������");
			break;
		case 2:
			m_listDoorLockStatus.SetItemText(i, 1, "�ƻ���·����");
			break;
		case 3:
			m_listDoorLockStatus.SetItemText(i, 1, "�ƻ���·����");
			break;
		case 4:
			m_listDoorLockStatus.SetItemText(i, 1, "�쳣����");
			break;
		default:
			break;
		}
	}
	for (i = 0; i < MAX_CARD_READER_NUM; i++)
	{
		strItem.Format("%d", i + 1);
		m_listReaderStatus.InsertItem(i, strItem);
		switch (lpStatus->byCardReaderOnlineStatus[i])
		{
		case 0:
			m_listReaderStatus.SetItemText(i, 1, "������");
			break;
		case 1:
			m_listReaderStatus.SetItemText(i, 1, "����");
			break;
		default:
			break;
		}
	}
}


void CDlgAcsLocalControllerStatus::OnClose()
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (m_lGetStatusHandle != -1)
	{
		NET_DVR_StopRemoteConfig(m_lGetStatusHandle);
		m_lGetStatusHandle = -1;
	}

	CDialogEx::OnClose();
}
