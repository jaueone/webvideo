// DlgExceptionCfg.cpp : implementation file
//
/**********************************************************
FileName:    DlgExceptionCfg.cpp
Description: �쳣��Ϣ����      
Date:        
Note: 		<ȫ��>�ṹ��,���GeneralDef.h, ȫ�ֱ���,������config.cpp   
Modification History:      
<version>   <time>         <desc>

***********************************************************/

#include "stdafx.h"
#include "config.h"
#include "DlgExceptionCfg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgExceptionCfg dialog


CDlgExceptionCfg::CDlgExceptionCfg(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgExceptionCfg::IDD, pParent)
	, m_bChkMonitorAlarm(FALSE)
	, m_bChkVoiceAlarm(FALSE)
	, m_bChkUploadCenter(FALSE)
	, m_bChkInvokeAlarmOut(FALSE)
	, m_bChkInvokeJpegCapture(FALSE)
	, m_iDeviceIndex(-1)
	, m_lLoginID(-1)
	, m_dwAlarmOutNum(0)
	, m_iExceptionType(0)
	, m_bSetPara(FALSE)
{
	memset(&m_struExceptionInfo, 0, sizeof(m_struExceptionInfo));
	memset(&m_bAlarmOut[0], 0, sizeof(BOOL)*MAX_ALARMOUT_V30);
}

void CDlgExceptionCfg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgExceptionCfg)
	DDX_Control(pDX, IDC_TREE_ALARM_OUT, m_treeAlarmOut);
	DDX_Control(pDX, IDC_COMBO_EXCEPTION_TYPE, m_comboExceptionType);
	DDX_Check(pDX, IDC_CHK_EXCEPTION_MONITOR_ALARM, m_bChkMonitorAlarm);
	DDX_Check(pDX, IDC_CHK_EXCEPTION_AUDIO_ALARM, m_bChkVoiceAlarm);
	DDX_Check(pDX, IDC_CHK_EXCEPTION_REPORT_CENTER, m_bChkUploadCenter);
	DDX_Check(pDX, IDC_CHK_EXCEPTION_INVOKE_ALARM_OUT, m_bChkInvokeAlarmOut);
	DDX_Check(pDX, IDC_CHK_INVOKE_JPEG_CAPTURE, m_bChkInvokeJpegCapture);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgExceptionCfg, CDialog)
	//{{AFX_MSG_MAP(CDlgExceptionCfg)
	ON_BN_CLICKED(IDC_BTN_EXCEPTION_OK, OnBnClickedBtnExceptionOk)
	ON_BN_CLICKED(IDC_CHK_EXCEPTION_INVOKE_ALARM_OUT, OnBnClickedChkExceptionInvokeAlarmOut)
	ON_CBN_SELCHANGE(IDC_COMBO_EXCEPTION_TYPE, OnCbnSelchangeComboExceptionType)
	ON_NOTIFY(NM_CLICK, IDC_TREE_ALARM_OUT, OnClickTreeAlarmOut)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgExceptionCfg message handlers


/*********************************************************
	Function:	CheckInitParam
	Desc:		�����豸�Ĳ���,����ʼ���Ի������ز���
	Input:	
	Output:	
	Return:	TRUE,�豸������ͬ,����ȷ,��ʼ���Ի������;FALSE,����ʼ��
**********************************************************/
BOOL CDlgExceptionCfg::CheckInitParam()
{
	int iDeviceIndex = 0;

	if (iDeviceIndex == -1)
	{
		return FALSE;
	}

	m_iDeviceIndex = iDeviceIndex;
	m_lLoginID = g_struDeviceInfo[m_iDeviceIndex].lLoginID;
	m_dwAlarmOutNum = g_struDeviceInfo[m_iDeviceIndex].iAlarmOutNum;

	
	m_comboExceptionType.SetCurSel(-1);
	m_bChkMonitorAlarm = FALSE;
	m_bChkVoiceAlarm = FALSE;
	m_bChkUploadCenter = FALSE;
	m_bChkInvokeAlarmOut = FALSE;
	m_bChkInvokeJpegCapture = FALSE;
	if (m_lLoginID < 0)
	{
		return FALSE;
	}
	DWORD dwReturned;
	CString sTemp;
	char szLan[128] = {0};
	m_iExceptionType = 0;
	m_comboExceptionType.SetCurSel(m_iExceptionType);
	if (!NET_DVR_GetDVRConfig(m_lLoginID, NET_DVR_GET_EXCEPTIONCFG_V30, 0, &m_struExceptionInfo, sizeof(NET_DVR_EXCEPTION_V30), &dwReturned))
	{
		g_StringLanType(szLan, "��ȡ�쳣����ʧ��", "Get exception configure failed");
		AfxMessageBox(szLan);
		return FALSE;
	}
	return TRUE;
}

