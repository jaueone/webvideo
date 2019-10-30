#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "webaxwidget.h"
#include "worker.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setObject(QJsonObject object);

public slots:
    void navigate(const QString &address);
    void on_WebBrowser_ProgressChange(int a, int b);
    void on_WebBrowser_TitleChange(const QString &title);
    void on_WebBrowser_CommandStateChange(int cmd, bool on);
    void on_WebBrowser_BeforeNavigate();
    void on_WebBrowser_NavigateComplete(const QString &address);

    virtual void resizeEvent(QResizeEvent *event);
    void showmsg(QString msg);

signals:
    void tell_worker_object(QJsonObject object);

private:
    Ui::MainWindow *ui;
    QString title;
    QLabel *label;
    QLabel *label_2;

    Worker *worker;
    QThread *thread;
    QString icon;
};

#endif // MAINWINDOW_H

