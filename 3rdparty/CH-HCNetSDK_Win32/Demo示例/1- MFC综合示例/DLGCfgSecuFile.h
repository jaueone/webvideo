#pragma once


// CDLGCfgSecuFile �Ի���

class CDLGCfgSecuFile : public CDialogEx
{
	DECLARE_DYNAMIC(CDLGCfgSecuFile)

public:
	CDLGCfgSecuFile(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDLGCfgSecuFile();

// �Ի�������
	enum { IDD = IDD_DLG_CFG_SECU_FILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnFileBrower();
    CString m_strFilePath;
    HANDLE  m_hFile;
    LONG m_iFileSize;
    CString m_strSecretKey;
    afx_msg void OnBnClickedBtnUpload();
    LONG m_lUploadHandle;
    HANDLE	m_hUpLoadThread;
    BOOL    m_bUpLoading;
    afx_msg void OnBnClickedBtnDwonload();
    HANDLE  m_hDownloadThread;
    LONG    m_lDownloadHandle;
    BOOL m_bStopDownload;
    LONG  m_lChannel;
    LONG  m_lUserID;
    int   m_iDeviceIndex;
};
