#pragma once


// CDlgPicBatchUpload �Ի���

class CDlgPicBatchUpload : public CDialog
{
	DECLARE_DYNAMIC(CDlgPicBatchUpload)

public:
	CDlgPicBatchUpload(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgPicBatchUpload();

// �Ի�������
	enum { IDD = IDD_DLG_PIC_BTACH_UPLOAD };
    CString m_strPicPath;
    CString m_strPicCommenState;
    CString m_strPicSingleState;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnUploadPic();
    afx_msg void OnBnClickedBtnUploadPicStop();
    virtual BOOL OnInitDialog();

public:
    LONG m_lUserID;
    int m_iDeviceIndex;
    LONG m_lUploadPicHandle;
    HANDLE	m_hPicUpLoadThread;
    BOOL m_bStopUploadPic;
    NET_DVR_UPLOAD_FILE_RET m_struPicRet;

    static DWORD WINAPI PicUpLoadThread(LPVOID pParam);
    BOOL PicUpload(CString strFileName, CString strFilePath, BYTE byPicType);
    
};
