#pragma once
#include "afxwin.h"

#define TIME_AUDIO_CONTROL_FILE_PROGRESS 1

// CDlgAudioControlFile �Ի���

class CDlgAudioControlFile : public CDialog
{
	DECLARE_DYNAMIC(CDlgAudioControlFile)

public:
	CDlgAudioControlFile(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgAudioControlFile();

// �Ի�������
	enum { IDD = IDD_DLG_AUDIO_CONTROL_FILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

protected:
    virtual BOOL OnInitDialog();

private:
    LONG m_lUpdownloadHandle; //�ϴ�/���ؾ��
    BOOL m_bUploading;        //�����ϴ�

public:
    LONG m_iUserID;
    LONG m_iDeviceIndex;
    afx_msg void OnBnClickedBtnOpenAudioFile();

    CString m_szAudioFilePath;
    afx_msg void OnBnClickedBtnAudioControlFileImport();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
};
