// ClientDemo.h : main header file for the CLIENTDEMO application
//
/**********************************************************
FileName:    ClientDemo.h
Description: this function will export global structs, defines, variables, and functions that will be used, most of documents should include this head file
Date:        2008/03/27
Note: 		<golbal>struct,define refer to GeneralDef.h, variables and functions refer to ClientDemo.cpp   
Modification History:      
    <version> <time>         <desc>
    <1.0    > <2008/03/27>       <created>
***********************************************************/

#if !defined(AFX_CLIENTDEMO_H__BFFEA4FD_9DA7_4869_B509_9D4ED18FA5F1__INCLUDED_)
#define AFX_CLIENTDEMO_H__BFFEA4FD_9DA7_4869_B509_9D4ED18FA5F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

//include general struct and Macro;then only need to include ClientDemo.h at start of other files;

#include "GeneralDef.h"
#include "ClientDemoDlg.h"
#include "DlgOutput.h"
#include "DlgRemoteCfg.h"
#include "DlgOutputCtrl.h"
#include "gdiplus/GdiPlus.h"
#include "DlgSensorCfg.h"

//provide global variables to other dlg classes, using 'extern' to avoid redefinition
extern LOCAL_FACE_PICTURE_INFO g_struFacePic[MAX_HUMAN_PICTURE_NUM];
extern LOCAL_FACE_MODEL_INFO g_struFaceModel[MAX_HUMAN_PICTURE_NUM];
extern LOCAL_SENSOR_ADD_PREVIEW g_struSensorAddPreview[MAX_ALARMHOST_ALARMIN_NUM];
extern STRU_DEVICE_INFO g_struDeviceInfo[MAX_DEVICES];
extern LOCAL_PARAM g_struLocalParam;
extern BOOL g_bCapPic[MAX_OUTPUTS];
extern BOOL g_bLocalCapPic;
extern BOOL g_bInSide;
extern BOOL g_bStandardCB;
extern BOOL g_bPtzMaualTrace;
extern int g_iHourDiffWithUTC;
extern int g_iMinDiffWithUTC;
extern BOOL g_bISO8601;
extern int g_iDisplayBufNum;

//2009-7-22
extern DWORD g_dwStreamTransType;
//2009-7-22 end

extern CDlgOutput g_dlgOutput[MAX_OUTPUTS];
extern char g_sSecretKey[17];
extern HBRUSH	g_hPlayHardBgBrush;//Brush of preview window background, different color in hardware decode from software decode 
extern HBRUSH	g_hPlaySoftBgBrush;//Brush of preview window background, different color in hardware decode from software decode 
extern BOOL g_bShareSound;

//all play parameters, select g_struLocalParam.bCycle to turn to next page automatically in circulation by order,not by schedule
extern PLAY_INFO g_struPlayAllInfo[MAX_PREVIEWS];
extern BOOL g_bPlayAllDevice;			//check if all the devices are playing
extern WORD g_nSurveillanceTotalNum;	//surveillance points for preview(camera number)
extern int g_iNextSurveillanceIndex;//the  serial NO. of last surveillance point in all play or circulative play
extern int g_pCycleTimer;			//the index of cycle timer

extern int g_nNetDelay;
extern char g_szWorkDir[MAX_PATH];
extern BOOL g_bCardInit;

extern CClientDemoDlg *g_pMainDlg;
extern CDlgSensorCfg *g_pDlgSensor;

extern CRect g_rcWindow;				//area of entire dialog
extern DWORD g_dwOldDeviceNum;	//display card num
extern int	g_iCurScreenWidth;    //width of current screen
extern int	g_iCurScreenHeight;   //height of current screen

extern LONG	g_lVoiceHandle;	//handle of voice talk

extern BOOL g_bMustReboot;
extern int g_bExitDemo;
extern CASE_INFO g_caseInfo;

