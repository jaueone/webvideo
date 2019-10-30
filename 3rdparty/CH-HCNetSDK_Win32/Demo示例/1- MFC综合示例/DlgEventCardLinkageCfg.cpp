// DlgEventCardLinkageCfg.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgEventCardLinkageCfg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgEventCardLinkageCfg dialog


DlgEventCardLinkageCfg::DlgEventCardLinkageCfg(CWnd* pParent /*=NULL*/)
	: CDialog(DlgEventCardLinkageCfg::IDD, pParent)
    , m_iLocalController(0)
    , m_dwEventID(1)
{
	//{{AFX_DATA_INIT(DlgEventCardLinkageCfg)
    m_byProMode = FALSE;
    m_byEmployeeNo = FALSE;
    m_byMacAddr = FALSE;
    byMainDevBuzzer = FALSE;
    m_sCardNo = _T("");
    m_sEmployeeNo = _T("");
    m_sMacAddr = _T("");
    m_dwEventSourceID = 0;
	m_bLinkCapPic = FALSE;
    m_bLinkRecordVideo = FALSE;
    m_bMainDevStopBuzzer = FALSE;
    m_dwAudioDisplayID = 0;
	//}}AFX_DATA_INIT
    memset(&m_struEventCardLinkageCfgV51, 0, sizeof(m_struEventCardLinkageCfgV51));
    m_struEventCardLinkageCfgV51.dwSize = sizeof(m_struEventCardLinkageCfgV51);
    memset(&m_struEventCardLinkageCond, 0, sizeof(m_struEventCardLinkageCond));
    m_struEventCardLinkageCond.dwSize = sizeof(m_struEventCardLinkageCond);
}


void DlgEventCardLinkageCfg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgEventCardLinkageCfg)
    DDX_Control(pDX, IDC_TREE_OPENDOOR, m_treeDoorOpen);
    DDX_Control(pDX, IDC_TREE_CLOSEDOOR, m_treeDoorClose);
    DDX_Control(pDX, IDC_TREE_READERBUZZER, m_treeCardReaderBuzzer);
    DDX_Control(pDX, IDC_TREE_READER_STOP_BUZZER, m_treeCardReaderStopBuzzer);
    DDX_Control(pDX, IDC_TREE_ALARMOUT, m_treeAlarmOut);
    DDX_Control(pDX, IDC_TREE_NORMALOPEN, m_treeNormalOpen);
    DDX_Control(pDX, IDC_TREE_NORMALCLOSE, m_treeNormalClose);
	DDX_Check(pDX, IDC_CHECK_PROMODE, m_byProMode);
    DDX_Check(pDX, IDC_CHECK_EMPLOYEE_NO, m_byEmployeeNo);
    DDX_Check(pDX, IDC_CHECK_MAC_ADDR, m_byMacAddr);
    DDX_Check(pDX, IDC_CHECK_BUZZER, byMainDevBuzzer);
    DDX_Control(pDX, IDC_COMBO_MAIN_TYPE, m_comboMainEventType);
    DDX_Control(pDX, IDC_COMBO_OTHER_TYPE, m_comboSubEventType);
	DDX_Text(pDX, IDC_EDIT_CARD, m_sCardNo);
    DDX_Text(pDX, IDC_EDIT_EMPLOYEE_NO, m_sEmployeeNo);
    DDX_Text(pDX, IDC_EDIT_MAC_ADDR, m_sMacAddr);
    DDX_Text(pDX, IDC_EDIT_EVENT_ID, m_dwEventSourceID);
	DDX_Check(pDX, IDC_CHK_LINKAGE_CAP_PIC, m_bLinkCapPic);
    DDX_Check(pDX, IDC_CHK_LINKAGE_RECORD_VIDEO, m_bLinkRecordVideo);
    DDX_Check(pDX, IDC_CHK_MAIN_DEV_STOP_BUZZER, m_bMainDevStopBuzzer);
	//}}AFX_DATA_MAP
    DDX_Text(pDX, IDC_EDIT_LOCAL_CONTROLLER, m_iLocalController);
    DDX_Control(pDX, IDC_TREE_ALARM_OUT_CLOSE, m_treeAlarmOutClose);
    DDX_Control(pDX, IDC_TREE_ALARM_IN_SETUP, m_treeAlarmInSetup);
    DDX_Control(pDX, IDC_TREE_ALARM_IN_CLOSE, m_treeAlarmInClose);
    DDX_Text(pDX, IDC_EDIT_EVENT, m_dwEventID);
    DDV_MinMaxUInt(pDX, m_dwEventID, 1, 500);
    DDX_Text(pDX, IDC_EDIT_AUDIO_DISPLAY_ID, m_dwAudioDisplayID);
    DDX_Control(pDX, IDC_CMB_AUDIO_DISPLAY_MODE, m_cmbAudioDisplayMode);
}


BEGIN_MESSAGE_MAP(DlgEventCardLinkageCfg, CDialog)
	//{{AFX_MSG_MAP(DlgEventCardLinkageCfg)
    ON_BN_CLICKED(IDC_BUTTON_SET, OnBtnSet)
    ON_BN_CLICKED(IDC_BUTTON_GET, OnBtnGet)
    ON_NOTIFY(NM_CLICK, IDC_TREE_READERBUZZER, OnClickTreeCardReaderBuzzer)
    ON_NOTIFY(NM_CLICK, IDC_TREE_ALARMOUT, OnClickTreeAlarmOut)
    ON_NOTIFY(NM_CLICK, IDC_TREE_OPENDOOR, OnClickTreeDoorOpen)
	ON_NOTIFY(NM_CLICK, IDC_TREE_CLOSEDOOR, OnClickTreeDoorClose)
    ON_NOTIFY(NM_CLICK, IDC_TREE_NORMALOPEN, OnClickTreeNormalOpen)
	ON_NOTIFY(NM_CLICK, IDC_TREE_NORMALCLOSE, OnClickTreeNormalClose)
    ON_CBN_SELCHANGE(IDC_COMBO_MAIN_TYPE, OnSelchangeEventMainType)
	//}}AFX_MSG_MAP
    ON_NOTIFY(NM_CLICK, IDC_TREE_ALARM_OUT_CLOSE, &DlgEventCardLinkageCfg::OnNMClickTreeAlarmOutClose)
    ON_NOTIFY(NM_CLICK, IDC_TREE_ALARM_IN_SETUP, &DlgEventCardLinkageCfg::OnNMClickTreeAlarmInSetup)
    ON_NOTIFY(NM_CLICK, IDC_TREE_ALARM_IN_CLOSE, &DlgEventCardLinkageCfg::OnNMClickTreeAlarmInClose)
    ON_NOTIFY(NM_CLICK, IDC_TREE_READER_STOP_BUZZER, &DlgEventCardLinkageCfg::OnClickTreeReaderStopBuzzer)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgEventCardLinkageCfg message handlers

BOOL DlgEventCardLinkageCfg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    m_iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
    m_lUserID = g_struDeviceInfo[m_iDeviceIndex].lLoginID;
    
    InitMainEventType();
    SubEventTypeDev();
    CreateTree();
    m_cmbAudioDisplayMode.SetCurSel(0);
    return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DlgEventCardLinkageCfg::OnBtnSet() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	GetTreeSel(); 
    char szLan[1024];
    m_struEventCardLinkageCfgV51.byCapturePic = m_bLinkCapPic;
    m_struEventCardLinkageCfgV51.byRecordVideo = m_bLinkRecordVideo;
    m_struEventCardLinkageCfgV51.byMainDevStopBuzzer = m_bMainDevStopBuzzer;
    m_struEventCardLinkageCfgV51.wAudioDisplayID = (WORD)m_dwAudioDisplayID;
    m_struEventCardLinkageCfgV51.byAudioDisplayMode = m_cmbAudioDisplayMode.GetCurSel();
    DWORD dwStatus = 0;
    m_struEventCardLinkageCond.dwEventID = m_dwEventID;
    m_struEventCardLinkageCond.wLocalControllerID = m_iLocalController;
    if (!NET_DVR_SetDeviceConfig(m_lUserID, NET_DVR_SET_EVENT_CARD_LINKAGE_CFG_V51, 1, &m_struEventCardLinkageCond, sizeof(m_struEventCardLinkageCond), &dwStatus, &m_struEventCardLinkageCfgV51, sizeof(m_struEventCardLinkageCfgV51)))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_SET_EVENT_CARD_LINKAGE_CFG_V51 Failed");
        g_StringLanType(szLan, "�����¼��򿨺�������������ʧ��", "Set event card linkage cfg Failed");
        MessageBox(szLan);
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_SET_EVENT_CARD_LINKAGE_CFG_V51 SUCC");
    }
}

void DlgEventCardLinkageCfg::OnBtnGet() 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE);
    DWORD dwReturned = 0;
    char szLan[1024];
    DWORD dwStatus = 0;
    m_struEventCardLinkageCond.dwEventID = m_dwEventID;
    m_struEventCardLinkageCond.wLocalControllerID = m_iLocalController;
    if (!NET_DVR_GetDeviceConfig(m_lUserID, NET_DVR_GET_EVENT_CARD_LINKAGE_CFG_V51, 1, &m_struEventCardLinkageCond, sizeof(m_struEventCardLinkageCond), &dwStatus, &m_struEventCardLinkageCfgV51, sizeof(m_struEventCardLinkageCfgV51)))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_GET_EVENT_CARD_LINKAGE_CFG_V51 Failed");
        g_StringLanType(szLan, "��ȡ�¼��򿨺�������������ʧ��", "Get event card linkage cfg Failed");
        MessageBox(szLan);
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_GET_EVENT_CARD_LINKAGE_CFG_V51 SUCC");
        if (m_struEventCardLinkageCfgV51.byProMode == 1)
        {
            m_byProMode = TRUE;
        }
        if (m_struEventCardLinkageCfgV51.byProMode == 2)
        {
            m_byMacAddr = TRUE;
        }
        if (m_struEventCardLinkageCfgV51.byProMode == 3)
        {
            m_byEmployeeNo = TRUE;
        }
        m_dwEventSourceID = m_struEventCardLinkageCfgV51.dwEventSourceID;
        byMainDevBuzzer = m_struEventCardLinkageCfgV51.byMainDevBuzzer;
        m_bLinkCapPic = m_struEventCardLinkageCfgV51.byCapturePic;
        m_bLinkRecordVideo = m_struEventCardLinkageCfgV51.byRecordVideo;
        m_bMainDevStopBuzzer = m_struEventCardLinkageCfgV51.byMainDevStopBuzzer;
        m_dwAudioDisplayID = m_struEventCardLinkageCfgV51.wAudioDisplayID;
        m_cmbAudioDisplayMode.SetCurSel(m_struEventCardLinkageCfgV51.byAudioDisplayMode);
        if (m_byProMode)
        {
            m_sCardNo.Format("%s", m_struEventCardLinkageCfgV51.uLinkageInfo.byCardNo);
        }
        if (m_byMacAddr)
        {
            m_sMacAddr.Format("%02X:%02X:%02X:%02X:%02X:%02X", m_struEventCardLinkageCfgV51.uLinkageInfo.byMACAddr[0], m_struEventCardLinkageCfgV51.uLinkageInfo.byMACAddr[1], \
                m_struEventCardLinkageCfgV51.uLinkageInfo.byMACAddr[2], m_struEventCardLinkageCfgV51.uLinkageInfo.byMACAddr[3], m_struEventCardLinkageCfgV51.uLinkageInfo.byMACAddr[4], \
                m_struEventCardLinkageCfgV51.uLinkageInfo.byMACAddr[5]);
        }
        if (m_byEmployeeNo)
        {
            m_sEmployeeNo.Format("%s", m_struEventCardLinkageCfgV51.uLinkageInfo.byEmployeeNo);
        }
        if (m_byProMode == FALSE && m_byMacAddr == FALSE && m_byEmployeeNo == FALSE)
        {
            m_comboMainEventType.SetCurSel(m_struEventCardLinkageCfgV51.uLinkageInfo.struEventLinkage.wMainEventType);

            DWORD dwMainEventType = m_comboMainEventType.GetCurSel();
            if (dwMainEventType == 0)
            {
                SubEventTypeDev();
            }
            else if (dwMainEventType == 1)
            {
                SubEventTypeAlarm();
            }
            else if (dwMainEventType == 2)
            {
                SubEventTypeDoor();
            }
            else if (dwMainEventType == 3)
            {
                SubEventTypeCardReader();
            }

            m_comboSubEventType.SetCurSel(m_struEventCardLinkageCfgV51.uLinkageInfo.struEventLinkage.wSubEventType);
        }
        UpdateData(FALSE);
        CreateTree();
    }
}

