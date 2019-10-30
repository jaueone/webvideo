#pragma once
#include "RecvCardArea.h"
#include "RecvCardVersion.h"

// CDlgLEDVersion �Ի���

class CDlgLEDVersion : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgLEDVersion)

public:
	CDlgLEDVersion(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgLEDVersion();

// �Ի�������
	enum { IDD = IDD_DLG_LED_VERSION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    DWORD m_dwNo;
    DWORD m_dwStartLine;
    DWORD m_dwStartColumn;
    DWORD m_dwEndLine;
    DWORD m_dwEndColumn;
    CString m_szBordType;
    CString m_szSoftType;
    CString m_szSoftVersion;
    afx_msg void OnBnClickedBtnGet();
    afx_msg void OnBnClickedBtnRestore();

private:
    LONG m_lUserID;
    int m_iDeviceIndex;
    RecvCardArea m_struArea;
    RecvCardVersion m_struVersion;
public:
    virtual BOOL OnInitDialog();
};
