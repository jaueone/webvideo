/**********************************************************
FileName:    DlgUpgrade.cpp
Description: update dialogbox     
Date:        2008/05/17
Note: 		<Global>struct, macro refer to GeneralDef.h, global variants and API refer to ClientDemo.cpp   
Modification History:      
    <version> <time>         <desc>
    <1.0    > <2008/05/17>       <created>
***********************************************************/

#include "stdafx.h"
#include "ClientDemo.h"
#include "DlgUpgrade.h"
#include ".\dlgupgrade.h"


// CDlgUpgrade dialog
/*********************************************************
  Function:	CDlgUpgrade
  Desc:		Constructor
  Input:	
  Output:	
  Return:	
**********************************************************/
IMPLEMENT_DYNAMIC(CDlgUpgrade, CDialog)
CDlgUpgrade::CDlgUpgrade(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgUpgrade::IDD, pParent)
	, m_csUpgradeFile(_T(""))
	, m_csUpgradeStat(_T(""))
	, m_lUpgradeHandle(0)
	, m_lServerID(-1)
	, m_lChannel(-1)
	, m_lpUpgradeTimer(NULL)
	, m_bUpgrade(FALSE)
    , m_dwAcsNo(0)
{
}

/*********************************************************
  Function:	~CDlgUpgrade
  Desc:		destructor
  Input:	
  Output:	
  Return:	
**********************************************************/
CDlgUpgrade::~CDlgUpgrade()
{
}

/*********************************************************
Function:	DoDataExchange
Desc:		the map between control and variable
Input:	
Output:	
Return:	
**********************************************************/
void CDlgUpgrade::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgUpgrade)
	DDX_Control(pDX, IDC_PROGRESS_STEP, m_progressSub);
	DDX_Control(pDX, IDC_COMBO_CHAN, m_comboChan);
	DDX_Control(pDX, IDC_COMBO_UPGRADE_TYPE, m_comboUpgradeType);
	DDX_Control(pDX, IDC_COMBO_ENVIRONMENT, m_comboEnvironment);
	DDX_Control(pDX, IDC_PROGRESS_UPGRADE, m_progressUpgrade);
	DDX_Control(pDX, IDC_COMBO_CARD_TYPE, m_comboCardType);
    DDX_Control(pDX, IDC_COMBO_AUX_DEV, m_comboAuxDev);
	DDX_Text(pDX, IDC_EDIT_UPGRADE_FILE, m_csUpgradeFile);
	DDX_Text(pDX, IDC_STATIC_UPGRADE, m_csUpgradeStat);
	DDX_Text(pDX, IDC_STATIC_STEP, m_csUpgradeStep);
	DDX_Check(pDX, IDC_CHK_FUZZYUPGRADE, m_bFuzzyUpgrade);
    DDX_Text(pDX, IDC_EDIT_ACS_NO, m_dwAcsNo);
    DDX_Text(pDX, IDC_EDT_UNIT_ID, m_csUnitID);
    //}}AFX_DATA_MAP
}


/*********************************************************
Function:	BEGIN_MESSAGE_MAP
Desc:		the map between control and function
Input:	
Output:	
Return:	
**********************************************************/
BEGIN_MESSAGE_MAP(CDlgUpgrade, CDialog)
	//{{AFX_MSG_MAP(CDlgUpgrade)	
	ON_BN_CLICKED(IDC_BTN_BROWSE_FILE, OnBnClickedBtnBrowseFile)
	ON_BN_CLICKED(IDC_BTN_UPGRADE, OnBnClickedBtnUpgrade)
	ON_BN_CLICKED(IDC_BTN_UPGRADE_EXIT, OnBnClickedBtnUpgradeExit)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_SET_ENVIRO, OnBtnSetEnviro)
	ON_CBN_SELCHANGE(IDC_COMBO_UPGRADE_TYPE, OnSelchangeComboUpgradeType)
	ON_BN_CLICKED(IDC_BTN_GETUPGRADEINFO, OnBtnGetupgradeinfo)
	//}}AFX_MSG_MAP
	
