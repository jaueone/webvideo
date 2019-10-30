#pragma once
#include "afxwin.h"


// CDlgRecordHostDirectedAdvanceCfg �Ի���

class CDlgRecordHostDirectedAdvanceCfg : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgRecordHostDirectedAdvanceCfg)

public:
	CDlgRecordHostDirectedAdvanceCfg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgRecordHostDirectedAdvanceCfg();

// �Ի�������
	enum { IDD = IDD_DLG_RECORD_HOST_DIRECTED_ADVANCE_CFG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonGet();
	afx_msg void OnBnClickedButtonSet();
	BOOL CreateDirectedAdvanceCfg(char* pBuf, DWORD dwBufLen, int &dwRet);
	int m_iValue;
	int m_iTypeTarget;
	LONG m_lUserID;
	int m_iDeviceIndex;
};
