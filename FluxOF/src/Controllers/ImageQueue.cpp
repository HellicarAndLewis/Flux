//
//  ImageQueue.cpp
//  FluxOF
//
//  Created by Jonas Jongejan on 14/06/14.
//
//

#include "ImageQueue.h"

void ImageQueue::loadQueueFromDir(){
    //Mockup data
    string path = "images";
    ofDirectory backgroundsDir(path);
    if(backgroundsDir.exists()){
		backgroundsDir.listDir();
        int total = backgroundsDir.getFiles().size();
        for (int i = 0; i < total; i++) {
            QueueItem newItem;
            newItem.path = "images/"+backgroundsDir.getName(i);
            newItem.image.loadImage(newItem.path);
            newItem.username = "@Username"+ofToString(i+1);
            
            incommingItemsQueue.push_back(newItem);
        }
    }
    
    transitionDelay = -1;
}

void ImageQueue::setupUI(){
    incommingSizeLabel = gui->addLabel("");;
    oldSizeLabel = gui->addLabel("");;
    
    lastImageUI = gui->addImage("Last image", nil, 100, 100, true);
    currentImageUI = gui->addImage("Next image", nil, 100, 100, true);
    
    gui->autoSizeToFitWidgets();
    
}


void ImageQueue::update(){
    incommingSizeLabel->setLabel(ofToString(incommingItemsQueue.size())+" incomming items");
    oldSizeLabel->setLabel(ofToString(oldItemsQueue.size())+" old items");
    
    
    // Check if there are items that need to get loaded
    //
    for(int i=0;i<incommingItemsQueue.size();i++){
        if(!incommingItemsQueue[i].image.isAllocated()){
            incommingItemsQueue[i].image.loadImage(incommingItemsQueue[i].path);
        }
    }
    for(int i=0;i<oldItemsQueue.size();i++){
        if(!oldItemsQueue[i].image.isAllocated()){
            oldItemsQueue[i].image.loadImage(oldItemsQueue[i].path);
        }
    }
    
    
    // Determine if we should start a new transition
    //
    if(renderEngine->transitionDone() || currentItem.path.length() == 0){
        // Determine if the transition should be delayed
        //
        if(transitionDelay == -1){
            if(incommingItemsQueue.size() > 0){
                transitionDelay = 0;
            } else {
                transitionDelay = ofRandom(5);
            }
        }
        transitionDelay -= 1.0/ofGetFrameRate();
        
        //If the delay counter has reached zero, then do the transition
        if(transitionDelay <= 0){
            transitionToNextItem();
        }
    }
    
}


//
// Determines what next image to show, and starts the transition and does some cleanup
//
void ImageQueue::transitionToNextItem(){
    
    //Determine if there are new images to show, or we should pick an old one
    //
    if(incommingItemsQueue.size() > 0){
        //Transition to the first item in the incommint items
        transitionTo(incommingItemsQueue[0]);
        oldItemsQueue.push_back(incommingItemsQueue[0]);
        incommingItemsQueue.pop_front();
    } else if(oldItemsQueue.size() > 0){
        //Transition to the first item in the old items, and move it to the back
        transitionTo(oldItemsQueue[0]);
        oldItemsQueue.push_back(oldItemsQueue[0]);
        oldItemsQueue.pop_front();
    }
    
    
    cleanupQueue();
    storeQueueToFile();
}

//
// Starts a transition to a given queue item
//
void ImageQueue::transitionTo(QueueItem item){
    lastItem = currentItem;
    currentItem = item;
    
    // Tell the render engine to start the transition
    //
    renderEngine->startTransitionTo(currentItem);
    renderEngine->getTimeline()->setDurationInSeconds(10);
    
    // Update the UI to show the images we transition between
    //
    if(lastItem.image.bAllocated())
        lastImageUI->setImage(&lastItem.image);
    currentImageUI->setImage(&currentItem.image);
}




//
// This function will go through the old items queue and remove duplicates
// TODO: remove images that are old
//
void ImageQueue::cleanupQueue(){
    for(int i=0;i<oldItemsQueue.size();i++){
        for(int u=i+1;u<oldItemsQueue.size();u++){
            if(oldItemsQueue[i].itemId == oldItemsQueue[u].itemId){
                oldItemsQueue.erase(oldItemsQueue.begin()+i);
            }
        }
    }
}

//
// Stores the old items queue to a file in case the application needs to restart
//
void ImageQueue::storeQueueToFile(){
    storedQueue.clear();
    
    for(int i=0;i<oldItemsQueue.size();i++){
        storedQueue.addTag("item");
        storedQueue.setValue("item:username", oldItemsQueue[i].username, i);
        storedQueue.setValue("item:itemId", oldItemsQueue[i].itemId, i);
        storedQueue.setValue("item:path", oldItemsQueue[i].path, i);
    }
    
    storedQueue.save("imageQueue.xml");
}

//
// Loads the file from the previous function
//
void ImageQueue::loadQueueFromFile(){
    ofxXmlSettings load;
    load.load("imageQueue.xml");
    
    for(int i=0;i<load.getNumTags("item");i++){
        load.pushTag("item",i);
        QueueItem newItem;
        newItem.path = load.getValue("path", "");
        newItem.username = load.getValue("username", "");
        newItem.itemId = load.getValue("itemId", 0);
        newItem.takePhoto = false;
        
        oldItemsQueue.push_back(newItem);
        
        load.popTag();
    }
}


//
//
//
void ImageQueue::guiEvent(ofxUIEventArgs &e){
    
}