END_MESSAGE_MAP()


// CDlgUpgrade message handlers
/*********************************************************
Function:	OnInitDialog
Desc:		Initialize the dialog
Input:	
Output:	
Return:	
**********************************************************/
BOOL CDlgUpgrade::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	m_csUpgradeFile.Format("c:\\digicap");
	UpdateData(FALSE);
	m_bUpgrade = FALSE;
	m_progressUpgrade.SetRange(0,100);
	m_progressUpgrade.SetPos(0);
	m_progressUpgrade.ShowWindow(SW_HIDE);
	m_comboEnvironment.SetCurSel(0);

    m_comboUpgradeType.ResetContent();
    m_comboUpgradeType.AddString("DVR");
    m_comboUpgradeType.AddString("Adapter");
    m_comboUpgradeType.AddString("Vca lib");
    m_comboUpgradeType.AddString("ACS");
	m_comboUpgradeType.AddString("IDS");
	m_comboUpgradeType.AddString("LED");
    m_comboUpgradeType.AddString("Intelligent");
    m_comboUpgradeType.SetCurSel(0);
	OnSelchangeComboUpgradeType();
    
    m_comboAuxDev.ResetContent();
    m_comboAuxDev.AddString("Keyboard");
    m_comboAuxDev.AddString("Movement");
	m_comboAuxDev.AddString("NetModule");
    m_comboAuxDev.AddString("Router");
    m_comboAuxDev.AddString("Zone");
    m_comboAuxDev.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

/*********************************************************
  Function:	OnBnClickedBtnBrowseFile
  Desc:		browse update firmware
  Input:	
  Output:	
  Return:	
**********************************************************/
void CDlgUpgrade::OnBnClickedBtnBrowseFile()
{
	UpdateData(TRUE);
	if (m_bFuzzyUpgrade)
	{
		CString Str = F_GetDirectoryPath();
		if(Str != "")
		{
			m_csUpgradeFile = Str;
			UpdateData(FALSE);
		}
	} 
	else
	{
		static char szFilter[]="All File(*.*)|*.*||";
		CFileDialog dlg(TRUE,"*.*","digicap",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
			szFilter);
		if (dlg.DoModal()==IDOK)
		{
			m_csUpgradeFile = dlg.GetPathName();
			UpdateData(FALSE);
		}
	}	
}

CString CDlgUpgrade::F_GetDirectoryPath()
{
    LPITEMIDLIST pidlRoot = NULL;
    SHGetSpecialFolderLocation(m_hWnd, CSIDL_DRIVES, &pidlRoot);
    BROWSEINFO bi;   //���봫��Ĳ���,�����������ṹ�Ĳ����ĳ�ʼ��
    CString strDisplayName;   //�����õ�,��ѡ��Ļ�ҳ��·��,�൱���ṩһ��������
    bi.hwndOwner = GetSafeHwnd();   //�õ�������Handleֵ
    bi.pidlRoot = pidlRoot;   //���������������������õ���.
    bi.pszDisplayName = strDisplayName.GetBuffer(MAX_PATH + 1);   //�õ�������ָ��
    bi.lpszTitle = "�ļ���";   //���ñ���
    bi.ulFlags = BIF_RETURNONLYFSDIRS;   //���ñ�־
    bi.lpfn = NULL;
    bi.lParam = 0;
    bi.iImage = 0;   //���������һЩ�޹صĲ���������,�����������,
    LPITEMIDLIST lpIDList = SHBrowseForFolder(&bi);	//�򿪶Ի���
    strDisplayName.ReleaseBuffer();   //�������GetBuffer()���Ӧ
    char pPath[MAX_PATH];
    CString Str;
    if(lpIDList)
    {
        SHGetPathFromIDList(lpIDList, pPath);
        Str = pPath;
    }
	
    return Str;
}

