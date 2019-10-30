// DlgUHFRFIDReaderCfg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ClientDemo.h"
#include "DlgUHFRFIDReaderCfg.h"
#include "afxdialogex.h"
#include "DlgUHFBasicInfo.h"
#include "DlgHardDiskStorageTest.h"


// CDlgUHFRFIDReaderCfg �Ի���

IMPLEMENT_DYNAMIC(CDlgUHFRFIDReaderCfg, CDialogEx)

CDlgUHFRFIDReaderCfg::CDlgUHFRFIDReaderCfg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgUHFRFIDReaderCfg::IDD, pParent)
    , m_lUserID(-1)
    , m_iDeviceIndex(-1)
{

}

CDlgUHFRFIDReaderCfg::~CDlgUHFRFIDReaderCfg()
{
}

void CDlgUHFRFIDReaderCfg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgUHFRFIDReaderCfg, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON_BASIC_CFG, &CDlgUHFRFIDReaderCfg::OnBnClickedButtonBasicCfg)
    ON_BN_CLICKED(IDC_BUTTON_HARD_DISK_STORAGE_TEST, &CDlgUHFRFIDReaderCfg::OnBnClickedButtonHardDiskStorageTest)
END_MESSAGE_MAP()


// CDlgUHFRFIDReaderCfg ��Ϣ�������


void CDlgUHFRFIDReaderCfg::OnBnClickedButtonBasicCfg()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CDlgUHFBasicInfo dlg;
    dlg.m_lUserID = m_lUserID;
    dlg.m_iDeviceIndex = m_iDeviceIndex;
    dlg.DoModal();
}


void CDlgUHFRFIDReaderCfg::OnBnClickedButtonHardDiskStorageTest()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CDlgHardDiskStorageTest dlg;
    dlg.m_lUserID = m_lUserID;
    dlg.m_iDeviceIndex = m_iDeviceIndex;
    dlg.DoModal();
}
