#if !defined(AFX_DLGACSFINGERPRINTCFG_H__627B5CBD_0A6A_4F33_83FD_7AED02B95B50__INCLUDED_)
#define AFX_DLGACSFINGERPRINTCFG_H__627B5CBD_0A6A_4F33_83FD_7AED02B95B50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAcsFingerPrintCfg.h : header file
//

#define MAX_COUNT    256

/////////////////////////////////////////////////////////////////////////////
// DlgAcsFingerPrintCfg dialog

class DlgAcsFingerPrintCfg : public CDialog
{
// Construction
public:
	DlgAcsFingerPrintCfg(CWnd* pParent = NULL);   // standard constructor
    ~DlgAcsFingerPrintCfg();

// Dialog Data
	//{{AFX_DATA(DlgAcsFingerPrintCfg)
	enum { IDD = IDD_DLG_ACS_FINGER_PRINT };
    CTreeCtrl	m_treeEnableCardReader;
    CTreeCtrl	m_treeReaderStatus;
    CTreeCtrl	m_treeDelFingerPrint;
    CListCtrl	m_listFingerPrintCfg;
    BOOL	m_byClearAllCard;
    BOOL	m_byCallbackMode;
    CComboBox	m_comboFingerType;
    CComboBox	m_comboDelMode;
    CString	m_sCardNo;
    CString	m_sFingerPrintPath;
    int     m_byFingerPrintID;
    int     m_dwFingerPrintLen;
    int     m_dwCardReaderNo;
    int m_dwCardNum;

