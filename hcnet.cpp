#include "hcnet.h"
#include "Logger.h"
#include <QDebug>

void MessageBoxs(QMessageBox::Icon icon,QString info,QString content){
    QMessageBox messageBox;
    messageBox.setWindowTitle(info);
    messageBox.setIcon(icon);
    messageBox.setText(content);
    QPushButton button(QString::fromLocal8Bit("确定"));
    messageBox.addButton(&button, QMessageBox::YesRole);
    messageBox.exec();
}

HCNet::HCNet(QObject *parent) : QObject(parent)
{
    NET_DVR_Init();
}

HCNet::~HCNet()
{
    NET_DVR_Cleanup();
}

bool HCNet::InitInterface(QString ip, QString port, QString user, QString password)
{

    //登录参数，包括设备地址、登录用户、密码等
    NET_DVR_USER_LOGIN_INFO struLoginInfo = {0};
    struLoginInfo.bUseAsynLogin = 0; //同步登录方式
    strcpy(struLoginInfo.sDeviceAddress, ip.toLatin1().data()); //设备IP地址
    struLoginInfo.wPort = port.toInt(); //设备服务端口
    strcpy(struLoginInfo.sUserName, user.toLatin1().data()); //设备登录用户名
    strcpy(struLoginInfo.sPassword, password.toLatin1().data()); //设备登录密码

    //设备信息, 输出参数
    NET_DVR_DEVICEINFO_V40 struDeviceInfoV40 = {0};

    lUserID = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfoV40);
    if (lUserID < 0)
    {
        qDebug("Login failed, error code: %d\n", NET_DVR_GetLastError());
        NET_DVR_Cleanup();
        return false;
    }
    qDebug("The max number of analog channels: %d %d\n",struDeviceInfoV40.struDeviceV30.byChanNum, struDeviceInfoV40.struDeviceV30.byStartChan); //模拟通道个数
    qDebug("The max number of IP channels: %d\n", struDeviceInfoV40.struDeviceV30.byIPChanNum + struDeviceInfoV40.struDeviceV30.byHighDChanNum * 256);//IP通道个数
    //---------------------------------------
    return true;
}

void HCNet::display_video(HWND ID, LONG channel)
{
    //---------------------------------------
    //启动预览并设置回调数据流
    LONG lRealPlayHandles;

    NET_DVR_PREVIEWINFO struPlayInfo = {0};
    struPlayInfo.hPlayWnd = ID;         //需要SDK解码时句柄设为有效值，仅取流不解码时可设为空    //获取窗口句柄
    struPlayInfo.lChannel     = channel;       //预览通道号
    struPlayInfo.dwStreamType = 0;       //0-主码流，1-子码流，2-码流3，3-码流4，以此类推
    struPlayInfo.dwLinkMode   = 0;       //0- TCP方式，1- UDP方式，2- 多播方式，3- RTP方式，4-RTP/RTSP，5-RSTP/HTTP
    struPlayInfo.bBlocked     = 1;       //0- 非阻塞取流，1- 阻塞取流

    lRealPlayHandles = NET_DVR_RealPlay_V40(lUserID, &struPlayInfo, NULL, NULL);
    if (channel == 1)
        this->lRealPlayHandle = lRealPlayHandles;
    else if (channel == 2)
        this->lRealPlayHandle_hot = lRealPlayHandles;

    if (lRealPlayHandles < 0)
    {
        qDebug("NET_DVR_RealPlay_V40 error %d\n", NET_DVR_GetLastError());
        NET_DVR_Logout(lUserID);
        NET_DVR_Cleanup();
        return;
    }
    NET_DVR_SetAudioMode(1);
    NET_DVR_OpenSound(this->lRealPlayHandle);
}

bool HCNet::start_video(LONG channel, QString name)
{
    LONG realhandle = NULL;
    if (channel == 1)
        realhandle = this->lRealPlayHandle;
    else if (channel == 2)
        realhandle = this->lRealPlayHandle_hot;
    bool ok = NET_DVR_SaveRealData_V30(realhandle,STREAM_TYPE::STREAM_3GPP, name.toLatin1().data());
    return ok;
}

