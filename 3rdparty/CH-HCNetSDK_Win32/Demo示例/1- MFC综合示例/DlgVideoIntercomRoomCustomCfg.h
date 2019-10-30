#pragma once


// CDlgVideoIntercomRoomCustomCfg �Ի���

class CDlgVideoIntercomRoomCustomCfg : public CDialog
{
	DECLARE_DYNAMIC(CDlgVideoIntercomRoomCustomCfg)

public:
	CDlgVideoIntercomRoomCustomCfg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgVideoIntercomRoomCustomCfg();
    virtual BOOL OnInitDialog();

// �Ի�������
	enum { IDD = IDD_DLG_VIDEO_INTERCOM_ROOM_CUSTOM_CFG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButtonAdd();
    afx_msg void OnBnClickedButtonSet();
    afx_msg void OnBnClickedButtonGet();
    afx_msg LRESULT OnMsgSetRoomCustomCfgFinish(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnMsgGetRoomCustomCfgFinish(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnMsgAddRoomCustomCfgTolist(WPARAM wParam, LPARAM lParam);
    LONG m_lServerID;
    LONG m_iDevIndex;
    DWORD m_dwFormerRoomNo;
    DWORD m_dwCustomRoomNo;
    afx_msg void OnBnClickedButtonDel();
    CListCtrl	m_listRoomCustomCfg;
    afx_msg void OnLvnItemchangedListRoomCustomCfg(NMHDR *pNMHDR, LRESULT *pResult);
    BOOL UpdateRoomCustomCfg(NET_DVR_ROOM_CUSTOM_CFG &struRoomCustomCfg);
    void AddToRoomCustomList(const NET_DVR_ROOM_CUSTOM_CFG &struRoomCustomCfg);
    int GetExistItem(const NET_DVR_ROOM_CUSTOM_CFG *lpRoomCustomCfg);
    void UpdateList(int iInsertIndex, const NET_DVR_ROOM_CUSTOM_CFG& struRoomCustomCfg);
    LPDWORD GetRoomCustomCfgPoint();    //��ȡָ�򷿼��Զ������õ�ָ��
    LPDWORD m_lpRoomCustomCfg;  //ָ�򷿼��Զ������õ�ָ��
    LONG m_lSetRoomCustomCfgHandle;
    LONG m_lGetRoomCustomCfgHandle;
    DWORD m_dwSendIndex;       //�·����
    BOOL SendFirstRoomCustom();
    BOOL SendNextRoomCustom();
    LPNET_DVR_ROOM_CUSTOM_CFG m_lpNowSendRoomCustom;
    void ProcessSetRoomCustomCfgCallbackData(DWORD dwType, void* lpBuffer, DWORD dwBufLen);
    void ProcessGetRoomCustomCfgCallbackData(DWORD dwType, void* lpBuffer, DWORD dwBufLen);
};
