#if !defined(AFX_DLGACSCARDPASSWDCFG_H__5CBB5D01_EE05_43A9_AE37_AB1C6726D7E6__INCLUDED_)
#define AFX_DLGACSCARDPASSWDCFG_H__5CBB5D01_EE05_43A9_AE37_AB1C6726D7E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "DlgGatewayCardInfo.h"
// DlgGatewayCardCfg.h : header file
//

#define MAX_RESEND_CARD_TIME 5

/////////////////////////////////////////////////////////////////////////////
// DlgAcsCardPasswdCfg dialog

class DlgAcsCardPasswdCfg : public CDialog
{
// Construction
public:
	DlgAcsCardPasswdCfg(CWnd* pParent = NULL);   // standard constructor
	~DlgAcsCardPasswdCfg();

// Dialog Data
	//{{AFX_DATA(DlgAcsCardPasswdCfg)
	enum { IDD = IDD_DLG_ACS_CARD_PASSWD_CFG };
	CListCtrl	m_listBelongGroup;
	CListCtrl	m_listCardRightPlan;
	CListCtrl	m_listDoorRight;
	CListCtrl	m_listCard;
	BOOL	m_BcheckCardNo;
	CString	m_csBatchFlag;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgAcsCardPasswdCfg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgAcsCardPasswdCfg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnAddToList();
	afx_msg void OnDeleteitemListCard(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnModifyToList();
	afx_msg void OnDblclkListCard(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnDelCard();
	afx_msg void OnBtnSetCardcfg();
	afx_msg void OnBtnGetAllCard();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg LRESULT OnMsgSetcardCfgFinish(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMsgGetcardCfgFinish(WPARAM wParam,LPARAM lParam);
    afx_msg LRESULT OnMsgAddCardCfgToList(WPARAM wParam,LPARAM lParam);
    afx_msg LRESULT OnMsgUpdateData(WPARAM wParam,LPARAM lParam);
	afx_msg void OnBtnClearAllCard();
	afx_msg void OnClickListCardCfg(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButSimulationAdd();
	afx_msg void OnBtnGetSel();
	afx_msg void OnBtnSetSel();
	afx_msg void OnButBatchAdd();
	afx_msg void OnBtnStopSend();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	void InitCtrlState();
	void AddToCardList(const NET_DVR_CARD_PASSWD_CFG& struCardInfo , const char * pCardNo);
	void ModifyToCardList(int nItemIndex,const NET_DVR_CARD_PASSWD_CFG& struCardInfo);
public:
	void ProcessSetCardCfgCallbackData(DWORD dwType, void* lpBuffer, DWORD dwBufLen);
	void ProcessGetCardCfgCallbackData(DWORD dwType, void* lpBuffer, DWORD dwBufLen);
	void SendCardData();
    void SendCardData(LPNET_DVR_CARD_PASSWD_CFG lpCardCfg, DWORD dtest = 0);  //dtest �ڲ������� 
	void SetCardCfgState(CString strCardNumber,BOOL bSucc,CString strUserDefineState = "");

protected:
	BOOL    StartProcThread();
	BOOL    StopProcThread();
    BOOL    m_BSendSel; 
    LPNET_DVR_CARD_PASSWD_CFG m_lpSelSendCardCfg; 
public:
	LONG m_lServerID;
	LONG m_iDevIndex;
	LONG m_lSetCardCfgHandle;
	LONG m_lGetCardCfgHandle;
	CWinThread* m_pProcThread; 
	HANDLE m_hStopProcEvent;

public:
    void UpdateList(int iInsertIndex, const char * pCardNo, const NET_DVR_CARD_PASSWD_CFG& struCardInfo); 
    void UpdateSubList(const NET_DVR_CARD_PASSWD_CFG *lpCardCfg); 
    LPNET_DVR_CARD_PASSWD_CFG GetSelItem(char *lpCardNo = NULL); 
    int GetExistItem(const NET_DVR_CARD_PASSWD_CFG *lpCardCfg); 
    void ClearList(); 

private:
    //�ศ����¼����
    DWORD m_dwNowSendItem; 
    BYTE  m_byListSendStatus; 
    int   m_iSelListItem; 

protected:
    void ShowSelListItem(); 

//10����
protected:
    LPDWORD m_lpRecordCardCfg; 
    CWinThread* m_pDisplayListThread; 
    DWORD m_dwBatchAddNum;    //��ӿ�����

    LPDWORD GetCardCfgPoint();    //��ȡָ�򿨲�����ָ��
    void CreateSameCfgCard(DWORD dwNum, const NET_DVR_CARD_PASSWD_CFG &lpCardCfg); //����������ͬ�����Ŀ������Ų�һ��

//�ϲ�ά��ͬ���޸�
protected:
	BOOL SendFirstCard();      //���͵�һ�ſ�
	BOOL ReSendLastCard();     //�ط���
	BOOL SendNextCard();       //������һ�ſ�
	DWORD m_dwCardNum;         //�·�����
	DWORD m_dwSendIndex;       //�·����	
	LPNET_DVR_CARD_PASSWD_CFG m_lpNowSendCard; 
	BYTE m_byLastCardSendTime;   //�ط�����
public:
    void BatchAddCardToList();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGACSCARDPASSWDCFG_H__5CBB5D01_EE05_43A9_AE37_AB1C6726D7E6__INCLUDED_)
