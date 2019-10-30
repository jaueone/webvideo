/**********************************************************
FileName:    GeneralDef.h
Description: clientdemo��ʹ�õ���ͨ�õĺ궨��ͽṹ�嶨��      
Date:        2008/03/27   
Modification History:      
    <version> <time>         <desc>
    <1.0    > <2008/03/27>       <created>
***********************************************************/

#ifndef _GLOBLE_DEF_H_
#define _GLOBLE_DEF_H_

//��sdk��ͽ������Ҫ�ı�����ͷ�ļ�һ����
#include "HCNetSDK.h"
#include "DecodeCardSdk.h"
#include "DataType.h"
#include "plaympeg4.h"

#define WM_MYCOMMAND					WM_USER + 1		//for sdk
#define WM_CHANGE_CHANNEL_ITEM_IMAGE	WM_USER + 2     //�޸�ͨ������ϵ�ͼ��
#define WM_CHANGE_DEVICE_ITEM_IMAGE		WM_USER + 3     //�޸��豸����ϵ�ͼ��
#define WM_ADD_LOG						WM_USER + 4		//�����־
#define WM_PROC_ALARM					WM_USER + 5		//��������

#define DEMO_FULL_WIDTH 1024	//demoĬ�ϵĿ��
#define DEMO_FULL_HEIGHT 768	//demoĬ�ϵĸ߶�
#define OUTPUT_INTERVAL 4		//ÿ������ļ����С
#define LIST_ENLARGE_HIGH 575	//������־��澯�б�������߶�	

#define WIDTH_4CIF			704
#define HEIGHT_4CIF_PAL		576
#define HEIGHT_4CIF_NTSC	480
#define WIDTH_CIF			352
#define HEIGHT_CIF_PAL		288
#define HEIGHT_CIF_NTSC		240
#define PRECISION_4CIF_FRAME	32//4cif�ֱ�����С����ı߳�����

//m_iMainType mian fuction bar����������,˳���Ӧ����ÿ��button
#define PREVIEW_T		0
#define CONFIG_ALL_T	1
#define PLAY_BACK_T		2

//config dialog type 
	//remote config type, ˳���Ӧ�����tabҳ
#define REMOTE_DEVICE_CFG_T		0	//�豸������Ϣ��������Ϣ���ý���
#define REMOTE_CHANNEL_CFG_T	1	//ͨ����������Ϣ���ý���
#define REMOTE_SEREIAL_CFG_T	2	//������Ϣ���ý���
#define REMOTE_AlARM_CFG_T		3	//�澯����/�����Ϣ���ý���
#define REMOTE_USER_CFG_T		4	//�û���Ϣ���ý���
#define REMOTE_EXCEPTION_CFG_T	5	//�쳣��Ϣ���ý���
#define REMOTE_ATM_INFO_CFG_T	6	//ATM����Ϣ���ý���
#define REMOTE_MAT_DEC_CFG_T	7	//��·���������ý���

	//˳���Ӧ���������ð�ť
#define LOCAL_CFG_T				8
#define LOG_QUERY_T				9
#define DECODE_CARD_CFG_T		10

//�ط�����,play back dialog type,˳���Ӧ�طŵ�tabҳ
#define  PLAY_REMOTE_FILE_T 0
#define  PLAY_LOCAL_FILE_T 1
#define  PLAY_REMOTE_TIME_T 2

//������־��������
#define ALARM_INFO_T		0	//�澯��Ϣ
#define OPERATION_SUCC_T	1	//�����ɹ���־
#define OPERATION_FAIL_T	2	//����ʧ����־
#define PLAY_SUCC_T			3	//�������ɹ���־
#define PLAY_FAIL_T			4	//������ʧ����־

#define DETECT_AREA_LEFT 10//���������������
#define DETECT_AREA_TOP 10//���������������
#define SCREEN_EDGE 30		//�������߿�Ĵ�С

#define MINSPACE		(200*1024*1024)//Ӳ����С�ռ�

