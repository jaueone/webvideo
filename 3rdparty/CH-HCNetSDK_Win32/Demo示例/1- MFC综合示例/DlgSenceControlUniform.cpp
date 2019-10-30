// DlgSenceControl2.cpp : implementation file
//

#include "stdafx.h"
#include "ClientDemo.h"
#include "DlgSenceControlUniform.h"

#define SCENE_CONFIGURATION_FILE_TIMER 20
#define DOWNLOAD_SCENE_CONFIGURATION_FILE_TIMER 21 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////
//��ʼ����Դ
BOOL SceneBatchManage::Init(LPNET_DVR_WALLSCENECFG lpSrc, LPNET_DVR_VIDEO_WALL_INFO lpIndex, DWORD max)
{
	return (m_RecordCfg.Init( lpSrc, max, sizeof(*lpSrc)) && m_RecordInfo.Init( lpIndex, max, sizeof(*lpIndex)));
}
//��ȡ�޸Ĺ���Դ, ���ظ���
int SceneBatchManage::GetModify(LPNET_DVR_WALLSCENECFG &lpModifySrc, LPNET_DVR_VIDEO_WALL_INFO &lpIndex)
{
	int iRetNumCfg = 0;
	int iRetNumInfo = 0; 
	void * pSrc = (void *) lpModifySrc; 
	void * pIndex = (void *) lpIndex; 
	iRetNumCfg	 = m_RecordCfg.GetMod( ((void **)&lpModifySrc) );
	iRetNumInfo	 = m_RecordInfo.GetMod( ((void **)&lpIndex) );
	return (iRetNumCfg == iRetNumInfo)?iRetNumInfo:-1; 
}
//ˢ��ĳһǽ
BOOL SceneBatchManage::Update(DWORD dwNum, const NET_DVR_WALLSCENECFG *lpSceneCfg, const NET_DVR_VIDEO_WALL_INFO &struIndex)    
{
	m_RecordInfo.Clear(); 
	m_RecordCfg.Clear(); 
	NET_DVR_VIDEO_WALL_INFO struWallInfo = {0}; 
	int iIndex = -1; 
	try
	{
		for (int i=0 ; i<dwNum; i++)
		{
			iIndex = m_RecordCfg.Update(lpSceneCfg+i); 
			memcpy(&struWallInfo, &struIndex, sizeof(struWallInfo));
			struWallInfo.dwSceneNo = lpSceneCfg[i].bySceneIndex; 
			m_RecordInfo.Update(&struWallInfo,  iIndex ); 
		}
	}
	catch (...)
	{
		return FALSE; 
	}
	return TRUE; 
}

//���±Ƚ���������
bool SceneIndexCmp (const void *ItemF, const void *ItemS)
{
	const NET_DVR_WALLSCENECFG * CfgItemF = (const NET_DVR_WALLSCENECFG *) ItemF; 
	const NET_DVR_WALLSCENECFG * CfgItemS = (const NET_DVR_WALLSCENECFG *) ItemS; 
	return (CfgItemF->bySceneIndex == CfgItemS->bySceneIndex);
}


//ˢ��ĳһ�� 
BOOL SceneBatchManage::Update(const NET_DVR_WALLSCENECFG &SceneCfg, const NET_DVR_VIDEO_WALL_INFO &struIndex)
{
	int iIndex; 
	if (( iIndex = m_RecordCfg.Update(& SceneCfg, SceneIndexCmp)) != -1)
	{
		return m_RecordInfo.Update(&struIndex , iIndex); 
	}
	return FALSE; 
}

BOOL SceneBatchManage::Modify(const NET_DVR_WALLSCENECFG &SceneCfg, const NET_DVR_VIDEO_WALL_INFO &struIndex)
{
	int iIndex; 
	if (( iIndex = m_RecordCfg.Add(& SceneCfg, SceneIndexCmp)) != -1)
	{
		return m_RecordInfo.Add(&struIndex , iIndex); 
	}
	return FALSE; 
}


