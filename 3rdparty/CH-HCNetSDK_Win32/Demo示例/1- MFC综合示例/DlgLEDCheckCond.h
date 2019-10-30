#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#define TIMER_UPDOWNLOAD_CONFIG_PROGRESS 1 //�ϴ����ؽ��ȶ�ʱ��
// CDlgLEDCheckCond �Ի���


class CDlgLEDCheckCond : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgLEDCheckCond)

public:
	CDlgLEDCheckCond(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgLEDCheckCond();

// �Ի�������
	enum { IDD = IDD_DLG_LED_CHECK };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    DWORD m_dwNo;
    CString m_szUploadFile;
    CString m_szDownloadFile;
    afx_msg void OnBnClickedBtnUploadScan();
    afx_msg void OnBnClickedBtnDownloadScan();
    afx_msg void OnBnClickedBtnUpload();
    afx_msg void OnBnClickedBtnCheckDownalod();


private:
    LONG m_lUserID;
    int m_iDeviceIndex;
    DWORD m_dwFileLength;
    NET_DVR_LED_CHECK_COND m_struCheckCond;

    LONG m_lUpDownloadHandle;//�ϴ����ؾ��
    BOOL m_bUploading;//�����ϴ�
    BOOL m_bDownloading;//��������
public:
    virtual BOOL OnInitDialog();
    CProgressCtrl m_CtrlProgress;
    CString m_szProgress;
    CStatic m_StaticProgress;
    afx_msg void OnTimer(UINT_PTR nIDEvent);
	int m_iStartPosColumn;
	int m_iStartPosLine;
    int m_iStartType;
    DWORD m_dwX;
    DWORD m_dwY;
    DWORD m_dwW;
    DWORD m_dwH;
};
