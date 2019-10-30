#pragma once
#include "afxwin.h"


// CDlgInquestTrayConfig �Ի���

class CDlgInquestTrayConfig : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgInquestTrayConfig)

public:
	CDlgInquestTrayConfig(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgInquestTrayConfig();

// �Ի�������
	enum { IDD = IDD_DLG_INQUEST_TRAY_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    LONG m_lUserID;
    int m_iDeviceIndex;
    CString m_strTrayNo;
    BOOL m_bIsAbnormal;
    CComboBox m_comboTrayStatus;
    afx_msg void OnBnClickedButtonGet();
    afx_msg void OnBnClickedButtonSet();
    BOOL CreateTrayConfigXML(char* pBuf, DWORD dwBufLen, int &dwRet);
};