//�ı�ĳһ������ ���޸�
BOOL SceneBatchManage::Change(const NET_DVR_WALLSCENECFG &SceneCfg, const NET_DVR_VIDEO_WALL_INFO &struIndex)
{
	int iIndex; 
	if (( iIndex = m_RecordCfg.Add(& SceneCfg, SceneIndexCmp)) != -1)
	{
		return m_RecordInfo.Add(&struIndex, iIndex); 
	}
	return FALSE; 
}
//��ȡ��Դ��Ŀ
int SceneBatchManage::GetNum()
{
	return m_RecordCfg.GetUsedNum(); 
}
//��ȡԭʼ��Դ��ַ
bool SceneBatchManage::GetSrc(LPNET_DVR_VIDEO_WALL_INFO &lpIndex, LPNET_DVR_WALLSCENECFG &lpSrc)
{
	lpIndex = (LPNET_DVR_VIDEO_WALL_INFO ) m_RecordInfo.GetSrc(); 
	lpSrc = (LPNET_DVR_WALLSCENECFG) m_RecordCfg.GetSrc(); 
	return true; 

}
//ɾ��ĳһ���
BOOL SceneBatchManage::Del(int iSceneIndex)
{
	NET_DVR_WALLSCENECFG struCfg = {0}; 
	struCfg.bySceneIndex = (BYTE)iSceneIndex;
	int iIndex = -1; 
	if ((iIndex = m_RecordCfg.Del(&struCfg, SceneIndexCmp)) != -1)
	{
		m_RecordInfo.Del(iIndex);
	}
	return TRUE; 
}

/////////////////////////////////////////////////////////////////////////////
// CDlgSenceControlUniform dialog


CDlgSenceControlUniform::CDlgSenceControlUniform(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSenceControlUniform::IDD, pParent)
	,m_lpOutBuffer(NULL)
{
	//{{AFX_DATA_INIT(CDlgSenceControlUniform)
	m_dwCopyScene = 0;
	m_csSceneName = _T("");
	m_dwSceneNum = 0;
	m_dwSceneNumOperate = 0;
	m_chEnable = FALSE;
	m_bySceneIndex = 0;
	m_dwWallNo = 1;
	m_dwStructNum = 0;
	m_dwWinNo = 1;
	m_dwSceneControl = 1;
	m_dwWallNoControl = 1;
	m_dwSceneNoCur = 1;
	m_dwWallNoCur = 1;
	m_csConfigFilePath = _T("");
	//}}AFX_DATA_INIT
    memset(&m_struSenceCfg, 0, sizeof(m_struSenceCfg));
    memset(&m_dwStatus, 0, sizeof(m_dwStatus));
    memset(&m_struWallInfo, 0, sizeof(m_struWallInfo)); 
    m_SceneCfgManage.Init(m_struSenceCfg, m_struWallInfo, SCENE_NUM); 
    m_lpGetAllSenceBuf = new char[sizeof(DWORD)+sizeof(NET_DVR_WALLSCENECFG)*SCENE_NUM]; 
    m_lUploadHandle = -1; 
    m_lDownloadHandle = -1; 
}

CDlgSenceControlUniform::~CDlgSenceControlUniform()
{
	if (NULL != m_lpOutBuffer)
	{
		delete[] m_lpOutBuffer;
		m_lpOutBuffer = NULL;
	}
    if (m_lpGetAllSenceBuf != NULL)
    {
        delete [] m_lpGetAllSenceBuf; 
        m_lpGetAllSenceBuf = NULL; 
    }
}

