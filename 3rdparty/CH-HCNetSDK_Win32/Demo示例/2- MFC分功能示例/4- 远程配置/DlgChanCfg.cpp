// DlgChanCfg.cpp : implementation file
//
/**********************************************************
FileName:    DlgChanCfg.cpp
Description: ͨ����Ϣ����      
Date:        
Note: 		<ȫ��>�ṹ��,���GeneralDef.h, ȫ�ֱ���,������config.cpp   
Modification History:      
<version>   <time>         <desc>

***********************************************************/

#include "stdafx.h"
#include "config.h"
#include "DlgChanCfg.h"
#include "DlgRemoteShowString.h"
#include "DlgRemoteHideArea.h"
#include "DlgRemoteHideAlarm.h"
#include "DlgRemoteRecordSchedule.h"
#include "DlgRemoteMotionDetect.h"
#include "DlgRemoteVideoInLost.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern LPNET_DVR_HIDEALARM_V30 g_lpHideAlarm;//�澯�ڵ�����

/////////////////////////////////////////////////////////////////////////////
// CDlgChanCfg dialog


CDlgChanCfg::CDlgChanCfg(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgChanCfg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgChanCfg)
	m_iFrame_I_Iterval = 0;
	m_bCheckRecord = FALSE;
	m_dwDurationTime = 0;
	m_bCheckOsd = FALSE;
	m_iOsdX = 0;
	m_iOsdY = 0;
	m_bChkDispWeek = FALSE;
	m_bCheckChanName = FALSE;
	m_iChanNameX = 0;
	m_iChanNameY = 0;
	m_bCheckHideArea = FALSE;
	m_bCheckMotion = FALSE;
	m_bCheckHideAlarm = FALSE;
	m_bCheckVILost = FALSE;
	m_iBitRate = 0;
	m_csChannelName = _T("");
	//}}AFX_DATA_INIT
}


