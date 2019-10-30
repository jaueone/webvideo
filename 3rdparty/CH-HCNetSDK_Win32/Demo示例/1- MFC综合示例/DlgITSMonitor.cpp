// DlgITSMonitor.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgITSMonitor.h"
#include "DlgDoTest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgITSMonitor dialog


CDlgITSMonitor::CDlgITSMonitor(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgITSMonitor::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgITSMonitor)
	m_fMaxCur = 0.0f;
	m_fMaxHum = 0.0f;
	m_fMaxTmp = 0.0f;
	m_fMaxVol = 0.0f;
	m_fMinCur = 0.0f;
	m_fMinHum = 0.0f;
	m_fMinTmp = 0.0f;
	m_fMinVol = 0.0f;
	m_startDate = COleDateTime::GetCurrentTime();
	m_startTime = COleDateTime::GetCurrentTime();
	m_stopDate = COleDateTime::GetCurrentTime();
	m_stopTime = COleDateTime::GetCurrentTime();
	//}}AFX_DATA_INIT


    
    m_lServerID = -1;
    m_iDevIndex = -1;
	m_lChannel = -1;
    m_bGetNext = FALSE;
    m_lHandle = -1;;
	
    memset(&m_struSensorCond, 0, sizeof(m_struSensorCond));
    memset(&m_struSensorCfg, 0, sizeof(m_struSensorCfg));
    memset(&m_szStatusBuf, 0, sizeof(ISAPI_STATUS_LEN));
}

CDlgITSMonitor::~CDlgITSMonitor()
{
    char szLan[128] = {0};
    if (m_lHandle >= 0)
    {
        if (!NET_DVR_StopRemoteConfig(m_lHandle))
        {
            m_bGetNext = FALSE;
            g_StringLanType(szLan, "���������ݲ�ѯֹͣʧ��", "Sensor Result List Stop Failed");
            AfxMessageBox(szLan);
            //return;
        }
	} 
}