void DlgEventCardLinkageCfg::OnSelchangeEventMainType() 
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);
    DWORD dwMainEventType = m_comboMainEventType.GetCurSel();
    if (dwMainEventType == 0)
    {
        SubEventTypeDev();
    } 
    else if (dwMainEventType == 1)
    {
        SubEventTypeAlarm();
    }
    else if (dwMainEventType == 2)
    {
        SubEventTypeDoor();
    }
    else if (dwMainEventType == 3)
    {
        SubEventTypeCardReader();
    }

    UpdateData(FALSE);
}

void DlgEventCardLinkageCfg::CreateTree()
{
    CString strTemp =_T("");
    CString strChanTmp = _T("");
    int i = 0;
    m_treeCardReaderBuzzer.DeleteAllItems();
    m_treeCardReaderStopBuzzer.DeleteAllItems();
    m_treeAlarmOut.DeleteAllItems();
    
    HTREEITEM hChanItem = NULL;
    HTREEITEM hFirstItem = NULL;
       
    for (i = 0; i < sizeof(m_struEventCardLinkageCfgV51.byReaderBuzzer); i++)
    {
        strTemp.Format("Buzzer %d", i + 1);
        hChanItem = m_treeCardReaderBuzzer.InsertItem(strTemp, 0, 0, TVI_ROOT);
        if (hFirstItem == NULL)
        {
            hFirstItem = hChanItem;
        }

        m_treeCardReaderBuzzer.SetItemData(hChanItem, i);

        if (m_struEventCardLinkageCfgV51.byReaderBuzzer[i])
        {
            m_treeCardReaderBuzzer.SetCheck(hChanItem, TRUE);
        }
    }

    m_treeCardReaderBuzzer.SelectItem(hFirstItem);
    m_treeCardReaderBuzzer.Expand(m_treeCardReaderBuzzer.GetRootItem(), TVE_EXPAND);

    hChanItem = NULL;
    hFirstItem = NULL;

    for (i = 0; i < sizeof(m_struEventCardLinkageCfgV51.byReaderStopBuzzer); i++)
    {
        strTemp.Format("Buzzer %d", i + 1);
        hChanItem = m_treeCardReaderStopBuzzer.InsertItem(strTemp, 0, 0, TVI_ROOT);
        if (hFirstItem == NULL)
        {
            hFirstItem = hChanItem;
        }

        m_treeCardReaderStopBuzzer.SetItemData(hChanItem, i);

        if (m_struEventCardLinkageCfgV51.byReaderStopBuzzer[i])
        {
            m_treeCardReaderStopBuzzer.SetCheck(hChanItem, TRUE);
        }
    }

    m_treeCardReaderStopBuzzer.SelectItem(hFirstItem);
    m_treeCardReaderStopBuzzer.Expand(m_treeCardReaderStopBuzzer.GetRootItem(), TVE_EXPAND);

    hChanItem = NULL;
    hFirstItem = NULL;

    for (i = 0; i < sizeof(m_struEventCardLinkageCfgV51.byAlarmout); i++)
    {
        strTemp.Format("AlarmOut %d", i + 1);
        hChanItem = m_treeAlarmOut.InsertItem(strTemp, 0, 0, TVI_ROOT);
        if (hFirstItem == NULL)
        {
            hFirstItem = hChanItem;
        }

        m_treeAlarmOut.SetItemData(hChanItem, i);

        if (m_struEventCardLinkageCfgV51.byAlarmout[i])
        {
            m_treeAlarmOut.SetCheck(hChanItem, TRUE);
        }
    }

    m_treeAlarmOut.SelectItem(hFirstItem);
	m_treeAlarmOut.Expand(m_treeAlarmOut.GetRootItem(),TVE_EXPAND);

    hChanItem = NULL;
    hFirstItem = NULL;
    m_treeDoorOpen.DeleteAllItems();    
    for (i = 0; i < sizeof(m_struEventCardLinkageCfgV51.byOpenDoor); i++)
    {
        strTemp.Format("Door %d", i + 1);
        hChanItem = m_treeDoorOpen.InsertItem(strTemp, 0, 0, TVI_ROOT);
        if (hFirstItem == NULL)
        {
            hFirstItem = hChanItem;
        }
        
        m_treeDoorOpen.SetItemData(hChanItem, i);
        
        if (m_struEventCardLinkageCfgV51.byOpenDoor[i])
        {
            m_treeDoorOpen.SetCheck(hChanItem, TRUE);
        }
    }
    m_treeDoorOpen.SelectItem(hFirstItem);
	m_treeDoorOpen.Expand(m_treeDoorOpen.GetRootItem(),TVE_EXPAND);


    hChanItem = NULL;
    hFirstItem = NULL;
    m_treeDoorClose.DeleteAllItems();
    for (i = 0; i < sizeof(m_struEventCardLinkageCfgV51.byCloseDoor); i++)
    {
        strTemp.Format("Door %d", i + 1);
        hChanItem = m_treeDoorClose.InsertItem(strTemp, 0, 0, TVI_ROOT);
        if (hFirstItem == NULL)
        {
            hFirstItem = hChanItem;
        }      
        m_treeDoorClose.SetItemData(hChanItem, i);
        
        if (m_struEventCardLinkageCfgV51.byCloseDoor[i])
        {
            m_treeDoorClose.SetCheck(hChanItem, TRUE);
        }
    }
    m_treeDoorClose.SelectItem(hFirstItem);
	m_treeDoorClose.Expand(m_treeDoorClose.GetRootItem(),TVE_EXPAND);

    hChanItem = NULL;
    hFirstItem = NULL;
    m_treeNormalOpen.DeleteAllItems();    
    for (i = 0; i < sizeof(m_struEventCardLinkageCfgV51.byNormalOpen); i++)
    {
        strTemp.Format("Door %d", i + 1);
        hChanItem = m_treeNormalOpen.InsertItem(strTemp, 0, 0, TVI_ROOT);
        if (hFirstItem == NULL)
        {
            hFirstItem = hChanItem;
        }
        
        m_treeNormalOpen.SetItemData(hChanItem, i);
        
        if (m_struEventCardLinkageCfgV51.byNormalOpen[i])
        {
            m_treeNormalOpen.SetCheck(hChanItem, TRUE);
        }
    }
    m_treeNormalOpen.SelectItem(hFirstItem);
	m_treeNormalOpen.Expand(m_treeNormalOpen.GetRootItem(),TVE_EXPAND);

    hChanItem = NULL;
    hFirstItem = NULL;
    m_treeNormalClose.DeleteAllItems();    
    for (i = 0; i < sizeof(m_struEventCardLinkageCfgV51.byNormalClose); i++)
    {
        strTemp.Format("Door %d", i + 1);
        hChanItem = m_treeNormalClose.InsertItem(strTemp, 0, 0, TVI_ROOT);
        if (hFirstItem == NULL)
        {
            hFirstItem = hChanItem;
        }
        
        m_treeNormalClose.SetItemData(hChanItem, i);
        
        if (m_struEventCardLinkageCfgV51.byNormalClose[i])
        {
            m_treeNormalClose.SetCheck(hChanItem, TRUE);
        }
    }
    m_treeNormalClose.SelectItem(hFirstItem);
    m_treeNormalClose.Expand(m_treeNormalClose.GetRootItem(),TVE_EXPAND);

    hChanItem = NULL;
    hFirstItem = NULL;
    m_treeAlarmOutClose.DeleteAllItems();
    for (i = 0; i < sizeof(m_struEventCardLinkageCfgV51.byAlarmOutClose); i++)
    {
        strTemp.Format("AlarmOut %d", i + 1);
        hChanItem = m_treeAlarmOutClose.InsertItem(strTemp, 0, 0, TVI_ROOT);
        if (hFirstItem == NULL)
        {
            hFirstItem = hChanItem;
        }

        m_treeAlarmOutClose.SetItemData(hChanItem, i);

        if (m_struEventCardLinkageCfgV51.byAlarmOutClose[i])
        {
            m_treeAlarmOutClose.SetCheck(hChanItem, TRUE);
        }
    }
    m_treeAlarmOutClose.SelectItem(hFirstItem);
    m_treeAlarmOutClose.Expand(m_treeAlarmOutClose.GetRootItem(), TVE_EXPAND);

    hChanItem = NULL;
    hFirstItem = NULL;
    m_treeAlarmInSetup.DeleteAllItems();
    for (i = 0; i < sizeof(m_struEventCardLinkageCfgV51.byAlarmInSetup); i++)
    {
        strTemp.Format("AlarmIn %d", i + 1);
        hChanItem = m_treeAlarmInSetup.InsertItem(strTemp, 0, 0, TVI_ROOT);
        if (hFirstItem == NULL)
        {
            hFirstItem = hChanItem;
        }

        m_treeAlarmInSetup.SetItemData(hChanItem, i);

        if (m_struEventCardLinkageCfgV51.byAlarmInSetup[i])
        {
            m_treeAlarmInSetup.SetCheck(hChanItem, TRUE);
        }
    }
    m_treeAlarmInSetup.SelectItem(hFirstItem);
    m_treeAlarmInSetup.Expand(m_treeAlarmInSetup.GetRootItem(), TVE_EXPAND);

    hChanItem = NULL;
    hFirstItem = NULL;
    m_treeAlarmInClose.DeleteAllItems();
    for (i = 0; i < sizeof(m_struEventCardLinkageCfgV51.byAlarmInClose); i++)
    {
        strTemp.Format("AlarmIn %d", i + 1);
        hChanItem = m_treeAlarmInClose.InsertItem(strTemp, 0, 0, TVI_ROOT);
        if (hFirstItem == NULL)
        {
            hFirstItem = hChanItem;
        }

        m_treeAlarmInClose.SetItemData(hChanItem, i);

        if (m_struEventCardLinkageCfgV51.byAlarmInClose[i])
        {
            m_treeAlarmInClose.SetCheck(hChanItem, TRUE);
        }
    }
    m_treeAlarmInClose.SelectItem(hFirstItem);
    m_treeAlarmInClose.Expand(m_treeAlarmInClose.GetRootItem(), TVE_EXPAND);
}

