/*******************************************************
Copyright 2008-2011 Digital Technology Co., Ltd. 
��  ����	PTZButton.cpp
������λ��	����
��  д��	shizhiping
��  �ڣ�	2009.5
��  ����	��̨��ť��
��  �ģ�	
********************************************************/
#include "stdafx.h"
#include "RealPlay.h"
#include "PTZButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


int g_iPtzBtnIndex  = 0;
/////////////////////////////////////////////////////////////////////////////
// CPTZButton

CPTZButton::CPTZButton()
{
	m_iSubBtnIndex = g_iPtzBtnIndex++;
	m_lPlayHandle  = -1;
}

CPTZButton::~CPTZButton()
{

}


BEGIN_MESSAGE_MAP(CPTZButton, CButton)
	//{{AFX_MSG_MAP(CPTZButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPTZButton message handlers

/*************************************************
������:    	OnLButtonDown
��������:	���������£���ʼ��̨����
�������:   
�������:   			
����ֵ:		
*************************************************/
void CPTZButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	LONG lPlayHandle = g_pMainDlg->GetPlayHandle();
	int iPTZSpeed = g_pMainDlg->GetPTZSpeed();
	switch(m_iSubBtnIndex)
	{
	case 0:        //��̨��
		PTZControlAll(lPlayHandle,TILT_UP,0,iPTZSpeed);
		break;
	case 1:			//��̨��
		PTZControlAll(lPlayHandle,TILT_DOWN,0,iPTZSpeed);
		break;
	case 2:			//��̨��
		PTZControlAll(lPlayHandle,PAN_LEFT,0,iPTZSpeed);
		break;
	case 3:			//��̨��
		PTZControlAll(lPlayHandle,PAN_RIGHT,0,iPTZSpeed);
		break;
	case 4:         //������
		PTZControlAll(lPlayHandle,ZOOM_IN,0,iPTZSpeed);
		break;
	case 5:			//������
		PTZControlAll(lPlayHandle,ZOOM_OUT,0,iPTZSpeed);
		break;
	case 6:			//�۽���
		PTZControlAll(lPlayHandle,FOCUS_NEAR,0,iPTZSpeed);
		break;
	case 7:			//�۽���
		PTZControlAll(lPlayHandle,FOCUS_FAR,0,iPTZSpeed);
		break;
	case 8:			//��Ȧ��
		PTZControlAll(lPlayHandle,IRIS_OPEN,0,iPTZSpeed);
		break;
	case 9:			//��Ȧ��
		PTZControlAll(lPlayHandle,IRIS_CLOSE,0,iPTZSpeed);
		break;
	case 10:        //����
		PTZControlAll(lPlayHandle,UP_LEFT,0,iPTZSpeed);
		break;
	case 11:        //����
		PTZControlAll(lPlayHandle,UP_RIGHT,0,iPTZSpeed);
		break;
	case 12:        //����
		PTZControlAll(lPlayHandle,DOWN_LEFT,0,iPTZSpeed);
		break;
	case 13:        //����
		PTZControlAll(lPlayHandle,DOWN_RIGHT,0,iPTZSpeed);
		break;

	default:
		break;
	}
	
	CButton::OnLButtonDown(nFlags, point);
}

/*************************************************
������:    	OnLButtonUp
��������:	����������ֹͣ��̨����
�������:   
�������:   			
����ֵ:		
*************************************************/
void CPTZButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
//	int iChanIndex = pMainDlg->GetCurChanIndex();
	LONG lPlayHandle = g_pMainDlg->GetPlayHandle();
	int iPTZSpeed = g_pMainDlg->GetPTZSpeed();
	switch(m_iSubBtnIndex)
	{
	case 0:        //��̨��
		PTZControlAll(lPlayHandle,TILT_UP,1,iPTZSpeed);
		break;
	case 1:			//��̨��
		PTZControlAll(lPlayHandle,TILT_DOWN,1,iPTZSpeed);
		break;
	case 2:			//��̨��
		PTZControlAll(lPlayHandle,PAN_LEFT,1,iPTZSpeed);
		break;
	case 3:			//��̨��
		PTZControlAll(lPlayHandle,PAN_RIGHT,1,iPTZSpeed);
		break;
	case 4:         //������
		PTZControlAll(lPlayHandle,ZOOM_IN,1,iPTZSpeed);
		break;
	case 5:			//������
		PTZControlAll(lPlayHandle,ZOOM_OUT,1,iPTZSpeed);
		break;
	case 6:			//�۽���
		PTZControlAll(lPlayHandle,FOCUS_NEAR,1,iPTZSpeed);
		break;
	case 7:			//�۽���
		PTZControlAll(lPlayHandle,FOCUS_FAR,1,iPTZSpeed);
		break;
	case 8:			//��Ȧ��
		PTZControlAll(lPlayHandle,IRIS_OPEN,1,iPTZSpeed);
		break;
	case 9:			//��Ȧ��
		PTZControlAll(lPlayHandle,IRIS_CLOSE,1,iPTZSpeed);
		break;
	case 10:        //����
		PTZControlAll(lPlayHandle,UP_LEFT,1,iPTZSpeed);
		break;
	case 11:        //����
		PTZControlAll(lPlayHandle,UP_RIGHT,1,iPTZSpeed);
		break;
	case 12:        //����
		PTZControlAll(lPlayHandle,DOWN_LEFT,1,iPTZSpeed);
		break;
	case 13:        //����
		PTZControlAll(lPlayHandle,DOWN_RIGHT,1,iPTZSpeed);
		break;
//	case 14:        //�Զ�
//		PTZControlAll(lPlayHandle,PAN_AUTO,1,iPTZSpeed);
//		break;
	default:
		break;
	}
	CButton::OnLButtonUp(nFlags, point);
}

/*************************************************
������:    	PTZControlAll
��������:	��̨���ƺ���
�������:   
�������:   			
����ֵ:		
*************************************************/
void CPTZButton::PTZControlAll(LONG lRealHandle, DWORD dwPTZCommand, DWORD dwStop, int Speed)
{
	if(lRealHandle>=0)
	{
		BOOL ret;
		if(Speed>=1)
		{
			ret = NET_DVR_PTZControlWithSpeed(lRealHandle,dwPTZCommand,dwStop,Speed);
			if(!ret)
			{
				MessageBox("��̨����ʧ��!");
				return;
			}
		}
		else
		{
			ret = NET_DVR_PTZControl(lRealHandle,dwPTZCommand,dwStop);
			if(!ret)
			{
				MessageBox("��̨����ʧ��!");
				return;
			}
		}
	}

}
