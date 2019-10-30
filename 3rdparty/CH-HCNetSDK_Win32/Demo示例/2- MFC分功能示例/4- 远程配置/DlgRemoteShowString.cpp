/**********************************************************
FileName:    DlgRemoteShowString.cpp
Description: Զ���ַ�����      
Date:       
Note: 		
Modification History:      
<version> <time>         <desc>
    
***********************************************************/


#include "stdafx.h"
#include "DlgRemoteShowString.h"
#include "config.h"


// CDlgRemoteShowString dialog
/*********************************************************
  Function:	CDlgRemoteShowString
  Desc:		Constructor
  Input:	
  Output:	
  Return:	
**********************************************************/
IMPLEMENT_DYNAMIC(CDlgRemoteShowString, CDialog)
CDlgRemoteShowString::CDlgRemoteShowString(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRemoteShowString::IDD, pParent)
	, m_bChkShow(FALSE)
	, m_iStringX(0)
	, m_iStringY(0)
	, m_csString(_T(""))
	, m_lLoginID(-1)
	, m_iChanShowNum(0)
	, dwReturned(0)
{
	memset(&m_struShowString, 0, sizeof(m_struShowString));
}

/*********************************************************
  Function:	~CDlgRemoteShowString
  Desc:		destructor
  Input:	
  Output:	
  Return:	
**********************************************************/
CDlgRemoteShowString::~CDlgRemoteShowString()
{
}
/*********************************************************
Function:	DoDataExchange
Desc:		the map between control and variable
Input:	
Output:	
Return:	
**********************************************************/
void CDlgRemoteShowString::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRemoteShowString)
	DDX_Control(pDX, IDC_COMBO_SHOW_AREA, m_comboShowArea);
	DDX_Check(pDX, IDC_CHK_SHOW, m_bChkShow);
	DDX_Text(pDX, IDC_EDIT_STRING_X, m_iStringX);
	DDX_Text(pDX, IDC_EDIT_STRING_Y, m_iStringY);
	DDX_Text(pDX, IDC_EDIT_STRING, m_csString);
    //}}AFX_DATA_MAP	
}


