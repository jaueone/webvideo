#pragma once


// CDlgOISCfg �Ի���

class CDlgOISCfg : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgOISCfg)

public:
	CDlgOISCfg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgOISCfg();

// �Ի�������
	enum { IDD = IDD_DLG_OIS_PARA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    LONG m_lServerID;
    int m_iDeviceIndex;
    LONG m_lChannel;
    CComboBox m_enable;
    CComboBox m_level;
    char m_szStatusBuf[ISAPI_STATUS_LEN];
    afx_msg void OnBnClickedButtonGet();
    NET_DVR_OIS_CFG m_struBuiltinOIS;
    afx_msg void OnBnClickedButtonSet();
    CComboBox m_OISSensitivity;
};
