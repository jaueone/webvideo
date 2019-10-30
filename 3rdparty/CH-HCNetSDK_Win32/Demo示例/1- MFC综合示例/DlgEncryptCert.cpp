// DlgEncryptCert.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgEncryptCert.h"
extern  HINSTANCE g_hDllCalib;
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgEncryptCert dialog


CDlgEncryptCert::CDlgEncryptCert(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgEncryptCert::IDD, pParent)
	, m_szChipID(_T(""))
	, m_szDevID(_T(""))
	, m_szCARootCert(_T(""))
	, m_szEncryptCert(_T(""))
	, m_szPulbicKey(_T(""))
	, m_dwPublicKeyLen(0)
	, m_szScertKeyPair(_T(""))
	, m_szSignCert(_T(""))
	, m_lUSerID(-1)
    , m_lChannel(-1)
    , m_dwScertKeyPairLen(0)
    , m_dwEncryptCertLen(0)
    , m_dwCARootCertLen(0)
    , m_dwSignCertLen(0)
    , m_dwCAPort(0)
{
	//{{AFX_DATA_INIT(CDlgEncryptCert)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgEncryptCert::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgEncryptCert)
    DDX_Control(pDX, IDC_COM_ALGORITHM, m_cmbAlgorithm);
    DDX_Control(pDX, IDC_COM_CERT_SAVE_LOCATION, m_cmbCertSaveLocation);
    DDX_Control(pDX, IDC_COM_MODEL_LEN, m_cmbModelLen);
    DDX_Text(pDX, IDC_EDIT_CHIP_ID, m_szChipID);
    DDX_Text(pDX, IDC_EDIT_DEV_ID, m_szDevID);
    DDX_Text(pDX, IDC_EDIT_CA_ROOT_CERT, m_szCARootCert);
    DDX_Text(pDX, IDC_EDIT_ENCRYPT_CERT, m_szEncryptCert);
    DDX_Text(pDX, IDC_EDIT_PUBLIC_KEY, m_szPulbicKey);
    DDX_Text(pDX, IDC_EDIT_PUBLIC_KEY_LEN, m_dwPublicKeyLen);
    DDX_Text(pDX, IDC_EDIT_SCERT_KEY_PAIR, m_szScertKeyPair);
    DDX_Text(pDX, IDC_EDIT_SIGN_CERT, m_szSignCert);
    //}}AFX_DATA_MAP

    DDX_Text(pDX, SCERT_KEY_PAIR_LEN, m_dwScertKeyPairLen);
    DDX_Text(pDX, IDC_EDIT_ENCRYPT_CERT_LEN, m_dwEncryptCertLen);
    DDX_Text(pDX, IDC_EDIT_CA_ROOT_CERT_LEN, m_dwCARootCertLen);
    DDX_Text(pDX, IDC_EDIT_SIGN_CERT_LEN, m_dwSignCertLen);
    DDX_Text(pDX, IDC_EDIT_CA_PORT, m_dwCAPort);
    DDX_Control(pDX, IDC_CA_IP, m_ConCAIP);
}


BEGIN_MESSAGE_MAP(CDlgEncryptCert, CDialog)
	//{{AFX_MSG_MAP(CDlgEncryptCert)
    ON_BN_CLICKED(IDC_BTN_GET_DEVICE_INFO, OnBnClickedBtnGetDeviceInfo)
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBnClickedBtnExit)
	//}}AFX_MSG_MAP

	ON_BN_CLICKED(IDC_BTN_GET_CA_ROOT_CERT, &CDlgEncryptCert::OnBnClickedBtnGetCaRootCert)
	ON_BN_CLICKED(IDC_BTN_UPLOAD_CA_ROOT_CERT, &CDlgEncryptCert::OnBnClickedBtnUploadCaRootCert)
	ON_BN_CLICKED(IDC_BTN_GET_SIGNCERT, &CDlgEncryptCert::OnBnClickedBtnGetSigncert)
	ON_BN_CLICKED(IDC_BTN_UPLOAD_SIGNCERT, &CDlgEncryptCert::OnBnClickedBtnUploadSigncert)
	ON_BN_CLICKED(IDC_BTN_GET_ENCRYPT_CERT, &CDlgEncryptCert::OnBnClickedBtnGetEncryptCert)
	ON_BN_CLICKED(IDC_BTN_UPLOAD_ENCRYPT_CERT, &CDlgEncryptCert::OnBnClickedBtnUploadEncryptCert)
    ON_BN_CLICKED(IDC_BTN_INIT_CHIP, &CDlgEncryptCert::OnBnClickedBtnInitChip)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEncryptCert message handlers

