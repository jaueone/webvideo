// DlgAlarmhostExternalDevLimitValue.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgAlarmhostPointCfg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDlgAlarmhostPointCfg dialog

DWORD WINAPI CDlgAlarmhostPointCfg::GetConfigThread(LPVOID lpArg)
{
	CDlgAlarmhostPointCfg* pThis = reinterpret_cast<CDlgAlarmhostPointCfg*>(lpArg);
	int bRet = 0;
	char szLan[128] = {0};
	while(pThis->m_bGetNext)
	{
		bRet = NET_DVR_GetNextRemoteConfig(pThis->m_lHandle, &pThis->m_struVariCfg[pThis->m_iVarCount], sizeof(NET_DVR_ALARM_VARIABLE_CFG));
		if (bRet == NET_SDK_GET_NEXT_STATUS_SUCCESS)
		{
			pThis->AddInfoToDlg();
		}
		else
		{
			if (bRet == NET_SDK_GET_NETX_STATUS_NEED_WAIT)
			{
				g_StringLanType(szLan, "���ڻ�ȡ...��ȴ�", "getting data, please wait");
				g_pMainDlg->AddLog(pThis->m_iDeviceIndex, OPERATION_SUCC_T, szLan);
				Sleep(5);
				continue;
			}
			if (bRet == NET_SDK_GET_NEXT_STATUS_FINISH)
			{
				g_StringLanType(szLan, "��ȡ���ý���!", "Get route info Ending");
				g_pMainDlg->AddLog(pThis->m_iDeviceIndex, OPERATION_SUCC_T, szLan);
				//AfxMessageBox(szLan);
				break;
			}
			else if(bRet == NET_SDK_GET_NEXT_STATUS_FAILED)
			{
				g_StringLanType(szLan, "��ȡ����ʧ��!", "Get route info failed");
				AfxMessageBox(szLan);
				break;
			}
			else
			{
				g_StringLanType(szLan, "δ֪״̬", "Unknown status");
				AfxMessageBox(szLan);
				break;
			}
		}
	}
	if (-1 != pThis->m_lHandle)
	{
		if (!NET_DVR_StopRemoteConfig(pThis->m_lHandle))
		{
			g_pMainDlg->AddLog(pThis->m_iDeviceIndex, OPERATION_FAIL_T, "Stop Remote Config Failed");
			pThis->m_bGetNext = FALSE;
		}
		else
		{
			g_pMainDlg->AddLog(pThis->m_iDeviceIndex, OPERATION_SUCC_T, "Stop Remote Config Successful");
			pThis->m_bGetNext = FALSE;
			pThis->m_lHandle = -1;
		}
	}
	return 0 ;
}

void CDlgAlarmhostPointCfg::AddInfoToDlg() 
{
	char szStr[128] = {0};
	//������
	int iCount = m_cmVarNo.GetCount();
	sprintf(szStr, "%d", m_struVariCfg[m_iVarCount].wVariableIndex);
	m_cmVarNo.AddString(szStr);
	m_cmVarNo.SetItemData(iCount, m_struVariCfg[m_iVarCount++].wVariableIndex);
}

CDlgAlarmhostPointCfg::CDlgAlarmhostPointCfg(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAlarmhostPointCfg::IDD, pParent)
	, m_lUserID(-1)
	, m_iDeviceIndex(-1)
	, m_hGetInfoThread(NULL)
	, m_lHandle(-1)
	, m_iItem(-1)
	, m_iSubItem(-1)
	, m_iVarCount(0)
    , m_byLinkagePicCount(0)
    , m_dwLinkageRecTime(0)
    , m_szOSDUnit(_T(""))
{
	//{{AFX_DATA_INIT(CDlgAlarmhostPointCfg)
	m_fLineValue1 = 0.0f;
	m_fLineValue2 = 0.0f;
	m_fLineValue3 = 0.0f;
	m_fLineValue4 = 0.0f;
	m_fMeasureHigh = 0.0f;
	m_fMeasureLow = 0.0f;
	m_fSensitive = 0.0f;
	m_dwPointNO = 0;
	m_csPointName = _T("");
	m_csVarName = _T("");
	//}}AFX_DATA_INIT
    memset(&m_byNVRChanType, 0, MAX_LINKAGE_CHAN_NUM);
    memset(&m_byPicNum, 0, MAX_LINKAGE_CHAN_NUM);
    memset(&m_wRecTime, 0, MAX_LINKAGE_CHAN_NUM);
    memset(&m_struAlarmLikageParam, 0, sizeof(NET_DVR_ALARM_LINKAGE_PARAM));
    memset(m_byTriggerLinkageInfo, 0, ALARMHOST_ALARMOUT_NUM);
    memset(m_byTriggerLinkageEnable, 0, ALARMHOST_ALARMOUT_NUM);
}


