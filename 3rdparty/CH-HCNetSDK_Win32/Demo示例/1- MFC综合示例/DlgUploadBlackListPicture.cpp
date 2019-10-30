// DlgUploadBlackListPicture.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ClientDemo.h"
#include "DlgUploadBlackListPicture.h"
#include "afxdialogex.h"

// ��������ɴ���������ȡ����������512K
#define MAX_PICTURE_LEN 1024 * 1024
#define WM_MSG_UPLOAD_PICTURE_FINISH 1002
// CDlgUploadBlackListPicture �Ի���

IMPLEMENT_DYNAMIC(CDlgUploadBlackListPicture, CDialogEx)

CDlgUploadBlackListPicture::CDlgUploadBlackListPicture(CWnd* pParent /*=NULL*/)
    : CDialogEx(CDlgUploadBlackListPicture::IDD, pParent)
    , m_szName(_T(""))
    , m_szPictureName(_T(""))
    , m_dwPictureLen(0)
    , m_szCardNumber(_T(""))
    , m_dwPictureNum(0)
    , m_bIsValid(FALSE)
{
    m_lServerID = -1;
    m_iDeviceIndex = -1;
    m_lChannel = 1;
    m_lUploadPicCfgHandle = -1;
    m_dwPictureNum = 0;
}

CDlgUploadBlackListPicture::~CDlgUploadBlackListPicture()
{
}

void CDlgUploadBlackListPicture::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_SEX, m_comboSex);
    DDX_Text(pDX, IDC_EDIT_NAME, m_szName);
    DDX_Text(pDX, IDC_EDIT_PICTURE_ADDRESS, m_szPictureName);
    DDX_Text(pDX, IDC_EDIT_PICTURE_LEN, m_dwPictureLen);
    DDX_Control(pDX, IDC_LIST_BALCK_LIST_PICTURE_CFG, m_listPicture);
    DDX_Text(pDX, IDC_EDIT_CARD_NUMBER, m_szCardNumber);
    DDX_Text(pDX, IDC_EDIT_NUM_OF_PICTURE, m_dwPictureNum);
    DDX_Check(pDX, IDC_CHECK_ISDELETE, m_bIsValid);
}


BEGIN_MESSAGE_MAP(CDlgUploadBlackListPicture, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON_BROWSE, &CDlgUploadBlackListPicture::OnBnClickedButtonBrowse)
    ON_BN_CLICKED(IDC_BUTTON_ADD, &CDlgUploadBlackListPicture::OnBnClickedButtonAdd)
    ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CDlgUploadBlackListPicture::OnBnClickedButtonClear)
    ON_BN_CLICKED(IDC_BUTTON_UPLOAD, &CDlgUploadBlackListPicture::OnBnClickedButtonUpload)
    ON_BN_CLICKED(IDOK, &CDlgUploadBlackListPicture::OnBnClickedOk)
    ON_MESSAGE(WM_MSG_UPLOAD_PICTURE_FINISH, OnMsgUploadPictureCfgFinish)
    ON_WM_DESTROY()
    ON_BN_CLICKED(IDC_BUTTON_DELETE, &CDlgUploadBlackListPicture::OnBnClickedButtonDelete)
    ON_BN_CLICKED(IDC_CHECK_ISDELETE, &CDlgUploadBlackListPicture::OnBnClickedCheckIsdelete)
END_MESSAGE_MAP()


// CDlgUploadBlackListPicture ��Ϣ�������


LRESULT CDlgUploadBlackListPicture::OnMsgUploadPictureCfgFinish(WPARAM wParam, LPARAM lParam)
{
    if (!NET_DVR_StopRemoteConfig(m_lUploadPicCfgHandle))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_StopRemoteConfig failed");
        return 0;
    }
    m_lUploadPicCfgHandle = -1;
    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_BULK_UPLOAD_BLACK_LIST_PICTURE Upload finished");
    return 0;
}

void CALLBACK g_fUploadPictureCallback(DWORD dwType, void* lpBuffer, DWORD dwBufLen, void* pUserData)
{
    CDlgUploadBlackListPicture* pDlg = (CDlgUploadBlackListPicture*)pUserData;
    if (pDlg == NULL)
    {
        return;
    }
    pDlg->ProcessUploadPictureCfgCallbackData(dwType, lpBuffer, dwBufLen);
}

