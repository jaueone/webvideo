
#ifndef _INFO_DIFFUSION_PARAMS_CONVERT_H_
#define _INFO_DIFFUSION_PARAMS_CONVERT_H_

#include "XMLParamsConvert.h"

/************************************************************************/
/* �궨��                                                                 
/************************************************************************/
#define MAX_LEN_32                  32    //����32
#define MAX_LEN_128                 128   //����128
#define MAX_LEN_NAME                64    //������󳤶�
#define MAX_LEN_REMARKS             256   //��ע��󳤶�
#define MAX_LEN_URL                 512   //URL��󳤶�
#define MAX_LEN_TEXT                512   //�ı���󳤶� 
#define MAX_NUM_MATERIAL            1024  //�ز��������
#define MAX_PAGES_A_PROGRAM         128   //һ����Ŀ��ҳ���������
#define MAX_CONTENTS_A_PAGE         32    //һ��ҳ��������������
#define MAX_NUM_PROGRAM             32    //��Ŀ�������
#define MAX_NUM_CONTAINER           128   //��Ƭ�������
#define MAX_SPANS_A_DAY             8     //һ��Ĳ���ʱ���������
#define MAX_NUM_SELDEFINE           32    //�Զ���ʱ�ε��������
#define MAX_PROGRAMS_A_LOOP         16    //ѭ���ճ̵Ľ�Ŀ�������
#define MAX_DAYS_A_WEEK             7     //һ�ܵ�����
#define MAX_NUM_SCHEDULE            128   //�ճ��������
#define MAX_NUM_TERMINAL            1024  //�ն��������
#define MAX_IPCS_A_TERMINAL         32    //һ���ն�֧�ֵ�IPC�������
#define MAX_CPUS_A_TERMINAL         32    //һ���ն˵�CPU������
#define MAX_MEMS_A_TERMINAL         32    //һ���ն˵��ڴ�������
#define MAX_DISK_A_TERMINAL         32    //һ���ն˵Ĵ���������
#define MAX_NUM_TERGROUP            128   //�ն����������
#define MAX_TERS_A_GROUP            128   //�����ն��������
#define MAX_ITEMS_A_PLAYLIST        8     //�����б��б����������
#define MAX_NUM_SWITCH_PLAN         32    //���ػ��ƻ��������
#define MAX_NUM_VOLUME_PLAN         32    //�����ƻ��������

/************************************************************************/
/* �ṹ�嶨��                                              
/************************************************************************/
//�زĹ���
typedef struct tagRGB //RGB
{
	DWORD dwRed;   //��ɫ
	DWORD dwGreen; //��ɫ
	DWORD dwBlue;  //��ɫ
}RGB, *LPRGB;

typedef struct tagNET_DVR_DISP_PARAM
{
	BOOL byEnable;
	DWORD dwFontSize;
	RGB struFontColor;
	RGB struBackColor;
	DWORD dwPositionX;
	DWORD dwPositionY;
	DWORD dwHeight;
	DWORD dwWidth;
}NET_DVR_DISP_PARAM,*LPDVR_DISP_PARAM;

typedef struct tagNET_DVR_CLOCK
{
	DWORD dwTotalHeight;
	DWORD dwTotalWidth;
	DWORD dwBackPicId;
	BOOL byIconEnable;
	char szIconType[MAX_LEN_32];
	DWORD dwIconX;
	DWORD dwIconY;
	DWORD dwIconH;
	DWORD dwIconW;
	NET_DVR_DISP_PARAM struYmd;
	NET_DVR_DISP_PARAM struHms;
	NET_DVR_DISP_PARAM struWeek;
}NET_DVR_CLOCK,*LPNET_DVR_CLOCK;

typedef struct tagNET_DVR_WEATHER
{
	DWORD dwTotalHeight;
	DWORD dwTotalWidth;
	DWORD dwBackPicId;
	BOOL byIconEnable;
	DWORD dwIconX;
	DWORD dwIconY;
	DWORD dwIconH;
	DWORD dwIconW;
	NET_DVR_DISP_PARAM struDate;
	NET_DVR_DISP_PARAM struTemp;
	NET_DVR_DISP_PARAM struContent;
	NET_DVR_DISP_PARAM struCity;
	NET_DVR_DISP_PARAM struHum;
	NET_DVR_DISP_PARAM struAir;
	NET_DVR_DISP_PARAM struUpdate;
	NET_DVR_DISP_PARAM struWind;
}NET_DVR_WEATHER,*LPNET_DVR_WEATHER;

typedef struct tagNET_DVR_STATIC_MATERIAL //��̬�ز�
{
	char  szStaticMaterialType[MAX_LEN_32];   //��̬�ز�����
	char  szStaticMaterialFormat[MAX_LEN_32]; //��̬�زĸ�ʽ
	DWORD dwFileSize;                         //�ز��ļ���С����λ���ֽ�
	DWORD dwDuration;                         //�ز�ʱ��
	NET_DVR_CLOCK struLock;                    //ʱ��
	NET_DVR_WEATHER struWeather;              //����
}NET_DVR_STATIC_MATERIAL, *LPNET_DVR_STATIC_MATERIAL;

typedef struct tagNET_DVR_IPADDR_ //IP��ַ
{		
    char	szIpV4[16];	 //IPv4��ַ
    char 	szIpV6[128]; //����
}NET_DVR_IPADDR_, *LPNET_DVR_IPADDR_;

