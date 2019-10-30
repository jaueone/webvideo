#pragma once
#include "afxwin.h"


// CDlgWiegandCfg �Ի���

class CDlgWiegandCfg : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgWiegandCfg)

public:
	CDlgWiegandCfg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgWiegandCfg();

// �Ի�������
	enum { IDD = IDD_DLG_ACS_WIEGAND_CFG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	int m_iWiegandNo;
	CComboBox m_cmbCommunicationDirection;
	CComboBox m_cmbWiegandMode;
	afx_msg void OnBnClickedButtonGet();
	afx_msg void OnBnClickedButtonSet();

	LONG m_lUserID;
	int m_iDeviceIndex;
	BOOL CreateWiegandCfg(char* pBuf, DWORD dwBufLen, int &dwRet);
	afx_msg void OnCbnSelchangeComboCommunicationDirection();
    DWORD m_dwSignalInterval;
};
