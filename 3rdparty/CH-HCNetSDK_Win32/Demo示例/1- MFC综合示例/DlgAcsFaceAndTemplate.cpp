// DlgAcsFaceAndTemplate.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ClientDemo.h"
#include "DlgAcsFaceAndTemplate.h"
#include "afxdialogex.h"
#include "DlgSelectDevice.h"

// DlgAcsFaceAndTemplate �Ի���
#define MAX_PICTURE_LEN 200 * 1024
#define MAX_TEMPLATE_LEN 4 * 1024
#define WM_MSG_SET_FACE_FINISH 1002
#define WM_MSG_GET_FACE_FINISH 1003
#define WM_MSG_ADD_FACE_TOLIST 1004

IMPLEMENT_DYNAMIC(DlgAcsFaceAndTemplate, CDialog)

DlgAcsFaceAndTemplate::DlgAcsFaceAndTemplate(CWnd* pParent /*=NULL*/)
	: CDialog(DlgAcsFaceAndTemplate::IDD, pParent)
    , m_csCardNo(_T(""))
    , m_csFacePath(_T(""))
    , m_csTemplatePath(_T(""))
    , m_dwFaceNum(0)
{
    m_dwDataLen = 0;
    m_lSetDataCfgHandle = -1;
    m_lGetDataCfgHandle = -1;
}

DlgAcsFaceAndTemplate::~DlgAcsFaceAndTemplate()
{
}

void DlgAcsFaceAndTemplate::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_FACE_TEMPALTE_CFG, m_listFaceAndTemplate);
    DDX_Text(pDX, IDC_EDIT_CARD, m_csCardNo);
    DDX_Text(pDX, IDC_EDIT_FACE_PATH, m_csFacePath);
    DDX_Text(pDX, IDC_EDIT_TEMPLATE_ADDRESS, m_csTemplatePath);
    DDX_Text(pDX, IDC_EDIT_FACE_NUMBER, m_dwFaceNum);
    DDX_Control(pDX, IDC_PROGRESS, m_Progress);
}


BEGIN_MESSAGE_MAP(DlgAcsFaceAndTemplate, CDialog)
    ON_BN_CLICKED(IDC_BUTTON_ADD, &DlgAcsFaceAndTemplate::OnBnClickedButtonAdd)
    ON_BN_CLICKED(IDCANCEL, &DlgAcsFaceAndTemplate::OnBnClickedCancel)
    ON_BN_CLICKED(IDC_BUTTON_STOP, &DlgAcsFaceAndTemplate::OnBnClickedButtonStop)
    ON_BN_CLICKED(IDC_BUTTON_GET, &DlgAcsFaceAndTemplate::OnBnClickedButtonGet)
    ON_BN_CLICKED(IDC_BUTTON_SET, &DlgAcsFaceAndTemplate::OnBnClickedButtonSet)
    ON_BN_CLICKED(IDC_BUTTON_BROWSE_FACE, &DlgAcsFaceAndTemplate::OnBnClickedButtonBrowseFace)
    ON_BN_CLICKED(IDC_BUTTON_BROWSE_TEMPLATE, &DlgAcsFaceAndTemplate::OnBnClickedButtonBrowseTemplate)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_FACE_TEMPALTE_CFG, &DlgAcsFaceAndTemplate::OnLvnItemchangedListFaceTempalteCfg)
    ON_MESSAGE(WM_MSG_SET_FACE_FINISH, &DlgAcsFaceAndTemplate::OnMsgSetFaceFinish)
    ON_MESSAGE(WM_MSG_GET_FACE_FINISH, &DlgAcsFaceAndTemplate::OnMsgGetFaceFinish)
    ON_MESSAGE(WM_MSG_ADD_FACE_TOLIST, &DlgAcsFaceAndTemplate::OnMsgAddFaceTolist)
    ON_BN_CLICKED(IDC_BTN_GET_ALL, &DlgAcsFaceAndTemplate::OnBnClickedBtnGetAll)
    ON_BN_CLICKED(IDC_BTN_SET_ALL, &DlgAcsFaceAndTemplate::OnBnClickedBtnSetAll)
END_MESSAGE_MAP()