void DlgEventCardLinkageCfg::GetTreeSel()
{
	UpdateData(TRUE); 
	memset(&m_struEventCardLinkageCfgV51, 0 , sizeof(m_struEventCardLinkageCfgV51));
	m_struEventCardLinkageCfgV51.dwSize = sizeof(m_struEventCardLinkageCfgV51);
    if (m_byProMode == TRUE)
    {
        m_struEventCardLinkageCfgV51.byProMode = 1;
    }
    if (m_byMacAddr == TRUE)
    {
        m_struEventCardLinkageCfgV51.byProMode = 2;
    }
    if (m_byEmployeeNo == TRUE)
    {
        m_struEventCardLinkageCfgV51.byProMode = 3;
    }
    if (m_byProMode == FALSE && m_byMacAddr == FALSE && m_byEmployeeNo == FALSE)
    {
        m_struEventCardLinkageCfgV51.byProMode = 0;
    }
	m_struEventCardLinkageCfgV51.dwEventSourceID = m_dwEventSourceID;
    m_struEventCardLinkageCfgV51.byMainDevBuzzer = byMainDevBuzzer;
    if (m_struEventCardLinkageCfgV51.byProMode == 1)
    {
        memcpy(m_struEventCardLinkageCfgV51.uLinkageInfo.byCardNo, m_sCardNo, m_sCardNo.GetLength());
    }
    if (m_struEventCardLinkageCfgV51.byProMode == 2)
    {
        char sMac[32] = { 0 };
        int iLen = m_sMacAddr.GetLength();
        memcpy(sMac, m_sMacAddr, iLen);
        StrToMac(sMac, m_struEventCardLinkageCfgV51.uLinkageInfo.byMACAddr);
    }
    if (m_struEventCardLinkageCfgV51.byProMode == 3)
    {
        memcpy(m_struEventCardLinkageCfgV51.uLinkageInfo.byEmployeeNo, m_sEmployeeNo, m_sEmployeeNo.GetLength());
    }
    if (m_struEventCardLinkageCfgV51.byProMode == 0)
    {
        m_struEventCardLinkageCfgV51.uLinkageInfo.struEventLinkage.wMainEventType = m_comboMainEventType.GetCurSel();
        m_struEventCardLinkageCfgV51.uLinkageInfo.struEventLinkage.wSubEventType = m_comboSubEventType.GetCurSel();
    }

    int i = 0; 
	HTREEITEM hTreeItem;

	BOOL bCheck;
	DWORD dwIndex;
	DWORD dwCount = 0; 
	CTreeCtrl *treeCtr = &m_treeAlarmOut; 
    dwCount = m_treeAlarmOut.GetCount(); 
	hTreeItem = m_treeAlarmOut.GetRootItem();

    for ( i=0; i<dwCount; i++)
    {
		bCheck = treeCtr->GetCheck(hTreeItem);
		dwIndex = treeCtr->GetItemData(hTreeItem);
		m_struEventCardLinkageCfgV51.byAlarmout[dwIndex] = bCheck;
		hTreeItem = treeCtr->GetNextSiblingItem(hTreeItem); 
    }

	treeCtr = &m_treeCardReaderBuzzer;
	hTreeItem = m_treeCardReaderBuzzer.GetRootItem();
	dwCount = m_treeCardReaderBuzzer.GetCount(); 
    for ( i=0; i<dwCount; i++)
    {
		bCheck = treeCtr->GetCheck(hTreeItem);
		dwIndex = treeCtr->GetItemData(hTreeItem);
		m_struEventCardLinkageCfgV51.byReaderBuzzer[dwIndex] = bCheck;
		hTreeItem = treeCtr->GetNextSiblingItem(hTreeItem); 
    }

    treeCtr = &m_treeCardReaderStopBuzzer;
    hTreeItem = m_treeCardReaderStopBuzzer.GetRootItem();
    dwCount = m_treeCardReaderStopBuzzer.GetCount();
    for (i = 0; i < dwCount; i++)
    {
        bCheck = treeCtr->GetCheck(hTreeItem);
        dwIndex = treeCtr->GetItemData(hTreeItem);
        m_struEventCardLinkageCfgV51.byReaderStopBuzzer[dwIndex] = bCheck;
        hTreeItem = treeCtr->GetNextSiblingItem(hTreeItem);
    }

	treeCtr = &m_treeDoorOpen;
	hTreeItem = m_treeDoorOpen.GetRootItem();
	dwCount = m_treeDoorOpen.GetCount(); 
    for ( i=0; i<dwCount; i++)
    {
		bCheck = treeCtr->GetCheck(hTreeItem);
		dwIndex = treeCtr->GetItemData(hTreeItem);
		m_struEventCardLinkageCfgV51.byOpenDoor[dwIndex] = bCheck;
		hTreeItem = treeCtr->GetNextSiblingItem(hTreeItem); 
    }

	treeCtr = &m_treeDoorClose;
	hTreeItem = m_treeDoorClose.GetRootItem();
	dwCount = m_treeDoorClose.GetCount(); 
    for ( i=0; i<dwCount; i++)
    {
		bCheck = treeCtr->GetCheck(hTreeItem);
		dwIndex = treeCtr->GetItemData(hTreeItem);
		m_struEventCardLinkageCfgV51.byCloseDoor[dwIndex] = bCheck;
		hTreeItem = treeCtr->GetNextSiblingItem(hTreeItem); 
    }

    treeCtr = &m_treeNormalOpen;
    hTreeItem = m_treeNormalOpen.GetRootItem();
    dwCount = m_treeNormalOpen.GetCount(); 
    for ( i=0; i<dwCount; i++)
    {
        bCheck = treeCtr->GetCheck(hTreeItem);
        dwIndex = treeCtr->GetItemData(hTreeItem);
        m_struEventCardLinkageCfgV51.byNormalOpen[dwIndex] = bCheck;
        hTreeItem = treeCtr->GetNextSiblingItem(hTreeItem); 
    }

    treeCtr = &m_treeNormalClose;
    hTreeItem = m_treeNormalClose.GetRootItem();
    dwCount = m_treeNormalClose.GetCount(); 
    for ( i=0; i<dwCount; i++)
    {
        bCheck = treeCtr->GetCheck(hTreeItem);
        dwIndex = treeCtr->GetItemData(hTreeItem);
        m_struEventCardLinkageCfgV51.byNormalClose[dwIndex] = bCheck;
        hTreeItem = treeCtr->GetNextSiblingItem(hTreeItem); 
    }

    treeCtr = &m_treeAlarmOutClose;
    hTreeItem = m_treeAlarmOutClose.GetRootItem();
    dwCount = m_treeAlarmOutClose.GetCount();
    for (i = 0; i < dwCount; i++)
    {
        bCheck = treeCtr->GetCheck(hTreeItem);
        dwIndex = treeCtr->GetItemData(hTreeItem);
        m_struEventCardLinkageCfgV51.byAlarmOutClose[dwIndex] = bCheck;
        hTreeItem = treeCtr->GetNextSiblingItem(hTreeItem);
    }

    treeCtr = &m_treeAlarmInSetup;
    hTreeItem = m_treeAlarmInSetup.GetRootItem();
    dwCount = m_treeAlarmInSetup.GetCount();
    for (i = 0; i < dwCount; i++)
    {
        bCheck = treeCtr->GetCheck(hTreeItem);
        dwIndex = treeCtr->GetItemData(hTreeItem);
        m_struEventCardLinkageCfgV51.byAlarmInSetup[dwIndex] = bCheck;
        hTreeItem = treeCtr->GetNextSiblingItem(hTreeItem);
    }

    treeCtr = &m_treeAlarmInClose;
    hTreeItem = m_treeAlarmInClose.GetRootItem();
    dwCount = m_treeAlarmInClose.GetCount();
    for (i = 0; i < dwCount; i++)
    {
        bCheck = treeCtr->GetCheck(hTreeItem);
        dwIndex = treeCtr->GetItemData(hTreeItem);
        m_struEventCardLinkageCfgV51.byAlarmInClose[dwIndex] = bCheck;
        hTreeItem = treeCtr->GetNextSiblingItem(hTreeItem);
    }
} 

void DlgEventCardLinkageCfg::OnClickTreeCardReaderBuzzer(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE);

    CPoint pt(0,0);
    CRect rc(0,0,0,0);
    GetCursorPos(&pt);
    m_treeCardReaderBuzzer.ScreenToClient(&pt);
    GetDlgItem(IDC_TREE_READERBUZZER)->GetWindowRect(&rc);
    m_treeCardReaderBuzzer.ScreenToClient(&rc);
    pt.x = pt.x - rc.left;
    pt.y = pt.y - rc.top;
    
    UINT uFlag = 0;
    HTREEITEM hSelect = m_treeCardReaderBuzzer.HitTest(pt, &uFlag);
    
    if (NULL == hSelect) 
    {
        return;
    }
    m_treeCardReaderBuzzer.SelectItem(hSelect);
    DWORD dwIndex = m_treeCardReaderBuzzer.GetItemData(hSelect);
    BOOL bCheck = m_treeCardReaderBuzzer.GetCheck(hSelect);
    m_treeCardReaderBuzzer.SetCheck(hSelect, !bCheck);
    m_struEventCardLinkageCfgV51.byReaderBuzzer[dwIndex] = !bCheck;
    //switch checkbox status on click
    if (uFlag & LVHT_ONITEM ||uFlag & LVHT_TOLEFT || uFlag & LVHT_ONITEMLABEL)//LVHT_TOLEFT)
    {
        m_treeCardReaderBuzzer.SetCheck(hSelect, !bCheck);
        m_struEventCardLinkageCfgV51.byReaderBuzzer[dwIndex] = !bCheck;
    }
    else
    {
        m_treeCardReaderBuzzer.SetCheck(hSelect, bCheck);
        m_struEventCardLinkageCfgV51.byReaderBuzzer[dwIndex] = bCheck;
	}
    UpdateData(FALSE); 
	*pResult = 0;
}

void DlgEventCardLinkageCfg::OnClickTreeAlarmOut(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE);
    CPoint pt(0,0);
    CRect rc(0,0,0,0);
    GetCursorPos(&pt);
    m_treeAlarmOut.ScreenToClient(&pt);
    GetDlgItem(IDC_TREE_ALARMOUT)->GetWindowRect(&rc);
    m_treeAlarmOut.ScreenToClient(&rc);
    pt.x = pt.x - rc.left;
    pt.y = pt.y - rc.top;
    
    UINT uFlag = 0;
    HTREEITEM hSelect = m_treeAlarmOut.HitTest(pt, &uFlag);
    
    if (NULL == hSelect) 
    {
        return;
    }
    m_treeAlarmOut.SelectItem(hSelect);
    DWORD dwIndex = m_treeAlarmOut.GetItemData(hSelect);
    BOOL bCheck = m_treeAlarmOut.GetCheck(hSelect);
    m_treeAlarmOut.SetCheck(hSelect, !bCheck);
    m_struEventCardLinkageCfgV51.byAlarmout[dwIndex] = !bCheck;
    //switch checkbox status on click
    if (uFlag & LVHT_ONITEM ||uFlag & LVHT_TOLEFT || uFlag & LVHT_ONITEMLABEL)
    {
        m_treeAlarmOut.SetCheck(hSelect, !bCheck);
        m_struEventCardLinkageCfgV51.byAlarmout[dwIndex] = !bCheck;
    }
    else
    {
        m_treeAlarmOut.SetCheck(hSelect, bCheck);
        m_struEventCardLinkageCfgV51.byAlarmout[dwIndex] = bCheck;
	}
    UpdateData(FALSE);
	*pResult = 0;
}