void CDlgSenceControlUniform::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSenceControlUniform)
	DDX_Control(pDX, IDC_LIST_SCENE, m_listScene);
	DDX_Control(pDX, IDC_COMBO_SCENE_OPERATE, m_comboSceneOperate);
	DDX_Text(pDX, IDC_EDIT_COPY_SCENE, m_dwCopyScene);
	DDX_Text(pDX, IDC_EDIT_SCENE_NAME, m_csSceneName);
	DDX_Check(pDX, IDC_CHECK_ENABLE, m_chEnable);
	DDX_Text(pDX, IDC_EDIT_SCENE_INDEX, m_bySceneIndex);
	DDX_Text(pDX, IDC_EDIT_WALL_NO, m_dwWallNo);
	DDX_Text(pDX, IDC_EDIT_STRUCT_NUM, m_dwStructNum);
	DDX_Text(pDX, IDC_EDT_VVSS_WINNO, m_dwWinNo);
	DDX_Text(pDX, IDC_EDIT_CONTROL_SCENENO, m_dwSceneControl);
	DDX_Text(pDX, IDC_EDIT_CONTROL_WALL_NO, m_dwWallNoControl);
	DDV_MinMaxDWord(pDX, m_dwWallNoControl, 0, 255);
	DDX_Text(pDX, IDC_EDT_CURRENT_SCENENO, m_dwSceneNoCur);
	DDX_Text(pDX, IDC_EDT_CURRENT_WALL_NO, m_dwWallNoCur);
	DDV_MinMaxDWord(pDX, m_dwWallNoCur, 0, 255);
	DDX_Text(pDX, IDC_EDT_VM_SCENE_CONFIGUREFILE_PATH, m_csConfigFilePath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSenceControlUniform, CDialog)
	//{{AFX_MSG_MAP(CDlgSenceControlUniform)
	ON_BN_CLICKED(IDC_BTN_MODE_GET, OnBtnModeGet)
	ON_BN_CLICKED(IDC_BTN_MODE_SET, OnBtnModeSet)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBtnSave)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SCENE, OnClickListScene)
	ON_BN_CLICKED(IDC_BTN_GET_SCENE, OnBtnSceneGet)
	ON_CBN_SELCHANGE(IDC_COMBO_SCENE_OPERATE, OnSelchangeComboSceneOperate)
	ON_BN_CLICKED(IDC_BTN_CONTROL, OnBtnControl)
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBtnExit)
	ON_BN_CLICKED(IDC_BTN_GET_ALL_SCENE, OnBtnGetAllScene)
	ON_BN_CLICKED(IDC_BUT_VWSS_REFRESH, OnButVwssRefresh)
	ON_BN_CLICKED(IDC_BUT_VW_SCENE_BROWSER, OnButBrowser)
	ON_BN_CLICKED(IDC_BUT_VM_SCENEUPLOAD, OnButSceneUpload)
	ON_BN_CLICKED(IDC_BUT_VM_SCENEDOWNLOAD, OnButSceneDownload)
    ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSenceControlUniform message handlers

BOOL CDlgSenceControlUniform::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_listScene.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT); //����list���
	m_iCurSel = -1;
	int len = sizeof(m_struSenceCfg) + 4;
	m_lpOutBuffer = new char[len];
	if (NULL == m_lpOutBuffer)
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "alloc memory failed");
		return FALSE;
	}

	m_iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
	m_lUserID = g_struDeviceInfo[m_iDeviceIndex].lLoginID;
	char szLan[128] = {0};
	m_listScene.SetExtendedStyle(m_listScene.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
	g_StringLanType(szLan, "������", "Scene No.");
	m_listScene.InsertColumn(0, szLan, LVCFMT_LEFT, 80);
	g_StringLanType(szLan, "������", "Scene Name");
	m_listScene.InsertColumn(1, szLan, LVCFMT_LEFT, 200);
	g_StringLanType(szLan, "�Ƿ���Ч", "Y/N");
	m_listScene.InsertColumn(2, szLan, LVCFMT_LEFT, 200);
    
	OnBtnModeGet();
	OnSelchangeComboSceneOperate();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//��ȡ���г�������
void CDlgSenceControlUniform::OnBtnModeGet() 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE);
    NET_DVR_VIDEO_WALL_INFO struWallInfo={0}; 
    struWallInfo.dwSize = sizeof(struWallInfo);
    struWallInfo.dwWindowNo = (m_dwWallNo << 24);    //ָ��ĳһ��ǽ
	if (!NET_DVR_GetDeviceConfig(m_lUserID, NET_DVR_GET_VW_SCENE_PARAM, 0xffffffff, &struWallInfo, sizeof(struWallInfo), m_dwStatus, m_lpGetAllSenceBuf, (sizeof(DWORD)+SCENE_NUM*sizeof(NET_DVR_WALLSCENECFG))))
	{
        g_StringLanType(m_szLan, "��ȡ���г�������ʧ��", "Failed to get all the papam of scene");
        sprintf(m_szLan, "%s, Error Code %d", m_szLan, NET_DVR_GetLastError()); 
		AfxMessageBox(m_szLan);
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_GET_VW_SCENE_PARAM");		
		return;
	}
	else
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_GET_VW_SCENE_PARAM");
        m_dwCount = *((DWORD *)m_lpGetAllSenceBuf); 
        LPNET_DVR_WALLSCENECFG lpSceneCfg = (LPNET_DVR_WALLSCENECFG) (m_lpGetAllSenceBuf+sizeof(DWORD)); 
        m_SceneCfgManage.Update(m_dwCount, lpSceneCfg, struWallInfo);       //ˢ��ĳһ��ǽ 
	    DrawList();
	}	
}

