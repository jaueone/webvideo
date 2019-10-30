#pragma once
#include "afxwin.h"


// CDlgIperf �Ի���

class CDlgIperf : public CDialog
{
	DECLARE_DYNAMIC(CDlgIperf)

public:
	CDlgIperf(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgIperf();

// �Ի�������
	enum { IDD = IDD_DLG_IPERF };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeComboProType();
	afx_msg void OnBnClickedBtnTest();
private:
	// Э����������
	CComboBox m_ComboProType;
	CEdit m_EdtBandWidth;
	// ������IP
	CString m_strServerIP;
	// �������˿�
	short m_wPort;
	UINT m_dwBandWidth;
	// ����ʱ��
	UINT m_dwTestTime;

protected:
	void RunIperf();
	HANDLE m_hProcess;
};
