// DlgXmlSub.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ClientDemo.h"
#include "DlgXmlSub.h"
#include "afxdialogex.h"
#include "GeneralDef.h"

// CDlgXmlSub �Ի���

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

    // TODO:  �ڴ���Ӷ���ĳ�ʼ��
    isXmlSub = false;

    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣:  OCX ����ҳӦ���� FALSE
}

// CDlgXmlSub ��Ϣ�������


void CDlgXmlSub::OnBnClickedBtnCanael()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    isXmlSub = false;
    CDialog::OnCancel();
}


void CDlgXmlSub::OnBnClickedBtnConfirm()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);
    strcpy(m_xmlData, m_strSubEventXml.GetBuffer(m_strSubEventXml.GetLength()));
    isXmlSub = true;
    OnCancel();
}

void CDlgXmlSub::getXmlSub(char *m_pSub)
{
    m_xmlData = m_pSub;
}

