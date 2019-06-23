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
    bool isCounting = false;
    bool debugWindow = false;

    const int HUMAN_SIZE_MIN = 950;
    const int HUMAN_SIZE_MAX = 5800;


    void process();
    void checkIfDeviceAlreadyOpened(QByteArray device);
    bool checkIfCrossedLine(Point p1, Point p2);
public:
    explicit OpencvManager(QObject * parent = 0);
    ~OpencvManager();

signals:
    void sendSourceFrame(QImage image);
    void sendStatus(QString msg, int code);
    void sendUpCounter(int countUp);
    void sendDownCounter(int countDown);
public slots:
    void receiveSetup(QByteArray device);
    void receiveGrabFrame();
    void receiveToggleStream();
    void receiveIsCounting(bool shouldCount);
    void receiveClearCount();
    void receiveToggleDebug();
};


#endif // OPENCVMANAGER_H
