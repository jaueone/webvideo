#pragma once


// CDlgRS485ProtocolVersion �Ի���

class CDlgRS485ProtocolVersion : public CDialog
{
	DECLARE_DYNAMIC(CDlgRS485ProtocolVersion)

public:
	CDlgRS485ProtocolVersion(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgRS485ProtocolVersion();
    CString  m_csVersion;
    LONG m_lUserID;
// �Ի�������
	enum { IDD = IDD_DLG_RS485_VERSION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnGet();
};