void CDlgITSMonitor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgITSMonitor)
	DDX_Control(pDX, IDC_COMBO_SWITCH, m_comSwitch);
	DDX_Control(pDX, IDC_COMBO_DEVICE_TYPE2, m_comDeviceType2);
	DDX_Control(pDX, IDC_COMBO_DEVICE_ID2, m_comDeviceID2);
	DDX_Control(pDX, IDC_LIST_SENSOR, m_lstSensor);
	DDX_Control(pDX, IDC_COMBO_SWITCH_STATUS, m_comSwitchStatus);
	DDX_Control(pDX, IDC_COMBO_SENSOR_STATUS, m_comSensorStatus);
	DDX_Control(pDX, IDC_COMBO_QUERY_TYPE, m_comQueryType);
	DDX_Control(pDX, IDC_COMBO_DEVICE_TYPE, m_comDeviceType);
	DDX_Control(pDX, IDC_COMBO_DEVICE_ID, m_comDeviceID);
	DDX_Text(pDX, IDC_EDIT_MAX_CUR, m_fMaxCur);
	DDX_Text(pDX, IDC_EDIT_MAX_HUM, m_fMaxHum);
	DDX_Text(pDX, IDC_EDIT_MAX_TMP, m_fMaxTmp);
	DDX_Text(pDX, IDC_EDIT_MAX_VOL, m_fMaxVol);
	DDX_Text(pDX, IDC_EDIT_MIN_CUR, m_fMinCur);
	DDX_Text(pDX, IDC_EDIT_MIN_HUM, m_fMinHum);
	DDX_Text(pDX, IDC_EDIT_MIN_TMP, m_fMinTmp);
	DDX_Text(pDX, IDC_EDIT_MIN_VOL, m_fMinVol);
	DDX_DateTimeCtrl(pDX, IDC_START_DATE, m_startDate);
	DDX_DateTimeCtrl(pDX, IDC_START_TIME, m_startTime);
	DDX_DateTimeCtrl(pDX, IDC_STOP_DATE, m_stopDate);
	DDX_DateTimeCtrl(pDX, IDC_STOP_TIME, m_stopTime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgITSMonitor, CDialog)
	//{{AFX_MSG_MAP(CDlgITSMonitor)
	ON_BN_CLICKED(IDC_BTN_GET_SENSOR_CFG, OnBtnGetSensorCfg)
	ON_CBN_SELCHANGE(IDC_COMBO_QUERY_TYPE, OnSelchangeComboQueryType)
	ON_CBN_SELCHANGE(IDC_COMBO_DEVICE_TYPE, OnSelchangeComboDeviceType)
	ON_BN_CLICKED(IDC_BTN_SENSOR_SWITCH, OnBtnSensorSwitch)
	ON_CBN_SELCHANGE(IDC_COMBO_DEVICE_TYPE2, OnSelchangeComboDeviceType2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgITSMonitor message handlers

BOOL CDlgITSMonitor::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    char szLanTemp[128] = {0};
    m_lstSensor.SetExtendedStyle(m_lstSensor.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_SUBITEMIMAGES);
    g_StringLanType(szLanTemp, "ʹ��", "Enable");
    m_lstSensor.InsertColumn(0, szLanTemp, LVCFMT_LEFT, 60, -1);
    g_StringLanType(szLanTemp, "�豸����", "Device Type");
    m_lstSensor.InsertColumn(1, szLanTemp, LVCFMT_LEFT, 60, -1);
    g_StringLanType(szLanTemp, "�豸ID", "Device ID");
    m_lstSensor.InsertColumn(2, szLanTemp, LVCFMT_LEFT, 60, -1);
    g_StringLanType(szLanTemp, "ʱ��", "Time");
    m_lstSensor.InsertColumn(3, szLanTemp, LVCFMT_LEFT, 60, -1);
    g_StringLanType(szLanTemp, "������һ������", "Is First");
    m_lstSensor.InsertColumn(4, szLanTemp, LVCFMT_LEFT, 60, -1);
    g_StringLanType(szLanTemp, "�Ƿ�ͨ��", "Is Switch On");
    m_lstSensor.InsertColumn(5, szLanTemp, LVCFMT_LEFT, 60, -1);
    g_StringLanType(szLanTemp, "������״̬", "Sensor Status");
    m_lstSensor.InsertColumn(6, szLanTemp, LVCFMT_LEFT, 60, -1);
    g_StringLanType(szLanTemp, "����������", "Sensor Type");
    m_lstSensor.InsertColumn(7, szLanTemp, LVCFMT_LEFT, 60, -1);
    g_StringLanType(szLanTemp, "��ѹ", "Voltage");
    m_lstSensor.InsertColumn(8, szLanTemp, LVCFMT_LEFT, 60, -1);
    g_StringLanType(szLanTemp, "����", "Current");
    m_lstSensor.InsertColumn(9, szLanTemp, LVCFMT_LEFT, 60, -1);
    g_StringLanType(szLanTemp, "�¶�", "Temperature");
    m_lstSensor.InsertColumn(10, szLanTemp, LVCFMT_LEFT, 60, -1);
    g_StringLanType(szLanTemp, "ʪ��", "Humidity");
    m_lstSensor.InsertColumn(11, szLanTemp, LVCFMT_LEFT, 60, -1);
    g_StringLanType(szLanTemp, "����������", "Sensor Name");
    m_lstSensor.InsertColumn(12, szLanTemp, LVCFMT_LEFT, 60, -1);
    g_StringLanType(szLanTemp, "����������", "Sensor Description");
    m_lstSensor.InsertColumn(13, szLanTemp, LVCFMT_LEFT, 60, -1);
    g_StringLanType(szLanTemp, "����ֵ", "Sensor Actually Value");
    m_lstSensor.InsertColumn(14, szLanTemp, LVCFMT_LEFT, 60, -1);
    g_StringLanType(szLanTemp, "��λ", "Sensor Unit");
    m_lstSensor.InsertColumn(15, szLanTemp, LVCFMT_LEFT, 60, -1);

    m_comQueryType.SetCurSel(0);
    OnSelchangeComboQueryType();

    m_comDeviceType.SetCurSel(0);
    OnSelchangeComboDeviceType();

    m_comDeviceType2.SetCurSel(0);
    OnSelchangeComboDeviceType2();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgITSMonitor::AddSensorResultLst(LPNET_DVR_SENSOR_CFG lpInter)
{
    int iItemCount = m_lstSensor.GetItemCount();
    m_lstSensor.InsertItem(iItemCount, "");
    
    char szStr[128] = {0};


    //ʹ��
    memset(szStr, 0, sizeof(szStr));
    if (0 == lpInter->byEnable)
    {
        sprintf(szStr, "�ر�");
    } 
    else
    {
        sprintf(szStr, "����");
    } 
    m_lstSensor.SetItemText(iItemCount, 0, szStr);

    //�豸����
    memset(szStr, 0, sizeof(szStr));
    if (0 == lpInter->byDeviceType)
    {
        sprintf(szStr, "δ֪");
    } 
    else if (1 == lpInter->byDeviceType)
    {
        sprintf(szStr, "ģ����������");
    } 
    else if (2 == lpInter->byDeviceType)
    {
        sprintf(szStr, "�ϵ����");
    } 
    else if (3 == lpInter->byDeviceType)
    {
        sprintf(szStr, "�ϵ����");
    } 
    m_lstSensor.SetItemText(iItemCount, 1, szStr);

    //�豸id
    memset(szStr, 0, sizeof(szStr));
    sprintf(szStr, "%d", lpInter->byDeviceID);
    m_lstSensor.SetItemText(iItemCount, 2, szStr);
     
    //ʱ��
    memset(szStr, 0, sizeof(szStr));
    sprintf(szStr, "%4.4d%2.2d%2.2d%2.2d%2.2d%2.2d%3.3d", lpInter->struTime.wYear, lpInter->struTime.byMonth, \
        lpInter->struTime.byDay, lpInter->struTime.byHour, lpInter->struTime.byMinute,lpInter->struTime.bySecond,\
        lpInter->struTime.wMilliSec);
    m_lstSensor.SetItemText(iItemCount, 3, szStr);
    
    //�Ƿ����������һ������(����Ƿ�������)
    memset(szStr, 0, sizeof(szStr));
    if (0 == lpInter->byIsFirst)
    {
        sprintf(szStr, "��");
    } 
    else
    {
        sprintf(szStr, "��");
    } 
    m_lstSensor.SetItemText(iItemCount, 4, szStr);

    //�Ƿ�ͨ��
    memset(szStr, 0, sizeof(szStr));
    if (0 == lpInter->byIsSwitchOn)
    {
        sprintf(szStr, "��");
    } 
    else
    {
        sprintf(szStr, "��");
    } 
    m_lstSensor.SetItemText(iItemCount, 5, szStr);

    //������״̬
    memset(szStr, 0, sizeof(szStr));
    if (0 == lpInter->bySensorStatus)
    {
        sprintf(szStr, "����");
    } 
    else if (1 == lpInter->bySensorStatus)
    {
        sprintf(szStr, "�쳣");
    } 
    else if (2 == lpInter->bySensorStatus)
    {
        sprintf(szStr, "����");
    } 
    m_lstSensor.SetItemText(iItemCount, 6, szStr);

    //����������
    memset(szStr, 0, sizeof(szStr));
    if (0 == lpInter->bySensorType)
    {
        sprintf(szStr, "δ֪");
    } 
    else if (1 == lpInter->bySensorType)
    {
        sprintf(szStr, "�¶�");
    } 
    else if (2 == lpInter->bySensorType)
    {
        sprintf(szStr, "ʪ��");
    }
    else if (3 == lpInter->bySensorType)
    {
        sprintf(szStr, "����");
    }
    else if (4 == lpInter->bySensorType)
    {
        sprintf(szStr, "����");
    }
    else if (5 == lpInter->bySensorType)
    {
        sprintf(szStr, "������ѹ");
    }
    else if (6 == lpInter->bySensorType)
    {
        sprintf(szStr, "��������");
    }
    else if (7 == lpInter->bySensorType)
    {
        sprintf(szStr, "ֱ����ѹ");
    }
    else if (8 == lpInter->bySensorType)
    {
        sprintf(szStr, "ֱ������");
    }
    else if (9 == lpInter->bySensorType)
    {
        sprintf(szStr, "ˮѹ");
    }
    else if (10 == lpInter->bySensorType)
    {
        sprintf(szStr, "ѹ��������");
    }
    else if (11 == lpInter->bySensorType)
    {
        sprintf(szStr, "����������");
    }
    else if (12 == lpInter->bySensorType)
    {
        sprintf(szStr, "©ˮ������");
    }
    else if (13 == lpInter->bySensorType)
    {
        sprintf(szStr, "һ�廯�±�");
    }
    else if (14 == lpInter->bySensorType)
    {
        sprintf(szStr, "����ʽ�±�");
    }
    else if (15 == lpInter->bySensorType)
    {
        sprintf(szStr, "����������");
    }
    else if (16 == lpInter->bySensorType)
    {
        sprintf(szStr, "AC����");
    }
    else
    {
        sprintf(szStr, "δ����");
    }
    m_lstSensor.SetItemText(iItemCount, 7, szStr);

    //��ѹ
    memset(szStr, 0, sizeof(szStr));
    sprintf(szStr, "%f", lpInter->fVoltage);
    m_lstSensor.SetItemText(iItemCount, 8, szStr);

    //����
    memset(szStr, 0, sizeof(szStr));
    sprintf(szStr, "%f", lpInter->fCurrent);
    m_lstSensor.SetItemText(iItemCount, 9, szStr);

    //�¶�ֵ
    memset(szStr, 0, sizeof(szStr));
    sprintf(szStr, "%f", lpInter->fTemperatureValue);
    m_lstSensor.SetItemText(iItemCount, 10, szStr);

    //ʪ��ֵ
    memset(szStr, 0, sizeof(szStr));
    sprintf(szStr, "%f", lpInter->fHumidityValue);
    m_lstSensor.SetItemText(iItemCount, 11, szStr);

    //����������
    memset(szStr, 0, sizeof(szStr));
    if (g_struDeviceInfo[m_iDevIndex].byCharaterEncodeType == ENUM_MEM_CHAR_ENCODE_UNICODE)
    {
        TranslateNetWcharToMutilChar(szStr, lpInter->bySensorName, sizeof(lpInter->bySensorName));
    }
    else
    {
        sprintf(szStr, "%s", lpInter->bySensorName);
    }
    m_lstSensor.SetItemText(iItemCount, 12, szStr);

    //���������ݵ�������Ϣ
    memset(szStr, 0, sizeof(szStr));
    if (g_struDeviceInfo[m_iDevIndex].byCharaterEncodeType == ENUM_MEM_CHAR_ENCODE_UNICODE)
    {
        TranslateNetWcharToMutilChar(szStr, lpInter->byDescription, sizeof(lpInter->byDescription));
    }
    else
    {
        sprintf(szStr, "%s", lpInter->byDescription);
    }
    m_lstSensor.SetItemText(iItemCount, 13, szStr);

    //��ȷֵ
    memset(szStr, 0, sizeof(szStr));
    sprintf(szStr, "%f", lpInter->fActualValue);
    m_lstSensor.SetItemText(iItemCount, 14, szStr);

    //��λ
    memset(szStr, 0, sizeof(szStr));
    if (g_struDeviceInfo[m_iDevIndex].byCharaterEncodeType == ENUM_MEM_CHAR_ENCODE_UNICODE)
    {
        TranslateNetWcharToMutilChar(szStr, lpInter->byUnit, sizeof(lpInter->byUnit));
    }
    else
    {
        sprintf(szStr, "%s", lpInter->byUnit);
    }
    m_lstSensor.SetItemText(iItemCount, 15, szStr);
}

DWORD WINAPI CDlgITSMonitor::GetSensorResultThread(LPVOID lpDlg)
{
    CDlgITSMonitor* pThis = reinterpret_cast<CDlgITSMonitor*>(lpDlg);
    int iRet = 0;
    char szLan[128] = {0};
    char szInfoBuf[128] = {0};
    while (pThis->m_bGetNext)
    {
        iRet = NET_DVR_GetNextRemoteConfig(pThis->m_lHandle, &pThis->m_struSensorCfg, sizeof(NET_DVR_SENSOR_CFG)); 
        if (iRet == NET_SDK_GET_NEXT_STATUS_SUCCESS)
        {
            pThis->AddSensorResultLst(&pThis->m_struSensorCfg);
        }
        else
        {
            if (iRet == NET_SDK_GET_NETX_STATUS_NEED_WAIT)
            {
                //g_StringLanType(szLan, "���ڲ�ѯ!", "getting....");
                g_pMainDlg->AddLog(0, OPERATION_SUCC_T, "���ڲ�ѯ!");
                Sleep(200);
                continue;
            }
            if (iRet == NET_SDK_GET_NEXT_STATUS_FINISH)
            {
                g_StringLanType(szLan, "���������ݲ�ѯ����!", "Get Sensor Result List Finish");
                sprintf(szInfoBuf, "%s[Info Count:%d]", szLan, pThis->m_lstSensor.GetItemCount());
                AfxMessageBox(szInfoBuf);
                break;
            }
            else if(iRet == NET_SDK_GET_NEXT_STATUS_FAILED)
            {
                g_StringLanType(szLan, "�����Ӵ��������ݲ�ѯʧ��", "Get Sensor Result List failed");
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
    return 0;
}

void CDlgITSMonitor::OnBtnGetSensorCfg() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
    memset(&m_struSensorCond, 0, sizeof(m_struSensorCond));
    m_struSensorCond.dwSize = sizeof(m_struSensorCond);
    m_struSensorCond.byQueryType = m_comQueryType.GetCurSel();

    if (4 == m_comDeviceType.GetCurSel())
    {
        m_struSensorCond.byDeviceType = 0xff;
    } 
    else
    {
        m_struSensorCond.byDeviceType = m_comDeviceType.GetCurSel();
    }

    if (4 != m_comDeviceType.GetCurSel())
    {
        if ((8 == m_comDeviceID.GetCurSel() && 1 == m_comDeviceType.GetCurSel())\
            || (4 == m_comDeviceID.GetCurSel() && 2 == m_comDeviceType.GetCurSel())\
            || (2 == m_comDeviceID.GetCurSel() && 3 == m_comDeviceType.GetCurSel()))
        {
            m_struSensorCond.byDeviceID = 0xff;
        } 
        else
        {
            m_struSensorCond.byDeviceID = m_comDeviceID.GetCurSel()+1;
        }

    }

    m_struSensorCond.struHistoricalQueryParam.struStartTime.wYear = m_startDate.GetYear();
    m_struSensorCond.struHistoricalQueryParam.struStartTime.byMonth = m_startDate.GetMonth();
    m_struSensorCond.struHistoricalQueryParam.struStartTime.byDay = m_startDate.GetDay();
    m_struSensorCond.struHistoricalQueryParam.struStartTime.byHour = m_startTime.GetHour();
    m_struSensorCond.struHistoricalQueryParam.struStartTime.byMinute = m_startTime.GetMinute();
    m_struSensorCond.struHistoricalQueryParam.struStartTime.bySecond = m_startTime.GetSecond();
  
    m_struSensorCond.struHistoricalQueryParam.struEndTime.wYear = m_stopDate.GetYear();
    m_struSensorCond.struHistoricalQueryParam.struEndTime.byMonth = m_stopDate.GetMonth();
    m_struSensorCond.struHistoricalQueryParam.struEndTime.byDay = m_stopDate.GetDay();
    m_struSensorCond.struHistoricalQueryParam.struEndTime.byHour = m_stopTime.GetHour();
    m_struSensorCond.struHistoricalQueryParam.struEndTime.byMinute = m_stopTime.GetMinute();
	m_struSensorCond.struHistoricalQueryParam.struEndTime.bySecond = m_stopTime.GetSecond();

    m_struSensorCond.struHistoricalQueryParam.struVoltageValue.fMaxValue = m_fMaxVol;
    m_struSensorCond.struHistoricalQueryParam.struVoltageValue.fMinValue = m_fMinVol;

    if (2 == m_comSwitchStatus.GetCurSel())
    {
        m_struSensorCond.struHistoricalQueryParam.bySwitchStatus = 0xff;
    } 
    else
    {
        m_struSensorCond.struHistoricalQueryParam.bySwitchStatus = m_comSwitchStatus.GetCurSel();
    }
    
    if (3 == m_comSensorStatus.GetCurSel())
    {
        m_struSensorCond.struHistoricalQueryParam.bySensorStatus = 0xff;
    } 
    else
    {
        m_struSensorCond.struHistoricalQueryParam.bySensorStatus = m_comSensorStatus.GetCurSel();
    }
    

    char szLan[128] = {0};
    if (m_lHandle >= 0)
    {
        if (!NET_DVR_StopRemoteConfig(m_lHandle))
        {
            m_bGetNext = FALSE;
            g_StringLanType(szLan, "���������ݲ�ѯֹͣʧ��", "Sensor Result List Stop Failed");
            AfxMessageBox(szLan);
            return;
        }
	} 
    
    m_lHandle = NET_DVR_StartRemoteConfig(m_lServerID, NET_DVR_GET_SENSOR_INFO, &m_struSensorCond, sizeof(m_struSensorCond), NULL, NULL);
    if (m_lHandle >= 0)
    {
        m_bGetNext = TRUE;
        DWORD dwThreadId;
        m_lstSensor.DeleteAllItems();
        m_hGetInfoThread = CreateThread(NULL, 0, LPTHREAD_START_ROUTINE(GetSensorResultThread), this, 0, &dwThreadId);
        g_StringLanType(szLan, "���������ݲ�ѯ�ɹ�", "Sensor Result List succ");
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, szLan);
    }
    else
    {
        m_bGetNext = FALSE;
        g_StringLanType(szLan, "���������ݲ�ѯʧ��", "Sensor Result List failed");
        AfxMessageBox(szLan);
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_GET_SENSOR_INFO");
        return;
    }
	
}

void CDlgITSMonitor::OnSelchangeComboQueryType() 
{
	// TODO: Add your control notification handler code here
	int i = m_comQueryType.GetCurSel();
    if (0 == i)
    {
        GetDlgItem(IDC_START_DATE)->EnableWindow(FALSE);
        GetDlgItem(IDC_START_TIME)->EnableWindow(FALSE);
        GetDlgItem(IDC_STOP_DATE)->EnableWindow(FALSE);
        GetDlgItem(IDC_STOP_TIME)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_MAX_VOL)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_MIN_VOL)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_MAX_CUR)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_MIN_CUR)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_MAX_TMP)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_MIN_TMP)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_MAX_HUM)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_MIN_HUM)->EnableWindow(FALSE);
        GetDlgItem(IDC_COMBO_SWITCH_STATUS)->EnableWindow(FALSE);
        GetDlgItem(IDC_COMBO_SENSOR_STATUS)->EnableWindow(FALSE);
    } 
    else if(1 == i)
    {
        GetDlgItem(IDC_START_DATE)->EnableWindow(TRUE);
        GetDlgItem(IDC_START_TIME)->EnableWindow(TRUE);
        GetDlgItem(IDC_STOP_DATE)->EnableWindow(TRUE);
        GetDlgItem(IDC_STOP_TIME)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDIT_MAX_VOL)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDIT_MIN_VOL)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDIT_MAX_CUR)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDIT_MIN_CUR)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDIT_MAX_TMP)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDIT_MIN_TMP)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDIT_MAX_HUM)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDIT_MIN_HUM)->EnableWindow(TRUE);
        GetDlgItem(IDC_COMBO_SWITCH_STATUS)->EnableWindow(TRUE);
        GetDlgItem(IDC_COMBO_SENSOR_STATUS)->EnableWindow(TRUE);
    }
}

