/*
Custom UVC Camera wrapper
For now just supports GRAY16 and MJPEG frame formats

Note: C++ libuvc streams using a callback, I had to use different callbacks depending
on the format
*/

#ifndef UVC_CAMERA_HPP
#define UVC_CAMERA_HPP

#include "libuvc/libuvc.h"
#include <queue>
#include <exception>
#include <cassert>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <mutex>
#define FIXED_HEIGHT 240
#define FIXED_WIDTH 320

using namespace cv;
using namespace std;


void cb(uvc_frame_t *frame, void *ptr);
void cb_y16(uvc_frame_t *frame, void *ptr);

class UVC_Camera{
    public:
        UVC_Camera(uvc_context_t *ctx, int vid, int pid,
                    int wid, int hei, string frame = "MJPEG", const char* serial = NULL);
        ~UVC_Camera();
        bool isStreaming();
        bool hasFrames();
        void addFrame(Mat *fr);
        bool start();
        bool stop();
        Mat getFrame();


    private:
        mutex mtx; //needed for race conditions
        uvc_context_t *_ctx;
        uvc_device_t *_dev;
        uvc_device_handle_t *_devh;
        uvc_stream_ctrl_t _ctrl;
        string _format;
        int _wid, _hei;
        bool _streaming;
        queue<Mat*> *_frames;
        //static void cb(uvc_frame_t *frame, void *ptr);

};

#endif // UVC_CAMERA_HPP