BOOL CDlgEncryptCert::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  �ڴ���Ӷ���ĳ�ʼ��
    m_cmbAlgorithm.SetCurSel(1);
    m_cmbModelLen.SetCurSel(2);
    m_cmbCertSaveLocation.SetCurSel(0);
    memset(&m_struPublicKey, 0, sizeof(m_struPublicKey));
    m_dwCAPort = 9009;
    //10.2.33.24 0xa023334
    m_ConCAIP.SetAddress(167911704);
    UpdateData(FALSE);
    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣:  OCX ����ҳӦ���� FALSE
}
//��ȡ�����豸��Ϣ
void CDlgEncryptCert::OnBnClickedBtnGetDeviceInfo()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	//�����ṹ�帳ֵ
	NET_DVR_ENCRYPT_DEVICE_COND struEncryptDevCond = { 0 };
	struEncryptDevCond.dwSize = sizeof(NET_DVR_ENCRYPT_DEVICE_COND);
    struEncryptDevCond.dwChannel = m_lChannel;
	struEncryptDevCond.byAlgorithm = m_cmbAlgorithm.GetCurSel() + 1;
	struEncryptDevCond.byModelLen = m_cmbModelLen.GetCurSel() + 1;
	struEncryptDevCond.byCERTSaveLocation = m_cmbCertSaveLocation.GetCurSel() + 1;
	//�����ṹ��
	NET_DVR_ENCRYPT_DEVICE_INFO struEncryptDevInfo = { 0 };
	NET_DVR_STD_CONFIG struStdConfig = { 0 };
	struStdConfig.lpCondBuffer = &struEncryptDevCond;
	struStdConfig.dwCondSize = sizeof(NET_DVR_ENCRYPT_DEVICE_COND);
    struStdConfig.lpOutBuffer = &struEncryptDevInfo;
	struStdConfig.dwOutSize = sizeof(NET_DVR_ENCRYPT_DEVICE_INFO);
	int iRet = 0;
	char szLan[128] = { 0 };
	char szTemp[256] = { 0 };
	iRet = NET_DVR_GetSTDConfig(m_lUSerID, NET_DVR_GET_ENCRYPT_DEVICE_INFO, &struStdConfig);
	if (iRet == 1)
	{
		g_StringLanType(szLan, "��ȡ�����豸��Ϣ�ɹ�", "Get Encrypt device info  success");
		AfxMessageBox(szLan);
	}
	else
	{
		g_StringLanType(szLan, "��ȡ�����豸��Ϣʧ��", "Get Encrypt device info  failed");
		sprintf(szTemp, "%s %s %d", szLan, NET_DVR_GetErrorMsg(), NET_DVR_GetLastError());
		AfxMessageBox(szTemp);
        return;
	}
    m_dwPublicKeyLen = struEncryptDevInfo.dwPublicKeyLen;
    m_szDevID.Format("%s", struEncryptDevInfo.szDeviceID);
    m_szChipID.Format("%s", struEncryptDevInfo.szChipSerialNumber);
    m_szPulbicKey.Format("%s", (char*)struEncryptDevInfo.szPublicKey + sizeof(DWORD)+32);

    
     //������Կ�ṹ��
     m_struPublicKey.bits = *((DWORD*)struEncryptDevInfo.szPublicKey);
     memcpy(m_struPublicKey.x, ((char*)struEncryptDevInfo.szPublicKey) + sizeof(DWORD) , ECCref_MAX_LEN);
     memcpy(m_struPublicKey.y, ((char*)struEncryptDevInfo.szPublicKey) + sizeof(DWORD)+ 64, ECCref_MAX_LEN);
	UpdateData(FALSE);

}


