// DlgAlarmHostTransparent.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgAlarmHostTransparent.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAlarmHostTransparent dialog
CDlgAlarmHostTransparent *g_pDlgAlarmHostTransparent = NULL;

void CALLBACK g_fAlarmHostSerialDataCallBack(LONG lSerialHandle, LONG lPort, LONG lDateType, char *pRecvDataBuffer, DWORD  dwBufSize,void *pUser)
{
    
    CString tmp1 = "";
    CString tmp2 = "";
    char ch[1024] = {0};
    CTime ct;
    if (NULL != g_pDlgAlarmHostTransparent)
    {
        int nIndex = 0;
        ct = CTime::GetCurrentTime();
        tmp1.Format("%d-%d-%d %d:%d:%d", ct.GetYear(), ct.GetMonth(), ct.GetDay(), ct.GetHour(),
            ct.GetMinute(), ct.GetSecond());
        int nItem = g_pDlgAlarmHostTransparent->m_listRecvData.GetItemCount();
        nItem = g_pDlgAlarmHostTransparent->m_listRecvData.InsertItem(nItem, tmp1, 0);
        nIndex++;            
        tmp1.Format("%d", lPort);
        g_pDlgAlarmHostTransparent->m_listRecvData.SetItemText(nItem, nIndex, tmp1);
        nIndex++;
        if (0 == lDateType)
        {
            tmp1 = "Date";
        }
        else if (1 == lDateType)
        {
            tmp1 = "Fail to send";
        }
        g_pDlgAlarmHostTransparent->m_listRecvData.SetItemText(nItem, nIndex, tmp1);
        nIndex++;

        memcpy(ch, pRecvDataBuffer, dwBufSize);

        g_pDlgAlarmHostTransparent->m_listRecvData.SetItemText(nItem, nIndex, ch);
        nIndex++;
        for (int i=0; i<(int)dwBufSize; i++)
        {
            tmp2.Format("0x%x ", (BYTE)pRecvDataBuffer[i]);
            tmp1 += tmp2;
        }
        g_pDlgAlarmHostTransparent->m_listRecvData.SetItemText(nItem, nIndex, tmp1);
        nIndex++;
            
    }

}


CDlgAlarmHostTransparent::CDlgAlarmHostTransparent(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAlarmHostTransparent::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAlarmHostTransparent)
	m_csData = _T("");
	//}}AFX_DATA_INIT
    m_lServerID = -1;
    memset(&m_struDevAbility, 0, sizeof(m_struDevAbility));
    m_lSerialHandle = -1;

}


void CDlgAlarmHostTransparent::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAlarmHostTransparent)
	DDX_Control(pDX, IDC_LISTRECVDATA, m_listRecvData);
	DDX_Control(pDX, IDC_COMBO_CHAN, m_comboChan);
	DDX_Control(pDX, IDC_COMBO_SERIAL_TYPE, m_comboSerialType);
	DDX_Text(pDX, IDC_EDIT_DATA, m_csData);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAlarmHostTransparent, CDialog)
	//{{AFX_MSG_MAP(CDlgAlarmHostTransparent)
	ON_BN_CLICKED(IDC_CREATECHANNEL, OnCreatechannel)
	ON_BN_CLICKED(IDC_DESTROYCHANNEL, OnDestroychannel)
	ON_CBN_SELCHANGE(IDC_COMBO_SERIAL_TYPE, OnSelchangeComboSerialType)
	ON_BN_CLICKED(IDC_BTN_SEND_DATA, OnBtnSendData)
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBtnExit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAlarmHostTransparent message handlers

