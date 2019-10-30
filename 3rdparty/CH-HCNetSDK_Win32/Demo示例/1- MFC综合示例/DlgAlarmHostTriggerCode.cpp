// DlgAlarmHostTriggerCode.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgAlarmHostTriggerCode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


DWORD WINAPI CDlgAlarmHostTriggerCode::GetTriggerListThread(LPVOID lpArg)
{
	CDlgAlarmHostTriggerCode* pThis = reinterpret_cast<CDlgAlarmHostTriggerCode*>(lpArg);
	int bRet = 0;
	char szLan[128] = {0};
	while(pThis->m_bGetNext)
	{
		bRet = NET_DVR_GetNextRemoteConfig(pThis->m_lHandle, &pThis->m_lpStruAlarmOut[pThis->m_iStruCount], sizeof(NET_DVR_ALARMOUT_PARAM));
		if (bRet == NET_SDK_GET_NEXT_STATUS_SUCCESS)
		{
			pThis->AddTriggerInfoToDlg(&(pThis->m_lpStruAlarmOut[pThis->m_iStruCount]));
			pThis->m_iStruCount++;
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
				g_StringLanType(szLan, "��ȡ�������б����!", "Get route info Ending");
				g_pMainDlg->AddLog(pThis->m_iDeviceIndex, OPERATION_SUCC_T, szLan);
				//AfxMessageBox(szLan);
				break;
			}
			else if(bRet == NET_SDK_GET_NEXT_STATUS_FAILED)
			{
				g_StringLanType(szLan, "��ȡ�������б�ʧ��!", "Get route info failed");
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
			pThis->m_btnGet.EnableWindow(TRUE);
		}
	}
	return 0 ;
}

/////////////////////////////////////////////////////////////////////////////
// CDlgAlarmHostTriggerCode dialog

CDlgAlarmHostTriggerCode::CDlgAlarmHostTriggerCode(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAlarmHostTriggerCode::IDD, pParent)
	, m_iLastItem(-1)
	, m_lHandle(-1)
	, m_iDeviceIndex(-1)
	, m_lUserID(-1)
	, m_lpStruAlarmOut(NULL)
	, m_iStruCount(0)
	, m_bGetNext(FALSE)
{
	//{{AFX_DATA_INIT(CDlgAlarmHostTriggerCode)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CDlgAlarmHostTriggerCode::~CDlgAlarmHostTriggerCode()
{
	delete[] m_lpStruAlarmOut;
}
void CDlgAlarmHostTriggerCode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAlarmHostTriggerCode)
	DDX_Control(pDX, IDC_BTN_GET, m_btnGet);
	DDX_Control(pDX, IDC_LIST_TRIGGER, m_listTrigger);
	DDX_Control(pDX, IDC_EDIT_TEXT, m_Edit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAlarmHostTriggerCode, CDialog)
	//{{AFX_MSG_MAP(CDlgAlarmHostTriggerCode)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_TRIGGER, OnDblclkListTrigger)
	ON_EN_KILLFOCUS(IDC_EDIT_TEXT, OnKillfocusEditText)
	ON_BN_CLICKED(IDC_BTN_SET, OnBtnSet)
	ON_BN_CLICKED(IDC_BTN_GET, OnBtnGet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAlarmHostTriggerCode message handlers

BOOL CDlgAlarmHostTriggerCode::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
	m_lUserID = g_struDeviceInfo[m_iDeviceIndex].lLoginID;
	
	CString csStr;
	int i = 0;
	char szLan[128] = {0};
	m_lpStruAlarmOut = new NET_DVR_ALARMOUT_PARAM[MAX_ALARMHOST_ALARMOUT_NUM];
	memset(m_lpStruAlarmOut, 0, sizeof(NET_DVR_ALARMOUT_PARAM)*MAX_ALARMHOST_ALARMOUT_NUM);
	
	
	RECT  m_rect;
	m_listTrigger.GetClientRect(&m_rect); //��ȡlist�Ŀͻ���
	m_listTrigger.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT); //����list���
	
	g_StringLanType(szLan, "��������", "Trigger No");
	m_listTrigger.InsertColumn(0, szLan, LVCFMT_LEFT, m_rect.right/10);
	g_StringLanType(szLan, "״̬", "Status");
	m_listTrigger.InsertColumn(1, szLan, LVCFMT_LEFT, m_rect.right/10);
	g_StringLanType(szLan, "��������ַ", "Trigger Address");
	m_listTrigger.InsertColumn(2, szLan, LVCFMT_LEFT, m_rect.right/10);
	g_StringLanType(szLan, "������ͨ��", "Trigger Channel");
	m_listTrigger.InsertColumn(3, szLan, LVCFMT_LEFT, m_rect.right/10);
	g_StringLanType(szLan, "����������", "Trigger Name");
	m_listTrigger.InsertColumn(4, szLan, LVCFMT_LEFT, m_rect.right/10);
	g_StringLanType(szLan, "����������", "Trigger Type");
	m_listTrigger.InsertColumn(5, szLan, LVCFMT_LEFT, m_rect.right/10);

	g_StringLanType(szLan, "����ģʽ", "work Mode");
	m_listTrigger.InsertColumn(6, szLan, LVCFMT_LEFT, m_rect.right/10);
	g_StringLanType(szLan, "���ģʽ", "Alarm Out Mode");
	m_listTrigger.InsertColumn(7, szLan, LVCFMT_LEFT, m_rect.right/10);
	g_StringLanType(szLan, "��ʱ��", "Time on");
	m_listTrigger.InsertColumn(8, szLan, LVCFMT_LEFT, m_rect.right/10);
	g_StringLanType(szLan, "��ʱ��", "Time off");
	m_listTrigger.InsertColumn(9, szLan, LVCFMT_LEFT, m_rect.right/10);
	
	