void CDlgSenceControlUniform::DrawList(BOOL bIndexType)
{
    m_listScene.DeleteAllItems();
    CString cs; 
    int iCount = m_SceneCfgManage.GetNum(); 
    for(int i = 0; i < iCount ; i++) 
    {
        
        cs.Format("%d",  m_struSenceCfg[i].bySceneIndex);
        m_listScene.InsertItem(i, cs, 0);
        m_listScene.SetItemData(i, m_struSenceCfg[i].bySceneIndex); 
        cs.Format("%s", m_struSenceCfg[i].sSceneName);
        m_listScene.SetItemText(i, 1, cs);
        if (1 == m_struSenceCfg[i].byEnable)
        {
            cs.Format("%s", "Y");
        }
        else
        {
            cs.Format("%s", "N");
        }
        m_listScene.SetItemText(i, 2, cs);
    }
    UpdateData(FALSE); 
}

void CDlgSenceControlUniform::OnBtnModeSet() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
    DWORD dwSetNum; 
    LPNET_DVR_VIDEO_WALL_INFO lpWallInfo = NULL; 
    LPNET_DVR_WALLSCENECFG lpSceneCfg=NULL; 
    dwSetNum = m_SceneCfgManage.GetModify(lpSceneCfg, lpWallInfo); 
    if ( dwSetNum == 0 || lpSceneCfg == NULL)
    {
        g_StringLanType(m_szLan, "û���޸���", "No Change Item");
        MessageBox(m_szLan);
        return ; 
    }
	if (!NET_DVR_SetDeviceConfig(m_lUserID, NET_DVR_SET_VW_SCENE_PARAM, dwSetNum, lpWallInfo, dwSetNum*sizeof(NET_DVR_VIDEO_WALL_INFO), m_dwStatus, lpSceneCfg, dwSetNum*sizeof(NET_DVR_WALLSCENECFG)))
	{
        g_StringLanType(m_szLan, "���ò���ʧ��", "Failed to set papam"); 
        sprintf(m_szLan, "%s, Error Code %d", m_szLan, NET_DVR_GetLastError()); 
		AfxMessageBox(m_szLan);
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_SET_VW_SCENE_PARAM");
		return;
	}
	else
	{
		for(int  i = 0; i < dwSetNum; i++)
		{  
			if ( m_dwStatus[i] )
			{
				sprintf(m_szLan, "scene[%d] status = %d\n", lpWallInfo[i].dwSceneNo, m_dwStatus[i]);
				g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, m_szLan);
				m_SceneCfgManage.Del(lpWallInfo[i].dwSceneNo);
			}		
		}
        DrawList();
	}

}

void CDlgSenceControlUniform::OnBtnSave() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
    NET_DVR_WALLSCENECFG struSceneCfg = {0};
    NET_DVR_VIDEO_WALL_INFO struWallInfo = {0}; 
    struSceneCfg.dwSize = sizeof(struSceneCfg);
    strncpy((char *)struSceneCfg.sSceneName, m_csSceneName, m_csSceneName.GetLength());
    struSceneCfg.byEnable = m_chEnable;
    struSceneCfg.bySceneIndex = m_bySceneIndex;
    struWallInfo.dwSceneNo = m_bySceneIndex; 
    struWallInfo.dwSize = sizeof(struWallInfo);
    struWallInfo.dwWindowNo = (m_dwWallNo << 24); 
    m_SceneCfgManage.Modify(struSceneCfg, struWallInfo); 
    DrawList(); 
}

