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
    thread = new QThread();
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

    OpencvManager * cvManager = new OpencvManager();
    QTimer * trigger = new QTimer();
    trigger->setInterval(30);

    connect(trigger, SIGNAL(timeout()), cvManager, SLOT(receiveGrabFrame()));
    connect(this, SIGNAL(sendSetup(QByteArray)), cvManager, SLOT(receiveSetup(QByteArray)));
    connect(this, SIGNAL(sendToggleStream()), cvManager, SLOT(receiveToggleStream()));
    connect(ui->playButton, SIGNAL(clicked(bool)), this, SLOT(receiveToggleStream()));
    connect(cvManager, SIGNAL(sendSourceFrame(QImage)), this, SLOT(receiveSourceFrame(QImage)));
    connect(thread, SIGNAL(finished()), cvManager, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), trigger, SLOT(deleteLater()));
    connect(cvManager, SIGNAL(sendUpCounter(int)), this, SLOT(receiveUpCount(int)));
    connect(cvManager, SIGNAL(sendDownCounter(int)), this, SLOT(receiveDownCount(int)));
    connect(ui->startCountButton, SIGNAL(clicked(bool)), this, SLOT(receiveShouldCount(bool)));
    connect(this, SIGNAL(sendIsCounting(bool)), cvManager, SLOT(receiveIsCounting(bool)));
    connect(ui->clearCountButton, SIGNAL(clicked(bool)), this, SLOT(receiveClearCount()));
    connect(this, SIGNAL(sendClearCount()), cvManager, SLOT(receiveClearCount()));

    connect(ui->actionCam, SIGNAL(triggered()), this, SLOT(receiveCam()));
    connect(ui->actionVideo_File, SIGNAL(triggered()), this, SLOT(receiveVideoFile()));

    connect(ui->actionDebug_window, SIGNAL(triggered()),this, SLOT(receiveToggleDebug()));
    connect(this, SIGNAL(sendToggleDebug()), cvManager, SLOT(receiveToggleDebug()));

    trigger->start();
    cvManager-> moveToThread(thread);
    trigger->moveToThread(thread);

    thread->start();


}

void MainWindow::receiveVideoFile()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Video"), "/Users/MM/Documents/", tr("Video files (*.avi *.mp4)"));
    emit sendSetup(filename.toUtf8());
}

void MainWindow::receiveCam()
{
    emit sendSetup(QString::number(0).toUtf8());
}


void MainWindow::receiveSourceFrame(QImage frame)
{
    ui->videoFieldSource->setPixmap(QPixmap::fromImage(frame.rgbSwapped()));
    ui->videoFieldSource->setScaledContents(true);
    ui->videoFieldSource->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
}



void MainWindow::receiveToggleStream()
{
    if (!ui->playButton->text().compare("Play")) ui->playButton->setText("Pause");
    else ui->playButton->setText("Play");

    emit sendToggleStream();
}

void MainWindow::receiveUpCount(int count)
{
    this->ui->upLabel->setText(QString("Up ") + QString::number(count));
}

void MainWindow::receiveDownCount(int count)
{
    this->ui->downLabel->setText(QString("Down ") + QString::number(count));
}

void MainWindow::receiveShouldCount(bool shouldCount)
{
    if (!ui->startCountButton->text().compare("Count"))
    {
        ui->startCountButton->setText("Counting");
        emit sendIsCounting(true);
    }else {
        ui->startCountButton->setText("Count");
        emit sendIsCounting(false);
    }

}

void MainWindow::receiveClearCount()
{
    this->ui->upLabel->setText(QString("Up 0"));
    this->ui->downLabel->setText(QString("Down 0"));

    emit sendClearCount();
}

void MainWindow::receiveToggleDebug()
{
    emit sendToggleDebug();
}