//��ʼ��
void CDlgEncryptCert::OnBnClickedBtnInitChip()
{

    // TODO:  �ڴ���ӿؼ�֪ͨ����������

	//��ȡ��������
	if (g_hDllCalib == NULL)
	{
		return ;
	}
	//�����ӿ�
	m_fnCAInit = (CALIB_CA_Init)GetProcAddress(g_hDllCalib, "CA_Init");
	m_fnCAGetRootCert = (CALIB_CA_GetRootCert)GetProcAddress(g_hDllCalib, "CA_GetRootCert");
	m_fnCAGenUserCert = (CALIB_CA_GenUserCert)GetProcAddress(g_hDllCalib, "CA_GenUserCert");
    //��ʼ��Ca������
	
    if (!InitCA())
    {
        return;
    }
    //��ȡ���ϴ�Ca��֤��
    if (!GetUploadCARootCert())
    {
        return;
    }
    //��ȡ���ϴ�ǩ��֤��
    if (!GetUploadSignCert())
    {
        return;
    }
    //��ȡ���ϴ�����֤�顢������Կ��
    if (!GetUploadEncryptCert())
   {
        return;
   }
    char szLan[128] = { 0 };
    char szTemp[256] = { 0 };
    g_StringLanType(szLan, "��ʼ������оƬ�ɹ�!", "Init Encrypt Chip success!");
    sprintf(szTemp, "%s", szLan);
    AfxMessageBox(szTemp);

}
//��ʼ���ܹܷ�����
int CDlgEncryptCert::InitCA()
{
	
    //��ʼ���ܹܷ�����
    char szLan[128] = { 0 };
    char szTemp[256] = { 0 };
    DWORD dwCAIP = 0;
    m_ConCAIP.GetAddress(dwCAIP);
    CString strCAIP = IPToStr(dwCAIP);
    int iInitRet = m_fnCAInit(strCAIP.GetBuffer(0), m_dwCAPort);
    if (iInitRet != 0)
    {
        g_StringLanType(szLan, "��ʼ���ܹܷ�����ʧ��", "Init KMS CA fail");
        sprintf(szTemp, "%s,Errcode : %d", szLan, iInitRet);
        AfxMessageBox(szTemp);
        return FALSE;
    }
    return TRUE;
}
int CDlgEncryptCert::GetUploadCARootCert()
{
    //��ȡCA��֤��
    char szLan[128] = { 0 };
    char szTemp[128] = { 0 };
    char *pCARootCertBuf = new char[1024];
    if (pCARootCertBuf == NULL)
    {
        AfxMessageBox("pCARootCertBuf is NULL");
        return FALSE;
    }
    unsigned int dwCARootCertLen = 1024;
    int iCARet = m_fnCAGetRootCert((unsigned char*)pCARootCertBuf, &dwCARootCertLen);
    if (iCARet != 0)
    {
        g_StringLanType(szLan, "��ȡCA��֤��ʧ��", "Get CA root Cert  fail");
        sprintf(szTemp, "%s,ErrorCode:%d", szLan, iCARet);
        AfxMessageBox(szTemp);
        return FALSE;
    }
    //�ϴ�CA��֤��
    NET_DVR_ENCRYPT_CERT_PARAM struEncryptCertParam = { 0 };
    struEncryptCertParam.dwSize = sizeof(NET_DVR_ENCRYPT_CERT_PARAM);
    struEncryptCertParam.wCertType = 0;
    struEncryptCertParam.dwCertLen = dwCARootCertLen;
    struEncryptCertParam.pCertBuf = pCARootCertBuf;
    //memcpy(struEncryptCertParam.pCertBuf, m_szCARootCert, m_dwCARootCertLen);
    long lHandel = NET_DVR_UploadFile_V40(m_lUSerID, UPLOAD_ENCRYPT_CERT, &struEncryptCertParam, sizeof(struEncryptCertParam), NULL, NULL, 0);
    if (lHandel == -1)
    {
        g_StringLanType(szLan, "�ϴ�CA��֤��ʧ��", "Upload CA root Cert  failed");
        sprintf(szTemp, "%s %s %d", szLan, NET_DVR_GetErrorMsg(), NET_DVR_GetLastError());
        AfxMessageBox(szTemp);
        if (pCARootCertBuf != NULL)
        {
            delete pCARootCertBuf;
            pCARootCertBuf = NULL;
        }
        return FALSE;
    }
    else
    {
        //��ȡ�ϴ�����
        DWORD dwProcess = 0;
        long lState = NET_DVR_GetUploadState(lHandel, &dwProcess);
        while (lState != 1)
        {
            if (lState == 2)//�����ϴ�
            {
                lState = NET_DVR_GetUploadState(lHandel, &dwProcess);
                g_StringLanType(szLan, "�����ϴ�CA��֤", "Uploading CA root Cert ");
                //AfxMessageBox(szLan);
            }
            else //�ϴ�ʧ��
            {
                g_StringLanType(szLan, "�ϴ�CA��֤��ʧ��", "Upload CA root Cert  failed");
                sprintf(szTemp, "%s %s %d", szLan, NET_DVR_GetErrorMsg(), NET_DVR_GetLastError());
                AfxMessageBox(szTemp);
                if (lState == 4)
                {
                    NET_DVR_UploadClose(lHandel);
                }
                if (pCARootCertBuf != NULL)
                {
                    delete pCARootCertBuf;
                    pCARootCertBuf = NULL;
                }
                return FALSE;
            }
        }
    }
    return TRUE;
}

//************************************
/** fn:    GetUploadSignCert
 *  brief
 *  return:   int
 */
