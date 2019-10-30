#pragma once


// CDLGGetChannelInfo �Ի���

class CDLGGetChannelInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CDLGGetChannelInfo)

public:
	CDLGGetChannelInfo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDLGGetChannelInfo();

// �Ի�������
	enum { IDD = IDD_DLG_GET_CHANNEL_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
    afx_msg void OnBnClickedBtnGetSingleChaninfo();
    afx_msg void OnBnClickedBtnGetChaninfo();
    afx_msg void OnBnClickedBtnGetOnlineUserlist();
    afx_msg void OnBnClickedBtnGetMutexFunction();
	DECLARE_MESSAGE_MAP()
public:
    CString m_channelInfo;

    LONG  m_lChannel;
    LONG m_lUserID;
    int m_iDeviceIndex;
    char m_szStatusBuf[ISAPI_STATUS_LEN];

};