typedef struct tagNET_DVR_ADDRESS_ //��ý���ַ
{
	char  szAddressType[MAX_LEN_32]; //��ַ����
	char  szHostName[MAX_LEN_32];    //��������
	NET_DVR_IPADDR_ struIPAddr;      //IP��ַ
	DWORD dwPortNo;                  //�˿ں�
}NET_DVR_ADDRESS_, *LPNET_DVR_ADDRESS_;

typedef struct tagNET_DVR_STREAM_MEDIA //��ý��
{
	NET_DVR_ADDRESS_ struAddress; //��ý���ַ
	char szUrl[MAX_LEN_URL];                  //url
}NET_DVR_STREAM_MEDIA, *LPNET_DVR_STREAM_MEDIA;

typedef struct tagNET_DVR_DYNAMIC_MATERIAL //��̬�ز�
{
	char szDynamicMaterialType[MAX_LEN_32]; //��̬�ز�����
	NET_DVR_STREAM_MEDIA struStreamMedia;   //��ý��
	char szWebUrl[MAX_LEN_URL];             //Web Url
	char szRssUrl[MAX_LEN_URL];             //Rss Url
}NET_DVR_DYNAMIC_MATERIAL, *LPNET_DVR_DYNAMIC_MATERIAL;

typedef struct tagNET_DVR_MATERIAL //�ز�
{
	DWORD dwSize;
	DWORD dwMaterialID;                           //�ز�ID
	char  szMaterialName[MAX_LEN_NAME];           //�ز�����
	char  szMaterialRemarks[MAX_LEN_REMARKS];     //�ز�����
	char  szMaterialType[MAX_LEN_32];             //�ز�����
	char  szApprove[MAX_LEN_32];                  //���
	char  szApproveRemarks[MAX_LEN_REMARKS];      //������
	char  szSharePropery[MAX_LEN_32];             //��������
	char  szUploadUser[MAX_LEN_32];               //�ϴ���
	char  szUploadTime[MAX_LEN_32];               //�ϴ�ʱ��
	NET_DVR_STATIC_MATERIAL struStaticMaterial;   //��̬�ز�
	NET_DVR_DYNAMIC_MATERIAL struDynamicMaterial; //��̬�ز�
    char szOperatorType[MAX_LEN_32];              //������������
    DWORD dwOrgNo;
}NET_DVR_MATERIAL, *LPNET_DVR_MATERIAL;

typedef struct tagNET_DVR_MATERIAL_LIST //�ز��б�
{
	DWORD dwSize;
	NET_DVR_MATERIAL struMaterialList[MAX_NUM_MATERIAL]; //�ز��б�
}NET_DVR_MATERIAL_LIST, *LPNET_DVR_MATERIAL_LIST;

//��Ŀ����
typedef struct tagNET_DVR_RESOLUTION //�ֱ���
{
	DWORD dwImageWidth;                 //ͼ����
	DWORD dwImageHeight;                //ͼ��߶�
}NET_DVR_RESOLUTION, *LPNET_DVR_RESOLUTION;

typedef struct tagNET_DVR_POSITION //λ��
{
	DWORD dwPositionX; //���Ͻ�X����
	DWORD dwPositionY; //���Ͻ�Y����
	DWORD dwWidth;     //���
	DWORD dwHeight;    //�߶�
}NET_DVR_POSITION, *LPNET_DVR_POSITION;

typedef struct tagNET_DVR_TEXT //�ı�
{
	char  szContent[MAX_LEN_TEXT];       //�ı�����
	char  szFontType[MAX_LEN_32];        //��������
	DWORD dwFontSize;                    //�����С
	char  szAlign[MAX_LEN_32];           //���뷽ʽ
	DWORD dwRowHeight;                   //�и�
	char  szScrollType[MAX_LEN_32];      //��������
	char  szScrollDirection[MAX_LEN_32]; //��������
	DWORD dwScrollSpeed;                 //�����ٶ�
}NET_DVR_TEXT, *LPNET_DVR_TEXT;

typedef struct tagNET_DVR_MARQUEE //�����
{
	char  szScrollType[MAX_LEN_32];      //��������
	char  szScrollDirection[MAX_LEN_32]; //��������
	DWORD dwScrollSpeed;                 //�����ٶ�
}NET_DVR_MARQUEE, *LPNET_DVR_MARQUEE;

typedef struct tagNET_DVR_DURATION //����ʱ��
{
	char  szDurationType[MAX_LEN_32]; //ʱ������
	DWORD dwDuration;                 //����ʱ��
}NET_DVR_DURATION, *LPNET_DVR_DURATION;



typedef struct tagNET_DVR_CHARACTERS_EFECT//������ʾЧ��
{
	DWORD dwFontSize;                  //�����С
	RGB struFontColor;                 //������ɫ
	RGB struBackColor;                 //������ɫ
	DWORD dwBackTransparent;           //����͸����
	BYTE bySubtitlesEnabled;           //ʹ���ַ���ʾģʽ
	char szScrollDirection[MAX_LEN_32];//���ֹ�������
	DWORD dwScrollSpeed;               //���ֹ����ٶ�
}NET_DVR_CHARACTERS_EFECT,*LPNET_DVR_CHARACTERS_EFECT;

typedef struct tagNET_DVR_PLAY_ITEM_ //������Ŀ 
{
	DWORD dwPlayID;  
	DWORD dwMaterialID;                //�ز�ID
	char  szPlayEffect[MAX_LEN_32];    //����Ч��
	NET_DVR_MARQUEE  struMarquee;      //�����
	NET_DVR_DURATION struPlayDuration; //����ʱ�� 
	NET_DVR_CHARACTERS_EFECT struCharactersEffect;//������ʾЧ�������ز�����Ϊ�ı�txtʱ��Ч
	char szSwitchEffect[MAX_LEN_32];               //�����ز��л�Ч����ͼƬ�ز�ʱ��Ч
}NET_DVR_PLAY_ITEM_, *LPNET_DVR_PLAY_ITEM_;