void CDlgAlarmhostPointCfg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgAlarmhostPointCfg)
    DDX_Control(pDX, IDC_LIST_OSD_IP_CHAN, m_listOSDIPChan);
    DDX_Control(pDX, IDC_COMBO_LIMIT_LINE_TYPE, m_cmLimitLineType);
    DDX_Control(pDX, IDC_LIST_OSD_CHAN, m_listOsdChan);
    DDX_Control(pDX, IDC_COMBO_VAR_TYPE, m_cmVarType);
    DDX_Control(pDX, IDC_COMBO_VAR_NO, m_cmVarNo);
    DDX_Control(pDX, IDC_COMBO_CHAN_TYPE, m_cmChanType);
    DDX_Control(pDX, IDC_BTN_SET_POINT_CFG, m_btnSet);
    DDX_Control(pDX, IDC_BTN_GET_POINT_CFG, m_btnGet);
    DDX_Control(pDX, IDC_COMBO_SLOT_INDEX, m_cmSlotIndex);
    DDX_Control(pDX, IDC_COMBO_CHAN_INDEX, m_cmChanIndex);
    DDX_Control(pDX, IDC_COMBO_LINE4, m_comboLineMode4);
    DDX_Control(pDX, IDC_COMBO_LINE3, m_comboLineMode3);
    DDX_Control(pDX, IDC_COMBO_LINE2, m_comboLineMode2);
    DDX_Control(pDX, IDC_COMBO_LINE1, m_comboLineMode1);
    DDX_Text(pDX, IDC_EDIT_LINE_VALUE1, m_fLineValue1);
    DDX_Text(pDX, IDC_EDIT_LINE_VALUE2, m_fLineValue2);
    DDX_Text(pDX, IDC_EDIT_LINE_VALUE3, m_fLineValue3);
    DDX_Text(pDX, IDC_EDIT_LINE_VALUE4, m_fLineValue4);
    DDX_Text(pDX, IDC_EDIT_MEASURE_HIGH, m_fMeasureHigh);
    DDX_Text(pDX, IDC_EDIT_MEASURE_LOW, m_fMeasureLow);
    DDX_Text(pDX, IDC_EDIT_SENSITIVE, m_fSensitive);
    DDX_Text(pDX, IDC_EDIT_POINT_NO, m_dwPointNO);
    DDX_Text(pDX, IDC_EDIT_POINT_NAME, m_csPointName);
    DDX_Text(pDX, IDC_EDIT_VAR_NAME, m_csVarName);
    //}}AFX_DATA_MAP
    DDX_Control(pDX, IDC_COMBO_ALARM_NVR_CHANNEL, m_cmNvrChanNo);
    DDX_Control(pDX, IDC_COMBO_ALARM_NVR_EVENT_TYPE2, m_cmNvrChanEventType);
    DDX_Text(pDX, IDC_EDIT_ALARM_NVR_GRAP_PIC_COUNT, m_byLinkagePicCount);
    DDX_Text(pDX, IDC_EDIT_ALARM_NVR_REC_TIME, m_dwLinkageRecTime);
    DDX_Control(pDX, IDC_COMBO_OBTAIN_VAR_COND, m_cmVariableCond);
    DDX_Text(pDX, IDC_EDIT_OSD_OVERLAY_UNIT, m_szOSDUnit);
    DDX_Control(pDX, IDC_COMBO_TRIGGER_NO, m_cmbTriggerNo);
    DDX_Control(pDX, IDC_CHK_ALARM_LINE1, m_bAlarmLine1);
    DDX_Control(pDX, IDC_CHK_ALARM_LINE2, m_bAlarmLine2);
    DDX_Control(pDX, IDC_CHK_ALARM_LINE3, m_bAlarmLine3);
    DDX_Control(pDX, IDC_CHK_ALARM_LINE4, m_bAlarmLine4);
    DDX_Control(pDX, IDC_CHK_BREAK_DOWN, m_bTriggerBreakDown);
    DDX_Control(pDX, IDC_CHK_TRIGGER_ENABLED, m_chkTriggerEnabled);
    DDX_Control(pDX, IDC_COMBO_LINKAGE_IPC_TYPE, m_cmbLinkageIPCCfg);
    DDX_Control(pDX, IDC_LIST_OSD_IP_SWITCH_CHAN, m_listOSDIPSwitchChan);
    DDX_Control(pDX, IDC_CHK_OSD_ENABLED, m_chkOSDEnabled);
}


BEGIN_MESSAGE_MAP(CDlgAlarmhostPointCfg, CDialog)
	//{{AFX_MSG_MAP(CDlgAlarmhostPointCfg)
	ON_BN_CLICKED(IDC_BTN_GET, OnBtnGet)
	ON_BN_CLICKED(IDC_BTN_GET_POINT_CFG, OnBtnGetPointCfg)
	ON_BN_CLICKED(IDC_BTN_SET_POINT_CFG, OnBtnSetPointCfg)
	ON_CBN_SELCHANGE(IDC_COMBO_CHAN_TYPE, OnSelchangeComboChanType)
	ON_BN_CLICKED(IDC_BTN_STOP, OnBtnStop)
	ON_CBN_SELCHANGE(IDC_COMBO_VAR_NO, OnSelchangeComboVarNo)
	//}}AFX_MSG_MAP
    ON_CBN_SELCHANGE(IDC_COMBO_ALARM_NVR_CHANNEL, &CDlgAlarmhostPointCfg::OnCbnSelchangeComboAlarmNvrChannel)
    ON_BN_CLICKED(IDC_BTN_SAVE_ALARM_NVR_CHAN_INFO, &CDlgAlarmhostPointCfg::OnBnClickedBtnSaveAlarmNvrChanInfo)
    ON_BN_CLICKED(IDC_BTN_SET_CHAN_LINKAGE_CFG, &CDlgAlarmhostPointCfg::OnBnClickedBtnSetChanLinkageCfg)
    ON_BN_CLICKED(IDC_BTN_GET_CHAN_LINKAGE_CFG, &CDlgAlarmhostPointCfg::OnBnClickedBtnGetChanLinkageCfg)
    ON_CBN_SELCHANGE(IDC_COMBO_TRIGGER_NO, &CDlgAlarmhostPointCfg::OnCbnSelchangeComboTriggerNo)
    ON_BN_CLICKED(IDC_CHK_TRIGGER_ENABLED, &CDlgAlarmhostPointCfg::OnBnClickedChkTriggerEnabled)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAlarmhostPointCfg message handlers