void CDlgChanCfg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgChanCfg)
	DDX_Control(pDX, IDC_COMBO_COPY_CHAN, m_comboCopyChan);
	DDX_Control(pDX, IDC_COMBO_HOUR_TYPE, m_comboHourType);
	DDX_Control(pDX, IDC_COMBO_OSD_TYPE, m_comboOsdType);
	DDX_Control(pDX, IDC_COMBO_OSD_ATTRIB, m_comboOsdAttrib);
	DDX_Control(pDX, IDC_COMBO_REC_DELAY, m_comboRecordDelay);
	DDX_Control(pDX, IDC_COMBO_PRERECORD_TIME, m_comboPrerecordTime);
	DDX_Control(pDX, IDC_COMBO_COMPRESSION_TYPE, m_comboCompressionType);
	DDX_Control(pDX, IDC_COMBO_CHAN, m_comboChanNum);
	DDX_Control(pDX, IDC_COMBO_FRAME_BP_INTERVAL, m_comboFrameBPInterval);
	DDX_Control(pDX, IDC_COMBO_MAX_BIT_RATE, m_comboMaxBitRate);
	DDX_Control(pDX, IDC_COMBO_FRAME_RATE, m_comboFrameRate);
	DDX_Control(pDX, IDC_COMBO_RESOLUTION, m_comboResolution);
	DDX_Control(pDX, IDC_COMBO_BIT_RATE_TYPE, m_comboBitRateType);
	DDX_Control(pDX, IDC_COMBO_STREAM_TYPE, m_comboStreamType);
	DDX_Control(pDX, IDC_COMBO_IMAGE_QUALITY, m_comboImageQuality);
	DDX_Text(pDX, IDC_EDIT_FRAME_I_INTERVAL, m_iFrame_I_Iterval);
	DDX_Check(pDX, IDC_CHK_RECORD, m_bCheckRecord);
	DDX_Text(pDX, IDC_EDIT_DURATION_TIME, m_dwDurationTime);
	DDX_Check(pDX, IDC_CHK_OSD, m_bCheckOsd);
	DDX_Text(pDX, IDC_EDIT_OSD_X, m_iOsdX);
	DDX_Text(pDX, IDC_EDIT_OSD_Y, m_iOsdY);
	DDX_Check(pDX, IDC_CHK_DISPLAY_WEEK, m_bChkDispWeek);
	DDX_Check(pDX, IDC_CHK_CHAN_NAME, m_bCheckChanName);
	DDX_Text(pDX, IDC_EDIT_CHAN_NAME_X, m_iChanNameX);
	DDX_Text(pDX, IDC_EDIT_CHAN_NAME_Y, m_iChanNameY);
	DDX_Check(pDX, IDC_CHK_HIDE_AREA, m_bCheckHideArea);
	DDX_Check(pDX, IDC_CHK_MOTION, m_bCheckMotion);
	DDX_Check(pDX, IDC_CHK_HIDE_ALARM, m_bCheckHideAlarm);
	DDX_Check(pDX, IDC_CHK_VILOST, m_bCheckVILost);
	DDX_Text(pDX, IDC_EDIT_BIT_RATE, m_iBitRate);
	DDX_Text(pDX, IDC_EDIT_CHANNEL_NAME, m_csChannelName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgChanCfg, CDialog)
	//{{AFX_MSG_MAP(CDlgChanCfg)
	ON_BN_CLICKED(IDC_CHK_HIDE_AREA, OnBnClickedChkHideArea)
	ON_BN_CLICKED(IDC_CHK_RECORD, OnBnClickedChkRecord)
	ON_BN_CLICKED(IDC_CHK_MOTION, OnBnClickedChkMotion)
	ON_BN_CLICKED(IDC_CHK_VILOST, OnBnClickedChkVilost)
	ON_BN_CLICKED(IDC_CHK_HIDE_ALARM, OnBnClickedChkHideAlarm)
	ON_BN_CLICKED(IDC_CHK_OSD, OnBnClickedChkOsd)
	ON_BN_CLICKED(IDC_CHK_CHAN_NAME, OnBnClickedChkChanNamePos)
	ON_BN_CLICKED(IDC_BTN_CMP_OK, OnButtonSetCompressionCfg)
	ON_BN_CLICKED(IDC_BTN_RECORD_OK, OnButtonSetRecordCfg)
	ON_BN_CLICKED(IDC_BTN_PIC_OK, OnButtonSetPictureCfg)
	ON_BN_CLICKED(IDC_BTN_STRING, OnBnClickedBtnString)
	ON_BN_CLICKED(IDC_BTN_HIDE_SETUP, OnBnClickedBtnHideSetup)
	ON_BN_CLICKED(IDC_BTN_HIDE_ALARM_SETUP, OnBnClickedBtnHideAlarmSetup)
	ON_BN_CLICKED(IDC_BTN_REMOTE_RECORD_SETUP, OnBnClickedBtnRemoteRecordSetup)
	ON_BN_CLICKED(IDC_BTN_MOTION_SETUP, OnBnClickedBtnMotionSetup)
	ON_BN_CLICKED(IDC_BTN_VILOST_SETUP, OnBnClickedBtnVilostSetup)
	ON_CBN_SELCHANGE(IDC_COMBO_CHAN, OnCbnSelchangeComboChan)
	ON_CBN_SELCHANGE(IDC_COMBO_MAX_BIT_RATE, OnCbnSelchangeComboMaxBitRate)
	ON_BN_CLICKED(IDC_BTN_ChanCfg_OK, OnBnClickedChanCfgOK)
	ON_BN_CLICKED(IDC_BTN_CHAN_COPY, OnBnClickedChanCfgCopy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgChanCfg message handlers
/*********************************************************
  Function:	CurCfgUpdate
  Desc:		��ȡ����ͨ����ѹ����¼��ͼ���������
  Input:	
  Output:	
  Return:	
**********************************************************/
void CDlgChanCfg::CurCfgUpdate()
{
	
	if (g_lUserID < 0)
	{
		return;
	}
	
	int i;
	DWORD dwReturned = 0;
	int iChanShow = 0;
	int iCopyIndex = 0;
	m_lStartChannel = g_struDeviceInfo[0].iStartChan;//�豸����ʼͨ����һ���1��ʼ��
	int m_iChanCount = g_struDeviceInfo[0].iDeviceChanNum;//�豸��ģ��ͨ������

	m_comboChanNum.ResetContent();//��½��ͬ�豸ʱ��ɾ��combobox�������һ�ֵļ�¼�����ɻ�ȱ��
	m_comboCopyChan.ResetContent();//��½��ͬ�豸ʱ��ɾ��combobox�������һ�ֵļ�¼��

	m_comboCopyChan.AddString("ȫ��ͨ��");
	m_comboCopyChan.SetItemData(iCopyIndex, -1);


	for (i = 0; i < MAX_CHANNUM_V30; i++)
	{
		iChanShow = i+g_struDeviceInfo[0].iStartChan;
		CString sTemp;
		CString sTempIP;
		
		sTemp.Format("%s%d", "ͨ��", iChanShow);
		sTempIP.Format("%s%d", "IPͨ��", iChanShow-MAX_ANALOG_CHANNUM);//ipͨ����Ҫ��ȥ32���ܴ�1��ʼ��
    
    
		
		//ͨ���Ƿ�ʹ��
		if (g_struDeviceInfo[0].struChanInfo[i].bEnable)
		{
			//ģ��ͨ��
			if (i<m_iChanCount)
			{
				m_comboChanNum.AddString(sTemp);
				m_comboChanNum.SetItemData(iCopyIndex, i);
				
				iCopyIndex++;
				m_comboCopyChan.AddString(sTemp);
				m_comboCopyChan.SetItemData(iCopyIndex, i);

				memset(&m_struCompressionCfg[i], 0, sizeof(m_struCompressionCfg[i]));
				if (!NET_DVR_GetDVRConfig(g_lUserID, NET_DVR_GET_COMPRESSCFG_V30, iChanShow, &m_struCompressionCfg[i], sizeof(NET_DVR_COMPRESSIONCFG_V30), &dwReturned))
				{
					AfxMessageBox("���ѹ������ʧ�ܣ�");	
				}
				memset(&m_struRecordCfg[i], 0, sizeof(m_struRecordCfg[i]));
				if (!NET_DVR_GetDVRConfig(g_lUserID, NET_DVR_GET_RECORDCFG_V30, iChanShow,&m_struRecordCfg[i], sizeof(NET_DVR_RECORD_V30), &dwReturned))
				{
					MessageBox("���¼�����ʧ��","warning");
				}
				memset(&m_struPicCfg[i], 0, sizeof(m_struPicCfg[i]));
				if (!NET_DVR_GetDVRConfig(g_lUserID, NET_DVR_GET_PICCFG_V30, iChanShow,&m_struPicCfg[i], sizeof(NET_DVR_PICCFG_V30), &dwReturned))
				{
					MessageBox("���ͼ�����ʧ��","warning");
				}
				
				
				//ֱ�ӽ���һ��ʹ��ͨ����һ����ͨ��1������������ʾ�������ٽ���forѭ���������ע�ᵽ��ʾ������о���һ��ʱ�����ʱ��
				if(i==g_struDeviceInfo[0].lFirstEnableChanIndex)//�ӵ�һ��ʹ��ͨ����ʼ����ʾ��
				{
					//**ѹ�����ֵĲ���**//
					m_comboImageQuality.SetCurSel(m_struCompressionCfg[i].struNormHighRecordPara.byPicQuality);
					m_comboResolution.SetCurSel(m_struCompressionCfg[i].struNormHighRecordPara.byResolution);
					m_comboStreamType.SetCurSel(m_struCompressionCfg[i].struNormHighRecordPara.byStreamType);
					m_comboBitRateType.SetCurSel(m_struCompressionCfg[i].struNormHighRecordPara.byBitrateType);
					m_comboFrameRate.SetCurSel(m_struCompressionCfg[i].struNormHighRecordPara.dwVideoFrameRate);
					if ((m_struCompressionCfg[i].struNormHighRecordPara.dwVideoBitrate >> 31) && 0x01)
					{
						GetDlgItem(IDC_EDIT_BIT_RATE)->EnableWindow(TRUE);
						m_comboMaxBitRate.SetCurSel(22);		//�Զ�������
						m_iBitRate = (m_struCompressionCfg[i].struNormHighRecordPara.dwVideoBitrate & 0x7fffffff)/1024;
					}
					else
					{
						m_comboMaxBitRate.SetCurSel(m_struCompressionCfg[i].struNormHighRecordPara.dwVideoBitrate-2);			
						m_iBitRate = 0;
						GetDlgItem(IDC_EDIT_BIT_RATE)->EnableWindow(FALSE);
					}
					m_comboFrameBPInterval.SetCurSel(m_struCompressionCfg[i].struNormHighRecordPara.byIntervalBPFrame);
					m_comboChanNum.SetCurSel(0);//����ǰ���м���ͨ���������������������Ǵ�û������ͨ����ʼ���뵽combobox�ģ����Կ϶��Ǵ�0��ʼ��Ҳ���Ǵӵ�һ��û��������ͨ����ʼ��
					m_comboCopyChan.SetCurSel(0);
					m_comboCompressionType.SetCurSel(0);
					GetDlgItem(IDC_EDIT_FRAME_I_INTERVAL)->EnableWindow(TRUE);
					m_iFrame_I_Iterval=m_struCompressionCfg[i].struNormHighRecordPara.wIntervalFrameI;
					
					//**¼�񲿷ֵĲ���**//
					m_bCheckRecord = m_struRecordCfg[i].dwRecord;
					m_comboPrerecordTime.SetCurSel(m_struRecordCfg[i].dwPreRecordTime);
					m_comboRecordDelay.SetCurSel(m_struRecordCfg[i].dwRecordTime);
					
					//���漸�����ж϶�ʱ¼���Ƿ�򿪣��Ƿ���Խ���Զ��¼��ƻ������á�
					if (m_bCheckRecord)
					{
						GetDlgItem(IDC_BTN_REMOTE_RECORD_SETUP)->EnableWindow(TRUE);
						GetDlgItem(IDC_COMBO_PRERECORD_TIME)->EnableWindow(TRUE);
						GetDlgItem(IDC_COMBO_REC_DELAY)->EnableWindow(TRUE);
					}
					else
					{
						GetDlgItem(IDC_BTN_REMOTE_RECORD_SETUP)->EnableWindow(FALSE);
						GetDlgItem(IDC_COMBO_PRERECORD_TIME)->EnableWindow(FALSE);
						GetDlgItem(IDC_COMBO_REC_DELAY)->EnableWindow(FALSE);
					}
					
					//**ͼ�񲿷ֵĲ���**//
					m_csChannelName=m_struPicCfg[i].sChanName;
					

					m_bCheckOsd=m_struPicCfg[i].dwShowOsd;
					GetDlgItem(IDC_EDIT_OSD_X)->EnableWindow(m_bCheckOsd);
					GetDlgItem(IDC_EDIT_OSD_Y)->EnableWindow(m_bCheckOsd);
					GetDlgItem(IDC_COMBO_OSD_ATTRIB)->EnableWindow(m_bCheckOsd);
					GetDlgItem(IDC_COMBO_OSD_TYPE)->EnableWindow(m_bCheckOsd);
					GetDlgItem(IDC_CHK_DISPLAY_WEEK)->EnableWindow(m_bCheckOsd);
					m_comboOsdType.SetCurSel(m_struPicCfg[i].byOSDType);
					m_comboOsdAttrib.SetCurSel(m_struPicCfg[i].byOSDAttrib-1);//����֮����Ҫ��1������Ϊ������0��Ӧ��OSD���ԣ�0������ʾOSD��1��͸��,��˸�ȣ�����ʾOSD��������ȡ������check�ؼ��ˡ���������һ���±�Ϊ0����Ŀ��
					m_bChkDispWeek=m_struPicCfg[i].byDispWeek;
					m_iOsdX=m_struPicCfg[i].wOSDTopLeftX;
					m_iOsdY=m_struPicCfg[i].wOSDTopLeftY;
					m_comboHourType.SetCurSel(m_struPicCfg[i].byHourOSDType);
					
					m_bCheckChanName=m_struPicCfg[i].dwShowChanName;
					GetDlgItem(IDC_EDIT_CHAN_NAME_X)->EnableWindow(m_bCheckChanName);
					GetDlgItem(IDC_EDIT_CHAN_NAME_Y)->EnableWindow(m_bCheckChanName);
					m_iChanNameX=m_struPicCfg[i].wShowNameTopLeftX;
					m_iChanNameY=m_struPicCfg[i].wShowNameTopLeftY;
					
					m_bCheckHideArea=m_struPicCfg[i].dwEnableHide;//�ж��Ƿ���ڸǹ��ܣ�ʹ���ڸǡ���ťʹ�ܡ�
					GetDlgItem(IDC_BTN_HIDE_SETUP)->EnableWindow(m_bCheckHideArea);
					
					m_bCheckMotion = m_struPicCfg[i].struMotion.byEnableHandleMotion;
					GetDlgItem(IDC_BTN_MOTION_SETUP)->EnableWindow(m_bCheckMotion);
					
					m_bCheckVILost=m_struPicCfg[i].struVILost.byEnableHandleVILost;
					GetDlgItem(IDC_BTN_VILOST_SETUP)->EnableWindow(m_bCheckVILost);
					
					if (m_struPicCfg[i].struHideAlarm.dwEnableHideAlarm > 0)//����Ҫ���Ƿ��ڸǱ����Ĳ��������жϣ���Ϊ����ֵ����ȡ0��1��2��3���������1����ѡ���޷����Ϲ���
					{
						m_bCheckHideAlarm = TRUE;
					}
					else
					{
						m_bCheckHideAlarm = FALSE;
					}
					//m_bCheckHideAlarm=m_struPicCfg[i].struHideAlarm.dwEnableHideAlarm;�������������жϣ������д�Ļ�������ָ�ѡ��û�򹳵������
	                GetDlgItem(IDC_BTN_HIDE_ALARM_SETUP)->EnableWindow(m_bCheckHideAlarm);

					
					UpdateData(FALSE);
				}
			}
			//����9000�豸��ipͨ��
			else if (i >= MAX_ANALOG_CHANNUM)
			{
				m_comboChanNum.AddString(sTempIP);
				m_comboChanNum.SetItemData(iCopyIndex, i);
				
				iCopyIndex++;
				m_comboCopyChan.AddString(sTempIP);
				m_comboCopyChan.SetItemData(iCopyIndex, i);

				memset(&m_struCompressionCfg[i], 0, sizeof(m_struCompressionCfg[i]));
				if (!NET_DVR_GetDVRConfig(g_lUserID, NET_DVR_GET_COMPRESSCFG_V30, iChanShow, &m_struCompressionCfg[i], sizeof(NET_DVR_COMPRESSIONCFG_V30), &dwReturned))
				{
					AfxMessageBox("���ѹ������ʧ�ܣ�");	
				}
				memset(&m_struRecordCfg[i], 0, sizeof(m_struRecordCfg[i]));
				if (!NET_DVR_GetDVRConfig(g_lUserID, NET_DVR_GET_RECORDCFG_V30, iChanShow,&m_struRecordCfg[i], sizeof(NET_DVR_RECORD_V30), &dwReturned))
				{
					MessageBox("���¼�����ʧ��","warning");
				}
				memset(&m_struPicCfg[i], 0, sizeof(m_struPicCfg[i]));
				if (!NET_DVR_GetDVRConfig(g_lUserID, NET_DVR_GET_PICCFG_V30, iChanShow,&m_struPicCfg[i], sizeof(NET_DVR_PICCFG_V30), &dwReturned))
				{
					MessageBox("���ͼ�����ʧ��","warning");
				}
				
				
				//ֱ�ӽ���һ��ʹ��ͨ����һ����ͨ��1������������ʾ�������ٽ���forѭ���������ע�ᵽ��ʾ������о���һ��ʱ�����ʱ��
				if(i==g_struDeviceInfo[0].lFirstEnableChanIndex)//�ӵ�һ��ʹ��ͨ����ʼ����ʾ��
				{
					//**ѹ�����ֵĲ���**//
					m_comboImageQuality.SetCurSel(m_struCompressionCfg[i].struNormHighRecordPara.byPicQuality);
					m_comboResolution.SetCurSel(m_struCompressionCfg[i].struNormHighRecordPara.byResolution);
					m_comboStreamType.SetCurSel(m_struCompressionCfg[i].struNormHighRecordPara.byStreamType);
					m_comboBitRateType.SetCurSel(m_struCompressionCfg[i].struNormHighRecordPara.byBitrateType);
					m_comboFrameRate.SetCurSel(m_struCompressionCfg[i].struNormHighRecordPara.dwVideoFrameRate);
					if ((m_struCompressionCfg[i].struNormHighRecordPara.dwVideoBitrate >> 31) && 0x01)
					{
						GetDlgItem(IDC_EDIT_BIT_RATE)->EnableWindow(TRUE);
						m_comboMaxBitRate.SetCurSel(22);		//�Զ�������
						m_iBitRate = (m_struCompressionCfg[i].struNormHighRecordPara.dwVideoBitrate & 0x7fffffff)/1024;
					}
					else
					{
						m_comboMaxBitRate.SetCurSel(m_struCompressionCfg[i].struNormHighRecordPara.dwVideoBitrate-2);			
						m_iBitRate = 0;
						GetDlgItem(IDC_EDIT_BIT_RATE)->EnableWindow(FALSE);
					}
					m_comboFrameBPInterval.SetCurSel(m_struCompressionCfg[i].struNormHighRecordPara.byIntervalBPFrame);
					m_comboChanNum.SetCurSel(0);//����ǰ���м���ͨ���������������������Ǵ�û������ͨ����ʼ���뵽combobox�ģ����Կ϶��Ǵ�0��ʼ��Ҳ���Ǵӵ�һ��û��������ͨ����ʼ��
					m_comboCopyChan.SetCurSel(0);
					m_comboCompressionType.SetCurSel(0);
					GetDlgItem(IDC_EDIT_FRAME_I_INTERVAL)->EnableWindow(TRUE);
					m_iFrame_I_Iterval=m_struCompressionCfg[i].struNormHighRecordPara.wIntervalFrameI;
					
					//**¼�񲿷ֵĲ���**//
					m_bCheckRecord = m_struRecordCfg[i].dwRecord;
					m_comboPrerecordTime.SetCurSel(m_struRecordCfg[i].dwPreRecordTime);
					m_comboRecordDelay.SetCurSel(m_struRecordCfg[i].dwRecordTime);
					
					//���漸�����ж϶�ʱ¼���Ƿ�򿪣��Ƿ���Խ���Զ��¼��ƻ������á�
					if (m_bCheckRecord)
					{
						GetDlgItem(IDC_BTN_REMOTE_RECORD_SETUP)->EnableWindow(TRUE);
						GetDlgItem(IDC_COMBO_PRERECORD_TIME)->EnableWindow(TRUE);
						GetDlgItem(IDC_COMBO_REC_DELAY)->EnableWindow(TRUE);
					}
					else
					{
						GetDlgItem(IDC_BTN_REMOTE_RECORD_SETUP)->EnableWindow(FALSE);
						GetDlgItem(IDC_COMBO_PRERECORD_TIME)->EnableWindow(FALSE);
						GetDlgItem(IDC_COMBO_REC_DELAY)->EnableWindow(FALSE);
					}
					
					//**ͼ�񲿷ֵĲ���**//
					GetDlgItem(IDC_BTN_STRING)->EnableWindow(FALSE);//ͨ��������32��NET_DVR_SHOWSTRING_V30�Ļ������GET���������
					m_csChannelName=m_struPicCfg[i].sChanName;
					m_bCheckOsd=m_struPicCfg[i].dwShowOsd;
					GetDlgItem(IDC_EDIT_OSD_X)->EnableWindow(m_bCheckOsd);
					GetDlgItem(IDC_EDIT_OSD_Y)->EnableWindow(m_bCheckOsd);
					GetDlgItem(IDC_COMBO_OSD_ATTRIB)->EnableWindow(m_bCheckOsd);
					GetDlgItem(IDC_COMBO_OSD_TYPE)->EnableWindow(m_bCheckOsd);
					GetDlgItem(IDC_CHK_DISPLAY_WEEK)->EnableWindow(m_bCheckOsd);
					m_comboOsdType.SetCurSel(m_struPicCfg[i].byOSDType);
					m_comboOsdAttrib.SetCurSel(m_struPicCfg[i].byOSDAttrib-1);//����֮����Ҫ��1������Ϊ������0��Ӧ��OSD���ԣ�0������ʾOSD��1��͸��,��˸�ȣ�����ʾOSD��������ȡ������check�ؼ��ˡ���������һ���±�Ϊ0����Ŀ��
					m_bChkDispWeek=m_struPicCfg[i].byDispWeek;
					m_iOsdX=m_struPicCfg[i].wOSDTopLeftX;
					m_iOsdY=m_struPicCfg[i].wOSDTopLeftY;
					m_comboHourType.SetCurSel(m_struPicCfg[i].byHourOSDType);
					
					m_bCheckChanName=m_struPicCfg[i].dwShowChanName;
					GetDlgItem(IDC_EDIT_CHAN_NAME_X)->EnableWindow(m_bCheckChanName);
					GetDlgItem(IDC_EDIT_CHAN_NAME_Y)->EnableWindow(m_bCheckChanName);
					m_iChanNameX=m_struPicCfg[i].wShowNameTopLeftX;
					m_iChanNameY=m_struPicCfg[i].wShowNameTopLeftY;
					
					m_bCheckHideArea=m_struPicCfg[i].dwEnableHide;//�ж��Ƿ���ڸǹ��ܣ�ʹ���ڸǡ���ťʹ�ܡ�
					GetDlgItem(IDC_BTN_HIDE_SETUP)->EnableWindow(m_bCheckHideArea);
					
					m_bCheckMotion = m_struPicCfg[i].struMotion.byEnableHandleMotion;
					GetDlgItem(IDC_BTN_MOTION_SETUP)->EnableWindow(m_bCheckMotion);
					
					m_bCheckVILost=m_struPicCfg[i].struVILost.byEnableHandleVILost;
					GetDlgItem(IDC_BTN_VILOST_SETUP)->EnableWindow(m_bCheckVILost);
					
					if (m_struPicCfg[i].struHideAlarm.dwEnableHideAlarm > 0)//����Ҫ���Ƿ��ڸǱ����Ĳ��������жϣ���Ϊ����ֵ����ȡ0��1��2��3���������1����ѡ���޷����Ϲ���
					{
						m_bCheckHideAlarm = TRUE;
					}
					else
					{
						m_bCheckHideAlarm = FALSE;
					}
					//m_bCheckHideAlarm=m_struPicCfg[i].struHideAlarm.dwEnableHideAlarm;�������������жϣ������д�Ļ�������ָ�ѡ��û�򹳵������
	                GetDlgItem(IDC_BTN_HIDE_ALARM_SETUP)->EnableWindow(m_bCheckHideAlarm);

					
					UpdateData(FALSE);
				}
            
			}
		}
	}
}

/*********************************************************
Function:	OnBnClickedChkHideArea
Desc:		�Ƿ�����ڸ����������
Input:	
Output:	
Return:	
**********************************************************/
void CDlgChanCfg::OnBnClickedChkHideArea() 
{
	UpdateData(TRUE);
	if (m_bCheckHideArea)
	{
		GetDlgItem(IDC_BTN_HIDE_SETUP)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BTN_HIDE_SETUP)->EnableWindow(FALSE);
	}	
}

