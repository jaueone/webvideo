#include "mainwindow.h"
#include <QApplication>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QTextCodec>
#include "Logger.h"

using namespace Logger;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec *codec = QTextCodec::codecForName("gbk"); //GBK     QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForLocale(codec);


    QFile file("./config.json");

    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QString value = file.readAll();


    QJsonParseError parseJsonErr;
    QJsonDocument document = QJsonDocument::fromJson(value.toUtf8(),&parseJsonErr);
    if(!(parseJsonErr.error == QJsonParseError::NoError))
    {
        QMessageBox messageBox;
        messageBox.setWindowTitle("错误");
        messageBox.setIcon(QMessageBox::Critical);
        messageBox.setText("配置文件解析失败");
        QPushButton button("确定");
        messageBox.addButton(&button, QMessageBox::YesRole);
        messageBox.exec();
        file.close();
        return -1;
    }

    QJsonObject config = document.object();

    initLog(config["LogPath"].toString());
    MainWindow w;
    w.setObject(config);
    file.close();
    w.show();


    return a.exec();
}
