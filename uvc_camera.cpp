#include "uvc_camera.hpp"

UVC_Camera::UVC_Camera(uvc_context_t *ctx, int vid, int pid, int wid, int hei,
                        string frame, const char* serial){
    uvc_error_t err;
    _ctx = ctx;
    _format = string(frame);

    err = uvc_find_device(_ctx, &_dev, vid, pid, serial);
    if (err){
        uvc_perror(err, "find_device");
        exit(0); //abort if the device wasn't found
    }

    err = uvc_open(_dev, &_devh);
    if (err){
        uvc_perror(err, "open");
        exit(0); //or if there was an error opening the device (sometimes happens
                 //because of permissions, try sudo)
    }

    //Hardcoded format negotiation
    //You can always use v4l2-ctl to check all possible formats
    if (!_format.compare("MJPEG")){
        printf("MJPEG\n");
        err = uvc_get_stream_ctrl_format_size(\
                    _devh, &_ctrl,\
                    UVC_FRAME_FORMAT_MJPEG, wid, hei, 30);
    }
    else if (!_format.compare("Y16")){
        printf("Y16\n");
        err = uvc_get_stream_ctrl_format_size(\
                    _devh, &_ctrl,\
                    UVC_FRAME_FORMAT_GRAY16, wid, hei, 9);
    }
    else{
        printf("UNKNOWN\n");
        err = uvc_get_stream_ctrl_format_size(\
                    _devh, &_ctrl,\
                    UVC_FRAME_FORMAT_ANY, wid, hei, 30);
    }

    //error on format
    if (err){
        uvc_perror(err, "format");
        exit(0);
    }
    _frames = new queue<Mat*>();
    _streaming = false;
    _hei = hei;
    _wid = wid;
    printf("(%s) Everything is ready to start...\n", _format.c_str());
}

//remember to destroy every references before finishing the app
UVC_Camera::~UVC_Camera(){
    if (this->_streaming)
        uvc_stop_streaming(_devh);
    uvc_close(_devh);
    uvc_unref_device(_dev);
}

bool UVC_Camera::start(){
    uvc_error_t err;
    void (*callback)(uvc_frame_t*, void*);
    printf("(%s) Stream started\n", _format.c_str());
    //hack to handle the callback
    (!_format.compare("Y16")) ? callback = &cb_y16 : callback = &cb;

    err = uvc_start_streaming(_devh, &_ctrl, callback, this, 0);
    if (err) {
        printf("%s", _format.c_str());
        uvc_perror(err, "start_streaming");
        return false;
    }
    //automatic exposure (dunno wats dis)
    uvc_set_ae_mode(_devh, 1);

    this->_streaming = true;
    return true;
}

void UVC_Camera::addFrame(Mat *fr){
    _frames->push(fr);
}

bool UVC_Camera::stop(){
    if (!this->_streaming) return false;

    uvc_stop_streaming(_devh);
    this->_streaming = false;

    return true;
}

bool UVC_Camera::hasFrames(){
    mtx.lock();
    bool isEmpty = _frames->empty();
    mtx.unlock();
    if (isEmpty) return false;
    return true;
}

Mat UVC_Camera::getFrame(){
    mtx.lock();
    Mat *ptr = new Mat();
    if (_frames->empty()){
        mtx.unlock();
        return *ptr;
    }
    delete ptr;

    ptr = _frames->front();
    Mat ret = *ptr;

    _frames->pop();
    mtx.unlock();
    delete ptr;

    return ret;
}

bool UVC_Camera::isStreaming(){
    return this->_streaming;
}

/*callback recieves a void *ptr, I pass the memdir of the camera, so the
    function can call the addFrame method
*/
void cb(uvc_frame_t *frame, void *ptr){
    Mat *img = new Mat(frame->height, frame->width, CV_16UC1, frame->data);
    *img = imdecode(*img, 1);

    UVC_Camera *cam = (UVC_Camera*)ptr;
    cam->addFrame(img);
}

void cb_y16(uvc_frame_t *frame, void *ptr){
    if (frame->data_bytes != 2 * frame->height * frame->width)
        return; //FRAMECITO MALO

    Mat *img = new Mat(frame->height, frame->width, CV_16UC1, frame->data);
    resize(*img, *img, Size(FIXED_WIDTH, FIXED_HEIGHT));

    UVC_Camera *cam = (UVC_Camera*)ptr;
    cam->addFrame(img);
}