// 	m_listTrigger.InsertItem(0,_T("001"));
// 	m_listTrigger.SetItemText(0,1,_T("����"));
// 	m_listTrigger.SetItemText(0,2,_T("1"));
// 	m_listTrigger.SetItemText(0,3,_T("1"));
// 	m_listTrigger.SetItemText(0,4,_T("������"));
// 	m_listTrigger.SetItemText(0,5,_T("��ģ��"));
// 	
// 	m_listTrigger.InsertItem(1,_T("001"));
// 	m_listTrigger.SetItemText(1,1,_T("����"));
// 	m_listTrigger.SetItemText(1,2,_T("1"));
// 	m_listTrigger.SetItemText(1,3,_T("1"));
// 	m_listTrigger.SetItemText(1,4,_T("9¥"));
// 	m_listTrigger.SetItemText(1,5,_T("8·������"));
	m_iRowCount = m_listTrigger.GetItemCount();  //��ȡ����
	
	m_bNeedSave = FALSE; //��ʼ��ΪFLASE;

	memset(&m_struChangeState, 0, sizeof(m_struChangeState));
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAlarmHostTriggerCode::OnDblclkListTrigger(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW  *pEditCtrl = (NM_LISTVIEW *)pNMHDR;
	CRect  EditRect;
	m_iItem = pEditCtrl->iItem;
	m_iSubItem = pEditCtrl->iSubItem;
	
	if (m_iItem == -1)
	{
		return;
	}
	if (0==m_iSubItem || 1==m_iSubItem || 5==m_iSubItem)
	{
		return;
	}
	
	if (m_iItem < m_iRowCount)
	{	
		m_listTrigger.GetSubItemRect(m_iItem,m_iSubItem,LVIR_LABEL,EditRect);
		m_bNeedSave = TRUE;
		m_Edit.SetParent(&m_listTrigger); //��list control����Ϊ������,���ɵ�Edit������ȷ��λ
		
		EditRect.SetRect(EditRect.left,EditRect.top,EditRect.left+m_listTrigger.GetColumnWidth(m_iSubItem),EditRect.bottom);
		
		//��ѡ�е�list�ؼ��е�����copy��edit�ؼ���
		CString strItem = m_listTrigger.GetItemText(m_iItem,m_iSubItem);
		m_Edit.MoveWindow(&EditRect);
		m_Edit.ShowWindow(SW_SHOW);
		m_Edit.SetWindowText(strItem);
		m_Edit.SetFocus();//����Ϊ����
		m_Edit.SetSel(0,-1);  //0,-1��ʾ��Ԫ������ȫѡ��
	}
	else
	{
		m_Edit.ShowWindow(SW_HIDE);
	}
	*pResult = 0;
}

