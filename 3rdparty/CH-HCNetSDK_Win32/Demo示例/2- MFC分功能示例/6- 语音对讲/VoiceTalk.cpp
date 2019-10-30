/*******************************************************
Copyright 2008-2011 Hikvision Digital Technology Co., Ltd. 
��  ����	TalkMR.cpp
������λ��	���ݺ�������
��  д��	shizhiping
��  �ڣ�	2009.5
��  ����	�����Խ�
��  �ģ�	
********************************************************/

#include "stdafx.h"
#include "Voice.h"
#include "VoiceTalk.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/*************************************************
������:    	fVoiceTalkDataCallBack
��������:	�ص��������ɼ�PCM���ݣ�����wavͷ
�������:   lVoiceComHandle-����ת�����
            pRecvDataBuffer-���ݻ�����
            dwBufSize-��������С
            byAudioFlag-������������0-���زɼ���������1-���յ�������
			pUser-�û�����
�������:   		
����ֵ:	   
*************************************************/
void CALLBACK fVoiceTalkDataCallBack(LONG lVoiceComHandle, char *pRecvDataBuffer, DWORD dwBufSize, BYTE byAudioFlag, void *pUser)
{
    ASSERT(pRecvDataBuffer);
	CVoiceTalk *pVoiceTalkDlg = (CVoiceTalk*)pUser;
	if(!pVoiceTalkDlg->m_bIsRecording)  //¼����δ����
		return;
    //0-�������ݣ�1-DVR������������
	if(byAudioFlag == 1)
	{
		::mmioWrite(pVoiceTalkDlg->m_hmmio,pRecvDataBuffer,dwBufSize);
	}
	
}



/////////////////////////////////////////////////////////////////////////////
// CVoiceTalk dialog


CVoiceTalk::CVoiceTalk(CWnd* pParent /*=NULL*/)
	: CDialog(CVoiceTalk::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVoiceTalk)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_lVoiceHandle = -1;
	m_bIsRecording = FALSE;
	m_iCurChanIndex = -1;
}


void CVoiceTalk::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVoiceTalk)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVoiceTalk, CDialog)
	//{{AFX_MSG_MAP(CVoiceTalk)
	ON_BN_CLICKED(IDC_BUTTON_TALK, OnButtonTalk)
	ON_BN_CLICKED(IDC_BUTTON_RECORD, OnButtonRecord)
	ON_BN_CLICKED(IDC_RADIO_AUDIO1, OnRadioAudio1)
	ON_BN_CLICKED(IDC_RADIO_AUDIO2, OnRadioAudio2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVoiceTalk message handlers

BOOL CVoiceTalk::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
    m_iCurChanIndex = 1;
	((CButton*)GetDlgItem(IDC_RADIO_AUDIO1))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_RADIO_DATA_PCM))->SetCheck(1);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*************************************************
������:    	OnButtonTalk
��������:	��ʼ�Խ�/ֹͣ�Խ� ��ť
�������:   
�������:   		
����ֵ:	   
*************************************************/
void CVoiceTalk::OnButtonTalk() 
{
	
	if(!g_pMainDlg->m_bIsLogin)
	{
		MessageBox("Login fisrt!\n");
		return;
	}

	if(m_lVoiceHandle <0)    //start voice talk
	{
		StartVoiceTalk();
	}
	else     //stop talk
	{
	    StopVoiceTalk();
	}
	
}

/*************************************************
������:    	OnButtonRecord
��������:	��ʼ¼��/ֹͣ¼�� ��ť
�������:   
�������:   		
����ֵ:	   
*************************************************/
void CVoiceTalk::OnButtonRecord() 
{
	if(-1 == m_lVoiceHandle)
	{
		MessageBox(_T("start talk first"));
		return;
	}

	if(!m_bIsRecording)   //start record
	{
		StartRecord();
	}
	else                  //stop record
	{
		StopRecord();
	}	
}

/*************************************************
������:    	CreateWaveFile
��������:	����wave��Ƶ�ļ�
�������:   sFileName-�ļ���
�������:   		
����ֵ:	   
*************************************************/
BOOL CVoiceTalk::CreateWaveFile(char *sFileName)
{
	m_struWaveFormat.cbSize =			 sizeof(m_struWaveFormat);
	m_struWaveFormat.nBlockAlign =	 CHANNEL * BITS_PER_SAMPLE / 8;
	m_struWaveFormat.nChannels =		 CHANNEL;
	m_struWaveFormat.nSamplesPerSec =	 SAMPLES_PER_SECOND;
	m_struWaveFormat.wBitsPerSample =	 BITS_PER_SAMPLE;
	m_struWaveFormat.nAvgBytesPerSec = SAMPLES_PER_SECOND * m_struWaveFormat.nBlockAlign;
	m_struWaveFormat.wFormatTag =		 WAVE_FORMAT_PCM;
	
	if(!(m_hmmio = mmioOpen(sFileName,NULL,MMIO_CREATE|MMIO_WRITE|MMIO_EXCLUSIVE)))
	{
		//printf("opem file error\n");
		return FALSE;
	}
	
	ZeroMemory(&m_mmckinfoParent, sizeof(MMCKINFO));
	m_mmckinfoParent.fccType = mmioFOURCC('W','A','V','E');
	MMRESULT mmResult = ::mmioCreateChunk( m_hmmio,&m_mmckinfoParent, MMIO_CREATERIFF);
	
	ZeroMemory(&m_mmckinfoChild,sizeof(MMCKINFO));
	m_mmckinfoChild.ckid = mmioFOURCC('f','m','t',' ');
	m_mmckinfoChild.cksize = sizeof(WAVEFORMATEX) + m_struWaveFormat.cbSize;
	mmResult = ::mmioCreateChunk(m_hmmio, &m_mmckinfoChild, 0);
	
	mmResult = ::mmioWrite(m_hmmio, (char*)&m_struWaveFormat, sizeof(WAVEFORMATEX) + m_struWaveFormat.cbSize); 
	mmResult = ::mmioAscend(m_hmmio, &m_mmckinfoChild, 0);
	m_mmckinfoChild.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmResult = ::mmioCreateChunk(m_hmmio, &m_mmckinfoChild, 0);
	return TRUE;
}

