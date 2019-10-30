// DlgThermometry.cpp : 实现文件
//

#include "stdafx.h"
#include "ClientDemo.h"
#include "DlgThermometry.h"
#include "afxdialogex.h"
#include "DrawFun.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
// CDlgThermometry 对话框

IMPLEMENT_DYNAMIC(CDlgThermometry, CDialogEx)

CDlgThermometry::CDlgThermometry(CWnd* pParent /*=NULL*/)
    : CDialogEx(CDlgThermometry::IDD, pParent)
    , m_RuleID(0)
    , m_Max(0)
    , m_Min(0)
    , m_Ave(0)
    , m_max_x(0)
    , m_max_y(0)
    , m_min_x(0)
    , m_min_y(0)
    , m_fDistance(0)
    , m_fEnviroTemperature(0)
    , m_fCorrEmissivity(0)
    , m_fPreTemperature1(0)
    , m_fPreTemperature2(0)
    , m_fPreTemperature3(0)
    , m_fPreTemperature4(0)
    , m_strFilePath(_T(""))
    , m_bStatus1(FALSE)
    , m_bStatus2(FALSE)
    , m_bStatus3(FALSE)
    , m_bStatus4(FALSE)
    , m_Freezedata(_T(""))
{
    memset(&m_struPolyGon, 0, sizeof(m_struPolyGon));
    m_pBuffer = new char[MAX_LEN_XML];
    memset(m_pBuffer, 0, MAX_LEN_XML);
    for (int i = 0; i < TEMPERATURE_POINT_NUM; ++i)
    {
        m_fPreTemperature[i] = 0;
        m_fCoordinatesX[i] = 0;
        m_fCoordinatesY[i] = 0;
    }
    memset(&m_struPoint, 0, sizeof(m_struPoint));
    m_dwCurrentIndex = 0;


}

CDlgThermometry::~CDlgThermometry()
{
    StopRealPlay();
}

void CALLBACK g_DrawTempRgn(LONG lRealHandle, HDC hDc, DWORD dwUser)
{
    CDlgThermometry* pDlg = (CDlgThermometry*)dwUser;

    if (pDlg != NULL)
    {
        pDlg->DrawRgnFun(hDc);
    }
}

BOOL CDlgThermometry::StartRealPlay()
{
    NET_DVR_CLIENTINFO struPlay;
    struPlay.hPlayWnd = GetDlgItem(IDC_PLAYWND)->GetSafeHwnd();
    struPlay.lChannel = m_lChannel;
    struPlay.lLinkMode = 0;
    struPlay.sMultiCastIP = "";
    m_lRealHandle = NET_DVR_RealPlay_V30(m_lUserID, &struPlay, NULL, NULL, TRUE);
    BOOL bRet = FALSE;
    if (m_lRealHandle < 0)
    {
        g_pMainDlg->AddLog(-1, OPERATION_FAIL_T, "NET_DVR_RealPlay_V30");
        AfxMessageBox("NET_DVR_RealPlay_V30 failed");
    }
    else
    {
        g_pMainDlg->AddLog(-1, OPERATION_SUCC_T, "NET_DVR_RealPlay_V30");
        bRet = NET_DVR_RigisterDrawFun(m_lRealHandle, g_DrawTempRgn, (DWORD)this);//DC Draw callback
    }
    return bRet;
}

BOOL CDlgThermometry::StopRealPlay()
{
    BOOL bRet = FALSE;
    if (!NET_DVR_StopRealPlay(m_lRealHandle))
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_StopRealPlay");
        bRet = FALSE;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_StopRealPlay");
        m_lRealHandle = -1;
        bRet = TRUE;
    }

    return bRet;
}

