// DlgAlarmHostZoomAddr.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgAlarmHostZoomAddr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAlarmHostZoneAddr dialog

DWORD WINAPI CDlgAlarmHostZoneAddr::GetZoomListThread(LPVOID lpArg)
{
	CDlgAlarmHostZoneAddr* pThis = reinterpret_cast<CDlgAlarmHostZoneAddr*>(lpArg);
	//WaitForSingleObject(pThis->m_hRecvOver, INFINITE);
	int bRet = 0;
	char szLan[128] = {0};
	while(pThis->m_bGetNext)
	{
		if(-1 == pThis->m_lHandle)
		{
			continue;
		}
		bRet = NET_DVR_GetNextRemoteConfig(pThis->m_lHandle, &pThis->m_lpStruAlarmIn[pThis->m_iZoomCount], sizeof(NET_DVR_ALARMIN_PARAM));
		if (bRet == NET_SDK_GET_NEXT_STATUS_SUCCESS)
		{
			pThis->AddZoomInfoToDlg(&(pThis->m_lpStruAlarmIn[pThis->m_iZoomCount]));
			pThis->m_iZoomCount++;
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
				g_StringLanType(szLan, "��ȡ�����б����!", "Get route info Ending");
				//AfxMessageBox(szLan);
				break;
			}
			else if(bRet == NET_SDK_GET_NEXT_STATUS_FAILED)
			{
				g_StringLanType(szLan, "��ȡ�����б�ʧ��!", "Get route info failed");
				AfxMessageBox(szLan);
				break;
			}
			else
			{
				g_StringLanType(szLan, "δ֪״̬", "Unknown status");
			//	AfxMessageBox(szLan);
				break;
			}
		}
	}
	if (-1 != pThis->m_lHandle)
	{
		if (!NET_DVR_StopRemoteConfig(pThis->m_lHandle))
		{
			g_pMainDlg->AddLog(pThis->m_iDeviceIndex, OPERATION_FAIL_T, "Stop Remote Config Failed");
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

CDlgAlarmHostZoneAddr::CDlgAlarmHostZoneAddr(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAlarmHostZoneAddr::IDD, pParent)
	,m_iLastItem(-1)
	, m_lHandle(-1)
	, m_iDeviceIndex(-1)
	, m_lUserID(-1)
	, m_lpStruAlarmIn(NULL)
	, m_iStruCount(0)
	, m_bGetNext(FALSE)
	, m_iZoomCount(0)
{
	//{{AFX_DATA_INIT(CDlgAlarmHostZoneAddr)
	//}}AFX_DATA_INIT
}

CDlgAlarmHostZoneAddr::~CDlgAlarmHostZoneAddr()
{
	delete[] m_lpStruAlarmIn;
	m_lpStruAlarmIn = NULL;
}
void CDlgAlarmHostZoneAddr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAlarmHostZoneAddr)
	DDX_Control(pDX, IDC_BTN_GET, m_btnGet);
	DDX_Control(pDX, IDC_COMBO_SUBSYSTEM_INDEX, m_cmSubsystem);
	DDX_Control(pDX, IDC_EDIT_TEXT, m_Edit);
	DDX_Control(pDX, IDC_LIST_ZOOM, m_listZoom);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAlarmHostZoneAddr, CDialog)
	//{{AFX_MSG_MAP(CDlgAlarmHostZoneAddr)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ZOOM, OnDblclkListZoom)
	ON_EN_KILLFOCUS(IDC_EDIT_TEXT, OnKillfocusEditText)
	ON_CBN_SELCHANGE(IDC_COMBO_SUBSYSTEM_INDEX, OnSelchangeComboSubsystemIndex)
	ON_BN_CLICKED(IDC_BTN_GET, OnBtnGet)
	ON_BN_CLICKED(IDC_BTN_SET, OnBtnSet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAlarmHostZoneAddr message handlers

BOOL CDlgAlarmHostZoneAddr::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
	m_lUserID = g_struDeviceInfo[m_iDeviceIndex].lLoginID;

	CString csStr;
	int i = 0;
	char szLan[128] = {0};
	m_struAlarmHostAbility.dwSize = sizeof(m_struAlarmHostAbility);
	if (!NET_DVR_GetDeviceAbility(m_lUserID, ALARMHOST_ABILITY, NULL, 0, (char*)&m_struAlarmHostAbility, sizeof(m_struAlarmHostAbility)))
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_GetDeviceAbility FAILED");
	}
	else
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_GetDeviceAbility SUCC");
	}
	//��ϵͳ��
	for (i=0; i<m_struAlarmHostAbility.wSubSystemNum; i++)
	{
		g_StringLanType(szLan, "��ϵͳ", "SubSystem");
		csStr.Format("%s%d", szLan, i+1);
		m_cmSubsystem.AddString(csStr);
	}
	g_StringLanType(szLan, "������ϵͳ", "All SubSystem");
	csStr.Format("%s", szLan);
	m_cmSubsystem.AddString(csStr);
	m_lpStruAlarmIn = new NET_DVR_ALARMIN_PARAM[MAX_ALARMHOST_ALARMIN_NUM];
	memset(m_lpStruAlarmIn, 0, sizeof(NET_DVR_ALARMIN_PARAM)*MAX_ALARMHOST_ALARMIN_NUM);


	RECT  m_rect;
	m_listZoom.GetClientRect(&m_rect); //��ȡlist�Ŀͻ���
	m_listZoom.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT); //����list���
	
	g_StringLanType(szLan, "����", "Zoom");
	m_listZoom.InsertColumn(0, szLan, LVCFMT_LEFT, m_rect.right/7);
	g_StringLanType(szLan, "��ϵͳ", "SubSystem");
	m_listZoom.InsertColumn(1, szLan, LVCFMT_LEFT, m_rect.right/7);
	g_StringLanType(szLan, "״̬", "Status");
	m_listZoom.InsertColumn(2, szLan, LVCFMT_LEFT, m_rect.right/7);
	g_StringLanType(szLan, "ģ���ַ", "Module Address");
	m_listZoom.InsertColumn(3, szLan, LVCFMT_LEFT, m_rect.right/7);
	g_StringLanType(szLan, "ģ��ͨ��", "Module Channel");
	m_listZoom.InsertColumn(4, szLan, LVCFMT_LEFT, m_rect.right/7);
	g_StringLanType(szLan, "��������", "Zoom Name");
	m_listZoom.InsertColumn(5, szLan, LVCFMT_LEFT, m_rect.right/7);
	g_StringLanType(szLan, "��������", "Zoom Type");
	m_listZoom.InsertColumn(6, szLan, LVCFMT_LEFT, m_rect.right/7);

	