BOOL CDlgAlarmhostPointCfg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
	m_lUserID = g_struDeviceInfo[m_iDeviceIndex].lLoginID;
	m_lAnalogChannel = g_struDeviceInfo[m_iDeviceIndex].iAnalogChanNum;
	m_lIPChanNum = g_struDeviceInfo[m_iDeviceIndex].iIPChanNum;
	memset(m_struVariCfg, 0, sizeof(m_struVariCfg));

	m_struAbility.dwSize = sizeof(m_struAbility);
	if(!NET_DVR_GetDeviceAbility(m_lUserID, ALARMHOST_ABILITY, NULL, 0, (char*)&m_struAbility, sizeof(m_struAbility)))
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "Get Alarmhost Ability failed");
		return FALSE;
	}
	InitWnd();

	m_cmChanIndex.SetCurSel(0);
	m_cmSlotIndex.SetCurSel(0);
	m_cmChanType.SetCurSel(0);
	UpdateData(FALSE);
	OnSelchangeComboChanType() ;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgAlarmhostPointCfg::OnBtnGet() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	char szLan[128] = {0};
	m_struInputInfo.dwSize = sizeof(m_struInputInfo);
	m_iVarCount = 0;
	memset(m_struVariCfg, 0, sizeof(m_struVariCfg));
	if (m_lHandle>=0)
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "It is getting data, wait for a moment");
	}
	else
	{
		m_iVarCount = 0;
		m_cmVarNo.ResetContent();
		memset((char*)&m_struInputInfo, 0, sizeof(m_struInputInfo));
		m_struInputInfo.dwSize = sizeof(m_struInputInfo);
		m_struInputInfo.byChanType = m_cmChanType.GetCurSel() + 1;
		m_struInputInfo.dwChanNo = m_cmChanIndex.GetCurSel() + 1;
		m_struInputInfo.dwSubChanNo = m_cmSlotIndex.GetCurSel() + 1;
        m_struInputInfo.byRemoteType = m_cmVariableCond.GetCurSel();
		m_lHandle = NET_DVR_StartRemoteConfig(m_lUserID, NET_DVR_GET_ALL_VARIABLE_INFO, &m_struInputInfo, sizeof(m_struInputInfo), NULL, this);
		if (m_lHandle>=0)
		{
			m_bGetNext = TRUE;
			g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "Start Remote Config successfully");
			DWORD dwThreadId;
			m_hGetInfoThread = CreateThread(NULL,0,LPTHREAD_START_ROUTINE(GetConfigThread), this, 0, &dwThreadId);
		}
		else
		{
			g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "Start Remote Config failed");
			return;
		}
	}
}