LRESULT DlgAcsFaceAndTemplate::OnMsgSetFaceFinish(WPARAM wParam, LPARAM lParam)
{
    if (!NET_DVR_StopRemoteConfig(m_lSetDataCfgHandle))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_StopRemoteConfig failed");
        return 0;
    }
    m_lSetDataCfgHandle = -1;
    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_SET_FACE_AND_TEMPLATE Set finish");
    return 0;
}

LRESULT DlgAcsFaceAndTemplate::OnMsgGetFaceFinish(WPARAM wParam, LPARAM lParam)
{
    NET_DVR_StopRemoteConfig(m_lGetDataCfgHandle);
    m_lGetDataCfgHandle = -1;
    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_GET_FACE_AND_TEMPLATE Get finish");
    return 0;
}

LRESULT DlgAcsFaceAndTemplate::OnMsgAddFaceTolist(WPARAM wParam, LPARAM lParam)
{
    LPNET_DVR_FACE_AND_TEMPLATE_CFG lpCardCfg = (LPNET_DVR_FACE_AND_TEMPLATE_CFG)wParam;
    if (lpCardCfg->dwSize == 0)
    {
        return 0;
    }

    LPNET_DVR_FACE_AND_TEMPLATE_CFG pCardInfo = NULL;
    int iItemIndex = GetExistItem(lpCardCfg);
    if (iItemIndex == -1)
    {
        pCardInfo = new NET_DVR_FACE_AND_TEMPLATE_CFG;
        if (pCardInfo == NULL)
        {
            return 0;
        }
        int iSize = sizeof(NET_DVR_FACE_AND_TEMPLATE_CFG);
        int nItemCount = m_listFaceAndTemplate.GetItemCount();

        CString strItem = "";
        strItem.Format("%d", nItemCount + 1);
        iItemIndex = m_listFaceAndTemplate.InsertItem(nItemCount, strItem);
        m_listFaceAndTemplate.SetItemData(nItemCount, (DWORD)pCardInfo);
    }
    else
    {
        pCardInfo = (LPNET_DVR_FACE_AND_TEMPLATE_CFG)m_listFaceAndTemplate.GetItemData(iItemIndex);
    }

    memcpy(pCardInfo, lpCardCfg, sizeof(*lpCardCfg));

    char szLan[1024] = { 0 };

    sprintf(szLan, "%d", iItemIndex);
    sprintf(szLan, "%s", pCardInfo->byCardNo);
    m_listFaceAndTemplate.SetItemText(iItemIndex, 1, szLan);
    sprintf(szLan, "%s", m_mFacePaths[pCardInfo->byCardNo]);
    m_listFaceAndTemplate.SetItemText(iItemIndex, 2, szLan);
    sprintf(szLan, "%s", m_mTemplatePaths[pCardInfo->byCardNo]);
    m_listFaceAndTemplate.SetItemText(iItemIndex, 3, szLan);

    delete lpCardCfg;
    UpdateData(FALSE);

    CString str;
    str.Format("%d", m_listFaceAndTemplate.GetItemCount());
    GetDlgItem(IDC_STATIC_GET)->SetWindowText(str);
    return 0;
}

void CALLBACK g_fSetFaceCallback(DWORD dwType, void* lpBuffer, DWORD dwBufLen, void* pUserData)
{
    DlgAcsFaceAndTemplate* pDlg = (DlgAcsFaceAndTemplate*)pUserData;
    if (pDlg == NULL)
    {
        return;
    }
    pDlg->ProcessFaceCallbackData(dwType, lpBuffer, dwBufLen);
}

void DlgAcsFaceAndTemplate::ProcessFaceCallbackData(DWORD dwType, void* lpBuffer, DWORD dwBufLen)
{
    if (dwType == NET_SDK_CALLBACK_TYPE_STATUS)
    {
        DWORD dwStatus = *(DWORD*)lpBuffer;
        if (dwStatus == NET_SDK_CALLBACK_STATUS_PROCESSING)
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "Set PROCESSING %d", m_dwSendIndex);
            
            SendNextPicture();
        }
        else if (dwStatus == NET_SDK_CALLBACK_STATUS_FAILED)
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "Set Err:NET_SDK_CALLBACK_STATUS_FAILED %d", m_dwSendIndex);

            SendNextPicture();
        }
        //���������رճ�����
        else if (dwStatus == NET_SDK_CALLBACK_STATUS_SUCCESS)
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "Set SUCCESS");
            PostMessage(WM_MSG_SET_FACE_FINISH, 0, 0);
        }
        else if (dwStatus == NET_SDK_CALLBACK_STATUS_EXCEPTION)
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "Set Exception");
            PostMessage(WM_MSG_SET_FACE_FINISH, 0, 0);
        }
        else
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "Set SUCCESS");
            PostMessage(WM_MSG_SET_FACE_FINISH, 0, 0);
        }
    }
    else if (dwType == NET_SDK_CALLBACK_TYPE_DATA)
    {
        NET_DVR_FACE_AND_TEMPLATE_STATUS struCfg = { 0 };
        memcpy(&struCfg, lpBuffer, sizeof(struCfg));

        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "Set PROCESSING,Set Status:%d", struCfg.byRecvStatus);
        
        SendNextPicture();
    }
}

