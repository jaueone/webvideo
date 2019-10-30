#ifndef WEBAXWIDGET_H
#define WEBAXWIDGET_H

#include <Windows.h>
#include <ActiveQt/QAxWidget>


class WebAxWidget : public QAxWidget
{
public:

    WebAxWidget(QWidget* parent = 0, Qt::WindowFlags f = 0)
        : QAxWidget(parent, f)
    {
    }
protected:
    bool translateKeyEvent(int message, int keycode) const Q_DECL_OVERRIDE
    {
        if (message >= WM_KEYFIRST && message <= WM_KEYLAST)
            return true;
        else
            return QAxWidget::translateKeyEvent(message, keycode);
    }
};
#endif
