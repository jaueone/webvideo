#pragma once


// CDlgUHFRFIDReaderCfg �Ի���

class CDlgUHFRFIDReaderCfg : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgUHFRFIDReaderCfg)

public:
	CDlgUHFRFIDReaderCfg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgUHFRFIDReaderCfg();

// �Ի�������
	enum { IDD = IDD_DLG_UHF_RFID_READER_CFG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButtonBasicCfg();
    afx_msg void OnBnClickedButtonHardDiskStorageTest();

    LONG m_lUserID;
    int m_iDeviceIndex;
};