//��ʱ��
#define SYSTEM_TIMER			1		//ϵͳ��صĲ���,ʱ��,cpu�ȵĶ�ʱ��
#define CYCLE_PREVIEW_TIMER		2		//��Ѳ���,��ʱ��
#define	AUTORECORD_TIMER		3		//�Զ�¼��ʱ��
#define	FREESPACE_TIMER			4		//���Ӳ��ʱ��
#define	FILEINTERVAL_TIMER		5		//�����ļ�ʱ����ʱ��
#define UPGRADE_TIMER			6		//����ʱ��
#define PLAY_TIMER				7		//���ػط�ʱ��ʾ����״̬ʱ��
#define REMOTE_PLAY_STATE_TIMER	8		//Զ�̻ط��ļ�����״̬ʱ��
#define DOWNSTATE_TIMER			9		//�����ļ�ʱ��
#define FORMAT_TIMER			10		//Զ�̸�ʽ��Ӳ��
#define EVENTLOG_TIMER			11		//�¼���־
#define PLAYBYTIME_TIMER		12		// ��ʱ��ط�
#define DOWNBYTIME_TIMER		13		//��ʱ������
#define SERIAL_TIMER			14		//����͸��ͨ������
#define	CHECK_DEVICE_TIMER		15		//Ӳ��¼���Уʱʱ��

//�����Ӧ��һЩĬ�ϲ���
#define DEFAULT_BRIGHTNESS	6			//Ĭ������
#define DEFAULT_CONTRAST	6			//Ĭ�϶Աȶ�
#define DEFAULT_SATURATION  6			//Ĭ�ϱ��Ͷ�
#define DEFAULT_HUE			6			//Ĭ��ɫ��
#define DEFAULT_VOLUME		50			//Ĭ������


#define MAX_DEVICES			50	//�ɼ�ص�����豸��
#define MAX_OUTPUTS			81	//16���Ĵ�����
#define MAX_PREVIEWS		512	//��Ԥ���������

//�豸��������
#define TREE_ALL_T	 0        //������ڵ�
#define DEVICETYPE 	 1		  //�����豸
#define CHANNELTYPE	 2	      //����ͨ��		

/////
//bmp ����state
//��λͼ���������Ӧ
#define TREE_ALL			 0	//�豸��	
#define DEVICE_LOGOUT	     1   //�豸�Ѿ�ע��,��δ��½
#define DEVICE_LOGIN	     2   //�豸�Ѿ���½
#define DEVICE_FORTIFY	     3   //�豸����
#define DEVICE_ALARM		 4   //�豸�и澯
#define DEVICE_FORTIFY_ALARM 5	//�豸�������и澯

#define CHAN_ORIGINAL		 6   //��Ԥ��,��¼���״̬
#define CHAN_PLAY		 	 7   //ֻ��Ԥ��
#define CHAN_RECORD			 8   //ֻ��¼��
#define CHAN_PLAY_RECORD	 9   //Ԥ������¼��

#define CHAN_ALARM		 	 10   //��Ԥ��,��¼��,ֻ�и澯��״̬
#define CHAN_PLAY_ALARM			 11   //Ԥ��,��¼��,�и澯
#define CHAN_PLAY_RECORD_ALARM	 12   //Ԥ������¼�����и澯
#define CHAN_OFF_LINE			 13	 //��Ӧ��ͨ��������

#define CHAN_AUDIO				 1   //��Ƶͨ��

//ATMר��,��Ϣ���뷽ʽ
#define NET_LISTEN				0	//��������;
#define NET_ACCEPT				1	//�������;
#define SERIAL_INPUT_DIRECTLY	2	//����ֱ������;
#define SERIAL_ATM_CMD_INPUT	3	//����ATM��������;

//ѹ����������compression parameter
#define NORM_HIGH_STREAM_COMPRESSION		0//������ͼ��ѹ������,ѹ������ǿ,���ܿ��Ը���
#define SUB_STREAM_COMPRESSION		1//������ͼ��ѹ������
#define EVENT_INVOKE_COMPRESSION	2//�¼�����ͼ��ѹ������,һЩ������Թ̶�
#define NORM_LOW_STREAM_COMPRESSION		3//������ͼ��ѹ������,ѹ������ǿ,���ܿ��Ը���

//��������
#define SERIAL_232		1	//232����
#define SERIAL_485		2	//485����


#define	ANALOG_CHAN_DISABLE 0
#define	ANALOG_CHAN_ENABLE	1
#define	IP_CHAN_DISABLE		2
#define	IP_CHAN_ENABLE		3

#define ALARM_OUT_FORMAT		("AlarmOut%d")
#define ALARM_IN_NAME			("AlarmIn%d")
#define IP_ALARM_IN_NAME		("IPAlarmIn%d")
#define IP_ALARM_OUT_NAME		("IPAlarmOut%d")
#define ANALOG_C_FORMAT			("Camera%d")
#define IP_CAMERA_NAME			("IPCamera%02d")
#define DIGITAL_C_FORMAT		("IPCamera%d")