/*********************************************************
Function:	OnBnClickedChkRecord
Desc:		�Ƿ����Զ��¼��ƻ�������
Input:	
Output:	
Return:	
**********************************************************/
void CDlgChanCfg::OnBnClickedChkRecord() 
{
	UpdateData(TRUE);
	if (m_bCheckRecord)
	{
		GetDlgItem(IDC_BTN_REMOTE_RECORD_SETUP)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_PRERECORD_TIME)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_REC_DELAY)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BTN_REMOTE_RECORD_SETUP)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_PRERECORD_TIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_REC_DELAY)->EnableWindow(FALSE);
	}	
}

/*********************************************************
Function:	OnBnClickedChkMotion
Desc:		�Ƿ�����ƶ��������
Input:	
Output:	
Return:	
**********************************************************/
void CDlgChanCfg::OnBnClickedChkMotion() 
{
	UpdateData(TRUE);
	GetDlgItem(IDC_BTN_MOTION_SETUP)->EnableWindow(m_bCheckMotion);	
}

/*********************************************************
Function:	OnBnClickedChkVilost
Desc:		�Ƿ������Ƶ��ʧ�澯�Ĳ�������
Input:	
Output:	
Return:	
**********************************************************/
void CDlgChanCfg::OnBnClickedChkVilost() 
{
	UpdateData(TRUE);
	GetDlgItem(IDC_BTN_VILOST_SETUP)->EnableWindow(m_bCheckVILost);	
}