//************************************
int CDlgEncryptCert::GetUploadSignCert()
{
    char szLan[128] = { 0 };
    char szTemp[128] = { 0 };
    char *pSignCertBuf = new  char[1024];
    char * pSecretKey = new  char[1024];
    if (pSignCertBuf == NULL || pSecretKey == NULL)
    {
        AfxMessageBox("pSignCertBuf or pSecretKey is NULL");
        return FALSE;
    }
    unsigned int dwSignCertLen = 1024;
    unsigned int dwSecertKeyLen = 1024;
    int iReqType = 1;//ǩ��֤��
    int iPriType = 2;//SKF
    //�û���Ϣ
    SWCA_CERT_REQ struUserInfo = { 0 };
    memcpy(struUserInfo.CommonName, m_szDevID, strlen(m_szDevID.GetBuffer(0)));
    struUserInfo.ValidYears = 10;
    int iSignRet = m_fnCAGenUserCert(iReqType, m_cmbAlgorithm.GetCurSel() + 1, 256, iPriType, (unsigned char*)&m_struPublicKey, m_dwPublicKeyLen, (unsigned char *)&struUserInfo, \
        sizeof(struUserInfo), (unsigned char*)pSignCertBuf, &dwSignCertLen, (unsigned char*)pSecretKey, &dwSecertKeyLen);

    if (iSignRet != 0)
    {
        g_StringLanType(szLan, "��ȡǩ��֤��ʧ��", "GetSign Cert  fail");
        sprintf(szTemp, "%s,ErrorCode:%d", szLan, iSignRet);
        AfxMessageBox(szTemp);
    }
    
    NET_DVR_ENCRYPT_CERT_PARAM struEncryptCertParam = { 0 };
    struEncryptCertParam.dwSize = sizeof(NET_DVR_ENCRYPT_CERT_PARAM);
    //ǩ��֤��
    struEncryptCertParam.wCertType = 1;
    struEncryptCertParam.dwCertLen = dwSignCertLen;
    struEncryptCertParam.pCertBuf = pSignCertBuf;
    long lHandel = NET_DVR_UploadFile_V40(m_lUSerID, UPLOAD_ENCRYPT_CERT, &struEncryptCertParam, sizeof(struEncryptCertParam), NULL, NULL, 0);
    if (lHandel == -1)
    {
        g_StringLanType(szLan, "�ϴ�ǩ��֤��ʧ��", "Upload Sign Cert  failed");
        sprintf(szTemp, "%s %s %d", szLan, NET_DVR_GetErrorMsg(), NET_DVR_GetLastError());
        AfxMessageBox(szTemp);
        return FALSE;
    }
    else
    {
        //��ȡ�ϴ�����
        DWORD dwProcess = 0;
        long lState = NET_DVR_GetUploadState(lHandel, &dwProcess);
        while (lState != 1)
        {
            if (lState == 2)//�����ϴ�
            {
                lState = NET_DVR_GetUploadState(lHandel, &dwProcess);
                g_StringLanType(szLan, "�����ϴ�ǩ��֤��", "Uploading  sign Cert ");
                //AfxMessageBox(szLan);
            }
            else //�ϴ�ʧ��
            {
                g_StringLanType(szLan, "�ϴ�ǩ��֤��ʧ��", "Upload sign Cert  failed");
                sprintf(szTemp, "%s %s %d", szLan, NET_DVR_GetErrorMsg(), NET_DVR_GetLastError());
                AfxMessageBox(szTemp);
                if (lState == 4)
                {
                    NET_DVR_UploadClose(lHandel);
                }
                if (pSignCertBuf != NULL)
                {
                    delete pSignCertBuf;
                    pSignCertBuf = NULL;
                }
                if (pSecretKey != NULL)
                {
                    delete pSecretKey;
                    pSecretKey = NULL;
                }
                return FALSE;
            }
        }
        //�ϴ��ɹ�
        NET_DVR_UploadClose(lHandel);
    }
    if (pSignCertBuf != NULL)
    {
        delete pSignCertBuf;
        pSignCertBuf = NULL;
    }
    if (pSecretKey != NULL)
    {
        delete pSecretKey;
        pSecretKey = NULL;
    }
    return TRUE;
    UpdateData(FALSE);
}

