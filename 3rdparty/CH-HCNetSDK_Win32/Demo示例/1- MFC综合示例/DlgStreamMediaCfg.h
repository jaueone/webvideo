#pragma once
#include "afxcmn.h"


// CDlgStreamMediaCfg �Ի���

class CDlgStreamMediaCfg : public CDialog
{
	DECLARE_DYNAMIC(CDlgStreamMediaCfg)

public:
	CDlgStreamMediaCfg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgStreamMediaCfg();

    NET_DVR_STREAM_MEDIA_CFG	m_struStreamMediaCfg[64];

// �Ի�������
	enum { IDD = IDD_DLG_STREAM_MEDIA_CFG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    CString m_csRelatedChannel;
    CString m_csStatus;
    CString m_csStreamID;
    CString m_csUrl;
    CListCtrl m_listCtrlStreamMediaCfg;
    afx_msg void OnBnClickedButtonAdd();
    afx_msg void OnBnClickedButtonDel();
    afx_msg void OnBnClickedButtonSet();
    afx_msg void OnBnClickedButtonGet();
    afx_msg void OnClickListStreamMediaCfg(NMHDR* pNMHDR, LRESULT* pResult);
};