void CDlgAlarmhostPointCfg::OnBtnGetPointCfg() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_struInputInfo.dwSize = sizeof(m_struInputInfo);
	char szListStr[NAME_LEN] = {0};
	char szTemp[256] = {0};
	DWORD dwReturn = 0; 
	DWORD dwVarNo = 0;

	//�������
	dwVarNo = m_cmVarNo.GetItemData(m_cmVarNo.GetCurSel());
	
	m_struInputInfo.dwSize = sizeof(m_struInputInfo);
	m_struInputInfo.byChanType = m_cmChanType.GetCurSel() + 1;
	m_struInputInfo.dwChanNo = m_cmChanIndex.GetCurSel() + 1;
	m_struInputInfo.dwSubChanNo = m_cmSlotIndex.GetCurSel() + 1;
	m_struInputInfo.dwVariableNo = dwVarNo;

	if (!NET_DVR_GetDeviceConfig(m_lUserID, NET_DVR_GET_ALARM_POINT_CFG, 1, &m_struInputInfo, sizeof(m_struInputInfo), &dwStatusList, &m_struPointCfg, sizeof(m_struPointCfg)))
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "Get point config failed");
		return;
	}
	else
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "Get point config successful");
	}

	m_dwPointNO = m_struPointCfg.dwPointNo;
    m_csPointName.Format("%s", m_struPointCfg.sPointDescribe);
	
    m_fMeasureHigh = (float)((float)m_struPointCfg.struPointParam.struSensor.nMeasureHigh/1000);
    m_fMeasureLow = (float)((float)m_struPointCfg.struPointParam.struSensor.nMeasureLow/1000);
    m_fLineValue1 = (float)((float)m_struPointCfg.struPointParam.struSensor.nAlarmValue[0]/1000);
    m_fLineValue2 = (float)((float)m_struPointCfg.struPointParam.struSensor.nAlarmValue[1]/1000);
    m_fLineValue3 = (float)((float)m_struPointCfg.struPointParam.struSensor.nAlarmValue[2]/1000);
    m_fLineValue4 = (float)((float)m_struPointCfg.struPointParam.struSensor.nAlarmValue[3]/1000);
	
    m_comboLineMode1.SetCurSel((m_struPointCfg.struPointParam.struSensor.byAlarmMode >> 0) & 0x01 );
    m_comboLineMode2.SetCurSel((m_struPointCfg.struPointParam.struSensor.byAlarmMode >> 1) & 0x01);
    m_comboLineMode3.SetCurSel((m_struPointCfg.struPointParam.struSensor.byAlarmMode >> 2) & 0x01);
    m_comboLineMode4.SetCurSel((m_struPointCfg.struPointParam.struSensor.byAlarmMode >> 3) & 0x01);
	
	m_fSensitive = (float)((float)m_struPointCfg.struPointParam.struSensor.dwSensitive/10);

	int i = 0;
    for (i = 0; i < m_listOsdChan.GetItemCount(); i++)
    {
        m_listOsdChan.SetCheck(i, ((m_struPointCfg.struPointParam.struSensor.dwOsdCfg >> i ) & 0x01));
    }

	for (i = 0; i < m_listOSDIPChan.GetItemCount(); i++)
    {
        m_listOSDIPChan.SetCheck(i, ((m_struPointCfg.struPointParam.struSensor.dwIPChanOsd >> i ) & 0x01));
    }

    for (i = 0; i < m_listOSDIPSwitchChan.GetItemCount(); i++)
    {
        m_listOSDIPSwitchChan.SetCheck(i, ((m_struPointCfg.struPointParam.struSwitch.dwIPCChanOsd >> i) & 0x01));
    }

    m_szOSDUnit.Format("%s", m_struPointCfg.struPointParam.struSensor.szOSDUnit);

    UpdateData(FALSE);
}

void CDlgAlarmhostPointCfg::OnBtnSetPointCfg() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	DWORD dwChanIndex = 0;
	DWORD dwSlotIndex = 0;

	memset((char*)&m_struPointCfg, 0, sizeof(m_struPointCfg));
	m_struPointCfg.dwSize = sizeof(m_struPointCfg);
	m_struPointCfg.byPointType = m_cmVarType.GetCurSel() + 1;

    strncpy((char*)m_struPointCfg.sPointDescribe, m_csPointName.GetBuffer(0), NAME_LEN);
    m_struPointCfg.dwPointNo = m_dwPointNO;

    m_struPointCfg.struPointParam.struSensor.nMeasureHigh = (DWORD)(m_fMeasureHigh * 1000);
    m_struPointCfg.struPointParam.struSensor.nMeasureLow = (DWORD)(m_fMeasureLow * 1000);
    m_struPointCfg.struPointParam.struSensor.nAlarmValue[0] =  (DWORD)(m_fLineValue1 * 1000);
	m_struPointCfg.struPointParam.struSensor.nAlarmValue[1] =  (DWORD)(m_fLineValue2 * 1000);
	m_struPointCfg.struPointParam.struSensor.nAlarmValue[2] =  (DWORD)(m_fLineValue3 * 1000);
	m_struPointCfg.struPointParam.struSensor.nAlarmValue[3] =  (DWORD)(m_fLineValue4 * 1000);
    memcpy(m_struPointCfg.struPointParam.struSensor.szOSDUnit, (LPCSTR)m_szOSDUnit, MAX_OSD_UNIT_LEN);
    
    m_struPointCfg.struPointParam.struSensor.byAlarmMode = ((m_comboLineMode1.GetCurSel() << 0) | (m_comboLineMode2.GetCurSel() << 1)\
        | (m_comboLineMode3.GetCurSel() << 2) | (m_comboLineMode4.GetCurSel() << 3)  );
    
    m_struPointCfg.struPointParam.struSensor.dwSensitive = (DWORD)(m_fSensitive * 10);

    m_struPointCfg.struPointParam.struSensor.dwOsdCfg = 0;
	int i = 0;
    for (i = 0; i < m_listOsdChan.GetItemCount(); i++)
    {
        m_struPointCfg.struPointParam.struSensor.dwOsdCfg |=  (m_listOsdChan.GetCheck(i) << i);
    }
	
	m_struPointCfg.struPointParam.struSensor.dwIPChanOsd = 0;
    for (i = 0; i < m_listOSDIPChan.GetItemCount(); i++)
    {
        m_struPointCfg.struPointParam.struSensor.dwIPChanOsd |=  (m_listOSDIPChan.GetCheck(i) << i);
    }

    for  ( i = 0; i < m_listOSDIPSwitchChan.GetItemCount(); i++)
    {
        m_struPointCfg.struPointParam.struSwitch.dwIPCChanOsd |= (m_listOSDIPSwitchChan.GetCheck(i) << i);
    }

	memset((char*)&m_struInputInfo, 0, sizeof(m_struInputInfo));
	m_struInputInfo.dwSize = sizeof(m_struInputInfo);
	m_struInputInfo.byChanType = m_cmChanType.GetCurSel() + 1;
	m_struInputInfo.dwChanNo = m_cmChanIndex.GetCurSel() + 1;
	m_struInputInfo.dwSubChanNo = m_cmSlotIndex.GetCurSel() + 1;
	m_struInputInfo.dwVariableNo = m_cmVarNo.GetItemData(m_cmVarNo.GetCurSel());

	if (!NET_DVR_SetDeviceConfig(m_lUserID, NET_DVR_SET_ALARM_POINT_CFG, 1, &m_struInputInfo, sizeof(m_struInputInfo), &dwStatusList, &m_struPointCfg, sizeof(m_struPointCfg)))
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "Set point config failed");
		return;
	}
	else
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "Set point config successful");
	}
}