void CDlgAlarmHostTriggerCode::OnKillfocusEditText() 
{
	// TODO: Add your control notification handler code here
	CString  csEditStr;
	char  szListStr[NAME_LEN];
	m_listTrigger.GetItemText(m_iItem, m_iSubItem, szListStr, NAME_LEN);
	m_Edit.GetWindowText(csEditStr); 
	if (csEditStr.Compare(szListStr))
	{
		m_listTrigger.SetItemText(m_iItem, m_iSubItem, csEditStr); 
		m_iLastItem = m_iItem;
		SaveTriggerInfo();
	}	
	m_Edit.ShowWindow(SW_HIDE);
}

void CDlgAlarmHostTriggerCode::SaveTriggerInfo() 
{
	char  szStr[128] = {0};
	//ģ���ַ
	m_listTrigger.GetItemText(m_iLastItem, 2, szStr, 128);
	m_lpStruAlarmOut[m_iLastItem].wModuleAddress = atoi(szStr);
	//ģ��ͨ��
	m_listTrigger.GetItemText(m_iLastItem, 3, szStr, 128);
	m_lpStruAlarmOut[m_iLastItem].byModuleChan = atoi(szStr);
	//������
	m_listTrigger.GetItemText(m_iLastItem, 4, szStr, 128);
	memcpy(m_lpStruAlarmOut[m_iLastItem].byName, szStr, NAME_LEN);
}

void CDlgAlarmHostTriggerCode::OnBtnSet() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	char szStr[128] = {0};
	int iTriggerNo = 0;
	if (m_iLastItem == -1)
	{
		g_StringLanType(szStr, "û�в��������ı�", "No Parameter Changed");
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, szStr);
		return;
	}
	m_listTrigger.GetItemText(m_iLastItem, 0, szStr, 128);
	iTriggerNo = atoi(szStr) - 1;
	if (NET_DVR_SetDVRConfig(m_lUserID, NET_DVR_SET_ALARMOUT_PARAM, iTriggerNo, &m_lpStruAlarmOut[m_iLastItem], sizeof(NET_DVR_ALARMOUT_PARAM)))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_SET_ALARMOUT_PARAM");
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_SET_ALARMOUT_PARAM");
    }	
}

void CDlgAlarmHostTriggerCode::OnBtnGet() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	char szLan[128] = {0};
	m_struListInfo.dwSize = sizeof(NET_DVR_LIST_INFO);
	m_listTrigger.DeleteAllItems();
	m_iRowCount = 0;
	m_iStruCount = 0;
	if (m_lHandle>=0)
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "It is getting data, wait for a moment");
	}
	else
	{
		m_lHandle = NET_DVR_StartRemoteConfig(m_lUserID, NET_DVR_GET_ALARMHOST_TRIGGER_LIST, &m_struListInfo, sizeof(m_struListInfo), NULL, this);
		if (m_lHandle>=0)
		{
			m_bGetNext = TRUE;
			g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "Start Remote Config successfully");
			DWORD dwThreadId;
			m_hGetInfoThread = CreateThread(NULL,0,LPTHREAD_START_ROUTINE(GetTriggerListThread),this,0,&dwThreadId);
			GetDlgItem(IDC_BTN_GET)->EnableWindow(FALSE);
		}
		else
		{
			g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "Start Remote Config failed");
			return;
		}
	}
}

