#pragma once


// CDlgXmlSub �Ի���

class CDlgXmlSub : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgXmlSub)

public:
	CDlgXmlSub(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgXmlSub();

// �Ի�������
	enum { IDD = IDD_DLG_XML_SUB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnCanael();
    afx_msg void OnBnClickedBtnConfirm();

    void getXmlSub(char* m_pData);
    char* m_xmlData;
    bool isXmlSub;
    virtual BOOL OnInitDialog();
    CString m_strSubEventXml;
};
