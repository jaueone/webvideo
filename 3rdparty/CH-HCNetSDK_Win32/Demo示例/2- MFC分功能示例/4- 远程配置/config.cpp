// config.cpp : Defines the class behaviors for the application.

/**********************************************************
FileName:    config.cpp
Description: Ӧ�ó����ʼ�࣬�����ﶨ�������󲿷ֵ�ȫ�ֱ���
Date:        
Note: 		 ����ȫ�ֱ���,ʵ��ȫ�ֺ���   
Modification History:      
<version>    <time>         <desc>

***********************************************************/

#include "stdafx.h"
#include "config.h"
#include "configDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//ȫ�ֱ�����ȫ�ֺ�����config.cpp���壬��config.h��extern�������ļ�ֻ��Ҫ����config.h�ļ����ɡ�
STRU_DEVICE_INFO g_struDeviceInfo[MAX_DEVICES];
LONG g_lUserID=-1;
LPNET_DVR_MOTION_V30 g_lpMotionDetect = NULL;
LPNET_DVR_HIDEALARM_V30 g_lpHideAlarm = NULL;
LPNET_DVR_VILOST_V30 g_lpVILost = NULL;

//ȫ�ֺ����ڱ��ļ��ĺ��涨�壬��config.h��extern��
void g_CreateAlarmOutTree(CTreeCtrl &m_treeAlarmOut, int iDeviceIndex, BOOL *bAlarmOut);
void g_StringLanType(char *szDstLan, char *szLanCn, char *szLanEn);
BOOL DoGetDeviceResoureCfg(int iDeviceIndex);
void RefreshIPDevLocalCfg(int iDeviceIndex);
BOOL DoGetIPAlarmInCfg(int iDeviceIndex);
BOOL DoGetIPAlarmOutCfg(int iDeviceIndex);
BOOL CheckIPStr(CString ip);



/////////////////////////////////////////////////////////////////////////////
// CConfigApp

BEGIN_MESSAGE_MAP(CConfigApp, CWinApp)
	//{{AFX_MSG_MAP(CConfigApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigApp construction

/*********************************************************
Function:	CConfigApp
Desc:		Constructor
Input:	
Output:	
Return:	
**********************************************************/
CConfigApp::CConfigApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CConfigApp object

CConfigApp theApp;


/////////////////////////////////////////////////////////////////////////////
// CConfigApp initialization

/*********************************************************
Function:	InitInstance
Desc:		����Ӧ�ó���ʵ����
Input:	
Output:	
Return:	
**********************************************************/
BOOL CConfigApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
    
	//��ʼ��SDK
	NET_DVR_Init();

	CConfigDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

/*********************************************************
Function:	ExitInstance
Desc:		�˳�Ӧ�ó���ʵ��
Input:	
Output:	
Return:	
**********************************************************/
int CConfigApp::ExitInstance() 
{
	//�ͷ�SDK��Դ
	NET_DVR_Cleanup();
	
	return CWinApp::ExitInstance();
}

/*********************************************************
Function:	g_CreateAlarmOutTree
Desc:		�������������
Input:	
Output:	
Return:	
**********************************************************/
void g_CreateAlarmOutTree(CTreeCtrl &m_treeAlarmOut, int iDeviceIndex, BOOL *bAlarmOut)
{
	if (iDeviceIndex == -1)
	{
		return;
	}
	m_treeAlarmOut.DeleteAllItems();
	CString strTemp =_T("");
	CString strChanTmp = _T("");
	int i = 0;
	HTREEITEM hChanItem = NULL;
	HTREEITEM hFirstItem = NULL;
	BOOL m_bAlarmOut[MAX_ALARMOUT_V30];
	DWORD dwID = 0;
	for (i = 0; i < MAX_ALARMOUT_V30; i++)
	{
		m_bAlarmOut[i] = BOOL(*(bAlarmOut+i));
		if ((i <  g_struDeviceInfo[iDeviceIndex].iAlarmOutNum))
		{
			strTemp.Format(ALARM_OUT_FORMAT, i+1);
			hChanItem =  m_treeAlarmOut.InsertItem(strTemp, 0, 0, TVI_ROOT);
			if (hFirstItem == NULL)
			{
				hFirstItem = hChanItem;
			}
			m_treeAlarmOut.SetItemData(hChanItem, 0*1000 + i);	
			if (m_bAlarmOut[i])
			{
				m_treeAlarmOut.SetCheck(hChanItem, TRUE);
			}
		}
		else if (i >= MAX_ANALOG_ALARMOUT)
		{
			dwID = g_struDeviceInfo[iDeviceIndex].struAlarmOutCfg.struIPAlarmOutInfo[i-MAX_ANALOG_ALARMOUT].byIPID;
			if (dwID != 0)
			{
				strTemp.Format(IP_ALARM_OUT_NAME, i+1-MAX_ANALOG_ALARMOUT);
				hChanItem =  m_treeAlarmOut.InsertItem(strTemp, 0, 0, TVI_ROOT);
				m_treeAlarmOut.SetItemData(hChanItem, 1*1000 + i);	
				if (m_bAlarmOut[i])
				{
					m_treeAlarmOut.SetCheck(hChanItem, TRUE);
				}
			}
		}
	}
	m_treeAlarmOut.SelectItem(hFirstItem);//��ֹ����ˢ��
	m_treeAlarmOut.Expand(m_treeAlarmOut.GetRootItem(),TVE_EXPAND);
}

