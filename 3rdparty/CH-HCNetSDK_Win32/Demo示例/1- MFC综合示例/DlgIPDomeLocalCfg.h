#if !defined(AFX_DLGIPDOMELOCALCFG_H__B3C7755B_50BC_490F_9A37_59479C25594B__INCLUDED_)
#define AFX_DLGIPDOMELOCALCFG_H__B3C7755B_50BC_490F_9A37_59479C25594B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgIPDomeLocalCfg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgIPDomeLocalCfg dialog

class CDlgIPDomeLocalCfg : public CDialog
{
// Construction
public:
	CDlgIPDomeLocalCfg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgIPDomeLocalCfg)
	enum { IDD = IDD_DLG_IPDOME_LOCAL_CFG };
	CComboBox	m_comboZoomAdjust;
	CComboBox	m_comboPositionID2;
	CComboBox	m_comboTrackPoint;
	CComboBox	m_comboPositionID;
	CComboBox	m_comboTrackMode;
	int		m_iAlarmDelay;
	int		m_iTrackHold;
	int		m_iDwellTime;
	CString	m_csPositionName;
	BOOL	m_bEnablePosition;
	BOOL	m_bPreDirection;
	BOOL	m_bTrackSmooth;
	int		m_iMaxTrackZoom;
	BOOL	m_bStopTrack;
	int		m_iStopValue;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgIPDomeLocalCfg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgIPDomeLocalCfg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnSetUp();
	afx_msg void OnBtnRealplay();
	afx_msg void OnBtnPatrolSet();
	afx_msg void OnBtnPatrolConfirm();
	afx_msg void OnSelchangeComboTrackPoint();
	afx_msg void OnBtnSetPosition();
	afx_msg void OnBtnGetPtz();
	afx_msg void OnBtnPtzCtrl();
	afx_msg void OnSelchangeComboPositionId2();
	afx_msg void OnChkEnable();
	afx_msg void OnBtnSetPatrolLimitangle();
	afx_msg void OnBtnGetMovement();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
    void InitComboZoomAdjust();
    long    m_lServerID;                        // �û�ID
    int     m_iDevIndex;                        // �豸����
    int     m_iChannel;                         // ͨ����
    int     m_iPositionID;                      // ����ID
    long    m_lPlayHandle;                      // Ԥ�����
    NET_DVR_PTZ_POSITION    m_struPtzPosition;  // PTZ λ����Ϣ
    NET_DVR_TRACK_PARAMCFG   m_struTrackParam;  // ����˵���������
    NET_DVR_PATROL_TRACKCFG m_struPatrolTrack;  //  ����˵�Ѳ����������
    int     m_iTrackIPointndex;                       // Ѳ�������� 
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGIPDOMELOCALCFG_H__B3C7755B_50BC_490F_9A37_59479C25594B__INCLUDED_)
