#pragma once
#include "afxwin.h"


// CDlgAcsLocalControllerCfg �Ի���

class CDlgAcsLocalControllerCfg : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAcsLocalControllerCfg)

public:
	CDlgAcsLocalControllerCfg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgAcsLocalControllerCfg();

// �Ի�������
	enum { IDD = IDD_DLG_ACS_LOCAL_CONTROLLER_CFG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_iLocalControllerID;
	CString m_strDeviceName;
	CString m_strIPAddress;
	int m_iPortNo;
	CString m_strDefaultGateway;
	CString m_strSubnetMask;
	BOOL m_bNetworkEnable;
	CComboBox m_cmbDeviceType;
	CComboBox m_cmbOfflineWorkMode;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonGet();
	afx_msg void OnBnClickedButtonSet();
	afx_msg void OnBnClickedButtonDelete();

	LONG m_lUserID;
	int m_iDeviceIndex;
	BOOL CreateLocalControllerXML(char* pBuf, DWORD dwBufLen, int &dwRet);
	afx_msg void OnBnClickedButtonDeleteAll();
	CComboBox m_cmbCommandType;
	afx_msg void OnBnClickedButtonControl();
	BOOL CreateLocalControllerControlXML(char* pBuf, DWORD dwBufLen, int &dwRet);

};
