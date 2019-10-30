#pragma once


// CDlgAcsXmlDiffusion �Ի���

class CDlgAcsXmlDiffusion : public CDialog
{
	DECLARE_DYNAMIC(CDlgAcsXmlDiffusion)

public:
	CDlgAcsXmlDiffusion(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgAcsXmlDiffusion();
    BOOL OnInitDialog();
    void GetCommandStrAndInputParam();

// �Ի�������
	enum { IDD = IDD_DLG_ACS_XML_DIFFUSION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    int m_iDevIndex;
    long m_lServerID;
    CComboBox m_cmbOperateType;
    CComboBox m_cmbCommand;
    CString	m_csCommandStr;
    CString	m_csInputParam;
    CString	m_csOutputParam;
    afx_msg void OnBnClickedBtnGet();
    afx_msg void OnBnClickedBtnExit();
    afx_msg void OnCbnSelchangeComboOperateType();
    afx_msg void OnCbnSelchangeComboCommand();
    BOOL m_bJsonFormat;
};