void DlgEventCardLinkageCfg::OnClickTreeDoorOpen(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
    CPoint pt(0,0);
    CRect rc(0,0,0,0);
    GetCursorPos(&pt);
    m_treeDoorOpen.ScreenToClient(&pt);   
    m_treeDoorOpen.GetWindowRect(&rc);
    m_treeDoorOpen.ScreenToClient(&rc);
    pt.x = pt.x - rc.left;
    pt.y = pt.y - rc.top;
    
    UINT uFlag = 0;
    HTREEITEM hSelect = m_treeDoorOpen.HitTest(pt, &uFlag);
    
    if (NULL == hSelect) 
    {
        return;
    }
    m_treeDoorOpen.SelectItem(hSelect);
    DWORD dwIndex = m_treeDoorOpen.GetItemData(hSelect);
    BOOL bCheck = m_treeDoorOpen.GetCheck(hSelect);
    m_treeDoorOpen.SetCheck(hSelect, !bCheck);
    m_struEventCardLinkageCfgV51.byOpenDoor[dwIndex] = !bCheck;
    //switch checkbox status on click
    if (uFlag & LVHT_ONITEM ||uFlag & LVHT_TOLEFT || uFlag & LVHT_ONITEMLABEL)
    {
		m_treeDoorOpen.SetCheck(hSelect, !bCheck);
        m_struEventCardLinkageCfgV51.byOpenDoor[dwIndex] = !bCheck;
    }
    else
    {
        m_treeDoorOpen.SetCheck(hSelect, bCheck);
        m_struEventCardLinkageCfgV51.byOpenDoor[dwIndex] = bCheck;
	}
    UpdateData(FALSE);
	*pResult = 0;
}

void DlgEventCardLinkageCfg::OnClickTreeDoorClose(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE);
    CPoint pt(0,0);
    CRect rc(0,0,0,0);
    GetCursorPos(&pt);
    m_treeDoorClose.ScreenToClient(&pt);
    m_treeDoorClose.GetWindowRect(&rc);
    m_treeDoorClose.ScreenToClient(&rc);
    pt.x = pt.x - rc.left;
    pt.y = pt.y - rc.top;

    UINT uFlag = 0;
    HTREEITEM hSelect = m_treeDoorClose.HitTest(pt, &uFlag);

    if (NULL == hSelect) 
    {
        return;
    }
    m_treeDoorClose.SelectItem(hSelect);
    DWORD dwIndex = m_treeDoorClose.GetItemData(hSelect);
    BOOL bCheck = m_treeDoorClose.GetCheck(hSelect);
    m_treeDoorClose.SetCheck(hSelect, !bCheck);
    m_struEventCardLinkageCfgV51.byCloseDoor[dwIndex] = !bCheck;

    //switch checkbox status on click
    if (uFlag & LVHT_ONITEM ||uFlag & LVHT_TOLEFT || uFlag & LVHT_ONITEMLABEL )
    {
        m_treeDoorClose.SetCheck(hSelect, !bCheck);
        m_struEventCardLinkageCfgV51.byCloseDoor[dwIndex] = !bCheck;
    }
    else
    {
        m_treeDoorClose.SetCheck(hSelect, bCheck);
        m_struEventCardLinkageCfgV51.byCloseDoor[dwIndex] = bCheck;
    }	
    UpdateData(FALSE);
	*pResult = 0;
}

void DlgEventCardLinkageCfg::OnClickTreeNormalOpen(NMHDR* pNMHDR, LRESULT* pResult) 
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);
    CPoint pt(0,0);
    CRect rc(0,0,0,0);
    GetCursorPos(&pt);
    m_treeNormalOpen.ScreenToClient(&pt);   
    m_treeNormalOpen.GetWindowRect(&rc);
    m_treeNormalOpen.ScreenToClient(&rc);
    pt.x = pt.x - rc.left;
    pt.y = pt.y - rc.top;
    
    UINT uFlag = 0;
    HTREEITEM hSelect = m_treeNormalOpen.HitTest(pt, &uFlag);
    
    if (NULL == hSelect) 
    {
        return;
    }
    m_treeNormalOpen.SelectItem(hSelect);
    DWORD dwIndex = m_treeNormalOpen.GetItemData(hSelect);
    BOOL bCheck = m_treeNormalOpen.GetCheck(hSelect);
    m_treeNormalOpen.SetCheck(hSelect, !bCheck);
    m_struEventCardLinkageCfgV51.byNormalOpen[dwIndex] = !bCheck;
    //switch checkbox status on click
    if (uFlag & LVHT_ONITEM ||uFlag & LVHT_TOLEFT || uFlag & LVHT_ONITEMLABEL)
    {
        m_treeNormalOpen.SetCheck(hSelect, !bCheck);
        m_struEventCardLinkageCfgV51.byNormalOpen[dwIndex] = !bCheck;
    }
    else
    {
        m_treeNormalOpen.SetCheck(hSelect, bCheck);
        m_struEventCardLinkageCfgV51.byNormalOpen[dwIndex] = bCheck;
    }
    UpdateData(FALSE);
    *pResult = 0;
}

void DlgEventCardLinkageCfg::OnClickTreeNormalClose(NMHDR* pNMHDR, LRESULT* pResult) 
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);
    CPoint pt(0,0);
    CRect rc(0,0,0,0);
    GetCursorPos(&pt);
    m_treeNormalClose.ScreenToClient(&pt);   
    m_treeNormalClose.GetWindowRect(&rc);
    m_treeNormalClose.ScreenToClient(&rc);
    pt.x = pt.x - rc.left;
    pt.y = pt.y - rc.top;
    
    UINT uFlag = 0;
    HTREEITEM hSelect = m_treeNormalClose.HitTest(pt, &uFlag);
    
    if (NULL == hSelect) 
    {
        return;
    }
    m_treeNormalClose.SelectItem(hSelect);
    DWORD dwIndex = m_treeNormalClose.GetItemData(hSelect);
    BOOL bCheck = m_treeNormalClose.GetCheck(hSelect);
    m_treeNormalClose.SetCheck(hSelect, !bCheck);
    m_struEventCardLinkageCfgV51.byNormalClose[dwIndex] = !bCheck;
    //switch checkbox status on click
    if (uFlag & LVHT_ONITEM || uFlag & LVHT_TOLEFT || uFlag & LVHT_ONITEMLABEL)
    {
        m_treeNormalClose.SetCheck(hSelect, !bCheck);
        m_struEventCardLinkageCfgV51.byNormalClose[dwIndex] = !bCheck;
    }
    else
    {
        m_treeNormalClose.SetCheck(hSelect, bCheck);
        m_struEventCardLinkageCfgV51.byNormalClose[dwIndex] = bCheck;
    }
    UpdateData(FALSE);
    *pResult = 0;
}

void DlgEventCardLinkageCfg::OnNMClickTreeAlarmOutClose(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);
    CPoint pt(0, 0);
    CRect rc(0, 0, 0, 0);
    GetCursorPos(&pt);
    m_treeAlarmOutClose.ScreenToClient(&pt);
    m_treeAlarmOutClose.GetWindowRect(&rc);
    m_treeAlarmOutClose.ScreenToClient(&rc);
    pt.x = pt.x - rc.left;
    pt.y = pt.y - rc.top;

    UINT uFlag = 0;
    HTREEITEM hSelect = m_treeAlarmOutClose.HitTest(pt, &uFlag);

    if (NULL == hSelect)
    {
        return;
    }
    m_treeAlarmOutClose.SelectItem(hSelect);
    DWORD dwIndex = m_treeAlarmOutClose.GetItemData(hSelect);
    BOOL bCheck = m_treeAlarmOutClose.GetCheck(hSelect);
    m_treeAlarmOutClose.SetCheck(hSelect, !bCheck);
    m_struEventCardLinkageCfgV51.byAlarmOutClose[dwIndex] = !bCheck;
    //switch checkbox status on click
    if (uFlag & LVHT_ONITEM || uFlag & LVHT_TOLEFT || uFlag & LVHT_ONITEMLABEL)
    {
        m_treeAlarmOutClose.SetCheck(hSelect, !bCheck);
        m_struEventCardLinkageCfgV51.byAlarmOutClose[dwIndex] = !bCheck;
    }
    else
    {
        m_treeAlarmOutClose.SetCheck(hSelect, bCheck);
        m_struEventCardLinkageCfgV51.byAlarmOutClose[dwIndex] = bCheck;
    }
    UpdateData(FALSE);
    *pResult = 0;
}


void DlgEventCardLinkageCfg::OnNMClickTreeAlarmInSetup(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);
    CPoint pt(0, 0);
    CRect rc(0, 0, 0, 0);
    GetCursorPos(&pt);
    m_treeAlarmInSetup.ScreenToClient(&pt);
    m_treeAlarmInSetup.GetWindowRect(&rc);
    m_treeAlarmInSetup.ScreenToClient(&rc);
    pt.x = pt.x - rc.left;
    pt.y = pt.y - rc.top;

    UINT uFlag = 0;
    HTREEITEM hSelect = m_treeAlarmInSetup.HitTest(pt, &uFlag);

    if (NULL == hSelect)
    {
        return;
    }
    m_treeAlarmInSetup.SelectItem(hSelect);
    DWORD dwIndex = m_treeAlarmInSetup.GetItemData(hSelect);
    BOOL bCheck = m_treeAlarmInSetup.GetCheck(hSelect);
    m_treeAlarmInSetup.SetCheck(hSelect, !bCheck);
    m_struEventCardLinkageCfgV51.byAlarmInSetup[dwIndex] = !bCheck;
    //switch checkbox status on click
    if (uFlag & LVHT_ONITEM || uFlag & LVHT_TOLEFT || uFlag & LVHT_ONITEMLABEL)
    {
        m_treeAlarmInSetup.SetCheck(hSelect, !bCheck);
        m_struEventCardLinkageCfgV51.byAlarmInSetup[dwIndex] = !bCheck;
    }
    else
    {
        m_treeAlarmInSetup.SetCheck(hSelect, bCheck);
        m_struEventCardLinkageCfgV51.byAlarmInSetup[dwIndex] = bCheck;
    }
    UpdateData(FALSE);
    *pResult = 0;
}


void DlgEventCardLinkageCfg::OnNMClickTreeAlarmInClose(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);
    CPoint pt(0, 0);
    CRect rc(0, 0, 0, 0);
    GetCursorPos(&pt);
    m_treeAlarmInClose.ScreenToClient(&pt);
    m_treeAlarmInClose.GetWindowRect(&rc);
    m_treeAlarmInClose.ScreenToClient(&rc);
    pt.x = pt.x - rc.left;
    pt.y = pt.y - rc.top;

    UINT uFlag = 0;
    HTREEITEM hSelect = m_treeAlarmInClose.HitTest(pt, &uFlag);

    if (NULL == hSelect)
    {
        return;
    }
    m_treeAlarmInClose.SelectItem(hSelect);
    DWORD dwIndex = m_treeAlarmInClose.GetItemData(hSelect);
    BOOL bCheck = m_treeAlarmInClose.GetCheck(hSelect);
    m_treeAlarmInClose.SetCheck(hSelect, !bCheck);
    m_struEventCardLinkageCfgV51.byAlarmInClose[dwIndex] = !bCheck;
    //switch checkbox status on click
    if (uFlag & LVHT_ONITEM || uFlag & LVHT_TOLEFT || uFlag & LVHT_ONITEMLABEL)
    {
        m_treeAlarmInClose.SetCheck(hSelect, !bCheck);
        m_struEventCardLinkageCfgV51.byAlarmInClose[dwIndex] = !bCheck;
    }
    else
    {
        m_treeAlarmInClose.SetCheck(hSelect, bCheck);
        m_struEventCardLinkageCfgV51.byAlarmInClose[dwIndex] = bCheck;
    }
    UpdateData(FALSE);
    *pResult = 0;
}


void DlgEventCardLinkageCfg::InitMainEventType()
{
    // WORD  wMainEventType;  //�¼������ͣ�0-�豸�¼���1-���������¼���2-���¼���3-��������
	char szLan[1024] = {0};
    m_comboMainEventType.ResetContent();
	g_StringLanType(szLan, "�豸�¼�", "Device Event");
    m_comboMainEventType.InsertString(0,szLan);
	g_StringLanType(szLan, "���������¼�", "AlarmIn Event");
    m_comboMainEventType.InsertString(1,szLan);
	g_StringLanType(szLan, "���¼�", "Door Event");
    m_comboMainEventType.InsertString(2,szLan);
	g_StringLanType(szLan, "�������¼�", "CardReader Event");
    m_comboMainEventType.InsertString(3,szLan);
    m_comboMainEventType.SetCurSel(0);
}

