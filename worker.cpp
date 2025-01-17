﻿#include "worker.h"
#include "hcnet.h"
#include "HCNetSDK.h"
#include "Logger.h"
#include <string>
#include <sstream>
#include <QMap>
#include <QDateTime>
#include <QJsonObject>

void sleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

Worker::Worker(){
    this->serial = new QSerialPort(this);
    this->timer = new QTimer(this);
    camera = new HCNet;
//    if (camera->InitInterface("169.254.128.98","8000","admin","abcd1234"));
//    {
//        emit tell_window_msg("确保相机IP为169.254.128.98，账户admin密码abcd1234可以登陆\n并且和PC位于同一网段内，再重启软件");
//    }
//    this->init_setting_and_connect("COM7",115200);
}

Worker::~Worker(){
    camera->ExitInterface();
    delete camera;
}

void Worker::init_setting_and_connect(QString name, int baudrate)
{

    this->timer->setInterval(12000);
    serial->setPortName(name);
    serial->setBaudRate(115200);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);


    qRegisterMetaType<QSerialPort::SerialPortError>("QSerialPort::SerialPortError");


    this->connect(this->serial,&QSerialPort::readyRead,this,&Worker::accept_read_data);
    this->connect(this->timer,&QTimer::timeout,this,&Worker::accept_timeout);
    this->connect(this->serial,&QSerialPort::errorOccurred, this,&Worker::accept_serial_error);
    if(serial->open(QIODevice::ReadWrite)){
        serial->setDataTerminalReady(true);
        LOG_INFO << u8"串口打开成功----------------------------------------------------------------------";
        qDebug() << serial->isOpen() << "------------------------------------------------------------------------";
    }
    else {
        qDebug() << "open serial failed------------------------------------------------------------------------";
        LOG_CRIT << u8"串口打开失败";
        emit tell_window_msg(QString::fromLocal8Bit(("确保把串口连接到指定COM口，重新打开软件")));
    }
}


void Worker::setObject(QJsonObject object)
{
    QJsonObject cameras = object["Camera"].toObject();
    QJsonObject serials = object["Serial"].toObject();
    ip = cameras["ip"].toString();
    port = "8000";
    user = cameras["user"].toString();
    password = cameras["password"].toString();
    if (camera->InitInterface(ip,port,user,password))
    {
        LOG_INFO << u8"相机登陆成功----------------------------------------------------------------------";
    }
    else{
        emit tell_window_msg(QString::fromLocal8Bit("确保相机IP，账户密码可以登陆,并且和PC位于同一网段内，再重启软件"));
        LOG_CRIT << u8"相机登陆失败，无法传输显示数据";
    }

    this->init_setting_and_connect(serials["name"].toString(), serials["baudrate"].toInt());
}



QMap<int, QByteArray> Worker::load_data(const QByteArray &data)
{
    QMap<int, QByteArray> map = {};
    if (data.length() < 10)
         return map;
    QByteArray ba(data);

    QDataStream in(&ba,QIODevice::ReadOnly);
    uint16_t head,  command, crc;
    uint8_t num,len, com_num, data_len, byte;

    in >> head >> num >> len >> com_num;
    if (head != 0xFDFD)
        return map;
    if (num != 0X00)
        return map;
    if(len != data.size()){
        qDebug() << len << data.size();
         return map;
    }

    for (int i = 0; i < com_num; i++) {
        in >> command >> data_len;
        for (int j = 0;j < data_len; j++) {
            in >> byte;
            map[command].append((char)byte);
        }
    }
    in >> crc;

    uint16_t ca_crc = 0;
    for (int i = 0; i < ba.size() - 2 ; i++) {
        ca_crc += (uint8_t)ba.at(i);
    }
    if (ca_crc != crc){
        for (auto key = map.begin();key !=map.end(); key++) {
            map.erase(key);
        }
        return map;
    }
    else {
        return map;
    }
}

void Worker::display(QByteArray array, bool connected)
{

}

