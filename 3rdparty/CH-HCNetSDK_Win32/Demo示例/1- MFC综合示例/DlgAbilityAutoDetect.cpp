// DlgAbilityAutoDetect.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgAbilityAutoDetect.h"


#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAbilityAutoDetect dialog
#define XML_ABILITY_IN_LEN	1024
#define XML_ABILITY_OUT_LEN	3*1024*1024

CDlgAbilityAutoDetect::CDlgAbilityAutoDetect(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAbilityAutoDetect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAbilityAutoDetect)
	m_strDevAbility = _T("");
	m_strLostAbility = _T("");
	m_dwLastError = 0;
	m_dwReturnValue = 0;
	m_lServerID = -1;
	m_pOutBuf = NULL;
	m_dwAbilityType = 0;
	m_strInputParam = _T("");
    m_bSTDAbility = FALSE;
	//}}AFX_DATA_INIT
	memset(&m_struSdkLocalCfg, 0, sizeof(m_struSdkLocalCfg));
}


void CDlgAbilityAutoDetect::DoDataExchange(CDataExchange* pDX)
{

    CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAbilityAutoDetect)
	DDX_Control(pDX, IDC_CMB_STREAM_TYPE, m_cmbStreamType);
	DDX_Control(pDX, IDC_COMBO_CHANNEL, m_cmbChannel);
	DDX_Control(pDX, IDC_CMB_ABILITY_TYPE, m_cmbAbilityType);
	DDX_Text(pDX, IDC_EDT_ABILITY_SHOW, m_strDevAbility);
	DDX_Text(pDX, IDC_EDT_ABILITY_LOST, m_strLostAbility);
	DDX_Text(pDX, IDC_EDT_LAST_ERROR, m_dwLastError);
	DDX_Text(pDX, IDC_EDT_RETURN_VALUE, m_dwReturnValue);
	DDX_Text(pDX, IDC_EDT_INPUT_PARAM, m_strInputParam);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAbilityAutoDetect, CDialog)
	//{{AFX_MSG_MAP(CDlgAbilityAutoDetect)
	ON_BN_CLICKED(IDC_BTN_USE_SIM_ABILITY, OnBtnUseSimAbility)
	ON_BN_CLICKED(IDC_BTN_GET, OnBtnGet)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_CMB_ABILITY_TYPE, OnSelchangeCmbAbilityType)
	//}}AFX_MSG_MAP
    ON_CBN_EDITCHANGE(IDC_CMB_ABILITY_TYPE, &CDlgAbilityAutoDetect::OnCbnEditchangeCmbAbilityType)
    ON_CBN_SETFOCUS(IDC_CMB_ABILITY_TYPE, &CDlgAbilityAutoDetect::OnCbnSetfocusCmbAbilityType)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAbilityAutoDetect message handlers

void CDlgAbilityAutoDetect::OnBtnUseSimAbility() 
{
	// TODO: Add your control notification handler code here
	memset(&m_struSdkLocalCfg, 0 ,sizeof(m_struSdkLocalCfg));
	NET_DVR_GetSDKLocalConfig(&m_struSdkLocalCfg);
	
	if (1 == m_struSdkLocalCfg.byEnableAbilityParse)
	{
		m_struSdkLocalCfg.byEnableAbilityParse = 0;
		GetDlgItem(IDC_BTN_USE_SIM_ABILITY)->SetWindowText("��ʹ��ģ������");
	} 
	else
	{
		m_struSdkLocalCfg.byEnableAbilityParse = 1;
		GetDlgItem(IDC_BTN_USE_SIM_ABILITY)->SetWindowText("ʹ��ģ������");
	}
	
	if (!NET_DVR_SetSDKLocalConfig(&m_struSdkLocalCfg))
	{
		AfxMessageBox("fail!");
	}
}

void CDlgAbilityAutoDetect::OnBtnGet() 
{
	// TODO: Add your control notification handler code here
    GetDlgItem(IDC_EDT_ABILITY_SHOW)->SetWindowText("");
    GetDlgItem(IDC_EDT_ABILITY_LOST)->SetWindowText("");
    UpdateData(TRUE);
    if (m_dwAbilityType == 0)
    {
        AfxMessageBox("��ѡ����ȷ������������!");
        return ;
    }
    if (m_bSTDAbility == 1)
    {
        int nID = 0;
        NET_DVR_STD_ABILITY struSTDAbility = {0};
        NET_DVR_REGION_CLIP_COND struCond = {0};
        if(m_dwAbilityType == NET_DVR_GET_REGIONCLIP_CAPABILITIES)
        {
            struCond.dwSize = sizeof(struCond);
            struCond.dwChannel = m_cmbChannel.GetCurSel() + 1;
            struCond.dwStreamType = m_cmbStreamType.GetCurSel();
            struSTDAbility.lpCondBuffer = &struCond;
            struSTDAbility.dwCondSize = sizeof(struCond);
        }
		else if(m_dwAbilityType == NET_DVR_GET_RTMP_CFG_CAP)
        {
			NET_DVR_RTMP_COND struRTMPCond = {0};
            struRTMPCond.dwSize = sizeof(struRTMPCond);
            struRTMPCond.dwChannel = m_cmbChannel.GetCurSel() + 1;
            struRTMPCond.byStreamType = m_cmbStreamType.GetCurSel() + 1;
            struSTDAbility.lpCondBuffer = &struRTMPCond;
            struSTDAbility.dwCondSize = sizeof(struRTMPCond);
        }
        else if (m_dwAbilityType == NET_DVR_GET_INPUT_SOURCE_TEXT_CAP)
        {
            NET_DVR_INPUT_SOURCE_TEXT_COND struCond = { 0 };
            struCond.dwSize = sizeof(struCond);
            if (m_strInputParam.GetLength() != 0)
            {
                struCond.dwInputSourceNo = atoi(m_strInputParam.GetBuffer(0));
                m_strInputParam.ReleaseBuffer();
            }
            struSTDAbility.lpCondBuffer = &struCond;
            struSTDAbility.dwCondSize = sizeof(struCond);
        }
        else if (m_dwAbilityType == NET_DVR_GET_FACELIB_SCHEDULE_CAPABILITIES)
        {
            NET_DVR_FACELIB_GUARD_COND strFaceLibGuard = { 0 };
            strFaceLibGuard.dwSize = sizeof(strFaceLibGuard);
            strFaceLibGuard.dwChannel = m_cmbChannel.GetCurSel() + 1;;
            strncpy(strFaceLibGuard.szFDID, m_strInputParam.GetBuffer(m_strInputParam.GetLength()), 68);

            struSTDAbility.lpCondBuffer = &strFaceLibGuard;
            struSTDAbility.dwCondSize = sizeof(strFaceLibGuard);
        }
        else
        {
            if (m_strInputParam != "")
            {
                nID = atoi(m_strInputParam.GetBuffer(0));
                m_strInputParam.ReleaseBuffer();
            }
            struSTDAbility.lpCondBuffer = &nID;
            struSTDAbility.dwCondSize = sizeof(int);
        }        
        
        struSTDAbility.lpOutBuffer = m_pOutBuf;
        struSTDAbility.dwOutSize = XML_ABILITY_OUT_LEN;
        struSTDAbility.lpStatusBuffer = m_pOutBuf;
        struSTDAbility.dwStatusSize = XML_ABILITY_OUT_LEN;
        m_dwReturnValue = NET_DVR_GetSTDAbility(m_lServerID, m_dwAbilityType, &struSTDAbility);
    }
    else if (m_bSTDAbility == 2)
    {
        NET_DVR_XML_CONFIG_INPUT    struInput = {0};
        NET_DVR_XML_CONFIG_OUTPUT   struOuput = {0};
        struInput.dwSize = sizeof(struInput);
		struOuput.dwSize = sizeof(struOuput);
        struInput.lpRequestUrl = m_strInputParam.GetBuffer(0);
        struInput.dwRequestUrlLen = m_strInputParam.GetLength();
        struOuput.dwSize = sizeof(struOuput);
		memset(m_pOutBuf, 0, XML_ABILITY_OUT_LEN);
        struOuput.dwSize = sizeof(NET_DVR_XML_CONFIG_OUTPUT);
        struOuput.lpOutBuffer = m_pOutBuf;
        struOuput.dwOutBufferSize = XML_ABILITY_OUT_LEN;
        
        m_dwReturnValue = NET_DVR_STDXMLConfig(m_lServerID, &struInput, &struOuput);
    }
    else
    {
        // �ӿ��ڲ������ϸ�У��������˴����Ӵ���
        char *szInput = m_strInputParam.GetBuffer(0);
        if (m_strInputParam.GetLength() == 0)
        {
            szInput = NULL;
        }

        m_dwReturnValue = NET_DVR_GetDeviceAbility(m_lServerID, m_dwAbilityType, szInput, m_strInputParam.GetLength(), m_pOutBuf, XML_ABILITY_OUT_LEN);
        m_strInputParam.ReleaseBuffer();
    }	
    m_dwLastError = NET_DVR_GetLastError();
    if (m_dwReturnValue != 0)
    {
        CXmlBase xmlBase;
        xmlBase.Parse(m_pOutBuf);
        xmlBase.SetRoot();
        //m_strDevAbility = xmlBase.GetChildren().c_str();
        m_strDevAbility = m_pOutBuf;
        m_strDevAbility.Replace("\n", "\r\n");
        if (!m_bSTDAbility)
        {
            ParseAbility();
        }		
    }
    
	UpdateData(FALSE);
}