void DlgEventCardLinkageCfg::SubEventTypeDev()
{
    // WORD  wSubEventType;  
	char szLan[1024] = {0};

    m_comboSubEventType.ResetContent();
	g_StringLanType(szLan, "�������𱨾�", "Host Tamper Alarm");
    m_comboSubEventType.InsertString(EVENT_ACS_HOST_ANTI_DISMANTLE,szLan);
	g_StringLanType(szLan, "�����¼���90%����", "Device Event");
    m_comboSubEventType.InsertString(EVENT_ACS_OFFLINE_ECENT_NEARLY_FULL,szLan);
	g_StringLanType(szLan, "����Ͽ�", "Network Disconnect");
    m_comboSubEventType.InsertString(EVENT_ACS_NET_BROKEN,szLan);
	g_StringLanType(szLan, "����ָ�", "Network Recovery");
    m_comboSubEventType.InsertString(EVENT_ACS_NET_RESUME,szLan);
	g_StringLanType(szLan, "���ص�ѹ��", "Low Battery Voltage");
    m_comboSubEventType.InsertString(EVENT_ACS_LOW_BATTERY,szLan);
	g_StringLanType(szLan, "���ص�ѹ�ָ�����", "Battery Voltage normal");
    m_comboSubEventType.InsertString(EVENT_ACS_BATTERY_RESUME,szLan);
	g_StringLanType(szLan, "������ϵ�", "AC Power Off");
    m_comboSubEventType.InsertString(EVENT_ACS_AC_OFF,szLan);
	g_StringLanType(szLan, "������ָ�", "AC Power On");
    m_comboSubEventType.InsertString(EVENT_ACS_AC_RESUME,szLan);
	g_StringLanType(szLan, "SD���洢������", "SD Card Full Alarm");
    m_comboSubEventType.InsertString(EVENT_ACS_SD_CARD_FULL,szLan);
	g_StringLanType(szLan, "����ץ���¼�����", "Linkage Capture Event Alarm");
    m_comboSubEventType.InsertString(EVENT_ACS_LINKAGE_CAPTURE_PIC,szLan);
    g_StringLanType(szLan, "����ͼ���ʵ�", "Image Quality Low");
    m_comboSubEventType.InsertString(EVENT_ACS_IMAGE_QUALITY_LOW, szLan);
    g_StringLanType(szLan, "ָ��ͼ���ʵ�", "Finger Print Quality Low");
    m_comboSubEventType.InsertString(EVENT_ACS_FINGER_PRINT_QUALITY_LOW, szLan);
    g_StringLanType(szLan, "��ص�ѹ��", "Battery Electric Low");
    m_comboSubEventType.InsertString(EVENT_ACS_BATTERY_ELECTRIC_LOW, szLan);
    g_StringLanType(szLan, "��ص�ѹ�ָ�����", "Battery Electric Resume");
    m_comboSubEventType.InsertString(EVENT_ACS_BATTERY_ELECTRIC_RESUME, szLan);
    g_StringLanType(szLan, "���������·����", "Fire import short circuit");
    m_comboSubEventType.InsertString(EVENT_ACS_FIRE_IMPORT_SHORT_CIRCUIT, szLan);
    g_StringLanType(szLan, "���������·����", "Fire import broken circuit");
    m_comboSubEventType.InsertString(EVENT_ACS_FIRE_IMPORT_BROKEN_CIRCUIT, szLan);
    g_StringLanType(szLan, "��������ָ�", "Fire import resume");
    m_comboSubEventType.InsertString(EVENT_ACS_FIRE_IMPORT_RESUME, szLan);
    g_StringLanType(szLan, "����RS485��·�ڵ�Ͽ�", "Master RS485 loop node broken");
    m_comboSubEventType.InsertString(EVENT_ACS_MASTER_RS485_LOOPNODE_BROKEN, szLan);
    g_StringLanType(szLan, "����RS485��·�ڵ�ָ�", "Master RS485 loop node resume");
    m_comboSubEventType.InsertString(EVENT_ACS_MASTER_RS485_LOOPNODE_RESUME, szLan);
    g_StringLanType(szLan, "�͵ؿ���������", "Local control offline");
    m_comboSubEventType.InsertString(EVENT_ACS_LOCAL_CONTROL_OFFLINE, szLan);
    g_StringLanType(szLan, "�͵ؿ��������߻ָ�", "Local control resume");
    m_comboSubEventType.InsertString(EVENT_ACS_LOCAL_CONTROL_RESUME, szLan);
    g_StringLanType(szLan, "�͵�����RS485��·�Ͽ�", "Local downside RS485 loopnode broken");
    m_comboSubEventType.InsertString(EVENT_ACS_LOCAL_DOWNSIDE_RS485_LOOPNODE_BROKEN, szLan);
    g_StringLanType(szLan, "�͵�����RS485��·�ָ�", "Local downside RS485 loopnode resume");
    m_comboSubEventType.InsertString(EVENT_ACS_LOCAL_DOWNSIDE_RS485_LOOPNODE_RESUME, szLan);
    g_StringLanType(szLan, "�ֿ�������", "Distract controller online");
    m_comboSubEventType.InsertString(EVENT_ACS_DISTRACT_CONTROLLER_ONLINE, szLan);
    g_StringLanType(szLan, "�ֿ�������", "Distract controller offline");
    m_comboSubEventType.InsertString(EVENT_ACS_DISTRACT_CONTROLLER_OFFLINE, szLan);
    g_StringLanType(szLan, "������ť����", "Fire button trigger");
    m_comboSubEventType.InsertString(EVENT_ACS_FIRE_BUTTON_TRIGGER, szLan);
    g_StringLanType(szLan, "������ť�ָ�", "Fire button resume");
    m_comboSubEventType.InsertString(EVENT_ACS_FIRE_BUTTON_RESUME, szLan);
    g_StringLanType(szLan, "ά����ť����", "Maintenance button trigger");
    m_comboSubEventType.InsertString(EVENT_ACS_MAINTENANCE_BUTTON_TRIGGER, szLan);
    g_StringLanType(szLan, "ά����ť�ָ�", "Maintenance button resume");
    m_comboSubEventType.InsertString(EVENT_ACS_MAINTENANCE_BUTTON_RESUME, szLan);
    g_StringLanType(szLan, "������ť����", "Emergency button trigger");
    m_comboSubEventType.InsertString(EVENT_ACS_EMERGENCY_BUTTON_TRIGGER, szLan);
    g_StringLanType(szLan, "������ť�ָ�", "Emergency button resume");
    m_comboSubEventType.InsertString(EVENT_ACS_EMERGENCY_BUTTON_RESUME, szLan);
    g_StringLanType(szLan, "MAC���", "MAC detect");
    m_comboSubEventType.InsertString(EVENT_ACS_MAC_DETECT, szLan);
    g_StringLanType(szLan, "�뷴Ǳ�ط�����ͨ�ŶϿ�", "Submarinebreak comm break");
    m_comboSubEventType.InsertString(EVENT_ACS_SUBMARINEBACK_COMM_BREAK, szLan);
    g_StringLanType(szLan, "�뷴Ǳ�ط�����ͨ�Żָ�", "Submarinebreak comm resume");
    m_comboSubEventType.InsertString(EVENT_ACS_SUBMARINEBACK_COMM_RESUME, szLan);
    g_StringLanType(szLan, "Զ��ʵʱ����", "Remote actual guard");
    m_comboSubEventType.InsertString(EVENT_ACS_REMOTE_ACTUAL_GUARD, szLan);
    g_StringLanType(szLan, "Զ��ʵʱ����", "Remote actual unguard");
    m_comboSubEventType.InsertString(EVENT_ACS_REMOTE_ACTUAL_UNGUARD, szLan);
    g_StringLanType(szLan, "����򴫸����쳣", "Motor sensor exception");
    m_comboSubEventType.InsertString(EVENT_ACS_MOTOR_SENSOR_EXCEPTION, szLan);
    g_StringLanType(szLan, "CAN�����쳣", "Can bus exception");
    m_comboSubEventType.InsertString(EVENT_ACS_CAN_BUS_EXCEPTION, szLan);
    g_StringLanType(szLan, "CAN���߻ָ�", "Can bus resume");
    m_comboSubEventType.InsertString(EVENT_ACS_CAN_BUS_RESUME, szLan);
    g_StringLanType(szLan, "բ��ǻ���¶ȳ���", "Gate temperature overrun");
    m_comboSubEventType.InsertString(EVENT_ACS_GATE_TEMPERATURE_OVERRUN, szLan);
    g_StringLanType(szLan, "��������쳣", "IR emitter exception");
    m_comboSubEventType.InsertString(EVENT_ACS_IR_EMITTER_EXCEPTION, szLan);
    g_StringLanType(szLan, "�������ָ�", "IR emitter resume");
    m_comboSubEventType.InsertString(EVENT_ACS_IR_EMITTER_RESUME, szLan);
    g_StringLanType(szLan, "�ư�ͨ���쳣", "Lamp board communicate exception");
    m_comboSubEventType.InsertString(EVENT_ACS_LAMP_BOARD_COMM_EXCEPTION, szLan);
    g_StringLanType(szLan, "�ư�ͨ�Żָ�", "Lamp board communicate resume");
    m_comboSubEventType.InsertString(EVENT_ACS_LAMP_BOARD_COMM_RESUME, szLan);
    g_StringLanType(szLan, "����ת�Ӱ�ͨ���쳣", "IR adaptor board communicate exception");
    m_comboSubEventType.InsertString(EVENT_ACS_IR_ADAPTOR_BOARD_COMM_EXCEPTION, szLan);
    g_StringLanType(szLan, "����ת�Ӱ�ͨ�Żָ�", "MAC detect");
    m_comboSubEventType.InsertString(EVENT_ACS_IR_ADAPTOR_BOARD_COMM_RESUME, szLan);
    g_StringLanType(szLan, "ͨ�����������𱨾�", "Channel controller desmantle alarm");
    m_comboSubEventType.InsertString(EVENT_ACS_CHANNEL_CONTROLLER_DESMANTLE_ALARM, szLan);
    g_StringLanType(szLan, "ͨ�����������𱨾��ָ�", "Channel controller desmantle resume");
    m_comboSubEventType.InsertString(EVENT_ACS_CHANNEL_CONTROLLER_DESMANTLE_RESUME, szLan);
    g_StringLanType(szLan, "ͨ���������������뱨��", "Channel controller fire import alarm");
    m_comboSubEventType.InsertString(EVENT_ACS_CHANNEL_CONTROLLER_FIRE_IMPORT_ALARM, szLan);
    g_StringLanType(szLan, "ͨ���������������뱨���ָ�", "Channel controller fire import resume");
    m_comboSubEventType.InsertString(EVENT_ACS_CHANNEL_CONTROLLER_FIRE_IMPORT_RESUME, szLan);
    g_StringLanType(szLan, "�����¼�", "Stay event");
    m_comboSubEventType.InsertString(EVENT_ACS_STAY_EVENT, szLan);
    g_StringLanType(szLan, "���ߺϷ��¼���90%����", "Offline legal event nearly full");
    m_comboSubEventType.InsertString(EVENT_ACS_LEGAL_EVENT_NEARLY_FULL, szLan);

    m_comboSubEventType.SetCurSel(EVENT_ACS_HOST_ANTI_DISMANTLE);
}

