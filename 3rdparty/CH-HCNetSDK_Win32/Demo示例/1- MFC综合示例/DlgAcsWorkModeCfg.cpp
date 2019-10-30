// DlgAcsWorkModeCfg.cpp : 实现文件
//

#include "stdafx.h"
#include "ClientDemo.h"
#include "DlgAcsWorkModeCfg.h"
#include "afxdialogex.h"


#define OUT_BUFFER_LEN 1024 * 1024
// CDlgAcsWorkModeCfg 对话框

IMPLEMENT_DYNAMIC(CDlgAcsWorkModeCfg, CDialogEx)

CDlgAcsWorkModeCfg::CDlgAcsWorkModeCfg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgAcsWorkModeCfg::IDD, pParent)
{
    m_pOutBuf = NULL;
}

CDlgAcsWorkModeCfg::~CDlgAcsWorkModeCfg()
{
    if (m_pOutBuf != NULL)
    {
        delete[]m_pOutBuf;
        m_pOutBuf = NULL;
    }
}

void CDlgAcsWorkModeCfg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_WORK_MODE, m_cmbWorkMode);
}


BEGIN_MESSAGE_MAP(CDlgAcsWorkModeCfg, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON_GET, &CDlgAcsWorkModeCfg::OnBnClickedButtonGet)
    ON_BN_CLICKED(IDC_BUTTON_SET, &CDlgAcsWorkModeCfg::OnBnClickedButtonSet)
END_MESSAGE_MAP()


// CDlgAcsWorkModeCfg 消息处理程序

BOOL CDlgAcsWorkModeCfg::CreateWorkModeCfg(char* pBuf, DWORD dwBufLen, int &dwRet)
{
    CXmlBase xmlBase;
    CString strParam = _T("");
    //xmlBase.CreateRoot("IdentityTerminal");
    //xmlBase.SetAttribute("version", "2.0");
    //xmlBase.SetAttribute("xmlns", "http://www.isapi.org/ver20/XMLSchema");
    xmlBase.Parse(m_pOutBuf);
    if (xmlBase.FindElem("IdentityTerminal") && xmlBase.IntoElem())
    {
        if (m_cmbWorkMode.GetCurSel() == 1)
        {
            xmlBase.ModifyData("workMode", "accessControlMode");
        }
        else if (m_cmbWorkMode.GetCurSel() == 2)
        {
            xmlBase.ModifyData("workMode", "passMode");
        }
        xmlBase.OutOfElem();
    }
    return xmlBase.WriteToBuf(pBuf, dwBufLen, dwRet);
}

void CDlgAcsWorkModeCfg::OnBnClickedButtonGet()
{
    // TODO:  在此添加控件通知处理程序代码
    if (m_pOutBuf == NULL)
    {
        return;
    }

    UpdateData(TRUE);
    NET_DVR_XML_CONFIG_INPUT    struInput = { 0 };
    NET_DVR_XML_CONFIG_OUTPUT   struOuput = { 0 };
    struInput.dwSize = sizeof(struInput);
    struOuput.dwSize = sizeof(struOuput);
    char szUrl[256] = { 0 };
    sprintf(szUrl, "GET /ISAPI/AccessControl/IdentityTerminal\r\n");
    struInput.lpRequestUrl = szUrl;
    struInput.dwRequestUrlLen = strlen(szUrl);

    memset(m_pOutBuf, 0, OUT_BUFFER_LEN);
    struOuput.lpOutBuffer = m_pOutBuf;
    struOuput.dwOutBufferSize = OUT_BUFFER_LEN;

    if (!NET_DVR_STDXMLConfig(m_lUserID, &struInput, &struOuput))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, szUrl);
        return;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, szUrl);
    }

    CXmlBase xmlBase;
    xmlBase.Parse(m_pOutBuf);
    if (xmlBase.FindElem("IdentityTerminal") && xmlBase.IntoElem())
    {
        if (xmlBase.FindElem("workMode"))
        {
            if (xmlBase.GetData().compare("passMode") == 0)
            {
                m_cmbWorkMode.SetCurSel(2);
            }
            else if (xmlBase.GetData().compare("accessControlMode") == 0)
            {
                m_cmbWorkMode.SetCurSel(1);
            }
        }
        xmlBase.OutOfElem();
    }

    UpdateData(FALSE);

    return;
}


void CDlgAcsWorkModeCfg::OnBnClickedButtonSet()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);

    NET_DVR_XML_CONFIG_INPUT    struInput = { 0 };
    NET_DVR_XML_CONFIG_OUTPUT   struOuput = { 0 };
    struInput.dwSize = sizeof(struInput);
    struOuput.dwSize = sizeof(struOuput);
    char szUrl[256] = { 0 };
    sprintf(szUrl, "GET /ISAPI/AccessControl/IdentityTerminal\r\n");
    struInput.lpRequestUrl = szUrl;
    struInput.dwRequestUrlLen = strlen(szUrl);

    memset(m_pOutBuf, 0, OUT_BUFFER_LEN);
    struOuput.lpOutBuffer = m_pOutBuf;
    struOuput.dwOutBufferSize = OUT_BUFFER_LEN;

    if (!NET_DVR_STDXMLConfig(m_lUserID, &struInput, &struOuput))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, szUrl);
        return;
    }


    DWORD dwBufLen = 10 * 1024;
    char *pBuf = new char[dwBufLen];
    memset(pBuf, 0, dwBufLen);
    int dwXmlLen = 0;
    if (!CreateWorkModeCfg(pBuf, dwBufLen, dwXmlLen))
    {
        delete[]pBuf;
        pBuf = NULL;
        return;
    }

    char szLan[64] = { 0 };
    g_StringLanType(szLan, "切换工作模式后设备将重启，是否继续?", "Device will reboot after switching work mode,continue?");
    if (IDOK != MessageBox(szLan, "Warning", IDOK))
    {
        delete[]pBuf;
        pBuf = NULL;
        return;
    }

    sprintf(szUrl, "PUT /ISAPI/AccessControl/IdentityTerminal\r\n");
    memset(&struInput, 0, sizeof(struInput));
    memset(&struOuput, 0, sizeof(struOuput));
    struInput.dwSize = sizeof(struInput);
    struOuput.dwSize = sizeof(struOuput);
    struInput.lpRequestUrl = szUrl;
    struInput.dwRequestUrlLen = strlen(szUrl);
    struInput.lpInBuffer = pBuf;
    struInput.dwInBufferSize = dwXmlLen;
    if (!NET_DVR_STDXMLConfig(m_lUserID, &struInput, &struOuput))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, szUrl);
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, szUrl);
    }

    if (pBuf != NULL)
    {
        delete[]pBuf;
        pBuf = NULL;
    }

    UpdateData(FALSE);
}


BOOL CDlgAcsWorkModeCfg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    char szLan[64] = { 0 };

    m_cmbWorkMode.ResetContent();
    g_StringLanType(szLan, "无效", "invalid");
    m_cmbWorkMode.InsertString(0, szLan);
    g_StringLanType(szLan, "门禁模式", "accesscontrol");
    m_cmbWorkMode.InsertString(1, szLan);
    g_StringLanType(szLan, "直通模式", "passthrough");
    m_cmbWorkMode.InsertString(2, szLan);
    m_cmbWorkMode.SetCurSel(0);

    m_pOutBuf = new char[OUT_BUFFER_LEN];

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常:  OCX 属性页应返回 FALSE
}