/*********************************************************
  Function:	start update
  Input:	
  Output:	
  Return:	
**********************************************************/
void CDlgUpgrade::OnBnClickedBtnUpgrade()
{
	char m_szFileName[MAX_PATH];

	UpdateData(TRUE);
	strcpy(m_szFileName,m_csUpgradeFile);
    char szLan[128] = {0};
	/*
	CFile cFile;
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
	*/
     
    if (m_comboUpgradeType.GetCurSel() == 0) // DVR upgrade 
    {
	    m_lUpgradeHandle = NET_DVR_Upgrade(m_lServerID, m_szFileName);
    }
    else if (m_comboUpgradeType.GetCurSel() == 1) // adapter upgrade
    {
        m_lUpgradeHandle = NET_DVR_AdapterUpgrade(m_lServerID, m_szFileName);
    }
    else if (m_comboUpgradeType.GetCurSel() == 2) // vca lib upgrade
    {
        m_lUpgradeHandle = NET_DVR_VcalibUpgrade(m_lServerID, m_comboChan.GetCurSel() + 1, m_szFileName);
    }
    else if (m_comboUpgradeType.GetCurSel() == 3)
    {
        DWORD dwDevNo = m_dwAcsNo;
        m_lUpgradeHandle = NET_DVR_Upgrade_V40(m_lServerID, ENUM_UPGRADE_ACS, m_szFileName, &dwDevNo, sizeof(dwDevNo));
    }
	else if (m_comboUpgradeType.GetCurSel() == 4)
	{
		NET_DVR_AUXILIARY_DEV_UPGRADE_PARAM struAuxiliaryDevUpgradeParam = {0};
		struAuxiliaryDevUpgradeParam.dwSize = sizeof(struAuxiliaryDevUpgradeParam);
		//struAuxiliaryDevUpgradeParam.byDevType = 0;//Ŀǰ��Ƶ�������������豸����ֻ�м���
        struAuxiliaryDevUpgradeParam.byDevType = m_comboAuxDev.GetCurSel();
		struAuxiliaryDevUpgradeParam.dwDevNo = m_comboChan.GetCurSel();
		m_lUpgradeHandle = NET_DVR_Upgrade_V40(m_lServerID, ENUM_UPGRADE_AUXILIARY_DEV, m_szFileName, &struAuxiliaryDevUpgradeParam, sizeof(struAuxiliaryDevUpgradeParam));
	}
	else if (m_comboUpgradeType.GetCurSel() == 5)
	{
		DWORD dwCardType = m_comboCardType.GetCurSel()+1;
		m_lUpgradeHandle = NET_DVR_Upgrade_V40(m_lServerID, ENUM_UPGRADE_LED, m_szFileName, &dwCardType, sizeof(dwCardType));
	}
    else if (m_comboUpgradeType.GetCurSel() == 6)
    {
        DWORD dwCardType = m_comboCardType.GetCurSel() + 1;
        NET_DVR_UPGRADE_PARAM struUpgradeParam = { 0 };
        struUpgradeParam.dwUpgradeType = ENUM_UPGRADE_INTELLIGENT;
        struUpgradeParam.sFileName = m_szFileName;
        char szUnitID[128] = { 0 };
        strncpy(szUnitID, (char*)m_csUnitID.GetBuffer(), 128);

        struUpgradeParam.pUnitIdList[0] = szUnitID;// "829282394af74ffca1a11d3d5c68e29b";
        m_lUpgradeHandle = NET_DVR_Upgrade_V50(m_lServerID, &struUpgradeParam);
    }

	if (m_lUpgradeHandle < 0)
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_Upgrade");
		g_StringLanType(szLan, "����ʧ��", "Upgrade failed");
		AfxMessageBox(szLan);
	}
	else
	{
		GetDlgItem(IDC_STATIC_UPGRADE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_UPGRADE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_UPGRADE_EXIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_BROWSE_FILE)->EnableWindow(FALSE);
		m_progressUpgrade.SetPos(0);
		m_progressUpgrade.ShowWindow(SW_SHOW);
		g_StringLanType(szLan, "״̬��������������������ȴ�......", "Status: Server is upgrading, please wait......");
		m_csUpgradeStat.Format(szLan);
		m_bUpgrade = TRUE;
		m_lpUpgradeTimer = SetTimer(UPGRADE_TIMER, 500, NULL);
		UpdateData(FALSE);
	}	
}