void CDlgUploadBlackListPicture::ProcessUploadPictureCfgCallbackData(DWORD dwType, void* lpBuffer, DWORD dwBufLen)
{
    if (dwType == NET_SDK_CALLBACK_TYPE_STATUS)
    {
        DWORD dwStatus = *(DWORD*)lpBuffer;
        if (dwStatus == NET_SDK_CALLBACK_STATUS_PROCESSING)
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "Upload PROCESSING %d", m_dwSendIndex);

            SendNextPicture();
        }
        else if (dwStatus == NET_SDK_CALLBACK_STATUS_FAILED)
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "Upload Err:NET_SDK_CALLBACK_STATUS_FAILED %d", m_dwSendIndex);

            SendNextPicture();
        }
        //���������رճ�����
        else if (dwStatus == NET_SDK_CALLBACK_STATUS_SUCCESS)
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "Upload SUCCESS");
            PostMessage(WM_MSG_UPLOAD_PICTURE_FINISH, 0, 0);
        }
        else if (dwStatus == NET_SDK_CALLBACK_STATUS_EXCEPTION)
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "Upload Exception");
            PostMessage(WM_MSG_UPLOAD_PICTURE_FINISH, 0, 0);
        }
        else
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "Upload SUCCESS");
            PostMessage(WM_MSG_UPLOAD_PICTURE_FINISH, 0, 0);
        }
    }
    else if (dwType == NET_SDK_CALLBACK_TYPE_DATA)
    {
        NET_DVR_BLACK_LIST_PICTURE_STATUS struCfg = { 0 };
        memcpy(&struCfg, lpBuffer, sizeof(struCfg));


        BOOL bSendOk = FALSE;

        if (struCfg.byStatus == 3 || struCfg.byStatus == 1)
        {
            bSendOk = TRUE;
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "Upload PROCESSING,CardNo:%s", struCfg.byCardNo);
        }

        if (!bSendOk)
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "Upload Failed,CardNo:%s", struCfg.byCardNo);
        }

        SendNextPicture();
    }
}

void CDlgUploadBlackListPicture::OnBnClickedButtonBrowse()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CString strFilter = _T("All Files(*.*)|*.*||");
    CFileDialog dlg(TRUE, "*.*", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter);
    if (dlg.DoModal() != IDOK)
    {
        return;
    }
    m_szPictureName = dlg.GetPathName();
    GetDlgItem(IDC_EDIT_PICTURE_ADDRESS)->SetWindowText(m_szPictureName);
}


BOOL CDlgUploadBlackListPicture::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    char szLan[128] = { 0 };

    UpdateData(TRUE);

    m_comboSex.SetCurSel(1);

    m_szCardNumber = "1234567890";

    g_StringLanType(szLan, "����", "San Zhang");
    m_szName = szLan;

    m_listPicture.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
    g_StringLanType(szLan, "���", "No");
    m_listPicture.InsertColumn(0, szLan, LVCFMT_LEFT, 50, -1);
    g_StringLanType(szLan, "��������������", "Card No");
    m_listPicture.InsertColumn(1, szLan, LVCFMT_LEFT, 140, -1);
    g_StringLanType(szLan, "ͼƬ����·��", "Data Path");
    m_listPicture.InsertColumn(2, szLan, LVCFMT_LEFT, 200, -1);
    g_StringLanType(szLan, "����", "Name");
    m_listPicture.InsertColumn(3, szLan, LVCFMT_LEFT, 60, -1);
    g_StringLanType(szLan, "�Ա�", "Sex");
    m_listPicture.InsertColumn(4, szLan, LVCFMT_LEFT, 60, -1);

    m_dwLen = 0;
    m_bIsValid = TRUE;
    GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);

    UpdateData(FALSE);
    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣:  OCX ����ҳӦ���� FALSE
}


