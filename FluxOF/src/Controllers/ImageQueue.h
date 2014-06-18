#pragma once

#include "ofxXmlSettings.h"
#include "RenderEngine.h"

#include "UIClass.h"
#include "QueueItem.h"

class ImageQueue : public UIClass {
public:
    void loadQueueFromDir();
    void loadQueueFromFile();
    
    void storeQueueToFile();
    void cleanupQueue();
    
    void transitionToNextItem();
    void transitionTo(QueueItem item);
    
    void update();

    string  getClassName(){return "IMAGE-QUEUE";};
    
    //This will contain new incomming queue items from the node.js application
    deque<QueueItem> incommingItemsQueue;
    
    //This will contain old items that already have been shown, but in case there are no new images we show them again
    deque<QueueItem> oldItemsQueue;
    
    RenderEngine * renderEngine;
    
    ofxUILabel * incommingSizeLabel;
    ofxUILabel * oldSizeLabel;
    ofxUIImage * lastImageUI;
    ofxUIImage * currentImageUI;
    
    QueueItem lastItem;
    QueueItem currentItem;
    
    float transitionDelay;
    
    ofxXmlSettings storedQueue;
        
protected:
    void    setupUI();
    void    guiEvent(ofxUIEventArgs &e);    
};