/*********************************************************
  Function:	OnBnClickedBtnUpgradeExit
  Desc:		exit update
  Input:	
  Output:	
  Return:	
**********************************************************/
void CDlgUpgrade::OnBnClickedBtnUpgradeExit()
{
	if (m_lpUpgradeTimer)
	{
		KillTimer(UPGRADE_TIMER);
	}
	CDialog::OnCancel();
}

/*********************************************************
  Function:	OnTimer
  Desc:		refresh update status timer
  Input:	
  Output:	
  Return:	
**********************************************************/
#if (_MSC_VER >= 1500)	//vs2008
void CDlgUpgrade::OnTimer(UINT_PTR nIDEvent)
#else
void CDlgUpgrade::OnTimer(UINT nIDEvent) 
#endif
{
	// TODO: Add your message handler code here and/or call default
	char szLan[128] = {0};
	if (nIDEvent == UPGRADE_TIMER)
	{
		if (m_bUpgrade)
		{
			int UpgradeStatic = NET_DVR_GetUpgradeState(m_lUpgradeHandle);
			DWORD dwError = NET_DVR_GetLastError();
			int iPos = NET_DVR_GetUpgradeProgress(m_lUpgradeHandle);
			
			LONG iSubProgress = -1;
			int iStep = NET_DVR_GetUpgradeStep(m_lUpgradeHandle, &iSubProgress);

			if(iStep != -1)
			{
				GetDlgItem(IDC_STATIC_STEP)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_PROGRESS_STEP)->ShowWindow(SW_SHOW);	
				m_progressSub.SetPos(iSubProgress);
				switch(iStep)
				{
				case STEP_READY:
					g_StringLanType(szLan, "����׼������", "Ready to upgrade file");
					m_csUpgradeStep.Format("%s", szLan);
					break;
				case STEP_RECV_DATA:
					g_StringLanType(szLan, "���ڶ�ȡ�����ļ�", "Receving upgrade file");
					m_csUpgradeStep.Format("%s", szLan);
					break;
				case STEP_UPGRADE:
					g_StringLanType(szLan, "��������ϵͳ", "Upgrading system");
					m_csUpgradeStep.Format("%s", szLan);
					break;
				case STEP_BACKUP:
					g_StringLanType(szLan, "���ڱ���ϵͳ", "Backuping system");
					m_csUpgradeStep.Format("%s", szLan);
					break;
				case STEP_SEARCH:
					g_StringLanType(szLan, "�������������ļ�", "Searching  upgrade file");
					m_csUpgradeStep.Format("%s", szLan);
					break;
				default:
					g_StringLanType(szLan, "δ֪�׶�", "Unknow step");
					m_csUpgradeStep.Format("%s:%d", szLan, iStep);
					break;
				}
			}

			g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_GetUpgradeProgress= [%d]",iPos);
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
                bool modelFileNeedUpdate = false;
				switch (UpgradeStatic)
				{
				case -1:
					g_StringLanType(szLan, "����ʧ��", "Upgrade failed");
//					AfxMessageBox(szLan);			
					break;
                case 1:
                    if (true)
                    {
                        NET_DVR_XML_CONFIG_INPUT xmlInput = { 0 };
                        NET_DVR_XML_CONFIG_OUTPUT xmlOutput = { 0 };
                        xmlInput.dwSize = sizeof(NET_DVR_XML_CONFIG_INPUT);
                        xmlOutput.dwSize = sizeof(NET_DVR_XML_CONFIG_OUTPUT);
                        char *strUrl = "GET /ISAPI/ITC/AlgorithmsState\r\n";
                        xmlInput.lpRequestUrl = strUrl;
                        xmlInput.dwRequestUrlLen = strlen(strUrl);
                        xmlInput.lpInBuffer = NULL;
                        xmlInput.dwInBufferSize = 0;
                        xmlInput.dwRecvTimeOut = 1000;

                        char *pOutBuf = new char[5 * 1024];
                        memset(pOutBuf, 0, 5 * 1024);
                        xmlOutput.lpOutBuffer = pOutBuf;
                        xmlOutput.dwOutBufferSize = 5 * 1024;
                        if (NET_DVR_STDXMLConfig(m_lServerID, &xmlInput, &xmlOutput))
                        {
                            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_STDXMLConfig");
                            CString strRetXml = (const char*)xmlOutput.lpOutBuffer;
                            if (strRetXml.Find("modelFileNeedUpdate") > 0)
                            {
                                modelFileNeedUpdate = true;
                            }
                        }
                        else
                        {
                            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_STDXMLConfig");
                        }
                        delete[]pOutBuf;
                    }
                    if (m_comboUpgradeType.GetCurSel() == 4)
                    {
                        if (modelFileNeedUpdate)
                        {
                            g_StringLanType(szLan, "״̬�������豸�ɹ�������Ҫ����ģ���ļ�", "Status:upgrade successfully and model file need update");
                        }
                        else
                        {
                            g_StringLanType(szLan, "״̬�������豸�ɹ�", "Status:upgrade successfully");
                        }
                    }
                    else
                    {
                        if (modelFileNeedUpdate)
                        {
                            g_StringLanType(szLan, "״̬�������豸�ɹ�,������ģ���ļ��������豸", "Status:upgrade successfully, update mode file and reboot please");
                        }
                        else
                        {
                            g_StringLanType(szLan, "״̬�������豸�ɹ�,�������豸", "Status:upgrade successfully, reboot please");
                        }
                    }

					m_csUpgradeStat.Format(szLan);
					m_progressUpgrade.SetPos(100);
					break;			
				case 3:
					g_StringLanType(szLan, "״̬�������豸ʧ��", "Status:upgrade failed");
					m_csUpgradeStat.Format(szLan);	
					break;
				case 4:
					g_StringLanType(szLan, "״̬�����豸�������ݴ���, ״̬δ֪", "Status:get data with probrem from device, status unknown");
					m_csUpgradeStat.Format(szLan);					
					break;
				case 5:
					g_StringLanType(szLan, "״̬�������ļ����԰汾��ƥ��", "Status:Upgrade file language mismatch");
					m_csUpgradeStat.Format(szLan);				
					break;
				case 6:
					g_StringLanType(szLan, "״̬�������ļ�дflash�ļ�ʧ��", "Status:Upgrade file write Flash Fail!");
					m_csUpgradeStat.Format(szLan);				
					break;
                case 7:
                    g_StringLanType(szLan, "״̬�����������Ͳ�ƥ��", "Status:Upgrade Pack Type Mismatch!");
                    m_csUpgradeStat.Format(szLan);				
					break;
                case 8:
                    g_StringLanType(szLan, "״̬���������汾��ƥ��", "Status:Upgrade Pack Version Mismatch!");
                    m_csUpgradeStat.Format(szLan);				
					break;
                case 9:
                    g_StringLanType(szLan, "״̬��ϵͳ���������ļ�����", "Status:System has been locked (file lock)!");
                    m_csUpgradeStat.Format(szLan);
                    break;
                case 10:
                    g_StringLanType(szLan, "״̬�����������쳣", "Status:Backup regional anomaly!");
                    m_csUpgradeStat.Format(szLan);
                    break;
                case 11:
                    g_StringLanType(szLan, "״̬��ϵͳ����", "Status:System card is full!");
                    m_csUpgradeStat.Format(szLan);
                    break;
                case 12:
                    g_StringLanType(szLan, "״̬����������ʧ�ܣ���Ч��SessionID��", "Status:Reconnect failed(Invalid SessionID)!");
                    m_csUpgradeStat.Format(szLan);
                    break;
                case 13:
                    g_StringLanType(szLan, "״̬��������æ", "Status:Server is Busy!");
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
				if (!NET_DVR_CloseUpgradeHandle(m_lUpgradeHandle))
				{
					g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_CloseUpgradeHandle");
				}
				GetDlgItem(IDC_STATIC_STEP)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_PROGRESS_STEP)->ShowWindow(SW_HIDE);	
				m_lUpgradeHandle = -1;
			}
		}		
	}
	CDialog::OnTimer(nIDEvent);
}