void CALLBACK g_fGetFaceCallback(DWORD dwType, void* lpBuffer, DWORD dwBufLen, void* pUserData)
{
    DlgAcsFaceAndTemplate* pDlg = (DlgAcsFaceAndTemplate*)pUserData;
    if (pDlg == NULL)
    {
        return;
    }
    pDlg->ProcessGetFaceCallbackData(dwType, lpBuffer, dwBufLen);
}

void DlgAcsFaceAndTemplate::ProcessGetFaceCallbackData(DWORD dwType, void* lpBuffer, DWORD dwBufLen)
{
    if (dwType == NET_SDK_CALLBACK_TYPE_DATA)
    {
        LPNET_DVR_FACE_AND_TEMPLATE_CFG lpCardCfg = new NET_DVR_FACE_AND_TEMPLATE_CFG;
        memset(lpCardCfg, 0, sizeof(NET_DVR_FACE_AND_TEMPLATE_CFG));
        memcpy(lpCardCfg, lpBuffer, sizeof(*lpCardCfg));

        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "Get PROCESSING %s", lpCardCfg->byCardNo);

        FILE* fp = NULL;
        char szPath[1024] = { 0 };
        if (lpCardCfg->dwFaceLen != 0)
        {
            sprintf(szPath, "%s_facePicture.jpg", lpCardCfg->byCardNo);
            m_mFacePaths[lpCardCfg->byCardNo] = szPath;
            fp = fopen(szPath, "wb");
            if (fp == NULL)
            {
                g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "fopen write failed");
            }
            else
            {
                fwrite(lpCardCfg->pFaceBuffer, lpCardCfg->dwFaceLen, 1, fp);
                fclose(fp);
                lpCardCfg->pFaceBuffer = NULL;
            }
        }
        
        if (lpCardCfg->dwFaceTemplateLen != 0)
        {
            sprintf(szPath, "%s_faceTemplate.dat", lpCardCfg->byCardNo);
            m_mTemplatePaths[lpCardCfg->byCardNo] = szPath;
            fp = fopen(szPath, "wb");
            if (fp == NULL)
            {
                g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "fopen write failed");
            }
            else
            {
                fwrite(lpCardCfg->pFaceTemplateBuffer, lpCardCfg->dwFaceTemplateLen, 1, fp);
                fclose(fp);
                lpCardCfg->pFaceTemplateBuffer = NULL;
            }
        }
        PostMessage(WM_MSG_ADD_FACE_TOLIST, (WPARAM)lpCardCfg, 0);
    }
    else if (dwType == NET_SDK_CALLBACK_TYPE_STATUS)
    {
        DWORD dwStatus = *(DWORD*)lpBuffer;
        if (dwStatus == NET_SDK_CALLBACK_STATUS_SUCCESS)
        {
            PostMessage(WM_MSG_GET_FACE_FINISH, 0, 0);
        }
        else if (dwStatus == NET_SDK_CALLBACK_STATUS_FAILED)
        {
            //0-��ʾδ֪����
            DWORD dwErrCode = 0;
            if (dwBufLen == 8)
            {
                dwErrCode = *(DWORD*)((char *)lpBuffer + 4);
            }
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "GetFace STATUS_FAILED, Error code %d", dwErrCode);
        }
    }
}