void CDlgUploadBlackListPicture::OnBnClickedButtonAdd()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);

    //1.get data from control
    NET_DVR_BLACK_LIST_PICTURE_CFG struPictureCfg = { 0 };

    struPictureCfg.bySex = m_comboSex.GetCurSel();
    struPictureCfg.dwSize = sizeof(struPictureCfg);
    memcpy(struPictureCfg.byCardNo, m_szCardNumber.GetBuffer(m_szCardNumber.GetLength()), m_szCardNumber.GetLength());
    memcpy(struPictureCfg.byName, m_szName.GetBuffer(m_szName.GetLength()), m_szName.GetLength());
    struPictureCfg.byPictureValid = m_bIsValid;

    if (m_bIsValid)
    {
        struPictureCfg.pPictureBuffer = ReadPictureFromFile(m_szPictureName);
        struPictureCfg.dwPictureLen = m_dwLen;

        if (struPictureCfg.pPictureBuffer == NULL)
        {
            return;
        }
    }

    //2.is duplicated?
    LPNET_DVR_BLACK_LIST_PICTURE_CFG pPictureInfo = NULL;
    int iItemIndex = GetExistItem(&struPictureCfg);
    if (iItemIndex == -1)
    {
        pPictureInfo = new NET_DVR_BLACK_LIST_PICTURE_CFG;
        if (pPictureInfo == NULL)
        {
            return;
        }
        int iSize = sizeof(NET_DVR_BLACK_LIST_PICTURE_CFG);
        int nItemCount = m_listPicture.GetItemCount();

        CString strItem = "";
        strItem.Format("%d", nItemCount + 1);
        iItemIndex = m_listPicture.InsertItem(nItemCount, strItem);
        m_listPicture.SetItemData(nItemCount, (DWORD)pPictureInfo);
    }
    else
    {
        pPictureInfo = (LPNET_DVR_BLACK_LIST_PICTURE_CFG)m_listPicture.GetItemData(iItemIndex);
    }
    memcpy(pPictureInfo, &struPictureCfg, sizeof(struPictureCfg));

    //3.set to list
    char szLan[1024] = { 0 };

    sprintf(szLan, "%d", iItemIndex);
    sprintf(szLan, "%s", pPictureInfo->byCardNo);
    m_listPicture.SetItemText(iItemIndex, 1, szLan);
    sprintf(szLan, "%s", m_szPictureName);
    m_listPicture.SetItemText(iItemIndex, 2, szLan);
    sprintf(szLan, "%s", pPictureInfo->byName);
    m_listPicture.SetItemText(iItemIndex, 3, szLan);
    if (pPictureInfo->bySex == 0)
    {
        g_StringLanType(szLan, "��Ч", "invalid");
    }
    else if (pPictureInfo->bySex == 1)
    {
        g_StringLanType(szLan, "��", "male");
    }
    else
    {
        g_StringLanType(szLan, "Ů", "female");
    }
    m_listPicture.SetItemText(iItemIndex, 4, szLan);
    UpdateData(FALSE);

    return;
}


void CDlgUploadBlackListPicture::OnBnClickedButtonClear()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);
    m_listPicture.DeleteAllItems();
    Clear();
    UpdateData(FALSE);
}


void CDlgUploadBlackListPicture::OnBnClickedButtonUpload()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    if (m_lUploadPicCfgHandle != -1)
    {
        NET_DVR_StopRemoteConfig(m_lUploadPicCfgHandle);
    }
    UpdateData(TRUE);

    NET_DVR_BLACK_LIST_PICTURE_COND struCond = { 0 };
    struCond.dwSize = sizeof(struCond);
    struCond.dwPictureNum = m_dwPictureNum;

    m_lUploadPicCfgHandle = NET_DVR_StartRemoteConfig(m_lServerID, NET_DVR_BULK_UPLOAD_BLACK_LIST_PICTURE, &struCond, sizeof(struCond), g_fUploadPictureCallback, this);
    if (m_lUploadPicCfgHandle == -1)
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_BULK_UPLOAD_BLACK_LIST_PICTURE failed");
        return;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_BULK_UPLOAD_BLACK_LIST_PICTURE success");
    }

    //�ϴ���һ��ͼƬ, ʧ��ʱ�ر�����
    m_dwSendIndex = 0;
    if (!SendFirstPicture())
    {
        NET_DVR_StopRemoteConfig(m_lUploadPicCfgHandle);
        m_lUploadPicCfgHandle = -1;
    }
}

