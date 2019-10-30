#pragma once


// CDlgVcaVersionList �Ի���

class CDlgVcaVersionList : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgVcaVersionList)

public:
	CDlgVcaVersionList(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgVcaVersionList();

// �Ի�������
	enum { IDD = IDD_DLG_VCA_VERSION_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
    char m_szStatusBuf[ISAPI_STATUS_LEN];

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnGetVcaVersionlist();
    virtual BOOL OnInitDialog();
    CListCtrl m_lstVcaVersionList;
    long m_lChannel;
    long m_lUserID;
    int m_lDeviceIndex;
    void AddVcaVersionListItem(const NET_DVR_VCA_VERSION_LIST &Item);
};
