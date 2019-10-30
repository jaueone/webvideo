#pragma once


// CDlgRS485DLLParamCfg �Ի���

#define TIME_RS485_DLL_FILE_PROGRESS  1

class CDlgRS485DLLParamCfg : public CDialog
{
	DECLARE_DYNAMIC(CDlgRS485DLLParamCfg)

public:
	CDlgRS485DLLParamCfg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgRS485DLLParamCfg();

// �Ի�������
	enum { IDD = IDD_DLG_RS485_DLL_PARAM_CFG };

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
    DWORD m_dwUpLoadFileSize;

public:
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnBnClickedBtnChooseDllFile();
    afx_msg void OnBnClickedBtnSaveDllFile();
    afx_msg void OnBnClickedBtnImportDll();
    afx_msg void OnBnClickedBtnExportDll();
    CString m_sFilePath;
    CString m_sSaveFilePath;
};