//ȫ�����ź���Ѳ��ʱ�򲥷��豸��Ӧ�����к�
typedef struct STRU_PLAY_INFO{

	int		iDeviceIndex;		//��Ӧ���豸�������±�
	int		iChanIndex;			//���ͨ����

	STRU_PLAY_INFO()
	{
		iDeviceIndex = -1;
		iChanIndex   = -1;
	}
}PLAY_INFO,*PPLAY_INFO;

//�豸ͨ������Ϣ
typedef struct STRU_CHANNEL_INFO{ 
	int		iDeviceIndex;			//��Ӧ���豸�������±�
	int		iChanIndex;			//���ͨ����
	char	chChanName[100];	//����ͨ���������
	DWORD	dwProtocol;			//����Э��

	int		iPicResolution;				//ͼƬ�ֱ���
	int		iPicQuality;				//ͼƬ����
	LONG	lRealHandle;          //ʵʱԤ�����
	BOOL    bLocalManualRec;     //�Ƿ�¼��;
	BOOL    bAlarm;				//�Ƿ��б���;
	BOOL	bEnable;			//�Ƿ�ʹ��
	DWORD	dwImageType;		//ͨ��״̬��Ӧ��ͼƬ����
	char	chAccessChanIP[16];//ip����ͨ����ip��ַ

	STRU_CHANNEL_INFO *pNext;
	STRU_CHANNEL_INFO()
	{
		iDeviceIndex		= -1;
		iChanIndex			= -1;
		chChanName[0]		= '\0';
		dwProtocol			= 0;

		iPicResolution		= 0;
		iPicQuality			= 2;

		lRealHandle			= -1; 
		bLocalManualRec		= FALSE;
		bAlarm				= FALSE;
		bEnable				= FALSE;
		dwImageType			= CHAN_ORIGINAL;
		chAccessChanIP[0]	= '\0';	 		  
		pNext				= NULL;
	}
}CHANNEL_INFO,*PCHANNEL_INFO;

