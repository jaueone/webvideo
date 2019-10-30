// DlgDeviceCfg.cpp : implementation file

/**********************************************************
FileName:    DlgDeviceCfg.cpp
Description: �豸��Ϣ����      
Date:        
Note: 		<ȫ��>�ṹ��,���GeneralDef.h, ȫ�ֱ���,������config.cpp   
Modification History:      
<version>   <time>         <desc>

***********************************************************/

#include "stdafx.h"
#include "config.h"
#include "DlgDeviceCfg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDeviceCfg dialog


CDlgDeviceCfg::CDlgDeviceCfg(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDeviceCfg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDeviceCfg)
	m_iHdNum = 0;
	m_iAlarmInNum = 0;
	m_iAlarmOutNum = 0;
	m_iChanNum = 0;
	m_iYKQID = 0;
	m_csDeviceName = _T("");
	m_csDeviceSerialNo = _T("");
	m_csSoftwareVersion = _T("");
	m_csHardwareVersion = _T("");
	m_csDspVersion = _T("");
	m_csPanelVersion = _T("");
	m_iMtuPort = 0;
	m_iHttpPort = 0;
	m_iMtu = 0;
	m_csDeviceIP = _T("");
	m_csGatewayIP = _T("");
	m_csIPMask = _T("");
	m_csMacAddr = _T("");
	m_iMainPort = 0;
	m_csDNSIP1 = _T("");
	m_csAlarmHostIP = _T("");
	m_iAlarmHostPort = 0;
	m_csDNSIP2 = _T("");
	m_bChkDhcp = FALSE;
	m_csMultiIP = _T("");
	//}}AFX_DATA_INIT
}


