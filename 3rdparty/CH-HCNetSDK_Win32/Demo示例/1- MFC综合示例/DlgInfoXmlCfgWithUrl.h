#pragma once
#include "afxwin.h"


// CDlgInfoXmlCfgWithUrl �Ի���

class CDlgInfoXmlCfgWithUrl : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgInfoXmlCfgWithUrl)

public:
	CDlgInfoXmlCfgWithUrl(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgInfoXmlCfgWithUrl();

// �Ի�������
	enum { IDD = IDD_DLG_INFO_DIFFUSION_XML_CFG_URL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    CString m_csUrl;
    CString m_csInput;
    CString m_csOutput;
    afx_msg void OnBnClickedBtnGet();

    char	*m_pOutBuf;
    char *m_pInputBuf;

private:
    int   m_iDeviceIndex;
    LONG  m_lUserID;
public:
    virtual BOOL OnInitDialog();
    CComboBox m_cmbUrl;
    CString m_csCommand;
    afx_msg void OnSelchangeComboOperateType();
    afx_msg void OnBnClickedBtnOperate();
};
