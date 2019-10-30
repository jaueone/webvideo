// DlgAlarmhostSTDXMLConfig.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ClientDemo.h"
#include "DlgAlarmhostSTDXMLConfig.h"
#include "afxdialogex.h"


// CDlgAlarmhostSTDXMLConfig �Ի���

IMPLEMENT_DYNAMIC(CDlgAlarmhostSTDXMLConfig, CDialog)

CDlgAlarmhostSTDXMLConfig::CDlgAlarmhostSTDXMLConfig(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAlarmhostSTDXMLConfig::IDD, pParent)
    , m_csInputXml(_T(""))
    , m_csOutputXml(_T(""))
{
    memset(m_sCommand, 0, sizeof(m_sCommand));
    m_InPutXmlLen = 0;

    m_pXmlOutput = new char[ISAPI_DATA_LEN];
    memset(m_pXmlOutput, 0, ISAPI_DATA_LEN);
}

CDlgAlarmhostSTDXMLConfig::~CDlgAlarmhostSTDXMLConfig()
{
}

void CDlgAlarmhostSTDXMLConfig::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_CMB_COMMAND, m_cmbCommand);
    DDX_Text(pDX, IDC_EDIT_INPUT_PARAM, m_csInputXml);
    DDX_Text(pDX, IDC_EDIT_OUTPUT_PARAM, m_csOutputXml);
}


BEGIN_MESSAGE_MAP(CDlgAlarmhostSTDXMLConfig, CDialog)
    ON_CBN_SELCHANGE(IDC_CMB_COMMAND, &CDlgAlarmhostSTDXMLConfig::OnCbnSelchangeCmbCommand)
    ON_BN_CLICKED(IDC_BTN_CONFIG, &CDlgAlarmhostSTDXMLConfig::OnBnClickedBtnConfig)
END_MESSAGE_MAP()


// CDlgAlarmhostSTDXMLConfig ��Ϣ�������


BOOL CDlgAlarmhostSTDXMLConfig::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  �ڴ���Ӷ���ĳ�ʼ��
    char szLan[128] = { 0 };

    g_StringLanType(szLan, "��ȡ�豸ϵͳ������", "Get Device System Capabilities");
    m_cmbCommand.AddString(szLan);
    g_StringLanType(szLan, "��ȡ���߱�����������", "Get Wireless Alarm Host Capabilities");
    m_cmbCommand.AddString(szLan);
    g_StringLanType(szLan, "��ȡ��������������", "Get Speaker Config Capabilities");
    m_cmbCommand.AddString(szLan);
    g_StringLanType(szLan, "��ȡ��������������", "Get Speaker Config");
    m_cmbCommand.AddString(szLan);
    g_StringLanType(szLan, "����������������", "Set Speaker Config");
    m_cmbCommand.AddString(szLan);
    g_StringLanType(szLan, "��ȡ��������̽��������", "Get Zone Associated Detector Config Capabilities");
    m_cmbCommand.AddString(szLan);
    g_StringLanType(szLan, "��ȡ��������̽��������", "Get Zone Associated Detector Config");
    m_cmbCommand.AddString(szLan);
    g_StringLanType(szLan, "���÷�������̽��������", "Set Zone Associated Detector Config");
    m_cmbCommand.AddString(szLan);

    m_cmbCommand.SetCurSel(0);

    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣:  OCX ����ҳӦ���� FALSE
}


void CDlgAlarmhostSTDXMLConfig::OnCbnSelchangeCmbCommand()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CXmlBase xmlInputParam;
    CXmlBase xmlOutputParam;
    switch (m_cmbCommand.GetCurSel())
    {
    case 0:  //��ȡ�豸ϵͳ������      
        sprintf(m_sCommand, "GET /ISAPI/System/capabilities\r\n");
        break;
    case 1:  //��ȡ���߱�����������      
        sprintf(m_sCommand, "GET /ISAPI/SecurityCP/WirelessAlarmHostCap\r\n");
        break;
    case 2: //��ȡ��������������
        sprintf(m_sCommand, "GET /ISAPI/SecurityCP/BasicParam/SpeakerCfg/capabilities\r\n");
        break;
    case 3: //��ȡ��������������
        sprintf(m_sCommand, "GET /ISAPI/SecurityCP/BasicParam/SpeakerCfg\r\n");
        break;
    case 4: //������������������
        sprintf(m_sCommand, "POST /ISAPI/SecurityCP/BasicParam/SpeakerCfg\r\n");
        xmlInputParam.Parse("<SpeakerCfg version=\"2.0\" xmlns=\"http://www.isapi.org/ver20/XMLSchema\">\
                           <enabled>ture</enabled>\
                           <volume>6</volume >\
                           </SpeakerCfg>");
        break;
    case 5: //��ȡ��������̽��������
        sprintf(m_sCommand, "GET /ISAPI/SecurityCP/BasicParam/ZoneAssociatedDetectorCfg/capabilities\r\n");
        break;
    case 6: //��ȡ��������̽��������
        sprintf(m_sCommand, "GET /ISAPI/SecurityCP/BasicParam/ZoneAssociatedDetectorCfg\r\n");
        break;
    case 7: //���÷�������̽��������
        sprintf(m_sCommand, "POST /ISAPI/SecurityCP/BasicParam/ZoneAssociatedDetectorCfg\r\n");
        xmlInputParam.Parse("<ZoneCondList version=\"2.0\" xmlns=\"http://www.isapi.org/ver20/XMLSchema\">\
                           <ZoneCond>\
                           <id>0</id>\
                           </ZoneCond>\
                           </ZoneCondList>");
        break;
    default:
        break;
    }

    xmlInputParam.SetRoot();
    m_csInputXml = xmlInputParam.GetChildren().c_str();
    m_csInputXml.Replace("\n", "\r\n");

    xmlOutputParam.SetRoot();
    m_csOutputXml = xmlOutputParam.GetChildren().c_str();
    m_csOutputXml.Replace("\n", "\r\n");

    m_InPutXmlLen = m_csInputXml.GetLength();
    UpdateData(FALSE);
}


void CDlgAlarmhostSTDXMLConfig::OnBnClickedBtnConfig()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);

//     char* pXmlInput = NULL;
//     DWORD dwInSize = 0;
//     if (!ConvertScreenServerLoginParamStruToXml(&m_struLoginCfg, &pXmlInput, dwInSize))
//     {
//         return;
//     }

    char *pInputXml = NULL;
    pInputXml = new char[m_InPutXmlLen + 1];
    memset(pInputXml, 0, m_InPutXmlLen + 1);
    memcpy(pInputXml, m_csInputXml.GetBuffer(0), m_InPutXmlLen);

    NET_DVR_XML_CONFIG_INPUT struInput = { 0 };
    struInput.lpRequestUrl = m_sCommand;
    struInput.lpInBuffer = pInputXml;
    struInput.dwInBufferSize = m_InPutXmlLen;
    NET_DVR_XML_CONFIG_OUTPUT struOutput = { 0 };
    char sStatusBuf[1024] = { 0 };
    struOutput.lpOutBuffer = m_pXmlOutput;
    struOutput.dwOutBufferSize = ISAPI_DATA_LEN;
    struOutput.lpStatusBuffer = sStatusBuf;
    struOutput.dwStatusSize = sizeof(sStatusBuf);
    if (!NET_DVR_STDXMLConfig(m_lUserID, &struInput, &struOutput))
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_STDXMLConfig");
        return;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_STDXMLConfig");
    }

    m_csOutputXml = m_pXmlOutput;

    UpdateData(FALSE);
}