int CDlgEncryptCert::GetUploadEncryptCert()
{
    char szLan[128] = { 0 };
    char szTemp[128] = { 0 };
    char *pEncryptCertBuf = new char[1024];
    char * pSecretKey = new  char[1024];
    if (pEncryptCertBuf == NULL || pSecretKey == NULL)
    {
        AfxMessageBox("pSignCertBuf or pSecretKey is NULL");
        return FALSE;
    }
    unsigned int dwEncryptCertLen = 1024;
    unsigned int dwSecertKeyLen = 1024;
    int iReqType = 2;//����֤��
    int iPriType = 2;//SKF
    //�û���Ϣ
    SWCA_CERT_REQ struUserInfo = { 0 };
    memcpy(struUserInfo.CommonName, m_szDevID, strlen(m_szDevID.GetBuffer(0)));
    struUserInfo.ValidYears = 10;
    int iEncryptRet = m_fnCAGenUserCert(iReqType, m_cmbAlgorithm.GetCurSel() + 1, 256, iPriType, (unsigned char*)&m_struPublicKey, m_dwPublicKeyLen, (unsigned char *)&struUserInfo, \
        sizeof(struUserInfo), (unsigned char*)pEncryptCertBuf, &dwEncryptCertLen, (unsigned char*)pSecretKey, &dwSecertKeyLen);
 
    if (iEncryptRet != 0)
    {
        g_StringLanType(szLan, "��ȡ����֤��ʧ��", "Get Encrypt  Cert  fail");
        sprintf(szTemp, "%s,ErrorCode:%d", szLan, iEncryptRet);
        AfxMessageBox(szTemp);
    }

    NET_DVR_ENCRYPT_CERT_PARAM struEncryptCertParam = { 0 };
    struEncryptCertParam.dwSize = sizeof(NET_DVR_ENCRYPT_CERT_PARAM);
    //����֤��
    struEncryptCertParam.wCertType = 2;
    struEncryptCertParam.dwCertLen = dwEncryptCertLen;
    struEncryptCertParam.pCertBuf = pEncryptCertBuf;
    memcpy(struEncryptCertParam.pCertBuf, m_szEncryptCert, m_dwEncryptCertLen);
    long lHandel = NET_DVR_UploadFile_V40(m_lUSerID, UPLOAD_ENCRYPT_CERT, &struEncryptCertParam, sizeof(struEncryptCertParam), NULL, NULL, 0);
    if (lHandel == -1)
    {
        g_StringLanType(szLan, "�ϴ�����֤��ʧ��", "Upload Encrypt Cert  failed");
        sprintf(szTemp, "%s %s %d", szLan, NET_DVR_GetErrorMsg(), NET_DVR_GetLastError());
        AfxMessageBox(szTemp);
        return FALSE;
    }
    else
    {
        //��ȡ�ϴ�����
        DWORD dwProcess = 0;
        long lState = NET_DVR_GetUploadState(lHandel, &dwProcess);
        while (lState != 1)
        {
            if (lState == 2)//�����ϴ�
            {
                lState = NET_DVR_GetUploadState(lHandel, &dwProcess);
                g_StringLanType(szLan, "�����ϴ�����֤��", "Uploading  Encrypt Cert ");
                //AfxMessageBox(szLan);
            }
            else //�ϴ�ʧ��
            {
                g_StringLanType(szLan, "�ϴ�����֤��ʧ��", "Upload Encrypt Cert  failed");
                sprintf(szTemp, "%s %s %d", szLan, NET_DVR_GetErrorMsg(), NET_DVR_GetLastError());
                AfxMessageBox(szTemp);
                if (lState == 4)
                {
                    NET_DVR_UploadClose(lHandel);
                }
                if (pEncryptCertBuf != NULL)
                {
                    delete pEncryptCertBuf;
                    pEncryptCertBuf = NULL;
                }
                if (pSecretKey != NULL)
                {
                    delete pSecretKey;
                    pSecretKey = NULL;
                }
                return FALSE;
            }
        }
        //�ϴ��ɹ�
        NET_DVR_UploadClose(lHandel);

    }

    //�ϴ�������Կ
    char *pTempScertKey = new char[2024];
    memset(pTempScertKey, 1, 2024);
    struEncryptCertParam.wCertType = 3;
    struEncryptCertParam.dwCertLen = dwSecertKeyLen;
    struEncryptCertParam.pCertBuf = pSecretKey;
    lHandel = NET_DVR_UploadFile_V40(m_lUSerID, UPLOAD_ENCRYPT_CERT, &struEncryptCertParam, sizeof(struEncryptCertParam), NULL, NULL, 0);
    if (lHandel == -1)
    {
        g_StringLanType(szLan, "�ϴ�������Կ��ʧ��", "Upload Encrypt Secret key pair  failed");
        sprintf(szTemp, "%s %s %d", szLan, NET_DVR_GetErrorMsg(), NET_DVR_GetLastError());
        AfxMessageBox(szTemp);
        if (pEncryptCertBuf != NULL)
        {
            delete pEncryptCertBuf;
            pEncryptCertBuf = NULL;
        }
        if (pSecretKey != NULL)
        {
            delete pSecretKey;
            pSecretKey = NULL;
        }
        return FALSE;
    }
    else
    {
        //��ȡ�ϴ�����
        DWORD dwProcess = 0;
        long lState = NET_DVR_GetUploadState(lHandel, &dwProcess);
        while (lState != 1)
        {
            if (lState == 2)//�����ϴ�
            {
                lState = NET_DVR_GetUploadState(lHandel, &dwProcess);
                g_StringLanType(szLan, "�����ϴ�������Կ��", "Uploading  Encrypt Secret key pair ");
                //AfxMessageBox(szLan);
            }
            else //�ϴ�ʧ��
            {
                g_StringLanType(szLan, "�ϴ�������Կ��ʧ��", "Upload Encrypt Secret key pair  failed");
                sprintf(szTemp, "%s %s %d", szLan, NET_DVR_GetErrorMsg(), NET_DVR_GetLastError());
                AfxMessageBox(szTemp);
                if (lState == 4)
                {
                    NET_DVR_UploadClose(lHandel);
                }
                if (pEncryptCertBuf != NULL)
                {
                    delete pEncryptCertBuf;
                    pEncryptCertBuf = NULL;
                }
                if (pSecretKey != NULL)
                {
                    delete pSecretKey;
                    pSecretKey = NULL;
                }
                return FALSE;
            }
        }
    }
     
    //�ϴ��ɹ� �ͷ��ڴ�
    if (pEncryptCertBuf != NULL)
    {
        delete pEncryptCertBuf;
        pEncryptCertBuf = NULL;
    }
    if (pSecretKey != NULL)
    {
        delete pSecretKey;
        pSecretKey = NULL;
    }
    UpdateData(FALSE);
    return TRUE;
}

//�˳�
void CDlgEncryptCert::OnBnClickedBtnExit()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CDialog::OnCancel();

}



//��ȡ��֤��
void CDlgEncryptCert::OnBnClickedBtnGetCaRootCert()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
    char szLan[128] = { 0 };
    char szTemp[128] = { 0 };
    char *pCARootCertBuf = new char[1024];
    if (pCARootCertBuf == NULL)
    {
        AfxMessageBox("pCARootCertBuf is NULL");
        return;
    }
    unsigned int dwCARootCertLen = 1024;

    int iCARet = m_fnCAGetRootCert((unsigned char*)pCARootCertBuf, &dwCARootCertLen);
    if (iCARet == 0)
    {
        g_StringLanType(szLan, "��ȡCA��֤��ɹ�", "Get CA root Cert  success");
        AfxMessageBox(szLan);
        m_szCARootCert.Format("%s", pCARootCertBuf);
       // sprintf(m_szCARootCert.GetBuffer(0), "%s", pCARootCertBuf);
        //memcpy(m_szCaRootCert, pCARootCertBuf, dwCARootCertLen);
        m_dwCARootCertLen = dwCARootCertLen;
        if (pCARootCertBuf != NULL)
        {
            delete pCARootCertBuf;
            pCARootCertBuf = NULL;
        }
    }
    else
    {
        g_StringLanType(szLan, "��ȡCA��֤��ʧ��", "Get CA root Cert  fail");
        sprintf(szTemp, "%s,ErrorCode:%d", szLan, iCARet);
        AfxMessageBox(szTemp);
    }
    UpdateData(FALSE);

}