void HCNet::stop_video(LONG channel)
{
    LONG realhandle = NULL;
    if (channel == 1)
        realhandle = this->lRealPlayHandle;
    else if (channel == 2)
        realhandle = this->lRealPlayHandle_hot;
    NET_DVR_StopSaveRealData(realhandle);
}

bool HCNet::CapturePicture(CAPTURE_MODE model, LONG channel, QString name)
{
    LONG realhandle = NULL;
    if (channel == 1)
        realhandle = this->lRealPlayHandle;
    else if (channel == 2)
        realhandle = this->lRealPlayHandle_hot;
    NET_DVR_SetCapturePictureMode(model);

    return  NET_DVR_CapturePicture(realhandle,name.toLatin1().data());

}

bool HCNet::SetVideoEffect(LONG chanel, int brightness, int contrast, int saturation, int chroma)
{
    LONG realhandle = NULL;
    if (chanel == 1)
        realhandle = this->lRealPlayHandle;
    else if (chanel == 2)
        realhandle = this->lRealPlayHandle_hot;
    return NET_DVR_ClientSetVideoEffect(realhandle,brightness, contrast, saturation,chroma);
}

QVector<int> HCNet::GetVideoEffect(LONG chanel)
{
    QVector<int> vec;
    LONG realhandle = NULL;
    if (chanel == 1)
        realhandle = this->lRealPlayHandle;
    else if (chanel == 2)
        realhandle = this->lRealPlayHandle_hot;
    DWORD *pBrightValue;
    DWORD *pContrastValue;
    DWORD *pSaturationValue;
    DWORD *pHueValue;
    qDebug() << vec;
    if (!NET_DVR_ClientGetVideoEffect(realhandle,pBrightValue,pContrastValue,pSaturationValue,pHueValue)){
        return vec;
    }
    else {
        vec.append((int)(*pBrightValue));
        vec.append((int)(*pContrastValue));
        vec.append((int)(*pSaturationValue));
        vec.append((int)(*pHueValue));

        return vec;
    }
}





void HCNet::ExitInterface()
{
    NET_DVR_Logout(lUserID);
    NET_DVR_Cleanup();
}

