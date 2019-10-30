// DlgAcsAttendance.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ClientDemo.h"
#include "DlgAcsAttendance.h"
#include "afxdialogex.h"


// CDlgAcsAttendance �Ի���

IMPLEMENT_DYNAMIC(CDlgAcsAttendance, CDialog)

CDlgAcsAttendance::CDlgAcsAttendance(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAcsAttendance::IDD, pParent)
{

}

CDlgAcsAttendance::~CDlgAcsAttendance()
{
}

void CDlgAcsAttendance::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgAcsAttendance, CDialog)
    ON_BN_CLICKED(IDC_BTN_SCHEDULE_INFO, &CDlgAcsAttendance::OnBnClickedBtnScheduleInfo)
    ON_BN_CLICKED(IDC_BTN_ATTENDANCE_SUMMARY_INFO, &CDlgAcsAttendance::OnBnClickedBtnAttendanceSummaryInfo)
    ON_BN_CLICKED(IDC_BTN_ATTENDANCE_RECORD_INFO, &CDlgAcsAttendance::OnBnClickedBtnAttendanceRecordInfo)
    ON_BN_CLICKED(IDC_BTN_ATTENDANCE_RECORD_INFO, &CDlgAcsAttendance::OnBnClickedBtnAttendanceRecordInfo)
    ON_BN_CLICKED(IDC_BTN_ABNORMAL_INFO, &CDlgAcsAttendance::OnBnClickedBtnAbnormalInfo)
    ON_BN_CLICKED(IDC_BTN_DEPARTMENT_PARAM, &CDlgAcsAttendance::OnBnClickedBtnDepartmentParam)
    ON_BN_CLICKED(IDC_BTN_SCHEDULE_PLAN, &CDlgAcsAttendance::OnBnClickedBtnSchedulePlan)
    ON_BN_CLICKED(IDC_BTN_ATTENDANCE_RULE, &CDlgAcsAttendance::OnBnClickedBtnAttendanceRule)
    ON_BN_CLICKED(IDC_BTN_ORDINARY_CLASS, &CDlgAcsAttendance::OnBnClickedBtnOrdinaryClass)
    ON_BN_CLICKED(IDC_BTN_WORKING_CLASS, &CDlgAcsAttendance::OnBnClickedBtnWorkingClass)
    ON_BN_CLICKED(IDC_BTN_ATTENDANCE_HOLIDAY_GROUP, &CDlgAcsAttendance::OnBnClickedBtnAttendanceHolidayGroup)
    ON_BN_CLICKED(IDC_BTN_ATTENDANCE_HOLIDAY_PLAN, &CDlgAcsAttendance::OnBnClickedBtnAttendanceHolidayPlan)
END_MESSAGE_MAP()


// CDlgAcsAttendance ��Ϣ�������

#include "DlgScheduleInfo.h"
void CDlgAcsAttendance::OnBnClickedBtnScheduleInfo()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CDlgScheduleInfo dlg;
    dlg.m_lServerID = m_lServerID;
    dlg.m_iDevIndex = m_iDevIndex;
    dlg.DoModal();
}

#include "DlgAttendanceSummaryInfo.h"
void CDlgAcsAttendance::OnBnClickedBtnAttendanceSummaryInfo()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CDlgAttendanceSummaryInfo dlg;
    dlg.m_lServerID = m_lServerID;
    dlg.m_iDevIndex = m_iDevIndex;
    dlg.DoModal();
}

#include "DlgAttendanceRecordInfo.h"
void CDlgAcsAttendance::OnBnClickedBtnAttendanceRecordInfo()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CDlgAttendanceRecordInfo dlg;
    dlg.m_lServerID = m_lServerID;
    dlg.m_iDevIndex = m_iDevIndex;
    dlg.DoModal();
}

#include "DlgAbnormalInfo.h"
void CDlgAcsAttendance::OnBnClickedBtnAbnormalInfo()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CDlgAbnormalInfo dlg;
    dlg.m_lServerID = m_lServerID;
    dlg.m_iDevIndex = m_iDevIndex;
    dlg.DoModal();
}

#include "DlgDepartmentParam.h"
void CDlgAcsAttendance::OnBnClickedBtnDepartmentParam()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CDlgDepartmentParam dlg;
    dlg.m_lServerID = m_lServerID;
    dlg.m_iDevIndex = m_iDevIndex;
    dlg.DoModal();
}

#include "DlgSchedulePlan.h"
void CDlgAcsAttendance::OnBnClickedBtnSchedulePlan()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CDlgSchedulePlan dlg;
    dlg.m_lServerID = m_lServerID;
    dlg.m_iDevIndex = m_iDevIndex;
    dlg.DoModal();
}

#include "DlgAttendanceRule.h"
void CDlgAcsAttendance::OnBnClickedBtnAttendanceRule()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CDlgAttendanceRule dlg;
    dlg.m_lServerID = m_lServerID;
    dlg.m_iDevIndex = m_iDevIndex;
    dlg.DoModal();
}

#include "DlgOrdinaryClass.h"
void CDlgAcsAttendance::OnBnClickedBtnOrdinaryClass()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CDlgOrdinaryClass dlg;
    dlg.m_lServerID = m_lServerID;
    dlg.m_iDevIndex = m_iDevIndex;
    dlg.DoModal();
}

#include "DlgWorkingClass.h"
void CDlgAcsAttendance::OnBnClickedBtnWorkingClass()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CDlgWorkingClass dlg;
    dlg.m_lServerID = m_lServerID;
    dlg.m_iDevIndex = m_iDevIndex;
    dlg.DoModal();
}

#include "DlgAttendanceHolidayGroup.h"
void CDlgAcsAttendance::OnBnClickedBtnAttendanceHolidayGroup()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CDlgAttendanceHolidayGroup dlg;
    dlg.m_lServerID = m_lServerID;
    dlg.m_iDevIndex = m_iDevIndex;
    dlg.DoModal();
}

#include "DlgAttendanceHolidayPlan.h"
void CDlgAcsAttendance::OnBnClickedBtnAttendanceHolidayPlan()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CDlgAttendanceHolidayPlan dlg;
    dlg.m_lServerID = m_lServerID;
    dlg.m_iDevIndex = m_iDevIndex;
    dlg.DoModal();
}
