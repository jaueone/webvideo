// DlgInquestFile.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgInquestFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgInquestFile dialog

#define UPLOAD_TIMER WM_USER+1

CDlgInquestFile::CDlgInquestFile(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInquestFile::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgInquestFile)
	m_csUploadState = _T("");
	m_csFilePath = _T("");
	m_csInquestRoom = _T("");
	m_bChkCourtFile = FALSE;
	//}}AFX_DATA_INIT
	m_lUploadHandle = -1;
	memset(&m_strInquestRoom, 0, sizeof(m_strInquestRoom));
}


void CDlgInquestFile::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgInquestFile)
	DDX_Control(pDX, IDC_LIST_FILE, m_listFile);
	DDX_Text(pDX, IDC_UPLOADSTATE, m_csUploadState);
	DDX_Text(pDX, IDC_EDIT_FILE_PATH, m_csFilePath);
	DDX_Text(pDX, IDC_STATIC_INQUEST_ROOM, m_csInquestRoom);
	DDX_Check(pDX, IDC_CHK_COURT_FILE, m_bChkCourtFile);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgInquestFile, CDialog)
	//{{AFX_MSG_MAP(CDlgInquestFile)
	ON_BN_CLICKED(IDC_BUTTON_SCAN, OnButtonScan)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, OnButtonExit)
	ON_BN_CLICKED(IDC_BUTTON_UPLOAD, OnButtonUpload)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, OnButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgInquestFile message handlers

BOOL CDlgInquestFile::OnInitDialog() 
{
	CDialog::OnInitDialog();
	char szLan[128] = {0};

    DWORD m_dwDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
    m_lLoginID = g_struDeviceInfo[m_dwDeviceIndex].lLoginID;

	g_StringLanType(szLan, "��ǰ��Ѷ�ұ��:", "Current inquest room number:");
	m_csInquestRoom.Format("%s%d", szLan, m_strInquestRoom.byRoomIndex);
  
	g_StringLanType(szLan, "�ļ���", "File Name");
    m_listFile.InsertColumn(0, szLan, LVCFMT_LEFT, 0, 0);
	g_StringLanType(szLan, "�ļ�����(byte)", "File Size(byte)");
    m_listFile.InsertColumn(1, szLan, LVCFMT_LEFT, 0, 0);
    m_listFile.SetColumnWidth(0, 250);
    m_listFile.SetColumnWidth(1, 175);
    m_iFileNum = 0;

	UpdateData(FALSE);
    return TRUE;  
}

void CDlgInquestFile::OnButtonScan() 
{
	UpdateData(TRUE);
 
	char szLan[128] = {0};
    OPENFILENAME ofn = {0};
 
    memset(m_FileDir, 0, MAX_PATH);
    
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner   = this->GetSafeHwnd();
    ofn.lpstrFilter = "All Files\0*.*\0\0";
    ofn.lpstrFile   = m_FileDir;
    ofn.nMaxFile    = MAX_PATH;
    ofn.Flags       = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;
    
    if (GetOpenFileName(&ofn))
    {
        m_csFilePath.Format("%s", m_FileDir);
		g_StringLanType(szLan, "ѡ�����ļ� %s, Ҫ�ϴ����'�ϴ�'��ť\r\n", "Selected file:%s, please click the 'Upload' button\r\n!");
        m_csUploadState.Format(szLan, m_FileDir);
    }

    UpdateData(FALSE);	
}

void CDlgInquestFile::OnButtonExit() 
{
	CDialog::OnCancel();
}

