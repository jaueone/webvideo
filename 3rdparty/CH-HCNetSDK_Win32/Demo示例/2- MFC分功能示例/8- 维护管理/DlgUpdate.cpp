/*******************************************************
Copyright 2008-2011 Hikvision Digital Technology Co., Ltd. 
��  ����	DlgUpdate.cpp
������λ��	���ݺ�������
��  д��	shizhiping
��  �ڣ�	2009.5
��  ����	Զ������
��  �ģ�	
********************************************************/

#include "stdafx.h"
#include "Manage.h"
#include "DlgUpdate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgUpdate dialog


CDlgUpdate::CDlgUpdate(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgUpdate::IDD, pParent)
	, m_csUpgradeFile(_T(""))
	, m_csUpgradeStat(_T(""))
	, m_lUpgradeHandle(0)
	, m_lServerID(-1)
	, m_lpUpgradeTimer(NULL)
	, m_bUpgrade(FALSE)
{
	//{{AFX_DATA_INIT(CDlgUpdate)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgUpdate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgUpdate)
	DDX_Control(pDX, IDC_COMBO_ENVIRONMENT, m_comboEnvironment);
	DDX_Control(pDX, IDC_PROGRESS_UPGRADE, m_progressUpgrade);
	DDX_Text(pDX, IDC_EDIT_UPGRADE_FILE, m_csUpgradeFile);
	DDX_Text(pDX, IDC_STATIC_UPGRADE, m_csUpgradeStat);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgUpdate, CDialog)
	//{{AFX_MSG_MAP(CDlgUpdate)
	ON_BN_CLICKED(IDC_BTN_SET_ENVIRO, OnBtnSetEnviro)
	ON_BN_CLICKED(IDC_BTN_BROWSE_FILE, OnBtnBrowseFile)
	ON_BN_CLICKED(IDC_BTN_UPGRADE_EXIT, OnBtnUpgradeExit)
	ON_BN_CLICKED(IDC_BTN_UPGRADE, OnBtnUpgrade)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgUpdate message handlers

/*************************************************
������:    	OnBtnSetEnviro
��������:	�������绷��
�������:   
�������:   	
����ֵ:	   
*************************************************/
void CDlgUpdate::OnBtnSetEnviro() 
{
	if (!NET_DVR_SetNetworkEnvironment(m_comboEnvironment.GetCurSel()))
	{
		MessageBox("�������绷��ʧ��");
	}

	
}

