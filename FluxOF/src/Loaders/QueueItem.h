#pragma once
#include "ofMain.h"

class QueueItem {
public:
    ofImage image;
    string path;
    string username;
    int itemId;
    bool takePhoto;
};