//�ϴ�CA��֤��
void CDlgEncryptCert::OnBnClickedBtnUploadCaRootCert()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
    char *pTempCert = new char[1024];
    memset(pTempCert, 0, 1024);
    NET_DVR_ENCRYPT_CERT_PARAM struEncryptCertParam = {0};
	struEncryptCertParam.dwSize = sizeof(NET_DVR_ENCRYPT_CERT_PARAM);
    struEncryptCertParam.wCertType = 0;
	struEncryptCertParam.dwCertLen = m_dwCARootCertLen;
    struEncryptCertParam.pCertBuf = pTempCert;
    memcpy(struEncryptCertParam.pCertBuf, m_szCARootCert, m_dwCARootCertLen);
	char szLan[128] = { 0 };
	char szTemp[128] = { 0 };
	long lHandel = NET_DVR_UploadFile_V40(m_lUSerID, UPLOAD_ENCRYPT_CERT, &struEncryptCertParam, sizeof(struEncryptCertParam), NULL,NULL, 0);
    if (lHandel == -1)
	{
        g_StringLanType(szLan, "�ϴ�CA��֤��ʧ��", "Upload CA root Cert  failed");
        sprintf(szTemp, "%s %s %d", szLan, NET_DVR_GetErrorMsg(), NET_DVR_GetLastError());
        //AfxMessageBox(szTemp);
        return;
	}
	else
	{
        //��ȡ�ϴ�����
        DWORD dwProcess = 0;
        long lState = NET_DVR_GetUploadState(lHandel, &dwProcess);
        while (lState != 1)
        {
             if (lState == 2)//�����ϴ�
             {
                 lState = NET_DVR_GetUploadState(lHandel, &dwProcess);
                 g_StringLanType(szLan, "�����ϴ�CA��֤", "Uploading CA root Cert ");
                 //AfxMessageBox(szLan);
             }
             else //�ϴ�ʧ��
             {
                 g_StringLanType(szLan, "�ϴ�CA��֤��ʧ��", "Upload CA root Cert  failed");
                 sprintf(szTemp, "%s %s %d", szLan, NET_DVR_GetErrorMsg(), NET_DVR_GetLastError());
                 AfxMessageBox(szTemp);
                 if (lState == 4)
                 {
                     NET_DVR_UploadClose(lHandel);
                 }
                 if (pTempCert != NULL)
                 {
                     delete pTempCert;
                     pTempCert = NULL;
                 }
                 return;
             }
        }
        //�ϴ��ɹ�
        g_StringLanType(szLan, "�ϴ�CA��֤��ɹ�", "Upload CA root Cert  success");
        AfxMessageBox(szLan);
        NET_DVR_UploadClose(lHandel);
        if (struEncryptCertParam.pCertBuf != NULL)
        {
            delete struEncryptCertParam.pCertBuf;
            struEncryptCertParam.pCertBuf = NULL;
        }
	}
}

//��ȡǩ��֤��
void CDlgEncryptCert::OnBnClickedBtnGetSigncert()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
    char szLan[128] = { 0 };
    char szTemp[128] = { 0 };
    char *pSignCertBuf = new  char[1024];
     char * pSecretKey = new  char[1024];
    if (pSignCertBuf == NULL || pSecretKey ==NULL)
    {
        AfxMessageBox("pSignCertBuf or pSecretKey is NULL");
        return;
    }
    unsigned int dwSignCertLen = 1024;
    unsigned int dwSecertKeyLen = 1024;
    int iReqType = 1;//ǩ��֤��
    int iPriType = 2;//SKF
    //�û���Ϣ
    SWCA_CERT_REQ struUserInfo = { 0 };
    memcpy(struUserInfo.CommonName, m_szDevID, strlen(m_szDevID.GetBuffer(0)));
    struUserInfo.ValidYears = 10;
    int iSignRet = m_fnCAGenUserCert(iReqType, m_cmbAlgorithm.GetCurSel() + 1, 256, iPriType, (unsigned char*)&m_struPublicKey, m_dwPublicKeyLen, (unsigned char *)&struUserInfo, \
        sizeof(struUserInfo), (unsigned char*)pSignCertBuf, &dwSignCertLen, (unsigned char*)pSecretKey, &dwSecertKeyLen);
    if (iSignRet == 0)
    {
        g_StringLanType(szLan, "��ȡǩ��֤��ɹ�", "Get Sign Cert  success");
        AfxMessageBox(szLan);
       m_szSignCert.Format("%s", pSignCertBuf);
        m_dwSignCertLen = dwSignCertLen;
    }
    else
    {
        g_StringLanType(szLan, "��ȡǩ��֤��ʧ��", "GetSign Cert  fail");
        sprintf(szTemp, "%s,ErrorCode:%d", szLan, iSignRet);
        AfxMessageBox(szTemp);
    }
    if (pSignCertBuf != NULL)
    {
        delete pSignCertBuf;
        pSignCertBuf = NULL;
    }
    if (pSecretKey != NULL)
    {
        delete pSecretKey;
        pSecretKey = NULL;
    }
    UpdateData(FALSE);
}

