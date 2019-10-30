#pragma once
#include "afxcmn.h"
#include "DlgAcsFaceAndTemplate.h"

// CDlgSelectDevice 对话框
extern void CALLBACK g_fSetFaceCallback(DWORD dwType, void* lpBuffer, DWORD dwBufLen, void* pUserData);

class CDlgSelectDevice : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSelectDevice)

public:
	CDlgSelectDevice(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSelectDevice();

// 对话框数据
	enum { IDD = IDD_DLG_ACS_FACE_TEMPLATE_SELECTDEVICE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnExit();
    afx_msg void OnBnClickedBtnConfirm();
    void CreateTree();
    void LoadTreeImage();
    CImageList m_TreeImage;
    virtual BOOL OnInitDialog();
    CTreeCtrl m_treeAll;
    afx_msg void OnNMClickTreeall(NMHDR *pNMHDR, LRESULT *pResult);
    void getDevice(int *m_device);
};
