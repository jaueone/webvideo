#pragma once


// CDlgAcsSetExamineeInfo �Ի���

class CDlgAcsSetExamineeInfo : public CDialog
{
	DECLARE_DYNAMIC(CDlgAcsSetExamineeInfo)

public:
	CDlgAcsSetExamineeInfo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgAcsSetExamineeInfo();

// �Ի�������
	enum { IDD = IDD_DLG_ACS_SET_EXAMINEE_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnAdd();
    afx_msg void OnBnClickedBtnDelete();
    afx_msg void OnBnClickedBtnClear();
    afx_msg void OnBnClickedBtnSetList();
    afx_msg void OnBnClickedBtnDeleteByno();
    afx_msg void OnBnClickedBtnExit();
    virtual BOOL OnInitDialog();
    int GetExistItem(const NET_DVR_EXAMINEE_INFO_CFG *lpExamineeInfoCfg);
    void Clear();
    BOOL SendFirst();
    BOOL SendNext();
    void ProcessExamineeInfoCallbackData(DWORD dwType, void* lpBuffer, DWORD dwBufLen);

public:
    LONG m_lUserID;
    int m_iDeviceIndex;
    CListCtrl m_listSetExamineeInfo;
    CString m_csExamineeNo;
    CString m_csAdmissionTicket;
    CString m_csExamRoundNo;
    CString m_csName;
    CString m_csCardNo;
    CComboBox m_cmbSex;
    BOOL m_bExamineeInfoValid;
    LONG m_lSetExamineeInfoHandle;
    DWORD m_dwSendIndex;       //�·����
protected:
    afx_msg LRESULT OnMsgSetExamineeInfoFinish(WPARAM wParam, LPARAM lParam);
};
