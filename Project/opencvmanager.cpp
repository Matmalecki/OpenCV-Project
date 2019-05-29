#include "opencvmanager.h"

using namespace cv;
using namespace std;

OpencvManager::OpencvManager(QObject * parent) : QObject (parent), status(false), toggleStream(false)
{

    cap = new cv::VideoCapture();

     bgsubtractor = createBackgroundSubtractorMOG2();
    bgsubtractor->setNMixtures(100);
}

OpencvManager::~OpencvManager()
{
    if (cap->isOpened()) cap->release();
    delete cap;
    image.release();
    bgsubtractor.release();
}

void OpencvManager::checkIfDeviceAlreadyOpened(QByteArray device)
{

    if (cap->isOpened()) cap->release();
 /*
    if (strcmp(device, "cam") == 0) cap->open(0);
    else {
        qDebug(device.toStdString().c_str());
       cap->open(device.toStdString().c_str());
    }
    */

    cap->open("C:/Users/MM/Documents/QtProjects/Qt_Opencv/Project/sample_video.mp4");

    //qDebug(cap->get(CAP_PROP_FPS));
}


void OpencvManager::receiveGrabFrame()
{
    if (!toggleStream)
    {
        return;
    }
    cap->read(image);
    if (image.empty()) {
        cap->set(CV_CAP_PROP_POS_FRAMES, 0);
        cap->read(image);
    }

    process();

    QImage outputSource((const unsigned char *)image.data, image.cols, image.rows, QImage::Format_RGB888);
    emit sendSourceFrame(outputSource);

}


void OpencvManager::process()
{

     Mat processed;
     bgsubtractor->apply(image, processed);

     morphologyEx(processed, processed, MORPH_OPEN, getStructuringElement(0,Size(3,3)));

     vector<vector<Point> > contours;
     vector<Vec4i> hierarchy;
     findContours(processed, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));


     drawContours(image, contours, -1, Scalar(50, 150, 25));


     //debug
     namedWindow("debug");
     imshow("debug", processed);

     processed.release();


     waitKey(15);
}

void OpencvManager::receiveSetup(QByteArray device)
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