void CDlgITSMonitor::OnSelchangeComboDeviceType() 
{
	// TODO: Add your control notification handler code here
    char szLan[128] = {0};
	int i = m_comDeviceType.GetCurSel();

    GetDlgItem(IDC_COMBO_DEVICE_ID)->EnableWindow(FALSE);

    if (1 == i)
    {
        m_comDeviceID.ResetContent();
        for (int i = 0; i < 8; i++)
        {
            sprintf(szLan, "%d", i + 1);
            m_comDeviceID.AddString(szLan);
        }
        sprintf(szLan, "����ID");
        m_comDeviceID.AddString(szLan);

        GetDlgItem(IDC_COMBO_DEVICE_ID)->EnableWindow(TRUE);
    } 
    else if(2 == i)
    {
        m_comDeviceID.ResetContent();
        for (int i = 0; i < 4; i++)
        {
            sprintf(szLan, "%d", i + 1);
            m_comDeviceID.AddString(szLan);
        }
        sprintf(szLan, "����ID");
        m_comDeviceID.AddString(szLan);

        GetDlgItem(IDC_COMBO_DEVICE_ID)->EnableWindow(TRUE);
    }
    else if(3 == i)
    {
        m_comDeviceID.ResetContent();
        for (int i = 0; i < 2; i++)
        {
            sprintf(szLan, "%d", i + 1);
            m_comDeviceID.AddString(szLan);
        }
        sprintf(szLan, "����ID");
        m_comDeviceID.AddString(szLan);

        GetDlgItem(IDC_COMBO_DEVICE_ID)->EnableWindow(TRUE);
    }

}

