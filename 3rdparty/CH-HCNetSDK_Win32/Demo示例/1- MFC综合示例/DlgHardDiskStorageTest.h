#pragma once


// CDlgHardDiskStorageTest �Ի���

class CDlgHardDiskStorageTest : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgHardDiskStorageTest)

public:
	CDlgHardDiskStorageTest(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgHardDiskStorageTest();

// �Ի�������
	enum { IDD = IDD_DLG_HARD_DISK_STORAGE_TEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButtonGet();
    CString m_strCardNo;
    int m_iRssi;

    LONG m_lUserID;
    int m_iDeviceIndex;
    afx_msg void OnTimer(UINT_PTR nIDEvent);

    BOOL m_bStart;
    void GetHardDiskStorageTest();
    afx_msg void OnClose();
};