/*********************************************************
Function:	OnBnClickedChkHideAlarm
Desc:		�Ƿ�����ڵ��澯����
Input:	
Output:	
Return:	
**********************************************************/
void CDlgChanCfg::OnBnClickedChkHideAlarm() 
{
	UpdateData(TRUE);
	GetDlgItem(IDC_BTN_HIDE_ALARM_SETUP)->EnableWindow(m_bCheckHideAlarm);	
}

/*********************************************************
Function:	OnBnClickedChkOsd
Desc:		�Ƿ����osd������
Input:	
Output:	
Return:	
**********************************************************/
void CDlgChanCfg::OnBnClickedChkOsd() 
{
	UpdateData(TRUE);
    if (m_bCheckOsd)
    {
		GetDlgItem(IDC_EDIT_OSD_X)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_OSD_Y)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_OSD_ATTRIB)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_OSD_TYPE)->EnableWindow(TRUE);
	    GetDlgItem(IDC_CHK_DISPLAY_WEEK)->EnableWindow(TRUE);
		//GetDlgItem(IDC_COMBO_HOUR_TYPE)->EnableWindow(TRUE);//12Сʱ�ƺ����ڿͻ��˺�demo�϶���ûʵ�֡�
	} 
    else
    {
		GetDlgItem(IDC_EDIT_OSD_X)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_OSD_Y)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_OSD_ATTRIB)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_OSD_TYPE)->EnableWindow(FALSE);
	    GetDlgItem(IDC_CHK_DISPLAY_WEEK)->EnableWindow(FALSE);
		//GetDlgItem(IDC_COMBO_HOUR_TYPE)->EnableWindow(FALSE);
    }	
}

/*********************************************************
Function:	OnBnClickedChkChanNamePos
Desc:		�Ƿ�����ͨ�����Ƶ�λ��
Input:	
Output:	
Return:	
**********************************************************/
void CDlgChanCfg::OnBnClickedChkChanNamePos() 
{
	UpdateData(TRUE);
	if (m_bCheckChanName)
	{
		GetDlgItem(IDC_EDIT_CHAN_NAME_X)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_CHAN_NAME_Y)->EnableWindow(TRUE);
	} 
	else
	{
		GetDlgItem(IDC_EDIT_CHAN_NAME_X)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_CHAN_NAME_Y)->EnableWindow(FALSE);
	}
}

/*********************************************************
Function:	OnButtonSetCompressionCfg
Desc:		����ѹ��������Ϣ
Input:	
Output:	
Return:	
**********************************************************/
void CDlgChanCfg::OnButtonSetCompressionCfg() 
{
	if (g_lUserID < 0)
	{
		MessageBox("���ȵ�¼�豸��");
		return;
	}

	UpdateData(TRUE);
	int iSel = m_comboChanNum.GetCurSel();
	int m_iCurChanIndex = m_comboChanNum.GetItemData(iSel);

	m_struCompressionCfg[m_iCurChanIndex].struNormHighRecordPara.byPicQuality=m_comboImageQuality.GetCurSel();
	m_struCompressionCfg[m_iCurChanIndex].struNormHighRecordPara.byStreamType=m_comboStreamType.GetCurSel();
	m_struCompressionCfg[m_iCurChanIndex].struNormHighRecordPara.byResolution=m_comboResolution.GetCurSel();
	m_struCompressionCfg[m_iCurChanIndex].struNormHighRecordPara.byBitrateType=m_comboBitRateType.GetCurSel();
	m_struCompressionCfg[m_iCurChanIndex].struNormHighRecordPara.dwVideoFrameRate=m_comboFrameRate.GetCurSel();
    
	DWORD dwDefBitRate = 0;
	if (m_comboMaxBitRate.GetCurSel() == 22)	//�Զ�������
	{
		dwDefBitRate = m_iBitRate*1024;		
		if (dwDefBitRate < 32*1024)
		{
			dwDefBitRate = 32*1024;
		}
		if (dwDefBitRate > 8192*1024)
		{
			dwDefBitRate =  8192*1024;
		}
		dwDefBitRate |= 0x80000000;
		m_struCompressionCfg[m_iCurChanIndex].struNormHighRecordPara.dwVideoBitrate = dwDefBitRate;
	}
	else
	{
		m_struCompressionCfg[m_iCurChanIndex].struNormHighRecordPara.dwVideoBitrate = m_comboMaxBitRate.GetCurSel()+2;
	}

	m_struCompressionCfg[m_iCurChanIndex].struNormHighRecordPara.byIntervalBPFrame=m_comboFrameBPInterval.GetCurSel();
	m_struCompressionCfg[m_iCurChanIndex].struNormHighRecordPara.wIntervalFrameI=m_iFrame_I_Iterval;
	
	if (!NET_DVR_SetDVRConfig(g_lUserID, NET_DVR_SET_COMPRESSCFG_V30, m_iCurChanIndex+1,&m_struCompressionCfg[m_iCurChanIndex], sizeof(NET_DVR_COMPRESSIONCFG_V30)))
	{
		MessageBox("����ѹ������ʧ��!","warning");
	}
// 	else
// 	{
// 		MessageBox("����ѹ�������ɹ�!","warning");
// 	}
	
}