void CDlgInquestFile::OnButtonUpload() 
{
    UpdateData(TRUE);
    
	char szLan[128] = {0};
	if (m_bChkCourtFile)
	{
		//m_strInquestRoom.byRoomIndex = 1;
		m_strInquestRoom.byFileType = 1;
	}
	else
	{
		//m_strInquestRoom.byRoomIndex = 1;
		m_strInquestRoom.byFileType = 0;
	}

    m_lUploadHandle = NET_DVR_InquestUploadFile_V30(m_lLoginID, &m_strInquestRoom, m_FileDir);
    if (m_lUploadHandle == -1)
    {
		g_pMainDlg->AddLog(m_dwDeviceIndex, OPERATION_FAIL_T, "NET_DVR_InquestUploadFile");
		g_StringLanType(szLan, "��ʼ�ϴ��ļ�ʧ��", "Start upload file failed!");
	    m_csUploadState.Format("%s", szLan);	
		AfxMessageBox(szLan);
		
		return;
    }
    else
    {   
		g_pMainDlg->AddLog(m_dwDeviceIndex, OPERATION_SUCC_T, "NET_DVR_InquestUploadFile");
        SetTimer(UPLOAD_TIMER, 50, NULL);
    }

    UpdateData(FALSE);	
}

#if (_MSC_VER >= 1500)	//vs2008
void CDlgInquestFile::OnTimer(UINT_PTR nIDEvent)
#else
void CDlgInquestFile::OnTimer(UINT nIDEvent)
#endif
{
    UpdateData(TRUE);

	char szLan[128] = {0};

	if (nIDEvent == UPLOAD_TIMER)
    {
        DWORD dwProgress = 0;
        int state = NET_DVR_InquestGetUploadState(m_lUploadHandle, &dwProgress);

        if (state == 1)
        {
			g_StringLanType(szLan, "�ϴ��ɹ�", "Upload successfully");
            m_csUploadState.Format(szLan);
        }
        else if (state == 2)
        {
			g_StringLanType(szLan, "�����ϴ�,���ϴ�:%d", "Is uploading,progress:%d");
            m_csUploadState.Format(szLan, dwProgress);
        }
        else if (state == 3)
        {
			g_StringLanType(szLan, "�ϴ�ʧ��", "Upload failed");
            m_csUploadState.Format(szLan);
        }
        else if (state == 4)
        {
			g_StringLanType(szLan, "����Ͽ���״̬δ֪", "Network disconnect, status unknown");
            m_csUploadState.Format(szLan);
        }
		else if (state == 5)
		{
			g_StringLanType(szLan, "�ȴ���ȡ�ϴ�����", "Waiting for upload chance");
            m_csUploadState.Format(szLan);
		}
		else if(state == 6)
		{
			g_StringLanType(szLan, "Ӳ�̴���", "Hard Disk Error");
            m_csUploadState.Format(szLan);
		}
		else if(state == 7)
		{
			g_StringLanType(szLan, "����Ѷ�ļ������", "No inquest file save disk");
            m_csUploadState.Format(szLan);
		}
		else if(state == 8)
		{
			g_StringLanType(szLan, "��������", "No enough disk space");
            m_csUploadState.Format(szLan);
		}
		else if(state == 9)
		{
			g_StringLanType(szLan, "�豸��Դ����", "Device no resource");
            m_csUploadState.Format(szLan);
		}
		else if(state == 10)
		{
			if (m_bChkCourtFile)
			{
				g_StringLanType(szLan, "�ļ���������4", "Inquest file sum is more than 4");
				m_csUploadState.Format(szLan);
			}
			else
			{
				g_StringLanType(szLan, "�ļ���������40", "Inquest file sum is more than 40");
				m_csUploadState.Format(szLan);
			}
		}

        if (state != 2 && state != 5)
        {        
            KillTimer(UPLOAD_TIMER);
            NET_DVR_InquestUploadClose(m_lUploadHandle);
        }
    }
    CDialog::OnTimer(nIDEvent);  

    UpdateData(FALSE);
}

