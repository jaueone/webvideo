#pragma once
#include "afxwin.h"


// CDlgArmingHost �Ի���

class CDlgArmingHost : public CDialog
{
    DECLARE_DYNAMIC(CDlgArmingHost)

public:
    CDlgArmingHost(CWnd* pParent = NULL);   // ��׼���캯��
    virtual ~CDlgArmingHost();

    // �Ի�������
    enum { IDD = IDD_DLG_IPC_ANR_ARMING_HOST };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
    afx_msg void OnClickedBtnGet();
    afx_msg BOOL OnInitDialog();
    DECLARE_MESSAGE_MAP()

public:
    LONG    m_lServerID;
    int     m_iDevIndex;
    LONG    m_lChannel;

    DWORD m_dwPort;
    CString m_strIpv4;
    CString m_strIpv6;
    CComboBox m_cmbANRType;
    afx_msg void OnBnClickedCancel();
    CComboBox m_cmbConfirmMechanismEnabled;
};
