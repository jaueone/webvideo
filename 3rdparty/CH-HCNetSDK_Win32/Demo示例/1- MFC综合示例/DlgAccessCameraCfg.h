#if !defined(AFX_DLGACCESSCAMERACFG_H__DA93ACDC_7FA3_435D_893A_925DADD14DB0__INCLUDED_)
#define AFX_DLGACCESSCAMERACFG_H__DA93ACDC_7FA3_435D_893A_925DADD14DB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAccessCameraCfg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAccessCameraCfg dialog

#define MSG_SET_ACCESS_CAMERA_STATE WM_USER+102

class CDlgAccessCameraCfg : public CDialog
{
// Construction
public:
	CDlgAccessCameraCfg(CWnd* pParent = NULL);   // standard constructor
	~CDlgAccessCameraCfg();

	LONG    m_lUserID;
    LONG    m_lChannel;
	DWORD   m_dwDevIndex;
	INT     m_iHandle;   // ����ǰ��������ͳ�����handle
    BOOL    m_bTrialChannelFlag;//ǰ���豸����˽���ʱ����ͨ����Ϊ�豸����ǰѡ��ͨ����ͥ����������ͨ�����б������豸���ز�����̬����
// Dialog Data
	//{{AFX_DATA(CDlgAccessCameraCfg)
	enum { IDD = IDD_DIALOG_ACCESS_CAMER_CONFIG };
    CComboBox	m_cmbChanNo;
	CComboBox	m_cmbInferfaceType;
	CString	m_strAccessCameraInfo;
	CString m_strSetAccessCameraState;
	CString	m_szVlaue;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAccessCameraCfg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAccessCameraCfg)
	afx_msg void OnBtnSet();
	afx_msg void OnBtnGet();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	LRESULT OnSetAccessCameraState(WPARAM wParam, LPARAM lParam);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGACCESSCAMERACFG_H__DA93ACDC_7FA3_435D_893A_925DADD14DB0__INCLUDED_)