void HCNet::display_channel()
{
    int i=0;
    BYTE byIPID,byIPIDHigh;
    int iDevInfoIndex, iGroupNO, iIPCh;
    DWORD dwReturned = 0;

    //---------------------------------------
    // 初始化
    NET_DVR_Init();
    //设置连接时间与重连时间
    NET_DVR_SetConnectTime(2000, 1);
    NET_DVR_SetReconnect(10000, true);

    //---------------------------------------
    // 注册设备
    LONG lUserID;

    //登录参数，包括设备地址、登录用户、密码等
    NET_DVR_USER_LOGIN_INFO struLoginInfo = {0};
    struLoginInfo.bUseAsynLogin = 0; //同步登录方式
    strcpy(struLoginInfo.sDeviceAddress, "192.0.0.64"); //设备IP地址
    struLoginInfo.wPort = 8000; //设备服务端口
    strcpy(struLoginInfo.sUserName, "admin"); //设备登录用户名
    strcpy(struLoginInfo.sPassword, "abcd1234"); //设备登录密码

    //设备信息, 输出参数
    NET_DVR_DEVICEINFO_V40 struDeviceInfoV40 = {0};

    lUserID = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfoV40);
    if (lUserID < 0)
    {
        qDebug("Login failed, error code: %d\n", NET_DVR_GetLastError());
        NET_DVR_Cleanup();
        return;
    }

    qDebug("The max number of analog channels: %d\n",struDeviceInfoV40.struDeviceV30.byChanNum); //模拟通道个数
    qDebug("The max number of IP channels: %d\n", struDeviceInfoV40.struDeviceV30.byIPChanNum + struDeviceInfoV40.struDeviceV30.byHighDChanNum * 256);//IP通道个数

    //获取IP通道参数信息
    NET_DVR_IPPARACFG_V40 IPAccessCfgV40;
    memset(&IPAccessCfgV40, 0, sizeof(NET_DVR_IPPARACFG));
    iGroupNO=0;
    if (!NET_DVR_GetDVRConfig(lUserID, NET_DVR_GET_IPPARACFG_V40, iGroupNO, &IPAccessCfgV40, sizeof(NET_DVR_IPPARACFG_V40), &dwReturned))
    {
        qDebug("NET_DVR_GET_IPPARACFG_V40 error, %d\n", NET_DVR_GetLastError());
        NET_DVR_Logout(lUserID);
        NET_DVR_Cleanup();
        return;
    }
    else
    {
        for (i=0;i<IPAccessCfgV40.dwDChanNum;i++)
        {
            switch(IPAccessCfgV40.struStreamMode[i].byGetStreamType)
            {
            case 0: //直接从设备取流
                if (IPAccessCfgV40.struStreamMode[i].uGetStream.struChanInfo.byEnable)
                {
                    byIPID=IPAccessCfgV40.struStreamMode[i].uGetStream.struChanInfo.byIPID;
                    byIPIDHigh=IPAccessCfgV40.struStreamMode[i].uGetStream.struChanInfo.byIPIDHigh;
                    iDevInfoIndex=byIPIDHigh*256 + byIPID-1-iGroupNO*64;
                    qDebug("IP channel no.%d is online, IP: %s\n", i+1, IPAccessCfgV40.struIPDevInfo[iDevInfoIndex].struIP.sIpV4);
                }
                break;
            case 1: //从流媒体取流
                if (IPAccessCfgV40.struStreamMode[i].uGetStream.struPUStream.struStreamMediaSvrCfg.byValid)
                {
                    qDebug("IP channel %d connected with the IP device by stream server.\n", i+1);
                    qDebug("IP of stream server: %s, IP of IP device: %s\n",IPAccessCfgV40.struStreamMode[i].uGetStream.\
                    struPUStream.struStreamMediaSvrCfg.struDevIP.sIpV4, IPAccessCfgV40.struStreamMode[i].uGetStream.\
                    struPUStream.struDevChanInfo.struIP.sIpV4);
                }
                break;
            default:
                break;
            }
        }
    }

    //配置IP通道5;
    iIPCh=4;

    //支持自定义协议
    NET_DVR_CUSTOM_PROTOCAL struCustomPro;
    if (!NET_DVR_GetDVRConfig(lUserID, NET_DVR_GET_CUSTOM_PRO_CFG, 1, &struCustomPro, sizeof(NET_DVR_CUSTOM_PROTOCAL), &dwReturned))
    //获取自定义协议1
    {
        qDebug("NET_DVR_GET_CUSTOM_PRO_CFG error, %d\n", NET_DVR_GetLastError());
        NET_DVR_Logout(lUserID);
        NET_DVR_Cleanup();
        return;
    }
    struCustomPro.dwEnabled=1;   //启用主码流
    struCustomPro.dwEnableSubStream=1; //启用子码流
    strcpy((char *)struCustomPro.sProtocalName,"Protocal_RTSP");  //自定义协议名称:Protocal_RTSP,最大16字节
    struCustomPro.byMainProType=1;    //主码流协议类型: 1- RTSP
    struCustomPro.byMainTransType=2;  //主码流传输协议: 0- Auto, 1- udp, 2- rtp over rtsp
    struCustomPro.wMainPort=554;     //主码流取流端口
    strcpy((char *)struCustomPro.sMainPath,"rtsp://192.168.1.65/h264/ch1/main/av_stream");//主码流取流URL
    struCustomPro.bySubProType=1;    //子码流协议类型: 1- RTSP
    struCustomPro.bySubTransType=2;  //子码流传输协议: 0- Auto, 1- udp, 2- rtp over rtsp
    struCustomPro.wSubPort=554;     //子码流取流端口
    strcpy((char *)struCustomPro.sSubPath,"rtsp://192.168.1.65/h264/ch1/sub/av_stream");//子码流取流URL

    if (!NET_DVR_SetDVRConfig(lUserID, NET_DVR_SET_CUSTOM_PRO_CFG, 1, &struCustomPro, sizeof(NET_DVR_CUSTOM_PROTOCAL)))
    //设置自定义协议1
    {
        qDebug("NET_DVR_SET_CUSTOM_PRO_CFG error, %d\n", NET_DVR_GetLastError());
        NET_DVR_Logout(lUserID);
        NET_DVR_Cleanup();
        return;
    }

    qDebug("Set the custom protocol: %s\n", "Protocal_RTSP");
    NET_DVR_IPC_PROTO_LIST m_struProtoList;
    if (!NET_DVR_GetIPCProtoList(lUserID, &m_struProtoList)) //获取设备支持的前端协议
    {
        qDebug("NET_DVR_GetIPCProtoList error, %d\n", NET_DVR_GetLastError());
        NET_DVR_Logout(lUserID);
        NET_DVR_Cleanup();
        return;
    }

    IPAccessCfgV40.struIPDevInfo[iIPCh].byEnable=1;     //启用
    for (i = 0; i<m_struProtoList.dwProtoNum; i++)
    {
        if(strcmp((char *)struCustomPro.sProtocalName,(char *)m_struProtoList.struProto[i].byDescribe)==0)
        {
            IPAccessCfgV40.struIPDevInfo[iIPCh].byProType=m_struProtoList.struProto[i].dwType; //选择自定义协议
            break;
        }
    }

    //IPAccessCfgV40.struIPDevInfo[iIPCh].byProType=0;  //厂家私有协议
    strcpy((char *)IPAccessCfgV40.struIPDevInfo[iIPCh].struIP.sIpV4,"192.168.1.65"); //前端IP设备的IP地址
    IPAccessCfgV40.struIPDevInfo[iIPCh].wDVRPort=8000;  //前端IP设备服务端口
    strcpy((char *)IPAccessCfgV40.struIPDevInfo[iIPCh].sUserName,"admin");  //前端IP设备登录用户名
    strcpy((char *)IPAccessCfgV40.struIPDevInfo[iIPCh].sPassword,"12345");  //前端IP设备登录密码

    IPAccessCfgV40.struStreamMode[iIPCh].byGetStreamType=0;
    IPAccessCfgV40.struStreamMode[iIPCh].uGetStream.struChanInfo.byChannel=1;
    IPAccessCfgV40.struStreamMode[iIPCh].uGetStream.struChanInfo.byIPID=(iIPCh+1)%256;
    IPAccessCfgV40.struStreamMode[iIPCh].uGetStream.struChanInfo.byIPIDHigh=(iIPCh+1)/256;

    //IP通道配置，包括添加、删除、修改IP通道等
    if (!NET_DVR_SetDVRConfig(lUserID, NET_DVR_SET_IPPARACFG_V40, iGroupNO, &IPAccessCfgV40, sizeof(NET_DVR_IPPARACFG_V40)))
    {
        qDebug("NET_DVR_SET_IPPARACFG_V40 error, %d\n", NET_DVR_GetLastError());
        NET_DVR_Logout(lUserID);
        NET_DVR_Cleanup();
        return;
    }
    else
    {
        qDebug("Set IP channel no.%d, IP: %s\n", iIPCh+1, IPAccessCfgV40.struIPDevInfo[iIPCh].struIP.sIpV4);
    }

    //注销用户
    NET_DVR_Logout(lUserID);

    //释放SDK资源
    NET_DVR_Cleanup();
}

