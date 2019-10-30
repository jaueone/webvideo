// DlgXmlSub.cpp : 实现文件
//

#include "stdafx.h"
#include "ClientDemo.h"
#include "DlgXmlSub.h"
#include "afxdialogex.h"
#include "GeneralDef.h"

// CDlgXmlSub 对话框

IMPLEMENT_DYNAMIC(CDlgXmlSub, CDialogEx)

CDlgXmlSub::CDlgXmlSub(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgXmlSub::IDD, pParent)
    , m_strSubEventXml(_T(""))
{

}

CDlgXmlSub::~CDlgXmlSub()
{
}

void CDlgXmlSub::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_SUB_EVENT_XML, m_strSubEventXml);
}


BEGIN_MESSAGE_MAP(CDlgXmlSub, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_CANAEL, &CDlgXmlSub::OnBnClickedBtnCanael)
    ON_BN_CLICKED(IDC_BTN_CONFIRM, &CDlgXmlSub::OnBnClickedBtnConfirm)
END_MESSAGE_MAP()


BOOL CDlgXmlSub::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    isXmlSub = false;

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常:  OCX 属性页应返回 FALSE
}

// CDlgXmlSub 消息处理程序


void CDlgXmlSub::OnBnClickedBtnCanael()
{
    // TODO:  在此添加控件通知处理程序代码
    isXmlSub = false;
    CDialog::OnCancel();
}


void CDlgXmlSub::OnBnClickedBtnConfirm()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);
    strcpy(m_xmlData, m_strSubEventXml.GetBuffer(m_strSubEventXml.GetLength()));
    isXmlSub = true;
    OnCancel();
}

void CDlgXmlSub::getXmlSub(char *m_pSub)
{
    m_xmlData = m_pSub;
}

