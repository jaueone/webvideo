/*******************************************************
Copyright 2008-2011 Hikvision Digital Technology Co., Ltd. 
��  ����	DlgFormatHD.cpp
������λ��	���ݺ�������
��  д��	shizhiping
��  �ڣ�	2009.5
��  ����	Ӳ�̸�ʽ���Ի���
��  �ģ�	
********************************************************/
#include "stdafx.h"
#include "Manage.h"
#include "DlgFormatHD.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFormatHD dialog


CDlgFormatHD::CDlgFormatHD(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFormatHD::IDD, pParent)
	, m_csFormatStat(_T(""))
	, m_lFormatAll(-1)
	, m_nTime(0)
	, m_bFormatAll(FALSE)
	, m_dwDiskNum(0)
	, m_lDiskSel(0)
	, m_iDiskIndex(0)
{
	//{{AFX_DATA_INIT(CDlgFormatHD)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	for (int i = 0; i<MAX_DISKNUM_V30; i++)
	{
		m_lFormat[i] = -1; 
		m_bFormat[i] = FALSE;
	}
}


void CDlgFormatHD::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFormatHD)
	DDX_Control(pDX, IDC_COMBO_HD_DRIVER, m_comboHDDriver);
	DDX_Text(pDX, IDC_STATIC_FORMAT, m_csFormatStat);
	DDX_Control(pDX, IDC_PROGRESS_FORMAT, m_progressFormat);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFormatHD, CDialog)
	//{{AFX_MSG_MAP(CDlgFormatHD)
	ON_BN_CLICKED(IDC_BTN_DO_FORMAT, OnBtnDoFormat)
	ON_BN_CLICKED(IDC_BTN_FORMAT_EXIT, OnBtnFormatExit)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFormatHD message handlers


/*************************************************
������:    	OnBtnDoFormat
��������:	��ʽ��Ӳ��
�������:   
�������:   		
����ֵ:	   
**************************************************/
void CDlgFormatHD::OnBtnDoFormat() 
{

	if(IDYES != MessageBox("ȷ��Ҫ��ʽ��Ӳ��?","Warning",MB_YESNO))
	{
		return;
	}

	LONG lDiskSel;
	char szLan[1024] = {0};
	lDiskSel = m_comboHDDriver.GetCurSel();

	if (lDiskSel == 0)
	{
		m_lFormatAll = NET_DVR_FormatDisk(g_pMainDlg->m_struDeviceInfo.lLoginID, 0xFF);//0xFF��ʾ����Ӳ��
		if (m_lFormatAll < 0)
		{
			int Err = GetLastError();
			if (Err == NET_DVR_DISK_FORMATING)
			{
				g_StringLanType(szLan, "Ӳ�����ڸ�ʽ��,������������", "Hard disk is formatted, can not start operation");
				AfxMessageBox(szLan);
			}
			else
			{
				g_StringLanType(szLan, "��ʽ��ʧ��", "Format fail");
				AfxMessageBox(szLan);
			}
		}
		else
		{
			g_StringLanType(szLan, "״̬�����ڸ�ʽ��Ӳ�̣���ȴ�......", "Status:Is to reformat your hard disk, please wait");		
			m_csFormatStat.Format(szLan);
			m_bFormatAll = TRUE;
			GetDlgItem(IDC_BTN_DO_FORMAT)->EnableWindow(FALSE);
		}	
	}
	else
	{
		m_iDiskIndex = m_comboHDDriver.GetItemData(lDiskSel);
		m_lFormat[m_iDiskIndex] = NET_DVR_FormatDisk(g_pMainDlg->m_struDeviceInfo.lLoginID, m_iDiskIndex);
		if (m_lFormat[m_iDiskIndex] < 0)
		{
			g_StringLanType(szLan, "��ʽ��ʧ��", "Format fail");
			AfxMessageBox(szLan);
		}
		else
		{
			g_StringLanType(szLan, "״̬�����ڸ�ʽ��Ӳ�̣���ȴ�......", "Status:Is to reformat your hard disk, please wait");		
			m_csFormatStat.Format(szLan);
			m_bFormat[m_iDiskIndex] = TRUE;			
		}
	}

	UpdateData(FALSE);
	
}

/*************************************************
������:    	OnBtnFormatExit
��������:	�˳�
�������:   
�������:   		
����ֵ:	   
**************************************************/
void CDlgFormatHD::OnBtnFormatExit() 
{
	if (m_bFormatAll)
	{
		m_bFormatAll = FALSE;
		NET_DVR_CloseFormatHandle(m_lFormatAll);
	}
	else
	{
		for (int i=0; i<MAX_DISKNUM_V30; i++)
		{
			if (m_bFormat[i])
			{
				m_bFormat[i] = FALSE;
				NET_DVR_CloseFormatHandle(m_lFormat[i]);
			}
		}
	}
	if (m_nTime)
	{
		KillTimer(FORMAT_TIMER);
	}
	CDialog::OnCancel();
}

