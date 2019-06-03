#ifndef OPENCVMANAGER_H
#define OPENCVMANAGER_H

#include <QObject>
#include <QImage>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <opencv2/video/background_segm.hpp>
#include "person.h"

using namespace cv;
using namespace std;

class OpencvManager : public QObject {

    Q_OBJECT

private:
    Mat image;
    VideoCapture * cap;

    Ptr<BackgroundSubtractorMOG2>  bgsubtractor;
    vector<Person> people;
    const int DISTANCE_OBJECT = 40;
    const Scalar color;
    bool status;
    bool toggleStream;
    int nextId = 1;
    int countUp = 0;
    int countDown = 0;

    void process();
    void checkIfDeviceAlreadyOpened(QByteArray device);
    double distanceBetweenPoints(Point p1, Point p2);
    bool checkIfCrossedLine(Point p1, Point p2);
public:
    explicit OpencvManager(QObject * parent = 0);
    ~OpencvManager();
signals:
    void sendSourceFrame(QImage image);
    void sendStatus(QString msg, int code);
public slots:
    void receiveSetup(QByteArray device);
    void receiveGrabFrame();
    void receiveToggleStream();

};


#endif // OPENCVMANAGER_H
