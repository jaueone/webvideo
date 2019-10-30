// DlgProductDH.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ClientDemo.h"
#include "DlgProductDH.h"
#include "afxdialogex.h"
#include "DlgAlarmhostHistoryData.h"
#include "DlgAlarmhostPointCfg.h"
#include "DlgAlarmhostSensorLinkage.h"
#include "DlgScaleOutCfg.h"
#include "DlgAlarmhostDataUploadMode.h"
#include "DlgAlarmHostRs485.h"
#include "DlgAlarmHostExternalDevState.h"
#include "DlgAlarmHostLogSearch.h"
#include "DlgAlarmhostExternalDevLimitValue.h"
#include "DlgGetAllPointCfg.h"
#include "DlgGetAllSensor.h"
#include "DlgGetAllRs485Cfg.h"
#include "DlgGetAllRs485Slot.h"
#include "DlgDeviceSelfCheckState.h"
#include "DlgRS485ProtocolVersion.h"
#include "DlgAlarmHostZoneCfg.h"
#include "DlgAlarmOutCfg.h"
#include "DlgAlarmHostEnableCfg.h"
#include "DlgAlarmHostAbility.h"
#include "DlgAlarmNetUser.h"
#include "DlgAlarmOperatorUser.h"
#include "DlgAlarmHostStatus.h"
#include "DlgRS485DLLParamCfg.h"
// CDlgProductDH �Ի���

IMPLEMENT_DYNAMIC(CDlgProductDH, CDialog)

CDlgProductDH::CDlgProductDH(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgProductDH::IDD, pParent)
{
    m_iDevIndex = -1;
    m_lServerID = -1;
    m_lStartChan = -1;
    m_lChannel = -1;
    m_bInit = FALSE;
    memset(&m_struAlarmHostAbility, 0, sizeof(m_struAlarmHostAbility));
}

CDlgProductDH::~CDlgProductDH()
{
  
}

void CDlgProductDH::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgProductDH, CDialog)
    
    ON_BN_CLICKED(IDC_BTN_HISTORY_DATA, &CDlgProductDH::OnBnClickedBtnHistoryData)
    ON_BN_CLICKED(IDC_BTN_POINT_CFG, &CDlgProductDH::OnBnClickedBtnPointCfg)
    ON_BN_CLICKED(IDC_BTN_SENSOR_LINKAGE, &CDlgProductDH::OnBnClickedBtnSensorLinkage)
    ON_BN_CLICKED(IDC_BTN_SCALE_OUT_CFG, &CDlgProductDH::OnBnClickedBtnScaleOutCfg)
    ON_BN_CLICKED(IDC_BTN_UPLOAD_MODE, &CDlgProductDH::OnBnClickedBtnUploadMode)
    ON_BN_CLICKED(IDC_BTN_RS485_CFG, &CDlgProductDH::OnBnClickedBtnRs485Cfg)
    ON_BN_CLICKED(IDC_BTN_EXTERNAL_DEVICE_STATE, &CDlgProductDH::OnBnClickedBtnExternalDeviceState)
    ON_BN_CLICKED(IDC_BUT_NETALARM_ALARM_LOG_SEARCH, &CDlgProductDH::OnBnClickedButNetalarmAlarmLogSearch)
    ON_BN_CLICKED(IDC_BTN_SENSOR_CFG, &CDlgProductDH::OnBnClickedBtnSensorCfg)
    ON_BN_CLICKED(IDC_BTN_EXTERNAL_DEVICE_LIMIT_VALUE, &CDlgProductDH::OnBnClickedBtnExternalDeviceLimitValue)
    ON_BN_CLICKED(IDC_BTN_GET_ALL_POINT, &CDlgProductDH::OnBnClickedBtnGetAllPoint)
    ON_BN_CLICKED(IDC_BTN_GET_ALL_SENSOR, &CDlgProductDH::OnBnClickedBtnGetAllSensor)
    ON_BN_CLICKED(IDC_BTN_GET_ALL_RS485_CFG, &CDlgProductDH::OnBnClickedBtnGetAllRs485Cfg)
    ON_BN_CLICKED(IDC_BTN_GET_ALL_RS485_SLOT_CFG, &CDlgProductDH::OnBnClickedBtnGetAllRs485SlotCfg)
    ON_BN_CLICKED(IDC_BTN_GET_DEVICE_STATE, &CDlgProductDH::OnBnClickedBtnGetDeviceState)
    ON_BN_CLICKED(IDC_BTN_RS485_VERSION, &CDlgProductDH::OnBnClickedBtnRs485Version)
    ON_BN_CLICKED(IDC_BTN_ZONE_CFG, &CDlgProductDH::OnBnClickedBtnZoneCfg)
    ON_BN_CLICKED(IDC_BTN_OUTPUT_CFG, &CDlgProductDH::OnBnClickedBtnOutputCfg)
    ON_BN_CLICKED(IDC_BTN_ENABLE_CFG, &CDlgProductDH::OnBnClickedBtnEnableCfg)
    ON_BN_CLICKED(IDC_BTN_ABILITY_GET, &CDlgProductDH::OnBnClickedBtnAbilityGet)
    ON_BN_CLICKED(IDC_BTN_NETUSER_CFG, &CDlgProductDH::OnBnClickedBtnNetuserCfg)
    ON_BN_CLICKED(IDC_BTN_KEYPADUSER_CFG, &CDlgProductDH::OnBnClickedBtnKeypaduserCfg)
    ON_BN_CLICKED(IDC_BTN_STATUS_CHECK, &CDlgProductDH::OnBnClickedBtnStatusCheck)
    ON_BN_CLICKED(IDC_BTN_VOLTAGE_CHECK, &CDlgProductDH::OnBnClickedBtnVoltageCheck)
    ON_BN_CLICKED(IDC_BTN_RS485_DLL_CFG, &CDlgProductDH::OnBnClickedBtnRs485DllCfg)
    ON_BN_CLICKED(IDC_BTN_TEST_OTHER, &CDlgProductDH::OnBnClickedBtnTestOther)