void CDlgAlarmHostTransparent::OnCreatechannel() 
{
    //LONG lUserID, LONG lSerialType, fAlarmHostSerialDataCallBack cbSerialDataCallBack, void *pUser
    LONG lSerialHandle = -1;
    LONG lSerialType = m_comboSerialType.GetItemData(m_comboSerialType.GetCurSel());
	if (m_lSerialHandle >= 0)
	{
		AfxMessageBox("Already start a Direct Transmission");
		return;
	}
    lSerialHandle = NET_DVR_AlarmHostSerialStart(m_lServerID, lSerialType, g_fAlarmHostSerialDataCallBack, &m_lServerID);
    if (lSerialHandle == -1)
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_AlarmHostSerialStart serial[%d]", lSerialHandle);
        AfxMessageBox("Start Direct Transmission Failed");
        return;
    }
    g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_AlarmHostSerialStart serial[%d]", lSerialHandle);
    m_lSerialHandle = lSerialHandle;
}


// /******************************************
// ����:	fAlarmHostSerialDataCallBack
// ����:	����͸��ͨ���ص�����ָ��
// ����:	lSerialHandle NET_DVR_AlarmHostSerialStart����ֵ
// 		lPort 485�ڽ������ݵĲ�λ�� ��1��ʼ  232 �ò�����Ч
// 		lDateType �������� 0-��ʾ���ݽ������� 1-��ʾͨ����֧��͸��ͨ�������485�ڣ�
// 		pRecvDataBuffer ������ݵĻ�����ָ��
// 		pUser �û�����
// ���:	��
// ����ֵ: ��
// ******************************************/
// typedef void(CALLBACK *fAlarmHostSerialDataCallBack)
// (LONG lSerialHandle, LONG lPort, LONG lDateType, char *pRecvDataBuffer, DWORD  dwBufSize,void *pUser);
// 
// /******************************************
// ����:	NET_DVR_AlarmHostSerialStart
// ����:	�ͱ��������豸����͸��ͨ��
// ����:	lUserID NET_DVR_Login����ֵ
// 		lSerialType ����͸��ͨ������  1 - ����232ͨ�� 2 - ����485ͨ�� 
// 		cbSerialDataCallBack ָ��fAlarmHostSerialDataCallBack����ָ�룬���ڽ���͸��ͨ������
// 		dwUser  �û�����
// ���:	��
// ����ֵ: -1 ��ʾʧ�� ����ֵ��ΪNET_DVR_AlarmHostSerialSend�Ⱥ������
// ******************************************/
// NET_DVR_API LONG __stdcall NET_DVR_AlarmHostSerialStart(LONG lUserID, LONG lSerialType, fAlarmHostSerialDataCallBack cbSerialDataCallBack, void *pUser);
// 
// /******************************************
// ����:	NET_DVR_AlarmHostSerialSend
// ����:	��͸��ͨ��ָ����485�ڻ���232�ڷ�������
// ����:	lSerialHandle NET_DVR_AlarmHostSerialStart����ֵ
// 		lPort ��Ϊ485͸��ͨ��ʱʹ�ã�485�ڵĲ�λ�ţ���1��ʼ
// 		pSendBuf ָ�������ݻ�������ָ��
// 		dwBufSize �������ݵĴ�С
// ���:	��
// ����ֵ: �ɹ�-TURE ʧ��-FALSE
// ******************************************/
// NET_DVR_API BOOL __stdcall NET_DVR_AlarmHostSerialSend(LONG lSerialHandle,LONG lPort,char *pSendBuf,DWORD dwBufSize);
// 
// /******************************************
// ����:	NET_DVR_AlarmHostSerialStop
// ����:	�ر�͸��ͨ��
// ����:	lSerialHandle NET_DVR_AlarmHostSerialStart����ֵ
// ���:	��
// ����ֵ: �ɹ�-TRUE ʧ��-FALSE
// ******************************************/
// NET_DVR_API BOOL __stdcall NET_DVR_AlarmHostSerialStop(LONG  lSerialHandle);