typedef struct tagNET_DVR_CONTENT //ҳ������
{
	DWORD dwContentID;                     //����ID
	NET_DVR_POSITION  struPosition;        //����λ��
	DWORD dwLayerID;                       //ͼ��ID
	char  sWinMaterialType[MAX_LEN_32];    //�����ز�����
	char  sStaticMaterialType[MAX_LEN_32]; //��̬�ز�����
	NET_DVR_PLAY_ITEM_ struPlayList[MAX_ITEMS_A_PLAYLIST]; //�����б�
}NET_DVR_CONTENT, *LPNET_DVR_CONTENT;



typedef struct tagNET_DVR_PAGE_BASIC //ҳ�������Ϣ
{
	char  sPageName[MAX_LEN_NAME];       //ҳ������
	RGB   struBkColor;                   //����ɫ
	char  sPlayMode[MAX_LEN_32];         //����ģʽ
	DWORD dwPlayDuration;                //����ʱ��
	DWORD dwSwitchDuraton;               //�л�ʱ��
	char  szSwitchEffect[MAX_LEN_32];    //�л�Ч��
	DWORD dwBackgroundPic;               //����ͼƬ
}NET_DVR_PAGE_BASIC, *LPNET_DVR_PAGE_BASIC;

typedef struct tagNET_DVR_PAGE //ҳ��
{
	DWORD dwSize;
	DWORD dwPageID;                   //ҳ��ID
	NET_DVR_PAGE_BASIC struBasicInfo; //ҳ�������Ϣ
	NET_DVR_CONTENT struContentList[MAX_CONTENTS_A_PAGE]; //�����б�
}NET_DVR_PAGE, *LPNET_DVR_PAGE;

typedef struct tagNET_DVR_PAGE_LIST //ҳ���б�
{
	DWORD dwSize;
	NET_DVR_PAGE struPageList[MAX_PAGES_A_PROGRAM]; //ҳ���б�
}NET_DVR_PAGE_LIST, *LPNET_DVR_PAGE_LIST;

typedef struct tagNET_DVR_PROGRAM //��Ŀ
{
	DWORD dwSize;
	DWORD dwProgramID;                       //��ĿID
	char  szProgramName[MAX_LEN_NAME];       //��Ŀ����
	char  szProgramRemarks[MAX_LEN_REMARKS]; //��Ŀ��ע
	char  szShareProp[MAX_LEN_32];           //��������
	char  szApprove[MAX_LEN_32];             //���
	char  szApproveRemarks[MAX_LEN_REMARKS]; //������
	NET_DVR_RESOLUTION struResolution;       //�ֱ���
	DWORD dwProgramSize;                     //��Ŀ�ܴ�С
	DWORD dwProgramLength;                   //��Ŀ��ʱ��
	NET_DVR_PAGE_LIST  struPageList;         //ҳ���б�
    char szOperatorType[MAX_LEN_32];         //������������
}NET_DVR_PROGRAM, *LPNET_DVR_PROGRAM;

typedef struct tagNET_DVR_PROGRAM_LIST //��Ŀ�б�
{
	DWORD dwSize;
	NET_DVR_PROGRAM struProgramList[MAX_NUM_PROGRAM]; //��Ŀ�б�
}NET_DVR_PROGRAM_LIST, *LPNET_DVR_PROGRAM_LIST;

typedef struct tagNET_DVR_CONTAINER //��Ƭ
{
	DWORD dwSize;
	DWORD dwContainerID; //��ƬID
	char  szContainerName[MAX_LEN_NAME]; //��Ƭ����
	DWORD dwPlayDuration;                //����ʱ��
	DWORD dwSwitchTime;                  //�л�ʱ��
	char  szSwitchEffect[MAX_LEN_32];    //�л�Ч��
}NET_DVR_CONTAINER, *LPNET_DVR_CONTAINER;

typedef struct tagNET_DVR_CONTAINER_LIST //��Ƭ�б�
{
	DWORD dwSize;
	NET_DVR_CONTAINER struContainerList[MAX_NUM_CONTAINER]; //��Ƭ�б�
}NET_DVR_CONTAINER_LIST, *LPNET_DVR_CONTAINER_LIST;

//�ճ̹���
typedef struct tagNET_DVR_TIME_ //ʱ��
{
    DWORD dwYear;		//��
    DWORD dwMonth;		//��
    DWORD dwDay;		//��
    DWORD dwHour;		//ʱ
    DWORD dwMinute;		//��
    DWORD dwSecond;		//��
}NET_DVR_TIME_, *LPNET_DVR_TIME_;

typedef struct tagNET_DVR_TIME_RANGE //ʱ���
{
	NET_DVR_TIME_ struBeginTime; //��ʼʱ��
	NET_DVR_TIME_ struEndTime;   //����ʱ��
}NET_DVR_TIME_RANGE, *LPNET_DVR_TIME_RANGE;

typedef struct tagNET_DVR_PLAY_SPAN //ʱ�β���
{
	DWORD dwSpanID;    //ʱ��ID
	DWORD dwProgramID; //�����Ľ�ĿID
	NET_DVR_TIME_RANGE struTimeRange; //ʱ���
}NET_DVR_PLAY_SPAN, *LPNET_DVR_PLAY_SPAN;