void CDlgAbilityAutoDetect::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if (m_pOutBuf != NULL)
	{
		delete []m_pOutBuf;
		m_pOutBuf = NULL;
	}
}

BOOL CDlgAbilityAutoDetect::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	m_pOutBuf = new char[XML_ABILITY_OUT_LEN];
	memset(m_pOutBuf, 0, XML_ABILITY_OUT_LEN);
    m_cmbChannel.SetCurSel(0);
    m_cmbStreamType.SetCurSel(2);

    //ֻ��ȡ�ճ�ʼ��ʱ��������б�
    int iCount = ((CComboBox*)GetDlgItem(IDC_CMB_ABILITY_TYPE))->GetCount();
    for (int i = 0; i < iCount; ++i)
    {
        CString cstr;
        m_cmbAbilityType.GetLBText(i, cstr);
        m_vecAbilityType.push_back(cstr);
        m_vecCurAbilityType.push_back(cstr);
    }

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAbilityAutoDetect::OnSelchangeCmbAbilityType() 
{
	// TODO: Add your control notification handler code here
	CXmlBase xmlInput;
    m_bSTDAbility = FALSE;
    GetDlgItem(IDC_STATIC_CHANNEL)->ShowWindow(FALSE);
    GetDlgItem(IDC_STATIC_STREAM_TYPE)->ShowWindow(FALSE);
    GetDlgItem(IDC_COMBO_CHANNEL)->ShowWindow(FALSE);
    GetDlgItem(IDC_CMB_STREAM_TYPE)->ShowWindow(FALSE);

    int iIndex = GetSelectedAbilityIndex();
    if (iIndex < 0)
    {
        return;
    }
    switch (iIndex)
	{
	case 0:	//��Ӳ������
		m_dwAbilityType = DEVICE_SOFTHARDWARE_ABILITY;
		break;
	case 1:	//Wifi����
		m_dwAbilityType = DEVICE_NETWORK_ABILITY;
		break;
	case 2: //��������
		m_dwAbilityType = DEVICE_ENCODE_ALL_ABILITY;
		break;
	case 3:	//��ǰ��������
		m_dwAbilityType = DEVICE_ENCODE_CURRENT;
		xmlInput.Parse("<CurrentCompressInfo><ChannelNumber>1</ChannelNumber><VideoEncodeType>0</VideoEncodeType><VideoResolution>3</VideoResolution></CurrentCompressInfo>");
		break;
	case 4:	//ǰ�˲�������
		m_dwAbilityType = IPC_FRONT_PARAMETER;
		break;
	case 5:	//��������
		m_dwAbilityType = FISHEYE_ABILITY;
		break;
	case 6:	//Raid����
		m_dwAbilityType = DEVICE_RAID_ABILITY;
		break;
	case 7: //��������2.0
		m_dwAbilityType = DEVICE_ENCODE_ALL_ABILITY_V20;
		xmlInput.Parse("<AudioVideoCompressInfo><AudioChannelNumber>1</AudioChannelNumber><VoiceTalkChannelNumber>1</VoiceTalkChannelNumber><VideoChannelNumber>1</VideoChannelNumber></AudioVideoCompressInfo>");
		break;
	case 8: //ǰ�β���2.0
		m_dwAbilityType = IPC_FRONT_PARAMETER_V20;
		xmlInput.Parse("<CAMERAPARA><ChannelNumber>1</ChannelNumber></CAMERAPARA>");
		break;
	case 9: //����������
		m_dwAbilityType = DECODER_ABILITY;
		xmlInput.Parse("<DecoderAbility version='2.0'></DecoderAbility>");
		break;
	case 10: //�û���������
		m_dwAbilityType = DEVICE_USER_ABILITY;
		xmlInput.Parse("<UserAbility version='2.0'></UserAbility>");
		break;
	case 11: //����Ӧ������
        m_dwAbilityType = DEVICE_NETAPP_ABILITY;
        xmlInput.Parse("<NetAppAbility version='2.0'></NetAppAbility>");
		break;
	case 12: //��Ƶͼ������
		m_dwAbilityType = DEVICE_VIDEOPIC_ABILITY;
		xmlInput.Parse("<VideoPicAbility version='2.0'>	<channelNO>1</channelNO></VideoPicAbility>");
		break;
	case 13: //JPEGץͼ����
		m_dwAbilityType = DEVICE_JPEG_CAP_ABILITY;
		xmlInput.Parse("<JpegCaptureAbility version='2.0'><channelNO>1</channelNO></JpegCaptureAbility>");
		break;
	case 14: //��������
		m_dwAbilityType = DEVICE_SERIAL_ABILITY;
		xmlInput.Parse("<SerialAbility version='2.0'></SerialAbility>");
		break;
	case 15: //��Ƶ�ۺ�ƽ̨����
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		break;
	case 16: //������ϵͳ����
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		break;
	case 17: //������ϵͳ����
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		break;
	case 18: //����������ϵͳ
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		break;
	case 19: //���������ϵͳ
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		break;
	case 20: //�����ϵͳ
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		break;
	case 21: //���籨������
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<AlarmHostAbility version=\"2.0\"></AlarmHostAbility>");
		break;
	case 22: //ƽ̨������ϵͳ
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		break;
	case 23: //¼���������
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<RecordAbility version='2.0'></RecordAbility>");
		break;
	case 24: //�豸����ͨ������
		m_dwAbilityType = DEVICE_DYNCHAN_ABILITY;
		xmlInput.Parse("<DynChannelAbility version='2.0'><channelNO>1</channelNO></DynChanAbility>");
		break;
	case 25: //ͨ����������
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<ChannelInputAbility version='2.0'><channelNO>1</channelNO></ChannelInputAbility>");
		break;
	case 26: //��Ѷ������
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<InquestAbility version='2.0'></InquestAbility>");
		break;
	case 27: //CVR����
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<CVRAbility version='2.0'></CVRAbility>");
		break;
	case 28: //�豸�¼�����
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<EventAbility version='2.0'><channelNO>1</channelNO></EventAbility>");
		break;
	case 29: //ǰ�˽���ͨ������
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<GetAccessDeviceChannelAbility version='2.0'></GetAccessDeviceChannelAbility>");
		break;
	case 30: //����Ԥ���л�����
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<PreviewSwitchAbility version='2.0'></PreviewSwitchAbility>");
		break;
	case 31: //ROI����
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<ROIAbility version='2.0'><channelNO>1</channelNO></ROIAbility>");
		break;
	case 32: //��̨����
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<PTZAbility><channelNO>1</channelNO></PTZAbility>");
		break;
	case 33: //VQD����
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<VQDAbility version='2.0'><channelNO>1</channelNO></VQDAbility>");
		break;
	case 34://���ܽ�ͨ����
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<ITDeviceAbility version='2.0'><channelNO>1</channelNO></ITDeviceAbility>");
		break;
	case 35: //N+1����
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<NPlusOneAbility  version='2.0'></NPlusOneAbility >");
		break;
	case 36://�����������
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<HardDiskAbility version='2.0'></HardDiskAbility>");
		break;
	case 37://�豸��������
		m_dwAbilityType = DEVICE_ALARM_ABILITY;
		xmlInput.Parse("<AlarmAbility version='2.0'><channelID>1</channelID></AlarmAbility>");
		break;
	case 38://IPC�����ļ����뵼������
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<IPAccessConfigFileAbility version='2.0'></IPAccessConfigFileAbility>");
		break;
	case 39://ǰ�˶�̬������ȡ���ع�ʱ�䣩
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<CameraParaDynamicAbility version='2.0'><channelNO>1</channelNO><ExposureSetDynamicLinkTo><WDR><WDREnable>0</WDREnable></WDR><IrisMode><IrisType>0</IrisType></IrisMode></ExposureSetDynamicLinkTo></CameraParaDynamicAbility>");
		break;
	case 40://ǰ�˶�̬������ȡ��CaptureMode��
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<CameraParaDynamicAbility version='2.0'><channelNO>1</channelNO><AudioVideoCompressInfoDynamicLinkTo><captureMode opt='640*480@30fps'/></AudioVideoCompressInfoDynamicLinkTo></CameraParaDynamicAbility>");
		break;
	case 41://GBT28181Э��
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<GBT28181AccessAbility version='2.0'><channelNO>1</channelNO></GBT28181AccessAbility>");
		break;
	case 42://��־��������
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<SearchLogAbility version='2.0'><channelNO>1</channelNO></SearchLogAbility>");
		break;
	case 43://��������¼��
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<AlarmTriggerRecordAbility version='2.0'><channelNO>1</channelNO></AlarmTriggerRecordAbility>");
		break;
	case 44://IP���ӶԽ��ֻ�����
		m_dwAbilityType = IP_VIEW_DEV_ABILITY;
		break;
    case 45://��������
        m_dwAbilityType = MATRIX_ABILITY; 
        xmlInput.Parse("<MatrixAbility version='2.0'></MatrixAbility>"); 
        break; 
	case 46:
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<IOAbility version='2.0'><channelNO>1</channelNO></IOAbility>");
		break;
	case 47:
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<AccessProtocolAbility version='2.0'><channelNO>1</channelNO></AccessProtocolAbility>");
		break;
	case 48:
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<VcaDevAbility version='2.0'></VcaDevAbility>");
		break;
	case 49:
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<VcaCtrlAbility  version='2.0'><channelNO>1</channelNO></VcaCtrlAbility>");
		break;
	case 50:
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<VcaChanAbility  version='2.0'><channelNO>1</channelNO></VcaChanAbility>");
		break;
	case 51:
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<CameraMountAbility  version='2.0'><channelNO>1</channelNO></CameraMountAbility>");
		break;
	case 52:
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<RecordingHostAbility version='2.0'></RecordingHostAbility>");
		break;
	case 53://˫Ŀ����
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<BinocularAbility  version='2.0'><channelNO>1</channelNO></BinocularAbility>");
		break;
    case 54: //Smart����
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_SMART_CAPABILITIES;
        break;
    case 55: //�¼���������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_EVENT_TRIGGERS_CAPABILITIES;
        break;
    case 56: //���������������
        m_bSTDAbility = TRUE;    
        m_dwAbilityType = NET_DVR_GET_REGION_ENTRANCE_CAPABILITIES;
        m_strInputParam = "1";
        break;    
    case 57: //����������Ⲽ��ʱ������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_REGION_ENTRANCE_SCHEDULE_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 58: //�뿪�����������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_REGION_EXITINT_CAPABILITIES;
        m_strInputParam = "1";
        break;    
    case 59: //�뿪������Ⲽ��ʱ������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_REGION_EXITING_SCHEDULE_CAPABILITIES;
        m_strInputParam = "1";
        break;    
    case 60: //�ǻ��������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_LOITERING_CAPABILITIES;
        m_strInputParam = "1";
        break;                
    case 61: //�ǻ���Ⲽ��ʱ������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_LOITERING_SCHEDULE_CAPABILITIES;
        m_strInputParam = "1";
        break;                            
    case 62: //��Ա�ۼ��������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_GROUPDETECTION_CAPABILITIES;
        m_strInputParam = "1";
        break;                                        
    case 63: //��Ա�ۼ���Ⲽ��ʱ������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_GROUP_SCHEDULE_CAPABILITIES;
        m_strInputParam = "1";
        break;                                                    
    case 64: //�����˶��������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_RAPIDMOVE_CAPABILITIES;
        m_strInputParam = "1";
        break;                                                                
    case 65: //�����˶���Ⲽ��ʱ������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_RAPIDMOVE_SCHEDULE_CAPABILITIES;
        m_strInputParam = "1";
        break;                                                                            
    case 66: //ͣ���������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_PATKING_CAPABILITIES;
        m_strInputParam = "1";
        break;                                                                                        
    case 67: //ͣ����Ⲽ��ʱ������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_PARKING_SCHEDULE_CAPABILITIES;
        m_strInputParam = "1";
        break;                                                                                                    
    case 68: //��Ʒ�����������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_UNATTENDED_BAGGAGE_CAPABILITIES;
        m_strInputParam = "1";
        break;                                                                                                                
    case 69: //��Ʒ������Ⲽ��ʱ������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_UNATTENDEDBAGGAGE_SCHEDULE_CAPABILITIES;
        m_strInputParam = "1";
        break;            
    case 70: //��Ʒ��ȡ�������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_ATTENDEDBAGGAGE_CAPABILITIES;
        m_strInputParam = "1";
        break;                        
    case 71: //��Ʒ��ȡ��Ⲽ��ʱ������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_ATTENDEDBAGGAGE_SCHEDULE_CAPABILITIES;
        m_strInputParam = "1";
        break;            
    case 72: //����ü�����
        GetDlgItem(IDC_STATIC_CHANNEL)->ShowWindow(TRUE);
        GetDlgItem(IDC_STATIC_STREAM_TYPE)->ShowWindow(TRUE);
        GetDlgItem(IDC_COMBO_CHANNEL)->ShowWindow(TRUE);
        GetDlgItem(IDC_CMB_STREAM_TYPE)->ShowWindow(TRUE);
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_REGIONCLIP_CAPABILITIES;
        break;                        
    case 73: //Network����
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_NETWORK_CAPABILITIES;
        break;            
    case 74: //���߲��Ų�������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_WIRELESSDIAL_CAPABILITIES;
        m_strInputParam = "1";
        break;                        
    case 75: //���żƻ�����
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_WIRELESSDIAL_SCHEDULE_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 76:
        m_dwAbilityType = ACS_ABILITY;
        xmlInput.Parse("<AcsAbility version='2.0'></AcsAbility>");
		break;
    case 77:
        m_dwAbilityType = DEVICE_ABILITY_INFO;
        xmlInput.Parse("<ImageDisplayParamAbility version='2.0'><channelNO>1,2</channelNO></ImageDisplayParamAbility>");
		break;
	case 78:
		m_dwAbilityType = MERGEDEV_ABILITY;
		break; 
    case 79: //���������������
        m_bSTDAbility = TRUE;    
        m_dwAbilityType = NET_DVR_GET_REGION_ENTRANCE_CAPABILITIES;
        m_strInputParam = "1";
        break; 
    case 80:
        m_dwAbilityType = DEVICE_ABILITY_INFO;
        xmlInput.Parse("<SecurityAbility version='2.0'><channelNO>1</channelNO></SecurityAbility>");
		break;
    case 81:
        m_bSTDAbility = TRUE;    
        m_dwAbilityType = NET_DVR_GET_LITESTORAGE_CAPABILITIES;
        m_strInputParam = "1";
        break; 
    case 82:
        m_bSTDAbility = TRUE;    
        m_dwAbilityType = NET_DVR_GET_VEHICLE_CAPABILITIES;
        m_strInputParam = "1";
        break; 
    case 83:	//IPC��������
		m_dwAbilityType = IPC_UPGRADE_DESCRIPTION;
        break;
	case 84:
        m_bSTDAbility = TRUE;    
        m_dwAbilityType = NET_DVR_GET_SLAVECAMERA_CAPABILITIES;
        m_strInputParam = "1";
        break; 
    case 85:
        m_bSTDAbility = TRUE;    
        m_dwAbilityType = NET_DVR_GET_SLAVECAMERA_CALIB_CAPABILITIES;
        m_strInputParam = "1";
        break; 
	case 86:
        m_bSTDAbility = TRUE;    
        m_dwAbilityType = NET_DVR_GET_TRACKING_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 87:
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_MASTERSLAVETRACKING_CAPABILITIES;
        break;
    case 88:
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_DDNS_COUNTRY_ABILITY;
        break;
    case 89:
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_FIREDETECTION_CAPABILITIES;
        m_strInputParam = "2";
        break;
    case 90:
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_THERMAL_CAPABILITIES;
        //m_strInputParam = "";
        break;
    case 91:
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_SENSOR_PORT_CAPABILITIES;
        //m_strInputParam = "";
        break;
    case 92://NVR����IPC����
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_ACTIVATE_IPC_ABILITY;
        break;
    case 93://���в�������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_CENTRALIZEDCTRL_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 94://������������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_COMPASS_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 95://��Ƶ������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_STREAMING_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 96://��Ƶˢ��֡����
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_REFRESHFRAME_CAPABILITIES;
        m_strInputParam = "1";
        break;
	case 97:
		m_dwAbilityType = STREAM_ABILITY;
		xmlInput.Parse("<StreamAbility version='2.0'></StreamAbility>");
		break;
	case 98:
		m_dwAbilityType = ACS_ABILITY;
		xmlInput.Parse("<AcsAbility version='2.0'></AcsAbility>");
		break;
	case 99:
		m_bSTDAbility = TRUE;
		m_dwAbilityType = NET_DVR_GET_GBT28181_SERVICE_CAPABILITIES;
		break;
    case 100:
        m_dwAbilityType = DEVICE_ABILITY_INFO;
        xmlInput.Parse("<POSAbility  version='2.0'></POSAbility>");
		break;
    case 101: //�ȵ㹦����������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_WIRELESSSERVER_CAPABILITIES;
        m_strInputParam = "1";
		break;
    case 102: //�����豸�б�
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_CONNECT_LIST_CAPABILITIES;
        m_strInputParam = "1";
		break;
    case 103:
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_RECORDING_PUBLISH_FILE_CAP;
        m_strInputParam = "1";
		break;
    case 104:
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_RECORD_VIDEO_CFG_CAP;
        m_strInputParam = "1";
		break;
    case 105:
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_RECORD_HOST_CAP;
		break;
    case 106://��������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_EXTERNALDEVICE_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 107://���ò��������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_SUPPLEMENTLIGHT_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 108:
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_LOWPOWER_CAPABILITIES;
		m_strInputParam = "1";
        break;
    case 109:
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_ZOOMLINKAGE_CAPABILITIES;
		m_strInputParam = "1";
        break;
    case 110:
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_ONLINEUPGRADE_ABILITY;
        break;
	case 111: //��ص�����ʾ
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_OSD_BATTERY_POWER_CFG_CAPABILITIES;
        m_strInputParam = "1";
		break;
	case 112://�ն˻�����������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_CONFERENCE_REGION_CAP;
        break;
    case 113://�ն˺�����������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_TERMINAL_CALL_CFG_CAP;
        break;
    case 114://�ն˺��п�������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_TERMINAL_CTRL_CAP;
        break;
    case 115://���в�ѯ����
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_CALL_QUERY_CAP;
        break;
    case 116://VCS����
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_VCS_CAP;
        break;
	case 117://���������������
		m_bSTDAbility = TRUE;
		m_dwAbilityType = NET_DVR_GET_TERMINAL_INPUT_CFG_CAP;
		break;
	case 118://��ʪ������Э�������
		m_bSTDAbility = TRUE;
		m_dwAbilityType = NET_DVR_GET_THSCREEN_CAPABILITIES;
		break;
    case 119://IPCȫ�����ͼƬ��������
        m_bSTDAbility = TRUE;
		m_dwAbilityType = NET_DVR_GET_PANORAMAIMAGE_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 120://��������
        m_bSTDAbility = TRUE;
		m_dwAbilityType = NET_DVR_GET_STREAMENCRYPTION_CAPABILITIES;
        break;
	case 121://У׼GPS��γ������
        m_bSTDAbility = TRUE;
		m_dwAbilityType = NET_DVR_GET_REVISE_GPS_CAPABILITIES;
		m_strInputParam = "1";
        break;
    case 122:
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_TME_CHARGERULE_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 123:
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_PAPERCHARGEINFO_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 124:
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_PARKINGSAPCE_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 125:
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_PXOFFLINE_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 126:
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_PXMULTICTRL_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 127:
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_ILLEGALCARDFILTERING_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 128:
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_CHARGEACCOUNT_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 129:
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_TME_CAPABILITIES;
		// m_strInputParam = "1";
        break;
    case 130:
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_LEDDISPLAY_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 131:
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_VOICEBROADCAST_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 132:
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_PAPERPRINTFORMAT_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 133:
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_LOCkGATE_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 134://��ȡ������ͳ������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_COUNTING_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 135: //EPTZ��������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_EPTZ_CFG_CAPABILITIES;
		break;
    case 136: //���ĵ��������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_CENTER_POINT_CFG_CAPABILITIES;
		break;
    case 137: //STD���۲�������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_FISHEYE_CAPABILITIES;
		break;
	case 138://RTMP��������
		GetDlgItem(IDC_STATIC_CHANNEL)->ShowWindow(TRUE);
        GetDlgItem(IDC_STATIC_STREAM_TYPE)->ShowWindow(TRUE);
        GetDlgItem(IDC_COMBO_CHANNEL)->ShowWindow(TRUE);
        GetDlgItem(IDC_CMB_STREAM_TYPE)->ShowWindow(TRUE);
		m_cmbStreamType.SetCurSel(0);
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_RTMP_CFG_CAP;
        m_strInputParam = "1";
        break;
    case 139://ƽ�����ʶ�̬����
        m_dwAbilityType = DEVICE_ABILITY_INFO;
        xmlInput.Parse("<CameraParaDynamicAbility version='2.0'><channelNO>1</channelNO><VbrAverageCapDynamicLinkTo><streamType>main</streamType><codeType>smart264</codeType><videoQualityControlType>CBR</videoQualityControlType><vbrUpperCap>512</vbrUpperCap></VbrAverageCapDynamicLinkTo></CameraParaDynamicAbility>");
		break;
    case 140://��Ӱģʽ��������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_FILM_MODE_CFG_CAP;
        break;
    case 141://����������������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_DIRECTED_STRATEGY_CFG_CAP;
        break;
    case 142://����߿���������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_FRAME_CFG_CAP;
        m_strInputParam = "1";
        break;
    case 143://��ƵЧ���Ż���������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_AUDIO_EFFECTIVE_CFG_CAP;
        m_strInputParam = "1";
        break;
    case 144://¼����Ƶ������������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_RECORD_VIDEO_CFG_CAP;
        break;
    case 145://����ͼƬ��Ϊ����ͼƬ��������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_BACKGROUND_PIC_INFO_CAP;
        break;
	case 146:
		m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_LLDP_CAP;
		m_strInputParam = "1";
		break;
    case 147:
		m_bSTDAbility = TRUE;
		m_dwAbilityType = NET_DVR_GET_PORT_REMARKS_CAP;
		m_strInputParam = "1";
		break;
    case 148://�ƴ洢������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿ�,NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/ContentMgmt/CloudStorageServer/capabilities";
        break;
	case 149://LED����
		m_dwAbilityType = LED_ABILITY;
		xmlInput.Parse("<LedAbility  version='2.0'></LedAbility>");
		break;
    case 150://����ץ�Ĳ���ʱ������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_FACESNAP_SCHEDULE_CAPABILITIES;
        m_strInputParam = "1";
		break;
    case 151://Smart��������ʱ������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_STORAGEDETECTION_SCHEDULE_CAPABILITIES;
        m_strInputParam = "1";
		break;
    case 152://Smart��д����������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_STORAGEDETECTION_RWLOCK_CAPABILITIES;
        m_strInputParam = "1";
		break;

    case 153://��ȡSensor ���ڲ�����Э�������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_SENSOR_ADJUSTMENT_CAPABILITIES;
        break;
    case 154:
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_MANUALRANGING_CAPABILITIES;
        m_strInputParam = "2";
        break;
    case 155://SD��������������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_STORAGEDETECTION_UNLOCK_CAPABILITIES;
		break;
	case 156: //��Ļ����������
		m_bSTDAbility = TRUE;
		m_dwAbilityType = NET_DVR_GET_SCREEN_CONFIG_CAP;
		m_strInputParam = "1";
		break;
	case 157: //��Ļ��������
		m_dwAbilityType = SCREEN_EXCHANGE_ABILITY;
		xmlInput.Parse("<ScreenExchangeAbility version=\"2.0\"></ScreenExchangeAbility>");
		break;
	case 158: //��Ļ��������½��������
		m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/DisplayDev/Auxiliary/ScreenServer/1/loginCfg/capabilities\r\n";
		break;
    case 159: //ͼ���ּ����������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_IMAGE_DIFF_DETECTION_CFG_CAP;
        m_strInputParam = "5";
        break;
    case 160: //�����ļ���Ϣ��������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_RECORDING_PUBLISH_FILE_INFO_CAP;
        m_strInputParam = "1";
        break;        
    case 161: //�ֶ��γ�¼���������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_MANUAL_CURRICULUM_CONTROL_CAP;
		break;
    case 162: //����������Ϣ����
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_TMEVOICE_CAPABILITIES;
        m_strInputParam = "1";
		break;
	case 163://ftp�ϴ���Ϣ��������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_FTP_CAPABILITIES;
		break;
    case 164://��ȡptz�����������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_PTZ_CAPABILITIES;
        m_strInputParam = "1";
		break;
    case 165:
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_POSTRADAR_CAPABILITIES;
        m_strInputParam = "1";
		break;
	case 166://�ƴ洢URL����
		m_bSTDAbility = TRUE;
		m_dwAbilityType = NET_DVR_GET_CLOUD_URL_CAP;
		break;
	case 167://�ƴ洢��������
		m_bSTDAbility = TRUE;
		m_dwAbilityType = NET_DVR_GET_CLOUD_CFG_CAP;
		break;
	case 168://�ƴ洢�ϴ�������������
		m_bSTDAbility = TRUE;
		m_dwAbilityType = NET_DVR_GET_CLOUDSTORAGE_UPLOADSTRATEGY_CAP;
		m_strInputParam = "1";
		break;
	case 169: //¼������������������
		m_dwAbilityType = 0xffffffff;
		m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
		m_strInputParam = "GET /ISAPI/ContentMgmt/RecordingHost/hostParam/capabilities\r\n";
		break;
	case 170: //���ô�����ͳ����������
		m_dwAbilityType = 0xffffffff;
		m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
		m_strInputParam = "GET /ISAPI/ContentMgmt/RecordingHost/ClassDataControl/capabilities\r\n";
		break;
	case 171: //��ȡͳ�������б������
		m_dwAbilityType = 0xffffffff;
		m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
		m_strInputParam = "GET /ISAPI/ContentMgmt/RecordingHost/ClassDataStatisticCfg/getStatisticData/capabilities\r\n";
		break;
	case 172: //�����߼�������������
		m_dwAbilityType = 0xffffffff;
		m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
		m_strInputParam = "GET /ISAPI/ContentMgmt/RecordingHost/DirectedAdvanceCfg/commandID/<ID>/capabilities\r\n";
		break;
    case 173://��ȡ��ֻ��Ⲽ��ʱ������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_SHIPSDETECTION_SCHEDULE_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 174://��ֻ��������������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_SHIPSDETECTION_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 175:
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_THERMOMETRY_BASICPARAM_CAPABILITIES;
        m_strInputParam = "2";
        break;
    case 176:
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_THERMOMETRY_SCENE_CAPABILITIES;
        m_strInputParam = "2";
        break;
    case 177:
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_THERMOMETRY_ALARMRULE_CAPABILITIES;
        m_strInputParam = "2";
        break;
    case 178:
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_BAREDATAOVERLAY_CAPABILITIES;
        m_strInputParam = "2";
        break;
    case 179: //Э��͸��������
        m_bSTDAbility = 1;
        m_dwAbilityType = NET_DVR_GET_PASSTHROUGH_CAP;
        break;
	case 180:
		m_bSTDAbility = TRUE;
		m_dwAbilityType = NET_DVR_GET_MANUALDEICING_CAPABILITIES;
		m_strInputParam = "2";
		break;
	case 181: //Τ����������
		m_dwAbilityType = 0xffffffff;
		m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
		m_strInputParam = "GET /ISAPI/AccessControl/WiegandCfg/capabilities\r\n";
		break;
	case 182: //�ſذ�ȫģ������״̬����
		m_dwAbilityType = 0xffffffff;
		m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
		m_strInputParam = "GET /ISAPI/AccessControl/DoorSecurityModule/moduleStatus/capabilities\r\n";
		break;
	case 183: //�Ž�������
		m_dwAbilityType = 0xffffffff;
		m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
		m_strInputParam = "GET /ISAPI/AccessControl/capabilities\r\n";
		break;
    case 184: //����Դ�ַ���������
        m_bSTDAbility = 1;
        m_dwAbilityType = NET_DVR_GET_INPUT_SOURCE_TEXT_CAP;
        m_strInputParam = "1";
        break;
    case 185: //����Դ�Զ���ֱ�������
        m_bSTDAbility = 1;
        m_dwAbilityType = NET_DVR_GET_INPUT_SOURCE_RESOLUTION_CAP;
        m_strInputParam = "1";
        break;
    case 186://һ��ʽ������Ʒ����
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_EMERGENCE_ALARM_PRODUCT_CAP;
        break;
    case 187://���еȴ�������������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_CALL_WAITTING_CFG_CAP;
        break;
    case 188://���Ʋ�����������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_ALARM_LAMP_CFG_CAP;
        break;
    case 189://������ʾ��������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_VOICE_PROMPTION_CFG_CAP;
        break;
    case 190://����������������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_EMERGENCE_ALARM_RESPONSE_CAP;
        break;
	case 191: //��ȡ����ץ�Ĳ�������
		m_dwAbilityType = 0xffffffff;
		m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
		m_strInputParam = "GET /ISAPI/AccessControl/SnapConfig/capabilities\r\n";
		break;
    case 192://������Ϣ��������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_HIDDEN_INFORMATION_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 193://���Ų�����������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿ�,NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/AccessControl/DepartmentParam/capabilities";
        break;
    case 194://�Ű�ƻ���������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿ�,NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/AccessControl/SchedulePlan/capabilities";
        break;
    case 195://���ڹ�����������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿ�,NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/AccessControl/AttendanceRule/capabilities";
        break;
    case 196://��ͨ����������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿ�,NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/AccessControl/OrdinaryClass/capabilities";
        break;
    case 197://��ʱ����������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿ�,NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/AccessControl/WorkingClass/capabilities";
        break;
    case 198://���ڼ�������������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿ�,NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/AccessControl/AttendanceHolidayGroup/capabilities";
        break;
    case 199://���ڼ��ռƻ���������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿ�,NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/AccessControl/AttendanceHolidayPlan/capabilities";
        break;
    case 200://�Ž�������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿ�,NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/AccessControl/capabilities";
        break;
    case 201://������ЧID��ȡ����
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿ�,NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/AccessControl/AttendanceEffectiveID/capabilities";
        break;
    case 202: //��ȡ�������Ϣ����
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_ACCESSORY_CARD_INFO_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 203: //��ʾ�������������
        m_bSTDAbility = 1;
        m_dwAbilityType = NET_DVR_GET_DISPINPUT_CFG_CAP;
        break;
    case 204: //EDID�����ļ���Ϣ������
        m_bSTDAbility = 1;
        m_dwAbilityType = NET_DVR_GET_EDID_CFG_FILE_INFO_CAP;
        break;
    case 205: //�ȳ������ܻ�������
        m_bSTDAbility = 1;
        m_dwAbilityType = NET_DVR_GET_THERMINTELL_CAPABILITIES;
        break;
    case 206:
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_FIREDETECTION_SCHEDULE_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 207:
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_THERMOMETRY_SCHEDULE_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 208:
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_TEMPERTURE_SCHEDULE_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 209:
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_SEARCH_LOG_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 210://��ѧ��������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_OIS_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 211://MAC��ַ������������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_MACFILTER_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 212://ӥ�Ӿ۽��궨��������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_EAGLEFOCUS_CALCFG_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 213://ӥ�Ӿ۽���������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_EAGLEFOCUSING_CFG_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 214://Smart��Ϊ�궨���˳ߴ繦������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_SMARTCALIBRATION_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 215: //����OSD����
        m_bSTDAbility = 1;
        m_dwAbilityType = NET_DVR_GET_SUBWND_DECODE_OSD_CAP;
        m_strInputParam = "16842753";
        break;
    case 216://�洢��������RACM������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_RACM_CAP;
        break;
    case 217: //CVR�ش����ܻ�������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_RECORD_PASSBACK_BASIC_CFG_CAP;
        break;
    case 218: //�ش���ʷ¼��ƻ�����
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_RECORD_PASSBACK_HISTORY_PLAN_CFG_CAP;
        break;
    case 219://��ȡ�豸��ϵͳ����
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_SYSTEM_CAPABILITIES;
        break;
    case 220: // ��ȡ�豸ӥ������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_EAGLEEYE_CAPABILITIES;
        break;
    case 221://��ȡ�ֱ���ģʽ�л�����
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_RESOLUTION_SWITCH_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 222://�����������Խ���������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/ContentMgmt/RecordingHost/EducationServerConfiguration/capabilities\r\n";
        break;
    case 223://��Ƶ�л���������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/ContentMgmt/RecordingHost/VideoSwitchConfiguration/capabilities\r\n";
        break;
    case 224://Ƭͷ��������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/ContentMgmt/RecordingHost/VideoTitleConfiguration/capabilities\r\n";
        break;
    case 225://Ƭβ��������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/ContentMgmt/RecordingHost/VideoTrailerConfiguration/capabilities\r\n";
        break;
    case 226: //�͵ؿ�������������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/AccessControl/localController/capabilities\r\n";
        break;
    case 227: //�͵ؿ�������������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/AccessControl/localController/control/capabilities\r\n";
        break;
    case 228: //��ȡUSB�豸״̬����
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/AccessControl/USBStatus/capabilities\r\n";
        break;
    case 229: //USB��������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/AccessControl/USBControl/capabilities\r\n";
        break;
    case 230: //�Ž�������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/AccessControl/capabilities\r\n";
        break;
    case 231: //��ȡUSB���ƽ�������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/AccessControl/USBControlProgress/capabilities\r\n";
        break;
    case 232://��ȡ��ȡ������Ϣ�������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_VEHICLLE_RESULT_CAPABILITIES;
        break;
    case 233: //ͥ����������
        m_dwAbilityType = DEVICE_ABILITY_INFO;
        xmlInput.Parse("<TrialHostAbility version='2.0'></TrialHostAbility>");
        break;
    case 234: //�豸����ģʽ����
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿ�,NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/VideoIntercom/workMode/capabilities";
        break;
    case 235: //��ȡ���������������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_FIRESHIELDMASK_CAPABILITIES;
        m_strInputParam = "2";
        break;
    case 236: //��ȡ����������������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_SMOKESHIELDMASK_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 237: //��ȡ����ɨ������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_AREASCAN_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 238: //�豸Ѳ��ģʽ����
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_CRUISE_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 239: //������ʪ������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_TEMP_HUMI_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 240: //�ֶ�����ʵʱ��������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_MANUALTHERM_CAPABILITIES;
        m_strInputParam = "2";
        break;
    case 241: //��ȡ�ֶ����»���������������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_MANUALTHERM_BASIC_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 242: //�߾���PTZ����λ��������չ����
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_PTZABSOLUTEEX_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 243://��·���������Ƶ�����洢��������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/System/Audio/Mixing/capabilities\r\n";
        break;
    case 244://��Ƶ��������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/System/Audio/capabilities\r\n";
        break;
    case 245://�������ڷ�����������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/System/Serial/ports/1/capabilities";
        break;
    case 246://���ڷ�������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/System/Serial/capabilities\r\n";
        break;
    case 247: //�ݿؼ̵�������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/AccessControl/LadderControlRelay/capabilities\r\n";
        break;
    case 248: //��ȡ�����������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_SOFTWARE_SERVICE_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 249: //Զ�������շ�������
        m_dwAbilityType = REMOTE_NETMGR_FOT_ABILITY;
        xmlInput.Parse("<RemoteNetMgrFOTAbility version=\"2.0\"></RemoteNetMgrFOTAbility>");
        break;
    case 250: //�Ž��¼���ȡ����
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿ�,NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/AccessControl/GetAcsEvent/capabilities";
        break;
    case 251://��ȡ�����������ȶԲ���ʱ������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_BLACKLIST_FACECONTRAST_SCHEDULE_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 252://��ȡ�����������ȶԲ���ʱ������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_WHITELIST_FACECONTRAST_SCHEDULE_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 253: //�����ȶ���������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/Intelligent/channels/1/faceContrast/capabilities\r\n";
        break;
    case 254: //���������������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/ContentMgmt/RecordingHost/HumanInformation/capabilities\r\n";
        break;
    case 255: //�ȳ����л���������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_THERMAL_PIP_CAPABILITIES;
        m_strInputParam = "2";
        break;
    case 256: //�ȳ���OSD��������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_THERMAL_INTELRULEDISPLAY_CAPABILITIES;
        m_strInputParam = "2";
        break;
    case 257: //������������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_THERMOMETRY_MODE_CAPABILITIES;
        m_strInputParam = "2";
        break;
    case 258: //����������������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_CURRENT_LOCK_CAPABILITIES;
        break;
    case 259://����ʶ�𲼷�ʱ������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_HUMAN_RECOGNITION_SCHEDULE_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 260: //��������ʶ��������
        m_dwAbilityType = DEVICE_ABILITY_INFO;
        xmlInput.Parse("<VehicleRecogAbility version='2.0'><channelNO>1</channelNO></VehicleRecogAbility>");
        break;
    case 261: //��ԴIP��������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/System/Network/ResourceIPConfiguration/capabilities\r\n";
        break;
    case 262: //��д��������������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/Traffic/UHFRFIDReader/basicInformation/capabilities\r\n";
        break;
    case 263: //�̴��������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/Traffic/UHFRFIDReader/hardDiskStorageTest/capabilities\r\n";
        break;
    case 264: //��ȡԤ��ģʽ��������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_PREVIEW_MODE_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 265: //��ȡ����궨����
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_CALIB_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 266: //��ȡPos��������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_POSINFO_OVERLAY_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 267://��ȡ�����ȶԲ���ʱ������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_FACECONTRAST_SCHEDULE_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 268: //Τ��������������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/AccessControl/WiegandRuleCfg/capabilities\r\n";
        break;
    case 269: //M1��������֤������������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/AccessControl/M1CardEncryptCfg/capabilities\r\n";
        break;
    case 270: //������Ϣ��ȡ����
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/AccessControl/DeployInfo/capabilities\r\n";
        break;
    case 271: //��������Ǳ�ط�������������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/AccessControl/SubmarineBack/capabilities\r\n";
        break;
    case 272: //�����������Ǳ��������������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/AccessControl/SubmarineBackHostInfo/capabilities\r\n";
        break;
    case 273: //��ʼ��������������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/AccessControl/StartReaderInfo/capabilities\r\n";
        break;
    case 274: //��������Ǳ�ض�������������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/AccessControl/SubmarineBackReader/capabilities\r\n";
        break;
    case 275: //��������������Ǳ��ˢ����¼�������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/AccessControl/ClearCardRecord/capabilities\r\n";
        break;
    case 276: //��������Ǳ��ģʽ��������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/AccessControl/SubmarineBackMode/capabilities\r\n";
        break;
    case 277: //��������Ǳ����Ϣ�������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/AccessControl/ClearSubmarineBack/capabilities\r\n";
        break;
    case 278: //�����������Ǳ����������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/AccessControl/ServerDevice/capabilities\r\n";
        break;
    case 279: //��������������Ǳ����������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/AccessControl/ReaderAcrossHost/capabilities\r\n";
        break;
    case 280: //�Ž��¼���ȡ����
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/AccessControl/GetAcsEvent/capabilities\r\n";
        break;
    case 281://��������ͳ�Ʋ���ʱ������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_FRAMES_PEOPLE_COUNTING_SCHEDULE_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 282://ѧ��������Ⲽ��ʱ������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_STUDENTS_STOODUP_SCHEDULE_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 283://��ȡ��Ա�ܶȼ��Ĳ���ʱ������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_PERSONDENSITY_SCHEDULE_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 284: //�����ļ�����������������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/ContentMgmt/FileData/TransferSpeedConfiguration/capabilities\r\n";
        break;
    case 285: //�����ļ�������������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/ContentMgmt/FileData/LockConfiguration/capabilities\r\n";
        break;
    case 286: //��ȡ�����ļ���Ϣ����
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/ContentMgmt/FileData/SearchFile/capabilities\r\n";
        break;
    case 287: //��ر��ݱ�����Ϣ��������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/ContentMgmt/RemoteBackup/BackupMachineInfo/capabilities\r\n";
        break;
    case 288: //��ر�����������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/ContentMgmt/RemoteBackup/Task/capabilities\r\n";
        break;
    case 289://��ȡ�����ȶԿⲼ��ʱ������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_FACELIB_SCHEDULE_CAPABILITIES;
        m_strInputParam = "FDID";
        break;
    case 290: //���ӶԽ�ϵͳ�л���������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/VideoIntercom/SystemSwitchCfg/capabilities\r\n";
        break;
    case 291: //����ͨѶ��Կ��������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/VideoIntercom/BluetoothCommKey/capabilities\r\n";
        break;
    case 292: //������������������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/VideoIntercom/FaceServerCfg/capabilities\r\n";
        break;
    case 293: //������Ϣ�·�����
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/VideoIntercom/PlateInfo/capabilities\r\n";
        break;
    case 294: //��ͨ����������
        m_dwAbilityType = DEVICE_ABILITY_INFO;
        xmlInput.Parse("<ZeroChanEncodeAbility></ZeroChanEncodeAbility>");
        break;
    case 295://��ȡ��ȫñ���Ĳ���ʱ������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_SAFETY_HELMET_SCHEDULE_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 296: //Ȩ�޿�������Ƶ�ļ�������������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/AccessControl/RightControllerAudio/capabilities\r\n";
        break;
    case 297: //ͨ����������������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/AccessControl/ChannelControllerCfg/capabilities\r\n";
        break;
    case 298: //բ�����ز��뼰��Ϣ��ȡ����
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/AccessControl/GateDialAndInfo/capabilities\r\n";
        break;
    case 299: //բ��״̬��ȡ����
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/AccessControl/GateStatus/capabilities\r\n";
        break;
    case 300: //բ������״̬��ȡ����
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/AccessControl/GateIRStatus/capabilities\r\n";
        break;
    case 301: //բ���������״̬��ȡ����
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/AccessControl/GateRelatedPartsStatus/capabilities\r\n";
        break;
    case 302: //ͨ������������������������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/AccessControl/ChannelControllerAlarmLinkage/capabilities\r\n";
        break;
    case 303: //ͨ�����������������������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/AccessControl/ChannelControllerAlarmOut/capabilities\r\n";
        break;
    case 304: //ͨ�����������������������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/AccessControl/ChannelControllerAlarmOutControl/capabilities\r\n";
        break;
    case 305://�������²���ʱ������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_FACE_THERMOMETRY_SCHEDULE_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 306://��ȡSoftIO����ʱ������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_SOFTIO_SCHEDULE_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 307://��ȡ��Ա�ŶӼ��Ĳ���ʱ������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_PERSONQUEUE_SCHEDULE_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 308://��ȡ��Ƶ��Ա��Ⲽ��ʱ������
        m_bSTDAbility = TRUE;
        m_dwAbilityType = NET_DVR_GET_HFPD_SCHEDULE_CAPABILITIES;
        m_strInputParam = "1";
        break;
    case 309://��ȡ������������
        m_dwAbilityType = 0xffffffff;
        m_bSTDAbility = 2; //͸���ӿڣ�NET_DVR_STDXMLConfig
        m_strInputParam = "GET /ISAPI/IoTGateway/capabilities?format=json\r\n";
        break;
    case 310:
        m_dwAbilityType = WALL_ABILITY;
        xmlInput.Parse("<WallAbility><wallNo>1</wallNo></WallAbility>");
        break;
    default:
		m_dwAbilityType = 0;
		break;
	}
    if (!m_bSTDAbility)
    {
        xmlInput.SetRoot();
        m_strInputParam = xmlInput.GetChildren().c_str();
        m_strInputParam.Replace("\n", "\r\n");
    }	
	UpdateData(FALSE);
}

