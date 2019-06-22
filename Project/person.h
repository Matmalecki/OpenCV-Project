#pragma once


#ifndef PERSON_H
#define PERSON_H


#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
using namespace cv;
using namespace std;

class Person {

public:
    size_t personId;
    Rect currentRect;
    Point centerPositions;
    Point origin;
    bool counted = false;
    bool active = true;
    Person(Rect boundingRect, Point center);
    void predictNextPosition(void);

};

#endif // PERSON_H
