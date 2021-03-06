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
#include <unistd.h>
#include <QImage>
#include <QMetaType>
#include <QMenuBar>

#define VISUALCAM_PID 0x0030
#define VISUALCAM_VID 0x80ee
#define IRCAM_PID 0x0100
#define IRCAM_VID 0x1e4e

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

    //Ugly pathing stuff
    //Qt has better pathing
    char *path = new char[256];
    getcwd(path, 256);
    _path = QString((const char*)path); _path += "/";

    pepega = new QImage(_path + "Assets/stando.jpg");
    QImage *f2 = new QImage(_path + "Assets/god.jpg");

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

    vis = new CustomCamera(ctx, 0, VISUALCAM_VID, VISUALCAM_PID, 320, 240);
    ir = new CustomCamera(ctx, 1, IRCAM_VID, IRCAM_PID, 160, 120, "Y16");

    vis->moveToThread(tl);
    ir->moveToThread(thr);

    connect(tl, &QThread::started, vis, &CustomCamera::fetchFrame);
    connect(vis, &CustomCamera::frameAvailible, this, &Gui::update);

    connect(thr, &QThread::started, ir, &CustomCamera::fetchFrame);
    connect(ir, &CustomCamera::frameAvailible, this, &Gui::update);

    //connecting to write image
    qRegisterMetaType< cv::Mat >("cv::Mat");
    connect(vis, &CustomCamera::savePicture, this, &Gui::saveFrame);
    connect(ir, &CustomCamera::savePicture, this, &Gui::saveFrame);

    vis->start();
    ir->start();
    tl->start();
    thr->start();
    setCentralWidget(canvas);
}

Gui::~Gui(){
    vis->stop(); ir->stop();
    tl->exit(); thr->exit();
    delete ir;
    delete vis;
    uvc_exit(ctx);
}

void Gui::aboutWindow(){
    QMessageBox::about(this, tr("FootShot FootCam v0.3.1"), tr("Done by Pedro G. using C++ & Qt5"));
}

void Gui::changeStatus(QString s){
    status->setText(s);
}

void Gui::analyze(){
    changeStatus("Analizando imágenes...");
    return;
}

void Gui::capture(){
    int qnt = 5;
    changeStatus("Capturando...");
    vis->save(qnt);
    ir->save(qnt);
    return;
}

//prolly make more expressive
void Gui::update(QImage img, int id){
    if (img.height() == 0 || img.width() == 0) return;
    if (!id){
        ppg->setPixmap(QPixmap::fromImage(img));
    }
    else{
        //qDebug("(Y16) Updating");
        ther->setPixmap(QPixmap::fromImage(img.mirrored(true, true)));
    }
}

//this one too
void Gui::saveFrame(Mat raw, int id, int n_pic){
    std::string msg("saving frame from: ");
    msg += std::to_string(id);
    qDebug(msg.c_str());
    std::string sn_pic = std::to_string(n_pic);
    std::string file = (!id) ? "VIS_" + sn_pic+ ".png" : "IR_" + sn_pic + ".png";
    file = _path.toStdString() + "Muestras/" + file;
    imwrite((const char*)file.c_str(), raw);
}
