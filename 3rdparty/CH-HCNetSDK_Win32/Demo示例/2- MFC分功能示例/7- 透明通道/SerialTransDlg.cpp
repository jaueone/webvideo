/*******************************************************
Copyright 2008-2011 Hikvision Digital Technology Co., Ltd. 
��  ����	SerialTransDlg.cpp
������λ��	���ݺ�������
��  д��	shizhiping
��  �ڣ�	2009.5
��  ����	͸��ͨ��
��  �ģ�	
********************************************************/

#include "stdafx.h"
#include "SerialTrans.h"
#include "SerialTransDlg.h"
#include <process.h>
#include <string>
#include <cassert>

using std::string;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/***********************************
function: IsHexChar
desc:   �ж��ַ��Ƿ���0-9��a-f֮��
input�� char-�ַ�
Output��
return:  �ַ���������8-8��a-10�����
         ���ǣ�����-1
***********************************/
int IsHexChar(char hc)
{
	if('0'<=hc && hc<='9')
		return (int(hc)-int('0'));
	else if('a'<=hc && hc<='f')
		return (int(hc)-int('a')+10);
	else if('A'<=hc && hc<='F')
		return (int(hc)-int('A')+10);
	return -1;
}

/**************************************
function: Hex2Char
desc:     16������ת��Ϊ�ַ�
Input:    hex-16���ƣ��ַ�����ʽ��2�ַ�
Output:   
return:    ascii�ַ�
**************************************/
unsigned char Hex2Char(const string &hex)
{    	
	assert(hex.length() == 2);
	int high = IsHexChar(hex[0]);
	int low = IsHexChar(hex[1]);
	if(high == -1 || low == -1)
		return '\0';
	int asc = high*16+low;
	//	char b = toascii(asc);
	return asc;
}

/****************************
function: Hex2String
desc:     16�����ַ���ת��Ϊascii�ַ���
Input:    hex-16�����ַ���
Output:
return:  ascii�ַ���
*****************************/
string Hex2String(const string &hex)
{
	assert(hex.length()%2 == 0);
	string hstr;
	for(int i=0; i<hex.length(); i+=2)
	{
		string tmp = hex.substr(i,2);
		hstr.append(1,Hex2Char(tmp));
	}
	return hstr;
}
/****************************************
function��PrepareHexString
desc:  ȥ���ַ�����Ŀո�0x
Input:    str��ͨ�ַ���
Output:
return:  ȥ���ո��0x���ַ���
****************************************/
string PrepareHexString(string str)
{
	//��ɾ���ո�
	int i=0;
	while(i != str.length())
	{
		i = str.find(' ',i);
		if(i == -1)
			break;
		str = str.erase(i,1);
	}
	
	//ɾ��0x
    i = 0;
	string tmp("0x");
	while(i != str.length())
	{
		i = str.find(tmp,i);
		if(i == -1)
			break;
		str = str.erase(i,2);
	}
	
	//ɾ��0X
    i = 0;
	tmp = "0X";
	while(i != str.length())
	{
		i = str.find(tmp,i);
		if(i == -1)
			break;
		str = str.erase(i,2);
	}
	if(str.length()%2 != 0)
		str.append(1,'0');

	str = Hex2String(str);
    return str;
}


/*************************************************
������:    	fSerialDataCallBack
��������:	�ص��������ص����յ�����
�������:   lSerialHandle-���ھ��
            pRecvDataBuffer-���ݻ���
            dwBufSize-���ݴ�С
			dwUser-�û�����
�������:   			
����ֵ:		
*************************************************/
void CALLBACK fSerialDataCallBack(LONG lSerialHandle, char *pRecvDataBuffer, DWORD dwBufSize, DWORD dwUser)
{
     CSerialTransDlg *TransDlg = (CSerialTransDlg*)dwUser;
	 CString tmp1;
	 CString tmp2;
	 CTime ct;
	 int index = 0;
	 ct = CTime::GetCurrentTime();
	 tmp1.Format("%d-%d-%d %d:%d:%d", ct.GetYear(), ct.GetMonth(), ct.GetDay(), ct.GetHour(),
				ct.GetMinute(), ct.GetSecond());
	 index = TransDlg->m_RecvDataList.InsertItem(TransDlg->m_RecvDataList.GetItemCount(), tmp1, 0);
	 
	 TransDlg->m_RecvDataList.SetItemText(index, 1, pRecvDataBuffer);
	 tmp1.Empty();
	 for (int i=0; i<(int)dwBufSize; i++)
	 {
		 tmp2.Format("0x%x ", pRecvDataBuffer[i]);
		 tmp1 += tmp2;
	 }
	 TransDlg->m_RecvDataList.SetItemText(index, 2, tmp1);

}



