#pragma once
#include "afxcmn.h"


// CDlgAcsOnlineLocalControllerCfg �Ի���

class CDlgAcsOnlineLocalControllerCfg : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAcsOnlineLocalControllerCfg)

public:
	CDlgAcsOnlineLocalControllerCfg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgAcsOnlineLocalControllerCfg();

// �Ի�������
	enum { IDD = IDD_DLG_ACS_ONLINE_LOCAL_CONTROLLER_CFG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnMsgGetLocalControllerFinish(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsgAddLocalControllerToList(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsgUpdateData(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButtonSearch();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listOnlineLocalController;
	LONG m_lServerID;
	LONG m_iDevIndex;
	LONG m_lGetLocalControllerHandle;
	LONG m_iListIndex;

	void AddToLocalControllerList(const NET_DVR_ONLINE_LOCAL_CONTROLLER_CFG& struLocalControllerInfo, const WORD wLocalControllerNo);
	void ProcessGetLocalControllerCallbackData(DWORD dwType, void* lpBuffer, DWORD dwBufLen);
	afx_msg void OnClose();
};
