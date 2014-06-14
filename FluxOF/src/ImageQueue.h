#pragma once

#include "UIClass.h"
#include "Prototype01.h"

#include "QueueItem.h"

class ImageQueue : public UIClass {
public:
    void setup(Prototype01 * project);
    void update();
    
    void transitionToNextItem();
    void transitionTo(QueueItem item);

    string  getClassName(){return "IMAGE-QUEUE";};
    
    //This will contain new incomming queue items from the node.js application
    deque<QueueItem> incommingItemsQueue;
    
    //This will contain old items that already have been shown, but in case there are no new images we show them again
    deque<QueueItem> oldItemsQueue;
    
    
    Prototype01 * renderEngine;
    
    ofxUILabel * incommingSizeLabel;
    ofxUIImage * lastImageUI;
    ofxUIImage * currentImageUI;
    
    QueueItem lastItem;
    QueueItem currentItem;
        
protected:
    void    setupUI();
    void    guiEvent(ofxUIEventArgs &e);    
};

