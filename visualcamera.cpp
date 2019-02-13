#include <QImage>
#include <QString>
#include <QThread>
#include <QPixmap>
#include "visualcamera.h"

VisualCamera::VisualCamera(Context *ctx, int vid, int pid, QObject* _parent)
    : QObject(_parent){
    cam = new UVC_Camera(ctx, vid, pid, 320, 240);
}

void VisualCamera::start(){
    qDebug("Trying to start");
    if (cam->isStreaming())
        return;
    bool ret = cam->start();
    if (ret) qDebug("Started!");
}

void VisualCamera::stop(){
    if (!cam->isStreaming())
        return;
    cam->stop();
}

QImage VisualCamera::getFrame(){
    cv::Mat frame = cam->getFrame();
    if (!frame.empty())
        cv::cvtColor(frame, frame, CV_BGR2RGB);
    return QImage((uchar*)frame.data, frame.cols, frame.rows, QImage::Format_RGB888);
}

void VisualCamera::retrieveFrame(){
    qDebug("Starting loop");
    while(cam->isStreaming()){
        if (cam->hasFrames()){
            //qDebug("Frame availible");
            emit frameObtained();
        }
    }
    qDebug("Loop finished");
}
