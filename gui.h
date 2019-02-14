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
#include "customcamera.h"

class Gui : public QMainWindow{
    Q_OBJECT
    public:
        Gui(QWidget *parent = 0);
        void startCamera();

    private slots:
        void aboutWindow();
        void changeStatus(QString s);
        //void updateLeft();
        //void updateRight();
        void update(QImage img, int id);
        void analyze();
        void capture(int qnt);

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
        bool isRecording;
};

#endif // GUI_H