void CDlgITSMonitor::OnBtnSensorSwitch() 
{
	// TODO: Add your control notification handler code here
    NET_DVR_SENSOR_SWITCH_COND m_struSwitchCond = {0};
    m_struSwitchCond.dwSize = sizeof(NET_DVR_SENSOR_SWITCH_COND);
    m_struSwitchCond.byDeviceType = m_comDeviceType2.GetCurSel()+1;
    m_struSwitchCond.byDeviceID = m_comDeviceID2.GetCurSel()+1;
    m_struSwitchCond.bySwitch = m_comSwitch.GetCurSel();

    NET_DVR_STD_CONTROL struControl = {0};
    struControl.lpCondBuffer = &m_struSwitchCond;
    struControl.dwCondSize = sizeof(m_struSwitchCond);
    
    memset(m_szStatusBuf, 0, 1024);
    struControl.lpStatusBuffer = m_szStatusBuf;
    struControl.dwStatusSize = 1024;

    
    if(!NET_DVR_STDControl(m_lServerID, NET_DVR_SET_SENSOR_SWITCH, &struControl))
    {
        OutputDebugString(m_szStatusBuf);
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_SET_SENSOR_SWITCH");
    }
    else
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_SET_SENSOR_SWITCH");
    }
}

void CDlgITSMonitor::OnSelchangeComboDeviceType2() 
{
	// TODO: Add your control notification handler code here
    char szLan[128] = {0};
    int i = m_comDeviceType2.GetCurSel() + 1;
      
//     if (0 == i)
//     {
//         m_comDeviceID2.ResetContent();
//         for (int i = 0; i < 8; i++)
//         {
//             sprintf(szLan, "%d", i + 1);
//             m_comDeviceID2.AddString(szLan);
//         }
//     } 
    if(1 == i)
    {
        m_comDeviceID2.ResetContent();
        for (int i = 0; i < 4; i++)
        {
            sprintf(szLan, "%d", i + 1);
            m_comDeviceID2.AddString(szLan);
        }
    }
    else if(2 == i)
    {
        m_comDeviceID2.ResetContent();
        for (int i = 0; i < 2; i++)
        {
            sprintf(szLan, "%d", i + 1);
            m_comDeviceID2.AddString(szLan);
        }
    }
}