// DlgAcsFaceAndTemplate ��Ϣ�������
/** @fn  char * DlgAcsFaceAndTemplate::ReadDataFromFile(CString fileName)
*  @brief
*  @param(IN)
*  @return  char * -- success, NULL --fail.
*/
char * DlgAcsFaceAndTemplate::ReadDataFromFile(CString fileName, DATA_TYPE enumDataType)
{
    int bufferLen = 0;
    m_dwDataLen = 0;
    if (enumDataType == FACE)
    {
        bufferLen = MAX_PICTURE_LEN;
    }
    else if (enumDataType == TEMPLATE)
    {
        bufferLen = MAX_TEMPLATE_LEN;
    }
    else
    {
        return NULL;
    }

    char *pDataBuffer = new char[bufferLen];
    if (pDataBuffer == NULL)
    {
        AfxMessageBox("allocate resource failed!");
        return NULL;
    }

    if (fileName == "")
    {
        AfxMessageBox("need input picture filename with full path at first!");
        return NULL;
    }

    FILE* fp = NULL;
    fp = fopen(fileName.GetBuffer(), "rb"); //ֻ����ȡ
    if (fp == NULL)
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "HPR_ReadFile failed");
        return NULL;
    }
    fseek(fp, 0L, SEEK_END);
    int len = ftell(fp);

    m_dwDataLen = len;

    if (len > bufferLen)
    {
        char szPrompt[256] = { 0 };
        sprintf_s(szPrompt, "The picture is too big than %d bytes build-in sdk!", bufferLen);
        AfxMessageBox(szPrompt);
        return NULL;
    }

    rewind(fp);
    fread(pDataBuffer, 1, m_dwDataLen, fp);
    fclose(fp);
    return pDataBuffer;
}

/** @fn  DlgAcsFaceAndTemplate::GetExistItem(const NET_DVR_FACE_AND_TEMPLATE_CFG *lpCheckFacePictureCfg)
*  @brief ���б��в����Ƿ������ͬ��ʵ��
*  @param(IN) NET_DVR_CHECK_FACE_PICTURE_CFG *lpCheckFacePictureCfg
*  @return  index--the index, -1--no duplicate.
*/
int DlgAcsFaceAndTemplate::GetExistItem(const NET_DVR_FACE_AND_TEMPLATE_CFG *lpDataCfg)
{
    int nItemCount = m_listFaceAndTemplate.GetItemCount();
    int i = 0;
    LPNET_DVR_FACE_AND_TEMPLATE_CFG lpTemp = NULL;
    for (i = 0; i < nItemCount; i++)
    {
        lpTemp = (LPNET_DVR_FACE_AND_TEMPLATE_CFG)m_listFaceAndTemplate.GetItemData(i);
        if (!lpTemp)
        {
            continue;
        }
        if (!strcmp((char *)lpTemp->byCardNo, (char *)lpDataCfg->byCardNo))
        {
            return i;
        }
    }
    return -1;
}

void DlgAcsFaceAndTemplate::OnBnClickedButtonAdd()
{
    UpdateData(TRUE);

    //1.get data from control
    NET_DVR_FACE_AND_TEMPLATE_CFG struFaceAndTemplateCfg = { 0 };

    struFaceAndTemplateCfg.dwSize = sizeof(struFaceAndTemplateCfg);
    struFaceAndTemplateCfg.pFaceBuffer = ReadDataFromFile(m_csFacePath, FACE);
    struFaceAndTemplateCfg.dwFaceLen = m_dwDataLen;
    struFaceAndTemplateCfg.pFaceTemplateBuffer = ReadDataFromFile(m_csTemplatePath, TEMPLATE);
    struFaceAndTemplateCfg.dwFaceTemplateLen = m_dwDataLen;
    memcpy(struFaceAndTemplateCfg.byCardNo, m_csCardNo.GetBuffer(m_csCardNo.GetLength()), m_csCardNo.GetLength());

    if (struFaceAndTemplateCfg.pFaceBuffer == NULL && struFaceAndTemplateCfg.pFaceTemplateBuffer == NULL)
    {
        return;
    }

    //2.is duplicated?
    LPNET_DVR_FACE_AND_TEMPLATE_CFG pFaceAndTemplateInfo = NULL;
    int iItemIndex = GetExistItem(&struFaceAndTemplateCfg);
    if (iItemIndex == -1)
    {
        pFaceAndTemplateInfo = new NET_DVR_FACE_AND_TEMPLATE_CFG;
        if (pFaceAndTemplateInfo == NULL)
        {
            return;
        }
        int iSize = sizeof(NET_DVR_FACE_AND_TEMPLATE_CFG);
        int nItemCount = m_listFaceAndTemplate.GetItemCount();

        CString strItem = "";
        strItem.Format("%d", nItemCount + 1);
        iItemIndex = m_listFaceAndTemplate.InsertItem(nItemCount, strItem);
        m_listFaceAndTemplate.SetItemData(nItemCount, (DWORD)pFaceAndTemplateInfo);
    }
    else
    {
        pFaceAndTemplateInfo = (LPNET_DVR_FACE_AND_TEMPLATE_CFG)m_listFaceAndTemplate.GetItemData(iItemIndex);
    }
    memcpy(pFaceAndTemplateInfo, &struFaceAndTemplateCfg, sizeof(struFaceAndTemplateCfg));

    //3.set to list
    char szLan[1024] = { 0 };

    m_mFacePaths[m_csCardNo] = m_csFacePath;
    m_mTemplatePaths[m_csCardNo] = m_csTemplatePath;

    sprintf(szLan, "%d", iItemIndex);
    sprintf(szLan, "%s", struFaceAndTemplateCfg.byCardNo);
    m_listFaceAndTemplate.SetItemText(iItemIndex, 1, szLan);
    sprintf(szLan, "%s", m_csFacePath);
    m_listFaceAndTemplate.SetItemText(iItemIndex, 2, szLan);
    sprintf(szLan, "%s", m_csTemplatePath);
    m_listFaceAndTemplate.SetItemText(iItemIndex, 3, szLan);
    UpdateData(FALSE);
    return;
}


