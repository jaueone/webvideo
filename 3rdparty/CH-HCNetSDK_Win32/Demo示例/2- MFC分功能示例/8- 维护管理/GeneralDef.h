
#ifndef GENERALDEF_H
#define GENERALDEF_H

#include "HCNetSDK.h"
#include "plaympeg4.h"

#define MAX_CHANS      24     //�豸֧�ֵ����ͨ����
#define MAX_DEVICE     10     //����豸��
#define MAX_OUTPUTS	   16	//16���Ĵ�����
#define MAX_PREVIEWS   512	//��Ԥ���������

//�豸��������
#define TREE_ALL_T	 0        //������ڵ�
#define DEVICETYPE 	 1		  //�����豸
#define CHANNELTYPE	 2	      //����ͨ��


#define DEMO_FULL_WIDTH 1024	//demoĬ�ϵĿ��
#define DEMO_FULL_HEIGHT 768	//demoĬ�ϵĸ߶�
#define OUTPUT_INTERVAL 4		//ÿ������ļ����С


//��ʱ��ʱ��
#define UPGRADE_TIMER			6		//����ʱ��
#define FORMAT_TIMER			10		//Զ�̸�ʽ��Ӳ��

typedef struct STRU_CHANNEL_INFO
{
	char    chChanName[40];     //ͨ������
	int		iChanIndex;			//���ͨ���� = ��������+startchan
	int		iPicResolution;				//ͼƬ�ֱ���
	int		iPicQuality;				//ͼƬ����
	char	chAccessChanIP[16];     //ip����ͨ����ip��ַ
	BOOL    bEnable;              //�Ƿ���Ч

	STRU_CHANNEL_INFO()
	{
		chChanName[0]       ='\0';
		iChanIndex          = -1;
		iPicResolution		= 0;
		iPicQuality			= 2;
		chAccessChanIP[0]	= '\0';	
		bEnable             = FALSE;
	}
}CHANNEL_INFO,*pCHANNEL_INFO;

typedef struct STRU_DEVICE_INFO
{

	char    chDevName[100];        //�豸����
	char    sSerialNumber[100];     //�豸���к�
	char    chDevIP[40];            //�豸IP
	LONG    lLoginID;
	int		iDeviceChanNum;		    //�豸��ͨ����
	int		iStartChan;				//�豸��ʼͨ����
	int 	iIPChanNum;				//�������ͨ������
	int		iEnableChanNum;			//��Чͨ����
	BOOL    bIPRet;                 //�Ƿ�֧��ip����
	BYTE    byDiskNum;				    //Ӳ�̸���
    BYTE    byDVRType;				    //�豸����, 1:DVR 2:ATM DVR 3:DVS ......
	
	CHANNEL_INFO struChanInfo[MAX_CHANNUM_V30];


	STRU_DEVICE_INFO()
	{
         chDevName[0]        = '\0';
		 sSerialNumber[0]    = '\0';
		 chDevIP[0]          = '\0';
		 lLoginID = -1;
		 iDeviceChanNum      =  -1;
		 iStartChan          = 0;
		 iIPChanNum          = 0;
		 iEnableChanNum      = -1;
		 bIPRet              = FALSE;
		 byDiskNum           = 0;
         byDVRType           = 0;
	}	 
}LOCAL_DEVICE_INFO,*pLOCAL_DEVICE_INFO;




#endif