END_MESSAGE_MAP()


// CDlgProductDH ��Ϣ�������

void CDlgProductDH::UpdataParam()
{
    int iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
    if (iDeviceIndex < 0)
    {
        return;
    }

    int iChanIndex = g_pMainDlg->GetCurChanIndex();
    if (iChanIndex < 0)
    {
        iChanIndex = 0;//default to config channel one
    }
    m_iDevIndex = iDeviceIndex;
    m_lServerID = g_struDeviceInfo[iDeviceIndex].lLoginID;
    m_lStartChan = g_struDeviceInfo[iDeviceIndex].iStartChan;
    m_lChannel = iChanIndex + g_struDeviceInfo[iDeviceIndex].iStartChan;

    if (!m_bInit)
    {
        if (TRUE == GetAlarmHostAbility(m_struAlarmHostAbility))
        {
            m_bInit = TRUE;
        }
    }
    UpdateData(FALSE);
}

BOOL CDlgProductDH::GetAlarmHostAbility(NET_DVR_ALARMHOST_ABILITY& struAlarmHostAbility)
{
    BOOL bRet = FALSE;
    DWORD dwReturn = 0;
    if (NET_DVR_GetDeviceAbility(m_lServerID, ALARMHOST_ABILITY, NULL, 0, (char*)&struAlarmHostAbility, sizeof(struAlarmHostAbility)))
    {
        char szLan[128] = { 0 };
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "ALARMHOST_ABILITY");
        bRet = TRUE;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "ALARMHOST_ABILITY");
        bRet = FALSE;
    }
    return bRet;
}


void CDlgProductDH::OnBnClickedBtnHistoryData()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CDlgAlarmhostHistoryData dlg;
    dlg.DoModal();
}


void CDlgProductDH::OnBnClickedBtnPointCfg()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CDlgAlarmhostPointCfg dlg;
    dlg.DoModal();
}

void CDlgProductDH::OnBnClickedBtnSensorLinkage()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CDlgAlarmhostSensorLinkage dlg;
    dlg.DoModal();
}




void CDlgProductDH::OnBnClickedBtnScaleOutCfg()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CDlgScaleOutCfg dlg;
    dlg.DoModal();
}


void CDlgProductDH::OnBnClickedBtnUploadMode()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CDlgAlarmhostDataUploadMode dlg;
    dlg.DoModal();
}




void CDlgProductDH::OnBnClickedBtnRs485Cfg()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CDlgAlarmHostRs485 dlg;
    dlg.m_lServerID = m_lServerID;
    dlg.m_iDevIndex = m_iDevIndex;
    memcpy(&dlg.m_struDevAbility, &m_struAlarmHostAbility, sizeof(m_struAlarmHostAbility));
    dlg.DoModal();
}


void CDlgProductDH::OnBnClickedBtnExternalDeviceState()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CDlgAlarmHostExternalDevState dlg;
    dlg.DoModal();
}


void CDlgProductDH::OnBnClickedButNetalarmAlarmLogSearch()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    int iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
    if (iDeviceIndex == -1)
    {
        return;
    }
    CDlgAlarmHostLogSearch dlg;
    dlg.m_lServerID = g_struDeviceInfo[iDeviceIndex].lLoginID;
    dlg.m_iDevIndex = iDeviceIndex;
    dlg.DoModal();
}