typedef struct tagNET_DVR_PLAY_DAY //���첥��
{
	DWORD dwDayID;               //��ID
	char  szWeekday[MAX_LEN_32]; //�ܼ�
	NET_DVR_PLAY_SPAN struSpanList[MAX_SPANS_A_DAY]; //ʱ���б�
}NET_DVR_PLAY_DAY, *LPNET_DVR_PLAY_DAY;

typedef struct tagNET_DVR_PLAY_SELDEFINE //�Զ��岥��
{
	DWORD dwSeldefineID; //�Զ�����
	DWORD dwProgramID;   //��ĿID
	NET_DVR_TIME_RANGE struTimeRange; //ʱ���
}NET_DVR_PLAY_SELDEFINE, *LPNET_DVR_PLAY_SELDEFINE;

typedef struct tagNET_DVR_PLAY_LOOP //ѭ������
{
	DWORD dwLoopID;    //ѭ���ڵ�ID
	DWORD dwProgramID; //��ĿID
}NET_DVR_PLAY_LOOP, *LPNET_DVR_PLAY_LOOP;

typedef struct tagNET_DVR_PLAY_SCHEDULE //�ճ�
{
	DWORD dwSize;
	DWORD dwScheduleID;                       //�ճ�ID
	char  szScheduleName[MAX_LEN_NAME];       //�ճ�����
	char  szScheduleRemarks[MAX_LEN_REMARKS]; //�ճ̱�ע
	char  szScheduleType[MAX_LEN_32];         //�ճ�����
	char  szApprove[MAX_LEN_32];              //���
	char  szApproveRemarks[MAX_LEN_REMARKS];  //������
	char  szShareProp[MAX_LEN_32];            //��������
    char szScheduleMode[MAX_LEN_32];
    DWORD dwOrgNo;
	NET_DVR_PLAY_SPAN struDailySchedule[MAX_SPANS_A_DAY];            //���ճ�
	NET_DVR_PLAY_DAY struWeeklySchedule[MAX_DAYS_A_WEEK];            //���ճ�
	NET_DVR_PLAY_LOOP struLoopSchedule[MAX_PROGRAMS_A_LOOP];         //��Ŀѭ���ճ�
	NET_DVR_PLAY_SELDEFINE struSeldefineSchedule[MAX_NUM_SELDEFINE]; //�Զ����ճ�
}NET_DVR_PLAY_SCHEDULE, *LPNET_DVR_PLAY_SCHEDULE;


typedef struct tagNET_DVR_SCHEDULE_RELEASE //�ճ̷���
{
	DWORD dwSize;
	DWORD dwScheduleID;                          //�ճ�ID
	char  szReleaseType[MAX_LEN_32];             //��������
	DWORD dwTerminalCount;                       //�ն�����
	DWORD dwGroupCount;                          //�ն�������
	DWORD dwTerminalList[MAX_NUM_TERMINAL];      //�ն��б�
	DWORD dwTerminalGroupList[MAX_NUM_TERGROUP]; //�ն����б�
    char  szEffectTime[MAX_LEN_32];
}NET_DVR_SCHEDULE_RELEASE, *LPNET_DVR_SCHEDULE_RELEASE;

//���ſ���
typedef struct tagNET_DVR_INSERT_INFO //������Ϣ
{
	char  szInsertType[MAX_LEN_32]; //��������
	DWORD dwMaterialID;             //�زı��
	DWORD dwProgramID;              //��Ŀ���
	char  szPlayMode[MAX_LEN_32];   //����ģʽ
	DWORD dwCount;                  //���Ŵ���
	DWORD dwDuration;               //����ʱ��
	DWORD dwPositionX;              //���Ͻ�X����
	DWORD dwPositionY;              //���Ͻ�Y����
	DWORD dwPositionWidth;          //���
	DWORD dwPostionHeight;          //�߶�
	NET_DVR_CHARACTERS_EFECT struChacEff;//������ʾЧ��
    char  szEndTime[MAX_LEN_32];
}NET_DVR_INSERT_INFO, *LPNET_DVR_INSERT_INFO;

typedef struct tagNET_DVR_PLAN_CTRL
{
	char szPlanType[MAX_LEN_32];             //�ƻ�����
	BYTE byEnable;               //�ƻ���ͣ
}NET_DVR_PLAN_CTRL,*LPNET_DVR_PLAN_CTRL;

typedef struct tagNET_DVR_PLAY_CONTROL //���ſ���
{
	DWORD dwSize;
	char  szControlType[MAX_LEN_32];             //��������
	char  szTargetType[MAX_LEN_32];              //Ŀ������
	DWORD dwTerminalCount;                       //�ն�����
	DWORD dwGroupCount;                          //�ն�������
	DWORD dwTerminalList[MAX_NUM_TERMINAL];      //�ն��б�
	DWORD dwTerminalGroupList[MAX_NUM_TERGROUP]; //�ն����б�
	NET_DVR_INSERT_INFO struInsertInfo;          //������Ϣ
	NET_DVR_PLAN_CTRL  struPlanCtrl;             //�ƻ�����
}NET_DVR_PLAY_CONTROL, *LPNET_DVR_PLAY_CONTROL;

//�ն���
typedef struct tagNET_DVR_TERMINAL_GROUP //�ն���
{
	DWORD dwSize;                  
	DWORD dwGroupID;               //��ID
	char  szGroupName[MAX_LEN_32]; //������
	DWORD dwTermCount;             //�����ն�����
	DWORD dwTerminalList[MAX_TERS_A_GROUP]; //�ն��б�
}NET_DVR_TERMINAL_GROUP, *LPNET_DVR_TERMINAL_GROUP;

