#pragma once
#include "DlgXmlSub.h"
#include "afxwin.h"
// CAlarmAdvanceSetter 对话框

//上报信息xml的最大长度
#define MAX_SUBSCRIBE_DATA_LENGTH 10240

class CAlarmAdvanceSetter : public CDialogEx
{
    DECLARE_DYNAMIC(CAlarmAdvanceSetter)

public:
    CAlarmAdvanceSetter(CWnd* pParent = NULL);   // 标准构造函数
    virtual ~CAlarmAdvanceSetter();

// 对话框数据
    enum { IDD = IDD_DIALOG_ALARM_ADVANCESETTER };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()

public:
    virtual BOOL OnInitDialog();
    CComboBox m_cmbAlarmLevel;
    CComboBox m_cmbAlarmType;
    CComboBox m_cmbFaceDetectType;
    CComboBox m_cmbITSUploadType;
    BOOL m_bChePaiBroken;
    BOOL m_bKeLiuTongJiBroken;
    BOOL m_bHotMapBroken;
    BOOL m_bFaceSanpBroken;
    BOOL m_bFaceMatchBroken;
    CComboBox m_cmbFaceSnapDataType;
    CComboBox m_cmbFaceMatchDataType;
    CComboBox m_cmbEventJsonDataType;
    BOOL m_bCopilotFace;
    int m_iTaskNo;
    CComboBox m_cmbAlarmSubscribeInfo;
    BOOL m_bConfirmMechanism;
    
    //BOOL m_bIsSupportAlarmLevel;
    //BOOL m_bIsSupportITSUploadType;
    //BOOL m_bIsSupportAlarmTypeV40;
    //BOOL m_bIsSupportDevInfoVersion;
    BOOL m_bIsSupportFaceSnap;
    BOOL m_bIsSupportFaceDetection;
    //BOOL m_bIsSupportChePaiBroken;
    //BOOL m_bIsSupportKeLiuTongJiBroken;
    //BOOL m_bIsSupportHotMapBroken;
    //BOOL m_bIsSupportFaceSanpBroken;
    //BOOL m_bIsSupportFaceMatchBroken;
    
    BOOL m_bIsSupportBehaviorUploadByCloudStorageURL;



    afx_msg void OnCbnSelchangeComboAlarmSubscribeInfo();
    afx_msg void OnBnClickedButtonConfirm();
    
    void SetSubscribeItemsEnabled(BOOL enabled);

    NET_DVR_SETUPALARM_PARAM_V50 *m_pStruSetupAlarmParamV50;
    char *m_pData;// [MAX_SUBSCRIBE_DATA_LENGTH];
    void setAdvanceAlarmParam(NET_DVR_SETUPALARM_PARAM_V50* struSetupAlarmParamV50, char *pData);
    

    BOOL m_bIsSupoortSubscribe;
    int m_iDeviceIndex;  //用于接收选中的单个设备
    BOOL setDeviceAbility();
    void CheckITDeviceAbility();
    void CheckEventAbility();
    void CheckVcaChanAbility();
    void CheckBasicCapability();
    void CheckFaceContrast();
    void CheckVehicleRecogAbility();
    void CheckCloudStorageCfg();
    void UpdateChanStatus();
    void GetModeAllSubscribeParam(CString & strData);
    void GetChangedUploadSubParam(CString & strData);

    BOOL m_bJsonPass;
    CDlgXmlSub m_DlgXmlSub;
    char m_pSub[MAX_SUBSCRIBE_DATA_LENGTH];
    afx_msg void OnBnClickedBtnXmlsub();
    CComboBox m_cmbBehavior;

    CListCtrl m_listChan;
    BOOL m_bChannelEnable;
    CComboBox m_comboPictureUrlType;
    afx_msg void OnBnClickedButtonGetSubscribe();
    afx_msg void OnBnClickedButtonSetSubscribe();
    //afx_msg void OnBnClickedButtonAddSubscribe();
    CString m_strSubscribe;
    CComboBox m_comboEventType;

    void InitEventType();
    CListCtrl m_listEventParam;
    afx_msg void OnBnClickedButtonAddParam();
    afx_msg void OnBnClickedButtonDeleteParam();
    int m_iRowCount;
    int m_iSubItem;
    int m_iLastItem;
    afx_msg void OnNMClickListEventParam(NMHDR *pNMHDR, LRESULT *pResult);
    void GenerateSubcribeParam();
    void GenerateSubcribeListData();
    afx_msg void OnBnClickedCheckChannelEnable();
    BOOL m_bUploadAll;
    BOOL m_bUploadChannel;
    BOOL m_bUploadHd;
    BOOL m_bUploadCapability;
};