void CDlgProductDH::OnBnClickedBtnSensorCfg()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CDlgSensorCfg dlg;
    dlg.m_lServerID = m_lServerID;
    dlg.m_iDevIndex = m_iDevIndex;
    dlg.m_lChanNum = g_struDeviceInfo[m_iDevIndex].iAnalogChanNum;
    memcpy(&dlg.m_struDevAbility, &m_struAlarmHostAbility, sizeof(m_struAlarmHostAbility));
    dlg.DoModal();
}


void CDlgProductDH::OnBnClickedBtnExternalDeviceLimitValue()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CDlgAlarmhostExternalDevLimitValue dlg;
    dlg.DoModal();
}


void CDlgProductDH::OnBnClickedBtnGetAllPoint()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    DlgGetAllPointCfg dlg;
    dlg.DoModal();
}


void CDlgProductDH::OnBnClickedBtnGetAllSensor()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    DlgGetAllSensor dlg;
    dlg.DoModal();
}


void CDlgProductDH::OnBnClickedBtnGetAllRs485Cfg()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    DlgGetAllRs485Cfg dlg;
    dlg.DoModal();
}


void CDlgProductDH::OnBnClickedBtnGetAllRs485SlotCfg()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    DlgGetAllRs485Slot dlg;
    dlg.DoModal();
}


void CDlgProductDH::OnBnClickedBtnGetDeviceState()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    DlgDeviceSelfCheckState dlg;
    dlg.DoModal();
}

void CDlgProductDH::OnBnClickedBtnRs485Version()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CDlgRS485ProtocolVersion dlg;
    dlg.m_lUserID = m_lServerID;
    dlg.DoModal();
}


void CDlgProductDH::OnBnClickedBtnZoneCfg()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CDlgAlarmHostZoneCfg dlg;
    dlg.DoModal();
}


void CDlgProductDH::OnBnClickedBtnOutputCfg()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CDlgAlarmOutCfg dlg;
    dlg.m_lServerID = m_lServerID;
    dlg.m_iDevIndex = m_iDevIndex;
    memcpy(&dlg.m_struAbility, &m_struAlarmHostAbility, sizeof(m_struAlarmHostAbility));

    dlg.DoModal();
}


void CDlgProductDH::OnBnClickedBtnEnableCfg()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CDlgAlarmHostEnableCfg dlg;
    dlg.m_lServerID = m_lServerID;
    dlg.m_iDevIndex = m_iDevIndex;
    memcpy(&dlg.m_struAbility, &m_struAlarmHostAbility, sizeof(m_struAlarmHostAbility));
    dlg.DoModal();
}


void CDlgProductDH::OnBnClickedBtnAbilityGet()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CDlgAlarmHostAbility dlg;
    memcpy(&dlg.m_struAlarmHostAbility, &m_struAlarmHostAbility, sizeof(m_struAlarmHostAbility));
    dlg.DoModal();
}


void CDlgProductDH::OnBnClickedBtnNetuserCfg()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CDlgAlarmNetUser dlg;
    dlg.m_lLoginID = m_lServerID;
    dlg.m_iDevIndex = m_iDevIndex;
    dlg.m_dwNetUserNum = m_struAlarmHostAbility.wNetUserNum;
    dlg.DoModal();
}


void CDlgProductDH::OnBnClickedBtnKeypaduserCfg()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CDlgAlarmOperatorUser dlg;
    dlg.m_lServerID = m_lServerID;
    dlg.m_iDevIndex = m_iDevIndex;
    dlg.m_dwOperatorUserNum = m_struAlarmHostAbility.wOperatorUserNum;
    dlg.m_dwAlarmInNum = m_struAlarmHostAbility.wLocalAlarmInNum + m_struAlarmHostAbility.wExpandAlarmInNum;
    dlg.m_iSubSystemNum = m_struAlarmHostAbility.wSubSystemNum;
    dlg.m_dwKeyboardUserNum = m_struAlarmHostAbility.wKeyboardNum;
    dlg.DoModal();
}


void CDlgProductDH::OnBnClickedBtnStatusCheck()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CDlgAlarmHostStatus dlg;
    memcpy(&dlg.m_struAlarmHostAbility, &m_struAlarmHostAbility, sizeof(m_struAlarmHostAbility));
    dlg.m_iDevIndex = m_iDevIndex;
    dlg.m_lServerID = m_lServerID;
    dlg.m_lStartChan = m_lStartChan;
    dlg.m_lChannel = m_lChannel;
    dlg.DoModal();
}

BOOL CDlgProductDH::GetBatteryVoltage()
{
    if (NET_DVR_GetBatteryVoltage(m_lServerID, &m_fBatteryVoltage))
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_GetBatteryVoltage");
        return TRUE;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_GetBatteryVoltage");
        return FALSE;
    }
}

