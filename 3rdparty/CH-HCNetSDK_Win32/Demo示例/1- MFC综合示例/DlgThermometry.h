
#pragma once
#include "VcaCommonOperations.h"
#include "afxwin.h"

// CDlgThermometry 对话框
#define  TEMPERATURE_POINT_NUM    4
#define MAX_LEN_XML 10*1024*1024 //xml最大长度
#define MSG_SET_TEMP_INFO  WM_USER+100
#define  USERDEFINE_VALUE  0.005


class CDlgThermometry : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgThermometry)

public:
	CDlgThermometry(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgThermometry();

// 对话框数据
	enum { IDD = IDD_DLG_THERMOMETRY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
    BOOL StartRealPlay();
    BOOL StopRealPlay();
public:
    long        m_lPlayHandle; // 预览句柄
    int m_RuleID;
    float m_Max;
    float m_Min;
    float m_Ave;
    float m_max_x;
    float m_max_y;
    float m_min_x;
    float m_min_y;
    afx_msg void OnBnClickedButton10();
    LONG    m_lUserID;
    int     m_iDevIndex;
    LONG    m_lChannel;
    NET_DVR_THERMOMETRYRULE_TEMPERATURE_INFO m_struTempInfo;
    float m_fDistance;
    float m_fEnviroTemperature;
    float m_fCorrEmissivity;
    float m_fPreTemperature1;
    float m_fPreTemperature2;
    float m_fPreTemperature3;
    float m_fPreTemperature4;
    CString m_strFilePath;
	CString m_Freezedata;
    BOOL m_bStatus1;
    BOOL m_bStatus2;
    BOOL m_bStatus3;
    BOOL m_bStatus4;
    afx_msg void OnBnClickedBtnSetFile();
    afx_msg void OnBnClickedBtnSetTemperature1();
    afx_msg void OnBnClickedBtnSetTemperature2();
    afx_msg void OnBnClickedBtnSetTemperature3();
    afx_msg void OnBnClickedBtnSetTemperature4();
    void  SetTemperatureInfo(DWORD dwTemperaturePoint, float fPreTemperature);
    DWORD m_dwCurrentIndex;
    float m_fPreTemperature[TEMPERATURE_POINT_NUM];
    float m_fCoordinatesX[TEMPERATURE_POINT_NUM];
    float m_fCoordinatesY[TEMPERATURE_POINT_NUM];
    NET_VCA_POINT m_struPoint;
    NET_DVR_XML_CONFIG_INPUT m_struInput;
    NET_DVR_XML_CONFIG_OUTPUT m_struOuput;
    char *m_pBuffer;
    LRESULT SetTemperaturePointInfo(WPARAM wParam, LPARAM lParam);
    void ConvertModeXMLNodeToShow(const char* pXMLBuf);
    void DrawRgnFun(HDC hdc);

    afx_msg void OnBnClickedBtnImport();
    afx_msg void OnBnClickedBtnGetCorrectionParam();
    CButton m_btnStartCorrect;
    afx_msg void OnBnClickedBtnStratCorrect();
    CRect   m_rcPlayWnd;
    NET_VCA_POLYGON m_struPolyGon;
    LONG m_lRealHandle;

    virtual BOOL OnInitDialog();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	
};