typedef struct tagNET_DVR_TERMINAL_GROUP_LIST //�ն����б�
{
	DWORD dwSize;
	NET_DVR_TERMINAL_GROUP struGroupList[MAX_NUM_TERGROUP]; //�ն����б�
}NET_DVR_TERMINAL_GROUP_LIST, *LPNET_DVR_TERMINAL_GROUP_LIST;


typedef struct tagNET_DVR_PUBLISH_SERVER_ADDR
{
    DWORD dwSize;
    char szFormatType[MAX_LEN_32]; //��ַ����
    char szHostName[MAX_LEN_32]; //������
    char szIpv4[128]; //Ipv4
    char szIpv6[128]; //Ipv6
    WORD wPort; //�˿ں�
    char szOparaType[MAX_LEN_32]; //���ö���
    BOOL byRegister; //ע��״̬
    DWORD dwTerminalNo;//�ն˸���
    DWORD arrTerminal[MAX_NUM_TERMINAL];
    DWORD dwGroupNo;//�ն������
    DWORD arrGroup[MAX_NUM_TERGROUP];//�ն�����
    char szUserName[MAX_LEN_32]; 
    char szPwd[MAX_LEN_32]; 
}NET_DVR_PUBLISH_SERVER_ADDR,*LPNET_DVR_PUBLISH_SERVER_ADDR;


//��������
typedef struct tagNET_DVR_SCREEN_PARAM //��Ļ����
{
	DWORD dwBackLightLevel;  //�������
	BYTE  byBootLogoEnabled;  //����logo�Ƿ���ʾ
}NET_DVR_SCREEN_PARAM, *LPNET_DVR_SCREEN_PARAM;

typedef struct tagNET_DVR_PLAY_CFG //��������
{
	DWORD dwSize;
	DWORD dwTerminalID;   //�ն�ID
	DWORD dwPlayerVolume; //����������
	NET_DVR_SCREEN_PARAM struScreenParam; //��Ļ����
}NET_DVR_PLAY_CFG, *LPNET_DVR_PLAY_CFG;

typedef struct tagNET_DVR_PLAY_CFG_LIST //���������б�
{
	DWORD dwSize;
	NET_DVR_PLAY_CFG struPlayCfgList[MAX_NUM_TERMINAL]; //�ն˵Ĳ�������
}NET_DVR_PLAY_CFG_LIST, *LPNET_DVR_PLAY_CFG_LIST;

//���ػ��ƻ�
typedef struct tagNET_DVR_SWITCH_SPAN //���ػ�ʱ��
{
	DWORD dwSpanID;                //ID
	char  sSwitchType[MAX_LEN_32]; //����/�ػ�
	NET_DVR_TIME_ struTime;        //ʱ��
}NET_DVR_SWITCH_SPAN, LPNET_DVR_SWITCH_SPAN;

typedef struct tagNET_DVR_SWITCH_DAY //�տ��ػ�ʱ��
{
	DWORD dwDayID;              //ID
	char  sWeekday[MAX_LEN_32]; //�ܼ�
	NET_DVR_SWITCH_SPAN struSpanList[MAX_SPANS_A_DAY]; //ʱ���б�
}NET_DVR_SWITCH_DAY, *LPNET_DVR_SWITCH_DAY;

typedef struct tagNET_DVR_SWITCH_PLAN //���ػ��ƻ�
{
	DWORD dwSize;
	char  sPlanType[MAX_LEN_32];   //�ƻ�����
	NET_DVR_SWITCH_SPAN struDailyPlan[MAX_SPANS_A_DAY];        //�ռƻ�
	NET_DVR_SWITCH_DAY  struWeekyPlan[MAX_DAYS_A_WEEK];        //�ܼƻ�
	NET_DVR_SWITCH_SPAN struSelfdefinePlan[MAX_NUM_SELDEFINE]; //�Զ���ƻ�
}NET_DVR_SWITCH_PLAN, *LPNET_DVR_SWITCH_PLAN;

typedef struct tagNET_DVR_SWITCH_PLAN_LIST //���ػ������б�
{
	DWORD dwSize;
	NET_DVR_SWITCH_PLAN struSwitchPlanList[MAX_NUM_SWITCH_PLAN];
}NET_DVR_SWITCH_PLAN_LIST, *LPNET_DVR_SWITCH_PLAN_LIST;

typedef struct tagNET_DVR_SWITCH_PLAN_CFG
{
    DWORD dwSize;
    NET_DVR_SWITCH_PLAN struSwitchPlan;
    char szOperType[MAX_LEN_NAME]; //��������
    DWORD dwGroupNo; //�ն������
    DWORD dwListGroup[MAX_NUM_TERGROUP];
    DWORD dwTerminalNo; //�ն˸���
    DWORD dwListTerminal[MAX_NUM_TERMINAL];

}NET_DVR_SWITCH_PLAN_CFG,*LPNET_DVR_SWITCH_PLAN_CFG;


typedef struct tagNET_DVR_VOLUME_SPAN //���ػ�ʱ��
{
    DWORD dwSpanID;                //ID
    DWORD  dwVolume;               //����
    NET_DVR_TIME_ struTime;        //ʱ��
    NET_DVR_TIME_ struTimeEnd;
}NET_DVR_VOLUME_SPAN, LPNET_DVR_VOLUME_SPAN;

typedef struct tagNET_DVR_VOLUME_DAY //�տ��ػ�ʱ��
{
    DWORD dwDayID;              //ID
    char  sWeekday[MAX_LEN_32]; //�ܼ�
    NET_DVR_VOLUME_SPAN struSpanList[MAX_SPANS_A_DAY]; //ʱ���б�
}NET_DVR_VOLUME_DAY, *LPNET_DVR_VOLUME_DAY;