void CDlgProductDH::OnBnClickedBtnVoltageCheck()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    char szLan[128] = { 0 };
    if (GetBatteryVoltage())
    {
        sprintf(szLan, "Battery voltage %0.3f", m_fBatteryVoltage);
        AfxMessageBox(szLan);
    }
}


void CDlgProductDH::OnBnClickedBtnRs485DllCfg()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CDlgRS485DLLParamCfg dlg;
    dlg.m_iUserID = m_lServerID;
    dlg.m_iDeviceIndex = m_iDevIndex;
    dlg.DoModal();
}


void CDlgProductDH::OnBnClickedBtnTestOther()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
   /* NET_DVR_WALLOUTPUTPARAM struWallOutputParam = { 0 };
    DWORD dwInbuff = 4;
    DWORD dwStatus = 0;


    int retValue = NET_DVR_GetDeviceConfig(m_lServerID, NET_DVR_WALLOUTPUT_GET, 1, &dwInbuff, -1, &dwStatus, &struWallOutputParam, sizeof(NET_DVR_WALLOUTPUTPARAM));
    if (!retValue)
    {
        g_pMainDlg->AddLog(m_lServerID, OPERATION_FAIL_T, "NET_DVR_GetDeviceConfig get capability error!!");
    }
    else
    {
        g_pMainDlg->AddLog(m_lServerID, OPERATION_SUCC_T, "NET_DVR_GetDeviceConfig get capability suss");
    }*/
    
    //NET_DVR_DEVLIST struDevList = { 0 };
    //struDevList.dwSize = sizeof(struDevList);
    //struDevList.pBuffer = NULL;
    //struDevList.dwBufLen = 0;
    //int retError = 0;

    //int retValue = NET_DVR_GetDevList(m_lServerID, &struDevList);
    //
    //if (retValue)
    //{
    //    if (struDevList.dwDevNums == 0 || struDevList.dwDevNums > 512)
    //    {
    //        return;
    //    }
    //    LPNET_DVR_SINGLE_DEV_INFO lpStruDevInfo = new NET_DVR_SINGLE_DEV_INFO[struDevList.dwDevNums];
    //    lpStruDevInfo->dwSize = sizeof(*lpStruDevInfo);
    //    struDevList.pBuffer = (BYTE *)lpStruDevInfo;
    //    struDevList.dwBufLen = sizeof(NET_DVR_SINGLE_DEV_INFO)*struDevList.dwDevNums;
    //    BOOL BRet = FALSE;
    //    //��ȡ�豸��Ϣ
    //    if (!NET_DVR_GetDevList(m_lServerID, &struDevList))
    //    {
    //        g_pMainDlg->AddLog(m_lServerID, OPERATION_FAIL_T, "NET_DVR_GetDevList get device info");

    //    }
    //    else
    //    {
    //        g_pMainDlg->AddLog(m_lServerID, OPERATION_SUCC_T, "NET_DVR_GetDevList get device info");
    //        BRet = TRUE;
    //    }
    //    return;
    //}
    //else
    //{
    //    retError = NET_DVR_GetLastError();
    //}

    /*NET_VCA_CHAN_IN_PARAM strVcaInputBuff = { 0 };
    DWORD dwInbuffLen = 2147483648;
    NET_VCA_BEHAVIOR_ABILITY strVcaOutputBuff = { 0 };
    strVcaOutputBuff.dwSize = sizeof(NET_VCA_BEHAVIOR_ABILITY);
    DWORD dwOutputBuffLen = sizeof(NET_VCA_BEHAVIOR_ABILITY);

    if (!NET_DVR_GetDeviceAbility(m_lServerID, VCA_CHAN_ABILITY, (char*)&strVcaInputBuff, dwInbuffLen, (char*)&strVcaOutputBuff, dwOutputBuffLen))
    {
        g_pMainDlg->AddLog(m_lServerID, OPERATION_FAIL_T, "NET_DVR_GetDeviceAbility get capability error!!");
    }
    else
    {
        g_pMainDlg->AddLog(m_lServerID, OPERATION_SUCC_T, "NET_DVR_GetDeviceAbility get capability suss");
    }*/

    //NET_DVR_UPGRADE_FIRMWARE_INFO strUpgradeFirmware = { 0 };
    //DWORD iRet = 0;
    //if (!NET_DVR_GetDVRConfig(m_lServerID, NET_DVR_GET_UPGRADE_INFO, 0, &strUpgradeFirmware, sizeof(NET_DVR_UPGRADE_FIRMWARE_INFO), &iRet))
    //{
    //    g_pMainDlg->AddLog(m_lServerID, OPERATION_FAIL_T, "NET_DVR_GetDVRConfig get capability error!!");
    //}
    //else
    //{
    //    g_pMainDlg->AddLog(m_lServerID, OPERATION_SUCC_T, "NET_DVR_GetDVRConfig get capability suss");
    //}
}
