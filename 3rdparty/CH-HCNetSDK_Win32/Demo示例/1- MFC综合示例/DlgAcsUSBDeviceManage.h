#pragma once
#include "afxwin.h"


// CDlgAcsUSBDeviceManage �Ի���

class CDlgAcsUSBDeviceManage : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAcsUSBDeviceManage)

public:
	CDlgAcsUSBDeviceManage(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgAcsUSBDeviceManage();

// �Ի�������
	enum { IDD = IDD_DLG_USB_DEVICE_MANAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_comCommandType;
	CComboBox m_comConnetStatus;
	CString m_strDeviceName;
	int m_iUSBNumber;
	afx_msg void OnBnClickedButtonGet();
	afx_msg void OnBnClickedButtonControl();

	LONG m_lUserID;
	int m_iDeviceIndex;
	BOOL CreateUSBControlXML(char* pBuf, DWORD dwBufLen, int &dwRet);

#if (_MSC_VER >= 1500)	//vs2008
    afx_msg void OnTimer(UINT_PTR nIDEvent);
#else
    afx_msg void OnTimer(UINT nIDEvent);
#endif

    int m_iProgress;
};
