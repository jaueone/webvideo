#pragma once
#include "atltime.h"


// CDlgCVRPassbackBasicCfg �Ի���

class CDlgCVRPassbackBasicCfg : public CDialogEx
{
    DECLARE_DYNAMIC(CDlgCVRPassbackBasicCfg)

public:
    CDlgCVRPassbackBasicCfg(CWnd* pParent = NULL);   // ��׼���캯��
    virtual ~CDlgCVRPassbackBasicCfg();

    // �Ի�������
    enum { IDD = IDD_DLG_CVR_PASSBACK_BASIC_CFG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

    DECLARE_MESSAGE_MAP()
public:
    int m_iMaxTotalConcurrenceNum;
    int m_iMaxDvrConcurrenceNum;
    afx_msg void OnBnClickedButtonGet();
    afx_msg void OnBnClickedButtonSet();
    virtual BOOL OnInitDialog();
    char    m_szStatusBuf[ISAPI_STATUS_LEN];

    LONG  m_lServerID;
    LONG  m_iDevIndex;
    DWORD m_dwStartTime;
    DWORD m_dwStopTime;
};