    CListCtrl	m_listScreen;
    CComboBox	m_comboY;
    CComboBox	m_comboX;
    BOOL	m_bEnable;
    BYTE	m_byWallNo;
    DWORD	m_dwOutputNum;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgAcsFingerPrintCfg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgAcsFingerPrintCfg)
    virtual BOOL OnInitDialog();
    afx_msg void OnBtnSetFingerPrintcfg();
	afx_msg void OnBtnGetAllFingerPrint();
    afx_msg void OnBtnAddToList();
	afx_msg void OnBtnDel();
    afx_msg void OnClickTreeEnableCardReader(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnClickTreeDelFingerPrint(NMHDR* pNMHDR, LRESULT* pResult);

    afx_msg void OnClose();
    afx_msg void OnDestroy();
    afx_msg LRESULT OnMsgSetFingerPrintCfgFinish(WPARAM wParam,LPARAM lParam);
    afx_msg LRESULT OnMsgGetFingerPrintCfgFinish(WPARAM wParam,LPARAM lParam);
    afx_msg LRESULT OnMsgAddFingerPrintCfgToList(WPARAM wParam,LPARAM lParam);
    afx_msg LRESULT OnMsgUpdateData(WPARAM wParam,LPARAM lParam);
    afx_msg LRESULT OnMsgDelFingerPrintFinish(WPARAM wParam, LPARAM lParam);

    afx_msg void OnBtnStopSend();


	afx_msg void OnBtnSave();
	afx_msg void OnBtnSet();
	afx_msg void OnBtnExit();		
	afx_msg void OnClickListScreen(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnGetAll();
	afx_msg void OnBtnSaveCfg();
	
	afx_msg void OnSelchangeComboOutput();
	afx_msg void OnBtnGet();
	afx_msg void OnButVideowallControl();
	afx_msg void OnButVwscSend();
	afx_msg void OnChkVwsAlldisplayno();
	afx_msg void OnButDispParamAllSame();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	LONG m_lUserID;
	LONG m_iDeviceIndex;
    LONG m_lSetFingerPrintCfgHandle;
	LONG m_lGetFingerPrintCfgHandle;
    CWinThread* m_pProcThread; 
    HANDLE m_hStopProcEvent;

    NET_DVR_FINGER_PRINT_CFG m_struFingerPrintOne;
    NET_DVR_FINGER_PRINT_INFO_CTRL m_struDelFingerPrint;
    
    BYTE m_byInitWallNo;                                            //��ʼ��ǽ��
	DWORD m_dwCount;                                                //�޸ĸ���
	DWORD m_dwDispNum;                                              //��ʾ����
	int m_iCurSel;                                                      
	LONG  m_ModifyChan[MAX_COUNT];                                    //�޸Ĺ���ʾ���������                              
	LONG m_lDispChanSet[MAX_COUNT];                                  //��¼m_struWallParam�޸����±�
    DWORD m_dwStatus[MAX_COUNT];                                     //״̬��
	NET_DVR_FINGER_PRINT_CFG m_struFingerPrintCfg[MAX_COUNT];      //
	NET_DVR_FINGER_PRINT_CFG m_struFingerPrintCfgSet[MAX_COUNT];   //�޸Ĺ�� ��������
	NET_DVR_MATRIX_ABILITY_V41 m_struAblity;
    LONG    m_lDispChan[MAX_COUNT];                                   //��ʾ���������    ��m_struOutputһһ��Ӧ
    
    LONG m_lPapamCount;                                               //�����������    ��ȡ����λ�ò����󲻻�仯
    LONG m_lRecordCount;                                              //��������޸ļ���
    LONG m_dwRecordPapam[MAX_COUNT];                                  //��¼�޸Ĺ������±�
    NET_DVR_WALLOUTPUTPARAM m_struOutput[MAX_COUNT];
	NET_DVR_WALLOUTPUTPARAM m_struOutputSet[MAX_COUNT];
    
    NET_DVR_VIDEOWALLDISPLAYMODE m_struDisplayMode;                   // ��ʾģʽ
  

	LONG m_lDispOutputSet[MAX_COUNT];
	DWORD m_dwOutputSet;
    LONG m_lRemoteHandle;
	void DrawList();


protected:
    BOOL    StartProcThread();
	BOOL    StopProcThread();
    LPDWORD m_lpRecordCardCfg; 
    CWinThread* m_pDisplayListThread; 
    DWORD m_dwBatchAddNum;    //��ӿ�����
    
    LPDWORD GetFingerPrintCfgPoint();    //��ȡָ�򿨲�����ָ��
    BOOL UpdateOutputNum(const NET_DVR_FINGER_PRINT_CFG &struDisplayPos); 
    BOOL UpdateFingerPrintCfg(NET_DVR_FINGER_PRINT_CFG &struDisplayPos);
    BOOL NewOutputNum(const NET_DVR_FINGER_PRINT_CFG &struDisplayPos);            //�»�ȡ��
    LPNET_DVR_FINGER_PRINT_CFG GetModifyDisplayPos();                             //��ȡ�޸Ĺ���
    LONG *GetModifyChan();                                                                //��ȡ�޸Ĺ���ʾ���������
    BOOL ClearModify();                                                                   //�����޸ļ�¼

    LONG *GetModifyPapamChan();                                                           //��ȡ�޸Ĺ����ò�������ʾ���������

    void InitOutputLinkMode();
    void AddToFingerPrintList(const NET_DVR_FINGER_PRINT_CFG& struFingerPrintCfg);
    void OnDeleteitemListCard(NMHDR* pNMHDR, LRESULT* pResult);
    int GetExistItem(const NET_DVR_FINGER_PRINT_CFG *lpCardCfg);
    void UpdateList(int iInsertIndex, const NET_DVR_FINGER_PRINT_CFG& m_struFingerPrintCfg);

public:
    void ProcessSetFingerPrintCfgCallbackData(DWORD dwType, void* lpBuffer, DWORD dwBufLen);
	void ProcessGetFingerPrintCfgCallbackData(DWORD dwType, void* lpBuffer, DWORD dwBufLen);
    void SendFingerPrintData();
    void SendFingerPrintData(LPNET_DVR_FINGER_PRINT_CFG lpCardCfg, DWORD dtest = 0);  //dtest �ڲ������� 
    void BatchAddFingerPrintToList();
    LPNET_DVR_FINGER_PRINT_CFG GetSelItem(char *lpCardNo = NULL); 
    
    //�ϲ�ά��ͬ���޸�
protected:
    BOOL SendFirstCard();      //���͵�һ��
    BOOL ReSendLastFingerPrint();     //�ط�
    BOOL SendNextFingerPrint();       //������һ��
    DWORD m_dwSendIndex;       //�·����	
    LPNET_DVR_FINGER_PRINT_CFG m_lpNowSendCard; 
	BYTE m_byLastCardSendTime;   //�ط�����

private:
    NET_DVR_DISPLAYCFG m_struDisplayCfg;                              //��ʾ�����״̬
    DWORD m_dwNowSendItem; 
    int   m_iSelListItem; 
    void CreateTree();
    void GetTreeSel(); 
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGACSFINGERPRINTCFG_H__627B5CBD_0A6A_4F33_83FD_7AED02B95B50__INCLUDED_)