void CDlgAlarmhostPointCfg::OnSelchangeComboChanType() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString csStr;
	m_cmChanIndex.ResetContent();
	BYTE byChanType = m_cmChanType.GetCurSel() + 1;
	if (1 == byChanType)
	{
		//ģ����
		for (int i=0; i<(m_struAbility.wTotalSensorNum/*m_struAbility.wExpandSensorNum */); i++)
		{
			csStr.Format("Chan%d", i+1);
			m_cmChanIndex.AddString(csStr);
		}
		m_cmSlotIndex.EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MEASURE_HIGH)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MEASURE_LOW)->EnableWindow(TRUE);

		GetDlgItem(IDC_COMBO_LINE1)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_LINE2)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_LINE3)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_LINE4)->EnableWindow(TRUE);

		GetDlgItem(IDC_EDIT_LINE_VALUE1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LINE_VALUE2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LINE_VALUE3)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LINE_VALUE4)->EnableWindow(TRUE);
		
		GetDlgItem(IDC_EDIT_SENSITIVE)->EnableWindow(TRUE);
	}
	else if (2 == byChanType)
	{
		//������
		for (int i=0; i<m_struAbility.wLocalAlarmInNum + m_struAbility.wExpandAlarmInNum; i++)
		{
			csStr.Format("Chan%d", i+1);
			m_cmChanIndex.AddString(csStr);
		}
		m_cmSlotIndex.EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MEASURE_HIGH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MEASURE_LOW)->EnableWindow(FALSE);
		
		GetDlgItem(IDC_COMBO_LINE1)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_LINE2)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_LINE3)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_LINE4)->EnableWindow(FALSE);
		
		GetDlgItem(IDC_EDIT_LINE_VALUE1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LINE_VALUE2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LINE_VALUE3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LINE_VALUE4)->EnableWindow(FALSE);
		
		GetDlgItem(IDC_EDIT_SENSITIVE)->EnableWindow(FALSE);
	}
	else if (3 == byChanType)
	{
		//485
		for (int i=0; i<(m_struAbility.wLocalRs485Num +  m_struAbility.wExpandRs485Num); i++)
		{
			csStr.Format("Chan%d", i+1);
			m_cmChanIndex.AddString(csStr);
		}
		m_cmSlotIndex.EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MEASURE_HIGH)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDIT_MEASURE_LOW)->EnableWindow(TRUE);
		
		GetDlgItem(IDC_COMBO_LINE1)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_LINE2)->EnableWindow(TRUE);
        GetDlgItem(IDC_COMBO_LINE3)->EnableWindow(TRUE);
        GetDlgItem(IDC_COMBO_LINE4)->EnableWindow(TRUE);
		
		GetDlgItem(IDC_EDIT_LINE_VALUE1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LINE_VALUE2)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDIT_LINE_VALUE3)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDIT_LINE_VALUE4)->EnableWindow(TRUE);
		
        GetDlgItem(IDC_EDIT_SENSITIVE)->EnableWindow(TRUE);
	}
	m_cmChanIndex.SetCurSel(0);
	UpdateData(FALSE);
}

void CDlgAlarmhostPointCfg::OnBtnStop() 
{
	// TODO: Add your control notification handler code here
	if (-1 == m_lHandle)
	{
		return;
	}
	if (!NET_DVR_StopRemoteConfig(m_lHandle))
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "Stop Remote Config Failed");
		m_bGetNext = FALSE;
	}
	else
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "Stop Remote Config Successful");
		m_bGetNext = FALSE;
		m_lHandle = -1;
	}
}

