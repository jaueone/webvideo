// DlgVcaVersionList.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ClientDemo.h"
#include "DlgVcaVersionList.h"
#include "afxdialogex.h"


// CDlgVcaVersionList �Ի���

IMPLEMENT_DYNAMIC(CDlgVcaVersionList, CDialogEx)

CDlgVcaVersionList::CDlgVcaVersionList(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgVcaVersionList::IDD, pParent)
{

}

CDlgVcaVersionList::~CDlgVcaVersionList()
{
}

void CDlgVcaVersionList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_VCA_VERSION_LIST, m_lstVcaVersionList);
}


BEGIN_MESSAGE_MAP(CDlgVcaVersionList, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_GET_VCA_VERSIONLIST, &CDlgVcaVersionList::OnBnClickedBtnGetVcaVersionlist)
END_MESSAGE_MAP()


// CDlgVcaVersionList ��Ϣ�������
BOOL CDlgVcaVersionList::OnInitDialog()
{
    CDialog::OnInitDialog();

//    m_cmbCommand.SetCurSel(0);
    char szLanTemp[128] = { 0 };
    m_lstVcaVersionList.SetExtendedStyle(m_lstVcaVersionList.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES);

    g_StringLanType(szLanTemp, "���汾��", "Major Version");
    m_lstVcaVersionList.InsertColumn(0, szLanTemp, LVCFMT_LEFT, 40, -1);
    g_StringLanType(szLanTemp, "�ΰ汾��", "Minor Version");
    m_lstVcaVersionList.InsertColumn(1, szLanTemp, LVCFMT_LEFT, 40, -1);
    g_StringLanType(szLanTemp, "������", "Revision Number");
    m_lstVcaVersionList.InsertColumn(2, szLanTemp, LVCFMT_LEFT, 80, -1);
    g_StringLanType(szLanTemp, "�����", "Build Number");
    m_lstVcaVersionList.InsertColumn(3, szLanTemp, LVCFMT_LEFT, 100, -1);
    g_StringLanType(szLanTemp, "�汾���ڡ���", "Version Year");
    m_lstVcaVersionList.InsertColumn(4, szLanTemp, LVCFMT_LEFT, 100, -1);
    g_StringLanType(szLanTemp, "�汾���ڡ���", "Version Month");
    m_lstVcaVersionList.InsertColumn(5, szLanTemp, LVCFMT_LEFT, 100, -1);
    g_StringLanType(szLanTemp, "�汾���ڡ���", "Version Day");
    m_lstVcaVersionList.InsertColumn(6, szLanTemp, LVCFMT_LEFT, 100, -1);
    g_StringLanType(szLanTemp, "�㷨������", "VCA Name");
    m_lstVcaVersionList.InsertColumn(7, szLanTemp, LVCFMT_LEFT, 100, -1);

    return TRUE;
}

void CDlgVcaVersionList::OnBnClickedBtnGetVcaVersionlist()
{
    UpdateData(TRUE);

    NET_DVR_VCA_VERSION_LIST struVcaVersionList = { 0 };
    struVcaVersionList.dwSize = sizeof(struVcaVersionList);

    NET_DVR_STD_CONFIG struCfg = { 0 };
    struCfg.lpCondBuffer = NULL;
    struCfg.dwCondSize = 0;
    struCfg.lpOutBuffer = &struVcaVersionList;
    struCfg.dwOutSize = sizeof(struVcaVersionList);
    memset(m_szStatusBuf, 0, ISAPI_STATUS_LEN);
    struCfg.lpStatusBuffer = m_szStatusBuf;
    struCfg.dwStatusSize = ISAPI_STATUS_LEN;

    if (!NET_DVR_GetSTDConfig(m_lUserID, NET_DVR_GET_VCA_VERSION_LIST, &struCfg))
    {
        OutputDebugString(m_szStatusBuf);
        g_pMainDlg->AddLog(m_lDeviceIndex, OPERATION_FAIL_T, "NET_DVR_GET_VCA_VERSION_LIST");
    }
    else
    {
        g_pMainDlg->AddLog(m_lDeviceIndex, OPERATION_SUCC_T, "NET_DVR_GET_VCA_VERSION_LIST");
        AddVcaVersionListItem(struVcaVersionList);
    }
}