/*********************************************************
  Function:	OnBtnSetEnviro
  Desc:		set the environment of the network
  Input:	none
  Output:	none
  Return:	none
**********************************************************/
void CDlgUpgrade::OnBtnSetEnviro() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	char szLan[128] = {0};

	if (!NET_DVR_SetNetworkEnvironment(m_comboEnvironment.GetCurSel()))
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_SetNetworkEnvironment[%d]", m_comboEnvironment.GetCurSel());
		g_StringLanType(szLan, "�������绷��", "Set up the network environment!");
		AfxMessageBox(szLan);
	}
	else
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_SetNetworkEnvironment[%d]", m_comboEnvironment.GetCurSel());
	}
	
}

void CDlgUpgrade::OnSelchangeComboUpgradeType() 
{
    char szLan[64] = {0};
    if (m_comboUpgradeType.GetCurSel() == 2)
    {
		g_StringLanType(szLan, "ͨ��", "Chan");
		GetDlgItem(IDC_STATIC_CHAN)->SetWindowText(szLan);
        GetDlgItem(IDC_STATIC_CHAN)->ShowWindow(SW_SHOW);
        m_comboChan.ShowWindow(SW_SHOW);

		m_comboChan.ResetContent();
		char szLan[128] = {0};
		for (int i = 0; i < g_struDeviceInfo[m_iDeviceIndex].iDeviceChanNum; i++)
		{
			sprintf(szLan, "Chan %d", i + 1);
			m_comboChan.AddString(szLan);
		}
    }
    else
    {
        GetDlgItem(IDC_STATIC_CHAN)->ShowWindow(SW_HIDE);
        m_comboChan.ShowWindow(SW_HIDE);
    }
    if (m_comboUpgradeType.GetCurSel() == 3)
    {
        GetDlgItem(IDC_STATIC_ACS_NO)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_EDIT_ACS_NO)->ShowWindow(SW_SHOW);
    }
    else
    {
        GetDlgItem(IDC_STATIC_ACS_NO)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_ACS_NO)->ShowWindow(SW_HIDE);
    }
    if (m_comboUpgradeType.GetCurSel() == 4)
    {
        g_StringLanType(szLan, "�豸", "Dev");
        GetDlgItem(IDC_STATIC_CHAN)->SetWindowText(szLan);
        GetDlgItem(IDC_STATIC_CHAN)->ShowWindow(SW_SHOW);
        m_comboChan.ShowWindow(SW_SHOW);
        
        m_comboChan.ResetContent();        
        for (int i = 0; i < 32; i++)
        {
            sprintf(szLan, "Dev %d", i);
            m_comboChan.AddString(szLan);
        }
    }
    else
    {
        GetDlgItem(IDC_STATIC_CHAN)->ShowWindow(SW_HIDE);
        m_comboChan.ShowWindow(SW_HIDE);
    }
	if (m_comboUpgradeType.GetCurSel() == 5)
	{
		g_StringLanType(szLan, "����", "Type");
        GetDlgItem(IDC_STATIC_CHAN)->SetWindowText(szLan);
		GetDlgItem(IDC_STATIC_TYPE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_COMBO_CARD_TYPE)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_STATIC_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_CARD_TYPE)->ShowWindow(SW_HIDE);
	}
}

