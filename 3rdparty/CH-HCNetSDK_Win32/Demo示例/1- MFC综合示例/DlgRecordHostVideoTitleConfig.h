#pragma once


// CDlgRecordHostVideoTitleConfig �Ի���

class CDlgRecordHostVideoTitleConfig : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgRecordHostVideoTitleConfig)

public:
	CDlgRecordHostVideoTitleConfig(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgRecordHostVideoTitleConfig();

// �Ի�������
	enum { IDD = IDD_DLG_RECORD_HOST_VIDEO_TITLE_HOST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnGet();
    afx_msg void OnBnClickedBtnSet();
    BOOL m_bEnable;
    LONG m_lUserID;
    int m_iDeviceIndex;
    int m_ibackgroundImageIndex;
    int m_idisplayTimeLength;
    BOOL CreateVideoTitleConfiguration(char* pBuf, DWORD dwBufLen, int &dwRet);
    BOOL CreateVideoTrailerConfiguration(char* pBuf, DWORD dwBufLen, int &dwRet);
    BOOL m_bEnableTrailer;
    int m_iImageIndexTrailer;
    int m_iTimeLengthTrailer;
    afx_msg void OnBnClickedBtnGetTrailer();
    afx_msg void OnBnClickedBtnSetTrailer();
};