void CDlgThermometry::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT3, m_RuleID);
    DDX_Text(pDX, IDC_EDIT10, m_Max);
    DDX_Text(pDX, IDC_EDIT11, m_Min);
    DDX_Text(pDX, IDC_EDIT13, m_Ave);
    DDX_Text(pDX, IDC_EDIT14, m_max_x);
    DDX_Text(pDX, IDC_EDIT15, m_max_y);
    DDX_Text(pDX, IDC_EDIT16, m_min_x);
    DDX_Text(pDX, IDC_EDIT17, m_min_y);
    DDX_Text(pDX, IDC_EDIT18, m_Freezedata);
    DDX_Text(pDX, IDC_EDIT_DISTANCE2, m_fDistance);
    DDX_Text(pDX, IDC_EDIT_ENVIRO_TEMPERATURE, m_fEnviroTemperature);
    DDX_Text(pDX, IDC_EDIT_EMISSIVITY2, m_fCorrEmissivity);
    DDX_Text(pDX, IDC_EDIT_PRE_TEMPERATURE1, m_fPreTemperature1);
    DDX_Text(pDX, IDC_EDIT_PRE_TEMPERATURE2, m_fPreTemperature2);
    DDX_Text(pDX, IDC_EDIT_PRE_TEMPERATURE3, m_fPreTemperature3);
    DDX_Text(pDX, IDC_EDIT_PRE_TEMPERATURE4, m_fPreTemperature4);
    DDX_Text(pDX, IDC_EDIT_FILE, m_strFilePath);
    DDX_Check(pDX, IDC_CHECK_ENABLE_STATUS1, m_bStatus1);
    DDX_Check(pDX, IDC_CHECK_ENABLE_STATUS2, m_bStatus2);
    DDX_Check(pDX, IDC_CHECK_ENABLE_STATUS3, m_bStatus3);
    DDX_Check(pDX, IDC_CHECK_ENABLE_STATUS4, m_bStatus4);
    DDX_Control(pDX, IDC_BTN_STRAT_CORRECT, m_btnStartCorrect);
}


BEGIN_MESSAGE_MAP(CDlgThermometry, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON10, &CDlgThermometry::OnBnClickedButton10)
    ON_BN_CLICKED(IDC_BTN_SET_FILE, &CDlgThermometry::OnBnClickedBtnSetFile)
    ON_BN_CLICKED(IDC_BTN_SET_TEMPERATURE1, &CDlgThermometry::OnBnClickedBtnSetTemperature1)
    ON_BN_CLICKED(IDC_BTN_SET_TEMPERATURE2, &CDlgThermometry::OnBnClickedBtnSetTemperature2)
    ON_BN_CLICKED(IDC_BTN_SET_TEMPERATURE3, &CDlgThermometry::OnBnClickedBtnSetTemperature3)
    ON_BN_CLICKED(IDC_BTN_SET_TEMPERATURE4, &CDlgThermometry::OnBnClickedBtnSetTemperature4)
    ON_MESSAGE(MSG_SET_TEMP_INFO, SetTemperaturePointInfo)
    ON_BN_CLICKED(IDC_BTN_IMPORT, &CDlgThermometry::OnBnClickedBtnImport)
    ON_BN_CLICKED(IDC_BTN_GET_CORRECTION_PARAM, &CDlgThermometry::OnBnClickedBtnGetCorrectionParam)
    ON_BN_CLICKED(IDC_BTN_STRAT_CORRECT, &CDlgThermometry::OnBnClickedBtnStratCorrect)

    ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



// CDlgThermometry 消息处理程序




