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

cv::Mat CustomCamera::treat(cv::Mat frame){
    cv::Mat ret; // = imdecode(frame, 1);
    cv::cvtColor(frame, ret, CV_BGR2RGB);
    return ret;
}

cv::Mat CustomCamera::treatY16(cv::Mat frame){
    cv::Mat ret;
    cv::normalize(frame, ret, 0, 65535, NORM_MINMAX);
    ret.convertTo(ret, CV_8U, 1 / 256.0);

    cv::cvtColor(ret, ret, CV_GRAY2RGB);
    return ret;
}

void CustomCamera::fetchFrame(){
    qDebug("Starting loop");
    while (cam->isStreaming()){
        if (cam->hasFrames()){
            cv::Mat frame = cam->getFrame();
            if (frame.empty()) continue;

            if (!_format.compare("Y16")){
                frame = this->treatY16(frame);
            }
            else frame = this->treat(frame);
            QImage img((uchar*)frame.data, frame.cols, frame.rows, QImage::Format_RGB888);
            emit frameAvailible(img, _id);
        }
    }
}