/*********************************************************
Function:	g_StringLanType
Desc:		��õ�ǰ������������/get the current operation language string type
Input:	szLanCn, Chinese string; szLanEn, English string;
Output:	szDstLan, current string
Return:	none
**********************************************************/
void g_StringLanType(char *szDstLan, char *szLanCn, char *szLanEn)
{
#ifdef DEMO_LAN_CN
	sprintf(szDstLan, "%s", szLanCn);	
#else	
	sprintf(szDstLan, "%s", szLanEn);
#endif
	return;
    WORD wLangID=PRIMARYLANGID(::GetSystemDefaultLangID());
    switch(wLangID)
    {
	case LANG_ENGLISH:
		sprintf(szDstLan, "%s", szLanEn);
		break;
	case LANG_CHINESE:
		sprintf(szDstLan, "%s", szLanCn);
		break;
	default:
		sprintf(szDstLan, "%s", szLanEn);
		break;
	}	
}


/*********************************************************
Function:	DoGetDeviceResoureCfg
Desc:		��ȡ�豸ip�������
Input:	iDeviceIndex, the device index
Output:	none
Return:	TRUE/FALSE
**********************************************************/
BOOL DoGetDeviceResoureCfg(int iDeviceIndex)
{
	int i = 0;
	CString csTemp;
	DWORD dwChanShow = 0;
	LPNET_DVR_IPPARACFG lpIPAccessCfg = &g_struDeviceInfo[iDeviceIndex].struIPParaCfg;
	DWORD dwReturned = 0;
	memset(lpIPAccessCfg, 0, sizeof(NET_DVR_IPPARACFG));
	// ip��������
	g_struDeviceInfo[iDeviceIndex].bIPRet = NET_DVR_GetDVRConfig(g_struDeviceInfo[iDeviceIndex].lLoginID, NET_DVR_GET_IPPARACFG, 0, lpIPAccessCfg, sizeof(NET_DVR_IPPARACFG), &dwReturned);
	if (!g_struDeviceInfo[iDeviceIndex].bIPRet)
	{	//8000���豸/device no support ip access//hxb��8000�豸����ͨ������Ĭ�ϴ򿪵ģ������޷�������8000�豸����NET_DVR_GetDVRConfig��NET_DVR_GET_IPPARACFG���ķ���ֵ��FALSE.
		g_struDeviceInfo[iDeviceIndex].lFirstEnableChanIndex = 0;
		for (i = 0; i < MAX_CHANNUM_V30; i++)
		{	
			if (i < g_struDeviceInfo[iDeviceIndex].iDeviceChanNum)
			{
				g_struDeviceInfo[iDeviceIndex].struChanInfo[i].iDeviceIndex = iDeviceIndex;
				g_struDeviceInfo[iDeviceIndex].struChanInfo[i].iChanIndex = i;
				g_struDeviceInfo[iDeviceIndex].struChanInfo[i].bEnable = TRUE;
				sprintf(g_struDeviceInfo[iDeviceIndex].struChanInfo[i].chChanName, ANALOG_C_FORMAT, i+g_struDeviceInfo[iDeviceIndex].iStartChan);	
			}
			else//clear the state of other channel
			{
				g_struDeviceInfo[iDeviceIndex].struChanInfo[i].iDeviceIndex = -1;
				g_struDeviceInfo[iDeviceIndex].struChanInfo[i].iChanIndex = -1;
				g_struDeviceInfo[iDeviceIndex].struChanInfo[i].bEnable = FALSE;
				sprintf(g_struDeviceInfo[iDeviceIndex].struChanInfo[i].chChanName, "");	
			}
		}
	} 
	else
	{
		RefreshIPDevLocalCfg(iDeviceIndex);
	}
	
	return g_struDeviceInfo[iDeviceIndex].bIPRet;
}

