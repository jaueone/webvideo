#pragma once


// CDlgDepartmentParam �Ի���

class CDlgDepartmentParam : public CDialog
{
	DECLARE_DYNAMIC(CDlgDepartmentParam)

public:
	CDlgDepartmentParam(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgDepartmentParam();
    BOOL OnInitDialog();

// �Ի�������
	enum { IDD = IDD_DLG_DEPARTMENT_PARAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    int m_iDevIndex;
    long m_lServerID;
    afx_msg void OnBnClickedBtnGetDepartmentParam();
    afx_msg void OnBnClickedBtnSetDepartmentParam();
    int m_iDepartmentID;
    CString m_csDepartmentName;
    BOOL CreateDepartmentParamXml(char* pBuf, DWORD dwBufLen, int &dwRet);
    CComboBox m_cmbEnable;
};