// 	m_listZoom.InsertItem(0,_T("001"));
// 	m_listZoom.SetItemText(0,1,_T("002"));
// 	m_listZoom.SetItemText(0,2,_T("����"));
// 	m_listZoom.SetItemText(0,3,_T("1"));
// 	m_listZoom.SetItemText(0,4,_T("1"));
// 	m_listZoom.SetItemText(0,5,_T("������"));
// 	m_listZoom.SetItemText(0,6,_T("��ģ��"));
// 
// 	m_listZoom.InsertItem(1,_T("002"));
// 	m_listZoom.SetItemText(1,1,_T("002"));
// 	m_listZoom.SetItemText(1,2,_T("����"));
// 	m_listZoom.SetItemText(1,3,_T("2"));
// 	m_listZoom.SetItemText(1,4,_T("2"));
// 	m_listZoom.SetItemText(1,5,_T("����"));
// 	m_listZoom.SetItemText(1,6,_T("8����ģ��"));
	m_iRowCount = m_listZoom.GetItemCount();  //��ȡ����
	
	m_bNeedSave = FALSE; //��ʼ��ΪFLASE;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgAlarmHostZoneAddr::OnDblclkListZoom(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW  *pEditCtrl = (NM_LISTVIEW *)pNMHDR;
	CRect  EditRect;
	//CPoint  point;
	//GetCursorPos(&point);
	//m_list.ScreenToClient(&point);
    
	//LVHITTESTINFO info;
	//info.pt = point;
	//info.flags = LVHT_ABOVE;
	
	
	m_iItem = pEditCtrl->iItem;
	m_iSubItem = pEditCtrl->iSubItem;
	//m_iItem = m_listZoom.SubItemHitTest(&info);
	
	if (m_iItem == -1)
	{
		return;
	}
	if (0==m_iSubItem || 1==m_iSubItem || 2==m_iSubItem || 6==m_iSubItem)
	{
		return;
	}
	//m_iSubItem = info.iSubItem;
	
	if (m_iItem < m_iRowCount)
	{	
		m_listZoom.GetSubItemRect(m_iItem,m_iSubItem,LVIR_LABEL,EditRect);
		m_bNeedSave = TRUE;
// 		if (m_Edit.m_hWnd == NULL)	
// 		{    
// 			//m_Edit.m_hWnd == NULL˵����û����	
// 			m_Edit.Create(ES_AUTOHSCROLL|WS_CHILD|ES_LEFT|ES_WANTRETURN|WS_BORDER,
// 				CRect(0,0,0,0),this,0xffff);//0xffff����Ҫ
// 			
// 			m_Edit.ShowWindow(SW_HIDE); //Edit�����������
// 			m_Edit.SetFont(this->GetFont(),FALSE);//��������
// 		}
		m_Edit.SetParent(&m_listZoom); //��list control����Ϊ������,���ɵ�Edit������ȷ��λ
		
		EditRect.SetRect(EditRect.left,EditRect.top,EditRect.left+m_listZoom.GetColumnWidth(m_iSubItem),EditRect.bottom);
		//m_listZoom.GetColumnWidth(m_iSubItem)��ȡ�еĿ��
		
		//��ѡ�е�list�ؼ��е�����copy��edit�ؼ���
		CString strItem = m_listZoom.GetItemText(m_iItem,m_iSubItem);
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

void CDlgAlarmHostZoneAddr::OnKillfocusEditText() 
{
	// TODO: Add your control notification handler code here
	CString  csEditStr;
	char  szListStr[NAME_LEN];
	m_listZoom.GetItemText(m_iItem, m_iSubItem, szListStr, NAME_LEN);
	m_Edit.GetWindowText(csEditStr); 
	if (csEditStr.Compare(szListStr))
	{
		m_listZoom.SetItemText(m_iItem, m_iSubItem, csEditStr); 
		m_iLastItem = m_iItem;
		SaveZoomInfo();
	}	
	m_Edit.ShowWindow(SW_HIDE);
}

void CDlgAlarmHostZoneAddr::SaveZoomInfo() 
{
	char  szStr[128] = {0};
	//ģ���ַ
	m_listZoom.GetItemText(m_iLastItem, 3, szStr, 128);
	m_lpStruAlarmIn[m_iLastItem].wModuleAddress = atoi(szStr);
	//ģ��ͨ��
	m_listZoom.GetItemText(m_iLastItem, 4, szStr, 128);
	m_lpStruAlarmIn[m_iLastItem].byModuleChan = atoi(szStr);
	//������
	m_listZoom.GetItemText(m_iLastItem, 5, szStr, 128);
	memcpy(m_lpStruAlarmIn[m_iLastItem].byName, szStr, NAME_LEN);
}

void CDlgAlarmHostZoneAddr::OnSelchangeComboSubsystemIndex() 
{
	// TODO: Add your control notification handler code here
	//OnBtnGet();
}

void CDlgAlarmHostZoneAddr::AddZoomInfoToDlg(LPNET_DVR_ALARMIN_PARAM lpInter) 
{
	char szStr[128] = {0};
	//������
	sprintf(szStr, "%3d", lpInter->wZoneIndex+1);
	m_listZoom.InsertItem(m_iRowCount, szStr);
	//��ϵͳ��
	sprintf(szStr, "%3d", lpInter->byJointSubSystem);
	m_listZoom.SetItemText(m_iRowCount, 1, szStr);
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
		g_StringLanType(szStr, "δ֪״̬", "UNKNOWN STATE");
	}
	m_listZoom.SetItemText(m_iRowCount, 2, szStr);
	//ģ���ַ
	if (0xffff == lpInter->wModuleAddress)
	{
		
		g_StringLanType(szStr, "δ֪", "UNKNOWN");
	}
	else
	{
		sprintf(szStr, "%3d", lpInter->wModuleAddress);
	}
	m_listZoom.SetItemText(m_iRowCount, 3, szStr);
	//ģ��ͨ��
	if (0xff == lpInter->byModuleChan)
	{
		g_StringLanType(szStr, "δ֪", "UNKNOWN");
	}
	else
	{
		sprintf(szStr, "%3d", lpInter->byModuleChan);
	}
	m_listZoom.SetItemText(m_iRowCount, 4, szStr);
	//��������
	memset(szStr, 0, sizeof(szStr));
	memcpy(szStr, lpInter->byName, NAME_LEN);
	m_listZoom.SetItemText(m_iRowCount, 5, szStr);
	//ģ������
	if (1 == lpInter->byModuleType)
	{
		g_StringLanType(szStr, "���ط���", "LOCAL ZONE");
	}
	else if (2 == lpInter->byModuleType)
	{
		g_StringLanType(szStr, "������", "SINGLE-ZONE");
	}
	else if (3 == lpInter->byModuleType)
	{
		g_StringLanType(szStr, "˫����", "DOUBLE-ZONE");
	}
	else if (4 == lpInter->byModuleType)
	{
		g_StringLanType(szStr, "8����", "8-ZONE");
	}
	else if (5 == lpInter->byModuleType)
	{
		g_StringLanType(szStr, "8·ģ��������", "8-ANALOG ZONE");
	}
	else if (6 == lpInter->byModuleType)
	{
		g_StringLanType(szStr, "������������", "1-Zone&Trigger");
	}
    else if (7 == lpInter->byModuleType)
    {
        g_StringLanType(szStr, "1�ž͵ؿ�����", "1 door controller");
    }
    else if (8 == lpInter->byModuleType)
    {
        g_StringLanType(szStr, "2�ž͵ؿ�����", "2 doors controller");
    }
    else if (9 == lpInter->byModuleType)
    {
        g_StringLanType(szStr, "4�ž͵ؿ�����", "4 doors controller");
    }
    else if (11 == lpInter->byModuleType)
    {
        g_StringLanType(szStr, "����", "keyboard");
    }
    else if (12 == lpInter->byModuleType)
    {
        g_StringLanType(szStr, "8��������", "8ZoneWired");
    }
    else if (13 == lpInter->byModuleType)
    {
        g_StringLanType(szStr, "��չ����", "extended zone");
    }
	else
	{
		g_StringLanType(szStr, "δ֪����", "UNKNOW TYPE");
	}
	m_listZoom.SetItemText(m_iRowCount++, 6, szStr);
}