void CDlgAlarmhostPointCfg::InitWnd()
{
    char szLan[256] = {0};
	CString csStr;
    m_listOsdChan.SetExtendedStyle(LVS_EX_CHECKBOXES);
	int i = 0;
	for (i = 0; i < m_lAnalogChannel; i++)
    {
        sprintf(szLan, "Chan%d", (i+1));
        m_listOsdChan.InsertItem(i, szLan);
    }

	m_listOSDIPChan.SetExtendedStyle(LVS_EX_CHECKBOXES);
	for (i = 0; i < m_lIPChanNum; i++)
    {
        sprintf(szLan, "IPChan%d", (i+1));
        m_listOSDIPChan.InsertItem(i, szLan);
    }

    m_listOSDIPSwitchChan.SetExtendedStyle(LVS_EX_CHECKBOXES);
    for (i = 0; i < m_lIPChanNum; i++)
    {
        sprintf(szLan, "IPChan%d", (i + 1));
        m_listOSDIPSwitchChan.InsertItem(i, szLan);
    }

	m_cmChanType.ResetContent();
	m_cmChanType.AddString("local sensor");
	m_cmChanType.AddString("local switch");
	m_cmChanType.AddString("485 chan");
	
	m_cmSlotIndex.ResetContent();
	for (i=0; i<m_struAbility.by485SlotNum; i++)
	{
		csStr.Format("Slot%d", i+1);
		m_cmSlotIndex.AddString(csStr);
	}

    //�����̵������
    m_cmbTriggerNo.ResetContent();
    for (i = 0; i < ALARMHOST_ALARMOUT_NUM; i++)
    {
        csStr.Format("%d", i + 1);
        m_cmbTriggerNo.AddString(csStr);
    }

	m_comboLineMode1.ResetContent();
	m_comboLineMode2.ResetContent();
	m_comboLineMode3.ResetContent();
	m_comboLineMode4.ResetContent();
	g_StringLanType(szLan, "����", "Lower limit");
	m_comboLineMode1.AddString(szLan);
	m_comboLineMode2.AddString(szLan);
	m_comboLineMode3.AddString(szLan);
	m_comboLineMode4.AddString(szLan);

	g_StringLanType(szLan, "����", "Upper limit");
	m_comboLineMode1.AddString(szLan);
	m_comboLineMode2.AddString(szLan);
	m_comboLineMode3.AddString(szLan);
	m_comboLineMode4.AddString(szLan);

	//��������
	m_cmVarType.ResetContent();
	g_StringLanType(szLan, "ģ����", "Sensor");
	m_cmVarType.AddString(szLan);
	g_StringLanType(szLan, "������", "Switch");
	m_cmVarType.AddString(szLan);

	//������
	m_cmVarNo.ResetContent();

	m_cmLimitLineType.ResetContent();
	g_StringLanType(szLan, "��֧��", "Not Support");
	m_cmLimitLineType.AddString(szLan);
	g_StringLanType(szLan, "����", "Two lines");
	m_cmLimitLineType.AddString(szLan);
	g_StringLanType(szLan, "����", "Four lines");
	m_cmLimitLineType.AddString(szLan);
}

void CDlgAlarmhostPointCfg::OnSelchangeComboVarNo() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int iIndex = m_cmVarNo.GetCurSel();
	m_cmVarType.SetCurSel(m_struVariCfg[iIndex].wVariableType - 1);
	m_csVarName.Format("%s", m_struVariCfg[iIndex].sDescribe);
	m_cmLimitLineType.SetCurSel(m_struVariCfg[iIndex].byLimitLineType);
    m_chkOSDEnabled.SetCheck(m_struVariCfg[iIndex].byOsdEnabled);
	UpdateData(FALSE);
}


/************************************************************************** 
 *  @fn void CDlgAlarmhostPointCfg::OnCbnSelchangeComboAlarmNvrChannel()
 *  @brief  �л�NVRͨ�����鿴NVRͨ���ı��������¼�����
 *  @return void
 *************************************************************************/
void CDlgAlarmhostPointCfg::OnCbnSelchangeComboAlarmNvrChannel()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);
    BYTE byNvrChanNo = m_cmNvrChanNo.GetCurSel() + 1;
    m_cmNvrChanEventType.SetCurSel(m_byNVRChanType[byNvrChanNo - 1]);
    m_byLinkagePicCount = m_byPicNum[byNvrChanNo - 1];
    m_dwLinkageRecTime = (DWORD)m_wRecTime[byNvrChanNo - 1];
    UpdateData(FALSE);
}


/************************************************************************** 
 *  @fn void CDlgAlarmhostPointCfg::OnBnClickedBtnSaveAlarmNvrChanInfo()
 *  @brief  ���浥��NVRͨ���Ĵ����¼�����
 *  @return void
 *************************************************************************/
void CDlgAlarmhostPointCfg::OnBnClickedBtnSaveAlarmNvrChanInfo()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);

    BYTE byNvrChanNo = m_cmNvrChanNo.GetCurSel() + 1;
    m_byNVRChanType[byNvrChanNo - 1] = m_cmNvrChanEventType.GetCurSel();            //����NVRͨ���¼����͵�����
    m_byPicNum[byNvrChanNo - 1] = m_byLinkagePicCount;
    m_wRecTime[byNvrChanNo - 1] = (WORD)m_dwLinkageRecTime;

    BYTE byTriggerIndex = m_cmbTriggerNo.GetCurSel();
    BYTE byAlarmline1 = m_bAlarmLine1.GetCheck();
    BYTE byAlarmline2 = m_bAlarmLine2.GetCheck();
    BYTE byAlarmline3 = m_bAlarmLine3.GetCheck();
    BYTE byAlarmline4 = m_bAlarmLine4.GetCheck();
    BYTE byTriggerBreakDown = m_bTriggerBreakDown.GetCheck();

    m_byTriggerLinkageEnable[byTriggerIndex] = m_chkTriggerEnabled.GetCheck();
    m_byTriggerLinkageInfo[byTriggerIndex] = (byAlarmline1) | (byAlarmline2 << 1) | (byAlarmline3 << 2) | (byAlarmline4 << 3) | (byTriggerBreakDown << 4);
}


