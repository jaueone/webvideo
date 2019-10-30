// CDlgPreviewOpts.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ClientDemo.h"
#include "DlgPreviewOpts.h"
#include "afxdialogex.h"


// DlgPreviewOpts �Ի���

IMPLEMENT_DYNAMIC(CDlgPreviewOpts, CDialogEx)

CDlgPreviewOpts::CDlgPreviewOpts(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgPreviewOpts::IDD, pParent)
    , m_csURL(_T(""))
    , m_bEnable(FALSE)
{
    m_iDeviceIndex = -1;
}

CDlgPreviewOpts::~CDlgPreviewOpts()
{
}

void CDlgPreviewOpts::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_PREVIEW_URL, m_csURL);
    DDX_Check(pDX, IDC_CHK_ENABLE, m_bEnable);
}


BEGIN_MESSAGE_MAP(CDlgPreviewOpts, CDialogEx)
    ON_BN_CLICKED(IDOK, &CDlgPreviewOpts::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &CDlgPreviewOpts::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgPreviewOpts ��Ϣ�������


void CDlgPreviewOpts::OnBnClickedOk()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);
    CDialogEx::OnOK();
}

BOOL CDlgPreviewOpts::GetEnableUrl()
{
    return m_bEnable;
}

CString & CDlgPreviewOpts::GetUrl()
{
    return m_csURL;
}

void CDlgPreviewOpts::OnBnClickedCancel()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CDialogEx::OnCancel();
}

void CDlgPreviewOpts::SetEnableUrl(BOOL bEnable)
{
    m_bEnable = bEnable;
    return;
}
void CDlgPreviewOpts::SetUrl(CString szUrl)
{
    m_csURL = szUrl;
    return;
}

BOOL CDlgPreviewOpts::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  �ڴ���Ӷ���ĳ�ʼ��
    UpdateData(TRUE);
    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣:  OCX ����ҳӦ���� FALSE
}
