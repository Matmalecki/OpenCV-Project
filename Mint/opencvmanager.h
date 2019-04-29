#ifndef OPENCVMANAGER_H
#define OPENCVMANAGER_H

#include <QObject>
#include <QImage>)
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;


class OpencvManager : public QObject {

    Q_OBJECT

private:
    Mat image;
    Mat processedImage;
    VideoCapture * cap;

    bool status;
    bool toggleStream;
    bool binaryThresholdEnable;
    int binaryThreshold;

    void process();
    void checkIfDeviceAlreadyOpened(const int device);

public:
    explicit OpencvManager(QObject * parent = 0);
    ~OpencvManager();
signals:
    void sendFrame(QImage frameProcessed);
    void sendStatus(QString msg, int code);
public slots:
    void receiveSetup(const int device);
    void receiveGrabFrame();
    void receiveToggleStream();

    void receiveEnableBinaryThreshold();
    void receiveBinaryThreshold(int threshold);

};


#endif // OPENCVMANAGER_H