void DlgAcsFaceAndTemplate::OnBnClickedCancel()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    Clear();
    CDialog::OnOK();
}


void DlgAcsFaceAndTemplate::OnBnClickedButtonStop()
{
    UpdateData(TRUE);
    Clear();
    m_listFaceAndTemplate.DeleteAllItems();
    m_Progress.SetPos(0);
    UpdateData(FALSE);
}

/** @fn  void DlgAcsFaceAndTemplate::Clear()
*  @brief ��Դ�ͷ�
*  @return  void
*/
void DlgAcsFaceAndTemplate::Clear()
{   
    int nItemCount = m_listFaceAndTemplate.GetItemCount();
    for (int i = 0; i < nItemCount; i++)
    {
        LPNET_DVR_FACE_AND_TEMPLATE_CFG pFaceAndTemplateInfo = (LPNET_DVR_FACE_AND_TEMPLATE_CFG)m_listFaceAndTemplate.GetItemData(i);
        if (pFaceAndTemplateInfo != NULL && pFaceAndTemplateInfo->pFaceBuffer != NULL)
        {
            delete[] pFaceAndTemplateInfo->pFaceBuffer;
            pFaceAndTemplateInfo->pFaceBuffer = NULL;
        }

        if (pFaceAndTemplateInfo != NULL && pFaceAndTemplateInfo->pFaceTemplateBuffer != NULL)
        {
            delete[] pFaceAndTemplateInfo->pFaceTemplateBuffer;
            pFaceAndTemplateInfo->pFaceTemplateBuffer = NULL;
        }

        if (pFaceAndTemplateInfo != NULL)
        {
            delete pFaceAndTemplateInfo;
            pFaceAndTemplateInfo = NULL;
        }
    }
    m_mFacePaths.clear();
    m_mTemplatePaths.clear();
    return;
}

void DlgAcsFaceAndTemplate::ClearFaceAndTemplate(int sendIndex)
{
    LPNET_DVR_FACE_AND_TEMPLATE_CFG pFaceAndTemplateInfo = (LPNET_DVR_FACE_AND_TEMPLATE_CFG)m_listFaceAndTemplate.GetItemData(sendIndex);
    if (pFaceAndTemplateInfo != NULL && pFaceAndTemplateInfo->pFaceBuffer != NULL)
    {
        delete[] pFaceAndTemplateInfo->pFaceBuffer;
        pFaceAndTemplateInfo->pFaceBuffer = NULL;
    }

    if (pFaceAndTemplateInfo != NULL && pFaceAndTemplateInfo->pFaceTemplateBuffer != NULL)
    {
        delete[] pFaceAndTemplateInfo->pFaceTemplateBuffer;
        pFaceAndTemplateInfo->pFaceTemplateBuffer = NULL;
    }

    return;
}

