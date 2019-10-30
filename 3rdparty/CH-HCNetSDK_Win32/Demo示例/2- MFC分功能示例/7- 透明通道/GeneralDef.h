
#ifndef GENERALDEF_H
#define GENERALDEF_H

#include "HCNetSDK.h"
#include "plaympeg4.h"


#define DATABUF_LEN  1024     //�������ݻ�����󳤶�

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

	LONG    lLoginID;
	int		iDeviceChanNum;		    //�豸��ͨ����
	int		iStartChan;				//�豸��ʼͨ����
	int 	iIPChanNum;				//�������ͨ������
	int		iEnableChanNum;			//��Чͨ����
	BOOL    bIPRet;                 //�Ƿ�֧��ip����
	
	CHANNEL_INFO struChanInfo[MAX_CHANNUM_V30];


	STRU_DEVICE_INFO()
	{
		 lLoginID = -1;
		 iDeviceChanNum      =  -1;
		 iStartChan          = 0;
		 iIPChanNum          = 0;
		 iEnableChanNum      = -1;
		 bIPRet              = FALSE;
	}	 
}LOCAL_DEVICE_INFO,*pLOCAL_DEVICE_INFO;




#endif