//�豸��Ϣ
typedef struct STRU_DEVICE_INFO{
	int		iDeviceIndex;			//�豸�����±�
	LONG	lLoginID;				//ע��ID
	DWORD	dwDevSoftVer;			//����

	char	chLocalNodeName[100];	//�����豸�ڵ�����
	char	chDeviceName[100];		//�豸����
	char	chDeviceIP[33];			//����ֱ�����ӵ��豸IP,�п�����:�豸��һ�����������õ�IP,pppoe��ַ,�豸�������ص�ַ
	//char	chDevNetCard1IP[33];	//�豸��һ�����������õ�IP
	char	chLoginUserName[30];	//��½�豸���û��������û������ʱ���õ�
	char	chLoginPwd[30];	//��½�豸������
	char	chDeviceMultiIP[20];	//�ಥ���ַ
	//char	chIPServerIP[32];		//IP��ַ
	char	chSerialNumber[50];		//���к�
	int		iDeviceChanNum;		    //�豸��ͨ����
	int		iStartChan;				//�豸��ʼͨ����
	int		iDeviceType;			//�豸����
	int 	iDiskNum;				//Ӳ�̸���
	LONG 	lDevicePort;			//�豸���˿ں�
	int		iAlarmInNum;			//�豸�澯����ĸ���
	int		iAlarmOutNum;			//�豸��������ĸ���
	int 	iAudioNum;				//�����Խ�����
	int 	iIPChanNum;				//�������ͨ������

	BOOL	bCycle;					//���豸�Ƿ���ѭ������
	BOOL	bPlayDevice;			//���豸�ǲ�������ͨ�����ڲ���, add after
	BOOL	bVoiceTalk;				//�Ƿ��������Խ�
	LONG    lAudioHandle[MAX_AUDIO_V30]; //�����㲥���
	BOOL	bCheckBroadcast[MAX_AUDIO_V30]; //�Ƿ���������㲥����
	LONG	lFortifyHandle;			//������� 
	BOOL    bAlarm;					//�Ƿ��б���;
	int	    iDeviceLoginType;		//ע��ģʽ  0 - �̶�IP   1- IPSERVER ģʽ    2 -  ��ͨ���� 
	DWORD	dwImageType;			//�豸״̬��Ӧ��ͼƬ
	BOOL	bIPRet;					//�Ƿ�֧��ip����
	int		iEnableChanNum;			//��Чͨ����
	BOOL	bDVRLocalAllRec;		//�豸����ȫ��¼��
	LONG	lVoiceCom[MAX_AUDIO_V30];	//����ת�����
	LONG	lFirstEnableChanIndex;			//��һ��ʹ��ͨ���±�
	LONG	lTranHandle;				//232͸��ͨ�����

	STRU_CHANNEL_INFO struChanInfo[MAX_CHANNUM_V30]; //ͨ���ṹ
	NET_DVR_IPPARACFG struIPParaCfg;				//IP�������
	NET_DVR_IPALARMINCFG struAlarmInCfg;
	NET_DVR_IPALARMOUTCFG struAlarmOutCfg;
	STRU_DEVICE_INFO *pNext;
	STRU_DEVICE_INFO()
	{						
		iDeviceIndex		= -1;
		lLoginID            = -1;	
		dwDevSoftVer		= 0;
		chLocalNodeName[0]  = '\0';	
		chDeviceName[0]     = '\0';	
		chDeviceIP[0]	    = '\0';	
		//chDevNetCard1IP[0]	= '\0';
		chLoginUserName[0]= '\0';	
		chLoginPwd[0] = '\0';	
		chDeviceMultiIP[0]  = '\0';		
		chSerialNumber[0]   = '\0';	 
		iDeviceChanNum      = -1;
		iStartChan			= 0;
		iDeviceType         = 0;	
		iDiskNum            = 0;				
		lDevicePort         = 8000;	
		iAlarmInNum			= 0;	
		iAlarmOutNum        = 0;
		iIPChanNum			= 0;
			
		bCycle              = FALSE;
		bPlayDevice			= FALSE;
		bVoiceTalk			= FALSE;
		bCheckBroadcast[0]     = FALSE;	
		bCheckBroadcast[1]     = FALSE;	
		lFortifyHandle      = -1;	
		bAlarm				= FALSE;
		iDeviceLoginType    = 0;	
		dwImageType			= DEVICE_LOGOUT;
		bIPRet				= FALSE;
		pNext = NULL;
		lVoiceCom[0] = -1;
		lVoiceCom[1] = -1;
		lFirstEnableChanIndex = 0;
		lTranHandle = -1;
		memset(&struIPParaCfg,0, sizeof(NET_DVR_IPPARACFG));
		memset(&struAlarmOutCfg,0, sizeof(NET_DVR_IPALARMOUTCFG));
		memset(&struAlarmInCfg,0, sizeof(NET_DVR_IPALARMINCFG));
	}

}LOCAL_DEVICE_INFO, *PLOCAL_DEVICE_INFO;

//��Ƶ����
typedef struct STRU_VIDEO_EFFECT{
	UINT	m_iBrightness;				//1-10
	UINT	m_iContrast;				//1-10
	UINT	m_iSaturation;				//1-10
	UINT	m_iHue;						//1-10
	STRU_VIDEO_EFFECT()
	{
		m_iBrightness = DEFAULT_BRIGHTNESS;
		m_iContrast = DEFAULT_CONTRAST;	
		m_iSaturation = DEFAULT_SATURATION;
		m_iHue = DEFAULT_HUE;
	}
}VIDEO_INFO, *PVIDEO_INFO;

//����¼��ʱ��ṹ
typedef struct STRU_LOCAL_RECORD_TIME{
	WORD	iStartHour;
	WORD	iStartMinute;
	WORD	iStopHour;
	WORD	iStopMinute;
	WORD	iStartTime;
	WORD	iStopTime;
}LOCAL_RECORD_TIME,*PLOCAL_RECORD_TIME;