/*************************************************
������:    	OnInitDialog
��������:	��ʼ���Ի���
�������:   
�������:   	
����ֵ:	   
*************************************************/
BOOL CDlgUpdate::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_csUpgradeFile.Format("c:\\digicap");
	UpdateData(FALSE);
	m_bUpgrade = FALSE;
	m_progressUpgrade.SetRange(0,100);
	m_progressUpgrade.SetPos(0);
	m_progressUpgrade.ShowWindow(SW_HIDE);
	m_comboEnvironment.SetCurSel(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*************************************************
������:    	OnBtnBrowseFile
��������:	��������ļ�
�������:   
�������:   	
����ֵ:	   
*************************************************/
void CDlgUpdate::OnBtnBrowseFile() 
{
	CFileDialog FileDlg(TRUE,NULL,"digicap",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"All Files(*.*)|*.*||");
	if(IDOK == FileDlg.DoModal())
	{
		m_csUpgradeFile = FileDlg.GetPathName();
		UpdateData(FALSE);
	}
	
}

/*************************************************
������:    	OnBtnUpgradeExit
��������:	�˳�
�������:   
�������:   	
����ֵ:	   
*************************************************/
void CDlgUpdate::OnBtnUpgradeExit() 
{
	if (m_lpUpgradeTimer)
	{
		KillTimer(UPGRADE_TIMER);
	}
	CDialog::OnCancel();
	
}

/*************************************************
������:    	OnBtnUpgrade
��������:	����
�������:   
�������:   	
����ֵ:	   
*************************************************/
void CDlgUpdate::OnBtnUpgrade() 
{
	LONG lLoginID = g_pMainDlg->m_struDeviceInfo.lLoginID;
	if(lLoginID == -1)
	{
		MessageBox("����ע��");
	}

	UpdateData(TRUE);
	char m_szFileName[256]={0};
	strcpy(m_szFileName,m_csUpgradeFile);
	CFile cFile;
	char szLan[128] = {0};
	if (!cFile.Open(m_szFileName,NULL))
	{
		g_StringLanType(szLan, "���ļ�ʧ�ܻ��޴��ļ�", "Open file failed or no this file");
		AfxMessageBox(szLan);
		return;
	}
	DWORD dwFileSize = (DWORD)cFile.GetLength();
	if (dwFileSize == 0)
	{
		g_StringLanType(szLan, "�����ļ�Ϊ��", "Upgrade file is empty");
		AfxMessageBox(szLan);
	}
	cFile.Close();

    if(IDYES != MessageBox("ȷ��Ҫ�����豸��","Warning",MB_YESNO))
	{
		return;
	}

    m_lUpgradeHandle = NET_DVR_Upgrade(lLoginID,m_szFileName);
	if(m_lUpgradeHandle == -1)
	{
          MessageBox("����ʧ��");
	      return;

	}

	GetDlgItem(IDC_STATIC_UPGRADE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BTN_UPGRADE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_UPGRADE_EXIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_BROWSE_FILE)->EnableWindow(FALSE);
	m_progressUpgrade.SetPos(0);
	m_progressUpgrade.ShowWindow(SW_SHOW);

	m_csUpgradeStat = "״̬��������������������ȴ�......";
	m_bUpgrade = TRUE;
    
	m_lpUpgradeTimer = SetTimer(UPGRADE_TIMER,500,NULL);
	UpdateData(FALSE);
	

}

/*************************************************
������:    	OnTimer
��������:	��ʱ����������ȡ����״̬
�������:   
�������:   	
����ֵ:	   
*************************************************/
void CDlgUpdate::OnTimer(UINT nIDEvent) 
{
	
	char szLan[128] = {0};
	if (nIDEvent == UPGRADE_TIMER)
	{
		if (m_bUpgrade)
		{
			int UpgradeStatic = NET_DVR_GetUpgradeState(m_lUpgradeHandle);
			int iPos = NET_DVR_GetUpgradeProgress(m_lUpgradeHandle);
			if (iPos >0)
			{
				m_progressUpgrade.SetPos(iPos);
			}
			if (UpgradeStatic == 2)
			{
				g_StringLanType(szLan, "״̬�����������豸����ȴ�......", "Status: Device is upgrading, please wait......");
				m_csUpgradeStat.Format(szLan);
				UpdateData(FALSE);
			}
			else
			{
				switch (UpgradeStatic)
				{
				case -1:
					g_StringLanType(szLan, "����ʧ��", "Upgrade failed");
					AfxMessageBox(szLan);			
					break;
				case 1:
					g_StringLanType(szLan, "״̬�������豸�ɹ�", "Status:upgrade successfully");
					m_csUpgradeStat.Format(szLan);
					m_progressUpgrade.SetPos(100);
					break;			
				case 3:
					g_StringLanType(szLan, "״̬�������豸ʧ��", "Status:upgrade failed");
					m_csUpgradeStat.Format(szLan);			
					break;
				case 4:
					g_StringLanType(szLan, "״̬������Ͽ�,״̬δ֪", "Status:Network disconnect, status unknown");
					m_csUpgradeStat.Format(szLan);					
					break;
				case 5:
					g_StringLanType(szLan, "״̬�������ļ����԰汾��ƥ��", "Status:Upgrade file language mismatch");
					m_csUpgradeStat.Format(szLan);				
					break;
				default: 
					break;
				}
				UpdateData(FALSE);
				m_bUpgrade = FALSE;
				GetDlgItem(IDC_BTN_UPGRADE)->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_UPGRADE_EXIT)->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_BROWSE_FILE)->EnableWindow(TRUE);
				NET_DVR_CloseUpgradeHandle(m_lUpgradeHandle);
			}
		}		
	}
	CDialog::OnTimer(nIDEvent);
}