void DlgEventCardLinkageCfg::SubEventTypeAlarm()
{
    // WORD  wSubEventType; 
	char szLan[1024] = {0};

    m_comboSubEventType.ResetContent();
	g_StringLanType(szLan, "������·����", "Zone Short-Circuit Alarm");
    m_comboSubEventType.InsertString(EVENT_ACS_ALARMIN_SHORT_CIRCUIT,szLan);
	g_StringLanType(szLan, "������·����", "Zone Circuit Alarm");
    m_comboSubEventType.InsertString(EVENT_ACS_ALARMIN_BROKEN_CIRCUIT,szLan);
	g_StringLanType(szLan, "�����쳣����", "Abnormal Zone Alarm");
    m_comboSubEventType.InsertString(EVENT_ACS_ALARMIN_EXCEPTION,szLan);
	g_StringLanType(szLan, "���������ָ�", "Zone Alarm Recover");
    m_comboSubEventType.InsertString(EVENT_ACS_ALARMIN_RESUME,szLan);
	g_StringLanType(szLan, "�¼����뱨��", "Event Input Alarm");
    m_comboSubEventType.InsertString(EVENT_ACS_CASE_SENSOR_ALARM,szLan);
	g_StringLanType(szLan, "�¼�����ָ�", "Event Recover Alarm");
    m_comboSubEventType.InsertString(EVENT_ACS_CASE_SENSOR_RESUME,szLan);
    m_comboSubEventType.SetCurSel(EVENT_ACS_ALARMIN_SHORT_CIRCUIT);
}

void DlgEventCardLinkageCfg::SubEventTypeDoor()
{
    // WORD  wSubEventType;  
    m_comboSubEventType.ResetContent();
    char szLan[1024] = { 0 };

    g_StringLanType(szLan, "�׿����ſ�ʼ", "Leader Card Open Begin");
    m_comboSubEventType.InsertString(EVENT_ACS_LEADER_CARD_OPEN_BEGIN, szLan);
    g_StringLanType(szLan, "�׿����Ž���", "Leader Card Open End");
    m_comboSubEventType.InsertString(EVENT_ACS_LEADER_CARD_OPEN_END, szLan);
    g_StringLanType(szLan, "����״̬��ʼ", "Always Open Begin");
    m_comboSubEventType.InsertString(EVENT_ACS_ALWAYS_OPEN_BEGIN, szLan);
    g_StringLanType(szLan, "����״̬����", "Always Open End");
    m_comboSubEventType.InsertString(EVENT_ACS_ALWAYS_OPEN_END, szLan);
    g_StringLanType(szLan, "����״̬��ʼ", "Always Close Begin");
    m_comboSubEventType.InsertString(EVENT_ACS_ALWAYS_CLOSE_BEGIN, szLan);
    g_StringLanType(szLan, "����״̬����", "Always Close End");
    m_comboSubEventType.InsertString(EVENT_ACS_ALWAYS_CLOSE_END, szLan);
    g_StringLanType(szLan, "������", "Lock Open");
    m_comboSubEventType.InsertString(EVENT_ACS_LOCK_OPEN, szLan);
    g_StringLanType(szLan, "�����ر�", "Lock Close");
    m_comboSubEventType.InsertString(EVENT_ACS_LOCK_CLOSE, szLan);
    g_StringLanType(szLan, "���Ű�ť����", "Door Button Press");
    m_comboSubEventType.InsertString(EVENT_ACS_DOOR_BUTTON_PRESS, szLan);
    g_StringLanType(szLan, "���Ű�ť�ſ�", "Door Button Release");
    m_comboSubEventType.InsertString(EVENT_ACS_DOOR_BUTTON_RELEASE, szLan);
    g_StringLanType(szLan, "�������ţ��Ŵţ�", "Door Open Normal");
    m_comboSubEventType.InsertString(EVENT_ACS_DOOR_OPEN_NORMAL, szLan);
    g_StringLanType(szLan, "�������ţ��Ŵţ�", "Door Close Normal");
    m_comboSubEventType.InsertString(EVENT_ACS_DOOR_CLOSE_NORMAL, szLan);
    g_StringLanType(szLan, "���쳣�򿪣��Ŵţ�", "Door Open Abnormal");
    m_comboSubEventType.InsertString(EVENT_ACS_DOOR_OPEN_ABNORMAL, szLan);
    g_StringLanType(szLan, "�Ŵ򿪳�ʱ���Ŵţ�", "Door Open Timeout");
    m_comboSubEventType.InsertString(EVENT_ACS_DOOR_OPEN_TIMEOUT, szLan);
    g_StringLanType(szLan, "Զ�̿���", "Remote Open Door");
    m_comboSubEventType.InsertString(EVENT_ACS_REMOTE_OPEN_DOOR, szLan);
    g_StringLanType(szLan, "Զ�̹���", "Remote Close Door");
    m_comboSubEventType.InsertString(EVENT_ACS_REMOTE_CLOSE_DOOR, szLan);
    g_StringLanType(szLan, "Զ�̳���", "Remote Always Open");
    m_comboSubEventType.InsertString(EVENT_ACS_REMOTE_ALWAYS_OPEN, szLan);
    g_StringLanType(szLan, "Զ�̳���", "Remote Always Close");
    m_comboSubEventType.InsertString(EVENT_ACS_REMOTE_ALWAYS_CLOSE, szLan);
    g_StringLanType(szLan, "�������ڶ�����֤Ⱥ��", "Not Belong Multi Group");
    m_comboSubEventType.InsertString(EVENT_ACS_NOT_BELONG_MULTI_GROUP, szLan);
    g_StringLanType(szLan, "�����ڶ�����֤ʱ�����", "Invalid Multi Verify Period");
    m_comboSubEventType.InsertString(EVENT_ACS_INVALID_MULTI_VERIFY_PERIOD, szLan);
    g_StringLanType(szLan, "������֤ģʽ����������֤ʧ��", "Multi Verify Super Right Fail");
    m_comboSubEventType.InsertString(EVENT_ACS_MULTI_VERIFY_SUPER_RIGHT_FAIL, szLan);
    g_StringLanType(szLan, "������֤ģʽԶ����֤ʧ��", "Multi Verify Remote Right Fail");
    m_comboSubEventType.InsertString(EVENT_ACS_MULTI_VERIFY_REMOTE_RIGHT_FAIL, szLan);
    g_StringLanType(szLan, "������֤�ɹ�", "Multi Verify Success");
    m_comboSubEventType.InsertString(EVENT_ACS_MULTI_VERIFY_SUCCESS, szLan);
    g_StringLanType(szLan, "���ض�����֤��ҪԶ�̿���", "Multi Verify Need Remote Open");
    m_comboSubEventType.InsertString(EVENT_ACS_MULTI_VERIFY_NEED_REMOTE_OPEN, szLan);
    g_StringLanType(szLan, "������֤����������֤�ɹ��¼�", "Multi Verify Super Password Verify Success");
    m_comboSubEventType.InsertString(EVENT_ACS_MULTI_VERIFY_SUPERPASSWD_VERIFY_SUCCESS, szLan);
    g_StringLanType(szLan, "������֤�ظ���֤ʧ��", "Multi Verify Repeat Verify Fail");
    m_comboSubEventType.InsertString(EVENT_ACS_MULTI_VERIFY_REPEAT_VERIFY_FAIL, szLan);
    g_StringLanType(szLan, "������֤��ʱʧ��", "Multi Verify Timeout");
    m_comboSubEventType.InsertString(EVENT_ACS_MULTI_VERIFY_TIMEOUT, szLan);
    g_StringLanType(szLan, "Զ��ץ��", "Remote Capture Picture");
    m_comboSubEventType.InsertString(EVENT_ACS_REMOTE_CAPTURE_PIC, szLan);
    g_StringLanType(szLan, "������", "Door Bell Ringing");
    m_comboSubEventType.InsertString(EVENT_ACS_DOORBELL_RINGING, szLan);
    g_StringLanType(szLan, "�ſذ�ȫģ����𱨾�", "Security Module Desmantle Alarm");
    m_comboSubEventType.InsertString(EVENT_ACS_SECURITY_MODULE_DESMANTLE_ALARM, szLan);
    g_StringLanType(szLan, "���������¼�", "Call Center");
    m_comboSubEventType.InsertString(EVENT_ACS_CALL_CENTER, szLan);
    g_StringLanType(szLan, "�׿���Ȩ��ʼ", "First Card Authorize Begin");
    m_comboSubEventType.InsertString(EVENT_ACS_FIRSTCARD_AUTHORIZE_BEGIN, szLan);
    g_StringLanType(szLan, "�׿���Ȩ����", "First Card Authorize End");
    m_comboSubEventType.InsertString(EVENT_ACS_FIRSTCARD_AUTHORIZE_END, szLan);
    g_StringLanType(szLan, "���������·����", "Door Lock Input Short Circuit");
    m_comboSubEventType.InsertString(EVENT_ACS_DOORLOCK_INPUT_SHORT_CIRCUIT, szLan);
    g_StringLanType(szLan, "���������·����", "Door Lock Input Broken Circuit");
    m_comboSubEventType.InsertString(EVENT_ACS_DOORLOCK_INPUT_BROKEN_CIRCUIT, szLan);
    g_StringLanType(szLan, "���������쳣����", "Door Lock Input Exception");
    m_comboSubEventType.InsertString(EVENT_ACS_DOORLOCK_INPUT_EXCEPTION, szLan);
    g_StringLanType(szLan, "�Ŵ������·����", "Door Contact Input Short Circuit");
    m_comboSubEventType.InsertString(EVENT_ACS_DOORCONTACT_INPUT_SHORT_CIRCUIT, szLan);
    g_StringLanType(szLan, "�Ŵ������·����", "Door Contact Input Broken Circuit");
    m_comboSubEventType.InsertString(EVENT_ACS_DOORCONTACT_INPUT_BROKEN_CIRCUIT, szLan);
    g_StringLanType(szLan, "�Ŵ������쳣����", "Door Contact Input Exception");
    m_comboSubEventType.InsertString(EVENT_ACS_DOORCONTACT_INPUT_EXCEPTION, szLan);
    g_StringLanType(szLan, "���Ű�ť�����·����", "Open Button Input Short Circuit");
    m_comboSubEventType.InsertString(EVENT_ACS_OPENBUTTON_INPUT_SHORT_CIRCUIT, szLan);
    g_StringLanType(szLan, "���Ű�ť�����·����", "Open Button Input Broken Circuit");
    m_comboSubEventType.InsertString(EVENT_ACS_OPENBUTTON_INPUT_BROKEN_CIRCUIT, szLan);
    g_StringLanType(szLan, "���Ű�ť�����쳣����", "Open Button Input Exception");
    m_comboSubEventType.InsertString(EVENT_ACS_OPENBUTTON_INPUT_EXCEPTION, szLan);
    g_StringLanType(szLan, "�����쳣��", "Door Lock Open Exception");
    m_comboSubEventType.InsertString(EVENT_ACS_DOORLOCK_OPEN_EXCEPTION, szLan);
    g_StringLanType(szLan, "�����򿪳�ʱ", "Door Lock Open Timeout");
    m_comboSubEventType.InsertString(EVENT_ACS_DOORLOCK_OPEN_TIMEOUT, szLan);
    g_StringLanType(szLan, "�׿�δ��Ȩ����ʧ��", "First Card Open Without Authorize");
    m_comboSubEventType.InsertString(EVENT_ACS_FIRSTCARD_OPEN_WITHOUT_AUTHORIZE, szLan);
    g_StringLanType(szLan, "���ݼ̵����Ͽ�", "Call Ladder Relay Break");
    m_comboSubEventType.InsertString(EVENT_ACS_CALL_LADDER_RELAY_BREAK, szLan);
    g_StringLanType(szLan, "���ݼ̵����պ�", "Call Ladder Relay Close");
    m_comboSubEventType.InsertString(EVENT_ACS_CALL_LADDER_RELAY_CLOSE, szLan);
    g_StringLanType(szLan, "�Զ������̵����Ͽ�", "Auto Key Relay Break");
    m_comboSubEventType.InsertString(EVENT_ACS_AUTO_KEY_RELAY_BREAK, szLan);
    g_StringLanType(szLan, "�Զ������̵����պ�", "Auto Key Relay Close");
    m_comboSubEventType.InsertString(EVENT_ACS_AUTO_KEY_RELAY_CLOSE, szLan);
    g_StringLanType(szLan, "�����ݿؼ̵����Ͽ�", "Key Control Relay Break");
    m_comboSubEventType.InsertString(EVENT_ACS_KEY_CONTROL_RELAY_BREAK, szLan);
    g_StringLanType(szLan, "�����ݿؼ̵����պ�", "Key Control Relay Close");
    m_comboSubEventType.InsertString(EVENT_ACS_KEY_CONTROL_RELAY_CLOSE, szLan);
    g_StringLanType(szLan, "�ÿͺ���", "Remote Visitor Call Ladder");
    m_comboSubEventType.InsertString(EVENT_ACS_REMOTE_VISITOR_CALL_LADDER, szLan);
    g_StringLanType(szLan, "ס������", "Remote House Hold Call Ladder");
    m_comboSubEventType.InsertString(EVENT_ACS_REMOTE_HOUSEHOLD_CALL_LADDER, szLan);
    g_StringLanType(szLan, "�Ϸ�����", "Legal Message");
    m_comboSubEventType.InsertString(EVENT_ACS_LEGAL_MESSAGE, szLan);
    g_StringLanType(szLan, "�Ƿ�����", "Illegal Message");
    m_comboSubEventType.InsertString(EVENT_ACS_ILLEGAL_MESSAGE, szLan);
    g_StringLanType(szLan, "β��ͨ��", "Trailing");
    m_comboSubEventType.InsertString(EVENT_ACS_TRAILING, szLan);
    g_StringLanType(szLan, "������", "Reverse Access");
    m_comboSubEventType.InsertString(EVENT_ACS_REVERSE_ACCESS, szLan);
    g_StringLanType(szLan, "������ײ", "Force Access");
    m_comboSubEventType.InsertString(EVENT_ACS_FORCE_ACCESS, szLan);
    g_StringLanType(szLan, "��Խ", "Climbing Over Gate");
    m_comboSubEventType.InsertString(EVENT_ACS_CLIMBING_OVER_GATE, szLan);
    g_StringLanType(szLan, "ͨ�г�ʱ", "Passing Timeout");
    m_comboSubEventType.InsertString(EVENT_ACS_PASSING_TIMEOUT, szLan);
    g_StringLanType(szLan, "�󴳱���", "Intrusion Alarm");
    m_comboSubEventType.InsertString(EVENT_ACS_INTRUSION_ALARM, szLan);
    g_StringLanType(szLan, "բ������ͨ��ʱδ��֤ͨ��", "Free Gate Pass Not Auth");
    m_comboSubEventType.InsertString(EVENT_ACS_FREE_GATE_PASS_NOT_AUTH, szLan);
    g_StringLanType(szLan, "�ڱ۱��赲", "Drop Arm Block");
    m_comboSubEventType.InsertString(EVENT_ACS_DROP_ARM_BLOCK, szLan);
    g_StringLanType(szLan, "�ڱ��赲����", "Drop Arm Block Resume");
    m_comboSubEventType.InsertString(EVENT_ACS_DROP_ARM_BLOCK_RESUME, szLan);
    g_StringLanType(szLan, "ң��������", "Remote Control Close Door");
    m_comboSubEventType.InsertString(EVENT_ACS_REMOTE_CONTROL_CLOSE_DOOR, szLan);
    g_StringLanType(szLan, "ң��������", "Remote Control Open Door");
    m_comboSubEventType.InsertString(EVENT_ACS_REMOTE_CONTROL_OPEN_DOOR, szLan);
    g_StringLanType(szLan, "ң����������", "Remote Control Always Open Door");
    m_comboSubEventType.InsertString(EVENT_ACS_REMOTE_CONTROL_ALWAYS_OPEN_DOOR, szLan);
    m_comboSubEventType.SetCurSel(EVENT_ACS_LEADER_CARD_OPEN_BEGIN);
}

