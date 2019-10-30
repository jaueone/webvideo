// DlgPicBatchUpload.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ClientDemo.h"
#include "DlgPicBatchUpload.h"
#include "afxdialogex.h"


// CDlgPicBatchUpload �Ի���

IMPLEMENT_DYNAMIC(CDlgPicBatchUpload, CDialog)

CDlgPicBatchUpload::CDlgPicBatchUpload(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPicBatchUpload::IDD, pParent)
    , m_strPicPath(_T(""))
    , m_strPicCommenState(_T(""))
    , m_strPicSingleState(_T(""))
{
    m_lUserID = -1;
    m_iDeviceIndex = -1;
    m_lUploadPicHandle = -1;
    m_hPicUpLoadThread = NULL;
    m_bStopUploadPic = FALSE;
    memset(&m_struPicRet, 0, sizeof(m_struPicRet));
    m_strPicPath = _T("c:/Picture");
    m_strPicCommenState = _T("");
    m_strPicSingleState = _T("");
}

CDlgPicBatchUpload::~CDlgPicBatchUpload()
{
}

void CDlgPicBatchUpload::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_PIC_PATH, m_strPicPath);
    DDX_Text(pDX, IDC_STATIC_PIC_COMMEN_STATE, m_strPicCommenState);
    DDX_Text(pDX, IDC_STATIC_PIC_SINGLE_STATE, m_strPicSingleState);
}


BEGIN_MESSAGE_MAP(CDlgPicBatchUpload, CDialog)
    ON_BN_CLICKED(IDC_BTN_UPLOAD_PIC, &CDlgPicBatchUpload::OnBnClickedBtnUploadPic)
    ON_BN_CLICKED(IDC_BTN_UPLOAD_PIC_STOP, &CDlgPicBatchUpload::OnBnClickedBtnUploadPicStop)
END_MESSAGE_MAP()


// CDlgPicBatchUpload ��Ϣ�������

BOOL CDlgPicBatchUpload::OnInitDialog()
{
    CDialog::OnInitDialog();

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgPicBatchUpload::OnBnClickedBtnUploadPic()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);

    m_lUploadPicHandle = NET_DVR_UploadFile_V40(m_lUserID, BATCH_UPLOAD_PICTURE_FILE, NULL, 0, NULL, NULL, 0);
    if (m_lUploadPicHandle < 0)
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "BATCH_UPLOAD_PICTURE_FILE");
        return;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "BATCH_UPLOAD_PICTURE_FILE");
    }
    m_bStopUploadPic = FALSE;
    DWORD dwThreadID = 0;
    m_hPicUpLoadThread = CreateThread(NULL, 0, LPTHREAD_START_ROUTINE(PicUpLoadThread), this, 0, &dwThreadID);
    if (m_hPicUpLoadThread == NULL)
    {
        char szLan[128] = { 0 };
        g_StringLanType(szLan, "���ϴ�ͼƬ�߳�ʧ��!", "open UpLoad picture thread Fail!");
        AfxMessageBox(szLan);
        return;
    }
}


void CDlgPicBatchUpload::OnBnClickedBtnUploadPicStop()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
}

DWORD WINAPI CDlgPicBatchUpload::PicUpLoadThread(LPVOID pParam)
{
    CDlgPicBatchUpload *pThis = (CDlgPicBatchUpload*)pParam;
    CFileFind finder;
    if (!finder.FindFile(pThis->m_strPicPath + "\\*.*"))
    {
        pThis->SetDlgItemText(IDC_STATIC_PIC_SINGLE_STATE, "���ļ���ʧ��");
        return 0;
    }
    FILE *pFile = NULL;
    BYTE byPicType = 0;
    BOOL bFindFile = TRUE;
    while (bFindFile)   //һ��ͼƬһ��ѭ��
    {
        if (pThis->m_bStopUploadPic)
        {
            break;
        }
        bFindFile = finder.FindNextFile();
        if (finder.IsDots())
        {
            continue;
        }
        else if (finder.IsDirectory())
        {
            continue;
        }
        else
        {
            CString strFilePath = finder.GetFilePath();
            CString strFileName = finder.GetFileName();
            if (strFileName.Find(".jpg") != -1 || strFileName.Find(".JPG") != -1)
            {
                byPicType = 1;
            }
            else if (strFileName.Find(".bmp") != -1 || strFileName.Find(".BMP") != -1)
            {
                byPicType = 2;
            }
            else if (strFileName.Find(".png") != -1 || strFileName.Find(".PNG") != -1)
            {
                byPicType = 3;
            }
            else
            {
                continue;
            }
            if (!pThis->PicUpload(strFileName, strFilePath, byPicType))
            {
                break;
            }
        }

        if (!bFindFile)
        {
            pThis->SetDlgItemText(IDC_STATIC_PIC_SINGLE_STATE, "���޸����ļ�");
            break;
        }
    }
    finder.Close();
    return 0;
}