void CDlgVcaVersionList::AddVcaVersionListItem(const NET_DVR_VCA_VERSION_LIST &Item)
{
    BOOL bTemp = FALSE;

    int iItemCount = 0;
    int i = 0;
    char szStr[128] = { 0 };
    for (i = 0; i < 64; i++)
    {
        
        int iItemCount = m_lstVcaVersionList.GetItemCount();
        m_lstVcaVersionList.InsertItem(iItemCount, "");
        
        if ((Item.struVcaVersion[i].wMajorVersion == 0) && (Item.struVcaVersion[i].wMinorVersion == 0) && (Item.struVcaVersion[i].wRevisionNumber == 0) && (Item.struVcaVersion[i].wBuildNumber == 0)\
             && (Item.struVcaVersion[i].wVersionYear == 0) && (Item.struVcaVersion[i].byVersionMonth == 0) && (Item.struVcaVersion[i].byVersionDay == 0) && (Item.struVcaVersion[i].byType == 0))
        {
            return;
        }

        //���汾��
        memset(szStr, 0, sizeof(szStr));
        sprintf(szStr, "%d", Item.struVcaVersion[i].wMajorVersion);
        m_lstVcaVersionList.SetItemText(iItemCount, 0, szStr);

        //�ΰ汾��
        memset(szStr, 0, sizeof(szStr));
        sprintf(szStr, "%d", Item.struVcaVersion[i].wMinorVersion);
        m_lstVcaVersionList.SetItemText(iItemCount, 1, szStr);

        //������
        memset(szStr, 0, sizeof(szStr));
        sprintf(szStr, "%d", Item.struVcaVersion[i].wRevisionNumber);
        m_lstVcaVersionList.SetItemText(iItemCount, 2, szStr);

        //�����
        memset(szStr, 0, sizeof(szStr));
        sprintf(szStr, "%d", Item.struVcaVersion[i].wBuildNumber);
        m_lstVcaVersionList.SetItemText(iItemCount, 3, szStr);

        //�汾����-��
        memset(szStr, 0, sizeof(szStr));
        sprintf(szStr, "%d", Item.struVcaVersion[i].wVersionYear);
        m_lstVcaVersionList.SetItemText(iItemCount, 4, szStr);

        //�汾����-��
        memset(szStr, 0, sizeof(szStr));
        sprintf(szStr, "%d", Item.struVcaVersion[i].byVersionMonth);
        m_lstVcaVersionList.SetItemText(iItemCount, 5, szStr);

        //�汾����-��
        memset(szStr, 0, sizeof(szStr));
        sprintf(szStr, "%d", Item.struVcaVersion[i].byVersionDay);
        m_lstVcaVersionList.SetItemText(iItemCount, 6, szStr);

        //�㷨����������
        memset(szStr, 0, sizeof(szStr));
        switch (Item.struVcaVersion[i].byType)
        {
        case 0:
            sprintf(szStr, "%s", "�豸���ʹ���");
            break;
        case 1:
            sprintf(szStr, "%s", "��������");
            break;
        case 2:
            sprintf(szStr, "%s", "��Ϊ����");
            break;
        case 3:
            sprintf(szStr, "%s", "��������");
            break;
        case 4:
            sprintf(szStr, "%s", "����ץ��");
            break;
        case 5:
            sprintf(szStr, "%s", "����ʶ��");
            break;
        case 6:
            sprintf(szStr, "%s", "�����Ա�");
            break;
        case 7:
            sprintf(szStr, "%s", "ѧ��վ�����");
            break;
        case 8:
            sprintf(szStr, "%s", "��������ͳ��");
            break;
        case 9:
            sprintf(szStr, "%s", "�������ֿ�");
            break;
        case 10:
            sprintf(szStr, "%s", "��ȫñ���");
            break;
        case 11:
            sprintf(szStr, "%s", "���ݼ���㷨��");
            break;
        case 12:
            sprintf(szStr, "%s", "������ͳ��");
            break;
        case 13:
            sprintf(szStr, "%s", "(����Ŀ��ṹ���㷨)HMS");
            break;
        case 14:
            sprintf(szStr, "%s", "��ʦ��Ϊ���");
            break;
        case 15:
            sprintf(szStr, "%s", "��Ա�ܶ�");
            break;
        case 16:
            sprintf(szStr, "%s", "�����쳣");
            break;
        case 17:
            sprintf(szStr, "%s", "��ڼ��");
            break;
        case 18:
            sprintf(szStr, "%s", "��Ա����");
            break;
        case 19:
            sprintf(szStr, "%s", "����ץ�Ľ�ģ");
            break;
        case 20:
            sprintf(szStr, "%s", "HMS��ģ");
            break;
        case 21:
            sprintf(szStr, "%s", "̬�Ʒ���");
            break;
        case 22:
            sprintf(szStr, "%s", "�ǹ��㷨");
            break;
        default:
            break;
        }
        m_lstVcaVersionList.SetItemText(iItemCount, 7, szStr);
    }

}


