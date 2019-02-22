#ifndef GUI_H
#define GUI_H

#include <QMainWindow>
#include <QWidget>
#include <QApplication>
#include <QLabel>
#include <QMessageBox>
#include <QGridLayout>
#include <QImage>
#include <QMutex>
#include <QTimer>
#include <QThread>
#include <opencv2/core/core.hpp>
#include "customcamera.h"

class Gui : public QMainWindow{
    Q_OBJECT
    public:
        Gui(QWidget *parent = 0);
        ~Gui();
        void startCamera();

    private slots:
        void aboutWindow();
        void changeStatus(QString s);
        //void updateLeft();
        //void updateRight();
        void update(QImage img, int id);
        void analyze();
        void saveFrame(cv::Mat raw, int id, int n_pic);
        void capture();

    private:
        //cameras soon
        CustomCamera *vis,
                     *ir;
        QThread *tl, *thr;
        QImage *visual,
               *pepega;
        QLabel *status,
               *ppg,
               *ther;
        QMutex mtx;
        QString _path;
        bool isRecording;
};

#endif // GUI_H