BOOL CDlgPicBatchUpload::PicUpload(CString strFileName, CString strFilePath, BYTE byPicType)
{
    FILE* pFile;
    pFile = fopen(strFilePath.GetBuffer(0), "rb");
    if (pFile == NULL)
    {
        return FALSE;
    }

    long curpos, length;
    curpos = ftell(pFile);
    fseek(pFile, 0L, SEEK_END);
    length = ftell(pFile);
    fseek(pFile, curpos, SEEK_SET);

    BYTE *pBuf = new BYTE[length];
    memset(pBuf, 0, length);
    fread(pBuf, length, 1, pFile);
    fclose(pFile);

    NET_DVR_SEND_PARAM_IN   struParamIn = { 0 };
    struParamIn.byPicType = byPicType;
    SYSTEMTIME sysTime = { 0 };
    GetLocalTime(&sysTime);
    struParamIn.struTime.wYear = sysTime.wYear;
    struParamIn.struTime.byMonth = sysTime.wMonth;
    struParamIn.struTime.byDay = sysTime.wDay;
    struParamIn.struTime.byHour = sysTime.wHour;
    struParamIn.struTime.byMinute = sysTime.wMinute;
    struParamIn.struTime.bySecond = sysTime.wSecond;
    struParamIn.struTime.wMilliSec = sysTime.wMilliseconds;
    struParamIn.pSendData = pBuf;
    struParamIn.dwSendDataLen = length;
    if (NET_DVR_UploadSend(m_lUploadPicHandle, &struParamIn, NULL) < 0)
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_UploadSend");
        delete[] pBuf;
        pBuf = NULL;
        return FALSE;
    }

    DWORD dwProgress = 0;
    int iStatus = -1;
    char szStatus[256] = { 0 };
    BOOL bRet = FALSE;
    while (1)
    {
        if (m_bStopUploadPic)
        {
            break;
        }
        memset(szStatus, 0, 256);
        iStatus = NET_DVR_GetUploadState(m_lUploadPicHandle, &dwProgress);
        if (iStatus == -1)  //����ڷ���ʧ��
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_GetUploadState");
            break;
        }
        else if (iStatus == 2) //�����ϴ�
        {
            sprintf(szStatus, "File[%s],status[%d],progress[%d]", strFileName.GetBuffer(0), iStatus, dwProgress);
            SetDlgItemText(IDC_STATIC_PIC_SINGLE_STATE, szStatus);
            Sleep(100);
            continue;
        }
        else if (iStatus == 1) //�ϴ��ɹ�
        {
            sprintf(szStatus, "File[%s],status[%d],progress[%d]", strFileName.GetBuffer(0), iStatus, dwProgress);
            SetDlgItemText(IDC_STATIC_PIC_SINGLE_STATE, szStatus);
            memset(&m_struPicRet, 0, sizeof(m_struPicRet));
            NET_DVR_GetUploadResult(m_lUploadPicHandle, &m_struPicRet, sizeof(m_struPicRet));
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, (char*)m_struPicRet.sUrl);
            Sleep(100); //���sleepֻ��Ϊ��addlog�ܹ���ʱ��ִ��
            bRet = TRUE;
            break;
        }
        else //�ϴ�ʧ��,��ʾ״̬
        {
            sprintf(szStatus, "File[%s],status[%d],progress[%d]", strFileName.GetBuffer(0), iStatus, dwProgress);
            SetDlgItemText(IDC_STATIC_PIC_SINGLE_STATE, szStatus);
            break;
        }
    }
    delete[] pBuf;
    pBuf = NULL;

    return bRet;
}