/*********************************************************
Function:	BEGIN_MESSAGE_MAP
Desc:		the map between control and function
Input:	
Output:	
Return:	
**********************************************************/
BEGIN_MESSAGE_MAP(CDlgRemoteShowString, CDialog)
	//{{AFX_MSG_MAP(CDlgRemoteShowString)	
	ON_BN_CLICKED(IDC_BTN_STRING_OK, OnBnClickedBtnStringOk)
	ON_BN_CLICKED(IDC_BTN_STRING_EXIT, OnBnClickedBtnStringExit)
	ON_BN_CLICKED(IDC_BTN_STRING_SET, OnBnClickedBtnStringSet)
	ON_CBN_SELCHANGE(IDC_COMBO_SHOW_AREA, OnCbnSelchangeComboShowArea)
	ON_BN_CLICKED(IDC_CHK_SHOW, OnBnClickedChkShow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*********************************************************
  Function:	OnInitDialog
  Desc:		Initialize the dialog
  Input:	none
  Output:	none
  Return:	none
**********************************************************/
BOOL CDlgRemoteShowString::OnInitDialog() 
{
	CDialog::OnInitDialog();
	char szLan[128] = {0};
	m_comboShowArea.SetCurSel(0);
	if (!NET_DVR_GetDVRConfig(m_lLoginID, NET_DVR_GET_SHOWSTRING_V30, m_iChanShowNum, &m_struShowString, sizeof(NET_DVR_SHOWSTRING_V30), &dwReturned))
	{
//		g_pMainDlg->AddLog(g_pMainDlg->m_iCurDeviceIndex, OPERATION_FAIL_T, "NET_DVR_GET_SHOWSTRING_V30");
		GetDlgItem(IDC_BTN_STRING_OK)->EnableWindow(FALSE);	
		g_StringLanType(szLan, "��ȡ�ַ�����ʧ��", "Failure to obtain character parameters");
		AfxMessageBox(szLan);
		return FALSE;
	}
	else
	{
		m_bChkShow = m_struShowString.struStringInfo[0].wShowString;
		EnableString(m_bChkShow);
		m_csString.Format("%s", m_struShowString.struStringInfo[0].sString);
		m_iStringX = m_struShowString.struStringInfo[0].wShowStringTopLeftX;
		m_iStringY = m_struShowString.struStringInfo[0].wShowStringTopLeftY;
	}
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

/*********************************************************
  Function:	OnBnClickedBtnStringOk
  Desc:		����ַ����ӵ�����
  Input:	
  Output:	
  Return:	
**********************************************************/
void CDlgRemoteShowString::OnBnClickedBtnStringOk()
{
	UpdateData(TRUE);
	char szLan[128] = {0};
	int i = m_comboShowArea.GetCurSel();
	m_struShowString.struStringInfo[i].wShowString = m_bChkShow;
	memcpy(m_struShowString.struStringInfo[i].sString, m_csString, 44);
	m_struShowString.struStringInfo[i].wStringSize = strlen(m_csString);
	m_struShowString.struStringInfo[i].wShowStringTopLeftX = m_iStringX;
	m_struShowString.struStringInfo[i].wShowStringTopLeftY = m_iStringY;	
	if (!NET_DVR_SetDVRConfig(m_lLoginID, NET_DVR_SET_SHOWSTRING_V30, m_iChanShowNum, &m_struShowString, sizeof(NET_DVR_SHOWSTRING_V30)))
	{
//		g_pMainDlg->AddLog(g_pMainDlg->m_iCurDeviceIndex, OPERATION_FAIL_T, "NET_DVR_SET_SHOWSTRING_V30");
		g_StringLanType(szLan, "�����ַ�����ʧ��", "Failure to set character parameters");
		AfxMessageBox(szLan);
		return ;
	}
//	g_pMainDlg->AddLog(g_pMainDlg->m_iCurDeviceIndex, OPERATION_SUCC_T, "NET_DVR_SET_SHOWSTRING_V30");
}

/*********************************************************
  Function:	OnBnClickedBtnStringExit
  Desc:		ȡ������ַ����ӵ�����
  Input:	
  Output:	
  Return:	
**********************************************************/
void CDlgRemoteShowString::OnBnClickedBtnStringExit()
{
	CDialog::OnCancel();
}

/*********************************************************
  Function:	OnBnClickedBtnStringSet
  Desc:		���Ķ�Ӧ������ַ�����
  Input:	
  Output:	
  Return:	
**********************************************************/
void CDlgRemoteShowString::OnBnClickedBtnStringSet()
{
	UpdateData(TRUE);
	int i = m_comboShowArea.GetCurSel();
	m_struShowString.struStringInfo[i].wShowString = m_bChkShow;
	memcpy(m_struShowString.struStringInfo[i].sString, m_csString, 44);
	m_struShowString.struStringInfo[i].wStringSize = m_csString.GetLength();//strlen(m_csString)
	m_struShowString.struStringInfo[i].wShowStringTopLeftX = m_iStringX;
	m_struShowString.struStringInfo[i].wShowStringTopLeftY = m_iStringY;	
}

/*********************************************************
  Function:	OnCbnSelchangeComboShowArea
  Desc:		ѡ���Ӧ������ַ�����
  Input:	
  Output:	
  Return:	
**********************************************************/
void CDlgRemoteShowString::OnCbnSelchangeComboShowArea()
{
	UpdateData(TRUE);
	int i = m_comboShowArea.GetCurSel();
	m_bChkShow = m_struShowString.struStringInfo[i].wShowString;
	EnableString(m_bChkShow);
	m_csString.Format("%s", m_struShowString.struStringInfo[i].sString);
	m_iStringX = m_struShowString.struStringInfo[i].wShowStringTopLeftX;
	m_iStringY = m_struShowString.struStringInfo[i].wShowStringTopLeftY;	
	UpdateData(FALSE); 
}

/*********************************************************
  Function:	OnBnClickedChkShow
  Desc:		��ǰ�ַ�����ʾ״̬
  Input:	
  Output:	
  Return:	
**********************************************************/
void CDlgRemoteShowString::OnBnClickedChkShow()
{
	UpdateData(TRUE);
	EnableString(m_bChkShow);
}

/*********************************************************
  Function:	EnableString
  Desc:		��ǰstring��״̬�ĸı�
  Input:	bFlag,TRUE/FALSE
  Output:	
  Return:	
**********************************************************/
void CDlgRemoteShowString::EnableString(BOOL bFlag)
{
	GetDlgItem(IDC_EDIT_STRING)->EnableWindow(m_bChkShow);
	GetDlgItem(IDC_EDIT_STRING_X)->EnableWindow(m_bChkShow);
	GetDlgItem(IDC_EDIT_STRING_Y)->EnableWindow(m_bChkShow);
}