void CDlgAlarmHostTriggerCode::AddTriggerInfoToDlg(LPNET_DVR_ALARMOUT_PARAM lpInter) 
{
	char szStr[128] = {0};
	//��������
	sprintf(szStr, "%3d", lpInter->wTriggerIndex+1);
	m_listTrigger.InsertItem(m_iRowCount, szStr);
	//״̬
	if (1 == lpInter->byModuleStatus)
	{
		g_StringLanType(szStr, "����", "ON-LINE");
	}
	else if (2 == lpInter->byModuleStatus)
	{
		g_StringLanType(szStr, "����", "OFF-LINE");
	}
	else
	{
		g_StringLanType(szStr, "δ֪״̬", "UNKNOWN");
	}
	m_listTrigger.SetItemText(m_iRowCount, 1, szStr);
	//ģ���ַ
	if (0xffff == lpInter->wModuleAddress)
	{

		g_StringLanType(szStr, "δ֪", "UNKNOWN");
	}
	else
	{
		sprintf(szStr, "%3d", lpInter->wModuleAddress);
	}
	m_listTrigger.SetItemText(m_iRowCount, 2, szStr);
	//ģ��ͨ����ַ
	if (0xff == lpInter->byModuleChan)
	{
		g_StringLanType(szStr, "δ֪", "UNKNOWN");
	}
	else
	{
		sprintf(szStr, "%3d", lpInter->byModuleChan);
	}
	m_listTrigger.SetItemText(m_iRowCount, 3, szStr);
	//��������
	memset(szStr, 0, sizeof(szStr));
	memcpy(szStr, lpInter->byName, NAME_LEN);
	m_listTrigger.SetItemText(m_iRowCount, 4, szStr);
	//ģ������
	if (1 == lpInter->byModuleType)
	{
		g_StringLanType(szStr, "���ش�����", "Local Trigger");
	}
	else if (2 == lpInter->byModuleType)
	{
		g_StringLanType(szStr, "4·������", "4-Triggers");
	}
	else if (3 == lpInter->byModuleType)
	{
		g_StringLanType(szStr, "8·������", "8-Triggers");
	}
	else if (4 == lpInter->byModuleType)
	{
		g_StringLanType(szStr, "������������", "1-Zone&Trigger");
	}
	else if (5 == lpInter->byModuleType)
	{
		g_StringLanType(szStr, "32·������", "32-Triggers");
	}
    else if (6 == lpInter->byModuleType)
    {
        g_StringLanType(szStr, "1�ž͵ؿ�����", "1 door controller");
    }
    else if (7 == lpInter->byModuleType)
    {
        g_StringLanType(szStr, "2�ž͵ؿ�����", "2 door controllers");
    }
    else if (8 == lpInter->byModuleType)
    {
        g_StringLanType(szStr, "4�ž͵ؿ�����", "4 door controllers");
    }
    else if (10 == lpInter->byModuleType)
    {
        g_StringLanType(szStr, "����", "keyboard");
    }
    else if (11 == lpInter->byModuleType)
    {
        g_StringLanType(szStr, "��չ������", "extended trigger");
    }
	else
	{
		g_StringLanType(szStr, "δ֪����", "Unknown type");
	}
	m_listTrigger.SetItemText(m_iRowCount, 5, szStr);
    if (1 == lpInter->byWorkMode)
	{
		g_StringLanType(szStr, "����", "linkage");
	}
    else if (2 == lpInter->byWorkMode)
	{
		g_StringLanType(szStr, "�涯", "dynamic");
	}
	m_listTrigger.SetItemText(m_iRowCount, 6, szStr);
	
    if (1 == lpInter->byAlarmOutMode)
	{
		g_StringLanType(szStr, "������ģʽ", "none pulse mode");
	}
    else if (2 == lpInter->byAlarmOutMode)
	{
		g_StringLanType(szStr, "����ģʽ", "pulse mode");
	}
	m_listTrigger.SetItemText(m_iRowCount, 7, szStr);
	
	sprintf(szStr, "%d", lpInter->byTimeOn);
	m_listTrigger.SetItemText(m_iRowCount, 8, szStr);
	
	sprintf(szStr, "%d", lpInter->byTimeOff);
	m_listTrigger.SetItemText(m_iRowCount, 9, szStr);

    m_iRowCount++;\
}

void CDlgAlarmHostTriggerCode::OnCancel()
{
	m_bGetNext = FALSE;
	if(WAIT_TIMEOUT == WaitForSingleObject(m_hGetInfoThread, 3000))
	{
		TerminateThread(m_hGetInfoThread, 0);
	}
	m_hGetInfoThread = NULL;
	CDialog::OnCancel();
}