bool HCNet::SetPIP(LPNET_DVR_THERMAL_PIP pip)
{
    LPNET_DVR_STD_CONFIG config;
    config->lpCondBuffer = NULL;
    config->dwCondSize = 0;
    config->lpInBuffer = (void*)pip;
    config->dwInSize = pip->dwSize;
    config->lpOutBuffer = NULL;
    config->dwOutSize = 0;
    config->lpStatusBuffer = NULL;
    config->dwStatusSize = 0;
    config->byDataType = 0;

    if (!NET_DVR_SetSTDConfig(lUserID,NET_DVR_SET_THERMAL_PIP,config)){
        return false;
    }
    if (config->lpStatusBuffer != NULL)
        return false;
    return true;
}

bool HCNet::SetDVRConfig(LONG channel, LPVOID lpInBuffer, DWORD size)
{
    return NET_DVR_SetDVRConfig(lUserID,NET_DVR_SET_CCDPARAMCFG,channel,lpInBuffer,size);
}

LPNET_DVR_CAMERAPARAMCFG HCNet::GetDVRConfig(LONG channel)
{
    LPNET_DVR_CAMERAPARAMCFG front_config;
    front_config->dwSize = 132;
    LPVOID out_config;    //获取数据指针
    LPDWORD out_size;       //获取到实际长度指针
    if (!NET_DVR_GetDVRConfig(lUserID,NET_DVR_GET_CCDPARAMCFG,channel,out_config,front_config->dwSize,out_size))    //获取前端设置函数
    {
        front_config->dwSize = 0;
        return front_config;
    }

    if (*out_size != front_config->dwSize)
        qDebug() << "获取数据长度不一致";
    front_config = (LPNET_DVR_CAMERAPARAMCFG)out_config;
    return front_config;
}

