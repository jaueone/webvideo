// DlgDebug.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgDebug.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDebug dialog


CDlgDebug::CDlgDebug(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDebug::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDebug)
	m_csDebugInfo = _T("");
	m_csDebugCMD = _T("");
	//}}AFX_DATA_INIT
    m_lHandle = -1;
    memset(&m_sDebugInfo, 0, sizeof(m_sDebugInfo));
    memset(&m_struDebugCMD, 0, sizeof(m_struDebugCMD));
    memset(&m_sAllDebugInfo, 0, sizeof(m_sAllDebugInfo));
}

CDlgDebug::~CDlgDebug()
{
    if (m_lHandle != -1)
    {
        NET_DVR_StopRemoteConfig(m_lHandle);
        m_lHandle = -1;
       
	}
}


void CDlgDebug::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDebug)
//	DDX_Text(pDX, IDC_EDIT_DEBUG_INFO, m_csDebugInfo);
	DDX_Text(pDX, IDC_EDIT_DEBUGCMD, m_csDebugCMD);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDebug, CDialog)
	//{{AFX_MSG_MAP(CDlgDebug)
	ON_BN_CLICKED(IDOK, OnOk)
    ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDebug message handlers
void CALLBACK g_fDebugInfoCallback(DWORD dwType, void* lpBuffer, DWORD dwBufLen, void* pUserData)
{
    CDlgDebug* pDlg = (CDlgDebug*)pUserData;
    if (pDlg == NULL)
    {
        return;
    }
    char szLan[128] = {0};
    if (dwType == NET_SDK_CALLBACK_TYPE_STATUS)
    {
        DWORD dwStatus = *(DWORD *)lpBuffer;
        if (dwStatus == NET_SDK_CALLBACK_STATUS_SUCCESS)
        {
            sprintf(pDlg->m_sDebugInfo, "设备发送信息成功!");
        }
        else if(dwStatus == NET_SDK_CALLBACK_STATUS_PROCESSING)
        {
            
            sprintf(pDlg->m_sDebugInfo, "设备发送信息处理中!");
 
        }
        else if(dwStatus == NET_SDK_CALLBACK_STATUS_FAILED)
        {
//             char* pBuf = new char[dwBufLen]; 
//             memcpy(pBuf, /*(char*)*/lpBuffer, dwBufLen);
//             char* pBufTemp = pBuf;
//             pBufTemp += 4;
//             int num = (dwBufLen-4)/4;
//             if (num > 0)
//             {
//                 for (int i = 0; i < num; i++)
//                 {
//                     if (*(DWORD*)(pBufTemp) != 0)
//                     {
//                         sprintf(szLan, "第 %d 组数据的错误码是: %d", i+1, *(DWORD*)(pBufTemp));
//                         sprintf(pDlg->m_sDebugInfo, "%s", (char*)szLan);
//                     }
//                     pBufTemp += 4;
//                 }
//             } 
//             else
//             {
                sprintf(pDlg->m_sDebugInfo, "接收设备信息失败!");
//            }
        }
        else if(dwStatus == NET_SDK_CALLBACK_STATUS_EXCEPTION)
        {
            sprintf(pDlg->m_sDebugInfo, "设备返回异常!");
            
        }
    }
    else if(dwType == NET_SDK_CALLBACK_TYPE_DATA)
    {
        char* pBuf = new char[dwBufLen]; 
        memcpy(pBuf, /*(char*)*/lpBuffer, dwBufLen);
        char* pBufTemp = pBuf;
        
        if (NULL != pBufTemp)
        {
            LPNET_DVR_DEBUG_INFO pDebugInfo = (LPNET_DVR_DEBUG_INFO)pBufTemp;
            sprintf(pDlg->m_sDebugInfo, "调试信息为: %s", pDebugInfo->szDebugInfo);
            CTime LogInfo = CTime::GetCurrentTime();
            //CString cLogInfo=
            sprintf(pDlg->m_sLogInfo, "%s ", LogInfo.Format("%y-%m-%d %H:%M:%S").GetBuffer(0));
            strcat(pDlg->m_sLogInfo, pDebugInfo->szDebugInfo);
            strcat(pDlg->m_sLogInfo, "\r\n");
            CFile LogFile;
            if (!LogFile.Open(_T(pDlg->m_cFileName), CFile::modeWrite | CFile::modeNoTruncate | CFile::modeCreate))
            {

                return;
            }
            else
            {
                LogFile.SeekToEnd();
                LogFile.Write(pDlg->m_sLogInfo, strlen(pDlg->m_sLogInfo));
                memset(&pDlg->m_sLogInfo, 0, sizeof(pDlg->m_sLogInfo));
                //LogFile.Flush();
                LogFile.Close();
            }
        }          
        else
        {
            sprintf(pDlg->m_sDebugInfo, "无调试信息");
        }
        
    }
    return;

    
    //pDlg->m_csDebugInfo.Format("%s", (char*)lpBuffer);
}