void CDlgUpgrade::OnBtnGetupgradeinfo() 
{
	// TODO: Add your control notification handler code here
	//2013-06-17
	UpdateData(TRUE);
	if(m_csUpgradeFile.Compare(_T("")) == 0)
	{
		AfxMessageBox("�ļ�·��Ϊ��!");
		return;
	}
	char szFileName[64] = {0};
	long lFileNameLen = 64;
	CString strFileName;
	CString strFilePath;
	char* szFilePath = new char[m_csUpgradeFile.GetLength()+1];
	memset(szFilePath, 0, sizeof(szFilePath));
	memcpy(szFilePath, m_csUpgradeFile, m_csUpgradeFile.GetLength()+1);
 	
// 	if(!NET_DVR_FindTargetFile(m_lServerID, m_lChannel, szFilePath, szFileName, lFileNameLen))
// 	{
// 		delete []szFilePath;
// 		szFilePath = NULL;
// 		DWORD dwRet = NET_DVR_GetLastError();
// 		if (dwRet == NET_DVR_DIR_ERROR)
// 		{
// 			AfxMessageBox("·������!");
// 			return;
// 		}
// 		if (dwRet == NET_DVR_NO_CURRENT_UPDATEFILE)
// 		{
// 			AfxMessageBox("û��ƥ���ļ�!");
// 			return;
// 		}
// 		return;
// 	}

	NET_DVR_FUZZY_UPGRADE struFuzzyUpgrade = {0};
	DWORD dwReturn = 0;
	BOOL bRet = NET_DVR_GetDVRConfig(m_lServerID, NET_DVR_GET_FUZZY_UPGRADE, m_lChannel, &struFuzzyUpgrade, sizeof(struFuzzyUpgrade), &dwReturn);
	if (!bRet)
	{
		char szLan[128] = {0};
		g_StringLanType(szLan, "��ȡ��Ϣʧ��", "Get ParamInfo failed");
 		AfxMessageBox(szLan);
		return;
	}
	char chTargeName[260] = {0};
	int iRet = findTargetFile(szFilePath, struFuzzyUpgrade.sUpgradeInfo, chTargeName);
	if(iRet == 0)
	{
 		strFileName.Format(_T("%s"), chTargeName);
		m_csUpgradeFile.Format(_T("%s"), strFileName);
		delete []szFilePath;
		szFilePath = NULL;
	}
	delete []szFilePath;
	szFilePath = NULL;
	UpdateData(FALSE);
}

