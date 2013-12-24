#ifndef VIDEOVIEWER_H
#define VIDEOVIEWER_H

#include <QLabel>
#include <QByteArray>

class VideoViewer : public QLabel
{
    Q_OBJECT

public:
    VideoViewer(QWidget *parent = 0);
    virtual ~VideoViewer();

    void setImage(const QByteArray &ba);
};

#endif // VIDEOVIEWER_H
