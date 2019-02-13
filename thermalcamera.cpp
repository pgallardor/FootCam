#include "thermalcamera.h"
#include <QDebug>
#include <QThread>

ThermalCamera::ThermalCamera(Context *ctx, int vid, int pid, QObject *_parent)
        : QObject(_parent){
    cam = new UVC_Camera(ctx, vid, pid, 160, 120, "Y16");
}

void ThermalCamera::start(){
    qDebug("Trying to start");
    if (cam->isStreaming())
        return;
    bool ret = cam->start();
    if (ret) qDebug("Started!");
}

void ThermalCamera::stop(){
    if (!cam->isStreaming())
        return;
    cam->stop();
}

QImage ThermalCamera::getFrame(){
    cv::Mat frame = cam->getFrame();
    if (!frame.empty())
        cv::cvtColor(frame, frame, CV_GRAY2RGB);
    return QImage((uchar*)frame.data, frame.cols, frame.rows, QImage::Format_RGB888);
}

void ThermalCamera::retrieveFrame(){
    qDebug("Starting loop");
    while(cam->isStreaming()){
        if (cam->hasFrames()){
            emit frameObtained();
        }
    }
    qDebug("Loop finished");
}
