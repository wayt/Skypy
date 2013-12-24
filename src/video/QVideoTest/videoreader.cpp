#include "videoreader.h"

VideoReader::VideoReader(QObject *parent) :
    QObject(parent),
    _cap(0)
{
}

VideoReader::~VideoReader()
{
}

bool VideoReader::readFrame(QImage &img)
{
    cv::Mat frame;

    if (!_cap.read(frame))
        return false;

    img = _mat2QImage(frame);
    return true;
}

QImage VideoReader::_mat2QImage(const cv::Mat &matImg)
{
    QImage qimg(matImg.data, matImg.cols, matImg.rows, matImg.step, QImage::Format_RGB888);
    QImage dest(qimg.rgbSwapped());
    dest.detach();

    return dest;
}