LPNET_DVR_THERMOMETRY_BASICPARAM HCNet::GetTempConfig()
{
    LPNET_DVR_THERMOMETRY_BASICPARAM config = NULL;

    LPNET_DVR_STD_CONFIG  lpConfigParam = new NET_DVR_STD_CONFIG;
    memset(lpConfigParam, 0, sizeof (NET_DVR_STD_CONFIG));

    if (!NET_DVR_GetSTDConfig(lUserID,NET_DVR_GET_THERMOMETRY_BASICPARAM,lpConfigParam))    //获取前端设置函数
    {
        config->dwSize = 0;
        return config;
    }

    if (lpConfigParam->dwOutSize != sizeof (NET_DVR_STD_CONFIG))
        qDebug() << "获取数据长度不一致";

    return (LPNET_DVR_THERMOMETRY_BASICPARAM)lpConfigParam->lpOutBuffer;
}

bool HCNet::SetTempConfig(LPNET_DVR_THERMOMETRY_BASICPARAM config)
{
    LPNET_DVR_STD_CONFIG  lpConfigParam = new NET_DVR_STD_CONFIG;
    memset(lpConfigParam, 0, sizeof (NET_DVR_STD_CONFIG));

    lpConfigParam->lpInBuffer = config;
    lpConfigParam->dwInSize = sizeof (NET_DVR_THERMOMETRY_BASICPARAM);

    return NET_DVR_SetSTDConfig(lUserID, NET_DVR_GET_THERMOMETRY_BASICPARAM,lpConfigParam);

}



bool HCNet::GetSTDAbility(LONG userid, DWORD dwAbilityType, LPNET_DVR_STD_ABILITY lpAbilityParam)
{
    return NET_DVR_GetSTDAbility(userid,dwAbilityType, lpAbilityParam);
}

NET_DVR_SHOWSTRINGINFO HCNet::OSDINFO(const char *str, int x, int y)
{
    NET_DVR_SHOWSTRINGINFO info;
    info.wShowString = 1;
    info.wStringSize = strlen(str);
    if (info.wStringSize > 44){
        LOG_CRIT << u8"数据超出OSD字符显示长度，将设置失败，实际长度:" << info.wStringSize << u8"最大长度：44";
    }
    info.wShowStringTopLeftX = x;
    info.wShowStringTopLeftY = y;
    strcpy(info.sString,str);
    return info;
}