void DlgEventCardLinkageCfg::SubEventTypeCardReader()
{
    // WORD  wSubEventType;  
    m_comboSubEventType.ResetContent();
    char szLan[1024] = { 0 };

    g_StringLanType(szLan, "в�ȱ���", "Stress Alarm");
    m_comboSubEventType.InsertString(EVENT_ACS_STRESS_ALARM, szLan);
    g_StringLanType(szLan, "���������𱨾�", "Card Reader Desmantle Alarm");
    m_comboSubEventType.InsertString(EVENT_ACS_CARD_READER_DESMANTLE_ALARM, szLan);
    g_StringLanType(szLan, "�Ϸ�����֤ͨ��", "Legal Card Pass");
    m_comboSubEventType.InsertString(EVENT_ACS_LEGAL_CARD_PASS, szLan);
    g_StringLanType(szLan, "ˢ����������֤ͨ��", "Card And Password Pass");
    m_comboSubEventType.InsertString(EVENT_ACS_CARD_AND_PSW_PASS, szLan);
    g_StringLanType(szLan, "ˢ����������֤ʧ��", "Card And Password Fail");
    m_comboSubEventType.InsertString(EVENT_ACS_CARD_AND_PSW_FAIL, szLan);
    g_StringLanType(szLan, "ˢ����������֤��ʱ", "Card And Password Timeout");
    m_comboSubEventType.InsertString(EVENT_ACS_CARD_AND_PSW_TIMEOUT, szLan);
    g_StringLanType(szLan, "��������֤ʧ�ܳ���", "Max Authenticate Fail");
    m_comboSubEventType.InsertString(EVENT_ACS_CARD_MAX_AUTHENTICATE_FAIL, szLan);
    g_StringLanType(szLan, "��δ����Ȩ��", "Card No Right");
    m_comboSubEventType.InsertString(EVENT_ACS_CARD_NO_RIGHT, szLan);
    g_StringLanType(szLan, "����ǰʱ����Ч", "Card Invalid Period");
    m_comboSubEventType.InsertString(EVENT_ACS_CARD_INVALID_PERIOD, szLan);
    g_StringLanType(szLan, "���Ź���", "Card Out Of Date");
    m_comboSubEventType.InsertString(EVENT_ACS_CARD_OUT_OF_DATE, szLan);
    g_StringLanType(szLan, "�޴˿���", "Invalid Card");
    m_comboSubEventType.InsertString(EVENT_ACS_INVALID_CARD, szLan);
    g_StringLanType(szLan, "��Ǳ����֤ʧ��", "Anti Sneak Fail");
    m_comboSubEventType.InsertString(EVENT_ACS_ANTI_SNEAK_FAIL, szLan);
    g_StringLanType(szLan, "������δ�ر�", "Interlock Door Not Close");
    m_comboSubEventType.InsertString(EVENT_ACS_INTERLOCK_DOOR_NOT_CLOSE, szLan);
    g_StringLanType(szLan, "ָ�Ʊȶ�ͨ��", "Fingerprint Compare Pass");
    m_comboSubEventType.InsertString(EVENT_ACS_FINGERPRINT_COMPARE_PASS, szLan);
    g_StringLanType(szLan, "ָ�Ʊȶ�ʧ��", "Fingerprint Compare Fail");
    m_comboSubEventType.InsertString(EVENT_ACS_FINGERPRINT_COMPARE_FAIL, szLan);
    g_StringLanType(szLan, "ˢ����ָ����֤ͨ��", "Card Fingerprint Verify Pass");
    m_comboSubEventType.InsertString(EVENT_ACS_CARD_FINGERPRINT_VERIFY_PASS, szLan);
    g_StringLanType(szLan, "ˢ����ָ����֤ʧ��", "Card Fingerprint Verify Fail");
    m_comboSubEventType.InsertString(EVENT_ACS_CARD_FINGERPRINT_VERIFY_FAIL, szLan);
    g_StringLanType(szLan, "ˢ����ָ����֤��ʱ", "Card Fingerprint Verify Timeout");
    m_comboSubEventType.InsertString(EVENT_ACS_CARD_FINGERPRINT_VERIFY_TIMEOUT, szLan);
    g_StringLanType(szLan, "ˢ����ָ�Ƽ�������֤ͨ��", "Card Fingerprint Password Verify Pass");
    m_comboSubEventType.InsertString(EVENT_ACS_CARD_FINGERPRINT_PASSWD_VERIFY_PASS, szLan);
    g_StringLanType(szLan, "ˢ����ָ�Ƽ�������֤ʧ��", "Card Fingerprint Password Verify Fail");
    m_comboSubEventType.InsertString(EVENT_ACS_CARD_FINGERPRINT_PASSWD_VERIFY_FAIL, szLan);
    g_StringLanType(szLan, "ˢ����ָ�Ƽ�������֤��ʱ", "Card Fingerprint Password Verify Timeout");
    m_comboSubEventType.InsertString(EVENT_ACS_CARD_FINGERPRINT_PASSWD_VERIFY_TIMEOUT, szLan);
    g_StringLanType(szLan, "ָ�Ƽ�������֤ͨ��", "Fingerprint Password Verify Pass");
    m_comboSubEventType.InsertString(EVENT_ACS_FINGERPRINT_PASSWD_VERIFY_PASS, szLan);
    g_StringLanType(szLan, "ָ�Ƽ�������֤ʧ��", "Fingerprint Password Verify Fail");
    m_comboSubEventType.InsertString(EVENT_ACS_FINGERPRINT_PASSWD_VERIFY_FAIL, szLan);
    g_StringLanType(szLan, "ָ�Ƽ�������֤��ʱ", "Fingerprint Password Verify Timeout");
    m_comboSubEventType.InsertString(EVENT_ACS_FINGERPRINT_PASSWD_VERIFY_TIMEOUT, szLan);
    g_StringLanType(szLan, "ָ�Ʋ�����", "Fingerprint Not Existence");
    m_comboSubEventType.InsertString(EVENT_ACS_FINGERPRINT_INEXISTENCE, szLan);
    g_StringLanType(szLan, "������֤ͨ��", "Face Verify Pass");
    m_comboSubEventType.InsertString(EVENT_ACS_FACE_VERIFY_PASS, szLan);
    g_StringLanType(szLan, "������֤ʧ��", "Face Verify Fail");
    m_comboSubEventType.InsertString(EVENT_ACS_FACE_VERIFY_FAIL, szLan);
    g_StringLanType(szLan, "������ָ����֤ͨ��", "Face And Fingerprint Verify Pass");
    m_comboSubEventType.InsertString(EVENT_ACS_FACE_AND_FP_VERIFY_PASS, szLan);
    g_StringLanType(szLan, "������ָ����֤ʧ��", "Face And Fingerprint Verify Fail");
    m_comboSubEventType.InsertString(EVENT_ACS_FACE_AND_FP_VERIFY_FAIL, szLan);
    g_StringLanType(szLan, "������ָ����֤��ʱ", "Face And Fingerprint Verify Timeout");
    m_comboSubEventType.InsertString(EVENT_ACS_FACE_AND_FP_VERIFY_TIMEOUT, szLan);
    g_StringLanType(szLan, "������������֤ͨ��", "Face And Password Verify Pass");
    m_comboSubEventType.InsertString(EVENT_ACS_FACE_AND_PW_VERIFY_PASS, szLan);
    g_StringLanType(szLan, "������������֤ʧ��", "Face And Password Verify Fail");
    m_comboSubEventType.InsertString(EVENT_ACS_FACE_AND_PW_VERIFY_FAIL, szLan);
    g_StringLanType(szLan, "������������֤��ʱ", "Face And Password Verify Timeout");
    m_comboSubEventType.InsertString(EVENT_ACS_FACE_AND_PW_VERIFY_TIMEOUT, szLan);
    g_StringLanType(szLan, "������ˢ����֤ͨ��", "Face And Card Verify Pass");
    m_comboSubEventType.InsertString(EVENT_ACS_FACE_AND_CARD_VERIFY_PASS, szLan);
    g_StringLanType(szLan, "������ˢ����֤ʧ��", "Face And Card Verify Fail");
    m_comboSubEventType.InsertString(EVENT_ACS_FACE_AND_CARD_VERIFY_FAIL, szLan);
    g_StringLanType(szLan, "������ˢ����֤��ʱ", "Face And Card Verify Timeout");
    m_comboSubEventType.InsertString(EVENT_ACS_FACE_AND_CARD_VERIFY_TIMEOUT, szLan);
    g_StringLanType(szLan, "�����������ָ����֤ͨ��", "Face And Password And Fingerprint Verify Pass");
    m_comboSubEventType.InsertString(EVENT_ACS_FACE_AND_PW_AND_FP_VERIFY_PASS, szLan);
    g_StringLanType(szLan, "�����������ָ����֤ʧ��", "Face And Password And Fingerprint Verify Fail");
    m_comboSubEventType.InsertString(EVENT_ACS_FACE_AND_PW_AND_FP_VERIFY_FAIL, szLan);
    g_StringLanType(szLan, "�����������ָ����֤��ʱ", "Face And Password And Fingerprint Verify Timeout");
    m_comboSubEventType.InsertString(EVENT_ACS_FACE_AND_PW_AND_FP_VERIFY_TIMEOUT, szLan);
    g_StringLanType(szLan, "������ˢ����ָ����֤ͨ��", "Face And Card And Fingerprint Verify Pass");
    m_comboSubEventType.InsertString(EVENT_ACS_FACE_AND_CARD_AND_FP_VERIFY_PASS, szLan);
    g_StringLanType(szLan, "������ˢ����ָ����֤ʧ��", "Face And Card And Fingerprint Verify Fail");
    m_comboSubEventType.InsertString(EVENT_ACS_FACE_AND_CARD_AND_FP_VERIFY_FAIL, szLan);
    g_StringLanType(szLan, "������ˢ����ָ����֤��ʱ", "Face And Card And Fingerprint Verify Timeout");
    m_comboSubEventType.InsertString(EVENT_ACS_FACE_AND_CARD_AND_FP_VERIFY_TIMEOUT, szLan);
    g_StringLanType(szLan, "���ż�ָ����֤ͨ��", "Employee No And Fingerprint Verify Pass");
    m_comboSubEventType.InsertString(EVENT_ACS_EMPLOYEENO_AND_FP_VERIFY_PASS, szLan);
    g_StringLanType(szLan, "���ż�ָ����֤ʧ��", "Employee No And Fingerprint Verify Fail");
    m_comboSubEventType.InsertString(EVENT_ACS_EMPLOYEENO_AND_FP_VERIFY_FAIL, szLan);
    g_StringLanType(szLan, "���ż�ָ����֤��ʱ", "Employee No And Fingerprint Verify Timeout");
    m_comboSubEventType.InsertString(EVENT_ACS_EMPLOYEENO_AND_FP_VERIFY_TIMEOUT, szLan);
    g_StringLanType(szLan, "���ż�ָ�Ƽ�������֤ͨ��", "Employee No And Fingerprint And Password Verify Pass");
    m_comboSubEventType.InsertString(EVENT_ACS_EMPLOYEENO_AND_FP_AND_PW_VERIFY_PASS, szLan);
    g_StringLanType(szLan, "���ż�ָ�Ƽ�������֤ʧ��", "Employee No And Fingerprint And Password Verify Fail");
    m_comboSubEventType.InsertString(EVENT_ACS_EMPLOYEENO_AND_FP_AND_PW_VERIFY_FAIL, szLan);
    g_StringLanType(szLan, "���ż�ָ�Ƽ�������֤��ʱ", "Employee No And Fingerprint And Password Verify Timeout");
    m_comboSubEventType.InsertString(EVENT_ACS_EMPLOYEENO_AND_FP_AND_PW_VERIFY_TIMEOUT, szLan);
    g_StringLanType(szLan, "���ż�������֤ͨ��", "Employee No And Face Verify Pass");
    m_comboSubEventType.InsertString(EVENT_ACS_EMPLOYEENO_AND_FACE_VERIFY_PASS, szLan);
    g_StringLanType(szLan, "���ż�������֤ʧ��", "Employee No And Face Verify Fail");
    m_comboSubEventType.InsertString(EVENT_ACS_EMPLOYEENO_AND_FACE_VERIFY_FAIL, szLan);
    g_StringLanType(szLan, "���ż�������֤��ʱ", "Employee No And Face Verify Timeout");
    m_comboSubEventType.InsertString(EVENT_ACS_EMPLOYEENO_AND_FACE_VERIFY_TIMEOUT, szLan);
    g_StringLanType(szLan, "����ʶ��ʧ��", "Face Recognize Fail");
    m_comboSubEventType.InsertString(EVENT_ACS_FACE_RECOGNIZE_FAIL, szLan);
    g_StringLanType(szLan, "���ż�������֤ͨ��", "Employee No And Password Pass");
    m_comboSubEventType.InsertString(EVENT_ACS_EMPLOYEENO_AND_PW_PASS, szLan);
    g_StringLanType(szLan, "���ż�������֤ʧ��", "Employee No And Password Fail");
    m_comboSubEventType.InsertString(EVENT_ACS_EMPLOYEENO_AND_PW_FAIL, szLan);
    g_StringLanType(szLan, "���ż�������֤��ʱ", "Employee No And Password Timeout");
    m_comboSubEventType.InsertString(EVENT_ACS_EMPLOYEENO_AND_PW_TIMEOUT, szLan);
    g_StringLanType(szLan, "���˼��ʧ��", "Human Detect Fail");
    m_comboSubEventType.InsertString(EVENT_ACS_HUMAN_DETECT_FAIL, szLan);
    g_StringLanType(szLan, "��֤�ȶ�ͨ��", "People And Id Card Compare Pass");
    m_comboSubEventType.InsertString(EVENT_ACS_PEOPLE_AND_ID_CARD_COMPARE_PASS, szLan);
    g_StringLanType(szLan, "��֤�ȶ�ʧ��", "People And Id Card Compare Fail");
    m_comboSubEventType.InsertString(EVENT_ACS_PEOPLE_AND_ID_CARD_COMPARE_FAIL, szLan);
    g_StringLanType(szLan, "��״̬���ջ�����״̬��֤ʧ��", "Door Open Or Dormant Fail");
    m_comboSubEventType.InsertString(EVENT_ACS_DOOR_OPEN_OR_DORMANT_FAIL, szLan);
    g_StringLanType(szLan, "��֤�ƻ�����ģʽ��֤ʧ��", "Auth Plan Dormant Fail");
    m_comboSubEventType.InsertString(EVENT_ACS_AUTH_PLAN_DORMANT_FAIL, szLan);
    g_StringLanType(szLan, "������У��ʧ��", "Card Encrypt Verify Fail");
    m_comboSubEventType.InsertString(EVENT_ACS_CARD_ENCRYPT_VERIFY_FAIL, szLan);
    g_StringLanType(szLan, "��Ǳ�ط�����Ӧ��ʧ��", "Submarineback Relay Fail");
    m_comboSubEventType.InsertString(EVENT_ACS_SUBMARINEBACK_REPLY_FAIL, szLan);
    g_StringLanType(szLan, "���벻ƥ��", "Password Mismatch");
    m_comboSubEventType.InsertString(EVENT_ACS_PASSWORD_MISMATCH, szLan);
    g_StringLanType(szLan, "���Ų�����", "Employee No Not Exist");
    m_comboSubEventType.InsertString(EVENT_ACS_EMPLOYEE_NO_NOT_EXIST, szLan);
    g_StringLanType(szLan, "�����֤ͨ��", "Combined Verify Pass");
    m_comboSubEventType.InsertString(EVENT_ACS_COMBINED_VERIFY_PASS, szLan);
    g_StringLanType(szLan, "�����֤��ʱ", "Combined Verify Timeout");
    m_comboSubEventType.InsertString(EVENT_ACS_COMBINED_VERIFY_TIMEOUT, szLan);
    g_StringLanType(szLan, "��֤��ʽ��ƥ��", "Verify Mode Mismatch");
    m_comboSubEventType.InsertString(EVENT_ACS_VERIFY_MODE_MISMATCH, szLan);
    m_comboSubEventType.SetCurSel(EVENT_ACS_STRESS_ALARM);
}

