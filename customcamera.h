#ifndef CUSTOMCAMERA_H
#define CUSTOMCAMERA_H

#include <QObject>
#include <QThread>
#include <queue>
#include <QImage>
#include "uvc_camera.hpp"
#include "libuvc/libuvc.h"
#include <opencv2/core/core.hpp>

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
        //QImage getFrame();
    private:
        UVC_Camera *cam;
        std::string _format;
        int _id;

    signals:
        void frameAvailible(QImage frame, int id);

    public slots:
        void fetchFrame();
};

#endif // CUSTOMCAMERA_H
