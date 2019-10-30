#pragma once
#include "afxwin.h"
#include "ATLComTime.h"


// CDlgAlarmInfo �Ի���

class CDlgAlarmInfo : public CDialog
{
	DECLARE_DYNAMIC(CDlgAlarmInfo)

public:
	CDlgAlarmInfo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgAlarmInfo();

// �Ի�������
	enum { IDD = IDD_DLG_ALARM_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButtonAlarmInfoGet();
    afx_msg void OnBnClickedOk();
    afx_msg LRESULT Finish(WPARAM wParam, LPARAM lParam);
    COleDateTime	m_startDate;
    COleDateTime	m_startTime;
    COleDateTime	m_stopDate;
    COleDateTime	m_stopTime;
    NET_DVR_ALARM_SEARCH_COND m_struAlarmSearchCond;
    NET_DVR_ALARM_SEARCH_RESULT m_struAlarmSearchResult;
    LONG m_lHandle;
    CComboBox m_cmbCommand;
    virtual BOOL OnInitDialog();
    int m_iDevIndex;
    long m_lServerID;
    BOOL m_bGetNext;
    void AddResolution();
    void ProcGetAlarmInfo(DWORD dwType, void* lpBuffer, DWORD dwBufLen);
    CString m_strUID;
    BOOL m_bUID;
    BOOL m_bTime;
    BOOL m_bUTC;
};