/*************************************************
������:    	StopVoiceTalk
��������:	ֹͣ�Խ�
�������:   
�������:   		
����ֵ:	   
*************************************************/
void CVoiceTalk::StopVoiceTalk()
{
	if(m_bIsRecording)
	{
		OnButtonRecord();   //stop record
	}
	NET_DVR_StopVoiceCom(m_lVoiceHandle);
	GetDlgItem(IDC_BUTTON_TALK)->SetWindowText("��ʼ�Խ�");
    m_lVoiceHandle = -1;
}

/*************************************************
������:    	StartVoiceTalk
��������:	��ʼ�Խ�
�������:   
�������:   		
����ֵ:	   
*************************************************/
void CVoiceTalk::StartVoiceTalk()
{
	int iCurChan = GetCurVoiceChanIndex();
	m_lVoiceHandle = NET_DVR_StartVoiceCom_V30(g_pMainDlg->m_DeviceInfo.lLoginID,m_iCurChanIndex,iCurChan,fVoiceTalkDataCallBack,(void*)this);
	if(-1 == m_lVoiceHandle)
	{
		MessageBox(_T("Voice talk failed!"));
		return;
	}
	
	GetDlgItem(IDC_BUTTON_TALK)->SetWindowText("ֹͣ�Խ�");
}

/*************************************************
������:    	StartRecord
��������:	��ʼ¼��
�������:   
�������:   		
����ֵ:	   
*************************************************/
void CVoiceTalk::StartRecord()
{
	char RecName[256] = {0};
	
	CTime CurTime = CTime::GetCurrentTime();;
	sprintf(RecName,"%04d%02d%02d%02d%02d%02d.wav",CurTime.GetYear(),CurTime.GetMonth(),CurTime.GetDay(), \
		CurTime.GetHour(),CurTime.GetMinute(),CurTime.GetSecond());
	
	if(!CreateWaveFile(RecName))
	{
		MessageBox("create record file failed");
		return;
	}
	m_bIsRecording = TRUE;
	GetDlgItem(IDC_BUTTON_RECORD)->SetWindowText("ֹͣ¼��");
}

/*************************************************
������:    	StopRecord
��������:	ֹͣ¼��
�������:   
�������:   		
����ֵ:	   
*************************************************/
void CVoiceTalk::StopRecord()
{
	if(m_hmmio)
	{
		::mmioAscend(m_hmmio,&m_mmckinfoChild,0);
		::mmioAscend(m_hmmio,&m_mmckinfoParent,0);
		::mmioClose(m_hmmio,0);
		m_hmmio = NULL;
		m_bIsRecording = FALSE;
		GetDlgItem(IDC_BUTTON_RECORD)->SetWindowText("��ʼ¼��");
	}
}

/*************************************************
������:    	GetCurVoiceChanIndex
��������:	��ȡ��ǰ������ͨ��
�������:   
�������:   		
����ֵ:	   ��ǰѡ�е�����ͨ��
*************************************************/
int CVoiceTalk::GetCurVoiceChanIndex()
{
    return m_iCurChanIndex;
}

/*************************************************
������:    	OnRadioAudio1
��������:	ѡ������ͨ��1
�������:   
�������:   		
����ֵ:	   
*************************************************/
void CVoiceTalk::OnRadioAudio1() 
{
	m_iCurChanIndex = 1;
}

/*************************************************
������:    	OnRadioAudio2
��������:	ѡ������ͨ��2
�������:   
�������:   		
����ֵ:	   
*************************************************/
void CVoiceTalk::OnRadioAudio2() 
{
    m_iCurChanIndex = 2;	
}

/*************************************************
������:    	SetChanRadioState
��������:	��������ͨ��״̬
�������:   iNum-ͨ����
            ChanStatus-״̬
�������:   		
����ֵ:	   
*************************************************/
void CVoiceTalk::SetChanRadioState(int iNum,BYTE *ChanStatus)
{
   switch (iNum)
   {
   case 1:
	   GetDlgItem(IDC_RADIO_AUDIO1)->EnableWindow(TRUE);
	   GetDlgItem(IDC_RADIO_AUDIO2)->EnableWindow(FALSE);
	   switch(ChanStatus[0])   //8000�豸��֧������ͨ��״̬
	   {
	   case 0:      //not used
		   break;
	   case 1:      // using
		   break;
	   case 0xff:   //
		   break;
	   default:
		   break;
	   }
		break;
   case 2:
	   GetDlgItem(IDC_RADIO_AUDIO1)->EnableWindow(TRUE);
	   GetDlgItem(IDC_RADIO_AUDIO2)->EnableWindow(TRUE);
	   break;
   default:
	   break;
   }
}