void CDlgAbilityAutoDetect::ParseAbility()
{
	CXmlBase xmlAll;
	char szPath[MAX_PATH] = {0};
	GetCurrentPath(szPath);
	sprintf(szPath, "%s\\SDK_ABILITY.xml", szPath);
	if(!xmlAll.LoadFile(szPath))
	{
		GetDlgItem(IDC_EDT_ABILITY_LOST)->SetWindowText("�����ļ����ش���");
		return;
	}
	xmlAll.SetRoot();
	xmlAll.IntoElem();

    int iIndex = GetSelectedAbilityIndex();
    if (iIndex < 0)
    {
        return;
    }
    switch (iIndex)
	{
	case 0:	//��Ӳ������
		m_dwAbilityType = DEVICE_SOFTHARDWARE_ABILITY;
		ParseSimpleAbility(xmlAll, "BasicCapability");
		break;
	case 1:	//Wifi����
		m_dwAbilityType = DEVICE_NETWORK_ABILITY;
		ParseSimpleAbility(xmlAll, "NetworkSetting");
		break;
	case 2: //��������
		m_dwAbilityType = DEVICE_ENCODE_ALL_ABILITY;
		break;
	case 3:	//��ǰ��������
		m_dwAbilityType = DEVICE_ENCODE_CURRENT;
		break;
	case 4:	//ǰ�˲�������
		m_dwAbilityType = IPC_FRONT_PARAMETER;
		break;
	case 5:	//��������
		m_dwAbilityType = FISHEYE_ABILITY;
		break;
	case 6:	//Raid����
		m_dwAbilityType = DEVICE_RAID_ABILITY;
		ParseSimpleAbility(xmlAll, "RAID");
		break;
	case 7: //��������2.0
		m_dwAbilityType = DEVICE_ENCODE_ALL_ABILITY_V20;
		ParseSimpleAbility(xmlAll, "AudioVideoCompressInfo");
		break;
	case 8: //ǰ�β���2.0
		m_dwAbilityType = IPC_FRONT_PARAMETER_V20;
		ParseSimpleAbility(xmlAll, "CAMERAPARA");
		break;
	case 9: //����������
		m_dwAbilityType = DECODER_ABILITY;
		ParseSimpleAbility(xmlAll, "DecoderAbility");
		break;
	case 10: //�û���������
		m_dwAbilityType = DEVICE_USER_ABILITY;
		ParseSimpleAbility(xmlAll, "UserAbility");
		break;
	case 11: //����Ӧ������
		m_dwAbilityType = DEVICE_NETAPP_ABILITY;
		ParseSimpleAbility(xmlAll, "NetAppAbility");
		break;
	case 12: //��Ƶͼ������
		m_dwAbilityType = DEVICE_VIDEOPIC_ABILITY;
		ParseSimpleAbility(xmlAll, "VideoPicAbility");
		break;
	case 13: //JPEGץͼ����
		m_dwAbilityType = DEVICE_JPEG_CAP_ABILITY;
		ParseSimpleAbility(xmlAll, "JpegCaptureAbility");
		break;
	case 14: //��������
		m_dwAbilityType = DEVICE_SERIAL_ABILITY;
		ParseSimpleAbility(xmlAll, "SerialAbility");
		break;
	case 15: //��Ƶ�ۺ�ƽ̨����
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		break;
	case 16: //������ϵͳ����
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		break;
	case 17: //������ϵͳ����
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		break;
	case 18: //����������ϵͳ
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		break;
	case 19: //���������ϵͳ
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		break;
	case 20: //�����ϵͳ
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		break;
	case 21: //���籨������
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "AlarmHostAbility");
		break;
	case 22: //ƽ̨������ϵͳ
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		break;
	case 23:
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "RecordAbility");
		break;
	case 24:
		m_dwAbilityType = DEVICE_DYNCHAN_ABILITY;
		ParseSimpleAbility(xmlAll, "DynChannelAbility");
		break;
	case 25:
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "ChannelInputAbility");
		break;
	case 26: //��Ѷ������
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "InquestAbility");
		break;
	case 27: //CVR����
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "CVRAbility");
		break;
	case 28: //�豸�¼�����
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "EventAbility");
		break;
	case 29: //ǰ�˽���ͨ������
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "GetAccessDeviceChannelAbility");
		break;
	case 30: //����Ԥ���л�����
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "PreviewSwitchAbility ");
		break;
	case 31: //ROI����
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "ROIAbility");
		break;
	case 32: //��̨����
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "PTZAbility");
		break;
	case 33: //VQD����
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "VQDAbility");
		break;
	case 34://���ܽ�ͨ
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "ITDeviceAbility");
		break;
	case 35: //N+1����
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "NPlusOneAbility");
		break;
	case 36://�����������
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "HardDiskAbility");
		break;
	case 37://�豸��������
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "DeviceAlarmAbility");
		break;
	case 38://IPC�����ļ����뵼������
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "IPAccessConfigFileAbility");
		break;
	case 39://ǰ�˶�̬������ȡ���ع�ʱ�䣩
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "ExposureSetDynamicAbility");
		//xmlInput.Parse("<CameraParaDynamicAbility version='2.0'><channelNO>1</channelNO><ExposureSetDynamicLinkTo><WDR><WDREnable>0</WDREnable></WRD><IrisMode><IrisType>0</IrisType></IrisMode></ExposureSetDynamicLinkTo></CameraParaDynamicAbility>");
		break;
	case 40://ǰ�˶�̬������ȡ��CaptureMode��
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "CaptureModeDynamicAbility");
		//xmlInput.Parse("<CameraParaDynamicAbility version='2.0'><channelNO>1</channelNO><AudioVideoCompressInfoDynamicLinkTo><captureMode opt='640*480@30fps'/></AudioVideoCompressInfoDynamicLinkTo></CameraParaDynamicAbility>");
		break;
	case 41://GBT28181Э��
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "GBT28181AccessAbility");
		//xmlInput.Parse("<GBT28181AccessAbility version='2.0'><channelNO>1</channelNO></GBT28181AccessAbility>");
		break;
	case 42://��־��������
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "SearchLogAbility");
		//xmlInput.Parse("<SearchLogAbility version='2.0'><channelNO>1</channelNO></SearchLogAbility>");
		break;
	case 43://��������¼��
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "AlarmTriggerRecordAbility");
		//xmlInput.Parse("<AlarmTriggerRecordAbility version='2.0'><channelNO>1</channelNO></AlarmTriggerRecordAbility>");
		break;
	case 44:
		m_dwAbilityType = IP_VIEW_DEV_ABILITY;
		ParseSimpleAbility(xmlAll, "IpViewDevAbility");
		break;
	case 48:
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "VcaDevAbility");
		break;
	case 49:
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "VcaCtrlAbility");
		break;
	case 50:
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "VcaChanAbility");
		break;
	case 51:
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "CameraMountAbility");
		break;
	case 53:
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "BinocularAbility");
		break;
    case 76:
        m_dwAbilityType = ACS_ABILITY;
        ParseSimpleAbility(xmlAll, "AcsAbility");
		break;
    case 77:
        m_dwAbilityType = DEVICE_ABILITY_INFO;
        ParseSimpleAbility(xmlAll, "ImageDisplayParamAbility");
		break;
	case 78:
		m_dwAbilityType = MERGEDEV_ABILITY;
		break; 
    case 79:
        m_dwAbilityType = DEVICE_ABILITY_INFO;
        ParseSimpleAbility(xmlAll, "SecurityAbility");
		break;
    case 82:	//IPC��������
		m_dwAbilityType = IPC_UPGRADE_DESCRIPTION;
        break;
	case 97:
		m_dwAbilityType = STREAM_ABILITY;
		ParseSimpleAbility(xmlAll, "StreamAbility");
		break;
	case 98:
		m_dwAbilityType = ACS_ABILITY;
		ParseSimpleAbility(xmlAll, "AcsAbility");
		break;
    case 100:
        m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "POSAbility");
        break;
    case 260: //��������ʶ��������
        m_dwAbilityType = DEVICE_ABILITY_INFO;
        ParseSimpleAbility(xmlAll, "VehicleRecogAbility");
        break;
    case 261:
        m_dwAbilityType = WALL_ABILITY;
        ParseSimpleAbility(xmlAll, "WallAbility");
        break;
	default:
		m_dwAbilityType = 0;
		break;
	}
}

