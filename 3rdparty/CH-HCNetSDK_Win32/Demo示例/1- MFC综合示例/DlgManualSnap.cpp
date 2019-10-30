// DlgManualSnap.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgManualSnap.h"
#include "DlgPtzCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgManualSnap dialog
CDlgManualSnap *g_lManualSnapDlg = NULL;


CDlgManualSnap::CDlgManualSnap(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgManualSnap::IDD, pParent)
    , m_iChannelID(0)
{
	//{{AFX_DATA_INIT(CDlgManualSnap)
	m_bOSDClose = FALSE;
	m_iVehicleIndex = 0;
	m_csTrafficLight = _T("");
	m_byLaneNo = 0;
	m_dwSnapWaitTime = 0;
	m_dwIntervalTime1 = 0;
	m_dwIntervalTime2 = 0;
	m_dwIntervalTime3 = 0;
	m_dwIntervalTime4 = 0;
	//}}AFX_DATA_INIT
	m_lServerID = -1; // �û�ID
    m_iDevIndex = -1; // �豸����
    m_lChannel = -1;
	m_lPlayHandle = -1;
	m_bCloseIn = FALSE;
	memset(&m_struManualSnap, 0, sizeof(m_struManualSnap));
    memset(&m_struResult, 0, sizeof(m_struResult));
}

CDlgManualSnap::~CDlgManualSnap()
{

	StopRealPlay();
}

void CDlgManualSnap::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgManualSnap)
    DDX_Control(pDX, IDC_COMBO_SNAP_TIMES, m_comSnapTimes);
    DDX_Control(pDX, IDC_COMBO_VEHICLE_COLOR, m_comVehicleColor);
    DDX_Control(pDX, IDC_REALTIME_LIST, m_RealtimeList);
    DDX_Check(pDX, IDC_CHECK_CLOSE_OSD, m_bOSDClose);
    DDX_Text(pDX, IDC_EDIT_VEHICLE_INDEX, m_iVehicleIndex);
    DDX_Text(pDX, IDC_EDIT_TRAFFIC_LIGHT, m_csTrafficLight);
    DDX_Text(pDX, IDC_EDIT_LANE_NO, m_byLaneNo);
    DDX_Text(pDX, IDC_EDIT_SNAP_TIME, m_dwSnapWaitTime);
    DDX_Text(pDX, IDC_EDIT_TIME1, m_dwIntervalTime1);
    DDX_Text(pDX, IDC_EDIT_TIME2, m_dwIntervalTime2);
    DDX_Text(pDX, IDC_EDIT_TIME3, m_dwIntervalTime3);
    DDX_Text(pDX, IDC_EDIT_TIME4, m_dwIntervalTime4);
    //}}AFX_DATA_MAP
    DDX_Text(pDX, IDC_EDIT_CHANNEL_ID, m_iChannelID);
}


BEGIN_MESSAGE_MAP(CDlgManualSnap, CDialog)
	//{{AFX_MSG_MAP(CDlgManualSnap)
	ON_BN_CLICKED(IDC_BTN_MANUAL_SNAP, OnBtnManualSnap)
	ON_BN_CLICKED(IDC_BTN_PTZ_CTRL, OnBtnPTZCtrl)
	ON_BN_CLICKED(IDC_BTN_CONTINUOUS_SHOOT, OnBtnContinuousShoot)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CDlgManualSnap message handlers

long CDlgManualSnap::StartRealPlay()
{
    NET_DVR_CLIENTINFO struClientInfo;
    memset(&struClientInfo, 0, sizeof(NET_DVR_CLIENTINFO));
    struClientInfo.lChannel = m_lChannel;
    struClientInfo.hPlayWnd = GetDlgItem(IDC_PLAYWND)->GetSafeHwnd();
    m_lPlayHandle = NET_DVR_RealPlay_V30(m_lServerID, &struClientInfo, NULL, NULL, TRUE); 
    if (m_lPlayHandle == -1) 
    {
        AfxMessageBox("Fail to Preview");
        return m_lPlayHandle;
    }
//     else
//     {
//         if (!NET_DVR_RigisterDrawFun(m_lPlayHandle, DrawManualSnapShow, 0)) 
//         {
//             g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_RigisterDrawFun Fail");
//             AfxMessageBox("Fail to set DrawFun callback");
//         }
//         
//     }
    return m_lPlayHandle;
}

