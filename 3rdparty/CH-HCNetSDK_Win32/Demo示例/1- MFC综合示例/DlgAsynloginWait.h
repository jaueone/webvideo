#pragma once


// CDlgAsynloginWait �Ի���

class CDlgAsynloginWait : public CDialog
{
	DECLARE_DYNAMIC(CDlgAsynloginWait)

public:
	CDlgAsynloginWait(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgAsynloginWait();

// �Ի�������
	enum { IDD = IDD_DIG_ASYNLOGIN_WAIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
