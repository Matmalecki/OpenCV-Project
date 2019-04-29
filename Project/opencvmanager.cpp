#include "opencvmanager.h"

using namespace cv;

OpencvManager::OpencvManager(QObject * parent) : QObject (parent), status(false), toggleStream(false),
  binaryThresholdEnable(false), binaryThreshold(127)
{

    cap = new cv::VideoCapture();



}

OpencvManager::~OpencvManager()
{
    if (cap->isOpened()) cap->release();
    delete cap;
}

void OpencvManager::checkIfDeviceAlreadyOpened(const int device)
{
    if (cap->isOpened()) cap->release();
    cap->open(device);

}


void OpencvManager::receiveGrabFrame()
{
    if (!toggleStream)
    {
        return;
    }
    cap->read(image);
    if (image.empty()) return;



    process();

    QImage outputProcessed((const unsigned char *)processedImage.data, processedImage.cols, processedImage.rows, QImage::Format_Indexed8);
    QImage outputSource((const unsigned char *)image.data, image.cols, image.rows, QImage::Format_RGB888);

    emit sendSourceFrame(outputSource);

    emit sendProcessedFrame(outputProcessed);

}

void OpencvManager::process()
{

     //set up
     cvtColor(image, processedImage, CV_BGR2GRAY);
     cvtColor(image, image,CV_BGR2RGB);

     if (binaryThresholdEnable)
        threshold(processedImage, processedImage, binaryThreshold, 255, CV_THRESH_BINARY);
}

void OpencvManager::receiveSetup(const int device)
{
    checkIfDeviceAlreadyOpened(device);
    if (!cap->isOpened())
    {
        status = false;
        return;
    }
    status = true;
}

void OpencvManager::receiveToggleStream()
{
    toggleStream = !toggleStream;
}

void OpencvManager::receiveEnableBinaryThreshold(){
    binaryThresholdEnable = !binaryThresholdEnable;
}

void OpencvManager::receiveBinaryThreshold(int threshold)
{
    binaryThreshold = threshold;
}