BOOL CDlgManualSnap::StopRealPlay()
{
    BOOL bRet;
    if (m_lPlayHandle >= 0)
    {
        bRet = NET_DVR_StopRealPlay(m_lPlayHandle);
        if (!bRet)
        {
            AfxMessageBox("Fail to stop preview");
        }
    }
    return bRet;
}

BOOL CDlgManualSnap::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	char szLanTemp[128] = {0};

    GetDlgItem(IDC_PLAYWND)->GetWindowRect(&m_rcWndRect);
    m_bCloseIn = TRUE;

	if (m_lPlayHandle >= 0)
    {
        StopRealPlay();
    }
	
    StartRealPlay();

	m_RealtimeList.SetExtendedStyle(m_RealtimeList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_SUBITEMIMAGES);
	g_StringLanType(szLanTemp, "ͨ��ʱ��", "Time");
    m_RealtimeList.InsertColumn(0, szLanTemp, LVCFMT_LEFT, 120, -1);
	g_StringLanType(szLanTemp, "������ɫ", "License Plate Color");
    m_RealtimeList.InsertColumn(1, szLanTemp, LVCFMT_LEFT, 60, -1);
	g_StringLanType(szLanTemp, "��������", "Vehicle Type");
	m_RealtimeList.InsertColumn(2, szLanTemp, LVCFMT_LEFT, 60, -1);
	g_StringLanType(szLanTemp, "������Ϣ", "License Info");
    m_RealtimeList.InsertColumn(3, szLanTemp, LVCFMT_LEFT, 75, -1);
	g_StringLanType(szLanTemp, "������Ϣ", "Vehicle Info");
    m_RealtimeList.InsertColumn(4, szLanTemp, LVCFMT_LEFT, 110, -1);
	g_StringLanType(szLanTemp, "�����ٶ�", "Vehicle Speed");
    m_RealtimeList.InsertColumn(5, szLanTemp, LVCFMT_LEFT, 60, -1);
	g_StringLanType(szLanTemp, "����ͼƬ", "Vehicle Image");
    m_RealtimeList.InsertColumn(6, szLanTemp, LVCFMT_LEFT, 200, -1);
	g_StringLanType(szLanTemp, "����ͼƬ", "License Plate Image");
    m_RealtimeList.InsertColumn(7, szLanTemp, LVCFMT_LEFT, 200, -1);
	g_StringLanType(szLanTemp, "������", "Lane No.");
    m_RealtimeList.InsertColumn(8, szLanTemp, LVCFMT_LEFT, 60, -1);
	g_StringLanType(szLanTemp, "Υ����Ϊ", "Violation Rule");
	m_RealtimeList.InsertColumn(9, szLanTemp, LVCFMT_LEFT, 150, -1);
	g_StringLanType(szLanTemp, "����", "Vehicle Sign");
	m_RealtimeList.InsertColumn(10, szLanTemp, LVCFMT_LEFT, 100, -1);
	g_StringLanType(szLanTemp, "��������", "License Plate Bright");
    m_RealtimeList.InsertColumn(11, szLanTemp, LVCFMT_LEFT, 60, -1);
	g_StringLanType(szLanTemp, "���ƺ���", "License Plate No.");
    m_RealtimeList.InsertColumn(12, szLanTemp, LVCFMT_LEFT, 60, -1);

	UpdateData(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void g_VehicleLogo(BYTE byVehicleLogoType, char* szLogoLan)
{
	switch(byVehicleLogoType)
	{
	case 1:
		g_StringLanType(szLogoLan, "����", "Volkswagen");
		break;
	case 2:
		g_StringLanType(szLogoLan, "���", "Buick");
		break;
	case 3:
		g_StringLanType(szLogoLan, "����", "BMW");
		break;
	case 4:
		g_StringLanType(szLogoLan, "����", "Honda");
		break;
	case 5:
		g_StringLanType(szLogoLan, "����", "Peugeot");
		break;
	case 6:
		g_StringLanType(szLogoLan, "����", "Toyota");
		break;
	case 7:
		g_StringLanType(szLogoLan, "����", "Ford");
		break;
	case 8:
		g_StringLanType(szLogoLan, "�ղ�", "Nissan");
		break;
	case 9:
		g_StringLanType(szLogoLan, "�µ�", "Audi");
		break;
	case 10:
		g_StringLanType(szLogoLan, "���Դ�", "Mazda");
		break;
	case 11:
		g_StringLanType(szLogoLan, "ѩ����", "Chevrolet");
		break;
	case 12:
		g_StringLanType(szLogoLan, "ѩ����", "Citroen");
		break;
	case 13:
		g_StringLanType(szLogoLan, "�ִ�", "Hyundai");
		break;
	case 14:
		g_StringLanType(szLogoLan, "����", "Qirui");
		break;
	case 15:
		g_StringLanType(szLogoLan, "����", "KIA");
		break;
	case 16:
		g_StringLanType(szLogoLan, "����", "ROEWE");
		break;
	case 17:
		g_StringLanType(szLogoLan, "����", "MITSUBISHI");
		break;
	case 18:
		g_StringLanType(szLogoLan, "˹�´�", "SKODA");
		break;
	case 19:
		g_StringLanType(szLogoLan, "����", "GEELY");
		break;
	case 20:
		g_StringLanType(szLogoLan, "�л�", "ZHONGHUA");
		break;
	case 21:
		g_StringLanType(szLogoLan, "�ֶ���", "VOLVO");
		break;
	case 22:
		g_StringLanType(szLogoLan, "�׿���˹", "LEXUS");
		break;
	case 23:
		g_StringLanType(szLogoLan, "������", "FIAT");
		break;
	case 24:
		g_StringLanType(szLogoLan, "�ۺ�", "EMGRAND");
		break;
	case 25:
		g_StringLanType(szLogoLan, "����", "DONGFENG");
		break;
	case 26:
		g_StringLanType(szLogoLan, "���ǵ�", "BYD");
		break;
	case 27:
		g_StringLanType(szLogoLan, "��ľ", "SUZUKI");
		break;
	case 28:
		g_StringLanType(szLogoLan, "��", "JINBEI");
		break;
	case 29:
		g_StringLanType(szLogoLan, "����", "HAIMA");
		break;
	case 30:
		g_StringLanType(szLogoLan, "����", "SGMW");
		break;

	case 31:
		g_StringLanType(szLogoLan, "����", "JAC");
		break;
	case 32:
		g_StringLanType(szLogoLan, "˹��³", "SUBARU");
		break;
	case 33:
		g_StringLanType(szLogoLan, "Ӣ��", "ENGLON");
		break;
	case 34:
		g_StringLanType(szLogoLan, "����", "GREATWALL");
		break;
	case 35:
		g_StringLanType(szLogoLan, "����", "HAFEI");
		break;
	case 36:
		g_StringLanType(szLogoLan, "��ʮ��", "ISUZU");
		break;
	case 37:
		g_StringLanType(szLogoLan, "����", "SOUEAST");
		break;
	case 38:
		g_StringLanType(szLogoLan, "����", "CHANA");
		break;
	case 39:
		g_StringLanType(szLogoLan, "����", "FOTON");
		break;
	case 40:
		g_StringLanType(szLogoLan, "����", "XIALI");
		break;
	case 41:
		g_StringLanType(szLogoLan, "����", "BENZ");
		break;
	case 42:
		g_StringLanType(szLogoLan, "һ��", "FAW");
		break;
	case 43:
		g_StringLanType(szLogoLan, "��ά��", "NAVECO");
		break;
	case 44:
		g_StringLanType(szLogoLan, "����", "LIFAN");
		break;
	case 45:
		g_StringLanType(szLogoLan, "һ������", "BESTURN");
		break;
	case 46:
		g_StringLanType(szLogoLan, "�ʹ�", "CROWN");
		break;
	case 47:
		g_StringLanType(szLogoLan, "��ŵ", "RENAULT");
		break;
	case 48:
		g_StringLanType(szLogoLan, "JMC", "JMC");
		break;
	case 49:
		g_StringLanType(szLogoLan, "MG����", "MG");
		break;
	case 50:
		g_StringLanType(szLogoLan, "����", "KAMA");
		break;
	case 51:
		g_StringLanType(szLogoLan, "��̩", "ZOTYE");
		break;
	case 52:
		g_StringLanType(szLogoLan, "����", "CHANGHE");
		break;
	case 53:
		g_StringLanType(szLogoLan, "���Ž���", "XMKINGLONG");
		break;
	case 54:
		g_StringLanType(szLogoLan, "�Ϻ�����", "HUIZHONG");
		break;
	case 55:
		g_StringLanType(szLogoLan, "���ݽ���", "SZKINGLONG");
		break;
	case 56:
		g_StringLanType(szLogoLan, "����", "HIGER");
		break;
	case 57:
		g_StringLanType(szLogoLan, "��ͨ", "YUTONG");
		break;
	case 58:
		g_StringLanType(szLogoLan, "�й�����", "CNHTC");
		break;
	case 59:
		g_StringLanType(szLogoLan, "�����ؿ�", "BEIBEN");
		break;
	case 60:
		g_StringLanType(szLogoLan, "��������", "XINGMA");
		break;
	case 61:
		g_StringLanType(szLogoLan, "Ծ��", "YUEJIN");
		break;
	case 62:
		g_StringLanType(szLogoLan, "�ƺ�", "HUANGHAI");
		break;
	default:
		g_StringLanType(szLogoLan, "����", "Other");
		break;
	}
}

void CDlgManualSnap::OnBtnManualSnap()
{
	UpdateData(TRUE);
	char szLan[128] = {0};
	CString csTemp;

	char szFileName[256] = {0};
	char szSubPicFilePath[256] = {0};  
	char szSubPlateFilePath[256] = {0};
	char szPicDataTime[256] = {0};
	char szPlateDataTime[256] = {0};
	char szPlateFileName[256] = {0};

	m_struManualSnap.byOSDEnable = m_bOSDClose;
    m_struManualSnap.byChannel = m_iChannelID;
	m_struResult.pBuffer1 = new BYTE[2 * 1024 * 1024];
    memset(m_struResult.pBuffer1, 0, 2 * 1024 * 1024);
	
	m_struResult.pBuffer2 = new BYTE[1024 * 1024];
    memset(m_struResult.pBuffer2, 0, 1024 * 1024);

	if (!NET_DVR_ManualSnap(m_lServerID, &m_struManualSnap, &m_struResult))
    {
		g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_ManualSnap");
        return;
    }  
    else
	{
		//ͨ��ʱ��
        csTemp.Format("%s", m_struResult.byAbsTime);
        m_RealtimeList.InsertItem(0, csTemp, -1);
        
		//��������
		if (0 != m_struResult.byVehicleType)
		{
			switch (m_struResult.byVehicleType)
			{
			case 0:
				g_StringLanType(szLan, "δ֪", "UnKnow");		
				csTemp.Format("%s", szLan);
				break;
			case 1:
				g_StringLanType(szLan, "�ͳ�", "Passenger Carriage");	
				csTemp.Format("%s", szLan);
				break;
			case 2:
				g_StringLanType(szLan, "����", "Freight Car");	
				csTemp.Format("%s", szLan);
				break;
			case 3:
				g_StringLanType(szLan, "�γ�", "Car");
				csTemp.Format("%s", szLan);
				break;
			case 4:
				g_StringLanType(szLan, "�����", "Microbus");
				csTemp.Format("%s", szLan);
				break;
			case 5:
				g_StringLanType(szLan, "С����", "Little Freight Car");
				csTemp.Format("%s", szLan);
				break;
			case 6:
				g_StringLanType(szLan, "����", "Passer");	
				csTemp.Format("%s", szLan);
				break;
			case 7:
				g_StringLanType(szLan, "���ֳ�", "Two-wheeler");	
				csTemp.Format("%s", szLan);
				break;
			case 8:
				g_StringLanType(szLan, "���ֳ�", "Three-wheeler");	
				csTemp.Format("%s", szLan);
				break;
			default:
				g_StringLanType(szLan, "δ֪", "UnKnow");		
				csTemp.Format("%s", szLan);
				break;
			}
		}
		else
		{
			//��������
			switch (m_struResult.struVehicleInfo.byVehicleType)
			{
			case 0:
				g_StringLanType(szLan, "��������", "Other Vehicles");	
				csTemp.Format("%s", szLan);
				//pDatabaseParam->m_csVehicleType.Format("��������");
				break;
			case 1:
				g_StringLanType(szLan, "С�ͳ�", "Small Cars");	
				csTemp.Format("%s", szLan);
				//pDatabaseParam->m_csVehicleType.Format("С�ͳ�");
				break;
			case 2:
				g_StringLanType(szLan, "���ͳ�", "Oversize Vehicle");	
				csTemp.Format("%s", szLan);
				//pDatabaseParam->m_csVehicleType.Format("���ͳ�");
				break;
			case 3:
				g_StringLanType(szLan, "����", "Passer");	
				csTemp.Format("%s", szLan);
				//pDatabaseParam->m_csVehicleType.Format("����");
				break;
			case 4:
				g_StringLanType(szLan, "���ֳ�", "Two-wheeler");	
				csTemp.Format("%s", szLan);
				//pDatabaseParam->m_csVehicleType.Format("���ֳ�");
				break;
			case 5:
				g_StringLanType(szLan, "���ֳ�", "Three-wheeler");	
				csTemp.Format("%s", szLan);
				//pDatabaseParam->m_csVehicleType.Format("���ֳ�");
				break;
			default:
				g_StringLanType(szLan, "δ֪", "UnKnow");	
				csTemp.Format("%s", szLan);
				//pDatabaseParam->m_csVehicleType.Format("δ֪");
				break;
			}
		}
		m_RealtimeList.SetItemText(0, 2, csTemp);

		//������ɫ
		switch(m_struResult.struPlateInfo.byColor)
		{
		case VCA_BLUE_PLATE:
			g_StringLanType(szLan, "��ɫ", "Blue");	
			csTemp.Format("%s", szLan);
			//csTemp.Format("��ɫ");
			break;
			
		case VCA_YELLOW_PLATE:
			g_StringLanType(szLan, "��ɫ", "Yellow");	
			csTemp.Format("%s", szLan);
			//csTemp.Format("��ɫ");
			break;
			
		case VCA_WHITE_PLATE:
			g_StringLanType(szLan, "��ɫ", "White");	
			csTemp.Format("%s", szLan);
			//csTemp.Format("��ɫ");
			break;
			
		case VCA_BLACK_PLATE:
			g_StringLanType(szLan, "��ɫ", "Black");	
			csTemp.Format("%s", szLan);
			//csTemp.Format("��ɫ");
			break;
			
		default:
			g_StringLanType(szLan, "δ֪", "UnKnow");	
			csTemp.Format("%s", szLan);
			break;
		} 
		m_RealtimeList.SetItemText(0, 1, csTemp);

		//�������
        m_iVehicleIndex = m_struResult.struVehicleInfo.dwIndex;

		//������ɫ
		m_comVehicleColor.SetCurSel(m_struResult.struVehicleInfo.byColor);

		//������Ϣ
        csTemp.Format("%s", m_struResult.struPlateInfo.sLicense);
        m_RealtimeList.SetItemText(0, 3, csTemp);

		//��������
        switch(m_struResult.struPlateInfo.byPlateType)
        {
        case VCA_STANDARD92_PLATE:
			g_StringLanType(szLan, "��׼���ó������", "civil and military ");	
			csTemp.Format("%s", szLan);
            //csTemp.Format("��׼���ó������");
            break;
            
        case VCA_STANDARD02_PLATE:
			g_StringLanType(szLan, "02ʽ���ó���", "02 type of civil license");	
			csTemp.Format("%s", szLan);
            //csTemp.Format("02ʽ���ó���");
            break;
            
        case VCA_WJPOLICE_PLATE:
			g_StringLanType(szLan, "�侯��", "Police Car");	
			csTemp.Format("%s", szLan);
            //csTemp.Format("�侯��");
            break;
            
        case VCA_JINGCHE_PLATE:
			g_StringLanType(szLan, "����", "Police Wagon");	
			csTemp.Format("%s", szLan);
            //csTemp.Format("����");
            break;
            
        case STANDARD92_BACK_PLATE:
			g_StringLanType(szLan, "���ó�˫��β��", "Double license");	
			csTemp.Format("%s", szLan);
            //csTemp.Format("���ó�˫��β��");
            break;
			
		case VCA_SHIGUAN_PLATE:
			g_StringLanType(szLan, "ʹ�ݳ���", "Embassy license");	
			csTemp.Format("%s", szLan);
			//csTemp.Format("ʹ�ݳ���");
			break;
			
		case VCA_NONGYONG_PLATE:
			g_StringLanType(szLan, "ũ�ó���", "Agricultural license");	
			csTemp.Format("%s", szLan);
			//csTemp.Format("ũ�ó���");
			break;
		case VCA_MOTO_PLATE:
			g_StringLanType(szLan, "Ħ�г�����", "Motorcycle license");	
			csTemp.Format("%s", szLan);
			//csTemp.Format("Ħ�г�����");
			break;
        case NEW_ENERGY_PLATE:
            g_StringLanType(szLan, "����Դ������", "new energy license");	
            csTemp.Format("%s", szLan);
			break;
            
        default:
			g_StringLanType(szLan, "δ֪", "UnKnow");	
			csTemp.Format("%s", szLan);
            //csTemp.Format("δ֪");
            break;
        }       
        m_RealtimeList.SetItemText(0, 4, csTemp);

        //�ٶ�
        csTemp.Format("%dkm/h", m_struResult.struVehicleInfo.wSpeed);
        m_RealtimeList.SetItemText(0, 5, csTemp);

		//��������
		csTemp.Format("%d", m_struResult.struPlateInfo.byBright);
		m_RealtimeList.SetItemText(0, 11, csTemp);
		
		//���ƺ���
       	csTemp.Format("%s", m_struResult.struPlateInfo.sLicense);
		m_RealtimeList.SetItemText(0, 12, csTemp);

		//ץ������
		switch(m_struResult.byTrafficLight)
		{
		case 0:
			g_StringLanType(szLan, "�Ǻ��̵�ʱץ��", "Non light Capture");	
			m_csTrafficLight.Format("%s", szLan);
			//pDatabaseParam->m_csSnapMode.Format("�Ǻ��̵�ʱץ��");
			break;
		case 1:
			g_StringLanType(szLan, "�̵�ʱץ��", "Capture in green light");	
			m_csTrafficLight.Format("%s", szLan);
			//pDatabaseParam->m_csSnapMode.Format("�̵�ʱץ��");
			break;
		case 2:
			g_StringLanType(szLan, "���ʱץ��", "Capture in red light");	
			m_csTrafficLight.Format("%s", szLan);
			//pDatabaseParam->m_csSnapMode.Format("���ʱץ��");
			break;
		default:
			g_StringLanType(szLan, "δ֪", "UnKnow");	
			m_csTrafficLight.Format("%s", szLan);
			//pDatabaseParam->m_csSnapMode.Format("δ֪");
			break;
        }

		//������
		g_StringLanType(szLan, "������", "Lane");	
		csTemp.Format("%s:%d", szLan, m_struResult.byDriveChan);                  
        m_RealtimeList.SetItemText(0, 8, csTemp);

		//Υ������
		switch(m_struResult.struVehicleInfo.byIllegalType)
		{
		case 0:
			g_StringLanType(szLan, "����", "Normal");
			csTemp.Format("%s\n", szLan);
			g_StringLanType(szLan, "����", "Intelligent Monitoring and Recording System");
			sprintf(szSubPicFilePath, "%s\\%s", g_struLocalParam.chPictureSavePath, szLan);
			sprintf(szSubPlateFilePath, "%s\\%s", g_struLocalParam.chPictureSavePath, szLan);
			break;
		case 1:
			g_StringLanType(szLan, "����", "Low Speed");
			csTemp.Format("%s\n", szLan);
			sprintf(szSubPicFilePath, "%s\\%s", g_struLocalParam.chPictureSavePath, szLan);
			sprintf(szSubPlateFilePath, "%s\\%s", g_struLocalParam.chPictureSavePath, szLan);
			break;
		case 2:
			g_StringLanType(szLan, "����", "Over Speed");
			csTemp.Format("%s\n", szLan);
			sprintf(szSubPicFilePath, "%s\\%s", g_struLocalParam.chPictureSavePath, szLan);
			sprintf(szSubPlateFilePath, "%s\\%s", g_struLocalParam.chPictureSavePath, szLan);
			break;
		case 3:
			g_StringLanType(szLan, "����", "Go in the Wrong Direction");
			csTemp.Format("%s\n", szLan);
			sprintf(szSubPicFilePath, "%s\\%s", g_struLocalParam.chPictureSavePath, szLan);
			sprintf(szSubPlateFilePath, "%s\\%s", g_struLocalParam.chPictureSavePath, szLan);
			break;
		case 4:
			g_StringLanType(szLan, "Υ����ͨ��ָʾ", "Run the Red Light");
			csTemp.Format("%s\n", szLan);
			sprintf(szSubPicFilePath, "%s\\%s", g_struLocalParam.chPictureSavePath, szLan);
			sprintf(szSubPlateFilePath, "%s\\%s", g_struLocalParam.chPictureSavePath, szLan);
			break;
		case 5:
			g_StringLanType(szLan, "ѹ������", "Over Lane Line");
			csTemp.Format("%s\n", szLan);
			sprintf(szSubPicFilePath, "%s\\%s", g_struLocalParam.chPictureSavePath, szLan);
			sprintf(szSubPlateFilePath, "%s\\%s", g_struLocalParam.chPictureSavePath, szLan);
			break;
		case 6:
			g_StringLanType(szLan, "��������", "Travel against Guiding");
			csTemp.Format("%s\n", szLan);
			sprintf(szSubPicFilePath, "%s\\%s", g_struLocalParam.chPictureSavePath, szLan);
			sprintf(szSubPlateFilePath, "%s\\%s", g_struLocalParam.chPictureSavePath, szLan);
			break;
		case 7:
			g_StringLanType(szLan, "·������", "Intersection retention");
			csTemp.Format("%s\n", szLan);
			sprintf(szSubPicFilePath, "%s\\%s", g_struLocalParam.chPictureSavePath, szLan);
			sprintf(szSubPlateFilePath, "%s\\%s", g_struLocalParam.chPictureSavePath, szLan);
			break;
		case 8:
			g_StringLanType(szLan, "��ռ��", "Motor Vehicle Running on Non-Motor Vehicle Lane");
			csTemp.Format("%s\n", szLan);
			sprintf(szSubPicFilePath, "%s\\%s", g_struLocalParam.chPictureSavePath, szLan);
			sprintf(szSubPlateFilePath, "%s\\%s", g_struLocalParam.chPictureSavePath, szLan);
			break;
		case 9:
			g_StringLanType(szLan, "Υ�����", "Illegal Lane Change");
			csTemp.Format("%s\n", szLan);
			sprintf(szSubPicFilePath, "%s\\%s", g_struLocalParam.chPictureSavePath, szLan);
			sprintf(szSubPlateFilePath, "%s\\%s", g_struLocalParam.chPictureSavePath, szLan);
			break;
		case 10:
			g_StringLanType(szLan, "ռ��ר�ó���", "Take up dedicated lane");
			csTemp.Format("%s\n", szLan);
			sprintf(szSubPicFilePath, "%s\\%s", g_struLocalParam.chPictureSavePath, szLan);
			sprintf(szSubPlateFilePath, "%s\\%s", g_struLocalParam.chPictureSavePath, szLan);
			break;
		case 11:
			g_StringLanType(szLan, "������", "Violation of Prohibition sign");
			csTemp.Format("%s\n", szLan);
			sprintf(szSubPicFilePath, "%s\\%s", g_struLocalParam.chPictureSavePath, szLan);
			sprintf(szSubPlateFilePath, "%s\\%s", g_struLocalParam.chPictureSavePath, szLan);
			break;
		default:
			g_StringLanType(szLan, "δ֪", "Unknown");
			csTemp.Format("%s\n", szLan);
			sprintf(szSubPicFilePath, "%s\\%s", g_struLocalParam.chPictureSavePath, szLan);
			sprintf(szSubPlateFilePath, "%s\\%s", g_struLocalParam.chPictureSavePath, szLan);
			break;
		}
		m_RealtimeList.SetItemText(0, 9, csTemp);

		//����
		g_VehicleLogo(m_struResult.struVehicleInfo.byVehicleLogoRecog, szLan);
		csTemp.Format("%s\n", szLan);
		m_RealtimeList.SetItemText(0, 10, csTemp);

		//����ͼƬ
		if (m_struResult.dwPicLen != 0 && m_struResult.byResultType == 1)
		{
			if (GetFileAttributes(szSubPicFilePath) != FILE_ATTRIBUTE_DIRECTORY)
			{
				CreateDirectory(szSubPicFilePath, NULL);
			}
			if (GetFileAttributes(szSubPlateFilePath) != FILE_ATTRIBUTE_DIRECTORY)
			{
				CreateDirectory(szSubPlateFilePath, NULL);
			}
			
			char szTime[9] = {0};
			memcpy(szTime, m_struResult.byAbsTime, 8);
			sprintf(szPicDataTime,"%s\\%s", szSubPicFilePath,szTime);  
			sprintf(szPlateDataTime,"%s\\%s", szSubPlateFilePath,szTime); 
			if (GetFileAttributes(szPicDataTime) != FILE_ATTRIBUTE_DIRECTORY)
			{
				CreateDirectory(szPicDataTime, NULL);
			}
			if (GetFileAttributes(szPlateDataTime) != FILE_ATTRIBUTE_DIRECTORY)
			{
				CreateDirectory(szPlateDataTime, NULL);
			}
			
			sprintf(szFileName, "%s\\%s.jpg", szPicDataTime/*szSubPicFilePath*/, m_struResult.byAbsTime);
			
            HANDLE lHandle = CreateFile(szFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
            m_RealtimeList.SetItemText(0, 6, szFileName);
            if (lHandle == INVALID_HANDLE_VALUE)
            {
                TRACE("create file failed! Error code:%d", WSAGetLastError());
                return;
            }	
			DWORD dwReturn = 0;	
            WriteFile(lHandle, m_struResult.pBuffer1, m_struResult.dwPicLen, &dwReturn, NULL);
            CloseHandle(lHandle);			
		}

		//����ͼƬ
		if (m_struResult.dwPicPlateLen != 0 && m_struResult.byResultType == 1)
		{
			g_StringLanType(szLan, "����", "License plate");
			sprintf(szPlateFileName, "%s\\%s%s.jpg", szPlateDataTime/*szSubPlateFilePath*/, szLan, m_struResult.byAbsTime);                    
			m_RealtimeList.SetItemText(0, 7, szPlateFileName);	
			if (m_struResult.dwPicPlateLen != 0 && m_struResult.byResultType == 1)
			{
				sprintf(szPlateFileName, "%s\\%s%s.jpg", szPlateDataTime/*szSubPlateFilePath*/, szLan, m_struResult.byAbsTime);                    
				m_RealtimeList.SetItemText(0, 7, szPlateFileName);	
				HANDLE lHandle = CreateFile(szPlateFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
				if (lHandle == INVALID_HANDLE_VALUE)
				{
					TRACE("create file failed! Error code:%d", WSAGetLastError());
					return;
				}                
				DWORD dwReturn = 0;	
				WriteFile(lHandle, m_struResult.pBuffer2, m_struResult.dwPicPlateLen, &dwReturn, NULL);
				CloseHandle(lHandle);
			}
		}

		 //��Ƶ�ļ�
		if (m_struResult.byResultType == 0)
        {
			csTemp.Format("%s\\", szPicDataTime/*g_LocalParam.chPicFilePath*/);		
			DWORD dwRet = GetFileAttributes(csTemp);
			if ((dwRet == -1) || !(dwRet & FILE_ATTRIBUTE_DIRECTORY))
			{
				CreateDirectory(csTemp, NULL);
			}
            //��Ƶ�ļ�
            sprintf(szFileName, "%s\\%s.mp4", szPicDataTime/*g_LocalParam.chPicFilePath*/, m_struResult.byAbsTime);
            
            HANDLE lHandle = CreateFile(szFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
            if (lHandle == INVALID_HANDLE_VALUE)
            {
                TRACE("create file failed! Error code:%d", WSAGetLastError());
                return;
            }
            
            DWORD dwReturn = 0;	
            
            WriteFile(lHandle, m_struResult.pBuffer1, m_struResult.dwVideoLen, &dwReturn, NULL);
            CloseHandle(lHandle);
        }


		g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_ManualSnap");

	}

	if (NULL != m_struResult.pBuffer1)
	{
		delete []m_struResult.pBuffer1;
		m_struResult.pBuffer1 = NULL;
	}
	
	if (NULL != m_struResult.pBuffer2)
	{
		delete []m_struResult.pBuffer2;
		m_struResult.pBuffer2 = NULL;
	}
}

void CDlgManualSnap::OnBtnPTZCtrl()
{
	CDlgPtzCtrl dlg;
    dlg.m_lPlayHandle = m_lPlayHandle;
    dlg.DoModal();	
}

void CDlgManualSnap::OnBtnContinuousShoot() 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE);
    NET_DVR_SNAPCFG struSnapCfg;
    memset(&struSnapCfg, 0, sizeof(NET_DVR_SNAPCFG));
    struSnapCfg.dwSize = sizeof(NET_DVR_SNAPCFG);
    struSnapCfg.byRelatedDriveWay = m_byLaneNo;
    struSnapCfg.bySnapTimes = m_comSnapTimes.GetCurSel();
    struSnapCfg.wSnapWaitTime = m_dwSnapWaitTime;
    struSnapCfg.wIntervalTime[0] = m_dwIntervalTime1;
    struSnapCfg.wIntervalTime[1] = m_dwIntervalTime2;
    struSnapCfg.wIntervalTime[2] = m_dwIntervalTime3;
    struSnapCfg.wIntervalTime[3] = m_dwIntervalTime4;
    
    if (!NET_DVR_ContinuousShoot(m_lServerID, &struSnapCfg))
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_ContinuousShoot");
        return;
	} 
}
