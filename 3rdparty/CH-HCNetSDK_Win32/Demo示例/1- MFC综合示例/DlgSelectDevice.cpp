// DlgSelectDevice.cpp : 实现文件
//

#include "stdafx.h"
#include "ClientDemo.h"
#include "DlgSelectDevice.h"
#include "afxdialogex.h"


// CDlgSelectDevice 对话框

IMPLEMENT_DYNAMIC(CDlgSelectDevice, CDialogEx)

CDlgSelectDevice::CDlgSelectDevice(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSelectDevice::IDD, pParent)
{

}

CDlgSelectDevice::~CDlgSelectDevice()
{
}

void CDlgSelectDevice::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TREEALL, m_treeAll);
}


BEGIN_MESSAGE_MAP(CDlgSelectDevice, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_EXIT, &CDlgSelectDevice::OnBnClickedBtnExit)
    ON_BN_CLICKED(IDC_BTN_CONFIRM, &CDlgSelectDevice::OnBnClickedBtnConfirm)
    ON_NOTIFY(NM_CLICK, IDC_TREEALL, &CDlgSelectDevice::OnNMClickTreeall)
END_MESSAGE_MAP()

BOOL CDlgSelectDevice::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    LoadTreeImage();
    CreateTree();

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常:  OCX 属性页应返回 FALSE
}

// CDlgSelectDevice 消息处理程序
void CDlgSelectDevice::CreateTree()
{
    char szLan[1024] = { 0 };
    g_StringLanType(szLan, "所有设备", "All Devices");
    HTREEITEM hRoot = m_treeAll.InsertItem(szLan, TREE_ALL, TREE_ALL);
    CString strTemp = _T("");
    CString strChanTmp = _T("");
    int i = 0;
    HTREEITEM hDevice = NULL;
    for (i = 0; i < MAX_DEVICES; i++)
    {
        if (g_struDeviceInfo[i].iDeviceChanNum != -1)
        {
            strTemp.Format("%s", g_struDeviceInfo[i].chLocalNodeName);

            //sprintf(buf,"%s\r\n",g_struDeviceInfo[i].chLocalNodeName);                //server title
            if (g_struDeviceInfo[i].lLoginID >= 0)
            {
                hDevice = m_treeAll.InsertItem(strTemp, DEVICE_LOGIN, DEVICE_LOGIN, hRoot);
                m_treeAll.SetItemData(hDevice, DEVICE_LOGIN * 1000 + g_struDeviceInfo[i].iDeviceIndex);
            }
            else
            {
                hDevice = m_treeAll.InsertItem(strTemp, DEVICE_LOGOUT, DEVICE_LOGOUT, hRoot);
                m_treeAll.SetItemData(hDevice, DEVICE_LOGOUT * 1000 + g_struDeviceInfo[i].iDeviceIndex);
            }

        }
    }
    m_treeAll.SetItemState(hRoot, INDEXTOSTATEIMAGEMASK(0), TVIS_STATEIMAGEMASK);
    m_treeAll.Expand(hRoot, TVE_EXPAND);
    m_treeAll.Expand(m_treeAll.GetRootItem(), TVE_EXPAND);
}

void CDlgSelectDevice::LoadTreeImage(void)
{
    CBitmap bmp[4];

    m_TreeImage.Create(16, 16, ILC_COLOR32 | ILC_MASK, 1, 1);
    bmp[TREE_ALL].LoadBitmap(IDB_BITMAP_TREE);
    m_TreeImage.Add(&bmp[TREE_ALL], RGB(1, 1, 1));
    bmp[DEVICE_LOGOUT].LoadBitmap(IDB_BITMAP_LOGOUT);
    m_TreeImage.Add(&bmp[DEVICE_LOGOUT], RGB(1, 1, 1));
    bmp[DEVICE_LOGIN].LoadBitmap(IDB_BITMAP_LOGIN);
    m_TreeImage.Add(&bmp[DEVICE_LOGIN], RGB(1, 1, 1));
    bmp[DEVICE_FORTIFY].LoadBitmap(IDB_BITMAP_FORTIFY);
    m_TreeImage.Add(&bmp[DEVICE_FORTIFY], RGB(1, 1, 1));

    m_treeAll.SetImageList(&m_TreeImage, LVSIL_NORMAL);
}

void CDlgSelectDevice::OnNMClickTreeall(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO:  在此添加控件通知处理程序代码
    CPoint pt(0, 0);
    CRect rc(0, 0, 0, 0);
    GetCursorPos(&pt);
    GetDlgItem(IDC_TREEALL)->GetWindowRect(&rc);
    ScreenToClient(&rc);
    ScreenToClient(&pt);
    pt.x = pt.x - rc.left;
    pt.y = pt.y - rc.top;

    UINT uFlag = 0;
    HTREEITEM hSelect = m_treeAll.HitTest(pt, &uFlag);

    if (NULL == hSelect)
    {
        return;
    }

    m_treeAll.SelectItem(hSelect);

    BOOL bCheck = m_treeAll.GetCheck(hSelect);
    bCheck = !bCheck;

    if (TREE_ALL_T == m_treeAll.GetItemData(hSelect) / 1000)
    {

    }
    else
    {
        int iDevIndex = m_treeAll.GetItemData(hSelect) % 1000;
        if ((g_struDeviceInfo[iDevIndex].lLoginID >= 0 && uFlag != LVHT_TOLEFT)\
            || (g_struDeviceInfo[iDevIndex].lLoginID < 0 && uFlag == LVHT_TOLEFT))
        {
            m_treeAll.SetCheck(hSelect, bCheck);
        }
        else
        {
            m_treeAll.SetCheck(hSelect, !bCheck);
        }
    }

    //最多同时选择一个设备
    HTREEITEM hRoot;
    HTREEITEM hChild;
    hRoot = m_treeAll.GetRootItem();
    if (hRoot == NULL)
    {
        return;
    }
    hChild = m_treeAll.GetChildItem(hRoot);
    while (hChild != NULL)
    {
        if (hChild != hSelect)
        {
            m_treeAll.SetCheck(hChild, FALSE);
        }
        hChild = m_treeAll.GetNextSiblingItem(hChild);
    }
    *pResult = 0;
}

void CDlgSelectDevice::OnBnClickedBtnExit()
{
    // TODO:  在此添加控件通知处理程序代码
    DlgAcsFaceAndTemplate *pDlg = (DlgAcsFaceAndTemplate *)this->GetParent();   //获得指向父窗口的指针
    pDlg->selectedID = -1;
    CDialog::OnCancel();
}


void CDlgSelectDevice::OnBnClickedBtnConfirm()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);

    DlgAcsFaceAndTemplate *pDlg = (DlgAcsFaceAndTemplate *)this->GetParent();   //获得指向父窗口的指针
    pDlg->selectedID = -1;
    HTREEITEM hRoot;
    HTREEITEM hChild;
    
    int iDevIndex = 0;
    BOOL bChk = FALSE;

    hRoot = m_treeAll.GetRootItem();
    if (hRoot == NULL)
    {
        return;
    }
    hChild = m_treeAll.GetChildItem(hRoot);
    while (hChild != NULL)
    {
        iDevIndex = m_treeAll.GetItemData(hChild) % 1000;
        bChk = m_treeAll.GetCheck(hChild);
        if (bChk)
        {
            pDlg->selectedID = g_struDeviceInfo[iDevIndex].lLoginID;
        }
        hChild = m_treeAll.GetNextSiblingItem(hChild);
    }

    CDialog::OnCancel();
}