/******************************************
����:	ConvertData
����:	���ü򵥵����������ݱ任�����������ļ��Ĵ���ͽ��  
����:	src - source data
		len - data length
���:	dst - destination  data
����ֵ: HPR_OK-�ɹ���HPR_ERROR-ʧ��
******************************************/
int CDlgUpgrade::ConvertData(const char *src, char *dst, int nLen)
{
    /* �̶��Ļ������������任 */
    BYTE byMagic[16] = {0xba, 0xcd, 0xbc, 0xfe, 0xd6, 0xca, 0xdd, 0xd3,
		0xba, 0xb9, 0xa3, 0xab, 0xbf, 0xcb, 0xb5, 0xbe};
    int i, j;
    int nMagiclen, nStartMagic;
	
	//�жϲ�����Ч��
    if(src == NULL || dst == NULL)
    {
        return -1;
    }
	
    nMagiclen = sizeof(byMagic);
	//lint --e{440}
    for(i = 0, nStartMagic = 0; i<nLen; nStartMagic = (nStartMagic + 1) % nMagiclen)
    {
        //��startmagic����ÿ����ѭ��magic����ʼλ��
        for(j = 0; (j < nMagiclen) && (i < nLen); j++, i++)
        {
			// �������任
            //��(char)������ǿ��ת�����������⣬��Ϊ������������
            *dst++ = *src++ ^ (char)byMagic[(nStartMagic + j) % nMagiclen];   
        }
    }
    return 0;
}
#define UPDATE_FILE_FLAG_SIZE  18
#define UPDATE_FILE_HEAD_SIZE  64

