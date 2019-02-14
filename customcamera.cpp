#include "customcamera.h"
#include <opencv2/core/core.hpp>

CustomCamera::CustomCamera(Context *ctx, int id, int vid, int pid,\
             int wid, int hei, std::string format, QObject *parent)
            : QObject(parent) {
    _id = id;
    _format = string(format);
    cam = new UVC_Camera(ctx, vid, pid, wid, hei, format);
}

void CustomCamera::start(){
    qDebug("Starting camera " + _id);
    if (cam->isStreaming()) return;

    if (cam->start()) qDebug("Camera started");
}

void CustomCamera::stop(){
    if (!cam->isStreaming()) return;
    cam->stop();
}

void CustomCamera::fetchFrame(){
    qDebug("Starting loop");
    while (cam->isStreaming()){
        if (cam->hasFrames()){
            cv::Mat frame = cam->getFrame();
            if (frame.empty()) continue;

            if (!_format.compare("Y16")){
                cv::cvtColor(frame, frame, CV_GRAY2RGB);
            }
            else cv::cvtColor(frame, frame, CV_BGR2RGB);
            QImage img((uchar*)frame.data, frame.cols, frame.rows, QImage::Format_RGB888);
            emit frameAvailible(img, _id);
        }
    }
}
