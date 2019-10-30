// configDlg.cpp : implementation file

/**********************************************************
FileName:    configDlg.cpp
Description: �������Ի���      
Date:        
Note: 		 the global Macro definition and structure is in "GeneralDef.h", global variable and function is in "Config.cpp"
Modification History:      
<version>    <time>         <desc>

***********************************************************/

#include "stdafx.h"
#include "config.h"
#include "configDlg.h"
#include "GeneralDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg dialog

CConfigDlg::CConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigDlg)
	m_iDevPort = 8000;
	m_csPassword = _T("12345");
	m_csUserName = _T("admin");
	m_bIsLogin = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigDlg)
	DDX_Control(pDX, IDC_DEV_IPADDRESS, m_ipctrlDevIP);
	DDX_Control(pDX, IDC_TAB_CONFIG_DLG, m_tabConfigDlg);
	DDX_Text(pDX, IDC_EDIT_DEV_PORT, m_iDevPort);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_csPassword);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_csUserName);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CConfigDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_CONFIG_DLG, OnSelchangeTabConfigDlg)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, OnButtonLogin)
	ON_BN_CLICKED(IDC_BUTTON_SHUTDOWN, OnButtonShutdown)
	ON_BN_CLICKED(IDC_BUTTON_REBOOT, OnButtonReboot)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, OnButtonRefreshCfg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg message handlers

/*********************************************************
Function:	OnInitDialog
Desc:		��ʼ���Ի���Initialize the dialog��
Input:	
Output:	
Return:	
**********************************************************/
BOOL CConfigDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	m_ipctrlDevIP.SetAddress(192,0,0,64);//��ʼ��ip��ַ�ؼ�����ip��ַ

	/**********************************��ʼ��tab�ؼ� begin*****************************************/
	m_tabConfigDlg.InsertItem(0,"�豸��������");
	m_tabConfigDlg.InsertItem(1,"ͨ����������");
	m_tabConfigDlg.InsertItem(2,"���ڲ�������");
	m_tabConfigDlg.InsertItem(3,"������������");
	m_tabConfigDlg.InsertItem(4,"�û�����");
	m_tabConfigDlg.InsertItem(5,"�쳣��Ϣ");

    m_DlgDeviceCfg.Create(IDD_DIALOG_DEVICE_CFG,&m_tabConfigDlg);
	m_DlgChanCfg.Create(IDD_DIALOG_CHAN_CFG,&m_tabConfigDlg);
	m_DlgSerialCfg.Create(IDD_DIALOG_SERIAL_CFG,&m_tabConfigDlg);
	m_DlgAlarmCfg.Create(IDD_DIALOG_ALARM_CFG,&m_tabConfigDlg);
	m_DlgUserCfg.Create(IDD_DIALOG_USER_CFG,&m_tabConfigDlg);
	m_DlgExceptionCfg.Create(IDD_DIALOG_EXCEPTION_CFG,&m_tabConfigDlg);

	
	CRect rc;
	m_tabConfigDlg.GetWindowRect(&rc);
	
    m_DlgDeviceCfg.MoveWindow(1,20,rc.Width()-5,rc.Height()-25);
	m_DlgChanCfg.MoveWindow(1,20,rc.Width()-5,rc.Height()-25);
	m_DlgSerialCfg.MoveWindow(1,20,rc.Width()-5,rc.Height()-25);
	m_DlgAlarmCfg.MoveWindow(1,20,rc.Width()-5,rc.Height()-25);
	m_DlgUserCfg.MoveWindow(1,20,rc.Width()-5,rc.Height()-25);
	m_DlgExceptionCfg.MoveWindow(1,20,rc.Width()-5,rc.Height()-25);
    m_DlgDeviceCfg.ShowWindow(SW_SHOW);
	m_tabConfigDlg.SetCurSel(0);
	/**********************************��ʼ��tab�ؼ� end******************************************/
    
	//��ע�ᵽ�豸֮ǰ�ø����Ի����ʹ�ܣ�ע��ɹ���ʹ֮ʹ�ܡ�
	//m_tabConfigDlg.EnableWindow(FALSE);//����ֻ��tab�ؼ���ʹ�ܾͿ����ˣ�ʡȥ��������䡣
	m_DlgDeviceCfg.EnableWindow(FALSE);
	m_DlgChanCfg.EnableWindow(FALSE);
	m_DlgSerialCfg.EnableWindow(FALSE);
	m_DlgAlarmCfg.EnableWindow(FALSE);
	m_DlgUserCfg.EnableWindow(FALSE);
	m_DlgExceptionCfg.EnableWindow(FALSE);
	

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CConfigDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CConfigDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CConfigDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

