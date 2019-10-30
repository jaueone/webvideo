#pragma once


// CDlgPreviewOpts �Ի���

class CDlgPreviewOpts : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPreviewOpts)

public:
    CDlgPreviewOpts(CWnd* pParent = NULL);   // ��׼���캯��
    virtual ~CDlgPreviewOpts();

// �Ի�������
	enum { IDD = IDD_DLG_PREVIEW_OPTS };

    BOOL GetEnableUrl();
    CString &GetUrl();
    void SetEnableUrl(BOOL bEnable);
    void SetUrl(CString szUrl);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    CString m_csURL;
    int m_iDeviceIndex;
    BOOL m_bEnable;
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    virtual BOOL OnInitDialog();
};
