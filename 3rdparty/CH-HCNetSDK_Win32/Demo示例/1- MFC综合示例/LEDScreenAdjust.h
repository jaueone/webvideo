#pragma once
#include "afxwin.h"
#include "afxcmn.h"

// LEDScreenAdjust �Ի���

class LEDScreenAdjust : public CDialog
{
	DECLARE_DYNAMIC(LEDScreenAdjust)

public:
	LEDScreenAdjust(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~LEDScreenAdjust();
	virtual BOOL OnInitDialog();

// �Ի�������
	enum { IDD = IDD_DLG_SCREENADJUST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_CombOperateType;
	CComboBox m_CombRGBSync;
	long m_lHandle;
	long m_lUserID;
	long m_iDeviceIndex;
	long m_lRgbPermil;
	long m_lRedPermil;
	long m_lGreenPermil;
	long m_lBluePermil;
	BOOL m_bFreshed;
	BOOL  m_needSave; //���ڱ����־,����й���д��ΪTURE,���´α༭ʱ����ԭ����λ������ԭ�����������
	CEdit m_Edit;
	int m_Item;  //��
	int m_SubItem; //��
	int m_rowCount;  //������
	int m_rowCountNew; //ʵ����Ч����
	HWND m_hWnd;	//���ھ��
	NET_SDK_LED_SCREEN_CHECK_PARAM m_struScreenCheck;
	CListCtrl m_List_ScreenArea;
	CProgressCtrl m_Progress_ScreenAdjust;
	void InitList();
	void OnFreshList();
	void OnScreenAdjust(DWORD dwType, void* lpBuffer, DWORD dwBufLen);
	void Init(LONG lUserID, int iDeviceIndex){ m_lUserID = lUserID; m_iDeviceIndex = iDeviceIndex; };
	afx_msg LRESULT OnScreenAdjustMessageSucc(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnScreenAdjustMessageFail(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnScreenAdjustMessageFresh(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedBtnOperate();	
	afx_msg void OnNMClickListScreenarea(NMHDR *pNMHDR, LRESULT *pResult);
};
