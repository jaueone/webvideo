#ifndef HCNET_H
#define HCNET_H

#include <QObject>
#include "HCNetSDK.h"
#include <QMessageBox>
#include <QPushButton>
#include <QVector>


void MessageBoxs(QMessageBox::Icon icon,QString info,QString content);


class HCNet : public QObject
{
    Q_OBJECT
public:
    explicit HCNet(QObject *parent = nullptr);
    ~HCNet();
    bool InitInterface(QString ip, QString port, QString user, QString password);

    void display_video(HWND ID, LONG channel);

    bool start_video(LONG channel,QString name);    //开始录像
    void stop_video(LONG channel);              //停止录像

    bool CapturePicture(CAPTURE_MODE model, LONG channel ,QString name);        //抓图
    bool SetVideoEffect(LONG chanel, int brightness, int contrast, int saturation, int chroma);     //设置亮度对比度视频参数
    QVector<int> GetVideoEffect(LONG chanel);       //获取视频亮度对比度等参数

    void ExitInterface();

    LONG lUserID = NULL;    //用户句柄
    LONG lRealPlayHandle = NULL;    //可见光通道
    LONG lRealPlayHandle_hot = NULL;    //热成像通道

    void display_channel();

    bool SetPIP(LPNET_DVR_THERMAL_PIP pip);  //配置画中画

    bool SetDVRConfig(LONG channel,LPVOID lpInBuffer, DWORD size = 132);    //设置前端参数    曝光，背光，白平衡，日夜转换等

    LPNET_DVR_CAMERAPARAMCFG GetDVRConfig(LONG channel);        //获取前端参数    曝光，背光，白平衡，日夜转换等
    LPNET_DVR_THERMOMETRY_BASICPARAM GetTempConfig();       //获取温度配置参数    曝光，背光，白平衡，日夜转换等
    bool SetTempConfig(LPNET_DVR_THERMOMETRY_BASICPARAM config);
    bool GetSTDAbility(LONG userid, DWORD dwAbilityType, LPNET_DVR_STD_ABILITY lpAbilityParam);

    NET_DVR_SHOWSTRINGINFO HCNet::OSDINFO(const char *str, int x, int y);
signals:

public slots:
};

#endif // HCNET_H