void CDlgSenceControlUniform::OnClickListScene(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	char szText[128] = {0};
	POSITION  iPos = m_listScene.GetFirstSelectedItemPosition();
	if (iPos == NULL)
	{
        return;
    }
    m_iCurSel =  m_listScene.GetNextSelectedItem(iPos);
    m_bySceneIndex = m_listScene.GetItemData(m_iCurSel); 
	m_listScene.GetItemText(m_iCurSel, 1, szText, 128);
	m_csSceneName.Format("%s", szText);

	m_listScene.GetItemText(m_iCurSel, 2, szText, 128);
	if ('Y' == szText[0])
	{
		m_chEnable = 1;	
	}
	else
	{
		m_chEnable = 0;
	}
    m_dwSceneControl = m_bySceneIndex; 
	UpdateData(FALSE);

	*pResult = 0;
}

//��ȡ��ǰ����
void CDlgSenceControlUniform::OnBtnSceneGet() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
    NET_DVR_VIDEO_WALL_INFO struWallInfo={0}; 
    struWallInfo.dwSize = sizeof(struWallInfo);
    struWallInfo.dwWindowNo = (m_dwWallNoCur << 24); 

    DWORD dwCurSceneNo; 
    if (!NET_DVR_GetDeviceConfig(m_lUserID, NET_DVR_GET_CURRENT_SCENE, 1, &struWallInfo, sizeof(struWallInfo), m_dwStatus, &dwCurSceneNo, sizeof(dwCurSceneNo)) )
    {
        g_StringLanType(m_szLan, "��ȡ��ǰ����ʧ��", "Failed to get Current scene");
        sprintf(m_szLan, "%s, Error Code %d", m_szLan, NET_DVR_GetLastError()); 
        AfxMessageBox(m_szLan);
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_GET_CURRENT_SCENE");		
        return;
	}
    else
    {
        m_dwSceneNoCur = dwCurSceneNo; 
        UpdateData(FALSE); 
    }
}

void CDlgSenceControlUniform::OnSelchangeComboSceneOperate() 
{
	// TODO: Add your control notification handler code here
	if (m_comboSceneOperate.GetCurSel() == 3)
	{
		GetDlgItem(IDC_EDIT_COPY_SCENE)->ShowWindow(SW_HIDE);
	}
	else
	{
		m_dwCopyScene = 0;
		GetDlgItem(IDC_EDIT_COPY_SCENE)->ShowWindow(SW_HIDE);
	}
}

void CDlgSenceControlUniform::OnBtnControl() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
    NET_DVR_SCENE_CONTROL_INFO struControlInfo = {0};
    struControlInfo.dwSize = sizeof(struControlInfo); 
    struControlInfo.struVideoWallInfo.dwSize  = sizeof(struControlInfo.struVideoWallInfo); 
    struControlInfo.struVideoWallInfo.dwWindowNo = m_dwWallNoControl << 24; 
    struControlInfo.struVideoWallInfo.dwSceneNo = m_dwSceneControl; 
    struControlInfo.dwCmd = m_comboSceneOperate.GetCurSel()+1; 

	if (!NET_DVR_RemoteControl(m_lUserID, NET_DVR_SCENE_CONTROL, &struControlInfo, sizeof(struControlInfo)) )
	{
        g_StringLanType(m_szLan, "��������ʧ��", "Failed to control the scene");
        sprintf(m_szLan, "%s, Error Code %d", m_szLan, NET_DVR_GetLastError()); 
        AfxMessageBox(m_szLan);     
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_SCENE_CONTROL");
	
	}
	else
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_SCENE_CONTROL ");
	}
}

void CDlgSenceControlUniform::OnBtnExit() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}

