// DlgAccessoryCardInfo.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ClientDemo.h"
#include "DlgAccessoryCardInfo.h"
#include "afxdialogex.h"


// CDlgAccessoryCardInfo �Ի���

IMPLEMENT_DYNAMIC(CDlgAccessoryCardInfo, CDialogEx)

CDlgAccessoryCardInfo::CDlgAccessoryCardInfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgAccessoryCardInfo::IDD, pParent)
    , m_szAccessoryCardInfo(_T(""))
{

}

CDlgAccessoryCardInfo::~CDlgAccessoryCardInfo()
{
}

void CDlgAccessoryCardInfo::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_ACCESSORY_CARD_INFO, m_szAccessoryCardInfo);
}


BEGIN_MESSAGE_MAP(CDlgAccessoryCardInfo, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_GET, &CDlgAccessoryCardInfo::OnBnClickedBtnGet)
    ON_BN_CLICKED(IDCANCEL, &CDlgAccessoryCardInfo::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgAccessoryCardInfo ��Ϣ�������


void CDlgAccessoryCardInfo::OnBnClickedBtnGet()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    NET_DVR_ACCESSORY_CARD_INFO struAccessoryCardInfo = { 0 };
    struAccessoryCardInfo.dwSize = sizeof(struAccessoryCardInfo);

    NET_DVR_STD_CONFIG  struConfig = { 0 };
    //     struConfig.lpCondBuffer = NULL;
    //     struConfig.dwCondSize = sizeof(struIDCodeCond);
    struConfig.lpOutBuffer = &struAccessoryCardInfo;
    struConfig.dwOutSize = sizeof(struAccessoryCardInfo);
    if (!NET_DVR_GetSTDConfig(m_lUserID, NET_DVR_GET_ACCESSORY_CARD_INFO, &struConfig))
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_GET_ACCESSORY_CARD_INFO");
    }
    else
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_GET_ACCESSORY_CARD_INFO");
        m_szAccessoryCardInfo = struAccessoryCardInfo.szAccessoryCardInfo;
    }
    UpdateData(FALSE);
}


void CDlgAccessoryCardInfo::OnBnClickedCancel()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CDialogEx::OnCancel();
}
