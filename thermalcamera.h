#ifndef THERMALCAMERA_H
#define THERMALCAMERA_H

#include <QObject>
#include <QImage>
#include "uvc_camera.hpp"
#include "libuvc/libuvc.h"
typedef uvc_context_t Context;

class ThermalCamera : public QObject{
    Q_OBJECT
    public:
        ThermalCamera(Context *ctx, int vid, int pid, QObject *_parent = 0);
        ~ThermalCamera() { delete cam; }
        void start();
        void stop();
        QImage getFrame();

    private:
        UVC_Camera *cam;

    public slots:
        void retrieveFrame();

    signals:
        void frameObtained();
};

#endif // THERMALCAMERA_H