/*********************************************************
Function:	OnButtonSetRecordCfg
Desc:		����¼�������Ϣ
Input:	
Output:	
Return:	
**********************************************************/
void CDlgChanCfg::OnButtonSetRecordCfg() 
{
	if (g_lUserID < 0)
	{
		MessageBox("���ȵ�¼�豸��");
		return;
	}

	UpdateData(TRUE);
	int iSel = m_comboChanNum.GetCurSel();
	int m_iCurChanIndex = m_comboChanNum.GetItemData(iSel);

	m_struRecordCfg[m_iCurChanIndex].dwRecord = m_bCheckRecord;
	m_struRecordCfg[m_iCurChanIndex].dwPreRecordTime=m_comboPrerecordTime.GetCurSel();
	m_struRecordCfg[m_iCurChanIndex].dwRecordTime=m_comboRecordDelay.GetCurSel();
	if (!NET_DVR_SetDVRConfig(g_lUserID, NET_DVR_SET_RECORDCFG_V30, m_iCurChanIndex+1,&m_struRecordCfg[m_iCurChanIndex], sizeof(NET_DVR_RECORD_V30)))
	{

		MessageBox("����¼�����ʧ��!","warning");
	}
// 	else
// 	{
// 		MessageBox("����¼������ɹ�!","warning");
// 	}	
}

/*********************************************************
Function:	OnButtonSetPictureCfg
Desc:		����ͼ�������Ϣ
Input:	
Output:	
Return:	
**********************************************************/
void CDlgChanCfg::OnButtonSetPictureCfg() 
{
	if (g_lUserID < 0)
	{
		MessageBox("���ȵ�¼�豸��");
		return;
	}
	UpdateData(TRUE);
	int iSel = m_comboChanNum.GetCurSel();
	int m_iCurChanIndex = m_comboChanNum.GetItemData(iSel);

    //��ҳ���ͨ�����Ļ�ȡ�����ã�������ͼ������ṹ���С�
    memcpy(m_struPicCfg[m_iCurChanIndex].sChanName,m_csChannelName,NAME_LEN);

	m_struPicCfg[m_iCurChanIndex].dwEnableHide=m_bCheckHideArea;
	m_struPicCfg[m_iCurChanIndex].struMotion.byEnableHandleMotion=m_bCheckMotion;
	m_struPicCfg[m_iCurChanIndex].struVILost.byEnableHandleVILost=m_bCheckVILost;
	if (!m_bCheckHideAlarm)//���ûѡ�ϡ��ڵ�����������m_bCheckHideAlarm��0����ʹdwEnableHideAlarm����0������dwEnableHideAlarm�����ڵ������ȣ���һ������1�����п�����2��3��
	{
		m_struPicCfg[m_iCurChanIndex].struHideAlarm.dwEnableHideAlarm = m_bCheckHideAlarm;
	}


	m_struPicCfg[m_iCurChanIndex].dwShowOsd=m_bCheckOsd;
	m_struPicCfg[m_iCurChanIndex].byOSDType=m_comboOsdType.GetCurSel();
	m_struPicCfg[m_iCurChanIndex].byOSDAttrib=m_comboOsdAttrib.GetCurSel()+1;//����֮����Ҫ��1������Ϊ������0��Ӧ��OSD���ԣ�0������ʾOSD��1��͸��,��˸�ȣ�����ʾOSD��������ȡ������check�ؼ��ˡ���������һ���±�Ϊ0����Ŀ��
	m_struPicCfg[m_iCurChanIndex].byDispWeek=m_bChkDispWeek;
	m_struPicCfg[m_iCurChanIndex].wOSDTopLeftX=m_iOsdX;
	m_struPicCfg[m_iCurChanIndex].wOSDTopLeftY=m_iOsdY;
	m_struPicCfg[m_iCurChanIndex].byHourOSDType=m_comboHourType.GetCurSel();
	m_struPicCfg[m_iCurChanIndex].dwShowChanName=m_bCheckChanName;
	m_struPicCfg[m_iCurChanIndex].wShowNameTopLeftX=m_iChanNameX;
	m_struPicCfg[m_iCurChanIndex].wShowNameTopLeftY=m_iChanNameY;
	if (!NET_DVR_SetDVRConfig(g_lUserID, NET_DVR_SET_PICCFG_V30, m_iCurChanIndex+1,&m_struPicCfg[m_iCurChanIndex], sizeof(NET_DVR_PICCFG_V30)))
	{
		MessageBox("����ͼ�����ʧ��!","warning");
	}
// 	else
// 	{
// 		MessageBox("����ͼ������ɹ�!","warning");
// 	}
	
}

/*********************************************************
  Function:	OnBnClickedBtnString	
  Desc:		�����ַ����ӵ�����
  Input:	
  Output:	
  Return:	
**********************************************************/
void CDlgChanCfg::OnBnClickedBtnString() 
{
	if (g_lUserID < 0)
	{
		MessageBox("���ȵ�¼�豸��");
		return;
	}
	int iSel = m_comboChanNum.GetCurSel();
	int m_iCurChanIndex = m_comboChanNum.GetItemData(iSel);

	CDlgRemoteShowString dlgRemoteShowString;
	
	dlgRemoteShowString.m_lLoginID = g_lUserID;
	dlgRemoteShowString.m_iChanShowNum = m_iCurChanIndex+1;
	dlgRemoteShowString.DoModal();
	
}

/*********************************************************
  Function:	OnBnClickedBtnHideSetup
  Desc:		�����ڸ����������
  Input:	
  Output:	
  Return:	
**********************************************************/
void CDlgChanCfg::OnBnClickedBtnHideSetup() 
{
	if (g_lUserID < 0)
	{
		MessageBox("���ȵ�¼�豸��");
		return;
	}
	
	CDlgRemoteHideArea dlg;

	int iSel = m_comboChanNum.GetCurSel();
	int m_iCurChanIndex = m_comboChanNum.GetItemData(iSel);

	int i = 0;
	
	for (i=0; i<MAX_SHELTERNUM; i++)
	{
		memcpy(&(dlg.m_struShelter[i]), &(m_struPicCfg[m_iCurChanIndex].struShelter[i]),sizeof(NET_DVR_SHELTER));
	}
	dlg.m_iChanShowNum = m_iCurChanIndex+1;//����ͨ����ͨ����
	dlg.m_lLoginID = g_lUserID;
	if (dlg.DoModal() == IDOK)
	{
		for (i=0; i<MAX_SHELTERNUM; i++)
		{
			memcpy(&(m_struPicCfg[m_iCurChanIndex].struShelter[i]), &(dlg.m_struShelter[i]),sizeof(NET_DVR_SHELTER));
		}
		//m_bSetChannelPara[m_iCurChanIndex] = TRUE;
	}
    m_struPicCfg[m_iCurChanIndex].dwEnableHide=m_bCheckHideArea;
	NET_DVR_SetDVRConfig(g_lUserID, NET_DVR_SET_PICCFG_V30, m_iCurChanIndex+1,&m_struPicCfg[m_iCurChanIndex], sizeof(NET_DVR_PICCFG_V30));
	
}

/*********************************************************
  Function:	OnBnClickedBtnHideAlarmSetup
  Desc:		�����ڵ��澯����������
  Input:	
  Output:	
  Return:	
**********************************************************/
void CDlgChanCfg::OnBnClickedBtnHideAlarmSetup() 
{
	if (g_lUserID < 0)
	{
		MessageBox("���ȵ�¼�豸��");
		return;
	}
	
	CDlgRemoteHideAlarm dlgRemoteHideAlarm;

	int iSel = m_comboChanNum.GetCurSel();
	int m_iCurChanIndex = m_comboChanNum.GetItemData(iSel);
	
	g_lpHideAlarm = &(m_struPicCfg[m_iCurChanIndex].struHideAlarm);
	dlgRemoteHideAlarm.m_iChanShowNum = m_iCurChanIndex+1;
	dlgRemoteHideAlarm.m_lLoginID = g_lUserID;
	//dlgRemoteHideAlarm.m_dwAlarmOutNum = g_struDeviceInfo[0].iAlarmOutNum;
	dlgRemoteHideAlarm.m_dwDevIndex = 0;
	if (dlgRemoteHideAlarm.DoModal() == IDOK)
	{
		NET_DVR_SetDVRConfig(g_lUserID, NET_DVR_SET_PICCFG_V30, m_iCurChanIndex+1,&m_struPicCfg[m_iCurChanIndex], sizeof(NET_DVR_PICCFG_V30));
	}
	
}