/*********************************************************
Function:	CurCfgUpdate
Desc:		���¶�Ӧ�豸�Ĳ���,��ˢ�¶�Ӧ�Ŀؼ�
Input:	
Output:	
Return:	
**********************************************************/
void CDlgExceptionCfg::CurCfgUpdate() 
{
	if (!CheckInitParam())
	{
		
		EnableWindow(FALSE);
		UpdateData(FALSE);
		return;
	}
	int i = 0;
	EnableWindow(TRUE);

	m_bChkMonitorAlarm = m_struExceptionInfo.struExceptionHandleType[m_iExceptionType].dwHandleType &0x01;
	m_bChkVoiceAlarm = (m_struExceptionInfo.struExceptionHandleType[m_iExceptionType].dwHandleType>>1)&0x01;
	m_bChkUploadCenter = (m_struExceptionInfo.struExceptionHandleType[m_iExceptionType].dwHandleType>>2)&0x01;
	m_bChkInvokeAlarmOut = (m_struExceptionInfo.struExceptionHandleType[m_iExceptionType].dwHandleType>>3)&0x01;
	m_bChkInvokeJpegCapture = (m_struExceptionInfo.struExceptionHandleType[m_iExceptionType].dwHandleType>>4)&0x01;
	m_treeAlarmOut.EnableWindow(m_bChkInvokeAlarmOut);
	for (i=0; i<MAX_ALARMOUT_V30; i++)
	{
		m_bAlarmOut[i] = m_struExceptionInfo.struExceptionHandleType[m_iExceptionType].byRelAlarmOut[i];
	}
	CreateTree();
	UpdateData(FALSE);
}

/*********************************************************
  Function:	OnBnClickedBtnExceptionOk
  Desc:		ȷ���쳣��������
  Input:	
  Output:	
  Return:	
**********************************************************/
void CDlgExceptionCfg::OnBnClickedBtnExceptionOk()
{
	if (g_lUserID < 0)
	{
		MessageBox("���ȵ�¼�豸��");
		return;
	}

	UpdateData(TRUE);
	int i=0;
	m_iExceptionType = m_comboExceptionType.GetCurSel();
	m_struExceptionInfo.struExceptionHandleType[m_iExceptionType].dwHandleType = 0;
	m_struExceptionInfo.struExceptionHandleType[m_iExceptionType].dwHandleType |= (m_bChkMonitorAlarm << 0);
	m_struExceptionInfo.struExceptionHandleType[m_iExceptionType].dwHandleType |= (m_bChkVoiceAlarm << 1);
	m_struExceptionInfo.struExceptionHandleType[m_iExceptionType].dwHandleType |= (m_bChkUploadCenter << 2);
	m_struExceptionInfo.struExceptionHandleType[m_iExceptionType].dwHandleType |= (m_bChkInvokeAlarmOut << 3);
	m_struExceptionInfo.struExceptionHandleType[m_iExceptionType].dwHandleType |= (m_bChkInvokeJpegCapture << 4);
	for (i=0; i<MAX_ALARMOUT_V30; i++)
	{
		m_struExceptionInfo.struExceptionHandleType[m_iExceptionType].byRelAlarmOut[i] = m_bAlarmOut[i];
	}
	
	m_bSetPara = TRUE;

	if (!NET_DVR_SetDVRConfig(m_lLoginID,NET_DVR_SET_EXCEPTIONCFG_V30, 0, &(m_struExceptionInfo), sizeof(NET_DVR_EXCEPTION_V30)))
	{
		char szLan[128] = {0};
		g_StringLanType(szLan, "��������ʧ��", "Save parameter failed");
		AfxMessageBox(szLan);
	}


}