//��������
typedef struct STRU_LOCAL_PARAM
{
	BOOL		bCyclePlay;				    //�Ƿ�ѭ������
	int			iCycleTime;				    //ѭ������ʱ��, default 20
	BOOL		bUseCard;					//�Ƿ����Ӳ����
	BOOL		bNTSC;						//Ӳ������Ƶ��ʽ,FALSE,PAL��;TRUE,NTSC��;Ĭ��Ϊpal��

	BOOL		bAutoRecord;				//�Ƿ��Զ�¼��;
	BOOL		bCycleRecord;				//�Ƿ�ѭ��¼��
	int			iStartRecordDriver;		    //�ͻ���¼����ʼ�̷�
	int			iEndRecordDriver;			//�ͻ���¼����ֹ�̷�
	int			iRecFileSize;				//¼���ļ������С
	int			iRecordFileInterval;		//¼����ʱ����
	char		chDownLoadPath[100];		//Զ���ļ����ر���·��
	char		chPictureSavePath[100];		//ץͼ����·��
	char		chRemoteCfgSavePath[100];	//Զ�������ļ�����·��
	char		chClientRecordPath[100];		//Զ���ļ����ر���·��

	BOOL		bAutoCheckDeviceTime;		//Ӳ��¼���Уʱ
	LONG		lCheckDeviceTime;			//Ӳ��¼���Уʱʱ����

	int			iAlarmDelayTime ;			//������ʱʱ��,��ⱨ���Ƿ�ʱ
	int			iAlarmListenPort;			//���ر��������˿�

	BOOL		bAutoSaveLog;				//�Ƿ��Զ����汾����־�б�����Ϣ
	BOOL		bAlarmInfo;					//�Ƿ��ڱ�����־�б�����ʾ�澯��Ϣ
	BOOL		bSuccLog;				//�Ƿ��ڱ�����־�б�����ʾ������־
	BOOL		bFailLog;					//�Ƿ��ڱ�����־�б�����ʾ������Ϣ
	BOOL		bAllDiskFull;				//Ӳ����

	//Ԥ����һЩ��־����
	BOOL		bPlaying;					//���ڲ��ŵı�־
	BOOL		bCycling;					//����ѭ�����ŵı�־
	BOOL		bPaused;					//ѭ����ͣ
	BOOL		bNextPage;				    //�ֶ�ѭ����һҳ
	BOOL		bFrontPage;				    //�ֶ�ѭ����һҳ
	BOOL		bEnlarged;				    //ͼ�񴰿ڷŴ�
	BOOL		bFullScreen;				//ͼ��ȫ���Ŵ�
	BOOL		bMultiScreen;				//�ര��ȫ��
	BOOL		bNoDecode;					//soft decode or not
	BOOL		bPreviewBlock;				//preview block or not

	VIDEO_INFO	struVideoInfo[MAX_OUTPUTS];	//��Ƶ����
	int			iVolume;					//����
	BOOL		bBroadCast;					//�Ƿ���������㲥
	char		chIPServerIP[16];
	BOOL		bOutputDebugString;

	LOCAL_RECORD_TIME struLocalRecordTime[7][4];

	STRU_LOCAL_PARAM()
	{
		bCyclePlay			= FALSE;
		iCycleTime			= 20;
		bUseCard		    = FALSE;
		bNTSC				= FALSE;
		bAutoRecord			= FALSE;
		bCycleRecord		= FALSE;
		iStartRecordDriver = 0;
		iEndRecordDriver   = 0;
		iRecFileSize	   = 1;
		iRecordFileInterval = 60;
		sprintf(chDownLoadPath, "%s","C:\\DownLoad");
		sprintf(chPictureSavePath, "%s", "C:\\Picture");
		sprintf(chRemoteCfgSavePath ,"%s", "C:\\SaveRemoteCfgFile");
		sprintf(chClientRecordPath ,"%s", "C:\\mpeg4record\\2008-04-30");

		bAutoCheckDeviceTime	=  FALSE;
		lCheckDeviceTime	= 0;

		iAlarmDelayTime		= 10;
		iAlarmListenPort	= 7200;

		bAutoSaveLog		= TRUE;
		bAlarmInfo			= TRUE;
		bSuccLog			= TRUE;
		bFailLog			= TRUE;

		bAllDiskFull		= FALSE;
		bPlaying			= FALSE;
		bCycling			= FALSE;
		bPaused				= FALSE;
		bNextPage			= FALSE;
		bFrontPage			= FALSE;
		bEnlarged			= FALSE;
		bFullScreen			= FALSE;
		bMultiScreen		= FALSE;
		iVolume				= DEFAULT_VOLUME;
		bBroadCast			= FALSE;
		bNoDecode			= FALSE;
		bPreviewBlock		= TRUE;
		chIPServerIP[0] 	= '\0';
		bOutputDebugString	= FALSE;
		memset(&struLocalRecordTime[0][0], 0, sizeof(LOCAL_RECORD_TIME)*7*4);
	}
}LOCAL_PARAM, *PLOCAL_PARAM;

#endif