void DlgAcsFaceAndTemplate::OnBnClickedButtonGet()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������

    if (m_lGetDataCfgHandle != -1)
    {
        NET_DVR_StopRemoteConfig(m_lGetDataCfgHandle);
    }
    Clear();
    m_Progress.SetPos(0);
    m_listFaceAndTemplate.DeleteAllItems();
    UpdateData(TRUE);
    NET_DVR_FACE_AND_TEMPLATE_COND struCond = { 0 };
    struCond.dwSize = sizeof(struCond);
    struCond.dwFaceNum = m_dwFaceNum;
    memcpy(struCond.byCardNo, m_csCardNo.GetBuffer(m_csCardNo.GetLength()), m_csCardNo.GetLength());

    m_lGetDataCfgHandle = NET_DVR_StartRemoteConfig(m_lServerID, NET_DVR_GET_FACE_AND_TEMPLATE, &struCond, sizeof(struCond), g_fGetFaceCallback, this);
    if (m_lGetDataCfgHandle == -1)
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_GET_FACE_AND_TEMPLATE failed");
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_GET_FACE_AND_TEMPLATE success");
    }

    return;
}

/** @fn BOOL SendFirstPicture()
*  @brief У���һ������ͼƬ
*  @return  HPR_TRUE--success, HPR_FALSE--fail.
*/
BOOL DlgAcsFaceAndTemplate::SendFirstPicture()
{
    int nItemCount = m_listFaceAndTemplate.GetItemCount();

    if (nItemCount <= 0)
    {
        AfxMessageBox("Please add info to list at first");
        return FALSE;
    }
    m_dwSendIndex = 0;
    //��ȡ��һ������ͼƬ��������

    LPNET_DVR_FACE_AND_TEMPLATE_CFG pPictureInfo = (LPNET_DVR_FACE_AND_TEMPLATE_CFG)m_listFaceAndTemplate.GetItemData(m_dwSendIndex);

    if (pPictureInfo == NULL)
    {
        return FALSE;
    }
    ClearFaceAndTemplate(m_dwSendIndex);
    pPictureInfo->dwSize = sizeof(*pPictureInfo);
    CString facePath = m_listFaceAndTemplate.GetItemText(m_dwSendIndex, 2);
    CString templatePath = m_listFaceAndTemplate.GetItemText(m_dwSendIndex, 3);
    CString cardId = m_listFaceAndTemplate.GetItemText(m_dwSendIndex, 1);
    pPictureInfo->pFaceBuffer = ReadDataFromFile(facePath, FACE);
    pPictureInfo->dwFaceLen = m_dwDataLen;
    pPictureInfo->pFaceTemplateBuffer = ReadDataFromFile(templatePath, TEMPLATE);
    pPictureInfo->dwFaceTemplateLen = m_dwDataLen;
    memcpy(pPictureInfo->byCardNo, cardId.GetBuffer(cardId.GetLength()), cardId.GetLength());

    if (!NET_DVR_SendRemoteConfig(m_lSetDataCfgHandle, ENUM_ACS_SEND_DATA, (char *)pPictureInfo, sizeof(*pPictureInfo)))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "Send Fail,Card No: %s", pPictureInfo->byCardNo);
        ClearFaceAndTemplate(m_dwSendIndex);
        return FALSE;
    }
    ClearFaceAndTemplate(m_dwSendIndex);
    return TRUE;
}

