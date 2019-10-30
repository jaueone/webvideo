#pragma once


// CDlgVideoIntercomNoticeVideoDataCfg �Ի���
#define TIMER_UPDOWNLOAD_CONFIG_PROGRESS 1 //�ϴ����ؽ��ȶ�ʱ��

class CDlgVideoIntercomNoticeVideoDataCfg : public CDialog
{
	DECLARE_DYNAMIC(CDlgVideoIntercomNoticeVideoDataCfg)

public:
	CDlgVideoIntercomNoticeVideoDataCfg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgVideoIntercomNoticeVideoDataCfg();

// �Ի�������
	enum { IDD = IDD_DLG_VIDEO_INTERCOM_NOTICE_VIDEO_DATA_CFG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
    virtual BOOL OnInitDialog();
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg LRESULT OnMsgGetVideoDataFinish(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnScan();
    afx_msg void OnBnClickedBtnUpload();
    void ProcessGetNoticeVideoDataCallbackData(DWORD dwType, void* lpBuffer, DWORD dwBufLen);
    LONG m_lServerID;
    LONG m_iDevIndex;

    CString	m_szUpload;
    DWORD m_dwFileLength;
    NET_DVR_NOTICE_VIDEO_DATA m_struNoticeVideoData;
    BOOL m_bUploading;        //�����ϴ�
    LONG m_lUpdownloadHandle; //�ϴ�/���ؾ��
    CProgressCtrl	m_ctrlProgress;
    CStatic	m_statProgress;
    afx_msg void OnBnClickedBtnStart();
    afx_msg void OnBnClickedBtnStop();
    LONG m_lGetHandle;
    DWORD m_dwVideoDataLen;
    HANDLE m_hVideoDataFile;
    DWORD m_dwVideoDataReturn;
    CString	m_strNoticeNumber;
};