/*********************************************************
Function:	OnSelchangeTabConfigDlg
Desc:		tabҳѡ�����Ϣ��Ӧ����
Input:	
Output:	
Return:	
**********************************************************/
void CConfigDlg::OnSelchangeTabConfigDlg(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int iCurSel = m_tabConfigDlg.GetCurSel();
	switch(iCurSel)
	{
	case 0:
		m_DlgDeviceCfg.ShowWindow(SW_SHOW);
		//�л�tabҳʱ�Դ�ҳ��Ϣ���и��¡�
		//m_DlgDeviceCfg.GetDeviceCfg();
	    //m_DlgDeviceCfg.GetNetCfg();
		m_DlgChanCfg.ShowWindow(SW_HIDE);
		m_DlgSerialCfg.ShowWindow(SW_HIDE);
		m_DlgAlarmCfg.ShowWindow(SW_HIDE);
		m_DlgUserCfg.ShowWindow(SW_HIDE);
		m_DlgExceptionCfg.ShowWindow(SW_HIDE);
		break;
	case 1:
		m_DlgDeviceCfg.ShowWindow(SW_HIDE);
		m_DlgChanCfg.ShowWindow(SW_SHOW);
		//�л�tabҳʱ�Դ�ҳ��Ϣ���и��¡�
		//m_DlgChanCfg.CurCfgUpdate();
		m_DlgSerialCfg.ShowWindow(SW_HIDE);
		m_DlgAlarmCfg.ShowWindow(SW_HIDE);
		m_DlgUserCfg.ShowWindow(SW_HIDE);
		m_DlgExceptionCfg.ShowWindow(SW_HIDE);
		break;
	case 2:
		m_DlgDeviceCfg.ShowWindow(SW_HIDE);
		m_DlgChanCfg.ShowWindow(SW_HIDE);
		m_DlgSerialCfg.ShowWindow(SW_SHOW);
		//�л�tabҳʱ�Դ�ҳ��Ϣ���и��¡�
		//m_DlgSerialCfg.GetRS232Cfg();
	    //m_DlgSerialCfg.GetRS485Cfg();
		m_DlgAlarmCfg.ShowWindow(SW_HIDE);
		m_DlgUserCfg.ShowWindow(SW_HIDE);
		m_DlgExceptionCfg.ShowWindow(SW_HIDE);
		break;
	case 3:
		m_DlgDeviceCfg.ShowWindow(SW_HIDE);
		m_DlgChanCfg.ShowWindow(SW_HIDE);
		m_DlgSerialCfg.ShowWindow(SW_HIDE);
		m_DlgAlarmCfg.ShowWindow(SW_SHOW);
		//�л�tabҳʱ�Դ�ҳ��Ϣ���и��¡�
		//m_DlgAlarmCfg.CurCfgUpdate();
		m_DlgUserCfg.ShowWindow(SW_HIDE);
		m_DlgExceptionCfg.ShowWindow(SW_HIDE);
		break;
	case 4:
		m_DlgDeviceCfg.ShowWindow(SW_HIDE);
		m_DlgChanCfg.ShowWindow(SW_HIDE);
		m_DlgSerialCfg.ShowWindow(SW_HIDE);
		m_DlgAlarmCfg.ShowWindow(SW_HIDE);
		m_DlgUserCfg.ShowWindow(SW_SHOW);
		//�л�tabҳʱ�Դ�ҳ��Ϣ���и��¡�
		//m_DlgUserCfg.CurCfgUpdate();
		m_DlgExceptionCfg.ShowWindow(SW_HIDE);
		break;
	case 5:
		m_DlgDeviceCfg.ShowWindow(SW_HIDE);
		m_DlgChanCfg.ShowWindow(SW_HIDE);
		m_DlgSerialCfg.ShowWindow(SW_HIDE);
		m_DlgAlarmCfg.ShowWindow(SW_HIDE);
		m_DlgUserCfg.ShowWindow(SW_HIDE);
		m_DlgExceptionCfg.ShowWindow(SW_SHOW);
		//�л�tabҳʱ�Դ�ҳ��Ϣ���и��¡�
		//m_DlgExceptionCfg.CurCfgUpdate();
		break;

	default:
		break;
	}
	*pResult = 0;
}
/*********************************************************
Function:	OnButtonLogin
Desc:		�ж��Ƿ��Ѿ�ע�ᣬע��ɹ�����ť��״̬��Ϊ��ע����
Input:	
Output:	
Return:	
**********************************************************/
void CConfigDlg::OnButtonLogin() 
{
	if(!m_bIsLogin)    //login
	{
		if(!DoLogin())
		{
			return;
		}
		GetDlgItem(IDC_BUTTON_LOGIN)->SetWindowText("ע��");
		m_bIsLogin = TRUE;		
		
	    //ע��ɹ����ü������öԻ���ʹ�ܡ�
		m_DlgDeviceCfg.EnableWindow(TRUE);
		m_DlgChanCfg.EnableWindow(TRUE);
		m_DlgSerialCfg.EnableWindow(TRUE);
		m_DlgAlarmCfg.EnableWindow(TRUE);
		m_DlgUserCfg.EnableWindow(TRUE);
	    m_DlgExceptionCfg.EnableWindow(TRUE);
	}
	else      //logout
	{
		NET_DVR_Logout_V30(g_lUserID);

		//��ȫ�ֽṹ��g_struDeviceInfo[0]���³�ʼ��
		STRU_DEVICE_INFO Tem_g_struDeviceInfo;
		memcpy(&g_struDeviceInfo[0],&Tem_g_struDeviceInfo,sizeof(STRU_DEVICE_INFO));
		
		GetDlgItem(IDC_BUTTON_LOGIN)->SetWindowText("ע��");
		g_lUserID = -1;
		m_bIsLogin = FALSE;

		//ע���ɹ����������Ի����ʹ�ܡ�
		m_DlgDeviceCfg.EnableWindow(FALSE);
		m_DlgChanCfg.EnableWindow(FALSE);
		m_DlgSerialCfg.EnableWindow(FALSE);
		m_DlgAlarmCfg.EnableWindow(FALSE);
		m_DlgUserCfg.EnableWindow(FALSE);
	    m_DlgExceptionCfg.EnableWindow(FALSE);
	}
}

