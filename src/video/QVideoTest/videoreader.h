#ifndef VIDEOREADER_H
#define VIDEOREADER_H

#include <QObject>
#include <QImage>
#include <opencv2/opencv.hpp>

class VideoReader : public QObject
{
    Q_OBJECT

public:
    VideoReader(QObject *parent = 0);
    virtual ~VideoReader();

    inline bool isInitialized() const { return _cap.isOpened(); }

    bool readFrame(QImage &img);

private:
    static QImage _mat2QImage(const cv::Mat &matImg);

private:
    cv::VideoCapture _cap;
};

#endif // VIDEOREADER_H