//�ϴ�ǩ��֤��
void CDlgEncryptCert::OnBnClickedBtnUploadSigncert()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);
    char *pTempCert = new char[1024];
    memset(pTempCert, 1, 1024);
    NET_DVR_ENCRYPT_CERT_PARAM struEncryptCertParam = { 0 };
    struEncryptCertParam.dwSize = sizeof(NET_DVR_ENCRYPT_CERT_PARAM);
    //ǩ��֤��
    struEncryptCertParam.wCertType = 1;
    struEncryptCertParam.dwCertLen = m_dwSignCertLen;
    struEncryptCertParam.pCertBuf = pTempCert;
    memcpy(struEncryptCertParam.pCertBuf, m_szSignCert, m_dwSignCertLen);
    char szLan[128] = { 0 };
    char szTemp[128] = { 0 };
    long lHandel = NET_DVR_UploadFile_V40(m_lUSerID, UPLOAD_ENCRYPT_CERT, &struEncryptCertParam, sizeof(struEncryptCertParam), NULL, NULL, 0);
    if (lHandel == -1)
    {
        g_StringLanType(szLan, "�ϴ�ǩ��֤��ʧ��", "Upload Sign Cert  failed");
        sprintf(szTemp, "%s %s %d", szLan, NET_DVR_GetErrorMsg(), NET_DVR_GetLastError());
        AfxMessageBox(szTemp);
        return;
    }
    else
    {
        //��ȡ�ϴ�����
        DWORD dwProcess = 0;
        long lState = NET_DVR_GetUploadState(lHandel, &dwProcess);
        while (lState != 1)
        {
            if (lState == 2)//�����ϴ�
            {
                lState = NET_DVR_GetUploadState(lHandel, &dwProcess);
                g_StringLanType(szLan, "�����ϴ�ǩ��֤��", "Uploading  sign Cert ");
                //AfxMessageBox(szLan);
            }
            else //�ϴ�ʧ��
            {
                g_StringLanType(szLan, "�ϴ�ǩ��֤��ʧ��", "Upload sign Cert  failed");
                sprintf(szTemp, "%s %s %d", szLan, NET_DVR_GetErrorMsg(), NET_DVR_GetLastError());
                AfxMessageBox(szTemp);
                if (lState == 4)
                {
                    NET_DVR_UploadClose(lHandel);
                }
                if (pTempCert != NULL)
                {
                    delete pTempCert;
                    pTempCert = NULL;
                }
                return;
            }
        }
        //�ϴ��ɹ�
        g_StringLanType(szLan, "�ϴ�ǩ��֤��ɹ�", "Upload sign Cert  success");
        AfxMessageBox(szLan);
        NET_DVR_UploadClose(lHandel);
        if (pTempCert != NULL)
        {
            delete pTempCert;
            pTempCert = NULL;
        }
    }
}

//��ȡ����֤��
void CDlgEncryptCert::OnBnClickedBtnGetEncryptCert()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
    char szLan[128] = { 0 };
    char szTemp[128] = { 0 };
     char *pEncryptCertBuf = new char[1024];
     unsigned char * pSecretKey = new unsigned char[1024];
    if (pEncryptCertBuf == NULL || pSecretKey == NULL)
    {
        AfxMessageBox("pSignCertBuf or pSecretKey is NULL");
        return;
    }
    unsigned int dwEncryptCertLen = 1024;
    unsigned int dwSecertKeyLen = 1024;
    int iReqType = 2;//����֤��
    int iPriType = 2;//SKF
    //�û���Ϣ
    SWCA_CERT_REQ struUserInfo = { 0 };
    memcpy(struUserInfo.CommonName, m_szDevID, strlen(m_szDevID.GetBuffer(0)));
    struUserInfo.ValidYears = 10;
    int iEncryptRet = m_fnCAGenUserCert(iReqType, m_cmbAlgorithm.GetCurSel() + 1, 256, iPriType,(unsigned char*)&m_struPublicKey, m_dwPublicKeyLen, (unsigned char *)&struUserInfo, \
        sizeof(struUserInfo), (unsigned char*)pEncryptCertBuf, &dwEncryptCertLen, pSecretKey, &dwSecertKeyLen);
    if (iEncryptRet == 0)
    {
        g_StringLanType(szLan, "��ȡ����֤��ɹ�", "Get Encrypt Cert  success");
        AfxMessageBox(szLan);
        m_szEncryptCert.Format("%s", pEncryptCertBuf);
        m_dwEncryptCertLen = dwEncryptCertLen;
        m_szScertKeyPair.Format("%s", pSecretKey);
        memset(m_ScertKeyPair, 0, 1024);
        memcpy(m_ScertKeyPair, pSecretKey, dwSecertKeyLen);
        m_dwScertKeyPairLen = dwSecertKeyLen;
        
    }
    else
    {
        g_StringLanType(szLan, "��ȡ����֤��ʧ��", "Get Encrypt  Cert  fail");
        sprintf(szTemp, "%s,ErrorCode:%d", szLan, iEncryptRet);
        AfxMessageBox(szTemp);
    }
    //�ͷ��ڴ�
    if (pEncryptCertBuf != NULL)
    {
        delete pEncryptCertBuf;
        pEncryptCertBuf = NULL;
    }
    if (pSecretKey != NULL)
    {
        delete pSecretKey;
        pSecretKey = NULL;
    }
    UpdateData(FALSE);
}