void CDlgAlarmHostZoneAddr::OnBtnGet() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	char szLan[128] = {0};
	m_struListInfo.dwSize = sizeof(NET_DVR_LIST_INFO);
	m_struListInfo.byIndex = m_cmSubsystem.GetCurSel() + 1;
	if (m_struAlarmHostAbility.wSubSystemNum+1 == m_struListInfo.byIndex)
	{
		m_struListInfo.byIndex = 0xff;
	}
	m_listZoom.DeleteAllItems();
	m_iRowCount = 0;
	m_iZoomCount = 0;
	if (m_lHandle>=0)
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "It is getting data, wait for a moment");
	}
	else
	{
		m_lHandle = NET_DVR_StartRemoteConfig(m_lUserID, NET_DVR_GET_ALARMHOST_ZONE_LIST_IN_SUBSYSTEM, &m_struListInfo, sizeof(m_struListInfo), NULL, this);
		if (m_lHandle>=0)
		{
			m_bGetNext = TRUE;
			g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "Start Remote Config successfully");
			DWORD dwThreadId;
			m_hGetInfoThread = CreateThread(NULL,0,LPTHREAD_START_ROUTINE(GetZoomListThread),this,0,&dwThreadId);
			m_btnGet.EnableWindow(FALSE);
		}
		else
		{
			g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "Start Remote Config failed");
			return;
		}
	}	
}

void CDlgAlarmHostZoneAddr::OnBtnSet() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	char szStr[128] = {0};
	int iZoomNo = 0;
	if (m_iLastItem == -1)
	{
		g_StringLanType(szStr, "û�в��������ı�", "No Parameter Changed");
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, szStr);
		return;
	}
	m_listZoom.GetItemText(m_iLastItem, 0, szStr, 128);
	iZoomNo = atoi(szStr) - 1;
	if (NET_DVR_SetDVRConfig(m_lUserID, NET_DVR_SET_ALARMIN_PARAM, iZoomNo, &m_lpStruAlarmIn[m_iLastItem], sizeof(NET_DVR_ALARMIN_PARAM)))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_SET_ALARMIN_PARAM");
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_SET_ALARMIN_PARAM");
    }
}

void CDlgAlarmHostZoneAddr::OnCancel()
{
	m_bGetNext = FALSE;
	if(WAIT_TIMEOUT == WaitForSingleObject(m_hGetInfoThread, 3000))
	{
		TerminateThread(m_hGetInfoThread, 0);
	}
	m_hGetInfoThread = NULL;
	CDialog::OnCancel();
}
