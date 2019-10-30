/*******************************************************
Copyright 2008-2011 Digital Technology Co., Ltd. 
��  ����	DlgPTZCruise.cpp
������λ��	����
��  д��	shizhiping
��  �ڣ�	2009.5
��  ����	��̨Ѳ��·�����öԻ���
��  �ģ�	
********************************************************/

#include "stdafx.h"
#include "RealPlay.h"
#include "DlgPTZCruise.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPTZCruise dialog


CDlgPTZCruise::CDlgPTZCruise(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPTZCruise::IDD, pParent)
	, m_iCruiseNum(1)
	, m_iSeqPoint(1)
	, m_iPresetNum(1)
	, m_iSeqDwell(2)
	, m_iSeqSpeed(30)
	, m_iChanNum(-1)
{
	//{{AFX_DATA_INIT(CDlgPTZCruise)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgPTZCruise::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPTZCruise)
	DDX_Control(pDX, IDC_COMBO_CRUISE_PRESET, m_comboSeqPreset);
	DDX_Control(pDX, IDC_COMBO_CRUISE_TIME, m_comboSeqTime);
	DDX_Control(pDX, IDC_COMBO_CRUISE_SPEED, m_comboSeqSpeed);
	DDX_Control(pDX, IDC_COMBO_CRUISE_POINT, m_comboPoint);
	DDX_Control(pDX, IDC_COMBO_CRUISE_ROUTE, m_comboCruiseRoute);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPTZCruise, CDialog)
	//{{AFX_MSG_MAP(CDlgPTZCruise)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBtnExit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPTZCruise message handlers

BOOL CDlgPTZCruise::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	int i;
	CString tmp;
	for(i=0; i<MAX_CRUISE_SEQ; i++)
	{
       tmp.Format("Ѳ��·��%d",i+1);
	   m_comboCruiseRoute.AddString(tmp);
	}
	m_comboCruiseRoute.SetCurSel(0);

	for(i=0; i<MAX_CRUISE_POINT; i++)
	{
       tmp.Format("%d",i+1);
	   m_comboPoint.AddString(tmp);
	}
	m_comboPoint.SetCurSel(0);

	for(i=0; i<MAX_CRUISE_PRESET; i++)
	{
		tmp.Format("%d",i+1);
	    m_comboSeqPreset.AddString(tmp);
	}
	m_comboSeqPreset.SetCurSel(0);

	for(i=0; i<MAX_CRUISE_TIME; i++)
	{
		tmp.Format("%d",i+1);
	    m_comboSeqTime.AddString(tmp);
	}
	m_comboSeqTime.SetCurSel(3);

	for(i=0; i<MAX_CRUISE_SPEED; i++)
	{
		tmp.Format("%d",i+1);
	    m_comboSeqSpeed.AddString(tmp);
	}
	m_comboSeqSpeed.SetCurSel(9);

   
    m_iChanNum = g_pMainDlg->m_struDeviceInfo.struChanInfo[g_pMainDlg->m_iCurChanIndex].iChanIndex;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


/*************************************************
������:    	OnButtonAdd
��������:	���Ѳ���㵽Ѳ��·��
�������:   
�������:   			
����ֵ:		
*************************************************/
void CDlgPTZCruise::OnButtonAdd() 
{
	UpdateData(TRUE);
	BOOL bRet = FALSE;
	m_iCruiseNum = m_comboCruiseRoute.GetCurSel()+1;
	m_iSeqPoint = m_comboPoint.GetCurSel()+1;
	m_iPresetNum = m_comboSeqPreset.GetCurSel()+1;
	m_iSeqDwell = m_comboSeqTime.GetCurSel()+1;
	m_iSeqSpeed = m_comboSeqSpeed.GetCurSel()+1;
    

	bRet = Set_NET_DVR_PTZCruise(m_iChanNum,FILL_PRE_SEQ,(BYTE)m_iCruiseNum,(BYTE)m_iSeqPoint,m_iPresetNum);
	if(bRet)
	{
		bRet = Set_NET_DVR_PTZCruise(m_iChanNum,SET_SEQ_DWELL,(BYTE)m_iCruiseNum,(BYTE)m_iSeqPoint,m_iSeqDwell);
		if(bRet)
		{
            bRet = Set_NET_DVR_PTZCruise(m_iChanNum,SET_SEQ_SPEED,(BYTE)m_iCruiseNum,(BYTE)m_iSeqPoint,m_iSeqSpeed);
		}
	}
	
}

/*************************************************
������:    	Set_NET_DVR_PTZCruise
��������:	����Ѳ��·��
�������:   
�������:   			
����ֵ:		
*************************************************/
BOOL CDlgPTZCruise::Set_NET_DVR_PTZCruise(int iChanNum, DWORD dwPTZCruiseCmd,BYTE byCruiseRoute, BYTE byCruisePoint, WORD wInput)
{
    if(g_pMainDlg->m_lPlayHandle >=0 )
	{
        if(!NET_DVR_PTZCruise(g_pMainDlg->m_lPlayHandle,dwPTZCruiseCmd,byCruiseRoute,byCruisePoint,wInput))
		{
			MessageBox("����ʧ��");
			return FALSE;
		}
	}
	else
	{
        if(!NET_DVR_PTZCruise_Other(g_pMainDlg->m_struDeviceInfo.lLoginID,m_iChanNum,dwPTZCruiseCmd,byCruiseRoute,byCruisePoint,wInput))
		{
            MessageBox("����ʧ��");
			return FALSE;
		}
	}

	return TRUE;
}

/*************************************************
������:    	OnButtonDel
��������:	ɾ��Ѳ����
�������:   
�������:   			
����ֵ:		
*************************************************/
void CDlgPTZCruise::OnButtonDel() 
{
	
	UpdateData(TRUE);
	BOOL bRet = FALSE;
	m_iCruiseNum = m_comboCruiseRoute.GetCurSel()+1;
	m_iSeqPoint = m_comboPoint.GetCurSel()+1;
	m_iPresetNum = m_comboSeqPreset.GetCurSel()+1;
    if(g_pMainDlg->m_lPlayHandle >=0 )
	{
        if(!NET_DVR_PTZCruise(g_pMainDlg->m_lPlayHandle,CLE_PRE_SEQ ,m_iCruiseNum,m_iSeqPoint,m_iPresetNum))
		{
			MessageBox("ɾ��ʧ��");
			return;
		}
	}
	else
	{
        if(!NET_DVR_PTZCruise_Other(g_pMainDlg->m_struDeviceInfo.lLoginID,m_iChanNum,CLE_PRE_SEQ ,m_iCruiseNum,m_iSeqPoint,m_iPresetNum))
		{
            MessageBox("ɾ��ʧ��");
			return;
		}
	}
}


/*************************************************
������:    	OnBtnExit
��������:	�˳�
�������:   
�������:   			
����ֵ:		
*************************************************/
void CDlgPTZCruise::OnBtnExit() 
{
	CDialog::OnCancel();
	
}
