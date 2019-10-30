#if !defined(AFX_PLAYREMOTEFILE_H__B24D222A_0E61_41C9_B462_5EF1F4F76443__INCLUDED_)
#define AFX_PLAYREMOTEFILE_H__B24D222A_0E61_41C9_B462_5EF1F4F76443__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlayRemoteFile.h : header file
//
#include "GeneralDef.h"

#define PLAY_STOP   0
#define PLAY_NORMAL 1
#define PLAY_PAUSE  2
#define PLAY_FAST   3
#define PLAY_SLOW   4

/////////////////////////////////////////////////////////////////////////////
// CPlayRemoteFile dialog

class CPlayRemoteFile : public CPropertyPage
{
	DECLARE_DYNCREATE(CPlayRemoteFile)

// Construction
public:
	void SetPlayState(int iState);
	void InitUI();
	BOOL PausePlayBack(BOOL bPlayOrPause);
	BOOL StopPlayback();
	BOOL StartPlayback();
	void InsertFileList();
	CPlayRemoteFile();
	~CPlayRemoteFile();

	LONG m_hPlayHandle;
	BOOL m_IsDownloading;
	LONG m_lDownloadHandle;
	BOOL m_IsPlaying;
	BOOL m_IsPause;
	BOOL m_bChangeSpeed;
	BOOL m_bPlayStep;
    BOOL m_bSound;
	BOOL m_bRecord;
	DWORD m_CurPlayPos;  //��ǰ���Ž���
	DWORD m_TotalFrames;  //��֡��
	DWORD m_CurFrames;    //��ǰ���ŵ�֡��
	DWORD m_TotalTime;    //��ʱ��
	DWORD m_CurTime;      //��ǰ����ʱ��
	NET_DVR_FILECOND   m_FileCond;   //�ļ����ҽṹ��
	int m_RecType;        //¼�����ͣ�0xff��ȫ����0����ʱ¼��1���ƶ���⣻2������������3-����|�ƶ���⣻4������&�ƶ���⣻5���������6���ֶ�¼��
	int m_FileProperty;   //�ļ����ԣ�0�������ļ���1�������ļ���0xff��ʾ�����ļ�
	CString m_csCurFile;  //��ǰ�����ļ���
// Dialog Data
	//{{AFX_DATA(CPlayRemoteFile)
	enum { IDD = IDD_PAGE_REMOTE };
	CSliderCtrl	m_sliderVolume;
	CSliderCtrl	m_sliderPlay;
	CProgressCtrl	m_progressDownload;
	CStatic	m_staticPlay;
	CListCtrl	m_ctrlFileList;
	CDateTimeCtrl	m_TimeStart;
	CDateTimeCtrl	m_TimeEnd;
	CDateTimeCtrl	m_DateEnd;
	CDateTimeCtrl	m_DateStart;
	CComboBox	m_coRecType;
	CComboBox	m_coFileProperty;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPlayRemoteFile)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hSoundStartIcon;
	HICON m_hSoundStopIcon;
	
	HICON m_hPlayEnableIcon;
	HICON m_hPlayDisableIcon;
	
	HICON m_hPauseEnableIcon;
	HICON m_hPauseDisableIcon;
	
	HICON m_hStopEnableIcon;
	HICON m_hStopDisableIcon;
	
	HICON m_hGotoHeadDisableIcon;
	HICON m_hGotoHeadEnableIcon;
	
	HICON m_hGotoTailEnableIcon;
	HICON m_hGotoTailDisableIcon;
	
	HICON m_hFastDisableIcon;
	HICON m_hFastEnableIcon;
	
	HICON m_hSlowDisableIcon;
	HICON m_hSlowEnableIcon;
	
	HICON m_hStepEnableIcon;
	HICON m_hStepDisableIcon;
	
	HICON m_hStepBackEnableIcon;
	HICON m_hStepBackDisableIcon;
	
	HICON m_hCaptureIcon;	
	// Generated message map functions
	//{{AFX_MSG(CPlayRemoteFile)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnSearch();
	afx_msg void OnDblclkFileList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonPlay();
	afx_msg void OnButtonStop();
	afx_msg void OnButtonDownload();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonBack();
	afx_msg void OnButtonStep();
	afx_msg void OnSelchangeComboType();
	afx_msg void OnSelchangeComboFileProperty();
	afx_msg void OnReleasedcaptureSliderPlay(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnButtonRemoteSlow();
	afx_msg void OnButtonRemoteFast();
	afx_msg void OnButtonRemotePic();
	afx_msg void OnButtonRemoteSound();
	afx_msg void OnReleasedcaptureSliderVolume(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonRemoteStopSave();
	afx_msg void OnButtonRemoteSave();
	afx_msg void OnClickFileList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonLockUnlock();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLAYREMOTEFILE_H__B24D222A_0E61_41C9_B462_5EF1F4F76443__INCLUDED_)