/*********************************************************
Function:	RefreshIPDevLocalCfg
Desc:		ˢ�±����豸����ṹ�е��豸��Ϣ/refresh the ip device configure of local device configure
Input:	iDeviceIndex, the device index in the local device group
Output:	none
Return:	none
**********************************************************/
void RefreshIPDevLocalCfg(int iDeviceIndex)
{
	LPNET_DVR_IPPARACFG lpIPAccessCfg = &g_struDeviceInfo[iDeviceIndex].struIPParaCfg;
	DWORD dwChanShow = 0;
	int iIPChanIndex = 0;
	int i = 0;
	g_struDeviceInfo[iDeviceIndex].iEnableChanNum = 0;
	
	for (i = 0; i < MAX_CHANNUM_V30; i++)
	{
		dwChanShow = i+g_struDeviceInfo[iDeviceIndex].iStartChan;
		if (i < g_struDeviceInfo[iDeviceIndex].iDeviceChanNum)
		{//analog devices
			if (lpIPAccessCfg->byAnalogChanEnable[i])
			{
				g_struDeviceInfo[iDeviceIndex].struChanInfo[i].iDeviceIndex = iDeviceIndex;
				g_struDeviceInfo[iDeviceIndex].struChanInfo[i].iChanIndex = i;
				g_struDeviceInfo[iDeviceIndex].struChanInfo[i].bEnable = TRUE;
				sprintf(g_struDeviceInfo[iDeviceIndex].struChanInfo[i].chChanName, ANALOG_C_FORMAT, i+g_struDeviceInfo[iDeviceIndex].iStartChan);	
				g_struDeviceInfo[iDeviceIndex].iEnableChanNum ++;
			}
			else
			{
				g_struDeviceInfo[iDeviceIndex].struChanInfo[i].iDeviceIndex = -1;
				g_struDeviceInfo[iDeviceIndex].struChanInfo[i].iChanIndex = -1;
				g_struDeviceInfo[iDeviceIndex].struChanInfo[i].bEnable = FALSE;	
				g_struDeviceInfo[iDeviceIndex].struChanInfo[i].bAlarm = FALSE;
				g_struDeviceInfo[iDeviceIndex].struChanInfo[i].bLocalManualRec = FALSE;
				g_struDeviceInfo[iDeviceIndex].struChanInfo[i].lRealHandle = -1;
				sprintf(g_struDeviceInfo[iDeviceIndex].struChanInfo[i].chChanName, ANALOG_C_FORMAT, "");	
			}
		}
		else if (i>=MAX_ANALOG_CHANNUM && i<=MAX_CHANNUM_V30)
		{
			iIPChanIndex = i-MAX_ANALOG_CHANNUM;
			//id��0������ͨ��Ϊ��Чͨ��/
			// 			AddLog(-1, OPERATION_SUCC_T, "IPc%d id%d chan[%d] ", iIPChanIndex,\
			// 				lpIPAccessCfg->struIPChanInfo[iIPChanIndex].byIPID, lpIPAccessCfg->struIPChanInfo[iIPChanIndex].byChannel);
			if (lpIPAccessCfg->struIPChanInfo[iIPChanIndex].byIPID != 0)
			{
				g_struDeviceInfo[iDeviceIndex].struChanInfo[i].iDeviceIndex = iDeviceIndex;
				g_struDeviceInfo[iDeviceIndex].struChanInfo[i].iChanIndex = i;
				g_struDeviceInfo[iDeviceIndex].struChanInfo[i].bEnable = TRUE;//
				if (lpIPAccessCfg->struIPChanInfo[iIPChanIndex].byEnable)
				{
					g_struDeviceInfo[iDeviceIndex].struChanInfo[i].dwImageType = CHAN_ORIGINAL;
				}
				else
				{
					g_struDeviceInfo[iDeviceIndex].struChanInfo[i].dwImageType = CHAN_OFF_LINE;
				}
				sprintf(g_struDeviceInfo[iDeviceIndex].struChanInfo[i].chChanName, DIGITAL_C_FORMAT, dwChanShow-MAX_ANALOG_CHANNUM);	
				g_struDeviceInfo[iDeviceIndex].iEnableChanNum ++;
			}
			else
			{
				g_struDeviceInfo[iDeviceIndex].struChanInfo[i].iDeviceIndex = -1;
				g_struDeviceInfo[iDeviceIndex].struChanInfo[i].iChanIndex = -1;
				g_struDeviceInfo[iDeviceIndex].struChanInfo[i].bEnable = FALSE;	
				g_struDeviceInfo[iDeviceIndex].struChanInfo[i].bAlarm = FALSE;
				g_struDeviceInfo[iDeviceIndex].struChanInfo[i].bLocalManualRec = FALSE;
				g_struDeviceInfo[iDeviceIndex].struChanInfo[i].lRealHandle = -1;
				sprintf(g_struDeviceInfo[iDeviceIndex].struChanInfo[i].chChanName, "");	
			}
		}
		else
		{
			g_struDeviceInfo[iDeviceIndex].struChanInfo[i].iDeviceIndex = -1;
			g_struDeviceInfo[iDeviceIndex].struChanInfo[i].iChanIndex = -1;
			g_struDeviceInfo[iDeviceIndex].struChanInfo[i].bEnable = FALSE;	
			g_struDeviceInfo[iDeviceIndex].struChanInfo[i].bAlarm = FALSE;
			g_struDeviceInfo[iDeviceIndex].struChanInfo[i].bLocalManualRec = FALSE;
			g_struDeviceInfo[iDeviceIndex].struChanInfo[i].lRealHandle = -1;
			sprintf(g_struDeviceInfo[iDeviceIndex].struChanInfo[i].chChanName, "");	
		}
	}
	for (i=0; i<MAX_CHANNUM_V30; i++)
	{
		if (g_struDeviceInfo[iDeviceIndex].struChanInfo[i].bEnable)
		{
			g_struDeviceInfo[iDeviceIndex].lFirstEnableChanIndex = i;
			break;
		}
	}
}