/*************************************************
������:    	OnInitDialog
��������:	��ʼ���Ի��򣬻�ȡӲ����Ϣ
�������:   
�������:   		
����ֵ:	   
**************************************************/
BOOL CDlgFormatHD::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	NET_DVR_HDCFG struHDCfg;
	CString csTemp;
	int i = 0;
	DWORD dwReturned = 0;
	char szLan[1024] = {0};
	
	if (!NET_DVR_GetDVRConfig(g_pMainDlg->m_struDeviceInfo.lLoginID, NET_DVR_GET_HDCFG, 0, &struHDCfg, sizeof(NET_DVR_HDCFG), &dwReturned))
	{
		g_StringLanType(szLan, "ȫ��Ӳ��", "All Hard Disk");
		m_comboHDDriver.AddString(szLan);
		for (i=0; i<(int)m_dwDiskNum; i++)
		{
			g_StringLanType(szLan, "Ӳ��", "HD");
			csTemp.Format("%s%d", szLan, i);
			m_comboHDDriver.AddString(csTemp);
			m_comboHDDriver.SetItemData(i+1, i);
			m_bFormat[i] = FALSE;
			m_lFormat[i] = -1;
		}		
	}
	else
	{
		g_StringLanType(szLan, "ȫ��Ӳ��", "All Hard Disk");
		m_comboHDDriver.AddString(szLan);
		m_comboHDDriver.SetItemData(0, 0xff);
		for (i=0; i<(int)struHDCfg.dwHDCount; i++)
		{
			g_StringLanType(szLan, "Ӳ��", "HD");
			csTemp.Format("%s%d", szLan, struHDCfg.struHDInfo[i].dwHDNo);
			m_comboHDDriver.AddString(csTemp);
			m_comboHDDriver.SetItemData(i+1, struHDCfg.struHDInfo[i].dwHDNo);
			m_bFormat[i] = FALSE;
			m_lFormat[i] = -1;
		}
	}
	m_lDiskSel = 0;
	
	m_comboHDDriver.SetCurSel(m_lDiskSel);
	GetDlgItem(IDC_STATIC_FORMAT)->ShowWindow(SW_SHOW);
	m_progressFormat.SetRange(0,100);
	m_progressFormat.SetPos(0);
	m_nTime = SetTimer(FORMAT_TIMER, 5000, NULL);
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*************************************************
������:    	OnTimer
��������:	��ʱ����������ȡӲ�̸�ʽ������
�������:   
�������:   		
����ֵ:	   
**************************************************/
void CDlgFormatHD::OnTimer(UINT nIDEvent) 
{
	LONG lCurDisk;
	LONG lCurDiskPos;
	LONG lFormatStatic;
	CString sTemp;
	char szLan[1024] = {0};
	
	if (nIDEvent == FORMAT_TIMER)
	{
		if (m_bFormatAll)
		{
			if (NET_DVR_GetFormatProgress(m_lFormatAll, &lCurDisk, &lCurDiskPos, &lFormatStatic))
			{
				switch (lFormatStatic) 
				{
				case 0:	
					g_StringLanType(szLan, "״̬�����ڸ�ʽ��Ӳ��", "Status: formatting hard disk");		
					m_csFormatStat.Format("%s%d", szLan, lCurDisk);
					break;
				case 1:
					g_StringLanType(szLan, "״̬��Ӳ�̸�ʽ���ɹ�", "Status: format hard disk successfully");	
					m_csFormatStat.Format(szLan);					
					GetDlgItem(IDC_BTN_DO_FORMAT)->EnableWindow(TRUE);
					m_bFormatAll = FALSE;
					lCurDiskPos = 100;
					NET_DVR_CloseFormatHandle(m_lFormatAll);
					break;			
				case 2:	
					g_StringLanType(szLan, "״̬����ʽ���쳣,״̬δ֪, Ӳ��", "Status: Unusual format, status unknown, HD");	
					m_csFormatStat.Format("%s%d", szLan, lCurDisk);				
					GetDlgItem(IDC_BTN_DO_FORMAT)->EnableWindow(TRUE);
					m_bFormatAll = FALSE;
					NET_DVR_CloseFormatHandle(m_lFormatAll);
					break;
				default:
					break;
				}
				m_progressFormat.SetPos(lCurDiskPos);
			}
		}
		for (int i=0; i<MAX_DISKNUM_V30; i++)
		{
			if (m_bFormat[i])
			{
				if (NET_DVR_GetFormatProgress(m_lFormat[i], &lCurDisk, &lCurDiskPos, &lFormatStatic))
				{
					switch (lFormatStatic) 
					{
					case 0:	
						g_StringLanType(szLan, "״̬�����ڸ�ʽ��Ӳ��", "Status: formatting hard disk");		
						m_csFormatStat.Format("%s%d", szLan, lCurDisk);
						break;
					case 1:
						g_StringLanType(szLan, "״̬��Ӳ�̸�ʽ���ɹ�", "Status: format hard disk successfully");	
						m_csFormatStat.Format(szLan);
						m_bFormat[i] = FALSE;
						lCurDiskPos = 100;
						NET_DVR_CloseFormatHandle(m_lFormat[i]);
						break;			
					case 2:	
						g_StringLanType(szLan, "״̬����ʽ���쳣,״̬δ֪, Ӳ��", "Status: Unusual format, status unknown, HD");	
						m_csFormatStat.Format("%s%d", szLan, lCurDisk);	
						m_bFormat[i] = FALSE;
						NET_DVR_CloseFormatHandle(m_lFormat[i]);
						break;
					default:
						break;
					}
					m_progressFormat.SetPos(lCurDiskPos);
				}
			}
		}
	}
	UpdateData(FALSE);
	
	CDialog::OnTimer(nIDEvent);
}