void Worker::accept_read_data()
{
//    qDebug() << "----------------------";
    QByteArray data = this->serial->readAll();
//    qDebug() << data.toHex();
    QMap<int,QByteArray> map = this->load_data(data);
    if (map.isEmpty()){
        qDebug() << QString::fromLocal8Bit("数据解析错误");
        LOG_CRIT << u8"数据解析失败";
        return;
    }
//    qDebug() << "successed" << map[0x8007].toHex();
    QDataStream in(&map[0x8007],QIODevice::ReadOnly);
    uint16_t word_1, word_2, word_3, word_4, word_5;
    uint32_t dword_1, dword_2;
    in >> word_1 >> dword_1 >> word_2 >> dword_2 >> word_3 >> word_4 >> word_5;
    LOG_INFO << u8"接收数据：" << u8"电量(%)" << word_1 << u8"，温度(℃)" << dword_1 << u8"，湿度(%RH)" << word_2 << u8"，压强(kpa)"
             << dword_2 << u8"，可燃气体(ppm)"<< word_3 << u8"，有害气体(ppm)" << word_4 << u8"，底盘速度(mm/s)" << word_5;
    std::stringstream words_1, words_2, words_3, words_4, words_5, words_6, words_7;
//if (connected)
    words_1 << "电量:" << word_1 << "% " << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss").toStdString() ;
//else
//    words_1 << "电量:" << word_1 << "% " << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss").toStdString()  << "未连接";

    words_2 << "温度:"  << dword_1 << "℃           " << "湿度:" << word_2 << "%RH";
    words_3 << "底盘速度:" << word_5 << "mm/s     " << "压强:" << dword_2 << "kpa";
    words_4 << "可燃气体:"  << word_3 << "ppm      " << "有害气体:" << word_4 << "ppm";

    LPNET_DVR_SHOWSTRING_V30 pvoid = new NET_DVR_SHOWSTRING_V30;

    pvoid->dwSize = sizeof (NET_DVR_SHOWSTRING_V30);

    pvoid->struStringInfo[0] = camera->OSDINFO(words_1.str().c_str(),0,0);
    pvoid->struStringInfo[1] = camera->OSDINFO(words_2.str().c_str(),0,32);
    pvoid->struStringInfo[2] = camera->OSDINFO(words_3.str().c_str(),0,64);
    pvoid->struStringInfo[3] = camera->OSDINFO(words_4.str().c_str(),0,96);

    if (NET_DVR_SetDVRConfig(camera->lUserID,NET_DVR_SET_SHOWSTRING_V30,1,pvoid,sizeof (NET_DVR_SHOWSTRING_V30))){
        LOG_INFO << u8"----------------------------------设置OSD成功----------------------------------------";
    }
    else {
        LOG_CRIT  << u8"丢失数据：" << u8"电量(%)" << word_1 << u8"，温度(℃)" << dword_1 << u8"，湿度(%RH)" << word_2 << u8"，压强(kpa)"
                 << dword_2 << u8"，可燃气体(ppm)"<< word_3 << u8"，有害气体(ppm)" << word_4 << u8"，底盘速度(mm/s)"
                 << word_5 << u8"   设置OSD失败 , 错误代码：" << NET_DVR_GetLastError();
    }
    delete pvoid;
}

void Worker::accept_timeout()
{
    timer->stop();
    qDebug() << "timeout";
}


void Worker::accept_serial_error(const QSerialPort::SerialPortError &error)
{
    if (error == QSerialPort::NoError)
        return;
    if (serial->isOpen())
        serial->close();
    LOG_WARN << u8"串口断开连接";
    emit tell_window_msg(QString::fromLocal8Bit("串口断开连接"));
    this->display(0,0);

    timer->stop();
    emit tell_window_serial_status(false);
}

void Worker::accept_return_serial_status()
{
    emit tell_window_serial_status(this->serial->isOpen());
}