void CDlgThermometry::DrawRgnFun(HDC hDc)
{
    DRAW_POLYGON_PARAM_EX struPolygonParam;
    DRAW_POLYGON_PARAM_EX struITCPolygonParam;
    DRAW_LINE_PARAM  struLaneLineParam;
        memset(&struPolygonParam, 0, sizeof(struPolygonParam));
        struPolygonParam.color = RGB(255, 0, 0);
        memcpy(&struPolygonParam.rcWnd, &m_rcPlayWnd, sizeof(m_rcPlayWnd));
        struPolygonParam.pArryPt = m_struPolyGon.struPos;
        struPolygonParam.dwPtNum = m_struPolyGon.dwPointNum;
        // CDrawFun::DrawPolygonEx(hDc, &struPolygonParam);

        //测试

        DRAW_POINT_PARAM  struPoint;
        memset(&struPoint, 0, sizeof(struPoint));
        struPoint.color = RGB(255, 0, 0);

        CPoint point;

        struPoint.point.x = m_struPoint.fX*  m_rcPlayWnd.Width() + USERDEFINE_VALUE;
        struPoint.point.y = m_struPoint.fY * m_rcPlayWnd.Height() + USERDEFINE_VALUE;
        CDrawFun::DrawPoint(hDc, &struPoint);
}



void CDlgThermometry::OnBnClickedButton10()
{
    UpdateData(TRUE);

    DWORD dwReturned = 0;
    memset(&m_struTempInfo, 0, sizeof(m_struTempInfo));
    if (!NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_THERMOMETRYRULE_TEMPERATURE_INFO, m_RuleID, &m_struTempInfo, sizeof(m_struTempInfo), &dwReturned))
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_GET_THERMOMETRYRULE_TEMPERATURE_INFO");
        MessageBox(NET_DVR_GetErrorMsg());
        return;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_GET_THERMOMETRYRULE_TEMPERATURE_INFO");
    }

    m_Max = m_struTempInfo.fMaxTemperature;
    m_Min = m_struTempInfo.fMinTemperature;
    m_Ave = m_struTempInfo.fAverageTemperature;

    m_max_x = m_struTempInfo.struHighestPoint.fX;
    m_max_y = m_struTempInfo.struHighestPoint.fY;

    m_min_x = m_struTempInfo.struLowestPoint.fX;
    m_min_y = m_struTempInfo.struLowestPoint.fY;
    if (0 == m_struTempInfo.byIsFreezedata)
    {
        m_Freezedata = "否";
    }
    else if (1 == m_struTempInfo.byIsFreezedata)
    {
        m_Freezedata = "是";
    }

    UpdateData(FALSE);
}


