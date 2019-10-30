#pragma once


// CDlgAcsSearchExamCompareResult �Ի���

class CDlgAcsSearchExamCompareResult : public CDialog
{
	DECLARE_DYNAMIC(CDlgAcsSearchExamCompareResult)

public:
	CDlgAcsSearchExamCompareResult(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgAcsSearchExamCompareResult();

// �Ի�������
	enum { IDD = IDD_DLG_ACS_SEARCH_EXAM_COMPARE_RESULT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnSearch();
    virtual BOOL OnInitDialog();
    void ProcessSearchExamCompareResultData(DWORD dwType, void* lpBuffer, DWORD dwBufLen);
    afx_msg LRESULT OnMsgAddExamCompareResultToList(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnMsgSearchExamCompareResultFinish(WPARAM wParam, LPARAM lParam);
public:
    LONG m_lUserID;
    int m_iDeviceIndex;
    CListCtrl m_listSearchExamCompareResult;
    CString m_csExamRoundNo;
    CString m_csExamNo;
    COleDateTime m_startDate;
    COleDateTime m_startTime;
    COleDateTime m_endDate;
    COleDateTime m_endTime;
    LONG m_lSearchExamCompareResultHandle;
    LONG m_lSearchNum;
};