/** @fn BOOL SendFirstPicture()
 *  @brief ���͵�һ�ź�����ͼƬ
 *  @return  HPR_TRUE--success, HPR_FALSE--fail.
 */
BOOL CDlgUploadBlackListPicture::SendFirstPicture()
{
    int nItemCount = m_listPicture.GetItemCount();

    if (nItemCount <= 0)
    {
        AfxMessageBox("Please add info to list at first");
        return FALSE;
    }
    m_dwSendIndex = 0;
    //��ȡ��һ����������������
    LPNET_DVR_BLACK_LIST_PICTURE_CFG pPictureInfo = (LPNET_DVR_BLACK_LIST_PICTURE_CFG)m_listPicture.GetItemData(m_dwSendIndex);

    pPictureInfo->byPictureValid = m_bIsValid;

    DWORD dwPictureLen = pPictureInfo->dwPictureLen;
    char*  pPictureBuffer = pPictureInfo->pPictureBuffer;

    //��Чʱ������ͼƬ
    if (!m_bIsValid)
    {
        pPictureInfo->pPictureBuffer = NULL;
        pPictureInfo->dwPictureLen = 0;
    }

    if (pPictureInfo == NULL)
    {
        return FALSE;
    }

    if (!NET_DVR_SendRemoteConfig(m_lUploadPicCfgHandle, ENUM_ACS_SEND_DATA, (char *)pPictureInfo, sizeof(*pPictureInfo)))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "Send Fail,CardNO: %s", pPictureInfo->byCardNo);
        return FALSE;
    }

    if (!m_bIsValid)
    {
        pPictureInfo->dwPictureLen = dwPictureLen;
        pPictureInfo->pPictureBuffer = pPictureBuffer;
    }

    return TRUE;
}

/** @fn BOOL SendNextPicture()
*  @brief ������һ�ź�����ͼƬ
*  @return  HPR_TRUE--success, HPR_FALSE--fail.
*/
BOOL CDlgUploadBlackListPicture::SendNextPicture()
{
    int nItemCount = m_listPicture.GetItemCount();

    if (nItemCount < 0)
    {
        AfxMessageBox("Please add info to list at first");
    }

    m_dwSendIndex++;
    //�Ѿ�������
    if (m_dwSendIndex >= m_dwPictureNum || m_dwSendIndex >= nItemCount)
    {
        return TRUE;
    }

    LPNET_DVR_BLACK_LIST_PICTURE_CFG pPictureInfo = (LPNET_DVR_BLACK_LIST_PICTURE_CFG)m_listPicture.GetItemData(m_dwSendIndex);
    
    pPictureInfo->byPictureValid = m_bIsValid;

    DWORD dwPictureLen = pPictureInfo->dwPictureLen;
    char*  pPictureBuffer = pPictureInfo->pPictureBuffer;

    //��Чʱ������ͼƬ
    if (!m_bIsValid)
    {
        pPictureInfo->pPictureBuffer = NULL;
        pPictureInfo->dwPictureLen = 0;
    }

    if (pPictureInfo == NULL)
    {
        return FALSE;
    }

    if (!NET_DVR_SendRemoteConfig(m_lUploadPicCfgHandle, ENUM_ACS_SEND_DATA, (char *)pPictureInfo, sizeof(*pPictureInfo)))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "Send Fail,CardNO: %s", pPictureInfo->byCardNo);
        PostMessage(WM_MSG_UPLOAD_PICTURE_FINISH, 0, 0);
        return FALSE;
    }

    if (!m_bIsValid)
    {
        pPictureInfo->dwPictureLen = dwPictureLen;
        pPictureInfo->pPictureBuffer = pPictureBuffer;
    }

    return TRUE;
}

/** @fn  CDlgUploadBlackListPicture::GetExistItem(const NET_DVR_BLACK_LIST_PICTURE_CFG *lpCardCfg)
 *  @brief ���б��в����Ƿ������ͬ��ʵ��
 *  @param(IN) NET_DVR_BLACK_LIST_PICTURE_CFG *lpCardCfg
 *  @return  index--the index, -1--no duplicate.
 */