void CDlgThermometry::OnBnClickedBtnSetFile()
{
    UpdateData(TRUE);

    char szLan[1024] = { 0 };
    static char szFilter[] = "All File(*.*)|*.*||";
    CFileDialog dlg(TRUE, "*.*", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
    if (dlg.DoModal() == IDOK)
    {
        m_strFilePath = dlg.GetPathName();
        SetDlgItemText(IDC_EDIT_FILE, m_strFilePath);
    }
}

DWORD temperatureInfo(LPARAM lParam)
{
    CDlgThermometry *pThis = (CDlgThermometry *)lParam;

    //DWORD dwCurrentIndex = pThis->m_dwCurrentIndex;

    //DWORD dwCurrentIndex = DWORD(wParam);
    char szUrl[256] = { 0 };
    char szLan[64] = { 0 };
    char szInput[1024] {0};

    sprintf(szUrl, "GET /ISAPI/Thermal/channels/<ID>/CorrectionParam?format=json\r\n");
    char szdistance[128] = { 0 };
    char szenviroTemperature[128] = { 0 };
    char szemissivity[128] = { 0 };
    char szTemperaturePointList[256] = { 0 };
    char szTemperaturePoint[256] = { 0 };
    char szid[128] = { 0 };
    char szpresetTemperature[128] = { 0 };
    char szCoordinates[128] = { 0 };

    sprintf(szdistance, "<distance>%f</distance>\r\n", pThis->m_fDistance);
    sprintf(szenviroTemperature, "<enviroTemperature>%f</enviroTemperature>\r\n", pThis->m_fEnviroTemperature);
    sprintf(szemissivity, "<emissivity>%f</emissivity>\r\n", pThis->m_fCorrEmissivity);

    sprintf(szid, "<id>%d</id>\r\n", pThis->m_dwCurrentIndex + 1);
    sprintf(szpresetTemperature, "<presetTemperature>%f</presetTemperature>\r\n", pThis->m_fPreTemperature[pThis->m_dwCurrentIndex]);
    sprintf(szCoordinates, "<Coordinates>\r\n<x>%f</x>\r\n<y>%f</y>\r\n</Coordinates>\r\n", pThis->m_fCoordinatesX[pThis->m_dwCurrentIndex], pThis->m_fCoordinatesY[pThis->m_dwCurrentIndex]);
    sprintf(szTemperaturePoint, "<TemperaturePoint>\r\n%s%s%s</TemperaturePoint>\r\n", szid, szpresetTemperature, szCoordinates);
    sprintf(szTemperaturePointList, "<TemperaturePointList>\r\n%s</TemperaturePointList>\r\n", szTemperaturePoint);


    sprintf(szInput, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n <CorrectionParam version=\"2.0\" xmlns=\"http://www.isapi.org/ver20/XMLSchema\">\r\n %s%s%s%s\r\n", szdistance, szenviroTemperature, szemissivity, szTemperaturePointList);
    sprintf(szInput, "%s</CorrectionParam>\r\n", szInput);

    //组装json数据

    //测试
    sprintf(szUrl, "PUT /ISAPI/Thermal/channels/%d/CorrectionParam", pThis->m_lChannel);
    pThis->m_struInput.dwSize = sizeof(pThis->m_struInput);
    pThis->m_struInput.lpRequestUrl = szUrl;
    pThis->m_struInput.dwRequestUrlLen = strlen(szUrl);
    pThis->m_struInput.lpInBuffer = szInput;
    pThis->m_struInput.dwInBufferSize = strlen(szInput);
    pThis->m_struInput.dwRecvTimeOut = 30000;//0秒超时,设置温度点设备返回比较慢，在15秒左右
    pThis->m_struOuput.dwSize = sizeof(pThis->m_struOuput);
    pThis->m_struOuput.lpOutBuffer = pThis->m_pBuffer;
    pThis->m_struOuput.dwOutBufferSize = MAX_LEN_XML;

    BOOL bStatus = FALSE;
    if (!NET_DVR_STDXMLConfig(pThis->m_lUserID, &pThis->m_struInput, &pThis->m_struOuput))
    {

        bStatus = FALSE;
    }
    else
    {
        bStatus = TRUE;
    }
    HWND hWnd = pThis->GetSafeHwnd();
    if (NULL == hWnd)
    {
        return 0;
    }
    ::SendMessage(hWnd, MSG_SET_TEMP_INFO, WPARAM(&bStatus), NULL);
    return 0;
}

LRESULT CDlgThermometry::SetTemperaturePointInfo(WPARAM wParam, LPARAM lParam)
{
    BOOL *pStatus = (BOOL *)wParam;
    if (!*pStatus)
    {
        char szLan[64] = { 0 };
        CString csPrompt;
        sprintf(szLan, "设置测温矫正参数失败：");
        csPrompt.Format("%s[%d]", szLan, NET_DVR_GetLastError());
        AfxMessageBox(csPrompt);

    }
    else
    {
        switch (m_dwCurrentIndex)
        {
        case 0:
            m_bStatus1 = TRUE;
            break;
        case 1:
            m_bStatus2 = TRUE;
            break;
        case 2:
            m_bStatus3 = TRUE;
            break;
        case 3:
            m_bStatus4 = TRUE;
            break;
        default:
            break;
        }

        if (m_bStatus1 &&m_bStatus2 &&m_bStatus3 &&m_bStatus4)
        {
            m_btnStartCorrect.EnableWindow(TRUE);

        }
        else
        {
            m_btnStartCorrect.EnableWindow(FALSE);
        }
    }



    GetDlgItem(IDC_BTN_SET_TEMPERATURE1)->EnableWindow(TRUE);
    GetDlgItem(IDC_BTN_SET_TEMPERATURE2)->EnableWindow(TRUE);
    GetDlgItem(IDC_BTN_SET_TEMPERATURE3)->EnableWindow(TRUE);
    GetDlgItem(IDC_BTN_SET_TEMPERATURE4)->EnableWindow(TRUE);
    UpdateData(FALSE);
    return TRUE;
}

void CDlgThermometry::SetTemperatureInfo(DWORD dwCurrentIndex, float fPreTemperature)
{
    UpdateData(TRUE);

    m_dwCurrentIndex = dwCurrentIndex;
    m_fPreTemperature[m_dwCurrentIndex] = fPreTemperature;
    m_fCoordinatesX[m_dwCurrentIndex] = m_struPoint.fX;
    m_fCoordinatesY[m_dwCurrentIndex] = m_struPoint.fY;

    //创建线程，处理设置
    DWORD dwThreadID = 0;
    HANDLE hThread = CreateThread(NULL, 0, LPTHREAD_START_ROUTINE(temperatureInfo), this, 0, &dwThreadID);
    if (hThread == NULL)
    {
        char szLan[256] = { 0 };
        g_StringLanType(szLan, "打开线程失败!", "Fail to open  thread!");
        AfxMessageBox(szLan);
        return;
    }

    CloseHandle(hThread);
    hThread = NULL;

    UpdateData(FALSE);
}

void CDlgThermometry::OnBnClickedBtnSetTemperature1()
{
    // TODO:  在此添加控件通知处理程序代码
    GetDlgItem(IDC_BTN_SET_TEMPERATURE1)->EnableWindow(FALSE);
    SetTemperatureInfo(0, m_fPreTemperature1);
}


void CDlgThermometry::OnBnClickedBtnSetTemperature2()
{
    GetDlgItem(IDC_BTN_SET_TEMPERATURE2)->EnableWindow(FALSE);
    SetTemperatureInfo(1, m_fPreTemperature2);
}


void CDlgThermometry::OnBnClickedBtnSetTemperature3()
{
    GetDlgItem(IDC_BTN_SET_TEMPERATURE3)->EnableWindow(FALSE);
    SetTemperatureInfo(2, m_fPreTemperature3);
}


void CDlgThermometry::OnBnClickedBtnSetTemperature4()
{
    GetDlgItem(IDC_BTN_SET_TEMPERATURE4)->EnableWindow(FALSE);
    SetTemperatureInfo(3, m_fPreTemperature4);
}


void CDlgThermometry::OnBnClickedBtnImport()
{
    BYTE *m_pPicInfo = NULL;
    //读取文件
    char szLan[128] = { 0 };
    memset(&m_struInput, 0, sizeof(m_struInput));
    memset(&m_struOuput, 0, sizeof(m_struOuput));
    CFile cPicFile;
    if (!cPicFile.Open(m_strFilePath, CFile::modeRead))
    {
        g_StringLanType(szLan, "打开文件失败或无此文件", "Open file failed or no this file");
        AfxMessageBox(szLan);
    }
    else
    {
        m_struInput.dwInBufferSize = (DWORD)cPicFile.GetLength();
        if (m_struInput.dwInBufferSize == 0)
        {
            g_StringLanType(szLan, "Pic文件为空", "Pic file is empty");
            AfxMessageBox(szLan);
        }

        m_pPicInfo = new BYTE[m_struInput.dwInBufferSize];
        cPicFile.Read(m_pPicInfo, m_struInput.dwInBufferSize);
        m_struInput.lpInBuffer = m_pPicInfo;

        cPicFile.Close();
    }


    char szUrl[256] = { 0 };
    char szTest[64] = { 0 };

    sprintf(szUrl, "PUT /ISAPI/Thermal/channels/%d/CorrectionParam/import\r\n", m_lChannel);


    //测试
    //sprintf(szUrl, "GET /ISAPI/System/capabilities");

    m_struInput.dwSize = sizeof(m_struInput);
    m_struInput.lpRequestUrl = szUrl;
    m_struInput.dwRequestUrlLen = strlen(szUrl);
    m_struOuput.dwSize = sizeof(m_struOuput);
    m_struOuput.lpOutBuffer = m_pBuffer;
    m_struOuput.dwOutBufferSize = MAX_LEN_XML;

    CString csPrompt;
    if (!NET_DVR_STDXMLConfig(m_lUserID, &m_struInput, &m_struOuput))
    {

        sprintf(szTest, "导入测温矫正参数失败：");
        csPrompt.Format("%s[%d]", szTest, NET_DVR_GetLastError());
        AfxMessageBox(csPrompt);
        //return;
    }
    else
    {
        //导入成功，获取参数
        OnBnClickedBtnGetCorrectionParam();
        sprintf(szTest, "导入测温矫正参数成功!");
        csPrompt.Format("%s", szTest);
        AfxMessageBox(csPrompt);
    }

    //开始矫正按钮置为不可选
    m_btnStartCorrect.EnableWindow(FALSE);
    UpdateData(FALSE);
    if (NULL != m_pPicInfo)
    {
        delete[]m_pPicInfo;
        m_pPicInfo = NULL;
    }
}

void CDlgThermometry::ConvertModeXMLNodeToShow(const char* pXMLBuf)
{
    CXmlBase struXml;
    if (struXml.Parse(pXMLBuf) == NULL)
    {
        return;
    }
    CString strTest = "";
    if (struXml.FindElem("CorrectionParam") && struXml.IntoElem())
    {

        //解析distance
        if (struXml.FindElem("distance"))
        {
            CString strdistance = struXml.GetData().c_str();
            m_fDistance = atof(strdistance);
        }
        //解析enviroTemperature
        if (struXml.FindElem("enviroTemperature"))
        {
            CString strenviroTemperature = struXml.GetData().c_str();
            m_fEnviroTemperature = atof(strenviroTemperature);
        }
        //解析发射率
        if (struXml.FindElem("emissivity"))
        {
            CString stremissivity = struXml.GetData().c_str();
            m_fCorrEmissivity = atof(stremissivity);
        }
        //解析温度点list
        if (struXml.FindElem("TemperaturePointList") && struXml.IntoElem())
        {
            int i = 0;
            //解析温度点
            while (struXml.FindElem("TemperaturePoint") && struXml.IntoElem())
            {
                if (struXml.FindElem("id"))
                {
                    CString strid = struXml.GetData().c_str();
                    int iID = atoi(strid);
                }
                if (struXml.FindElem("presetTemperature"))
                {
                    CString strpresetTemperature = struXml.GetData().c_str();
                    m_fPreTemperature[i] = atof(strpresetTemperature);
                }
                //解析温度点坐标
                if (struXml.FindElem("Coordinates") && struXml.IntoElem())
                {
                    if (struXml.FindElem("x"))
                    {
                        CString strX = struXml.GetData().c_str();
                        m_fCoordinatesX[i] = atof(strX);
                    }
                    if (struXml.FindElem("y"))
                    {
                        CString strY = struXml.GetData().c_str();
                        m_fCoordinatesY[i] = atof(strY);
                    }
                    struXml.OutOfElem();
                }
                i++;
                struXml.OutOfElem();
                if (!struXml.NextSibElem())
                {
                    break;
                }

            }
            m_fPreTemperature1 = m_fPreTemperature[0];
            m_fPreTemperature2 = m_fPreTemperature[1];
            m_fPreTemperature3 = m_fPreTemperature[2];
            m_fPreTemperature4 = m_fPreTemperature[3];

        }


    }
}   

void CDlgThermometry::OnBnClickedBtnGetCorrectionParam()
{
    UpdateData(TRUE);

    //Initbuff();
    char szUrl[256] = { 0 };
    char szLan[64] = { 0 };

    sprintf(szUrl, "GET /ISAPI/Thermal/channels/%d/CorrectionParam\r\n", m_lChannel);


    //测试
    //sprintf(szUrl, "GET /ISAPI/System/capabilities");
    memset(&m_struInput, 0, sizeof(m_struInput));
    memset(&m_struOuput, 0, sizeof(m_struOuput));
    m_struInput.dwSize = sizeof(m_struInput);
    m_struInput.lpRequestUrl = szUrl;
    m_struInput.dwRequestUrlLen = strlen(szUrl);
    m_struOuput.dwSize = sizeof(m_struOuput);
    m_struOuput.lpOutBuffer = m_pBuffer;
    m_struOuput.dwOutBufferSize = MAX_LEN_XML;

    if (!NET_DVR_STDXMLConfig(m_lUserID, &m_struInput, &m_struOuput))
    {
        CString csPrompt;
        sprintf(szLan, "获取信息失败：");
        csPrompt.Format("%s[%d]", szLan, NET_DVR_GetLastError());
        AfxMessageBox(csPrompt);
        return;
    }

    //解析json数据
    // ConvertModeJSONNodeToShow((char *)m_struOuput.lpOutBuffer);

    //解析XML数据
    ConvertModeXMLNodeToShow((char *)m_struOuput.lpOutBuffer);

    UpdateData(FALSE);
}


void CDlgThermometry::OnBnClickedBtnStratCorrect()
{
    // TODO:  在此添加控件通知处理程序代码
    char szUrl[256] = { 0 };
    char szLan[64] = { 0 };

    sprintf(szUrl, "PUT /ISAPI/Thermal/channels/%d/Correction\r\n", m_lChannel);


    //测试
    //sprintf(szUrl, "GET /ISAPI/System/capabilities");
    memset(&m_struInput, 0, sizeof(m_struInput));
    //memset(&m_struOuput, 0, sizeof(m_struOuput));
    m_struInput.dwSize = sizeof(m_struInput);
    m_struInput.lpRequestUrl = szUrl;
    m_struInput.dwRequestUrlLen = strlen(szUrl);
    m_struOuput.dwSize = sizeof(m_struOuput);
    m_struOuput.lpOutBuffer = m_pBuffer;
    m_struOuput.dwOutBufferSize = MAX_LEN_XML;

    //无论矫正成功、失败，都重置状态
    m_bStatus1 = FALSE;
    m_bStatus2 = FALSE;
    m_bStatus3 = FALSE;
    m_bStatus4 = FALSE;
    if (!NET_DVR_STDXMLConfig(m_lUserID, &m_struInput, &m_struOuput))
    {
        CString csPrompt;
        sprintf(szLan, "测试矫正失败：");
        csPrompt.Format("%s[%d]", szLan, NET_DVR_GetLastError());
        AfxMessageBox(csPrompt);
        //return;
    }
    else
    {
        CString csPrompt;
        sprintf(szLan, "测试矫正成功!");
        csPrompt.Format("%s", szLan);
        AfxMessageBox(csPrompt);

    }
    UpdateData(FALSE);
}


BOOL CDlgThermometry::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    StartRealPlay();
    GetDlgItem(IDC_PLAYWND)->GetWindowRect(&m_rcPlayWnd);
    ScreenToClient(&m_rcPlayWnd);

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常:  OCX 属性页应返回 FALSE
}


void CDlgThermometry::OnLButtonDown(UINT nFlags, CPoint point)
{
    UpdateData(TRUE);

    //m_bDrawEnd = FALSE;

    if (!m_rcPlayWnd.PtInRect(point))
    {
        return;
    }


    NET_VCA_POINT struPoint;
    struPoint.fX = (float)(point.x - m_rcPlayWnd.left) / (float)m_rcPlayWnd.Width();
    struPoint.fY = (float)(point.y - m_rcPlayWnd.top) / (float)m_rcPlayWnd.Height();
    //测试
    memcpy(&m_struPoint, &struPoint, sizeof(m_struPoint));

    CDialogEx::OnLButtonDown(nFlags, point);
}