typedef struct tagNET_DVR_VOLUME_PLAN //�������ƻ�
{
    DWORD dwSize;
    char  sPlanType[MAX_LEN_32];   //�ƻ�����
    NET_DVR_VOLUME_SPAN struDailyPlan[MAX_SPANS_A_DAY];        //�ռƻ�
    NET_DVR_VOLUME_DAY  struWeekyPlan[MAX_DAYS_A_WEEK];        //�ܼƻ�
}NET_DVR_VOLUME_PLAN, *LPNET_DVR_VOLUME_PLAN;


typedef struct tagNET_DVR_VOLUME_PLAN_LIST //���ػ������б�
{
    DWORD dwSize;
    NET_DVR_VOLUME_PLAN struVolumePlanList[MAX_NUM_VOLUME_PLAN];
}NET_DVR_VOLUME_PLAN_LIST, *LPNET_DVR_VOLUME_PLAN_LIST;

typedef struct tagNET_DVR_VOLUME_PLAN_CFG
{
    DWORD dwSize;
    NET_DVR_VOLUME_PLAN struVolumePlan;
    char szOperType[MAX_LEN_32];
    DWORD dwTerminalNo;
    DWORD dwListTerminal[MAX_NUM_TERMINAL];
    DWORD dwGroupNo;
    DWORD dwListGroup[MAX_NUM_TERGROUP];
}NET_DVR_VOLUME_PLAN_CFG,*LPNET_DVR_VOLUME_PLAN_CFG;

typedef struct tagNET_DVR_MATERIAL_SEARCH_DESCRIPTION //�زĲ�ѯ����
{
	char  sSearchID[MAX_LEN_32];            //����ID
	char  sApproveState[MAX_LEN_32];        //���״̬
	char  sMaterialType[MAX_LEN_32];        //�ز�����
	char  sShareProperty[MAX_LEN_32];       //��������
	char  sUploader[MAX_LEN_32];            //�ϴ���
	char  sStaticMaterialType[MAX_LEN_32];  //��̬�ز�����
	DWORD dwMinStaticMaterialSize;          //��̬�زĴ�С��Сֵ
	DWORD dwMaxStaticMaterialSize;          //��̬�زĴ�С���ֵ
	char  sDynamicMaterialType[MAX_LEN_32]; //��̬�ز�����
	NET_DVR_TIME_RANGE struTimeRange;       //��ѯʱ�䷶Χ
	DWORD dwMaxResults;                     //��෵�ص��ز�����
	DWORD dwSearchResultsPosition;          //�Ӳ�ѯ�����ָ��λ�ÿ�ʼ�����ز���Ϣ
}NET_DVR_MATERIAL_SEARCH_DESCRIPTION, *LPNET_DVR_MATERIAL_SEARCH_DESCRIPTION;

typedef struct tagNET_DVR_MATERIAL_SEARCH_RESULT //�زĲ�ѯ���
{
	char  sSearchID[MAX_LEN_32];             //����ID
	BOOL  bResponseStatus;                   //��ѯ״̬
	char  sResponseStatusString[MAX_LEN_32]; //��ѯ״̬�ַ���
	DWORD dwTotalMatches;                    //��ѯ�����ز�������
	DWORD dwNumOfMatches;                    //���ص��ز�����
	NET_DVR_MATERIAL struMaterialList[MAX_NUM_MATERIAL]; //�ز��б�
}NET_DVR_MATERIAL_SEARCH_RESULT, *LPNET_DVR_MATERIAL_SEARCH_RESULT;

typedef struct tagNET_DVR_PROGRAM_SEARCH_DESCRIPTION //��Ŀ��ѯ����
{
	char  sSearchID[MAX_LEN_32];            //����ID
	char  sApproveState[MAX_LEN_32];        //���״̬
	char  sShareProperty[MAX_LEN_32];       //��������
	char  sUploader[MAX_LEN_32];            //�ϴ���
	NET_DVR_RESOLUTION struResolution;      //��Ŀ�ֱ���
	NET_DVR_TIME_RANGE struTimeRange;       //��ѯʱ�䷶Χ
	DWORD dwMaxResults;                     //��෵�صĽ�Ŀ����
	DWORD dwSearchResultsPosition;          //�Ӳ�ѯ�����ָ��λ�ÿ�ʼ���ؽ�Ŀ��Ϣ
}NET_DVR_PROGRAM_SEARCH_DESCRIPTION, *LPNET_DVR_PROGRAM_SEARCH_DESCRIPTION;

typedef struct tagNET_DVR_PROGRAM_BASIC_INFO //��Ŀ������Ϣ
{
	DWORD dwProgramID;                       //��ĿID
	char  szProgramName[MAX_LEN_NAME];       //��Ŀ����
	char  szProgramRemarks[MAX_LEN_REMARKS]; //��Ŀ��ע
	char  szShareProp[MAX_LEN_32];           //��������
	char  szApprove[MAX_LEN_32];             //���
	char  szApproveRemarks[MAX_LEN_REMARKS]; //������
	NET_DVR_RESOLUTION struResolution;       //�ֱ���
	DWORD dwProgramSize;                     //��Ŀ�ܴ�С
	DWORD dwProgramLength;                   //��Ŀ��ʱ��
}NET_DVR_PROGRAM_BASIC_INFO, *LPNET_DVR_PROGRAM_BASIC_INFO;

