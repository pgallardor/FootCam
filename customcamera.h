#ifndef CUSTOMCAMERA_H
#define CUSTOMCAMERA_H

#include <QObject>
#include <QThread>
#include <queue>
#include <QImage>
#include "uvc_camera.hpp"
#include "libuvc/libuvc.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

typedef uvc_context_t Context;

class CustomCamera : public QObject {
    Q_OBJECT
    public:
        explicit CustomCamera(Context *ctx, int id, int vid, int pid,
                              int wid = 320, int hei = 240, std::string format = "MJPEG",
                              QObject *parent = nullptr);
        ~CustomCamera() { delete cam; }
        void start();
        void stop();
        void save(int qnt);
        //QImage getFrame();
    private:
        UVC_Camera *cam;
        std::string _format;
        int _id;
        int _to_save;
        cv::Mat treat(cv::Mat frame);
        cv::Mat treatY16(cv::Mat frame);
        //approach 1: just save it from the camera
        void saveCapture(cv::Mat frame, std::string path);

    signals:
        void frameAvailible(QImage frame, int id);
        //approach 2: ask the gui to save it
        void savePicture(cv::Mat raw, int id, int n_pic);

    public slots:
        void fetchFrame();
};

#endif // CUSTOMCAMERA_H