/*********************************************************
  Function:	OnBnClickedBtnRemoteRecordSetup
  Desc:		����Զ��¼��ƻ�������
  Input:	
  Output:	
  Return:	
**********************************************************/
void CDlgChanCfg::OnBnClickedBtnRemoteRecordSetup() 
{
	if (g_lUserID < 0)
	{
		MessageBox("���ȵ�¼�豸��");
		return;
	}
	
	CDlgRemoteRecordSchedule dlg;

	int iSel = m_comboChanNum.GetCurSel();
	int m_iCurChanIndex = m_comboChanNum.GetItemData(iSel);
	
	dlg.m_struRemoteRecSchedule = &(m_struRecordCfg[m_iCurChanIndex]);
	if (dlg.DoModal() == IDOK)
	{
		m_struRecordCfg[m_iCurChanIndex].dwRecord = m_bCheckRecord;
		//������¼��ʱ�䴰���У�������󣬰����˳�����ť��Ȼ����ú���ֱ���趨¼��ʱ�䡣
		NET_DVR_SetDVRConfig(g_lUserID, NET_DVR_SET_RECORDCFG_V30, m_iCurChanIndex+1,&m_struRecordCfg[m_iCurChanIndex], sizeof(NET_DVR_RECORD_V30));	
	}
	
}

/*********************************************************
Function:	OnBnClickedBtnMotionSetup
Desc:		�����ƶ���Ⲽ��ʱ������ȵ�����
Input:	
Output:	
Return:	
**********************************************************/
void CDlgChanCfg::OnBnClickedBtnMotionSetup() 
{
	if (g_lUserID < 0)
	{
		MessageBox("���ȵ�¼�豸��");
		return;
	}
	
	CDlgRemoteMotionDetect dlgRemoteMotionDetect;

	int iSel = m_comboChanNum.GetCurSel();
	int m_iCurChanIndex = m_comboChanNum.GetItemData(iSel);
	
	g_lpMotionDetect = &(m_struPicCfg[m_iCurChanIndex].struMotion);
	dlgRemoteMotionDetect.m_iChanShowNum = m_iCurChanIndex+1;
	dlgRemoteMotionDetect.m_lLoginID = g_lUserID;
	// 	dlgRemoteMotionDetect.m_dwAlarmOutNum = m_dwAlarmOutNum;
	dlgRemoteMotionDetect.m_iChanCount = g_struDeviceInfo[0].iDeviceChanNum;
	dlgRemoteMotionDetect.m_dwDevIndex = 0;
	if (dlgRemoteMotionDetect.DoModal() == IDOK)
	{
		//m_bSetChannelPara[m_iCurChanIndex] = TRUE;
		m_struPicCfg[m_iCurChanIndex].struMotion.byEnableHandleMotion=m_bCheckMotion;
		NET_DVR_SetDVRConfig(g_lUserID, NET_DVR_SET_PICCFG_V30, m_iCurChanIndex+1,&m_struPicCfg[m_iCurChanIndex], sizeof(NET_DVR_PICCFG_V30));
		
	}
 	
}

/*********************************************************
Function:	OnBnClickedBtnVilostSetup
Desc:		������Ƶ��ʧ�澯����������
Input:	
Output:	
Return:	
**********************************************************/
void CDlgChanCfg::OnBnClickedBtnVilostSetup() 
{
	if (g_lUserID < 0)
	{
		MessageBox("���ȵ�¼�豸��");
		return;
	}
	
	CDlgRemoteVideoInLost dlgRemoteVideoInLost;

	int iSel = m_comboChanNum.GetCurSel();
	int m_iCurChanIndex = m_comboChanNum.GetItemData(iSel);
	
	g_lpVILost = &(m_struPicCfg[m_iCurChanIndex].struVILost);
	dlgRemoteVideoInLost.m_iChanShowNum = m_iCurChanIndex+1;
	dlgRemoteVideoInLost.m_lLoginID = g_lUserID;
	//dlgRemoteVideoInLost.m_dwAlarmOutNum = m_dwAlarmOutNum;
	dlgRemoteVideoInLost.m_dwDevIndex = 0;
	if (dlgRemoteVideoInLost.DoModal() == IDOK)
	{
		m_struPicCfg[m_iCurChanIndex].struVILost.byEnableHandleVILost=m_bCheckVILost;
		NET_DVR_SetDVRConfig(g_lUserID, NET_DVR_SET_PICCFG_V30, m_iCurChanIndex+1,&m_struPicCfg[m_iCurChanIndex], sizeof(NET_DVR_PICCFG_V30));
	}	
}

/*********************************************************
Function:	OnCbnSelchangeComboChan
Desc:		ͨ��combobox��ͨ��ѡ����Ϣ��Ӧ����
Input:	
Output:	
Return:	
**********************************************************/
void CDlgChanCfg::OnCbnSelchangeComboChan() 
{
	UpdateData(TRUE);
	int iSel = m_comboChanNum.GetCurSel();
	int m_iCurChanIndex = m_comboChanNum.GetItemData(iSel);	

	if (m_iCurChanIndex < MAX_ANALOG_CHANNUM)
	{
		GetDlgItem(IDC_BTN_STRING)->EnableWindow(TRUE);	
	}
	else
	{
		GetDlgItem(IDC_BTN_STRING)->EnableWindow(FALSE);
	}

	//**ѹ�����ֵĲ���**//
	m_comboImageQuality.SetCurSel(m_struCompressionCfg[m_iCurChanIndex].struNormHighRecordPara.byPicQuality);
	m_comboResolution.SetCurSel(m_struCompressionCfg[m_iCurChanIndex].struNormHighRecordPara.byResolution);
	m_comboStreamType.SetCurSel(m_struCompressionCfg[m_iCurChanIndex].struNormHighRecordPara.byStreamType);
	m_comboBitRateType.SetCurSel(m_struCompressionCfg[m_iCurChanIndex].struNormHighRecordPara.byBitrateType);
	m_comboFrameRate.SetCurSel(m_struCompressionCfg[m_iCurChanIndex].struNormHighRecordPara.dwVideoFrameRate);
	if ((m_struCompressionCfg[m_iCurChanIndex].struNormHighRecordPara.dwVideoBitrate >> 31) && 0x01)
	{
		GetDlgItem(IDC_EDIT_BIT_RATE)->EnableWindow(TRUE);
		m_comboMaxBitRate.SetCurSel(22);		//�Զ�������
		m_iBitRate = (m_struCompressionCfg[m_iCurChanIndex].struNormHighRecordPara.dwVideoBitrate & 0x7fffffff)/1024;
	}
	else
	{
		m_comboMaxBitRate.SetCurSel(m_struCompressionCfg[m_iCurChanIndex].struNormHighRecordPara.dwVideoBitrate-2);			
		m_iBitRate = 0;
		GetDlgItem(IDC_EDIT_BIT_RATE)->EnableWindow(FALSE);
	}
	m_comboFrameBPInterval.SetCurSel(m_struCompressionCfg[m_iCurChanIndex].struNormHighRecordPara.byIntervalBPFrame);
	m_comboCopyChan.SetCurSel(0);
	m_comboCompressionType.SetCurSel(0);
	GetDlgItem(IDC_EDIT_FRAME_I_INTERVAL)->EnableWindow(TRUE);
	m_iFrame_I_Iterval=m_struCompressionCfg[m_iCurChanIndex].struNormHighRecordPara.wIntervalFrameI;
	
	//**¼�񲿷ֵĲ���**//
	m_bCheckRecord = m_struRecordCfg[m_iCurChanIndex].dwRecord;
	m_comboPrerecordTime.SetCurSel(m_struRecordCfg[m_iCurChanIndex].dwPreRecordTime);
	m_comboRecordDelay.SetCurSel(m_struRecordCfg[m_iCurChanIndex].dwRecordTime);
	
	//���漸�����ж϶�ʱ¼���Ƿ�򿪣��Ƿ���Խ���Զ��¼��ƻ������á�
	if (m_bCheckRecord)
	{
		GetDlgItem(IDC_BTN_REMOTE_RECORD_SETUP)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_PRERECORD_TIME)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_REC_DELAY)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BTN_REMOTE_RECORD_SETUP)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_PRERECORD_TIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_REC_DELAY)->EnableWindow(FALSE);
	}
	
	//**ͼ�񲿷ֵĲ���**//
	m_csChannelName=m_struPicCfg[m_iCurChanIndex].sChanName;
	m_bCheckOsd=m_struPicCfg[m_iCurChanIndex].dwShowOsd;
	GetDlgItem(IDC_EDIT_OSD_X)->EnableWindow(m_bCheckOsd);
	GetDlgItem(IDC_EDIT_OSD_Y)->EnableWindow(m_bCheckOsd);
	GetDlgItem(IDC_COMBO_OSD_ATTRIB)->EnableWindow(m_bCheckOsd);
	GetDlgItem(IDC_COMBO_OSD_TYPE)->EnableWindow(m_bCheckOsd);
	GetDlgItem(IDC_CHK_DISPLAY_WEEK)->EnableWindow(m_bCheckOsd);
	m_comboOsdType.SetCurSel(m_struPicCfg[m_iCurChanIndex].byOSDType);
	m_comboOsdAttrib.SetCurSel(m_struPicCfg[m_iCurChanIndex].byOSDAttrib-1);//����֮����Ҫ��1������Ϊ������0��Ӧ��OSD���ԣ�0������ʾOSD��1��͸��,��˸�ȣ�����ʾOSD��������ȡ������check�ؼ��ˡ���������һ���±�Ϊ0����Ŀ��
	m_bChkDispWeek=m_struPicCfg[m_iCurChanIndex].byDispWeek;
	m_iOsdX=m_struPicCfg[m_iCurChanIndex].wOSDTopLeftX;
	m_iOsdY=m_struPicCfg[m_iCurChanIndex].wOSDTopLeftY;
	m_comboHourType.SetCurSel(m_struPicCfg[m_iCurChanIndex].byHourOSDType);
	
	m_bCheckChanName=m_struPicCfg[m_iCurChanIndex].dwShowChanName;
	GetDlgItem(IDC_EDIT_CHAN_NAME_X)->EnableWindow(m_bCheckChanName);
	GetDlgItem(IDC_EDIT_CHAN_NAME_Y)->EnableWindow(m_bCheckChanName);
	m_iChanNameX=m_struPicCfg[m_iCurChanIndex].wShowNameTopLeftX;
	m_iChanNameY=m_struPicCfg[m_iCurChanIndex].wShowNameTopLeftY;
	
	m_bCheckHideArea=m_struPicCfg[m_iCurChanIndex].dwEnableHide;//�ж��Ƿ���ڸǹ��ܣ�ʹ���ڸǡ���ťʹ�ܡ�
	GetDlgItem(IDC_BTN_HIDE_SETUP)->EnableWindow(m_bCheckHideArea);
	
	m_bCheckMotion = m_struPicCfg[m_iCurChanIndex].struMotion.byEnableHandleMotion;
	GetDlgItem(IDC_BTN_MOTION_SETUP)->EnableWindow(m_bCheckMotion);
	
	m_bCheckVILost=m_struPicCfg[m_iCurChanIndex].struVILost.byEnableHandleVILost;
	GetDlgItem(IDC_BTN_VILOST_SETUP)->EnableWindow(m_bCheckVILost);
	
	if (m_struPicCfg[m_iCurChanIndex].struHideAlarm.dwEnableHideAlarm > 0)//����Ҫ���Ƿ��ڸǱ����Ĳ��������жϣ���Ϊ����ֵ����ȡ0��1��2��3���������1����ѡ���޷����Ϲ���
	{
		m_bCheckHideAlarm = TRUE;
	}
	else
	{
		m_bCheckHideAlarm = FALSE;
	}
	//m_bCheckHideAlarm=m_struPicCfg[i].struHideAlarm.dwEnableHideAlarm;�������������жϣ������д�Ļ�������ָ�ѡ��û�򹳵������
	GetDlgItem(IDC_BTN_HIDE_ALARM_SETUP)->EnableWindow(m_bCheckHideAlarm);
	
	UpdateData(FALSE);
}