/*********************************************************
Function:	DoLogin
Desc:		ע����̣�ע��ɹ����ȡ�豸�ĸ������ò�����Ϣ��
Input:	
Output:	
Return:	
**********************************************************/
BOOL CConfigDlg::DoLogin()
{
	UpdateData(TRUE);
	g_lUserID=-1;
	
	CString DeviceIp;
	//BYTE nField0,nField1,nField2,nField3; 
	//m_ctrlDevIp.GetAddress(nField0,nField1,nField2,nField3);//GetAddress��IP��ַ�ؼ���Ӧ��������CCIPAddressCtrl��һ��ʹ�ú��������Բ鿴msdn��
	//DeviceIp.Format("%d.%d.%d.%d",nField0,nField1,nField2,nField3);//DeviceIP��Ϊ�ˡ�172.7.94.6�����͵��ַ�����
	m_ipctrlDevIP.GetWindowText(DeviceIp);	
	
	memset(&DeviceInfoTmp,0,sizeof(NET_DVR_DEVICEINFO_V30));
	
	//��ʼע��(�ֶ�����IP���˿ڣ��û��������롣)
	g_lUserID =  NET_DVR_Login_V30(DeviceIp.GetBuffer(DeviceIp.GetLength()),m_iDevPort, \
		m_csUserName.GetBuffer(m_csUserName.GetLength()),m_csPassword.GetBuffer(m_csPassword.GetLength()),&DeviceInfoTmp);
	
	if(g_lUserID == -1)
	{
		MessageBox("ע�ᵽ�豸ʧ��!\n");
		return FALSE;
	}
	else
	{
		MessageBox("�ɹ�ע�ᵽ�豸!\n");
	}
	
	g_struDeviceInfo[0].iDeviceChanNum = DeviceInfoTmp.byChanNum;//��ȡ�豸��ģ��ͨ����
	g_struDeviceInfo[0].iStartChan = DeviceInfoTmp.byStartChan;//��ʼͨ������һ����1.����9000�豸���Խ����豸��
	g_struDeviceInfo[0].lLoginID = g_lUserID;//��½ID
    g_struDeviceInfo[0].iAlarmOutNum = DeviceInfoTmp.byAlarmOutPortNum;//�����������
	g_struDeviceInfo[0].iAlarmInNum = DeviceInfoTmp.byAlarmInPortNum;
	g_struDeviceInfo[0].iAudioNum = DeviceInfoTmp.byAudioChanNum;
	g_struDeviceInfo[0].iIPChanNum = DeviceInfoTmp.byIPChanNum;
	sprintf(g_struDeviceInfo[0].chLoginUserName, "%s", m_csUserName.GetBuffer(0));
	
	
	if(DoGetDeviceResoureCfg(0))//����豸��ͨ��ʹ����Ϣ��8000��ͨ��ȫʹ�ܣ�9000��Ҫ���жϡ�NET_DVR_IPALARMININFO�ṹ�塣
	{
		DoGetIPAlarmInCfg(0);//���ip�豸�ı����������ͨ����Ϣ��NET_DVR_IPALARMINCFG��NET_DVR_IPALARMOUTINFO�ṹ�塣
		DoGetIPAlarmOutCfg(0);
	}
	
	//��ȡ�豸�ĸ���������Ϣ������ʾ���Ի����ϡ�
	m_DlgDeviceCfg.GetDeviceCfg();
	m_DlgDeviceCfg.GetNetCfg();
	m_DlgChanCfg.CurCfgUpdate();
	m_DlgSerialCfg.GetRS232Cfg();
	m_DlgSerialCfg.GetRS485Cfg();
	m_DlgAlarmCfg.CurCfgUpdate();
	m_DlgUserCfg.CurCfgUpdate();
	m_DlgExceptionCfg.CurCfgUpdate();
	
	return TRUE;
}

