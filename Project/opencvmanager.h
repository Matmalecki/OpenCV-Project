#ifndef OPENCVMANAGER_H
#define OPENCVMANAGER_H

#include <QObject>
#include <QImage>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
using namespace cv;


class OpencvManager : public QObject {

    Q_OBJECT

private:
    Mat image;
    VideoCapture * cap;

    Ptr<BackgroundSubtractorMOG2>  bgsubtractor;

    bool status;
    bool toggleStream;

    void process();
    void checkIfDeviceAlreadyOpened(QByteArray device);

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
