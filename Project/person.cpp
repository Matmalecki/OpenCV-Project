#include "person.h"

using namespace std;
using namespace cv;

Person::Person(Rect boundingRect, Point center)
{
    this->currentRect = boundingRect;
    this->centerPositions = center;
    personId = 0;
}
