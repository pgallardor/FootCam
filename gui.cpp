#include "gui.h"
#include <QGridLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QFrame>
#include <QMenu>
#include <QFile>
#include <QColor>
#include <QImage>
#include <QMenuBar>
//#include "visualcamera.h"

Gui::Gui(QWidget *parent)
    : QMainWindow(parent) {

    status = new QLabel("Bienvenido a FootShot");
    QHBoxLayout *frames = new QHBoxLayout(), *buttons = new QHBoxLayout();

    //elementos del menu
    QAction *quit = new QAction("&Salir", this),
            *about = new QAction("&Acerca de", this),
            *search = new QAction("&Buscar paciente", this),
            *stopRecord = new QAction("&Detener", this),
            *resumeRecord = new QAction("&Reanudar", this);

    QMenu *options;
    options = menuBar()->addMenu("&Camara");
    options->addAction(stopRecord);
    options->addAction(resumeRecord);

    options = menuBar()->addMenu("&Opciones");
    options->addAction(search);
    options->addSeparator();
    options->addAction(about);
    options->addAction(quit);

    connect(quit, &QAction::triggered, qApp, QApplication::quit);
    connect(about, &QAction::triggered, this, &Gui::aboutWindow);

    connect(stopRecord, &QAction::triggered, this, [=](){
                    vis->stop(); ir->stop();
                    tl->exit(); thr->exit();
                });
    connect(resumeRecord, &QAction::triggered, this, [=](){
                    vis->start(); ir->start();
                    tl->start(); thr->start();
                });

    //definición de botones
    QPushButton *capture = new QPushButton("Capturar", this);
    QPushButton *analyze = new QPushButton("Analizar", this);
    QPushButton *results = new QPushButton("Resultados", this);

    connect(capture, &QPushButton::clicked, this, &Gui::capture);
    connect(analyze, &QPushButton::clicked, this, &Gui::analyze);

    /*QFrame *f1 = new QFrame(), *f2 = new QFrame();
    f1->setFixedHeight(240); f1->setFixedWidth(320);
    f1->setFrameStyle(QFrame::Box);

    f2->setFixedHeight(240); f2->setFixedWidth(320);
    f2->setFrameStyle(QFrame::Box);
    */
    pepega = new QImage("/home/pedro/Documents/FootCam/Assets/stando.jpg");
    QImage *f2 = new QImage("/home/pedro/Documents/FootCam/Assets/god.jpg");

    ther = new QLabel("");
    ppg = new QLabel("");
    ppg->setPixmap(QPixmap::fromImage(pepega->scaled(320, 240)));
    ther->setPixmap(QPixmap::fromImage(*f2));
    frames->addWidget(ppg);
    frames->addWidget(ther);

    buttons->addWidget(status);
    capture->setFixedSize(70, 30);
    buttons->addWidget(capture);

    analyze->setFixedSize(70, 30);
    buttons->addWidget(analyze);

    results->setFixedSize(80, 30);
    buttons->addWidget(results);

    QWidget *canvas = new QWidget();
    QVBoxLayout *general = new QVBoxLayout();
    general->addLayout(frames);
    general->addLayout(buttons);
    canvas->setLayout(general);

    //threading and cameras
    Context *ctx;
    uvc_init(&ctx, NULL);

    tl = new QThread();
    thr = new QThread();
    vis = new CustomCamera(ctx, 0, 0x80ee, 0x30, 320, 240);
    ir = new CustomCamera(ctx, 1, 0x1e4e, 0x0100, 160, 120, "Y16");

    vis->moveToThread(tl);
    ir->moveToThread(thr);

    connect(tl, &QThread::started, vis, &CustomCamera::fetchFrame);
    connect(vis, &CustomCamera::frameAvailible, this, &Gui::update);

    connect(thr, &QThread::started, ir, &CustomCamera::fetchFrame);
    connect(ir, &CustomCamera::frameAvailible, this, &Gui::update);

    vis->start();
    ir->start();
    tl->start();
    thr->start();
    setCentralWidget(canvas);
}

void Gui::aboutWindow(){
    QMessageBox::about(this, tr("FootShot FootCam v0.2.2"), tr("Done by Pedro G. using C++ & Qt5"));
}

void Gui::changeStatus(QString s){
    status->setText(s);
}

void Gui::analyze(){
    changeStatus("Analizando imágenes...");
    return;
}

void Gui::capture(int qnt = 5){
    Q_UNUSED(qnt);
    changeStatus("Capturando...");
    return;
}

void Gui::update(QImage img, int id){
    if (img.height() == 0 || img.width() == 0) return;
    if (!id)
        ppg->setPixmap(QPixmap::fromImage(img));
    else
        ther->setPixmap(QPixmap::fromImage(img));
}
/*
void Gui::updateLeft(){
    QImage buff = vis->getFrame();
    if (buff.height() == 0){
        return;
    }
    ppg->setPixmap(QPixmap::fromImage(buff));
}

void Gui::updateRight(){
    QImage buff = ir->getFrame();
    if (buff.height() == 0 || buff.width() == 0) return;
    ther->setPixmap(QPixmap::fromImage(buff));
}
*/
