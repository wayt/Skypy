#include <QImage>
#include <QPixmap>

#include "videoviewer.h"

VideoViewer::VideoViewer(QWidget *parent) :
    QLabel(parent)
{
}

VideoViewer::~VideoViewer()
{
}

void VideoViewer::setImage(const QByteArray &ba)
{
    QImage img = QImage::fromData(ba, "JPG");

    this->setPixmap(QPixmap::fromImage(img));
}