void CDlgAbilityAutoDetect::ParseSimpleAbility(CXmlBase &xmlAll, char* szAbilityName)
{	
	CXmlBase xmlDev;
	
	if (!xmlAll.FindElem(szAbilityName))
	{
		GetDlgItem(IDC_EDT_ABILITY_LOST)->SetWindowText("�����ļ���û�е�ǰ����");
		return ;
	}
	xmlAll.Parse(xmlAll.GetChildren().c_str());
	xmlAll.SetRoot();
	
	xmlDev.Parse(m_pOutBuf);
	if (!xmlDev.FindElem(szAbilityName))
	{
		GetDlgItem(IDC_EDT_ABILITY_LOST)->SetWindowText("���ݸ�ʽ����,�޷�����");
		return ;
	}

	ParseAbilityRecursive(xmlAll, xmlDev);

	UpdateData(FALSE);
}

void CDlgAbilityAutoDetect::ParseAbilityRecursive(CXmlBase &xmlAll, CXmlBase &xmlDev)
{
 	string strNode = xmlAll.GetNodeName();
 
 	if (!xmlDev.FindElem(strNode.c_str()))
 	{
 		m_strLostAbility += "LOST:";
		m_strLostAbility += strNode.c_str();
 		m_strLostAbility += "\r\n";
		if (xmlAll.NextSibElem())
		{
			ParseAbilityRecursive(xmlAll, xmlDev);
		}
		return;
 	}
 	
 	if (xmlAll.IntoElem())
	{
		if(xmlDev.IntoElem())
		{
			ParseAbilityRecursive(xmlAll, xmlDev);
			xmlDev.OutOfElem();
		}
		else
		{
			if (xmlDev.GetNodeName() == strNode && xmlDev.GetData() == "" && xmlDev.GetFirstAttributeValue() == "")
			{
				m_strLostAbility += "NULL:";
				m_strLostAbility += xmlDev.GetNodeName().c_str();
				m_strLostAbility += "\r\n";
			}			
			ParseAbilityRecursive(xmlAll, xmlDev);
		}
		xmlAll.OutOfElem();
	}
	else if (xmlDev.GetNodeName() == strNode && xmlDev.GetData() == "" && xmlDev.GetFirstAttributeValue() == "")
	{
		m_strLostAbility += "NULL:";
		m_strLostAbility += xmlDev.GetNodeName().c_str();
		m_strLostAbility += "\r\n";
	}

	if (xmlAll.NextSibElem())
	{
		if (!xmlDev.NextSibElem())	//������ж���Ϊ�˴�����ͬ���ƽڵ���ڶ�������
		{
			do 
			{
				m_strLostAbility += "LOST:";
				m_strLostAbility += xmlAll.GetNodeName().c_str();
 				m_strLostAbility += "\r\n";
			} while (xmlAll.NextSibElem());
			return;
		}
		ParseAbilityRecursive(xmlAll, xmlDev);
	}
}


