#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    managerTrigger->setInterval(1);

    connect(managerTrigger, SIGNAL(timeout()), manager, SLOT(receiveGrabFrame()));
    connect(this, SIGNAL(sendSetup(int)), manager, SLOT(receiveSetup(int)));
    connect(this, SIGNAL(sendToggleStream()), manager, SLOT(receiveToggleStream()));
    connect(ui->playButton, SIGNAL(clicked(bool)), this, SLOT(receiveToggleStream()));
    connect(manager, SIGNAL(sendFrame(QImage)), this, SLOT(receiveFrame(QImage)));
    connect(ui->sliderThreshold, SIGNAL(valueChanged(int)), manager, SLOT(receiveBinaryThreshold(int)));
    connect(ui->enableThresholdCheckbox, SIGNAL(toggled(bool)), manager, SLOT(receiveEnableBinaryThreshold()));
    connect(thread, SIGNAL(finished()), manager, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), managerTrigger, SLOT(deleteLater()));


    connect(ui->sliderThreshold, SIGNAL(valueChanged(int)), ui->thresholdValue, SLOT(setNum(int)));

    managerTrigger->start();
    manager-> moveToThread(thread);
    managerTrigger->moveToThread(thread);

    thread->start();

    emit sendSetup(0);
}


void MainWindow::receiveFrame(QImage frame)
{
    ui->videoField->setPixmap(QPixmap::fromImage(frame));
}

void MainWindow::receiveToggleStream()
{
    if (!ui->playButton->text().compare(">")) ui->playButton->setText("||");
    else ui->playButton->setText(">");

    emit sendToggleStream();
}
