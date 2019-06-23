#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QThread>
#include "opencvmanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QThread * thread;
    Ui::MainWindow *ui;
    void setUpManager();
signals:
    void sendSetup(QByteArray device);
    void sendToggleStream();
    void sendToggleDebug();
    void sendIsCounting(bool isCounting);
    void sendClearCount();
private slots:
    void receiveSourceFrame(QImage frame);
    void receiveToggleStream();
    void receiveUpCount(int count);
    void receiveDownCount(int count);
    void receiveShouldCount(bool shouldCount);
    void receiveClearCount();
    void receiveVideoFile();
    void receiveCam();
    void receiveToggleDebug();
};

#endif // MAINWINDOW_H