void CDlgAbilityAutoDetect::OnCbnEditchangeCmbAbilityType()
{
    int iCount = ((CComboBox*)GetDlgItem(IDC_CMB_ABILITY_TYPE))->GetCount();
    for (int i = 0; i < iCount; ++i)
    {
        m_cmbAbilityType.DeleteString(0);
    }
    m_vecCurAbilityType.clear();
    CString strTemp;
    ((CComboBox*)GetDlgItem(IDC_CMB_ABILITY_TYPE))->GetWindowText(strTemp);
    if (strTemp != "")
    {
        m_cmbAbilityType.ShowDropDown(TRUE);
    }
    for (std::vector<CString>::iterator iter = m_vecAbilityType.begin(); iter != m_vecAbilityType.end(); ++iter)
    {
        if (iter->Find(strTemp) >= 0)
        {
            m_cmbAbilityType.AddString(*iter);
            m_vecCurAbilityType.push_back(*iter);
        }
    }
}

int CDlgAbilityAutoDetect::GetSelectedAbilityIndex()
{
    int iSel = m_cmbAbilityType.GetCurSel();
    if (iSel < 0 || iSel >= m_vecCurAbilityType.size())
    {
        return -1;
    }

    CString strTemp = m_vecCurAbilityType[iSel];
    for (int iIndex = 0; iIndex < m_vecAbilityType.size(); ++iIndex)
    {
        if (strTemp == m_vecAbilityType[iIndex])
        {
            return iIndex;
        }
    }
    return -1;
}


void CDlgAbilityAutoDetect::OnCbnSetfocusCmbAbilityType()
{
    m_cmbAbilityType.ShowDropDown(TRUE);
}