//�ϴ�����֤��
void CDlgEncryptCert::OnBnClickedBtnUploadEncryptCert()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);
    char *pTempCert = new char[2024];
    memset(pTempCert, 1, 2024);
    NET_DVR_ENCRYPT_CERT_PARAM struEncryptCertParam = { 0 };
    struEncryptCertParam.dwSize = sizeof(NET_DVR_ENCRYPT_CERT_PARAM);
    //����֤��
    struEncryptCertParam.wCertType = 2;
    struEncryptCertParam.dwCertLen = m_dwEncryptCertLen;
    struEncryptCertParam.pCertBuf = pTempCert;
    memcpy(struEncryptCertParam.pCertBuf, m_szEncryptCert, m_dwEncryptCertLen);
    char szLan[128] = { 0 };
    char szTemp[128] = { 0 };
    long lHandel = NET_DVR_UploadFile_V40(m_lUSerID, UPLOAD_ENCRYPT_CERT, &struEncryptCertParam, sizeof(struEncryptCertParam), NULL, NULL, 0);
    if (lHandel == -1)
    {
        g_StringLanType(szLan, "�ϴ�����֤��ʧ��", "Upload Encrypt Cert  failed");
        sprintf(szTemp, "%s %s %d", szLan, NET_DVR_GetErrorMsg(), NET_DVR_GetLastError());
        AfxMessageBox(szTemp);
        return;
    }
    else
    {
        //��ȡ�ϴ�����
        DWORD dwProcess = 0;
        long lState = NET_DVR_GetUploadState(lHandel, &dwProcess);
        while (lState != 1)
        {
            if (lState == 2)//�����ϴ�
            {
                lState = NET_DVR_GetUploadState(lHandel, &dwProcess);
                g_StringLanType(szLan, "�����ϴ�����֤��", "Uploading  Encrypt Cert ");
                //AfxMessageBox(szLan);
            }
            else //�ϴ�ʧ��
            {
                g_StringLanType(szLan, "�ϴ�����֤��ʧ��", "Upload Encrypt Cert  failed");
                sprintf(szTemp, "%s %s %d", szLan, NET_DVR_GetErrorMsg(), NET_DVR_GetLastError());
                AfxMessageBox(szTemp);
                if (lState == 4)
                {
                    NET_DVR_UploadClose(lHandel);
                }
                if (pTempCert != NULL)
                {
                    delete pTempCert;
                    pTempCert = NULL;
                }
                return;
            }
        }
        //�ϴ��ɹ�
        g_StringLanType(szLan, "�ϴ�����֤��ɹ�", "Upload Encrypt Cert  success");
        AfxMessageBox(szLan);
        NET_DVR_UploadClose(lHandel);

    }

    //�ϴ�������Կ
    char *pTempScertKey = new char[2024];
    memset(pTempScertKey, 1, 2024);
    struEncryptCertParam.wCertType = 3;
    struEncryptCertParam.dwCertLen = m_dwScertKeyPairLen;
    struEncryptCertParam.pCertBuf = pTempScertKey;
    memcpy(struEncryptCertParam.pCertBuf, m_ScertKeyPair, m_dwScertKeyPairLen);
    lHandel = NET_DVR_UploadFile_V40(m_lUSerID, UPLOAD_ENCRYPT_CERT, &struEncryptCertParam, sizeof(struEncryptCertParam), NULL, NULL, 0);
    if (lHandel == -1)
    {
        g_StringLanType(szLan, "�ϴ�������Կ��ʧ��", "Upload Encrypt Secret key pair  failed");
        sprintf(szTemp, "%s %s %d", szLan, NET_DVR_GetErrorMsg(), NET_DVR_GetLastError());
        AfxMessageBox(szTemp);
        if (pTempCert != NULL)
        {
            delete pTempCert;
            pTempCert = NULL;
        }
        return;
    }
    else
    {
        //��ȡ�ϴ�����
        DWORD dwProcess = 0;
        long lState = NET_DVR_GetUploadState(lHandel, &dwProcess);
        while (lState != 1)
        {
            if (lState == 2)//�����ϴ�
            {
                lState = NET_DVR_GetUploadState(lHandel, &dwProcess);
                g_StringLanType(szLan, "�����ϴ�������Կ��", "Uploading  Encrypt Secret key pair ");
                //AfxMessageBox(szLan);
            }
            else //�ϴ�ʧ��
            {
                g_StringLanType(szLan, "�ϴ�������Կ��ʧ��", "Upload Encrypt Secret key pair  failed");
                sprintf(szTemp, "%s %s %d", szLan, NET_DVR_GetErrorMsg(), NET_DVR_GetLastError());
                AfxMessageBox(szTemp);
                if (lState == 4)
                {
                    NET_DVR_UploadClose(lHandel);
                }
                if (pTempScertKey != NULL)
                {
                    delete pTempScertKey;
                    pTempScertKey = NULL;
                }
                return;
            }
        }
        //�ϴ��ɹ�
        g_StringLanType(szLan, "�ϴ�������Կ�Գɹ�", "Upload Encrypt Secret key pair  success");
        AfxMessageBox(szLan);
        NET_DVR_UploadClose(lHandel);
        if (pTempScertKey != NULL)
        {
            delete pTempScertKey;
            pTempScertKey = NULL;
        }
        if (pTempCert != NULL)
        {
            delete pTempCert;
            pTempCert = NULL;
        }
    }
}