void CDlgDeviceCfg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDeviceCfg)
	DDX_Control(pDX, IDC_COMBO_MINORSCALE, m_comboMinorScale);
	DDX_Control(pDX, IDC_COMBO_MAJORSCALE, m_comboMajorScale);
	DDX_Control(pDX, IDC_COMBO_NET_FACE, m_comboNetFace);
	DDX_Control(pDX, IDC_COMBO_DEVICE_TYPE, m_comboDeviceType);
	DDX_Control(pDX, IDC_COMBO_CYCLE_RECORD, m_comboCycleRec);
	DDX_Text(pDX, IDC_EDIT_HD_NUM, m_iHdNum);
	DDX_Text(pDX, IDC_ALARM_IN_NUM, m_iAlarmInNum);
	DDX_Text(pDX, IDC_ALARM_OUT_NUM, m_iAlarmOutNum);
	DDX_Text(pDX, IDC_EDIT_CHAN_NUM, m_iChanNum);
	DDX_Text(pDX, IDC_EDIT_YKQID, m_iYKQID);
	DDX_Text(pDX, IDC_EDIT_DEVICE_NAME, m_csDeviceName);
	DDX_Text(pDX, IDC_EDIT_SERIAL, m_csDeviceSerialNo);
	DDX_Text(pDX, IDC_EDIT_SOFTWARE_VERSION, m_csSoftwareVersion);
	DDX_Text(pDX, IDC_EDIT_HARDWAR_VERSION, m_csHardwareVersion);
	DDX_Text(pDX, IDC_EDIT_DSP_SOFTWARE_VERSION, m_csDspVersion);
	DDX_Text(pDX, IDC_EDIT_PANEL_VERSION, m_csPanelVersion);
	DDX_Text(pDX, IDC_EDIT_HTTP_PORT, m_iHttpPort);
	DDX_Text(pDX, IDC_EDIT_MTU, m_iMtu);
	DDX_Text(pDX, IDC_EDIT_DEVICE_IP, m_csDeviceIP);
	DDX_Text(pDX, IDC_EDIT_GATEWAY_IP, m_csGatewayIP);
	DDX_Text(pDX, IDC_EDIT_IP_MASK, m_csIPMask);
	DDX_Text(pDX, IDC_EDIT_MAC_ADDR, m_csMacAddr);
	DDX_Text(pDX, IDC_EDIT_MAIN_PORT, m_iMainPort);
	DDX_Text(pDX, IDC_EDIT_DNS1, m_csDNSIP1);
	DDX_Text(pDX, IDC_EDIT_ALARM_HOST_IP, m_csAlarmHostIP);
	DDX_Text(pDX, IDC_EDIT_ALARM_HOST_PORT, m_iAlarmHostPort);
	DDX_Text(pDX, IDC_EDIT_DNS2, m_csDNSIP2);
	DDX_Check(pDX, IDC_CHK_DHCP, m_bChkDhcp);
	DDX_Text(pDX, IDC_SUB_EDIT_MULTIIP, m_csMultiIP);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDeviceCfg, CDialog)
	//{{AFX_MSG_MAP(CDlgDeviceCfg)
	ON_BN_CLICKED(IDC_BUTTON_SETNETCFG, OnButtonSetNetCfg)
	ON_BN_CLICKED(IDC_BUTTON_SETDEVICECFG, OnButtonSetDeviceCfg)
	ON_BN_CLICKED(IDC_CHK_DHCP, OnChkDhcp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDeviceCfg message handlers




/*********************************************************
Function:	GetDeviceCfg
Desc:		��ȡ�豸�Ļ���������Ϣ
Input:	
Output:	
Return:	
**********************************************************/
void CDlgDeviceCfg::GetDeviceCfg()
{
	if (g_lUserID < 0)
	{
		return;
	}

	//��ȡ�豸�������ò�������NET_DVR_GET_DEVICECFG����
	DWORD dwReturned = 0;
	memset(&m_struDeviceCfg, 0, sizeof(NET_DVR_DEVICECFG));
	if (!NET_DVR_GetDVRConfig(g_lUserID, NET_DVR_GET_DEVICECFG, 0,&m_struDeviceCfg, sizeof(NET_DVR_DEVICECFG), &dwReturned))
	{
		MessageBox("��ȡ�豸�������ò���ʧ��!");
	}
	m_iHdNum=m_struDeviceCfg.byDiskNum;//Ӳ����
	m_iAlarmInNum = m_struDeviceCfg.byAlarmInPortNum;//����������
	m_iAlarmOutNum =m_struDeviceCfg.byAlarmOutPortNum;//���������
	m_iChanNum=m_struDeviceCfg.byChanNum;//ͨ����
	m_iYKQID=m_struDeviceCfg.dwDVRID;//ң����ID
	m_comboCycleRec.SetCurSel(m_struDeviceCfg.dwRecycleRecord);//ʵ�֡��Ƿ�ѭ��¼��������0-��1-��
	
	/***********************************�豸����������begin*************************************/
	int m_iTypeNum = 0;
	m_comboDeviceType.ResetContent();
	m_comboDeviceType.AddString("DVR");//����ַ������൱���������������ҳ������ַ�������һ����
	m_comboDeviceType.SetItemData(m_iTypeNum, DVR);//���±�Ϊ0�����У����dataΪDVR,DVR�Ǹ��궨�壬��ʵ����1
	m_iTypeNum++;
	m_comboDeviceType.AddString("ATMDVR");
	m_comboDeviceType.SetItemData(m_iTypeNum, ATMDVR);
	m_iTypeNum++;
	m_comboDeviceType.AddString("DVS");
	m_comboDeviceType.SetItemData(m_iTypeNum, DVS);
	m_iTypeNum++;
	m_comboDeviceType.AddString("DEC");
	m_comboDeviceType.SetItemData(m_iTypeNum, DEC);
	m_iTypeNum++;
	m_comboDeviceType.AddString("ENC_DEC");
	m_comboDeviceType.SetItemData(m_iTypeNum, ENC_DEC);
	m_iTypeNum++;
	m_comboDeviceType.AddString("DVR_HC");
	m_comboDeviceType.SetItemData(m_iTypeNum, DVR_HC);
	m_iTypeNum++;
	m_comboDeviceType.AddString("DVR_HT");
	m_comboDeviceType.SetItemData(m_iTypeNum, DVR_HT);
	m_iTypeNum++;
	m_comboDeviceType.AddString("DVR_HF");
	m_comboDeviceType.SetItemData(m_iTypeNum, DVR_HF);
	m_iTypeNum++;
	m_comboDeviceType.AddString("DVR_HS");
	m_comboDeviceType.SetItemData(m_iTypeNum, DVR_HS);
	m_iTypeNum++;
	m_comboDeviceType.AddString("DVR_HTS");
	m_comboDeviceType.SetItemData(m_iTypeNum, DVR_HTS);
	m_iTypeNum++;
	m_comboDeviceType.AddString("DVR_HB");
	m_comboDeviceType.SetItemData(m_iTypeNum, DVR_HB);
	m_iTypeNum++;
	m_comboDeviceType.AddString("DVR_HCS");
	m_comboDeviceType.SetItemData(m_iTypeNum, DVR_HCS);
	m_iTypeNum++;
	m_comboDeviceType.AddString("DVS_A");
	m_comboDeviceType.SetItemData(m_iTypeNum, DVS_A);
	m_iTypeNum++;
	m_comboDeviceType.AddString("DVR_HC_S");
	m_comboDeviceType.SetItemData(m_iTypeNum, DVR_HC_S);
	m_iTypeNum++;
	m_comboDeviceType.AddString("DVR_HT_S");
	m_comboDeviceType.SetItemData(m_iTypeNum, DVR_HT_S);
	m_iTypeNum++;
	m_comboDeviceType.AddString("DVR_HF_S");
	m_comboDeviceType.SetItemData(m_iTypeNum, DVR_HF_S);
	m_iTypeNum++;
	m_comboDeviceType.AddString("DVR_HS_S");
	m_comboDeviceType.SetItemData(m_iTypeNum, DVR_HS_S);
	m_iTypeNum++;
	m_comboDeviceType.AddString("ATMDVR_S");
	m_comboDeviceType.SetItemData(m_iTypeNum, ATMDVR_S);
	m_iTypeNum++;
	m_comboDeviceType.AddString("DEC_MAT");
	m_comboDeviceType.SetItemData(m_iTypeNum, DEC_MAT);
	m_iTypeNum++;
	m_comboDeviceType.AddString("DVR_MOBILE");
	m_comboDeviceType.SetItemData(m_iTypeNum, DVR_MOBILE);
	m_iTypeNum++;
	m_comboDeviceType.AddString("DVR_HD_S");
	m_comboDeviceType.SetItemData(m_iTypeNum, DVR_HD_S);
	m_iTypeNum++;
	m_comboDeviceType.AddString("DVR_HD_SL");
	m_comboDeviceType.SetItemData(m_iTypeNum, DVR_HD_SL);
	m_iTypeNum++;
	m_comboDeviceType.AddString("DVR_HC_SL");
	m_comboDeviceType.SetItemData(m_iTypeNum, DVR_HC_SL);
	m_iTypeNum++;
	m_comboDeviceType.AddString("DVR_HS_ST");
	m_comboDeviceType.SetItemData(m_iTypeNum, DVR_HS_ST);
	m_iTypeNum++;
	m_comboDeviceType.AddString("DVS_HW");
	m_comboDeviceType.SetItemData(m_iTypeNum, DVS_HW);
	m_iTypeNum++;
	m_comboDeviceType.AddString("IPCAM");
	m_comboDeviceType.SetItemData(m_iTypeNum, IPCAM);
	m_iTypeNum++;
	m_comboDeviceType.AddString("MEGA_IPCAM");
	m_comboDeviceType.SetItemData(m_iTypeNum, MEGA_IPCAM);
	m_iTypeNum++;
	m_comboDeviceType.AddString("IPCAM_X62MF");
	m_comboDeviceType.SetItemData(m_iTypeNum, IPCAM_X62MF);
	m_iTypeNum++;
	m_comboDeviceType.AddString("IPDOME");
	m_comboDeviceType.SetItemData(m_iTypeNum, IPDOME);
	m_iTypeNum++;
	m_comboDeviceType.AddString("IPMOD");
	m_comboDeviceType.SetItemData(m_iTypeNum, IPMOD);
	m_iTypeNum++;
	m_comboDeviceType.AddString("DS71XX_H");
	m_comboDeviceType.SetItemData(m_iTypeNum, DS71XX_H);
	m_iTypeNum++;
	m_comboDeviceType.AddString("DS81XX_HS_S");
	m_comboDeviceType.SetItemData(m_iTypeNum, DS81XX_HS_S);
	m_iTypeNum++;
	m_comboDeviceType.AddString("DS90XX_HF_S");
	m_comboDeviceType.SetItemData(m_iTypeNum, DS90XX_HF_S);
	m_iTypeNum++;
	m_comboDeviceType.AddString("DS91XX_HF_S");
	m_comboDeviceType.SetItemData(m_iTypeNum, DS91XX_HF_S);
	m_iTypeNum++;
	m_comboDeviceType.AddString("DS91XX_HD_S");
	m_comboDeviceType.SetItemData(m_iTypeNum, DS91XX_HD_S);
	m_iTypeNum++;
	int m_iSelType;
	for (int i=0; i<m_iTypeNum; i++)//�����������Ѱ�Һ�������ͬ�ģ���������ʾ���������С�
	{
		if (m_comboDeviceType.GetItemData(i) == m_struDeviceCfg.byDVRType)//����±�Ϊi�����е�data���ݣ�����byDVRType�ȽϿ��Ƿ���ȡ�
		{
			m_iSelType = i;
			break;
		}
	}
	m_comboDeviceType.SetCurSel(m_iSelType);
	/***********************************�豸����������end*************************************/

	//��ʾ���豸��
	char cTemp[200] = {0};
	ZeroMemory(cTemp, 200);
	memcpy(cTemp, m_struDeviceCfg.sDVRName, NAME_LEN);
	m_csDeviceName.Format("%s", cTemp);
	//m_csDeviceName=m_struDeviceCfg.sDVRName;����д�ǲ��õģ�����֮�䲻��ֱ�Ӹ�ֵ�������ӿ��Կ�������Ϊcstring���ڲ����������鸳ֵ���غ�����
	//��ʾ���豸���к�
	ZeroMemory(cTemp, 200);
	memcpy(cTemp, m_struDeviceCfg.sSerialNumber, SERIALNO_LEN);
	m_csDeviceSerialNo.Format("%s", cTemp);
	//m_csDeviceSerialNo=m_struDeviceCfg.sSerialNumber;
	
	//��ʾ�豸�汾��Ϣ 
	if ((m_struDeviceCfg.dwSoftwareVersion>>24)&0xFF > 0)
	{
		m_csSoftwareVersion.Format("V%d.%d.%d build %02d%02d%02d",\
		(m_struDeviceCfg.dwSoftwareVersion>>24)&0xFF, (m_struDeviceCfg.dwSoftwareVersion>>16)&0xFF, m_struDeviceCfg.dwSoftwareVersion&0xFFFF, (m_struDeviceCfg.dwSoftwareBuildDate>>16)&0xFFFF, (m_struDeviceCfg.dwSoftwareBuildDate>>8)&0xFF,m_struDeviceCfg.dwSoftwareBuildDate&0xFF);
	}
	else
	{
		m_csSoftwareVersion.Format("V%d.%d build %02d%02d%02d",\
		(m_struDeviceCfg.dwSoftwareVersion>>16)&0xFFFF, m_struDeviceCfg.dwSoftwareVersion&0xFFFF, (m_struDeviceCfg.dwSoftwareBuildDate>>16)&0xFFFF, (m_struDeviceCfg.dwSoftwareBuildDate>>8)&0xFF,m_struDeviceCfg.dwSoftwareBuildDate&0xFF);
	}
	m_csDspVersion.Format("V%d.%d build %02d%02d%02d", (m_struDeviceCfg.dwDSPSoftwareVersion>>16)&0xFFFF, m_struDeviceCfg.dwDSPSoftwareVersion&0xFFFF, (m_struDeviceCfg.dwDSPSoftwareBuildDate>>16)&0xFFFF-2000, (m_struDeviceCfg.dwDSPSoftwareBuildDate>>8)&0xFF,m_struDeviceCfg.dwDSPSoftwareBuildDate&0xFF);	
	m_csHardwareVersion.Format("0x%x", m_struDeviceCfg.dwHardwareVersion);
	m_csPanelVersion.Format("V%d", m_struDeviceCfg.dwPanelVersion);
	
	//�Ƿ���������������
	memset(&m_struScaleCfg,0,sizeof(NET_DVR_SCALECFG));
	if (!NET_DVR_GetScaleCFG_V30(g_lUserID, &m_struScaleCfg))
	{
		MessageBox("��ȡ���Ų���ʧ��");		
	}
	else
	{
		m_comboMajorScale.SetCurSel(m_struScaleCfg.dwMajorScale);
		m_comboMinorScale.SetCurSel(m_struScaleCfg.dwMinorScale);
	}


    
	UpdateData(FALSE);
}

/*********************************************************
Function:	GetNetCfg
Desc:		��ȡ�豸������������Ϣ
Input:	
Output:	
Return:	
**********************************************************/
void CDlgDeviceCfg::GetNetCfg()
{
	if (g_lUserID < 0)
	{
		return;
	}

	//��ȡ�����������NET_DVR_GET_NETCFG_V30����
	DWORD dwReturned = 0;
	memset(&m_struNetCfg, 0, sizeof(NET_DVR_NETCFG_V30));
	if (!NET_DVR_GetDVRConfig(g_lUserID, NET_DVR_GET_NETCFG_V30, 0,&m_struNetCfg, sizeof(NET_DVR_NETCFG_V30), &dwReturned))
	{
		MessageBox("��ȡ�������ʧ��!");
	}

	if (m_struNetCfg.byUseDhcp == 0xff)
	{		
		GetDlgItem(IDC_CHK_DHCP)->EnableWindow(FALSE);
		m_bChkDhcp = 0;
		EnableDhcp();
	}
	else
	{
		GetDlgItem(IDC_CHK_DHCP)->EnableWindow(TRUE);
		m_bChkDhcp = m_struNetCfg.byUseDhcp;
		EnableDhcp();
	}

	m_iMtu=m_struNetCfg.struEtherNet[0].wMTU;
	m_iHttpPort=m_struNetCfg.wHttpPortNo;
	m_csDeviceIP = m_struNetCfg.struEtherNet[0].struDVRIP.sIpV4;
	m_csGatewayIP = m_struNetCfg.struGatewayIpAddr.sIpV4;
	m_csIPMask = m_struNetCfg.struEtherNet[0].struDVRIPMask.sIpV4;
	m_csMacAddr.Format("%02x:%02x:%02x:%02x:%02x:%02x", m_struNetCfg.struEtherNet[0].byMACAddr[0],m_struNetCfg.struEtherNet[0].byMACAddr[1],m_struNetCfg.struEtherNet[0].byMACAddr[2],m_struNetCfg.struEtherNet[0].byMACAddr[3],m_struNetCfg.struEtherNet[0].byMACAddr[4],m_struNetCfg.struEtherNet[0].byMACAddr[5]);
	m_csMultiIP = m_struNetCfg.struMulticastIpAddr.sIpV4;
	m_iMainPort = m_struNetCfg.struEtherNet[0].wDVRPort;
	m_comboNetFace.SetCurSel(m_struNetCfg.struEtherNet[0].dwNetInterface-1);
	m_iAlarmHostPort = m_struNetCfg.wAlarmHostIpPort;
	m_csAlarmHostIP=m_struNetCfg.struAlarmHostIpAddr.sIpV4;
	m_csDNSIP1 = m_struNetCfg.struDnsServer1IpAddr.sIpV4;
	m_csDNSIP2 = m_struNetCfg.struDnsServer2IpAddr.sIpV4;
	UpdateData(FALSE);
}

/*********************************************************
Function:	OnButtonSetDeviceCfg
Desc:		��ť�������豸�������ò�����Ϣ��
Input:	
Output:	
Return:	
**********************************************************/
void CDlgDeviceCfg::OnButtonSetDeviceCfg() 
{
	if (g_lUserID < 0)
	{
		MessageBox("���ȵ�¼�豸��");
		return;
	}

	UpdateData(TRUE);
    
	//ң����id������1-255֮�䣬������ֵ�����жϡ�
	if (m_iYKQID < 1 || m_iYKQID > 255)
	{
		MessageBox("ң����id������1-255֮��");
		return ;
	}
	m_struDeviceCfg.dwDVRID = m_iYKQID;
	memcpy(m_struDeviceCfg.sDVRName, m_csDeviceName, NAME_LEN);	
	m_struDeviceCfg.dwRecycleRecord = m_comboCycleRec.GetCurSel();
	//�����豸���ò�������NET_DVR_SET_DEVICECFG����
    if (!NET_DVR_SetDVRConfig(g_lUserID, NET_DVR_SET_DEVICECFG, 0,&m_struDeviceCfg, sizeof(NET_DVR_DEVICECFG)))
	{
		CString err;
		err.Format("���ø��豸�����ò���ʧ��,�������%d",NET_DVR_GetLastError());//�Դ��������ʽ��ʾ����
        
		//err.Format("���ø��豸�����ò���ʧ��,%s",NET_DVR_GetErrorMsg());//�Դ���������Ϣ��Ӣ�ģ�����������
		MessageBox(err);
	}
//     else
// 	{
// 		MessageBox("�豸�������óɹ�");
// 	}

	//�������Ų���
	m_struScaleCfg.dwMajorScale = m_comboMajorScale.GetCurSel();
	m_struScaleCfg.dwMinorScale = m_comboMinorScale.GetCurSel();
	if (!NET_DVR_SetScaleCFG_V30(g_lUserID, &m_struScaleCfg))
	{
        AfxMessageBox("���������Ƿ�����ʧ��!");		
	}
		
}

/*********************************************************
Function:	OnButtonSetNetCfg
Desc:		��ť�������豸���������Ϣ��
Input:	
Output:	
Return:	
**********************************************************/
void CDlgDeviceCfg::OnButtonSetNetCfg() 
{
	if (g_lUserID < 0)
	{
		MessageBox("���ȵ�¼�豸��");
		return;
	}

	UpdateData(TRUE);

	char szLan[128] = {0};
	
	if (m_iMainPort<1 || m_iMainPort>65535)
	{
		g_StringLanType(szLan, "�Ƿ����豸ͨѶ�˿ں�,������1-65535������", "Illegal device port number");
		AfxMessageBox(szLan);
		return;
	}

	if (m_iHttpPort<1 || m_iHttpPort>65535)
	{
		g_StringLanType(szLan, "�Ƿ���HTTP�˿ں�,������1-65535������", "Illegal HTTP port number");
		AfxMessageBox(szLan);
		return;
	}

	if (m_iAlarmHostPort<1 || m_iAlarmHostPort>65535)
	{
		g_StringLanType(szLan, "�Ƿ��ĸ澯�����˿ں�,������1-65535������", "Illegal alarmhost port number");
		AfxMessageBox(szLan);
		return;
	}
	
	if ((m_csDeviceIP == "0.0.0.0") || (m_csDeviceIP == "") || !CheckIPStr(m_csDeviceIP))
	{
		g_StringLanType(szLan, "�Ƿ����豸IP��ַ", "Illegal device ip address");
		AfxMessageBox(szLan);
		return;
	}
	
	if (!CheckIPStr(m_csIPMask))
	{
		g_StringLanType(szLan, "�Ƿ��������ַ", "Illegal device MAC address");
		AfxMessageBox(szLan);
		return;
	}

	
	if (!CheckIPStr(m_csMultiIP))
	{
		g_StringLanType(szLan, "�Ƿ��ĶಥIP��ַ", "Illegal Multicast address");
		AfxMessageBox(szLan);
		return;
	}
	
	if (!CheckIPStr(m_csGatewayIP))
	{
		g_StringLanType(szLan, "�Ƿ�������IP��ַ", "Illegal Gateway ip address");
		AfxMessageBox(szLan);
		return;
	}
	
	if (!CheckIPStr(m_csAlarmHostIP))
	{
		g_StringLanType(szLan, "�Ƿ��ĸ澯��������IP��ַ", "Illegal Gateway ip address");
		AfxMessageBox(szLan);
		return;
	}
	
	if (m_comboNetFace.GetCurSel() == 5)//1000M full
	{	
		g_StringLanType(szLan, "��ȷ��Ϊ1000M���磬�����޷��ٵ�½�豸", "Please this is 1000M network, or can not login device again");
		if (IDOK!=MessageBox(szLan, "Waring",IDOK))
		{
			return;
		}
	}
	

	m_struNetCfg.byUseDhcp = m_bChkDhcp;//�����8000�豸�������ǻҵģ��޷����á�
    m_struNetCfg.struEtherNet[0].wDVRPort= m_iMainPort;
	m_struNetCfg.wHttpPortNo=m_iHttpPort;
	
	memcpy(m_struNetCfg.struGatewayIpAddr.sIpV4, m_csGatewayIP,16);
	memcpy(m_struNetCfg.struEtherNet[0].struDVRIP.sIpV4, m_csDeviceIP, 16);
	memcpy(m_struNetCfg.struEtherNet[0].struDVRIPMask.sIpV4, m_csIPMask, 16);
	memcpy(m_struNetCfg.struDnsServer1IpAddr.sIpV4, m_csDNSIP1,16);
	memcpy(m_struNetCfg.struDnsServer2IpAddr.sIpV4, m_csDNSIP2,16);
	memcpy(m_struNetCfg.struAlarmHostIpAddr.sIpV4, m_csAlarmHostIP,16);
	m_struNetCfg.wAlarmHostIpPort = m_iAlarmHostPort;
	memcpy(m_struNetCfg.struMulticastIpAddr.sIpV4, m_csMultiIP,16);
	m_struNetCfg.struEtherNet[0].wMTU = m_iMtu;
	//memcpy(m_struNetCfg.byIpResolver, m_csIPResolver, MAX_DOMAIN_NAME);
	//m_struNetCfg.wIpResolverPort = m_iResolverPort;
	
	//���������������NET_DVR_SET_NETCFG_V30����
    if (!NET_DVR_SetDVRConfig(g_lUserID, NET_DVR_SET_NETCFG_V30, 0,&m_struNetCfg, sizeof(NET_DVR_NETCFG_V30)))
	{
		CString err;
		err.Format("���ø��豸�����ò���ʧ��,�������%d",NET_DVR_GetLastError());
		MessageBox(err);
	}
    else
	{
		MessageBox("����������óɹ�");
	}	
}

/*********************************************************
Function:	OnChkDhcp
Desc:		check���Ƿ�����dhcp�Զ����ip������Ϣ��Ӧ����
Input:	
Output:	
Return:	
**********************************************************/
void CDlgDeviceCfg::OnChkDhcp() 
{
	UpdateData(TRUE);
	EnableDhcp();
}

/*********************************************************
Function:	EnableDhcp
Desc:		�Ƿ�����dhcp���������edit���״̬��Ӧ����
Input:	
Output:	
Return:	
**********************************************************/
void CDlgDeviceCfg::EnableDhcp()
{
	GetDlgItem(IDC_EDIT_DEVICE_IP)->EnableWindow(!m_bChkDhcp);
	GetDlgItem(IDC_EDIT_IP_MASK)->EnableWindow(!m_bChkDhcp);
	GetDlgItem(IDC_EDIT_GATEWAY_IP)->EnableWindow(!m_bChkDhcp);
	GetDlgItem(IDC_EDIT_DNS1)->EnableWindow(!m_bChkDhcp);
	GetDlgItem(IDC_EDIT_DNS2)->EnableWindow(!m_bChkDhcp);
}