BOOL CDlgDebug::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    CString csPath(TEXT("c:\\SdkLog"));
    if (!PathIsDirectory(csPath))
    {
        CreateDirectory(csPath, NULL);

    }
    CTime CurrentTime = CTime::GetCurrentTime();
    /*char sTime[64] = {};
    sprintf(sTime, "%s", CurrentTime.Format("%y-%m-%d %H:%M:%S").GetBuffer(0));*/
    CString FilePath;
    CString FileName;
    FilePath = _T("c:\\SdkLog\\");
    CString Final = "设备调试.txt";
    FileName = CurrentTime.Format("20%y.%m.%d_%H.%M.%S").GetBuffer(0) + Final;
    FileName = FilePath + FileName;
    m_cFileName = FileName;//更新类中的文件名
    CFile file;

    if (!file.Open(_T(FileName), CFile::modeCreate | CFile::modeWrite))
    {
        MessageBox("保存文件失败");
        return FALSE;
    }
    file.Close();

    if (m_lHandle != -1)
    {
        NET_DVR_StopRemoteConfig(m_lHandle);
        m_lHandle = -1;
        
	}

    m_lHandle = NET_DVR_StartRemoteConfig(m_lServerID,NET_DVR_DEBUGINFO_START, NULL, 0, g_fDebugInfoCallback,this);
    if (m_lHandle == -1)
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_DEBUGINFO_START");
        return FALSE;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_DEBUGINFO_START");
	}

    SetTimer(NULL,100,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CDlgDebug::OnOk() 
{
	// TODO: Add your control notification handler code here
	if (GetDlgItem(IDC_EDIT_DEBUGCMD) == GetFocus())
	{
        UpdateData(TRUE);
        memset(&m_struDebugCMD, 0, sizeof(m_struDebugCMD));
        m_struDebugCMD.dwSize =  sizeof(m_struDebugCMD);
        strncpy(m_struDebugCMD.szDebugCMD, m_csDebugCMD, strlen(m_csDebugCMD));
        
        if (m_lHandle == -1)
        {
            g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "长连接句柄无效: %d", m_lHandle);
            return; 
        }
        if (!NET_DVR_SendRemoteConfig(m_lHandle, 6, (char*)&m_struDebugCMD ,sizeof(m_struDebugCMD)))
        {
            g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "调试命令发送失败!"); 
            return ; 
        }
        else
        {
            g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_SendRemoteConfig");
        }
        //输入调试命令也打印 到日志中
        CTime cmdToLog = CTime::GetCurrentTime();
        CString DebugCmd = " 调试命令为: ";
        CString ChangeRow = "\r\n";
        CString CmdLogTme = cmdToLog.Format("%y.%m.%d_%H.%M.%S").GetBuffer(0) + DebugCmd + m_csDebugCMD + ChangeRow;
        sprintf(m_sCmdToTxt, "%s", CmdLogTme);//注意有可能会溢出
        CFile InputToLog;

        if (!InputToLog.Open(m_cFileName, CFile::modeWrite | CFile::modeNoTruncate | CFile::modeCreate))
        {
            MessageBox("命令导入到txt文件中失败");
            return;
        }
        else
        {
            InputToLog.SeekToEnd();
            InputToLog.Write(m_sCmdToTxt, strlen(m_sCmdToTxt));
            InputToLog.Close();
        }

	}
    UpdateData(FALSE);
}

#if (_MSC_VER >= 1500)	//vs2008
void CDlgDebug::OnTimer(UINT_PTR nIDEvent)
#else
void CDlgDebug::OnTimer(UINT nIDEvent) 
#endif
{
    
    int iAll = strlen(m_sAllDebugInfo);
    if (iAll>5120)
    {
         memset(&m_sAllDebugInfo, 0, sizeof(m_sAllDebugInfo));
    }

    int i = strlen(m_sDebugInfo);
    if(i>0)
    {
        sprintf(m_sDebugInfo, "%s\r\n", m_sDebugInfo);
        sprintf(m_sAllDebugInfo, "%s%s", m_sAllDebugInfo, m_sDebugInfo);
        GetDlgItem(IDC_EDIT_DEBUG_INFO)->SetWindowText(m_sAllDebugInfo);
        memset(&m_sDebugInfo, 0, sizeof(m_sDebugInfo));
    }
    
    CDialog::OnTimer(nIDEvent);
}
