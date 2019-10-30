#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QJsonObject>
#include <QAxObject>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->removeToolBar(ui->mainToolBar);
    this->setWindowTitle(QString::fromLocal8Bit("录像"));
    this->label = new QLabel(ui->WebBrowser);
    this->label_2 = new QLabel(ui->WebBrowser);
    this->label->setGeometry(30,70,120,50);
    this->label->setStyleSheet("background-color:rgb(255,255,255)");
    this->label->hide();
    this->label_2->hide();
    ui->WebBrowser->hide();
    this->worker = new Worker;
    this->thread = new QThread(this);
    worker->moveToThread(thread);
    this->thread->start();
    this->ui->WebBrowser->setControl(QString::fromUtf8("{8856F961-340A-11D0-A96B-00C04FD705A2}"));
    this->ui->WebBrowser->setObjectName(QString::fromUtf8("WebBrowser"));
    this->setCentralWidget(ui->WebBrowser);
    this->setMinimumSize(QSize(1028,615));

    this->connect(this,&MainWindow::tell_worker_object,worker,&Worker::setObject,Qt::QueuedConnection);
    this->connect(worker,&Worker::tell_window_msg,this,&MainWindow::showmsg,Qt::QueuedConnection);

}

MainWindow::~MainWindow()
{
    this->thread->quit();
    this->thread->wait();
    delete ui;
}

void MainWindow::setObject(QJsonObject object)
{
    QJsonObject cameras = object["Camera"].toObject();
    this->setWindowTitle(object["Title"].toString());
    ui->WebBrowser->dynamicCall("Navigate(const QString&)", "https://www.baidu.com");
    this->icon = object["Logo"].toString();
    emit tell_worker_object(object);
}

void MainWindow::navigate(const QString &address)
{
    qDebug() << "navigate" << address;
    if (ui->WebBrowser->isVisible())
        ui->WebBrowser->hide();
    if (title == QString::fromLocal8Bit("登陆") && address.contains("/doc/page/preview.asp")){
        this->label->hide();
    }
}

void MainWindow::on_WebBrowser_ProgressChange(int a, int b)
{


}

void MainWindow::on_WebBrowser_TitleChange(const QString &title)
{
    qDebug() << "TitleChange" << title;
    this->title = title;

    if (title == QString::fromLocal8Bit("登录")){
        this->label->setStyleSheet("background-color:rgb(255,255,255)");
        this->label->clear();
        this->label->setGeometry(30,60,120,50);
        this->label->show();
        this->label_2->setStyleSheet("background-color:rgb(255,255,255)");
        this->label_2->setGeometry(0,550,this->size().width(),20);
        this->label_2->show();
        ui->WebBrowser->show();
    }
    else if (title == QString::fromLocal8Bit("预 览") ||title == QString::fromLocal8Bit("图 片") || title == QString::fromLocal8Bit("配 置") || title == QString::fromLocal8Bit("回 放")){
        this->label->setPixmap(QPixmap(this->icon));
        this->label->setScaledContents(true);
        this->label->setStyleSheet("background-color:rgb(45,45,45)");
        this->label->setGeometry(20,0,120,40);
        ui->WebBrowser->show();
        this->label_2->setStyleSheet("background-color:rgb(255,255,255)");
        this->label_2->setGeometry(0,this->size().height()-30,this->size().width(),30);

        this->label_2->show();
        this->label->show();



    }

}

void MainWindow::on_WebBrowser_CommandStateChange(int cmd, bool on)
{

}

void MainWindow::on_WebBrowser_BeforeNavigate()
{
    this->label->hide();
    ui->WebBrowser->hide();
}

void MainWindow::on_WebBrowser_NavigateComplete(const QString &address)
{
    qDebug() << "NavigateComplete" << address;

}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    if (title == QString::fromLocal8Bit("登录")){
        this->label_2->setStyleSheet("background-color:rgb(255,255,255)");
        this->label_2->setGeometry(0,550,this->size().width(),20);
        this->label_2->show();
    }

    else if (title == QString::fromLocal8Bit("预 览") || title == QString::fromLocal8Bit("回 放") ||title == QString::fromLocal8Bit("图 片") || title == QString::fromLocal8Bit("配 置")){
        this->label->setStyleSheet("background-color:rgb(45,45,45)");
        this->label->setGeometry(20,10,120,25);
        this->label->show();
        this->label_2->setGeometry(0,this->label_2->y(),event->size().width(),this->label_2->height());
        this->label_2->setStyleSheet("background-color:rgb(255,255,255)");
        this->label_2->setGeometry(0,this->size().height()-30,this->size().width(),30);
        this->label_2->show();
    }

}

void MainWindow::showmsg(QString msg)
{
    QMessageBox messageBox;
    messageBox.setWindowTitle(QString::fromLocal8Bit("警告"));
    messageBox.setIcon(QMessageBox::Warning);
    messageBox.setText(QString::fromLocal8Bit(msg.toLocal8Bit().data()));
    QPushButton button(QString::fromLocal8Bit("确定"));
    messageBox.addButton(&button, QMessageBox::YesRole);
    messageBox.exec();
}