typedef struct tagNET_DVR_PROGRAM_SEARCH_RESULT //��Ŀ��ѯ���
{
	char  sSearchID[MAX_LEN_32];             //����ID
	BOOL  bResponseStatus;                   //��ѯ״̬
	char  sResponseStatusString[MAX_LEN_32]; //��ѯ״̬�ַ���
	DWORD dwTotalMatches;                    //��ѯ���Ľ�Ŀ������
	DWORD dwNumOfMatches;                    //���صĽ�Ŀ����
	NET_DVR_PROGRAM_BASIC_INFO struProgramList[MAX_NUM_PROGRAM]; //��Ŀ�б�
}NET_DVR_PROGRAM_SEARCH_RESULT, *LPNET_DVR_PROGRAM_SEARCH_RESULT;

typedef struct tagNET_DVR_SCHEDULE_SEARCH_DESCRIPTION //�ճ̲�ѯ����
{
	char  sSearchID[MAX_LEN_32];            //����ID
	char  sApproveState[MAX_LEN_32];        //���״̬
	char  sShareProperty[MAX_LEN_32];       //��������
	char  sUploader[MAX_LEN_32];            //�ϴ���
	char  sScheduleType[MAX_LEN_32];        //�ճ�����
	NET_DVR_TIME_RANGE struTimeRange;       //��ѯʱ�䷶Χ
	DWORD dwMaxResults;                     //��෵�ص��ճ�����
	DWORD dwSearchResultsPosition;          //�Ӳ�ѯ�����ָ��λ�ÿ�ʼ�����ճ���Ϣ
}NET_DVR_SCHEDULE_SEARCH_DESCRIPTION, *LPNET_DVR_SCHEDULE_SEARCH_DESCRIPTION;

typedef struct tagNET_DVR_SCHEDULE_BASIC_INFO //�ճ̻�����Ϣ
{
	DWORD dwScheduleID;                       //�ճ�ID
	char  szScheduleName[MAX_LEN_NAME];       //�ճ�����
	char  szScheduleRemarks[MAX_LEN_REMARKS]; //�ճ̱�ע
	char  szScheduleType[MAX_LEN_32];         //�ճ�����
	char  szApprove[MAX_LEN_32];              //���
	char  szApproveRemarks[MAX_LEN_REMARKS];  //������
	char  szShareProp[MAX_LEN_32];            //��������
}NET_DVR_SCHEDULE_BASIC_INFO, *LPNET_DVR_SCHEDULE_BASIC_INFO;

typedef struct tagNET_DVR_SCHEDULE_SEARCH_RESULT //�ճ̲�ѯ���
{
	char  sSearchID[MAX_LEN_32];             //����ID
	BOOL  bResponseStatus;                   //��ѯ״̬
	char  sResponseStatusString[MAX_LEN_32]; //��ѯ״̬�ַ���
	DWORD dwTotalMatches;                    //��ѯ�����ճ�������
	DWORD dwNumOfMatches;                    //���ص��ճ�����
	NET_DVR_SCHEDULE_BASIC_INFO struScheduleList[MAX_NUM_SCHEDULE]; //�ճ��б�
}NET_DVR_SCHEDULE_SEARCH_RESULT, *LPNET_DVR_SCHEDULE_SEARCH_RESULT;

/************************************************************************/
/* ת������                                                             */
/************************************************************************/
//��ȡResponseStatus�е�״̬���ַ���
char* GetStatusCodeInfo(const DWORD dwStatusCide);

//��ȡResponseStatus��ID
DWORD GetResponseStatusID(const char* pXmlBuf);

//�ز�
BOOL ConvertMaterialParamsXmlToStru(CXmlBase& struXml, LPVOID pStruct);
BOOL ConvertMaterialParamsXmlToStru(const char* pXmlBuf, LPVOID pStruct);
BOOL ConvertMaterialParamsStruToXml(LPVOID pStruct, char* *pXmlBuf, DWORD &dwXmlLen);
BOOL ConvertMaterialParamsXmlToStruList(const char* pXmlBuf, LPVOID pStruct);
BOOL ConvertMaterialSearchDescriptionParamsStruToXml(LPVOID pStruct, char* *pXmlBuf, DWORD &dwXmlLen);
BOOL ConvertMaterialSearchResultXmlToStru(const char* pXmlBuf, LPVOID pStruct);

//ҳ��
BOOL ConvertPageParamsXmlToStru(CXmlBase& struXml, LPVOID pStruct);
BOOL ConvertPageParamsXmlToStru(const char* pXmlBuf, LPVOID pStruct);
BOOL ConvertPageParamsStruToXml(LPVOID pStruct, char* *pXmlBuf, DWORD &dwXmlLen);
BOOL ConvertPageParamsStruToXml(LPVOID pStruct, CXmlBase& struXml);
BOOL ConvertPageParamsXmlToStruList(CXmlBase& struXml, LPVOID pStruct);
BOOL ConvertPageParamsXmlToStruList(const char* pXmlBuf, LPVOID pStruct);
BOOL ConvertPageParamsStruToXmlList(LPVOID pStruct, CXmlBase& struXml);

//��Ŀ
BOOL ConvertProgramParamsXmlToStru(CXmlBase& struXml, LPVOID pStruct);
BOOL ConvertProgramParamsXmlToStru(const char* pXmlBuf, LPVOID pStruct);
BOOL ConvertProgramParamsStruToXml(LPVOID pStruct, char* *pXmlBuf, DWORD &dwXmlLen);
BOOL ConvertProgramParamsXmlToStruList(const char* pXmlBuf, LPVOID pStruct);
BOOL ConvertProgramSearchDescriptionParamsStruToXml(LPVOID pStruct, char* *pXmlBuf, DWORD &dwXmlLen);
BOOL ConvertProgramSearchResultParamsXmlToStru(const char* pXmlBuf, LPVOID pStruct);