extern void g_ReadDeviceConfig();
extern void g_SaveDeviceConfig();
extern void WriteLocalConfig();
extern void ReadLocalConfig();
extern void GetCurrentPath(char *path);
extern DWORD CStringTodwIP(CString strIP);
extern void g_CreateLocalDir();
extern CString IPToStr(DWORD dwIP);
extern BOOL CheckIPStr(CString ip);
extern void g_CreateAlarmOutTree(CTreeCtrl &m_treeAlarmOut, int iDeviceIndex, BOOL *bAlarmOut);
extern void g_CreateChannelTree(CTreeCtrl &m_treeChannel, int iDeviceIndex, BOOL *bChannelChecked);
extern void g_CreateAlarmOutTreeByGroup(CTreeCtrl &m_treeAlarmOut, int iDeviceIndex, BOOL *bAlarmOut, DWORD dwGroupN);
extern void g_CreateChannelTreeByGroup(CTreeCtrl &m_treeChannel, int iDeviceIndex, BOOL *bChannelChecked, DWORD dwGroupN);
extern void g_StringLanType(char *szDstLan, char *szLanCn, char *szLanEn);
extern char *gg_StringLanType(char *szLanCn, char *szLanEn, const char* format, ...);
NET_DVR_API BOOL __stdcall NET_DVR_RealPlayPause(LONG lRealHandle);
NET_DVR_API BOOL __stdcall NET_DVR_RealPlayRestart(LONG lRealHandle, HWND hPlayWnd);
extern void g_ReadLocalReadSensorAdd();
extern void g_SaveLocalReadSensorAdd();
extern int g_DelFromArray(DWORD *dwArray, int iMaxArrayIndex, int iCurIndex);
extern int g_FindFromArray(DWORD *dwArray, int iMaxArrayIndex, DWORD dwFindValue);
extern void g_AddChanInfoToComBox(CComboBox &cmbChanBox, int iDeviceIndex, BOOL bShowOfflineChannel=TRUE);
extern BOOL IsDevWinControl(int iDeviceIndex);
void GetSerialProto(unsigned int *&pOut, DWORD &dwNum);
extern void g_DrawRect(HDC hDc, NET_VCA_RECT struRect,  CRect rcWnd, DWORD dwCurRect, COLORREF color);
extern float g_GetFloatValue(float fValue);
void ClassifyDevClass(char *pDevClass, unsigned long ulDevClassLen, unsigned short wDevClass);
//Is ipv6?
BOOL g_ValidIPv6(BYTE *ip);
void g_replace(char* pOldString, char* pNewString, char* pSearchString, char* pRepalceString);
//Get Osd time macro
#define GET_YEAR(_time_)      (((_time_)>>26) + 2000) 
#define GET_MONTH(_time_)     (((_time_)>>22) & 15)
#define GET_DAY(_time_)       (((_time_)>>17) & 31)
#define GET_HOUR(_time_)      (((_time_)>>12) & 31) 
#define GET_MINUTE(_time_)    (((_time_)>>6)  & 63)
#define GET_SECOND(_time_)    (((_time_)>>0)  & 63)
/////////////////////////////////////////////////////////////////////////////
// CClientDemoApp:
// See ClientDemo.cpp for the implementation of this class
//
#define MINOR_FONT      0
#define NORMAL_FONT     1
#define MAJOR_FONT      2

