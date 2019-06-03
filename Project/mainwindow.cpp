#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QTimer>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setUpManager();
}

MainWindow::~MainWindow()
{
    thread->quit();
    while(!thread->isFinished());
    delete thread;
    delete ui;
}

void MainWindow::setUpManager()
{
    thread = new QThread();
    OpencvManager * manager = new OpencvManager();
    QTimer * managerTrigger = new QTimer();
    managerTrigger->setInterval(15);

    connect(managerTrigger, SIGNAL(timeout()), manager, SLOT(receiveGrabFrame()));
    connect(this, SIGNAL(sendSetup(QByteArray)), manager, SLOT(receiveSetup(QByteArray)));
    connect(this, SIGNAL(sendToggleStream()), manager, SLOT(receiveToggleStream()));
    connect(ui->playButton, SIGNAL(clicked(bool)), this, SLOT(receiveToggleStream()));
    connect(manager, SIGNAL(sendSourceFrame(QImage)), this, SLOT(receiveSourceFrame(QImage)));
    connect(thread, SIGNAL(finished()), manager, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), managerTrigger, SLOT(deleteLater()));


    managerTrigger->start();
    manager-> moveToThread(thread);
    managerTrigger->moveToThread(thread);

    thread->start();

    //QString filename = QFileDialog::getOpenFileName(this, tr("Open Video"), "/Users/MM/Documents/", tr("Video files (*.avi)"));
    QString filename = "aaa";
    emit sendSetup(filename.toUtf8());
}


void MainWindow::receiveSourceFrame(QImage frame)
{
    ui->videoFieldSource->setPixmap(QPixmap::fromImage(frame.rgbSwapped()));
    ui->videoFieldSource->setScaledContents(true);
    ui->videoFieldSource->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
}



void MainWindow::receiveToggleStream()
{
    if (!ui->playButton->text().compare(">")) ui->playButton->setText("||");
    else ui->playButton->setText(">");

    emit sendToggleStream();
}