void CDlgSenceControlUniform::OnBtnGetAllScene() 
{
	// TODO: Add your control notification handler code here
// 	UpdateData(TRUE);
// 	int i = 0;
// 	BOOL bOneFail = FALSE;
// 	char cs[2048] = {0};
// 	CString csTemp;
// 	m_struWallIndex[0].dwSize = sizeof(NET_DVR_WALL_INDEX);
// 	m_struWallIndex[0].byWallNo = (BYTE)m_dwWallNo;
// 
// //	if (!NET_DVR_GetDeviceConfig(m_lUserID, NET_DVR_WALLSCENEPARAM_GET, 0xffffffff, NULL, 0, NULL, m_lpOutBuffer, SCENE_NUM * sizeof(NET_DVR_WALLSCENECFG) + 4))
// 	if (!NET_DVR_GetDeviceConfig(m_lUserID, NET_DVR_GET_WALL_SCENE_PARAM_V41, 0xffffffff, &m_struWallIndex[0], sizeof(NET_DVR_WALL_INDEX), NULL, m_lpOutBuffer, SCENE_NUM * sizeof(NET_DVR_WALLSCENECFG) + 4))
// 	{
//         i = NET_DVR_GetLastError();
// 		sprintf(cs, "error code: %d", i);
// 		AfxMessageBox(cs);
// 		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_GET_WALL_SCENE_PARAM_V41");
// 		//return;
// 	}
// 	else
// 	{
// 		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_GET_WALL_SCENE_PARAM_V41");
// 		int iSceneNum = *((int*)m_lpOutBuffer);
// 		memcpy(&m_struSenceCfg, m_lpOutBuffer+4, iSceneNum * sizeof(NET_DVR_WALLSCENECFG));
// 		m_listScene.DeleteAllItems();
// 		DrawList(FALSE);
// 	}
}

void CDlgSenceControlUniform::OnButVwssRefresh() 
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);
    DWORD dwNum = m_SceneCfgManage.GetNum(); 
    if ( dwNum == 0)
    {
        return ; 
    }
    LPNET_DVR_VIDEO_WALL_INFO lpWallInfo = NULL; 
    LPNET_DVR_WALLSCENECFG   lpWallCfg = NULL; 
    m_SceneCfgManage.GetSrc(lpWallInfo, lpWallCfg); 
    if (!NET_DVR_GetDeviceConfig(m_lUserID, NET_DVR_GET_VW_SCENE_PARAM, dwNum, lpWallInfo, dwNum*sizeof(*lpWallInfo), m_dwStatus, lpWallCfg, dwNum*sizeof(NET_DVR_WALLSCENECFG)) )
    {
        g_StringLanType(m_szLan, "ˢ�³���ʧ��", "Failed to get all the papam of scene");
        sprintf(m_szLan, "%s, Error Code %d", m_szLan, NET_DVR_GetLastError()); 
        AfxMessageBox(m_szLan);
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_GET_VW_SCENE_PARAM");		
        return;
	}
//    m_SceneCfgManage.Update(struSceneCfg, struWallInfo); 

    //���½���
//    UpdateInterface(struSceneCfg);
    DrawList(); 
}

void CDlgSenceControlUniform::UpdateInterface(const NET_DVR_WALLSCENECFG &Item)
{
    m_bySceneIndex = Item.bySceneIndex; 
    m_chEnable = Item.byEnable; 
    m_csSceneName.Format("%s", (char*)Item.sSceneName); 
    UpdateData(FALSE); 
}


void CDlgSenceControlUniform::OnButBrowser() 
{
	// TODO: Add your control notification handler code here
    static char szFilter[] = "All File(*.*)|*.*||";
    CFileDialog dlg(TRUE,"*.*", NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter);
    if (dlg.DoModal() == IDOK)
    {
        m_csConfigFilePath = dlg.GetPathName();
        UpdateData(FALSE);
	}
}

