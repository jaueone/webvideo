#pragma once


// CDlgAccessoryCardInfo �Ի���

class CDlgAccessoryCardInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAccessoryCardInfo)

public:
	CDlgAccessoryCardInfo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgAccessoryCardInfo();

// �Ի�������
	enum { IDD = IDD_DLG_ACCESSORY_CARDINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    CString m_szAccessoryCardInfo;
    int m_lUserID;
    int m_lChanNo;
    int m_iDevIndex;
    afx_msg void OnBnClickedBtnGet();
    afx_msg void OnBnClickedCancel();
};
#pragma once