/************************************************************************** 
 *  @fn void CDlgAlarmhostPointCfg::OnBnClickedBtnGetChanLinkageCfg()
 *  @brief  ��ȡ����������Ϣ
 *  @return void
 *************************************************************************/
void CDlgAlarmhostPointCfg::OnBnClickedBtnGetChanLinkageCfg()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    char szLan[128] = { 0 };
    memset(&m_struAlarmLikageParam, 0, sizeof(NET_DVR_ALARM_LINKAGE_PARAM));
    NET_DVR_INPUT_INFO struChanInfo = { 0 };
    struChanInfo.dwSize = sizeof(NET_DVR_INPUT_INFO);
    struChanInfo.byChanType = m_cmChanType.GetCurSel() + 1;
    struChanInfo.dwChanNo = m_cmChanIndex.GetCurSel() + 1;
    struChanInfo.dwSubChanNo = m_cmSlotIndex.GetCurSel() + 1;
    struChanInfo.dwVariableNo = m_cmVarNo.GetItemData(m_cmVarNo.GetCurSel());
    struChanInfo.byLinkageIPCType = m_cmbLinkageIPCCfg.GetCurSel();
    DWORD dwStatusList = 0;
    if (!NET_DVR_GetDeviceConfig(m_lUserID, NET_DVR_GET_ALARM_LINKAGE_CFG, 1, &struChanInfo, sizeof(NET_DVR_INPUT_INFO), &dwStatusList, &m_struAlarmLikageParam, sizeof(NET_DVR_ALARM_LINKAGE_PARAM)))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_GET_ALARM_LINKAGE_CFG");
        g_StringLanType(szLan, "��ȡ������������ʧ��", "Get alarm linkage parameter fail");
        AfxMessageBox(szLan);
        return;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_GET_ALARM_LINKAGE_CFG");
    }

    //����NVR������Ϣ
    BYTE byRecEnable = 0;
    BYTE byPicEnable = 0;
    memset(&m_byNVRChanType, 0, MAX_LINKAGE_CHAN_NUM);
    memset(&m_byPicNum, 0, MAX_LINKAGE_CHAN_NUM);
    memset(&m_wRecTime, 0, MAX_LINKAGE_CHAN_NUM);
    memset(m_byTriggerLinkageInfo, 0, ALARMHOST_ALARMOUT_NUM);
    memset(m_byTriggerLinkageEnable, 0, ALARMHOST_ALARMOUT_NUM);
    for (int i = 0; i < MAX_LINKAGE_CHAN_NUM; i++)
    {
        if ((m_struAlarmLikageParam.wChanPic >> i) & 0x01)
        {
            byPicEnable = 1;
        }
        if ((m_struAlarmLikageParam.wChanRec >> i) & 0x01)
        {
            byRecEnable = 1;
        }

        if (byRecEnable == 0 && byPicEnable == 0)
        {
            m_byNVRChanType[i] = 0;                             //ͨ����Ч
            m_byPicNum[i] = 0;
            m_wRecTime[i] = 0;
        }
        else if (byRecEnable == 1 && byPicEnable == 0)
        {
            m_byNVRChanType[i] = 1;                             //ͨ��֧��¼��
            m_byPicNum[i] = 0;
            m_wRecTime[i] = m_struAlarmLikageParam.wRecTime[i];
        }
        else if (byRecEnable == 0 && byPicEnable == 1)
        {
            m_byNVRChanType[i] = 2;                             //ͨ��֧��ץͼ
            m_byPicNum[i] = m_struAlarmLikageParam.byPicNum[i];
            m_wRecTime[i] = 0;
        }
        else                             //byRecEnable == 0 && byPicEnable == 0
        {
            m_byNVRChanType[i] = 3;                             //ͨ��֧��ץͼ��¼��
            m_byPicNum[i] = m_struAlarmLikageParam.byPicNum[i];
            m_wRecTime[i] = m_struAlarmLikageParam.wRecTime[i];
        }
        byPicEnable = 0;
        byRecEnable = 0;
    }

    memcpy(m_byTriggerLinkageEnable, m_struAlarmLikageParam.byTriggerEnabled, ALARMHOST_ALARMOUT_NUM);
    memcpy(m_byTriggerLinkageInfo, m_struAlarmLikageParam.bySensorJointAlarmOut, ALARMHOST_ALARMOUT_NUM);

    m_cmNvrChanNo.SetCurSel(0);
    m_cmNvrChanEventType.SetCurSel(m_byNVRChanType[0]);
    m_byLinkagePicCount = m_byPicNum[0];
    m_dwLinkageRecTime = (DWORD)m_wRecTime[0];
    m_cmbTriggerNo.SetCurSel(0);

    UpdateData(FALSE);
}

/************************************************************************** 
 *  @fn void CDlgAlarmhostPointCfg::OnBnClickedBtnSetChanLinkageCfg()
 *  @brief  ��������������Ϣ
 *  @return void
 *************************************************************************/