/** @fn BOOL SendNextPicture()
*  @brief ������һ������
*  @return  HPR_TRUE--success, HPR_FALSE--fail.
*/
BOOL DlgAcsFaceAndTemplate::SendNextPicture()
{
    int nItemCount = m_listFaceAndTemplate.GetItemCount();

    if (nItemCount < 0)
    {
        AfxMessageBox("Please add info to list at first");
    }

    m_dwSendIndex++;
    m_Progress.SetPos(m_dwSendIndex);
    CString str;
    str.Format("%d", m_dwSendIndex);
    GetDlgItem(IDC_STATIC_SET)->SetWindowText(str);
    //�Ѿ�������
    if (m_dwSendIndex >= nItemCount || m_dwSendIndex >= m_dwFaceNum)
    {
        return TRUE;
    }

    LPNET_DVR_FACE_AND_TEMPLATE_CFG pPictureInfo = (LPNET_DVR_FACE_AND_TEMPLATE_CFG)m_listFaceAndTemplate.GetItemData(m_dwSendIndex);

    if (pPictureInfo == NULL)
    {
        return FALSE;
    }
    ClearFaceAndTemplate(m_dwSendIndex);
    pPictureInfo->dwSize = sizeof(*pPictureInfo);
    CString facePath = m_listFaceAndTemplate.GetItemText(m_dwSendIndex, 2);
    CString templatePath = m_listFaceAndTemplate.GetItemText(m_dwSendIndex, 3);
    CString cardId = m_listFaceAndTemplate.GetItemText(m_dwSendIndex, 1);
    pPictureInfo->pFaceBuffer = ReadDataFromFile(facePath, FACE);
    pPictureInfo->dwFaceLen = m_dwDataLen;
    pPictureInfo->pFaceTemplateBuffer = ReadDataFromFile(templatePath, TEMPLATE);
    pPictureInfo->dwFaceTemplateLen = m_dwDataLen;
    memcpy(pPictureInfo->byCardNo, cardId.GetBuffer(cardId.GetLength()), cardId.GetLength());
   
    if (!NET_DVR_SendRemoteConfig(m_lSetDataCfgHandle, ENUM_ACS_SEND_DATA, (char *)pPictureInfo, sizeof(*pPictureInfo)))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "Send Fail,Card No: %s", pPictureInfo->byCardNo);
        PostMessage(WM_MSG_SET_FACE_FINISH, 0, 0);
        ClearFaceAndTemplate(m_dwSendIndex);
        return FALSE;
    }
    ClearFaceAndTemplate(m_dwSendIndex);
    return TRUE;
}

void DlgAcsFaceAndTemplate::OnBnClickedButtonSet()
{
    if (m_lSetDataCfgHandle != -1)
    {
        NET_DVR_StopRemoteConfig(m_lSetDataCfgHandle);
    }
    UpdateData(TRUE);
    NET_DVR_FACE_AND_TEMPLATE_COND struCond = { 0 };
    struCond.dwSize = sizeof(struCond);
    struCond.dwFaceNum = m_dwFaceNum;
    if (m_dwFaceNum > m_listFaceAndTemplate.GetItemCount())
    {
        return;
    }
    m_lSetDataCfgHandle = NET_DVR_StartRemoteConfig(m_lServerID, NET_DVR_SET_FACE_AND_TEMPLATE, &struCond, sizeof(struCond), g_fSetFaceCallback, this);
    if (m_lSetDataCfgHandle == -1)
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_SET_FACE_AND_TEMPLATE failed");
        return;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_SET_FACE_AND_TEMPLATE success");
        m_Progress.SetRange32(0, struCond.dwFaceNum);
        CString str;
        str.Format("%d", m_dwFaceNum);
        GetDlgItem(IDC_STATIC_SETALL)->SetWindowText(str);
    }

    //���͵�һ��, ʧ�ܹر�����
    m_dwSendIndex = 0;
    if (!SendFirstPicture())
    {
        NET_DVR_StopRemoteConfig(m_lSetDataCfgHandle);
        m_lSetDataCfgHandle = -1;
    }
}


BOOL DlgAcsFaceAndTemplate::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  �ڴ���Ӷ���ĳ�ʼ��
    char szLan[128] = { 0 };

    m_listFaceAndTemplate.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
    g_StringLanType(szLan, "���", "No");
    m_listFaceAndTemplate.InsertColumn(0, szLan, LVCFMT_LEFT, 80, -1);
    g_StringLanType(szLan, "������������", "Card No");
    m_listFaceAndTemplate.InsertColumn(1, szLan, LVCFMT_LEFT, 130, -1);
    g_StringLanType(szLan, "��������·��", "Face Data Path");
    m_listFaceAndTemplate.InsertColumn(2, szLan, LVCFMT_LEFT, 160, -1);
    g_StringLanType(szLan, "ģ������·��", "Template Path");
    m_listFaceAndTemplate.InsertColumn(3, szLan, LVCFMT_LEFT, 160, -1);
    
    selectedID = -1;
    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣:  OCX ����ҳӦ���� FALSE
}


void DlgAcsFaceAndTemplate::OnBnClickedButtonBrowseFace()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CString strFilter = _T("All Files(*.*)|*.*||");
    CFileDialog dlg(TRUE, "*.*", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter);
    if (dlg.DoModal() != IDOK)
    {
        return;
    }
    m_csFacePath = dlg.GetPathName();
    GetDlgItem(IDC_EDIT_FACE_PATH)->SetWindowText(m_csFacePath);
}


