#pragma once


// CDlgAlarmHostDong �Ի���

#define TIME_MODBUS_CFG_FILE_PROGRESS 1

class CDlgAlarmHostDong : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAlarmHostDong)

public:
	CDlgAlarmHostDong(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgAlarmHostDong();

// �Ի�������
	enum { IDD = IDD_DLG_ALARMHOST_DONG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
    LONG m_iUserID;
    LONG m_iDeviceIndex;

private:
    LONG m_lUpdownloadHandle; //�ϴ�/���ؾ��
    BOOL m_bUploading;        //�����ϴ�
    BOOL m_bDownloading;      //��������

public:
    CString m_sFilePath;
    afx_msg void OnBnClickedBtnChooseFile();
    afx_msg void OnBnClickedBtnSaveFile();
    afx_msg void OnBnClickedBtnImport();
    afx_msg void OnBnClickedBtnExport();
    CString m_sSaveFilePath;
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    DWORD m_dwChanNo;
    DWORD m_dwSlotNo;
    CString m_sOutput1;
    CString m_sOutput2;
    CString m_sOutput3;
    CString m_sOutput4;
    CString m_sOutput5;
    CString m_sOutput6;
    afx_msg void OnBnClickedBtnControl();
    BOOL m_bSwitch;
};