void CDlgInquestFile::OnButtonSearch() 
{
	UpdateData(TRUE);
	char szLan[128] = {0};
    m_listFile.DeleteAllItems();
	if (m_bChkCourtFile)
	{
		//m_strInquestRoom.byRoomIndex = 1;
		m_strInquestRoom.byFileType = 1;
	}

    LONG lFileHandle =  NET_DVR_InquestFindFile_V30(m_lLoginID, &m_strInquestRoom);
    if (lFileHandle == -1)
    {
		g_pMainDlg->AddLog(m_dwDeviceIndex, OPERATION_FAIL_T, "NET_DVR_InquestFindFile");
		g_StringLanType(szLan, "������Ѷ�ļ�ʧ��", "Search inquest files failed!");
		AfxMessageBox(szLan);
		return;
    }
    else
    {   
		g_pMainDlg->AddLog(m_dwDeviceIndex, OPERATION_SUCC_T, "NET_DVR_InquestFindFile");
    }

    LONG      bRet = -1;
    char      tempfile[40] = {0};
    CString   tempstring;
    NET_DVR_INQUEST_FILEINFO strInquestFile = {0};

    while(1)
    {
        bRet = NET_DVR_InquestFindNextFile(lFileHandle, &strInquestFile);
        if(bRet == NET_DVR_FILE_SUCCESS)
        {	
            strcpy(tempfile, strInquestFile.sFileName);
            m_listFile.InsertItem(m_iFileNum, tempfile, 0);
            
            m_listFile.SetItemData(m_iFileNum, strInquestFile.dwFileLen);
            tempstring.Format("%d", strInquestFile.dwFileLen);
            
            m_listFile.SetItemText(m_iFileNum, 1, tempstring);    
            m_iFileNum ++;
        }
        else
        {
            if(bRet == NET_DVR_ISFINDING)
            {
                Sleep(5);
                continue;
            }
            if( (bRet == NET_DVR_NOMOREFILE) || (bRet == NET_DVR_FILE_NOFIND) )
            {
				g_StringLanType(szLan, "��ȡ�ļ��б�������ļ��Ѿ�ȫ���г�", "Finish to get file list, and documents have all been listed");
				AfxMessageBox(szLan);
				m_iFileNum = 0;
                break;
            }
            else
            {
				g_StringLanType(szLan, "���ڷ�����æ,���������,��ȡ�ļ��б��쳣��ֹ", "Since the server is busy, or network failure, abnormal termination of access to the file list");
				AfxMessageBox(szLan);
                m_iFileNum = 0;
                break;
            }
        }
    }

    NET_DVR_InquestFindClose(lFileHandle);	
}

void CDlgInquestFile::OnButtonDelete() 
{
	char szLan[256] = {0};
    int itemCount = 0;
    int index = 0;
    NET_DVR_INQUEST_FILES dFile = {0};

    POSITION pos = m_listFile.GetFirstSelectedItemPosition();
    if (pos == NULL)
    {
		g_StringLanType(szLan, "��ѡ��Ҫɾ�����ļ�", "Please select the files for delete");
		AfxMessageBox(szLan);
        return;
    }
	
    while (pos != NULL && itemCount < MAX_RECORD_FILE_NUM)
    {
        index = m_listFile.GetNextSelectedItem(pos);
        
        sprintf(dFile.struFileInfo[itemCount].sFileName, "%s", m_listFile.GetItemText(index, 0));
        dFile.struFileInfo[itemCount].dwFileLen = m_listFile.GetItemData(index);
        
        ++ itemCount;
    }
    if (pos != NULL)
    {
		g_StringLanType(szLan, "��ѡ�ļ���������(20)����", "The selected files' num is more than the 20");
        AfxMessageBox(szLan, MB_ICONWARNING);
        return;
    }
    dFile.dwFileNum = itemCount;
    if (m_bChkCourtFile)
	{
		//m_strInquestRoom.byRoomIndex = 1;
		m_strInquestRoom.byFileType = 1;
	}
    if (!NET_DVR_InquestDeleteFile_V30(m_lLoginID, &m_strInquestRoom, &dFile))
    {
		g_StringLanType(szLan, "ɾ���ļ�ʧ��", "Delete files failed");
        AfxMessageBox(szLan);
        return;
    }
	else
	{
		g_StringLanType(szLan, "ɾ���ɹ�", "Delete files successful");
        AfxMessageBox(szLan);
        return;
	}
}