typedef enum _REMOTE_FILE_TYPE_
{
    FILE_TIMING = 0, //��ʱ¼��
	FILE_MOTION_DETECT = 1, //�ƶ����¼��
	FILE_ALARMIN = 2, //��������¼��
	FILE_MOTION_OR_ALARMIN = 3, //����|�ƶ����
	FILE_MOTION_AND_ALARMIN = 4, //����&�ƶ����
	FILE_COMMAND = 5, //�����¼��
	FILE_MANUAL = 6, //�ֶ�¼��
	FILE_VCA = 7, //����¼��
	FILE_PIR_ALARM = 10, //PIR����¼��
	FILE_WIRELESS_ALARM = 11, //���߱���¼��
	FILE_CALLHELP_ALARM = 12, //���ȱ���
	FILE_PIR_WIRELESS_CALLHELP = 13,  //�ƶ�|��������|PIR|���߱���|���ȱ���
	FILE_INTELLIGENT_TRANSPORT = 14, //���ܽ�ͨ�¼�����
	FILE_LINE_DETECTION = 15,// Խ�����
	FILE_FIELD_DETECTUIN = 16,// ��������
	FILE_AUDIO_EXCEPTION = 17,// �����쳣
//	FILE_IPC_SMART = 18,
	FILE_SCENECHANGE_DETECTION = 18,//���������� 2013-07-16
	FILE_INTELLIGENT_DETECTION = 19,
	FILE_FACE_DETECTION = 20, //�������
    FILE_POS_RECORD = 25, //POS����
	FILE_REGION_ENTRANCE_DETECTION = 26, //�����������
	FILE_REGION_EXITING_DETECTION=27,//�뿪�������
	FILE_LOITERING_DETECTION = 28,//�ǻ����
	FILE_PEOPLE_GATHERING_DETECTION = 29,//��Ա�ۼ����
	FILE_FAST_MOVING_DETECTION = 30,//�����ƶ����
	FILE_PARKING_DETECTION = 31,//ͣ�����
	FILE_UNATTENDED_BAGGAGE_DETECTION = 32,//��Ʒ�������
	FILE_OBJECT_REMOVAL_DETECTION = 33,//��Ʒ��ȡ���
	FILE_OBJECT_FIRE_DETECTION = 34,//��Ʒ��ȡ���
    FILE_OBJECT_VANDALPROOF_DETECTION = 35,//���ƻ����
    FILE_THERMAL_SHIPS_DETECTION = 36, //��ֻ���
    FILE_THERMAL_THERMOMETRY_EARLYWARNING = 37, //����Ԥ��
    FILE_THERMAL_THERMOMETRY_ALARM = 38, //���±���
	FILE_THERMOMETRY_DIFF_ALARM = 42,//����
    FILE_THERMOMETRY_OFFLINE_ALARM = 43,//���߲��±���
    FILE_ZONE_ALARMIN_ALARM = 44,//��������
    FILE_EMERGENCY_CALL_HELP_ALARM = 45,//��������
    FILE_CONSULT = 46,//ҵ����ѯ
    FILE_VCA_EVENT_GET_UP = 47,// ������
    FILE_VCA_EVENT_ADV_REACH_HEIGHT = 48,// �����ʸ�
    FILE_VCA_EVENT_TOILET_TARRY = 49,// ��޳�ʱ
    FILE_VCA_EVENT_FACE_SNAP = 52,//����ץ��
    FILE_UNREGISTERED_STREET_VENDOR = 53, //�Ƿ���̯
    FILE_UNREGISTERED_HUMAN_RECOGNITION = 54, //Ŀ��ץ��
    FILE_VCA_EVENT_MOTION = 55,// �����˶�
    FILE_VCA_EVENT_LEAVE_POSITION = 56,// ��ڼ��
    FILE_VCA_EVENT_STAND_UP = 57,// �������
    FILE_VCA_EVENT_PEOPLE_NUM_CHANGE = 58,// �����仯
	FILE_ALL = 0xff //ȫ��
}REMOTE_FILE_TYPE;

typedef enum _REMOTE_FILE_CARD_TYPE_
{
    FILE_CARD_TIMING = 0, //��ʱ¼��
	FILE_CARD_MOTION_DETECT = 1, //�ƶ����¼��
	FILE_CARD_CLOSETO = 2, //�ӽ�����¼��
	FILE_CARD_BANKNOTE = 3, //��������¼��
	FILE_CARD_FEEDING = 4, //��������¼��
	FILE_CARD_COMMAND = 5, //�����¼��
	FILE_CARD_MANUAL = 6, //�ֶ�¼��
	FILE_CARD_VIBRATION = 7, //�𶯱���¼��
	FILE_CARD_ENVIRON = 8, //��������¼��
	FILE_CARD_VCA = 9, //����¼��
	FILE_CARD_PIR_ALARM = 10, //PIR����¼��
	FILE_CARD_WIRELESS_ALARM = 11, //���߱���¼��
	FILE_CARD_CALLHELP_ALARM = 12, //���ȱ���
	FILE_CARD_PIR_WIRELESS_CALLHELP = 13,  //�ƶ�|��������|PIR|���߱���|���ȱ���
	FILE_CARD_INTELLIGENT_TRANSPORT = 14, //���ܽ�ͨ�¼�����
	FILE_CARD_ALL = 0xff //ȫ��
}REMOTE_FILE_CARD_TYPE;

typedef enum _REMOTE_SEARCH_DEVICE_TYPE_
{
    FILE_ACS = 0,// �����Ž�����
    FILE_VIDEO_INTERCOM = 1,// �������ӶԽ��豸
    FILE_SECURITY_CONTROL_PANEL = 2,// �������������豸��Ԥ������ǰ��ʵ�֣�
    FILE_GJD = 3,// GJD��������
    FILE_LUMINITE = 4,// Luminite��������
    FILE_OPTEX = 5,// OPTEX��������
    FILE_CAMERA_DETECTOR = 6//ģ������������豸
}REMOTE_SEARCH_DEVICE_TYPE;


class CClientDemoApp : public CWinApp
{
public:
	CClientDemoApp();
	~CClientDemoApp();

    ULONG_PTR m_gdiplusToken;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientDemoApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CClientDemoApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTDEMO_H__BFFEA4FD_9DA7_4869_B509_9D4ED18FA5F1__INCLUDED_)