void CDlgAlarmhostPointCfg::OnBnClickedBtnSetChanLinkageCfg()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);
    char szLan[128] = { 0 };
    memset(&m_struAlarmLikageParam, 0, sizeof(NET_DVR_ALARM_LINKAGE_PARAM));
    m_struAlarmLikageParam.dwSize = sizeof(NET_DVR_ALARM_LINKAGE_PARAM);

    //����NVR������Ϣ
    for (int i = 0; i < MAX_LINKAGE_CHAN_NUM; i++)
    {
        switch (m_byNVRChanType[i])
        {
        case 0:             //��Ч
            break;
        case 1:             //֧��¼��
            m_struAlarmLikageParam.wChanRec |= (1 << i);
            m_struAlarmLikageParam.wRecTime[i] = m_wRecTime[i];
            break;
        case 2:             //֧��ץͼ
            m_struAlarmLikageParam.wChanPic |= (1 << i);
            m_struAlarmLikageParam.byPicNum[i] = m_byPicNum[i];
            break;
        case 3:             //֧��¼���ץͼ
            m_struAlarmLikageParam.wChanPic |= (1 << i);
            m_struAlarmLikageParam.wChanRec |= (1 << i);
            m_struAlarmLikageParam.wRecTime[i] = m_wRecTime[i];
            m_struAlarmLikageParam.byPicNum[i] = m_byPicNum[i];
            break;
        default:
            break;
        }
    }

    //���ô�����������Ϣ
    memcpy(m_struAlarmLikageParam.byTriggerEnabled, m_byTriggerLinkageEnable, ALARMHOST_ALARMOUT_NUM);
    memcpy(m_struAlarmLikageParam.bySensorJointAlarmOut, m_byTriggerLinkageInfo, ALARMHOST_ALARMOUT_NUM);

    NET_DVR_INPUT_INFO struChanInfo = { 0 };
    struChanInfo.dwSize = sizeof(NET_DVR_INPUT_INFO);
    struChanInfo.byChanType = m_cmChanType.GetCurSel() + 1;
    struChanInfo.dwChanNo = m_cmChanIndex.GetCurSel() + 1;
    struChanInfo.dwSubChanNo = m_cmSlotIndex.GetCurSel() + 1;
    struChanInfo.dwVariableNo = m_cmVarNo.GetItemData(m_cmVarNo.GetCurSel());
    struChanInfo.byLinkageIPCType = m_cmbLinkageIPCCfg.GetCurSel();
    DWORD dwStatusList = 0;
    if (!NET_DVR_SetDeviceConfig(m_lUserID, NET_DVR_SET_ALARM_LINKAGE_CFG, 1, &struChanInfo, sizeof(NET_DVR_INPUT_INFO), &dwStatusList, &m_struAlarmLikageParam, sizeof(NET_DVR_ALARM_LINKAGE_PARAM)))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_SET_ALARM_LINKAGE_CFG");
        g_StringLanType(szLan, "���ñ�����������ʧ��", "Set alarm linkage parameter fail");
        AfxMessageBox(szLan);
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_SET_ALARM_LINKAGE_CFG");
    }
}





void CDlgAlarmhostPointCfg::OnCbnSelchangeComboTriggerNo()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    int iTriggerIndex = m_cmbTriggerNo.GetCurSel();
    BYTE byAlarmLine1 = m_byTriggerLinkageInfo[iTriggerIndex] & 0x01;          //bit0
    BYTE byAlarmLine2 = m_byTriggerLinkageInfo[iTriggerIndex] >> 1 & 0x01;          //bit1
    BYTE byAlarmLine3 = m_byTriggerLinkageInfo[iTriggerIndex] >> 2 & 0x01;          //bit2
    BYTE byAlarmLine4 = m_byTriggerLinkageInfo[iTriggerIndex] >> 3 & 0x01;          //bit3
    BYTE byTriggerBreakDown = m_byTriggerLinkageInfo[iTriggerIndex] >> 4 & 0x01;    //bit4

    m_chkTriggerEnabled.SetCheck(m_byTriggerLinkageEnable[iTriggerIndex]);

    m_bAlarmLine1.SetCheck(byAlarmLine1);
    m_bAlarmLine2.SetCheck(byAlarmLine2);
    m_bAlarmLine3.SetCheck(byAlarmLine3);
    m_bAlarmLine4.SetCheck(byAlarmLine4);
    m_bTriggerBreakDown.SetCheck(byTriggerBreakDown);
}


void CDlgAlarmhostPointCfg::OnBnClickedChkTriggerEnabled()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    if (m_chkTriggerEnabled.GetCheck() == TRUE)
    {
        GetDlgItem(IDC_CHK_ALARM_LINE1)->EnableWindow(TRUE);
        GetDlgItem(IDC_CHK_ALARM_LINE2)->EnableWindow(TRUE);
        GetDlgItem(IDC_CHK_ALARM_LINE3)->EnableWindow(TRUE);
        GetDlgItem(IDC_CHK_ALARM_LINE4)->EnableWindow(TRUE);
        GetDlgItem(IDC_CHK_BREAK_DOWN)->EnableWindow(TRUE);
    }
    else
    {
        GetDlgItem(IDC_CHK_ALARM_LINE1)->EnableWindow(FALSE);
        GetDlgItem(IDC_CHK_ALARM_LINE2)->EnableWindow(FALSE);
        GetDlgItem(IDC_CHK_ALARM_LINE3)->EnableWindow(FALSE);
        GetDlgItem(IDC_CHK_ALARM_LINE4)->EnableWindow(FALSE);
        GetDlgItem(IDC_CHK_BREAK_DOWN)->EnableWindow(FALSE);
    }
}