void DlgAcsFaceAndTemplate::OnBnClickedButtonBrowseTemplate()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CString strFilter = _T("All Files(*.*)|*.*||");
    CFileDialog dlg(TRUE, "*.*", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter);
    if (dlg.DoModal() != IDOK)
    {
        return;
    }
    m_csTemplatePath = dlg.GetPathName();
    GetDlgItem(IDC_EDIT_TEMPLATE_ADDRESS)->SetWindowText(m_csTemplatePath);
}


void DlgAcsFaceAndTemplate::OnLvnItemchangedListFaceTempalteCfg(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    // TODO:  �ڴ���ӿؼ�֪ͨ����������

    if (LVIF_STATE == pNMLV->uChanged && (pNMLV->uNewState & LVIS_SELECTED))
    {
        int iItemIndex = pNMLV->iItem;
        if (-1 != iItemIndex)
        {
            LPNET_DVR_FACE_PARAM_CFG pCardInfo = (LPNET_DVR_FACE_PARAM_CFG)m_listFaceAndTemplate.GetItemData(iItemIndex);
            m_csCardNo = pCardInfo->byCardNo;
            m_csFacePath = m_mFacePaths[pCardInfo->byCardNo];
            m_csTemplatePath = m_mTemplatePaths[pCardInfo->byCardNo];
            UpdateData(FALSE);
        }
    }

    *pResult = 0;
}


void DlgAcsFaceAndTemplate::OnBnClickedBtnGetAll()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    if (m_lGetDataCfgHandle != -1)
    {
        NET_DVR_StopRemoteConfig(m_lGetDataCfgHandle);
    }
    Clear();
    m_Progress.SetPos(0);
    m_listFaceAndTemplate.DeleteAllItems();
    UpdateData(TRUE);
    NET_DVR_FACE_AND_TEMPLATE_COND struCond = {0};
    struCond.dwSize = sizeof(struCond);
    struCond.dwFaceNum = 0xffffffff;

    m_lGetDataCfgHandle = NET_DVR_StartRemoteConfig(m_lServerID, NET_DVR_GET_FACE_AND_TEMPLATE, &struCond, sizeof(struCond), g_fGetFaceCallback, this);
    if (m_lGetDataCfgHandle == -1)
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_GET_FACE_AND_TEMPLATE failed");
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_GET_FACE_AND_TEMPLATE success");
    }
    return;
}

/** @fn  void DlgAcsFaceAndTemplate::OnBnClickedBtnSetAll()
*  @brief ѡ���豸���·�����������ģ����Ϣ
*  @return  void
*/
void DlgAcsFaceAndTemplate::OnBnClickedBtnSetAll()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);
    CDlgSelectDevice dlg;
    dlg.DoModal();
    m_Progress.SetPos(0);
    if (m_lSetDataCfgHandle != -1)
    {
        NET_DVR_StopRemoteConfig(m_lSetDataCfgHandle);
    }
    if (selectedID == -1)
    {
        //selectedID����-1ʱ������ȫ���·�ʱû��ѡ���豸�����ߵ����ȡ��
        return;
    }
    NET_DVR_FACE_AND_TEMPLATE_COND struCond = { 0 };
    struCond.dwSize = sizeof(struCond);
    m_dwFaceNum = m_listFaceAndTemplate.GetItemCount();
    struCond.dwFaceNum = m_dwFaceNum;
    m_lSetDataCfgHandle = NET_DVR_StartRemoteConfig(selectedID, NET_DVR_SET_FACE_AND_TEMPLATE, &struCond, sizeof(struCond), g_fSetFaceCallback, this);
    if (m_lSetDataCfgHandle == -1)
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_SET_FACE_AND_TEMPLATE failed");
        return;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_SET_FACE_AND_TEMPLATE success");
        m_Progress.SetRange32(0, struCond.dwFaceNum);
        CString str;
        str.Format("%d", m_dwFaceNum);
        GetDlgItem(IDC_STATIC_SETALL)->SetWindowText(str);
    }

    //���͵�һ����ʧ�ܹر�����
    m_dwSendIndex = 0;
    if (!SendFirstPicture())
    {
        NET_DVR_StopRemoteConfig(m_lSetDataCfgHandle);
        m_lSetDataCfgHandle = -1;
    }
    UpdateData(FALSE);
}