/*********************************************************
Function:	DoGetIPAlarmInCfg
Desc:		��ȡ�豸ip������������
Input:	iDeviceIndex, the device index
Output:	none
Return:	TRUE/FALSE
**********************************************************/
BOOL DoGetIPAlarmInCfg(int iDeviceIndex)
{
	int i = 0;
	char szTmp[1024] = {0};
	char szTmp1[32] = {0};
	DWORD dwReturned = 0;
	memset(&g_struDeviceInfo[iDeviceIndex].struAlarmInCfg, 0, sizeof(NET_DVR_IPALARMINCFG));
	g_struDeviceInfo[iDeviceIndex].bIPRet = NET_DVR_GetDVRConfig(g_struDeviceInfo[iDeviceIndex].lLoginID, NET_DVR_GET_IPALARMINCFG, 0, &g_struDeviceInfo[iDeviceIndex].struAlarmInCfg, sizeof(NET_DVR_IPALARMINCFG), &dwReturned);
	
	return g_struDeviceInfo[iDeviceIndex].bIPRet;
}

/*********************************************************
Function:	DoGetIPAlarmOutCfg
Desc:		��ȡ�豸ip�����������
Input:	iDeviceIndex, the device index
Output:	none
Return:	TRUE/FALSE
**********************************************************/
BOOL DoGetIPAlarmOutCfg(int iDeviceIndex)
{
	int i = 0;
	char szTmp[1024] = {0};
	char szTmp1[32] = {0};
	DWORD dwReturned = 0;
	memset(&g_struDeviceInfo[iDeviceIndex].struAlarmOutCfg, 0, sizeof(NET_DVR_IPALARMOUTCFG));
	g_struDeviceInfo[iDeviceIndex].bIPRet = NET_DVR_GetDVRConfig(g_struDeviceInfo[iDeviceIndex].lLoginID, NET_DVR_GET_IPALARMOUTCFG, 0, &g_struDeviceInfo[iDeviceIndex].struAlarmOutCfg, sizeof(NET_DVR_IPALARMOUTCFG), &dwReturned);
	
	return g_struDeviceInfo[iDeviceIndex].bIPRet;
}

/*********************************************************
Function:	CheckIPStr
Desc:		���ip��ַ����Ч��
Input:	ip, Ҫ����ip��ַ
Output:	
Return:	TRUE,IP��ַ��ȷ;FALSE,IP��ַ����;
**********************************************************/
BOOL CheckIPStr(CString ip)
{	//���IP��ַ�������ȷ��
	char str[20];
	strcpy(str,ip);
	char a;
	int dot=0;
	int a3,a2,a1,a0,i = 0;
	a3 = a2 = a1 = a0 = -1;
	if (strlen(str)==0)
		return TRUE;
	while (a=str[i++])
	{
		if ((a==' ')||(a=='.')||((a>='0')&&(a<='9')))
		{
			if (a=='.')
				dot++;
		}
		else
			return FALSE;
	}
	if (dot!=3)
	{
		return FALSE;
	}
	else
	{
		sscanf(str,"%d.%d.%d.%d",&a3,&a2,&a1,&a0);
		if ((a0>255)||(a1>255)||(a2>255)||(a3>255))
		{
			return FALSE;
		}
		if ((a0 < 0) || (a1 < 0) || (a2 < 0) || (a3 < 0))
		{
			return FALSE;
		}
	}
	return TRUE;
}

