#ifndef VISUALCAMERA_H
#define VISUALCAMERA_H

#include <QImage>
#include <QString>
#include <QPixmap>
#include <opencv2/core/core.hpp>
#include "uvc_camera.hpp"
#include "libuvc/libuvc.h"

typedef uvc_context_t Context;
//wrapper class
class VisualCamera : public QObject {
    Q_OBJECT
    private:
        UVC_Camera *cam;

    public:
        VisualCamera(Context *ctx, int vid, int pid, QObject *_parent = 0);
        ~VisualCamera() { delete cam; }
        void start();
        void stop();
        QImage getFrame();

    public slots:
        void retrieveFrame();

    signals:
        void frameObtained();
};

#endif // VISUALCAMERA_H
