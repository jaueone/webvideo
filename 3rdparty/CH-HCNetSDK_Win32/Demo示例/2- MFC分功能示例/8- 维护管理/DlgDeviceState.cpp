/*******************************************************
Copyright 2008-2011 Hikvision Digital Technology Co., Ltd. 
��  ����	DlgDeviceState.cpp
������λ��	���ݺ�������
��  д��	shizhiping
��  �ڣ�	2009.5
��  ����	�豸״̬����ҳ
��  �ģ�	
********************************************************/

#include "stdafx.h"
#include "Manage.h"
#include "DlgDeviceState.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDeviceState dialog


CDlgDeviceState::CDlgDeviceState(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDeviceState::IDD, pParent)
	, m_csSerialNumber(_T(""))
	, m_csDeviceState(_T(""))
	, m_csRecState(_T(""))
	, m_csSignalState(_T(""))
	, m_csHardWareState(_T(""))
	, m_csBitrate(_T(""))
	, m_csDiskVolume(_T(""))
	, m_csDiskFreeSpace(_T(""))
	, m_csDiskState(_T(""))
	, m_csDeviceIP(_T(""))
{
	//{{AFX_DATA_INIT(CDlgDeviceState)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgDeviceState::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDeviceState)
	//	DDX_Text(pDX, IDC_STATIC_DEVNAME, m_csLocalNodeName);
	//	DDX_Text(pDX, IDC_STATIC_DEVSTATE, m_csDeviceState);
	DDX_Control(pDX, IDC_LIST_CHAN_STATE, m_listChanState);
	DDX_Control(pDX, IDC_LIST_DISK_STATE, m_listDiskState);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDeviceState, CDialog)
	//{{AFX_MSG_MAP(CDlgDeviceState)
	ON_BN_CLICKED(ID_BTN_REFRESH, OnBtnRefresh)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDeviceState message handlers