/*********************************************************
Function:	OnCbnSelchangeComboMaxBitRate
Desc:		ѡ���Զ�������
Input:	
Output:	
Return:	
**********************************************************/

void CDlgChanCfg::OnCbnSelchangeComboMaxBitRate() 
{
	UpdateData(TRUE);
	if (m_comboMaxBitRate.GetCurSel() == 22)	//self-define bitrate
	{
		GetDlgItem(IDC_EDIT_BIT_RATE)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_BIT_RATE)->EnableWindow(FALSE);
	}
	
}

/*********************************************************
Function:	OnBnClickedChanCfgOK
Desc:		����ͨ��������Ϣ
Input:	
Output:	
Return:	
**********************************************************/
void CDlgChanCfg::OnBnClickedChanCfgOK() 
{
	if (g_lUserID < 0)
	{
		MessageBox("���ȵ�¼�豸��");
		return;
	}
	OnButtonSetCompressionCfg();
	OnButtonSetRecordCfg();
	OnButtonSetPictureCfg();
	GetDlgItem(IDC_BTN_CHAN_COPY)->EnableWindow(TRUE);	
}

/*********************************************************
Function:	OnBnClickedChanCfgCopy
Desc:		����ͨ��������Ϣ
Input:	
Output:	
Return:	
**********************************************************/
void CDlgChanCfg::OnBnClickedChanCfgCopy() 
{
	UpdateData(TRUE);
	int i = 0,j = 0,k = 0;
	int m_iChanNum = g_struDeviceInfo[0].iDeviceChanNum;//��ȡ�豸��ģ��ͨ����
	int m_iCurChanIndex = m_comboChanNum.GetItemData(m_comboChanNum.GetCurSel());
	int m_iCopyChanIndex = m_comboCopyChan.GetItemData(m_comboCopyChan.GetCurSel());
	if (m_iCopyChanIndex == m_iCurChanIndex)
	{
		AfxMessageBox("ͨ����ͬ�����ø��ƣ�");
		return;
	}
	if (m_iCopyChanIndex == -1)//���Ƶ�ȫ��ͨ��
	{
		for ( i=0; i<m_iChanNum/*MAX_CHANNUM_V30*/; i++)//Ŀǰֻ���Ƶ�ģ��ͨ�������Ƶ�ipͨ������Щ���⡣
		{
			//ѹ����������
			m_struCompressionCfg[i].struNormHighRecordPara.byStreamType = m_struCompressionCfg[m_iCurChanIndex].struNormHighRecordPara.byStreamType;
			m_struCompressionCfg[i].struNormHighRecordPara.byPicQuality = m_struCompressionCfg[m_iCurChanIndex].struNormHighRecordPara.byPicQuality;
			m_struCompressionCfg[i].struNormHighRecordPara.dwVideoBitrate = m_struCompressionCfg[m_iCurChanIndex].struNormHighRecordPara.dwVideoBitrate;
			m_struCompressionCfg[i].struNormHighRecordPara.byResolution = m_struCompressionCfg[m_iCurChanIndex].struNormHighRecordPara.byResolution;
			m_struCompressionCfg[i].struNormHighRecordPara.dwVideoFrameRate = m_struCompressionCfg[m_iCurChanIndex].struNormHighRecordPara.dwVideoFrameRate;
			m_struCompressionCfg[i].struNormHighRecordPara.byBitrateType = m_struCompressionCfg[m_iCurChanIndex].struNormHighRecordPara.byBitrateType;
			m_struCompressionCfg[i].struNormHighRecordPara.byIntervalBPFrame = m_struCompressionCfg[m_iCurChanIndex].struNormHighRecordPara.byIntervalBPFrame;	
			m_struCompressionCfg[i].struNormHighRecordPara.wIntervalFrameI = m_struCompressionCfg[m_iCurChanIndex].struNormHighRecordPara.wIntervalFrameI;
            //¼������
			m_struRecordCfg[i].dwRecord = m_struRecordCfg[m_iCurChanIndex].dwRecord;
			m_struRecordCfg[i].dwRecordTime = m_struRecordCfg[m_iCurChanIndex].dwRecordTime;
			m_struRecordCfg[i].dwPreRecordTime = m_struRecordCfg[m_iCurChanIndex].dwPreRecordTime;
			//m_struRecordCfg[i].dwRecorderDuration = m_struRecordCfg[m_iCurChanIndex].dwRecorderDuration;
			//����ʱ���
			for (k=0; k<MAX_DAYS; k++)
			{
				memcpy(&(m_struRecordCfg[i].struRecAllDay[k]), &(m_struRecordCfg[m_iCurChanIndex].struRecAllDay[k]), sizeof(NET_DVR_RECORDDAY));
				for (j=0; j<MAX_TIMESEGMENT_V30; j++)
				{
					memcpy(&m_struRecordCfg[i].struRecordSched[k][j],&m_struRecordCfg[m_iCurChanIndex].struRecordSched[k][j],sizeof(NET_DVR_RECORDSCHED));				
				}
			}
			//ͼ���������
			m_struPicCfg[i].dwEnableHide = m_bCheckHideArea;//����ֱ������ҳ�������ֵ����ʵ��m_struPicCfg[m_iCurChanIndex].dwEnableHide��һ�µġ�����Ҳһ����
			m_struPicCfg[i].struMotion.byEnableHandleMotion = m_bCheckMotion;
			m_struPicCfg[i].struVILost.byEnableHandleVILost = m_bCheckVILost;
			if (!m_bCheckHideAlarm)
			{
				m_struPicCfg[i].struHideAlarm.dwEnableHideAlarm = m_bCheckHideAlarm;
			}
			m_struPicCfg[i].dwShowChanName = m_bCheckChanName;
			m_struPicCfg[i].wShowNameTopLeftX = m_iChanNameX;
			m_struPicCfg[i].wShowNameTopLeftY = m_iChanNameY;
			m_struPicCfg[i].dwShowOsd = m_bCheckOsd;
			m_struPicCfg[i].wOSDTopLeftX = m_iOsdX;
			m_struPicCfg[i].wOSDTopLeftY = m_iOsdY;
			m_struPicCfg[i].byDispWeek = m_bChkDispWeek;
			m_struPicCfg[i].byOSDAttrib = m_comboOsdAttrib.GetCurSel()+1;
			m_struPicCfg[i].byOSDType = m_comboOsdType.GetCurSel();
			m_struPicCfg[i].byHourOSDType = m_comboHourType.GetCurSel();

			char szLan[128] = {0};
			int iChanShow = i+m_lStartChannel;
			if (!NET_DVR_SetDVRConfig(g_lUserID,NET_DVR_SET_COMPRESSCFG_V30, iChanShow,&(m_struCompressionCfg[i]), sizeof(NET_DVR_COMPRESSIONCFG_V30)))
			{
				g_StringLanType(szLan, "ѹ����������ʧ��", "CompressCfg Save failed");
				AfxMessageBox(szLan);
			}
						
			if (!NET_DVR_SetDVRConfig(g_lUserID,NET_DVR_SET_RECORDCFG_V30, iChanShow,&(m_struRecordCfg[i]), sizeof(NET_DVR_RECORD_V30)))	
			{
				g_StringLanType(szLan, "¼���������ʧ��", " RecordCfg Save failed");
				AfxMessageBox(szLan);
			}
			if (!NET_DVR_SetDVRConfig(g_lUserID, NET_DVR_SET_PICCFG_V30, iChanShow, &(m_struPicCfg[i]), sizeof(NET_DVR_PICCFG_V30)))
			{
				g_StringLanType(szLan, "ͼ���������ʧ��", "PictureCfg Save failed");
				AfxMessageBox(szLan);
			}
		}
		//AfxMessageBox("���Ƶ�����ͨ���ɹ�!");
	}
	else//���Ƶ�����ͨ��
	{
		//ѹ����������
		m_struCompressionCfg[m_iCopyChanIndex].struNormHighRecordPara.byStreamType = m_struCompressionCfg[m_iCurChanIndex].struNormHighRecordPara.byStreamType;
		m_struCompressionCfg[m_iCopyChanIndex].struNormHighRecordPara.byPicQuality = m_struCompressionCfg[m_iCurChanIndex].struNormHighRecordPara.byPicQuality;
		m_struCompressionCfg[m_iCopyChanIndex].struNormHighRecordPara.dwVideoBitrate = m_struCompressionCfg[m_iCurChanIndex].struNormHighRecordPara.dwVideoBitrate;
		m_struCompressionCfg[m_iCopyChanIndex].struNormHighRecordPara.byResolution = m_struCompressionCfg[m_iCurChanIndex].struNormHighRecordPara.byResolution;
		m_struCompressionCfg[m_iCopyChanIndex].struNormHighRecordPara.dwVideoFrameRate = m_struCompressionCfg[m_iCurChanIndex].struNormHighRecordPara.dwVideoFrameRate;
		m_struCompressionCfg[m_iCopyChanIndex].struNormHighRecordPara.byBitrateType = m_struCompressionCfg[m_iCurChanIndex].struNormHighRecordPara.byBitrateType;
		m_struCompressionCfg[m_iCopyChanIndex].struNormHighRecordPara.byIntervalBPFrame = m_struCompressionCfg[m_iCurChanIndex].struNormHighRecordPara.byIntervalBPFrame;	
		m_struCompressionCfg[m_iCopyChanIndex].struNormHighRecordPara.wIntervalFrameI = m_struCompressionCfg[m_iCurChanIndex].struNormHighRecordPara.wIntervalFrameI;
		//¼������
		m_struRecordCfg[m_iCopyChanIndex].dwRecord = m_struRecordCfg[m_iCurChanIndex].dwRecord;
		m_struRecordCfg[m_iCopyChanIndex].dwRecordTime = m_struRecordCfg[m_iCurChanIndex].dwRecordTime;
		m_struRecordCfg[m_iCopyChanIndex].dwPreRecordTime = m_struRecordCfg[m_iCurChanIndex].dwPreRecordTime;
		//m_struRecordCfg[m_iCopyChanIndex].dwRecorderDuration = m_struRecordCfg[m_iCurChanIndex].dwRecorderDuration;
		//����ʱ���
		for (k=0; k<MAX_DAYS; k++)
		{
			memcpy(&(m_struRecordCfg[m_iCopyChanIndex].struRecAllDay[k]), &(m_struRecordCfg[m_iCurChanIndex].struRecAllDay[k]), sizeof(NET_DVR_RECORDDAY));
			for (j=0; j<MAX_TIMESEGMENT_V30; j++)
			{
				memcpy(&m_struRecordCfg[m_iCopyChanIndex].struRecordSched[k][j],&m_struRecordCfg[m_iCurChanIndex].struRecordSched[k][j],sizeof(NET_DVR_RECORDSCHED));				
			}
		}
		//ͼ���������
		m_struPicCfg[m_iCopyChanIndex].dwEnableHide = m_bCheckHideArea;//����ֱ������ҳ�������ֵ����ʵ��m_struPicCfg[m_iCurChanIndex].dwEnableHide��һ�µġ�����Ҳһ����
		m_struPicCfg[m_iCopyChanIndex].struMotion.byEnableHandleMotion = m_bCheckMotion;
		m_struPicCfg[m_iCopyChanIndex].struVILost.byEnableHandleVILost = m_bCheckVILost;
		if (!m_bCheckHideAlarm)
		{
			m_struPicCfg[m_iCopyChanIndex].struHideAlarm.dwEnableHideAlarm = m_bCheckHideAlarm;
		}
		m_struPicCfg[m_iCopyChanIndex].dwShowChanName = m_bCheckChanName;
		m_struPicCfg[m_iCopyChanIndex].wShowNameTopLeftX = m_iChanNameX;
		m_struPicCfg[m_iCopyChanIndex].wShowNameTopLeftY = m_iChanNameY;
		m_struPicCfg[m_iCopyChanIndex].dwShowOsd = m_bCheckOsd;
		m_struPicCfg[m_iCopyChanIndex].wOSDTopLeftX = m_iOsdX;
		m_struPicCfg[m_iCopyChanIndex].wOSDTopLeftY = m_iOsdY;
		m_struPicCfg[m_iCopyChanIndex].byDispWeek = m_bChkDispWeek;
		m_struPicCfg[m_iCopyChanIndex].byOSDAttrib = m_comboOsdAttrib.GetCurSel()+1;
		m_struPicCfg[m_iCopyChanIndex].byOSDType = m_comboOsdType.GetCurSel();
		m_struPicCfg[m_iCopyChanIndex].byHourOSDType = m_comboHourType.GetCurSel();	
		
		char szLan[128] = {0};
		int iChanShow = m_iCopyChanIndex+m_lStartChannel;
		if (!NET_DVR_SetDVRConfig(g_lUserID,NET_DVR_SET_COMPRESSCFG_V30, iChanShow,&(m_struCompressionCfg[m_iCopyChanIndex]), sizeof(NET_DVR_COMPRESSIONCFG_V30)))
		{
			g_StringLanType(szLan, "ѹ����������ʧ��", "CompressionCfg Save failed");
			AfxMessageBox(szLan);
		}
		
		if (!NET_DVR_SetDVRConfig(g_lUserID,NET_DVR_SET_RECORDCFG_V30, iChanShow,&(m_struRecordCfg[m_iCopyChanIndex]), sizeof(NET_DVR_RECORD_V30)))	
		{
			g_StringLanType(szLan, "¼���������ʧ��", " RecordCfg Save failed");
			AfxMessageBox(szLan);
		}
		if (!NET_DVR_SetDVRConfig(g_lUserID, NET_DVR_SET_PICCFG_V30, iChanShow, &(m_struPicCfg[m_iCopyChanIndex]), sizeof(NET_DVR_PICCFG_V30)))
		{
			g_StringLanType(szLan, "ͼ���������ʧ��", "PictureCfg Save failed");
			AfxMessageBox(szLan);
		}
		//AfxMessageBox("���Ƴɹ�!");
	}
	
}