void CDlgSenceControlUniform::OnButSceneUpload() 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE);
    char szFileName[MAX_PATH];
    strcpy(szFileName, m_csConfigFilePath);
    CFile cFile;
    char szLan[128] = {0};
    if (!cFile.Open(szFileName, NULL))
    {
        g_StringLanType(szLan, "���ļ�ʧ�ܻ��޴��ļ�", "Open file failed or no this file");
        AfxMessageBox(szLan);
        return;
    }
    DWORD dwFileSize = (DWORD)cFile.GetLength();
    if (dwFileSize == 0)
    {
        g_StringLanType(szLan, "�����ļ�Ϊ��", "Configure file is empty");
        AfxMessageBox(szLan);
    }
    cFile.Close();

    NET_DVR_VIDEO_WALL_INFO struWallInfo = {0}; 
    struWallInfo.dwSize = sizeof(struWallInfo);
    struWallInfo.dwSceneNo = m_bySceneIndex; 
    struWallInfo.dwWindowNo = m_dwWallNo << 24; 

    LONG lUploadHandle = -1; 
    lUploadHandle = NET_DVR_UploadFile(m_lUserID, SCENE_CONFIGURATION_FILE, &struWallInfo, sizeof(struWallInfo), szFileName);
    if (-1 == lUploadHandle)
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_UploadFile");
        AfxMessageBox("NET_DVR_UploadFile failed");
    }
    else
    {
        SetTimer(SCENE_CONFIGURATION_FILE_TIMER, 1000, NULL);
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_UploadFile");
    }
}

void CDlgSenceControlUniform::OnButSceneDownload() 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE);
    char szFileName[MAX_PATH];
    if (m_csConfigFilePath.IsEmpty())
    {
        MessageBox("�����ļ�·������Ϊ��"); 
        return ; 
    }
    strcpy(szFileName, m_csConfigFilePath);
    NET_DVR_VIDEO_WALL_INFO struWallInfo = {0}; 
    struWallInfo.dwSize = sizeof(struWallInfo);
    struWallInfo.dwSceneNo = m_bySceneIndex; 
    struWallInfo.dwWindowNo = m_dwWallNo << 24; 
    LONG lUploadHandle = -1; 
    lUploadHandle = NET_DVR_StartDownload(m_lUserID, NET_SDK_DOWNLOAD_SCENE_CONFIGURATION_FILE, &struWallInfo, sizeof(struWallInfo), szFileName);
    if (-1 == lUploadHandle)
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_StartDownload");
        AfxMessageBox("NET_DVR_StartDownload failed");
    }
    else
    {
        SetTimer(DOWNLOAD_SCENE_CONFIGURATION_FILE_TIMER, 1000, NULL);
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_StartDownload");      
    }
}

#if (_MSC_VER >= 1500)	//vs2008
void CDlgSenceControlUniform::OnTimer(UINT_PTR nIDEvent)
#else
void CDlgSenceControlUniform::OnTimer(UINT nIDEvent)
#endif
{
    int iStatus = -1;
    DWORD dwProgress = 0; 
    CString csShow; 
    switch (nIDEvent)
    {
    case SCENE_CONFIGURATION_FILE_TIMER:
        if ( m_lUploadHandle   == -1)
        {
            return ; 
        }
        iStatus = NET_DVR_GetUploadState(m_lUploadHandle, &dwProgress); 
        csShow.Format("Upload Configure File"); 
        break;
    case DOWNLOAD_SCENE_CONFIGURATION_FILE_TIMER:
        if ( m_lDownloadHandle == -1)
        {
            return ; 
        }
        iStatus = NET_DVR_GetDownloadState(m_lDownloadHandle, &dwProgress); 
        csShow.Format("Download Configure File"); 
        break; 
        
    default:
        return; 
    }

    switch ( iStatus )
    {
    case 1://��� ���ػ��ϴ�
        csShow += "Finish"; 
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, csShow);
        break; 
    case 2://���� ���ػ��ϴ�
        csShow += "Doing"; 
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, csShow);
        CDialog::OnTimer(nIDEvent);
        break; 
    case 3://ʧ�� ���ػ��ϴ�
        csShow += "Failed"; 
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, csShow);
        break; 
    case 4://����Ͽ�
        csShow += "Socket close"; 
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, csShow);
        break; 
    default:
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "Unknown status");      
    }
    
    switch (nIDEvent)
    {
    case SCENE_CONFIGURATION_FILE_TIMER:      
        NET_DVR_UploadClose(m_lUploadHandle);       
        break;
    case DOWNLOAD_SCENE_CONFIGURATION_FILE_TIMER:     
        NET_DVR_StopDownload(m_lDownloadHandle);         
        break; 
    }
}