BOOL CDlgDeviceState::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	char szLan[128] = {0};
	g_StringLanType(szLan, "ͨ����", "Chan No.");
	m_listChanState.InsertColumn(0, szLan, LVCFMT_LEFT, 90, -1);
	g_StringLanType(szLan, "¼��״̬", "Record Status");
	m_listChanState.InsertColumn(1, szLan, LVCFMT_LEFT, 100, -1);
	g_StringLanType(szLan, "�ź�״̬", "Signal Status");
	m_listChanState.InsertColumn(2, szLan, LVCFMT_LEFT, 100, -1);
	g_StringLanType(szLan, "Ӳ��״̬", "Hardware Status");
	m_listChanState.InsertColumn(3, szLan, LVCFMT_LEFT, 100, -1);
	g_StringLanType(szLan, "������", "Record Status");
	m_listChanState.InsertColumn(4, szLan, LVCFMT_LEFT, 80, -1);
	g_StringLanType(szLan, "��ǰ����(bps)", "Record Status");
	m_listChanState.InsertColumn(5, szLan,LVCFMT_LEFT,110, -1);
	g_StringLanType(szLan, "IPC������", "Record Status");
	m_listChanState.InsertColumn(6, szLan,LVCFMT_LEFT,80, -1);
	
	g_StringLanType(szLan, "Ӳ�̺�", "HD No.");
	m_listDiskState.InsertColumn(0, szLan, LVCFMT_LEFT, 90, -1);
	g_StringLanType(szLan, "Ӳ������(MB)", "HD Capacity");
	m_listDiskState.InsertColumn(1, szLan, LVCFMT_LEFT, 130, -1);
	g_StringLanType(szLan, "ʣ��ռ�(MB)", "Last Capacity");
	m_listDiskState.InsertColumn(2, szLan, LVCFMT_LEFT, 130, -1);
	g_StringLanType(szLan, "Ӳ��״̬", "HD Status");
	m_listDiskState.InsertColumn(3, szLan, LVCFMT_LEFT,250,-1);
	
	m_listChanState.SetExtendedStyle(m_listChanState.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_listDiskState.SetExtendedStyle(m_listDiskState.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgDeviceState::UpdateDeviceInfo()
{
	OnBtnRefresh();
}

/*************************************************
������:    	OnBtnRefresh
��������:	ˢ���豸״̬
�������:   
�������:   		
����ֵ:	   
**************************************************/
void CDlgDeviceState::OnBtnRefresh() 
{
	int i = 0;
	int iIndex = 0;
	int iTotalLen = 0;
	CString strDiskNum = _T("");
	CString strChanNum = _T("");
	CString strTemp = _T("");
	char szLan[1024] = {0};

	m_listChanState.DeleteAllItems();
	m_listDiskState.DeleteAllItems();

	NET_DVR_WORKSTATE_V30 struWorkState;
	memset(&struWorkState, 0, sizeof(NET_DVR_WORKSTATE_V30));
	if (!NET_DVR_GetDVRWorkState_V30(g_pMainDlg->m_struDeviceInfo.lLoginID, &struWorkState))
	{
		g_StringLanType(szLan, "��ȡ�豸״̬ʧ��!", "Get Device Status Err!");
		AfxMessageBox(szLan);
		return;
	}

	switch (struWorkState.dwDeviceStatic) 
	{
	case 0:
		g_StringLanType(szLan, "����", "Normal");
		m_csDeviceState.Format(szLan);
		break;
	case 1:
		g_StringLanType(szLan, "CPUռ����̫��,����85%", "CPU occupancy rate is too high");
		m_csDeviceState.Format(szLan);
		break;
	case 2:
		g_StringLanType(szLan, "Ӳ������", "Hardware error");
		m_csDeviceState.Format(szLan);
		break;
	default:
		ASSERT(struWorkState.dwDeviceStatic < 3);
		break;
	}

	GetDlgItem(IDC_STATIC_DEVSTATE)->SetWindowText(m_csDeviceState);

	if (0 == struWorkState.byAudioChanStatus[0])
	{
		//m_csSignalState.Format("δʹ��");
	}
	else if (1 == struWorkState.byAudioChanStatus[0])
	{
		//m_csSignalState.Format("��ʹ��");
	}

	//��ص�״̬
	for (i = 0; i < MAX_CHANNUM_V30; i++)  //
	{
		if (!g_pMainDlg->m_struDeviceInfo.struChanInfo[i].bEnable)
		{
			continue;
		}

		if (i < g_pMainDlg->m_struDeviceInfo.iDeviceChanNum)
		{
			strChanNum.Format("Camera %d", i+g_pMainDlg->m_struDeviceInfo.iStartChan);	
		}
		else if ( i>=MAX_ANALOG_CHANNUM)
		{
			strChanNum.Format("IPCamera %d", i+g_pMainDlg->m_struDeviceInfo.iStartChan-MAX_ANALOG_CHANNUM);	
		}

		if (0 == struWorkState.struChanStatic[i].byRecordStatic)
		{
			g_StringLanType(szLan, "��¼��", "Not Record");
			m_csRecState.Format(szLan);
		}
		else if (1 == struWorkState.struChanStatic[i].byRecordStatic)
		{
			g_StringLanType(szLan, "¼��", "Record");
			m_csRecState.Format(szLan);
		}

		if (0 == struWorkState.struChanStatic[i].bySignalStatic)
		{
			g_StringLanType(szLan, "����", "Normal");
			m_csSignalState.Format(szLan);
		}
		else if (1 == struWorkState.struChanStatic[i].bySignalStatic)
		{
			g_StringLanType(szLan, "�źŶ�ʧ", "Signal loss");
			m_csSignalState.Format(szLan);
		}

		if ( 0 == struWorkState.struChanStatic[i].byHardwareStatic)
		{
			g_StringLanType(szLan, "����", "Normal");
			m_csHardWareState.Format(szLan);
		}
		else if (1 == struWorkState.struChanStatic[i].byHardwareStatic)
		{
			g_StringLanType(szLan, "�쳣", "Abnormal");
			m_csHardWareState.Format(szLan);
		}

		m_csLinkCount.Format("%d", struWorkState.struChanStatic[i].dwLinkNum);
		iTotalLen+=struWorkState.struChanStatic[i].dwLinkNum;
		m_csBitrate.Format("%ld", struWorkState.struChanStatic[i].dwBitRate);
		//strTemp.Format("%d", );
		m_listChanState.InsertItem(iIndex, strChanNum, 0);
		m_listChanState.SetItemText(iIndex, 1, m_csRecState);
		m_listChanState.SetItemText(iIndex, 2, m_csSignalState);
		m_listChanState.SetItemText(iIndex, 3, m_csHardWareState);
		m_listChanState.SetItemText(iIndex, 4, m_csLinkCount);
		m_listChanState.SetItemText(iIndex, 5, m_csBitrate);
		strTemp.Format("%d", struWorkState.struChanStatic[i].dwIPLinkNum);
		m_listChanState.SetItemText(iIndex, 6, strTemp);
		iIndex++;
	}

	//Ӳ��״̬
	for (i = 0; i < MAX_DISKNUM_V30; i++)
	{
		g_StringLanType(szLan, "Ӳ��", "HD");
		strDiskNum.Format("%s%02d", szLan, i+1);

		m_csDiskVolume.Format("%ld", struWorkState.struHardDiskStatic[i].dwVolume);
		m_csDiskFreeSpace.Format("%ld", struWorkState.struHardDiskStatic[i].dwFreeSpace);

		if (struWorkState.struHardDiskStatic[i].dwVolume != 0)
		{
			switch (struWorkState.struHardDiskStatic[i].dwHardDiskStatic) 
			{
			case 0:
				g_StringLanType(szLan, "�", "Active");
				m_csDiskState.Format(szLan);
				break;
			case 1:
				g_StringLanType(szLan, "����", "Dormancy");
				m_csDiskState.Format(szLan);
				break;
			case 2:
				g_StringLanType(szLan, "������", "Abnormal");
				m_csDiskState.Format(szLan);
				m_csDiskState.Format("%s", "������");
				break;
// 			case 3:		
// 				g_StringLanType(szLan, "�����Ҳ�����", "Dormancy and Abnormal");
// 				m_csDiskState.Format(szLan);
// 				break;
			default:
				ASSERT(struWorkState.struHardDiskStatic[i].dwHardDiskStatic >= 0 &&\
					struWorkState.struHardDiskStatic[i].dwHardDiskStatic < 4);
				break;
			}
		}else
		{
			m_csDiskState.Format("");
		}

		strTemp.Format("%d",i);
		m_listDiskState.InsertItem(i,strDiskNum,0);
		m_listDiskState.SetItemText(i,1,m_csDiskVolume);
		m_listDiskState.SetItemText(i,2,m_csDiskFreeSpace);
		m_listDiskState.SetItemText(i,3,m_csDiskState);
	}

	strTemp.Format("%d",iTotalLen);
	GetDlgItem(IDC_STATIC_TOTAL_LINK)->SetWindowText(strTemp);

	UpdateData(false);
	
}
