//
//  ImageQueue.cpp
//  FluxOF
//
//  Created by Jonas Jongejan on 14/06/14.
//
//

#include "ImageQueue.h"

void ImageQueue::load(){
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
}

void ImageQueue::transitionToNextItem(){
    
    //Determine if there are new images to show, or we should pick an old one
    //
    if(incommingItemsQueue.size() > 0){
        //Transition to the first item in the incommint items
        transitionTo(incommingItemsQueue[0]);
        oldItemsQueue.push_back(incommingItemsQueue[0]);
        incommingItemsQueue.pop_front();
    } else {
        //Transition to the first item in the old items, and move it to the back
        transitionTo(oldItemsQueue[0]);
        oldItemsQueue.push_back(oldItemsQueue[0]);
        oldItemsQueue.pop_front();
    }
    
}

void ImageQueue::transitionTo(QueueItem item){
    lastItem = currentItem;
    currentItem = item;
    
    // Tell the render engine to start the transition
    //
    renderEngine->startTransitionTo(currentItem);
    renderEngine->getTimeline()->setDurationInSeconds(5);
    
    // Update the UI to show the images we transition between
    //
    if(lastItem.image.bAllocated())
        lastImageUI->setImage(&lastItem.image);
    currentImageUI->setImage(&currentItem.image);
}

void ImageQueue::update(){
    incommingSizeLabel->setLabel(ofToString(incommingItemsQueue.size())+" incomming items");
    
    // Determine if we should start a new transition
    //
    if(renderEngine->transitionDone()){
        transitionToNextItem();
    }
}

void ImageQueue::setupUI(){
    incommingSizeLabel = gui->addLabel("test");;

    lastImageUI = gui->addImage("Last image", nil, 100, 100, true);
    currentImageUI = gui->addImage("Next image", nil, 100, 100, true);
    
    gui->autoSizeToFitWidgets();

}

void ImageQueue::guiEvent(ofxUIEventArgs &e){
    
}
