#pragma once
#include "afxwin.h"

#define TIME_AUDIO_CONTROL_FILE_PROGRESS 1

// CDlgAudioControlFile 对话框

class CDlgAudioControlFile : public CDialog
{
	DECLARE_DYNAMIC(CDlgAudioControlFile)

public:
	CDlgAudioControlFile(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAudioControlFile();

// 对话框数据
	enum { IDD = IDD_DLG_AUDIO_CONTROL_FILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

protected:
    virtual BOOL OnInitDialog();

private:
    LONG m_lUpdownloadHandle; //上传/下载句柄
    BOOL m_bUploading;        //正在上传

public:
    LONG m_iUserID;
    LONG m_iDeviceIndex;
    afx_msg void OnBnClickedBtnOpenAudioFile();

    CString m_szAudioFilePath;
    afx_msg void OnBnClickedBtnAudioControlFileImport();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
};