//��Ƭ
BOOL ConvertContainerParamsXmlToStru(CXmlBase& struXml, LPVOID pStruct);
BOOL ConvertContainerParamsXmlToStru(const char* pXmlBuf, LPVOID pStruct);
BOOL ConvertContainerParamsStruToXml(LPVOID pStruct, char* *pXmlBuf, DWORD &dwXmlLen);
BOOL ConvertContainerParamsXmlToStruList(const char* pXmlBuf, LPVOID pStruct);

//�ճ�
BOOL ConvertTimeNodeData(CXmlBase &struXml, LPVOID pStruct, const char* pNodeName, BOOL bXmlToStru);
BOOL ConvertScheduleParamsXmlToStru(CXmlBase& struXml, LPVOID pStruct);
BOOL ConvertScheduleParamsXmlToStru(const char* pXmlBuf, LPVOID pStruct);
BOOL ConvertScheduleParamsStruToXml(LPVOID pStruct, char* *pXmlBuf, DWORD &dwXmlLen);
BOOL ConvertScheduleReleaseParamsStruToXml(LPVOID pStruct, char* *pXmlBuf, DWORD &dwXmlLen);
BOOL ConvertScheduleSearchDescriptionParamsStruToXml(LPVOID pStruct, char* *pXmlBuf, DWORD &dwXmlLen);
BOOL ConvertScheduleSearchResultParamsXmlToStru(const char* pXmlBuf, LPVOID pStruct);

//���ſ���
BOOL ConvertPlayControlParamsStruToXml(LPVOID pStruct, char* *pXmlBuf, DWORD &dwXmlLen);

//�ն���
BOOL ConvertTermGrpParamsXmlToStru(CXmlBase& struXml, LPVOID pStruct);
BOOL ConvertTermGrpParamsXmlToStru(const char* pXmlBuf, LPVOID pStruct);
BOOL ConvertTermGrpParamsStruToXml(LPVOID pStruct, char* *pXmlBuf, DWORD &dwXmlLen);
BOOL ConvertTermGrpParamsXmlToStruList(const char* pXmlBuf, LPVOID pStruct);

//��������
BOOL ConvertPlayParamsXmlToStru(CXmlBase& struXml, LPVOID pStruct);
BOOL ConvertPlayParamsXmlToStru(const char* pXmlBuf, LPVOID pStruct);
BOOL ConvertPlayParamsStruToXml(LPVOID pStruct, char* *pXmlBuf, DWORD &dwXmlLen);
BOOL ConvertPlayParamsXmlToStruList(const char* pXmlBuf, LPVOID pStruct);

//���ػ��ƻ�
BOOL ConvertSwitchPlanXmlToStru(CXmlBase& struXml, LPVOID pStruct);
BOOL ConvertSwitchPlanXmlToStru(const char* pXmlBuf, LPVOID pStruct);
BOOL ConvertSwitchPlanStruToXml(LPVOID pStruct, char* *pXmlBuf, DWORD &dwXmlLen);
BOOL ConvertSwitchPlanXmlToStruList(const char* pXmlBuf, LPVOID pStruct);

 
BOOL ConvertClockParam(NET_DVR_CLOCK &struLock,CXmlBase &struXml);			
BOOL ConvertWeatherParam(NET_DVR_WEATHER &struWeather,CXmlBase &struXml);
BOOL AddClockNode(NET_DVR_DISP_PARAM &struDisp,CXmlBase &struXml);

BOOL ConvertPlanParamsXmlToStru(CXmlBase& struXml, LPVOID pStruct);
BOOL ConvertPlanParamsXmlToStru(const char* pXmlBuf, LPVOID pStruct);
BOOL ConvertSwitchPlanListParamsXmlToStruList(const char *pXmlBuf,LPVOID pStruct);
BOOL ConvertPlanParamStruToXml(LPVOID pStruct, char* *pXmlBuf, DWORD &dwXmlLen);
BOOL ConvertVolumePlanParamStruToXml(LPVOID pStruct, char* *pXmlBuf, DWORD &dwXmlLen);
BOOL ConvertVolumePlanParamsXmlToStru(CXmlBase& struXml, LPVOID pStruct);
BOOL ConvertVolumePlanParamsXmlToStru(const char* pXmlBuf, LPVOID pStruct);
BOOL ConvertVolumePlanListParamsXmlToStruList(const char *pXmlBuf,LPVOID pStruct);

BOOL ConvertTermServerParamsStruToXml(LPVOID pStruct, char* *pXmlBuf, DWORD &dwXmlLen);
BOOL ConvertTerServerParamsXmlToStru(const char* pXmlBuf, LPVOID pStruct);

BOOL ConvertProgramIdToXml(LPVOID pStruct, int count,char* *pXmlBuf, DWORD &dwXmlLen);
BOOL ConvertMaterialIdToXml(LPVOID pStruct, int count, char* *pXmlBuf, DWORD &dwXmlLen);
BOOL ConvertMaterialBatchInfoToXml(LPVOID ArrayId, int CountId, LPVOID pStruct, char* *pXmlBuf, DWORD &dwXmlLen);
BOOL ConvertScheduleIdToXml(LPVOID pStruct, int count, char* *pXmlBuf, DWORD &dwXmlLen);
BOOL ConvertTerminalIdToXml(LPVOID pStruct, int Count, char* *pXmlBuf, DWORD &dwXmlLen);
BOOL ConvertProgramBatchInfoToXml(LPVOID ArrayId, int CountId, LPVOID pStruct, char* *pXmlBuf, DWORD &dwXmlLen);

#endif //_INFO_DIFFUSION_PARAMS_CONVERT_H_