/*********************************************************
Function:	OnButtonShutdown
Desc:		�ر��豸
Input:	
Output:	
Return:	
**********************************************************/
void CConfigDlg::OnButtonShutdown() 
{
	if (g_lUserID < 0)
	{
		MessageBox("���ȵ�¼�豸��");
		return;
	}

	if(IDOK == MessageBox("ȷ��Ҫ�ر��豸?","Warning",MB_OKCANCEL))//���û�ѡ���ǲ���Ҫ�ػ���
	{
        
        BOOL SuccessShutDown;
		SuccessShutDown=NET_DVR_ShutDownDVR(g_lUserID);
		if(SuccessShutDown)
		{
			MessageBox("�ػ��ɹ�!","ע��");
		}
		else
		{
			MessageBox("�ػ�ʧ��","warning");
			// CString err;
			//err.Format("�ػ�ʧ��,�������%d",NET_DVR_GetLastError());//�Դ��������ʽ��ʾ����			
			//err.Format("�ػ�ʧ��,%s",NET_DVR_GetErrorMsg());//�Դ���������Ϣ��Ӣ�ģ�����������
			//MessageBox(err,"warning");
		}
	}	
}

/*********************************************************
Function:	OnButtonReboot
Desc:		�����豸
Input:	
Output:	
Return:	
**********************************************************/
void CConfigDlg::OnButtonReboot() 
{
	if (g_lUserID < 0)
	{
		MessageBox("���ȵ�¼�豸��");
		return;
	}

	if(IDOK == MessageBox("ȷ��Ҫ�����豸?","Warning",MB_OKCANCEL))//���û�ѡ���ǲ���Ҫ������
	{
        BOOL SuccessReboot;
		SuccessReboot=NET_DVR_RebootDVR(g_lUserID);
		if(SuccessReboot)
		{
			MessageBox("�����ɹ�!���Ժ�","ע��");
		}
		else
		{
			MessageBox("����ʧ��");
		}
	}	
}

/*********************************************************
Function:	OnCancel
Desc:		�˳����Ի���ʱ����ʾ�Ƿ��˳����򣬲���û��ע���������ע����
Input:	
Output:	
Return:	
**********************************************************/
void CConfigDlg::OnCancel()
{
	char szLan[128] = {0};
	g_StringLanType(szLan, "ȷ��Ҫ�˳�����?", "Sure to exit?");
	
	if (IDOK != MessageBox(szLan,"Warning",IDOK))
	{
		return;
	}
	if (g_lUserID >= 0)
	{
		NET_DVR_Logout_V30(g_lUserID);
	}
	CDialog::OnCancel();
}

/*********************************************************
Function:	OnButtonRefreshCfg
Desc:		ˢ���豸�Ĳ�����Ϣ
Input:	
Output:	
Return:	
**********************************************************/
void CConfigDlg::OnButtonRefreshCfg() 
{
	if (g_lUserID < 0)
	{
		MessageBox("���ȵ�¼�豸��");
		return;
	}
	
    //ˢ�µ�ǰҳ���������Ϣ
	int iCurSel = m_tabConfigDlg.GetCurSel();
	switch(iCurSel)
	{
	case 0:
		m_DlgDeviceCfg.GetDeviceCfg();
		m_DlgDeviceCfg.GetNetCfg();
		break;
	case 1:
		m_DlgChanCfg.CurCfgUpdate();
		break;	
	case 2:
		m_DlgSerialCfg.GetRS232Cfg();
		m_DlgSerialCfg.GetRS485Cfg();
		break;
	case 3:
		m_DlgAlarmCfg.CurCfgUpdate();
		break;
	case 4:
		m_DlgUserCfg.CurCfgUpdate();
		break;
	case 5:
		m_DlgExceptionCfg.CurCfgUpdate();
		break;		
	default:
		break;
	}
}