BOOL CDlgAlarmHostTransparent::OnInitDialog() 
{
	CDialog::OnInitDialog();
    g_pDlgAlarmHostTransparent = this;
	InitWnd();
	m_comboSerialType.SetCurSel(1);
    OnSelchangeComboSerialType();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAlarmHostTransparent::InitWnd()
{
    InitSerialTypeCombo();
    InitRecvDataList();
}

void CDlgAlarmHostTransparent::InitSerialTypeCombo()
{
    int nIndex = 0;
    m_comboSerialType.ResetContent();
    
    m_comboSerialType.InsertString(nIndex, "Rs232");
    m_comboSerialType.SetItemData(nIndex, 1);
    nIndex++;
    
    m_comboSerialType.InsertString(nIndex, "RS485");
    m_comboSerialType.SetItemData(nIndex, 2);
    nIndex++;
}

void CDlgAlarmHostTransparent::InitChannelCombo(int nChanNum)
{
    int nIndex = 0;
    m_comboChan.ResetContent();
    char szLan[128] = {0};
    for (nIndex = 0; nIndex < nChanNum; nIndex++)
    {
        sprintf(szLan, "Rs485 %d", nIndex+1);
        m_comboChan.AddString(szLan);
    }
    m_comboChan.SetCurSel(0);
}

void CDlgAlarmHostTransparent::InitRecvDataList()
{
    m_listRecvData.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
    m_listRecvData.InsertColumn(0, "Time", LVCFMT_LEFT, 100, 0);
    m_listRecvData.InsertColumn(1, "Port", LVCFMT_LEFT, 50, 0);
    m_listRecvData.InsertColumn(2, "Status", LVCFMT_LEFT, 50, 0);
    m_listRecvData.InsertColumn(3, "Content", LVCFMT_LEFT, 200, 0);
    m_listRecvData.InsertColumn(4, "Hex", LVCFMT_LEFT, 450, 0);
}

void CDlgAlarmHostTransparent::OnDestroychannel() 
{
    if (NET_DVR_AlarmHostSerialStop(m_lSerialHandle))
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_AlarmHostSerialStop");
		m_lSerialHandle = -1;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_AlarmHostSerialStop");
    }
}

void CDlgAlarmHostTransparent::OnSelchangeComboSerialType() 
{
	CString csStr;
	if (m_comboSerialType.GetItemData(m_comboSerialType.GetCurSel()) == 1) //232
	{   
		m_comboChan.ResetContent();
		for (int i=0; i<MAX_SERIAL_PORT; i++)
		{
			csStr.Format("RS232 %d", i+1);
			m_comboChan.AddString(csStr);
		}
		m_comboChan.SetCurSel(0);
	}
    if (m_comboSerialType.GetItemData(m_comboSerialType.GetCurSel()) == 2) // 485
    {
        InitChannelCombo(m_struDevAbility.wLocalRs485Num + m_struDevAbility.wExpandRs485Num);
    }
}

void CDlgAlarmHostTransparent::OnBtnSendData() 
{
    char szLan[128] = {0};

    if (m_lSerialHandle < 0)
    {
        g_StringLanType(szLan, "���Ƚ���͸��ͨ��", "Please create serial transparent");
        AfxMessageBox(szLan);

        return;
    }
    
    UpdateData(TRUE);
    if (AlarmHostSerialSend())
    {
        m_csData.Empty();
    }
    else
    {
        AfxMessageBox("Fail to send data");
    }
        // NET_DVR_AlarmHostSerialStop
}


BOOL CDlgAlarmHostTransparent::AlarmHostSerialSend()
{
    if (NET_DVR_AlarmHostSerialSend(m_lSerialHandle, m_comboChan.GetCurSel() + 1, m_csData.GetBuffer(0), m_csData.GetLength()))
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_AlarmHostSerialSend");
        return TRUE;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_AlarmHostSerialSend");
        return FALSE;
    }
}

void CDlgAlarmHostTransparent::OnBtnExit() 
{
	OnCancel();
	
}

void CDlgAlarmHostTransparent::PostNcDestroy() 
{
    if (m_lSerialHandle >= 0)
    {
        NET_DVR_AlarmHostSerialStop(m_lSerialHandle);
    }
    	
	CDialog::PostNcDestroy();
}
