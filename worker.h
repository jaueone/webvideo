#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QThread>
#include <QString>
#include <QTimer>
#include <QDebug>
#include <QList>
#include <QTime>
#include <QCoreApplication>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "HCNetSDK.h"
#include "hcnet.h"
#include <QJsonObject>

class Worker:public QObject
{
    Q_OBJECT

public:
    Worker();
    ~Worker();

    void init_setting_and_connect(QString name ,int baudrate);

    static QMap<int,QByteArray> load_data(const QByteArray &data);

    void display(QByteArray array,bool connected);


signals:
    void tell_window_serial_status(bool isopened);

    void tell_window_msg(QString msg);

public slots:
    void accept_read_data();
    void setObject(QJsonObject object);
    void accept_return_serial_status();
    void accept_timeout();
    void accept_serial_error(const QSerialPort::SerialPortError &error);


private:
    QTimer *timer;
    QSerialPort *serial;
    HCNet *camera;
    QString ip;
    QString port;
    QString user;
    QString password;
};


#endif