/*************************************************
������:    	SendDataProc
��������:	���������߳�
�������:   pParam-�û�����
�������:   			
����ֵ:		
*************************************************/
void SendDataProc(LPVOID pParam)
{
    CSerialTransDlg *pDlg = (CSerialTransDlg*)pParam;
	BOOL bRet;
    (pDlg->GetDlgItem(IDC_BUTTON_SEND))->SetWindowText("ֹͣ");
	pDlg->m_bIsSending = TRUE;
	 (pDlg->GetDlgItem(IDC_STATIC_SEND_STATE))->SetWindowText("���ڷ�������.....");
	if(0 == pDlg->m_iCurInterface)
	{
        bRet = NET_DVR_SerialSend(pDlg->m_lSerialHandle,pDlg->m_iSerialChan,pDlg->m_DataBuf,pDlg->m_DataLen);
	}
	else
	{
        bRet = NET_DVR_SendToSerialPort(pDlg->m_struDeviceInfo.lLoginID,pDlg->m_iCurSerialType,pDlg->m_iSerialChan,pDlg->m_DataBuf,pDlg->m_DataLen);
	}

	
	
	if(!bRet)
	{
		(pDlg->GetDlgItem(IDC_STATIC_SEND_STATE))->SetWindowText("����ʧ��");
	}
	else
	{
        (pDlg->GetDlgItem(IDC_STATIC_SEND_STATE))->SetWindowText("���ͳɹ�");
	}

	if(pDlg->m_bSendContinual)
	{
		if(pDlg->m_iSendInterval <= 0)
		{
			AfxMessageBox("���ͼ���������0");
			goto exitsvc;
		}
		UINT i=0;
		(pDlg->GetDlgItem(IDC_STATIC_SEND_STATE))->SetWindowText("���ڷ�������.....");
		while(1)
		{
			
			for(i=0; i<pDlg->m_iSendInterval*2; i++)
			{
				Sleep(500);
				if(!pDlg->m_bSendContinual)
				{
					goto exitsvc;
				}
			}
       
	    	if(0 == pDlg->m_iCurInterface)
			{
		    	bRet = NET_DVR_SerialSend(pDlg->m_lSerialHandle,pDlg->m_iSerialChan,pDlg->m_DataBuf,pDlg->m_DataLen);
			}
	    	else
			{
		    	bRet = NET_DVR_SendToSerialPort(pDlg->m_struDeviceInfo.lLoginID,pDlg->m_iCurSerialType,pDlg->m_iSerialChan,pDlg->m_DataBuf,pDlg->m_DataLen);
			}
		}
	}
exitsvc:
	pDlg->m_bIsSending = FALSE;
	(pDlg->GetDlgItem(IDC_BUTTON_SEND))->SetWindowText("����");
	(pDlg->GetDlgItem(IDC_STATIC_SEND_STATE))->SetWindowText("");

}
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
// CSerialTransDlg dialog

