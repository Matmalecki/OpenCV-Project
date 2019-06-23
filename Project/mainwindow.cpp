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
    managerTrigger->setInterval(12);

    connect(managerTrigger, SIGNAL(timeout()), manager, SLOT(receiveGrabFrame()));
    connect(this, SIGNAL(sendSetup(QByteArray)), manager, SLOT(receiveSetup(QByteArray)));
    connect(this, SIGNAL(sendToggleStream()), manager, SLOT(receiveToggleStream()));
    connect(ui->playButton, SIGNAL(clicked(bool)), this, SLOT(receiveToggleStream()));
    connect(manager, SIGNAL(sendSourceFrame(QImage)), this, SLOT(receiveSourceFrame(QImage)));
    connect(thread, SIGNAL(finished()), manager, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), managerTrigger, SLOT(deleteLater()));
    connect(manager, SIGNAL(sendUpCounter(int)), this, SLOT(receiveUpCount(int)));
    connect(manager, SIGNAL(sendDownCounter(int)), this, SLOT(receiveDownCount(int)));
    connect(ui->startCountButton, SIGNAL(clicked(bool)), this, SLOT(receiveShouldCount(bool)));
    connect(this, SIGNAL(sendIsCounting(bool)), manager, SLOT(receiveIsCounting(bool)));
    connect(ui->clearCountButton, SIGNAL(clicked(bool)), this, SLOT(receiveClearCount()));
    connect(this, SIGNAL(sendClearCount()), manager, SLOT(receiveClearCount()));

    connect(ui->actionCam, SIGNAL(triggered()), this, SLOT(receiveCam()));
    connect(ui->actionVideo_File, SIGNAL(triggered()), this, SLOT(receiveVideoFile()));

    managerTrigger->start();
    manager-> moveToThread(thread);
    managerTrigger->moveToThread(thread);

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
    if (!ui->playButton->text().compare(">")) ui->playButton->setText("||");
    else ui->playButton->setText(">");

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
    if (!ui->startCountButton->text().compare("-"))
    {
        ui->startCountButton->setText("Counting");
        emit sendIsCounting(true);
    }else {
        ui->startCountButton->setText("-");
        emit sendIsCounting(false);
    }

}

void MainWindow::receiveClearCount()
{
    this->ui->upLabel->setText(QString("Up 0"));
    this->ui->downLabel->setText(QString("Down 0"));

    emit sendClearCount();
}