/*************************************************
Function: findTargetFile 
Description: ����Ŀ���ļ�
Input:     szFilePath: �ļ���·��
szFileFlag: ƥ���ʶ
Output: szTargetFileName: Ŀ���ļ� 
Return: 0�ɹ�,-1ʧ��
*************************************************/
int CDlgUpgrade::findTargetFile(const char* szFilePath, const char* szFileFlag, char* szTargetFileName)
{
#if defined(_WIN32) || defined(_WIN64)
	char szFind[MAX_PATH] = {0};
	char szFile[MAX_PATH] = {0};
	WIN32_FIND_DATA FindFileData;
	strcpy(szFind, szFilePath);
    strcat(szFind, "\\*.*");
	HANDLE hFind = ::FindFirstFileA(szFind, &FindFileData);
    if (INVALID_HANDLE_VALUE == hFind)
	{
		FindClose(hFind);
		return -1;
	}
	while(TRUE)
    {
        if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            if(FindFileData.cFileName[0] != '.') //�������ļ���
            {
                strcpy(szFile, szFilePath);
                strcat(szFile, "\\");
                strcat(szFile, FindFileData.cFileName);
                if (findTargetFile(szFile, szFileFlag, szTargetFileName) == 0)
                {
					return 0;
                }
            }
        }
        else
        {      //deal with FindFileData.cFileName
			strcpy(szFile,szFilePath);
			strcat(szFile, "\\");
			strcat(szFile,FindFileData.cFileName);
			
			HANDLE hFile = ::CreateFileA(szFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (INVALID_HANDLE_VALUE != hFind)
			{
				if (::SetFilePointer(hFile, 0, 0, FILE_BEGIN) != HFILE_ERROR)
				{
					char szReadBuf[UPDATE_FILE_HEAD_SIZE+1] = {0};
					DWORD dwRealReadNum = 0;
					if (ReadFile(hFile, szReadBuf, UPDATE_FILE_HEAD_SIZE, &dwRealReadNum, NULL))
					{
						if (dwRealReadNum == UPDATE_FILE_HEAD_SIZE)
						{
							char szDecodeRes[UPDATE_FILE_HEAD_SIZE+1] = {0};
							char szDecodeFlag[UPDATE_FILE_FLAG_SIZE+1] = {0};
							//�����ļ�ͷ��Ϣ
							ConvertData(szReadBuf, szDecodeRes, UPDATE_FILE_HEAD_SIZE);
							//���ļ�ͷ��ȡ��FLAG
							for (int i = 0; i < UPDATE_FILE_FLAG_SIZE; i++)
							{
								szDecodeFlag[i] = szDecodeRes[i+44];
							}
						//	DebugString("Flag:%s", szDecodeFlag);
							if (strcmp(szDecodeFlag, szFileFlag) == 0)  //�ҵ�ƥ���ļ������ڼ�������
							{
								strcpy(szTargetFileName, szFile);
								CloseHandle(hFile);
								FindClose(hFind);
								return 0;
							}
						}
					}
				}
			}
			CloseHandle(hFile);
			
        }
		if(!FindNextFile(hFind,&FindFileData)) //�ļ���������
		{
			break;
		}
    }
    FindClose(hFind);
	return -1;
#else
	return -1;
#endif
}
