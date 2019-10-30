#pragma once
#include "afxwin.h"


// CDLGTrialAudioMix �Ի���

class CDLGTrialAudioMix : public CDialogEx
{
	DECLARE_DYNAMIC(CDLGTrialAudioMix)

public:
	CDLGTrialAudioMix(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDLGTrialAudioMix();

// �Ի�������
	enum { IDD = IDD_DLG_TRIAL_AUTIO_MIX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnGetSingleAudiomixcfg();
    LONG    m_lUserID;
    LONG    m_lChannel;
    DWORD   m_dwDevIndex;
    CComboBox m_AudioMixID;

    afx_msg void OnBnClickedBtnSetSingleAudiomixcfg();
    CString m_AudioMixInfo;
    afx_msg void OnBnClickedBtnGetAudiomixcfg();
    afx_msg void OnBnClickedBtnSetAudiomixcfg();
    CComboBox m_SerialPortID;
    afx_msg void OnBnClickedBtnGetSingleSerialportCfg();
    afx_msg void OnBnClickedBtnSetSingleSerialportCfg();
    afx_msg void OnBnClickedBtnGetSerialportCfg();
    afx_msg void OnBnClickedBtnSetSerialportCfg();
};
