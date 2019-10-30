#include "resource.h"
#if !defined(AFX_DIALOGDOORCONTENT_H__B490BC03_7A35_4148_AA7B_A0D48F4EE03E__INCLUDED_)
#define AFX_DIALOGDOORCONTENT_H__B490BC03_7A35_4148_AA7B_A0D48F4EE03E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogDoorContent.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogDoorContent dialog
#define TIMER_UPDOWNLOAD_CONFIG_PROGRESS 1 //上传下载进度定时器

class CDialogDoorContent : public CDialog
{
// Construction
public:
	CDialogDoorContent(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogDoorContent)
	enum { IDD = IDD_DLG_DOOR_CONTENT };
	CProgressCtrl	m_ctrlProgress;
	CString	m_szDownload;
	CString	m_szUpload;
	CStatic	m_statProgress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogDoorContent)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogDoorContent)
	afx_msg void OnBtnScan();
	afx_msg void OnBtnScanD();
	afx_msg void OnBtnDownload();
	afx_msg void OnBtnUpload();
#if (_MSC_VER >= 1500)	//vs2008
    afx_msg void OnTimer(UINT_PTR nIDEvent);
#else
    afx_msg void OnTimer(UINT nIDEvent);
#endif
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	LONG m_lUserID;
	int m_iDeviceIndex;
    DWORD m_dwFileLength;
    NET_DVR_DOOR_FILE_UPLOAD_PARAM m_DoorFile;

	LONG m_lUpdownloadHandle; //上传/下载句柄
	BOOL m_bUploading;        //正在上传
	BOOL m_bDownloading;      //正在下载
public:
    afx_msg void OnBnClickedBtnCancon();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGDOORCONTENT_H__B490BC03_7A35_4148_AA7B_A0D48F4EE03E__INCLUDED_)
