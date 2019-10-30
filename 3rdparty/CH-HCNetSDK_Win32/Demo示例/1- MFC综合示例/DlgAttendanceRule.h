#pragma once


// CDlgAttendanceRule �Ի���

class CDlgAttendanceRule : public CDialog
{
	DECLARE_DYNAMIC(CDlgAttendanceRule)

public:
	CDlgAttendanceRule(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgAttendanceRule();
    BOOL OnInitDialog();

// �Ի�������
	enum { IDD = IDD_DLG_ATTENDANCE_RULE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    int m_iDevIndex;
    long m_lServerID;
    afx_msg void OnBnClickedBtnGetAttendanceRule();
    afx_msg void OnBnClickedBtnSetAttendanceRule();
    int m_iAttendanceRuleID;
    CComboBox m_cmbOnDutyEnable;
    CComboBox m_cmbOffDutyEnable;
    CComboBox m_cmbRestEnable;
    CComboBox m_cmbEnable;
    int m_iOnDutyAdvance;
    int m_iOnDutyDelay;
    int m_iOnDutyAbsence;
    int m_iOffDutyAdvance;
    int m_iOffDutyDelay;
    int m_iOffDutyAbsence;
    int m_iRest;
    BOOL CreateAttendanceRuleXml(char* pBuf, DWORD dwBufLen, int &dwRet);
};