int CDlgUploadBlackListPicture::GetExistItem(const NET_DVR_BLACK_LIST_PICTURE_CFG *lpCardCfg)
{
    int nItemCount = m_listPicture.GetItemCount();
    int i = 0;
    LPNET_DVR_BLACK_LIST_PICTURE_CFG lpTemp = NULL;
    for (i = 0; i < nItemCount; i++)
    {
        lpTemp = (LPNET_DVR_BLACK_LIST_PICTURE_CFG)m_listPicture.GetItemData(i);
        if (!lpCardCfg)
        {
            continue;
        }
        if (strcmp((char *)lpCardCfg->byCardNo, (char *)lpTemp->byCardNo) == 0 
            && strcmp((char *)lpCardCfg->byName, (char *)lpTemp->byName) == 0
            && lpCardCfg->bySex == lpTemp->bySex)
        {
            return i;
        }
    }
    return -1;
}

/** @fn  char * CDlgUploadBlackListPicture::ReadPictureFromFile(CString fileName)
 *  @brief 
 *  @param(IN) 
 *  @return  char * -- success, NULL --fail.
 */
char * CDlgUploadBlackListPicture::ReadPictureFromFile(CString fileName)
{
    //
    char *pFaceBuffer = new char[MAX_PICTURE_LEN];
    if (pFaceBuffer == NULL)
    {
        AfxMessageBox("allocate resource failed!");
        return NULL;
    }

    if (m_szPictureName == "")
    {
        AfxMessageBox("need input picture filename with full path at first!");
        return NULL;
    }

    FILE* fp = NULL;
    fp = fopen(m_szPictureName.GetBuffer(), "rb"); //ֻ����ȡ
    if (fp == NULL)
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "HPR_ReadFile failed");
        return NULL;
    }
    fseek(fp, 0L, SEEK_END);
    int len = ftell(fp);

    m_dwLen = len;
    m_dwPictureLen = len / 1024;

    if (len > MAX_PICTURE_LEN)
    {
        AfxMessageBox("The picture is too big than 512K build-in demo!");
        return NULL;
    }

    rewind(fp);
    fread(pFaceBuffer, 1, m_dwLen, fp);
    fclose(fp);
    return pFaceBuffer;
}

/** @fn  void CDlgUploadBlackListPicture::Clear()
 *  @brief ��Դ�ͷ�
 *  @return  void
 */
void CDlgUploadBlackListPicture::Clear()
{
    int nItemCount = m_listPicture.GetItemCount();
    for (int i = 0; i < nItemCount; i++)
    {
        LPNET_DVR_BLACK_LIST_PICTURE_CFG pPictureInfo = (LPNET_DVR_BLACK_LIST_PICTURE_CFG)m_listPicture.GetItemData(i);
        if (pPictureInfo != NULL && pPictureInfo->pPictureBuffer != NULL)
        {
            delete[] pPictureInfo->pPictureBuffer;
            pPictureInfo->pPictureBuffer = NULL;
        }
        if (pPictureInfo != NULL)
        {
            delete pPictureInfo;
            pPictureInfo = NULL;
        }
    }
    m_dwPictureLen = 0;
    return;
}

void CDlgUploadBlackListPicture::OnBnClickedOk()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    Clear();
    CDialogEx::OnOK();
}


void CDlgUploadBlackListPicture::OnDestroy()
{
    CDialogEx::OnDestroy();
    Clear();
    // TODO:  �ڴ˴������Ϣ����������
}


void CDlgUploadBlackListPicture::OnBnClickedButtonDelete()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CDlgUploadBlackListPicture::OnBnClickedButtonUpload();
}


void CDlgUploadBlackListPicture::OnBnClickedCheckIsdelete()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);
    if (m_bIsValid)
    {
        GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_UPLOAD)->EnableWindow(TRUE);
    }
    else
    {
        GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON_UPLOAD)->EnableWindow(FALSE);
    }
    UpdateData(FALSE);
}