CSerialTransDlg::CSerialTransDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSerialTransDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSerialTransDlg)
	m_csDataSend = _T("");
	m_iSendInterval = 0;
	m_bSendContinual = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_nDevPort = 8000;
	m_csUser = _T("admin");
	m_csPWD = _T("12345");
	m_bIsLogin = FALSE;
    m_iCurSerialType = 1;
	m_iSerialChan = 1;
	m_lSerialHandle = -1;
	m_iCurInterface = 0;
	m_DataLen = 0;
	m_bIsSending = FALSE;
	memset(m_DataBuf,0,sizeof(m_DataBuf));
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSerialTransDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSerialTransDlg)
	DDX_Control(pDX, IDC_LIST_DATA_RECV, m_RecvDataList);
	DDX_Control(pDX, IDC_COMBO_INTERFACE, m_comboInterface);
	DDX_Control(pDX, IDC_COMBO_CHAN, m_comboChan);
	DDX_Control(pDX, IDC_COMBO_SERIAL_TYPE, m_comboSerialType);
	DDX_Control(pDX, IDC_IPADDRESS_DEV, m_ctrlDevIp);
	DDX_Text(pDX, IDC_EDIT_PORT, m_nDevPort);
	DDX_Text(pDX, IDC_EDIT_USER, m_csUser);
	DDX_Text(pDX, IDC_EDIT_PWD, m_csPWD);
	DDX_Text(pDX, IDC_EDIT_DATA_SEND, m_csDataSend);
	DDV_MaxChars(pDX, m_csDataSend, 1000);
	DDX_Text(pDX, IDC_EDIT_INTERVAL, m_iSendInterval);
	DDX_Check(pDX, IDC_CHECK_SEND_CONTINUAL, m_bSendContinual);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSerialTransDlg, CDialog)
	//{{AFX_MSG_MAP(CSerialTransDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, OnButtonLogin)
	ON_CBN_SELCHANGE(IDC_COMBO_SERIAL_TYPE, OnSelchangeComboSerialType)
	ON_BN_CLICKED(IDC_BUTTON_SERIALSTART, OnButtonSerialstart)
	ON_BN_CLICKED(IDC_BUTTON_SEND, OnButtonSend)
	ON_CBN_SELCHANGE(IDC_COMBO_CHAN, OnSelchangeComboChan)
	ON_BN_CLICKED(IDC_BUTTON_SERIALSTOP, OnButtonSerialstop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSerialTransDlg message handlers

BOOL CSerialTransDlg::OnInitDialog()
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
	
	// Dev ip
	m_ctrlDevIp.SetAddress(172,7,96,7);

	//��������COMBO
	m_comboSerialType.SetCurSel(0);
	OnSelchangeComboSerialType();
     
	//���룬Ĭ��ΪASCII
	((CButton*)GetDlgItem(IDC_RADIO_ASCII))->SetCheck(1);

	//���ͽӿ�
	m_comboInterface.SetCurSel(0);

	//�����б�
	m_RecvDataList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_RecvDataList.InsertColumn(0, "Time", LVCFMT_LEFT, 100, 0);
	m_RecvDataList.InsertColumn(1, "Content", LVCFMT_LEFT, 200, 0);
	m_RecvDataList.InsertColumn(2, "Hex", LVCFMT_LEFT, 450, 0);

	//���͹رհ�ť״̬
	GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(FALSE);
    GetDlgItem(IDC_BUTTON_SERIALSTOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SERIALSTART)->EnableWindow(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSerialTransDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSerialTransDlg::OnPaint() 
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
HCURSOR CSerialTransDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


/*************************************************
������:    	OnButtonLogin
��������:	ע��/ע�� ��ť
�������:   
�������:   			
����ֵ:		
*************************************************/
void CSerialTransDlg::OnButtonLogin() 
{
	if(!m_bIsLogin)    //login
	{
		if(!DoLogin())
			return;
		GetDlgItem(IDC_BUTTON_LOGIN)->SetWindowText("Logout");
		GetDlgItem(IDC_BUTTON_SERIALSTART)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(TRUE);
		m_bIsLogin = TRUE;
		
	}
	else      //logout
	{
		
		if(m_lSerialHandle >=0 )
		{
			MessageBox("�ȹر�͸��ͨ��");
			return;
		}
        NET_DVR_Logout_V30(m_struDeviceInfo.lLoginID); 
		GetDlgItem(IDC_BUTTON_LOGIN)->SetWindowText("Login");
		GetDlgItem(IDC_BUTTON_SERIALSTART)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(FALSE);
		m_struDeviceInfo.lLoginID = -1;
		m_bIsLogin = FALSE;
	}
	
}

/*************************************************
������:    	DoLogin
��������:	���豸ע��
�������:   
�������:   			
����ֵ:		
*************************************************/
BOOL CSerialTransDlg::DoLogin()
{
	UpdateData(TRUE);
	CString DeviceIp;
	BYTE nField0,nField1,nField2,nField3; 
	m_ctrlDevIp.GetAddress(nField0,nField1,nField2,nField3);
	DeviceIp.Format("%d.%d.%d.%d",nField0,nField1,nField2,nField3);
	
	NET_DVR_DEVICEINFO_V30 DeviceInfoTmp;
	memset(&DeviceInfoTmp,0,sizeof(NET_DVR_DEVICEINFO_V30));
	
	LONG lLoginID = NET_DVR_Login_V30(DeviceIp.GetBuffer(DeviceIp.GetLength()),m_nDevPort, \
		m_csUser.GetBuffer(m_csUser.GetLength()),m_csPWD.GetBuffer(m_csPWD.GetLength()),&DeviceInfoTmp);
	if(lLoginID == -1)
	{
		MessageBox("Login to Device failed!\n");
		return FALSE;
	}
    m_struDeviceInfo.lLoginID = lLoginID;
	m_struDeviceInfo.iDeviceChanNum = DeviceInfoTmp.byChanNum;

	return TRUE;
}


/*****************************************
������:    OnSelchangeComboSerialType
��������:  Combo ��Ϣ��Ӧ����,ѡ�񴮿�����
�������:
�������:
����:
*****************************************/
void CSerialTransDlg::OnSelchangeComboSerialType() 
{
	CString csTmp;
	int i=0;
	m_iCurSerialType = m_comboSerialType.GetCurSel() + 1;
	m_comboChan.ResetContent();
	switch(m_iCurSerialType)
	{
	case 1:   //232
        csTmp = "Serial 1";
        m_comboChan.AddString(csTmp);
		m_comboChan.EnableWindow(FALSE);
		break;
	case 2:   //485
		for(i=0; i<m_struDeviceInfo.iDeviceChanNum; i++)
		{
             csTmp.Format("Channel %d",i+1);
			 m_comboChan.AddString(csTmp);
		}
		m_comboChan.EnableWindow(TRUE);
		break;
	default:
		break;
	}

	m_iSerialChan = 1;
	m_comboChan.SetCurSel(m_iSerialChan - 1);
	
}

/*************************************************
������:    	OnButtonSerialstart
��������:	����͸��ͨ��
�������:   
�������:   			
����ֵ:		
*************************************************/
void CSerialTransDlg::OnButtonSerialstart() 
{
	m_lSerialHandle = NET_DVR_SerialStart(m_struDeviceInfo.lLoginID,m_iCurSerialType,fSerialDataCallBack,(DWORD)this);
	if(m_lSerialHandle == -1)
	{
		CString err;
		err.Format("����͸��ͨ��ʧ��,������룺%d",NET_DVR_GetLastError());
		MessageBox(err);
	}
	else
	{
        MessageBox("����͸��ͨ���ɹ�");
		GetDlgItem(IDC_BUTTON_SERIALSTART)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SERIALSTOP)->EnableWindow(TRUE);
	}
	
	
}


/*************************************************
������:    	OnButtonSend
��������:	���Ͱ�ť
�������:   
�������:   			
����ֵ:		
*************************************************/
void CSerialTransDlg::OnButtonSend() 
{
	if(!m_bIsSending)
	{
    	UpdateData(TRUE);

        m_DataLen = m_csDataSend.GetLength();
	    if(m_DataLen == 0)
		{
	    	MessageBox("���ܷ��Ϳ�����");
	    	return;
		}

    	//�жϵ�ǰ�ı���
    	if(((CButton*)GetDlgItem(IDC_RADIO_HEX))->GetCheck())  //HEX���룬��ת����ASCII�ַ���
		{
             string hexstring = m_csDataSend.GetBuffer(m_csDataSend.GetLength());
			 hexstring = PrepareHexString(hexstring);
			 int len = hexstring.length();
             m_DataLen = len<DATABUF_LEN ? len : DATABUF_LEN;
			 //���ͻ���
             memcpy(m_DataBuf,hexstring.c_str(),m_DataLen);   
		}
		else
		{
             m_DataLen = m_DataLen<DATABUF_LEN  ? m_DataLen : DATABUF_LEN;
             //���ͻ���
             memcpy(m_DataBuf,m_csDataSend.GetBuffer(m_DataLen),m_DataLen);   
		}
    	
	    //ѡ���ͽӿ�
	    m_iCurInterface = m_comboInterface.GetCurSel();
	
	    //���������߳�
	    ULONG SendDataThread = _beginthread(SendDataProc,NULL,(void*)this);
	}
	else   //���ڷ��ͣ��ر�
	{
		m_bSendContinual = FALSE;
		//UpdateData(FALSE);     
	}
	
}

/*************************************************
������:    	OnSelchangeComboChan
��������:	ѡ��ͨ��
�������:   
�������:   			
����ֵ:		
*************************************************/
void CSerialTransDlg::OnSelchangeComboChan() 
{
    m_iSerialChan = m_comboChan.GetCurSel()+1;	
}

/*************************************************
������:    	OnButtonSerialstop
��������:	ֹͣ͸��ͨ��
�������:   
�������:   			
����ֵ:		
*************************************************/
void CSerialTransDlg::OnButtonSerialstop() 
{
	if(m_bIsSending)
	{
		MessageBox("��ֹͣ����");
		return;
	}

	if(!NET_DVR_SerialStop(m_lSerialHandle))
	{
		MessageBox("�ر�͸��ͨ��ʧ��");
	}

    GetDlgItem(IDC_BUTTON_SERIALSTART)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_SERIALSTOP)->EnableWindow(FALSE);
	m_lSerialHandle = -1;
}