/*********************************************************
  Function:	OnBnClickedChkExceptionInvokeAlarmOut
  Desc:		�쳣��Ϣ�����澯�����״̬ʹ��
  Input:	
  Output:	
  Return:	
**********************************************************/
void CDlgExceptionCfg::OnBnClickedChkExceptionInvokeAlarmOut()
{
	UpdateData(TRUE);
	m_treeAlarmOut.EnableWindow(m_bChkInvokeAlarmOut);
}

/*********************************************************
  Function:	OnCbnSelchangeComboExceptionType
  Desc:		�쳣��Ϣ���Ͷ�Ӧ�������ؼ���ˢ��
  Input:	
  Output:	
  Return:	
**********************************************************/
void CDlgExceptionCfg::OnCbnSelchangeComboExceptionType()
{
	int i = 0;	
	for (i=0; i<MAX_ALARMOUT_V30; i++)
	{
		m_struExceptionInfo.struExceptionHandleType[m_iExceptionType].byRelAlarmOut[i] = m_bAlarmOut[i];
	}
	
	UpdateData(TRUE);

	m_iExceptionType = m_comboExceptionType.GetCurSel();
	m_bChkMonitorAlarm = m_struExceptionInfo.struExceptionHandleType[m_iExceptionType].dwHandleType &0x01;
	m_bChkVoiceAlarm = (m_struExceptionInfo.struExceptionHandleType[m_iExceptionType].dwHandleType>>1)&0x01;
	m_bChkUploadCenter = (m_struExceptionInfo.struExceptionHandleType[m_iExceptionType].dwHandleType>>2)&0x01;
	m_bChkInvokeAlarmOut = (m_struExceptionInfo.struExceptionHandleType[m_iExceptionType].dwHandleType>>3)&0x01;
	m_bChkInvokeJpegCapture = (m_struExceptionInfo.struExceptionHandleType[m_iExceptionType].dwHandleType>>4)&0x01;
	m_treeAlarmOut.EnableWindow(m_bChkInvokeAlarmOut);
	for (i=0; i<MAX_ALARMOUT_V30; i++)
	{
		m_bAlarmOut[i] = m_struExceptionInfo.struExceptionHandleType[m_iExceptionType].byRelAlarmOut[i];
	}
	CreateTree();
	UpdateData(FALSE);
}

/*********************************************************
  Function:	OnClickTreeAlarmOut
  Desc:		invoke the correponding alarm chan
  Input:	none
  Output:	none
  Return:	none
**********************************************************/
void CDlgExceptionCfg::OnClickTreeAlarmOut(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);
	CPoint pt(0,0);
	CRect rc(0,0,0,0);
	GetCursorPos(&pt);
	GetDlgItem(IDC_TREE_ALARM_OUT)->GetWindowRect(&rc);
	ScreenToClient(&rc);
	ScreenToClient(&pt);
	pt.x = pt.x - rc.left;
	pt.y = pt.y - rc.top;

	UINT uFlag = 0;
	HTREEITEM hSelect = m_treeAlarmOut.HitTest(pt, &uFlag);

	if (NULL == hSelect) 
	{
		return;
	}
	//g_ClickAlarmOutItem(m_treeAlarmOut, hSelect, uFlag);	
	m_treeAlarmOut.SelectItem(hSelect);
	DWORD dwIndex = m_treeAlarmOut.GetItemData(hSelect)%1000;
	BOOL bCheck = m_treeAlarmOut.GetCheck(hSelect);
	m_bAlarmOut[dwIndex] = !bCheck;
	//������ʱ��ı�checkbox��ѡ��״̬
	if (uFlag != LVHT_TOLEFT)
	{
		m_treeAlarmOut.SetCheck(hSelect, !bCheck);
	}
	else//LVHT_TOLEFT
	{
		m_treeAlarmOut.SetCheck(hSelect, bCheck);
	}

	*pResult = 0;
}


/*********************************************************
  Function:	CreateTree
  Desc:		�����澯���ͨ����
  Input:	
  Output:	
  Return:	
**********************************************************/
void CDlgExceptionCfg::CreateTree(void)
{
	g_CreateAlarmOutTree(m_treeAlarmOut, m_iDeviceIndex, m_bAlarmOut);
}

/*********************************************************
  Function:	OnPaint
  Desc:		paint the tree
  Input:	none
  Output:	none
  Return:	none
**********************************************************/
void CDlgExceptionCfg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CreateTree();
	// Do not call CDialog::OnPaint() for painting messages
}

