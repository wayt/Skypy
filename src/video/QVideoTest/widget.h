#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLineEdit>

#include "videosocket.h"
#include "videoviewer.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(int id, QWidget *parent = 0);
    virtual ~Widget();

private:
    QLineEdit *_leIp;
    QPushButton *_pbStart;
    QVBoxLayout *_layV;
    VideoViewer *_videoViewer;
    VideoSocket *_videoSocket;
    int _id;

private slots:
    void _pbStart_clicked();
};

#endif // WIDGET_H
