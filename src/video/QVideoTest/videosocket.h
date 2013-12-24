#ifndef VIDEOSOCKET_H
#define VIDEOSOCKET_H

#include <QThread>
#include <QUdpSocket>
#include <QHostAddress>

#include "videoreader.h"
#include "videoviewer.h"

#define VIDEO_PORT 4244

class VideoSocket : public QThread
{
    Q_OBJECT

public:
    VideoSocket(VideoViewer *videoViewer, QObject *parent = 0);
    virtual ~VideoSocket();

    inline const VideoViewer* videoViewer() const { return _videoViewer; }
    inline VideoViewer* videoViewer() { return _videoViewer; }
    inline void setVideoViewer(VideoViewer *videoViewer) { _videoViewer = videoViewer; }

public slots:
    void setHostAddr(const QHostAddress &addr, quint16 port = VIDEO_PORT);
    void setPeerAddr(const QHostAddress& addr, quint16 port = VIDEO_PORT);

    void quit();
    void terminate();

protected:
    virtual void run();

private:
    QUdpSocket *_socket;
    QHostAddress _hostAddr;
    quint16 _hostPort;
    QHostAddress _peerAddr;
    quint16 _peerPort;
    bool _run;
    VideoReader *_videoReader;
    VideoViewer *_videoViewer;

private slots:
    void _socket_readyRead();
};

#endif // VIDEOSOCKET_H