UINT8 DlgEventCardLinkageCfg::charToData(const char ch)
{
    switch (ch)
    {
    case '0':
        return 0;
    case '1':
        return 1;
    case '2':
        return 2;
    case '3':
        return 3;
    case '4':
        return 4;
    case '5':
        return 5;
    case '6':
        return 6;
    case '7':
        return 7;
    case '8':
        return 8;
    case '9':
        return 9;
    case 'a':
    case 'A':
        return 10;
    case 'b':
    case 'B':
        return 11;
    case 'c':
    case 'C':
        return 12;
    case 'd':
    case 'D':
        return 13;
    case 'e':
    case 'E':
        return 14;
    case 'f':
    case 'F':
        return 15;
    }
    return 0;
}

bool DlgEventCardLinkageCfg::StrToMac(const char * szMac, UINT8 * pMac)
{
    const char * pTemp = szMac;
    for (int i = 0; i < 6; ++i)
    {
        pMac[i] = charToData(*pTemp++) * 16;
        pMac[i] += charToData(*pTemp++);
        pTemp++;
    }
    return TRUE;
}

void DlgEventCardLinkageCfg::OnClickTreeReaderStopBuzzer(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);

    CPoint pt(0, 0);
    CRect rc(0, 0, 0, 0);
    GetCursorPos(&pt);
    m_treeCardReaderStopBuzzer.ScreenToClient(&pt);
    GetDlgItem(IDC_TREE_READER_STOP_BUZZER)->GetWindowRect(&rc);
    m_treeCardReaderStopBuzzer.ScreenToClient(&rc);
    pt.x = pt.x - rc.left;
    pt.y = pt.y - rc.top;

    UINT uFlag = 0;
    HTREEITEM hSelect = m_treeCardReaderStopBuzzer.HitTest(pt, &uFlag);

    if (NULL == hSelect)
    {
        return;
    }
    m_treeCardReaderStopBuzzer.SelectItem(hSelect);
    DWORD dwIndex = m_treeCardReaderStopBuzzer.GetItemData(hSelect);
    BOOL bCheck = m_treeCardReaderStopBuzzer.GetCheck(hSelect);
    m_treeCardReaderStopBuzzer.SetCheck(hSelect, !bCheck);
    m_struEventCardLinkageCfgV51.byReaderStopBuzzer[dwIndex] = !bCheck;
    //switch checkbox status on click
    if (uFlag & LVHT_ONITEM || uFlag & LVHT_TOLEFT || uFlag & LVHT_ONITEMLABEL)//LVHT_TOLEFT)
    {
        m_treeCardReaderStopBuzzer.SetCheck(hSelect, !bCheck);
        m_struEventCardLinkageCfgV51.byReaderStopBuzzer[dwIndex] = !bCheck;
    }
    else
    {
        m_treeCardReaderStopBuzzer.SetCheck(hSelect, bCheck);
        m_struEventCardLinkageCfgV51.byReaderStopBuzzer[dwIndex] = bCheck;
    }
    UpdateData(FALSE);
    *pResult = 0;
}
