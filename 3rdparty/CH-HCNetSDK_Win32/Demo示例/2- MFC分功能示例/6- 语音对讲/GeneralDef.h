
#ifndef GENERALDEF_H
#define GENERALDEF_H

#include "HCNetSDK.h"
#include "plaympeg4.h"

//�豸��������
#define TREE_ALL_T	 0        //������ڵ�
#define DEVICETYPE 	 1		  //�����豸
#define CHANNELTYPE	 2	      //����ͨ��

#define MAX_AUDIO_V30 2        //�������ͨ����


//Wav
#define BITS_PER_SAMPLE		16
#define CHANNEL				1
#define SAMPLES_PER_SECOND	16000

#define BIT_RATE_16000		16000
#define AUDENCSIZE          1280
#define AUDDECSIZE          80
#define BUFFER_SIZE         1280
#define BUFFER_NUM           6
#define AUDIOBUF			(80*40L)



typedef struct STRU_DEVICE_INFO
{
	LONG	lLoginID;				//ע��ID
	char	chLocalNodeName[100];	//�����豸�ڵ�����
	char	chDeviceName[100];		//�豸����
	char	chDeviceIP[40];			//����ֱ�����ӵ��豸IP
	UINT	lDevicePort;			//�豸���˿ں�
	char	chLoginUserName[40];	//��½�豸���û��������û������ʱ���õ�
	char	chLoginPwd[40];	         //��½�豸������
	//char	chDeviceMultiIP[20];	//�ಥ���ַ
	char	chSerialNumber[60];		//���к�
	int		iDeviceType;			//�豸����
	BOOL    bIsLogin;               //�Ƿ���ע����豸
	int     iVoiceChanNum;             //����ͨ����
	BYTE    AudioChanStatus[MAX_AUDIO_V30];   //����ͨ��״̬     

	STRU_DEVICE_INFO()
	{
         lLoginID            = -1;
		 chLocalNodeName[0]  = '\0';
		 chDeviceName[0]     = '\0';
		 chDeviceIP[0]       = '\0';
         lDevicePort         = 8000;
         chLoginUserName[0]  = '\0';
         chLoginPwd[0]       ='\0';
         //chDeviceMultiIP[0]   ='\0';
		 chSerialNumber[0]   = '\0';
		 iDeviceType         = 0;
		 bIsLogin            = FALSE;
	}	 
}LOCAL_DEVICE_INFO,*pLOCAL_DEVICE_INFO;


typedef struct STRU_VOICEMR_INFO{
	int iDeviceIndex;   //�豸��
	int iChanIndex;     //����ͨ���ţ�1��2
	LONG hVoiceHandle;  //�������

    STRU_VOICEMR_INFO()
	{
        iDeviceIndex = -1;
        iChanIndex = -1;
		hVoiceHandle = -1;
	}
}VOICEMR_INFO,*PVOICEMR_INFO;

#endif