//
//  NodeCommunication.cpp
//  FluxOF
//
//  Created by Jonas Jongejan on 15/06/14.
//
//

#include "NodeCommunication.h"


void NodeCommunication::setup(){
    client.addListener(this);
}

void NodeCommunication::update(){
    if(!enabled){
        statusLabel->setLabel("DISABLED");
    } else {
        if(connected){
            statusLabel->setLabel("CONNECTED");
            statusLabel->setColorFill(ofColor(255,255,255));
        } else {
            statusLabel->setLabel("NOT CONNECTED!");
            statusLabel->setColorFill(ofColor(255,0,0));
        }
        
        if(!connected){
            reconnectCounter -= 1.0/ofGetFrameRate();
            if(reconnectCounter <= 0){
                cout<<"Reconnect node app"<<endl;
                client.connect("localhost",8080);
                reconnectCounter = 5.;
            }
        } else {
            //Connected
            if(imageQueue->currentItem.takePhoto || imageQueue->currentItem.username.length() > 0){
                if(triggerPicture && lastTriggerPicture != triggerPicture){
                    cout<<"Take picutre"<<endl;
                    //ofSaveScreen("images_screenshots/"+ofToString(imageQueue->currentItem.itemId)+".png");

                    client.send("{ \"type\": \"photoTrigger\", \"id\":"+ofToString(imageQueue->currentItem.itemId)+"}");
                }
            }
            lastTriggerPicture = triggerPicture;

        }
    }
}


//--------------------------------------------------------------
void NodeCommunication::onConnect( ofxLibwebsockets::Event& args ){
    cout<<"on connected"<<endl;
}

//--------------------------------------------------------------
void NodeCommunication::onOpen( ofxLibwebsockets::Event& args ){
    cout<<"on open"<<endl;
    connected = true;
}

//--------------------------------------------------------------
void NodeCommunication::onClose( ofxLibwebsockets::Event& args ){
    cout<<"on close"<<endl;
    connected = false;

}

//--------------------------------------------------------------
void NodeCommunication::onIdle( ofxLibwebsockets::Event& args ){
    cout<<"on idle"<<endl;
}

//--------------------------------------------------------------
void NodeCommunication::onMessage( ofxLibwebsockets::Event& args ){
    
    if(args.json["queue"].isArray()){
        const Json::Value queue = args.json["queue"];
        
        // Run thorugh the queue and look for new items we can add
        //
        for(int i=0;i<queue.size();i++){
            const Json::Value queueItem = queue[i];
            
            bool itemFound = false;
            
            //Look in the incomminItemsQueue for items with same ID
            for(int u=0;u<imageQueue->incommingItemsQueue.size();u++){
                if(imageQueue->incommingItemsQueue[u].itemId == queueItem["id"].asInt()){
                    itemFound = true;
                }
            }
            
            if(imageQueue->currentItem.itemId == queueItem["id"].asInt()){
                itemFound = true;
            }
            if(imageQueue->lastItem.itemId == queueItem["id"].asInt()){
                itemFound = true;
            }
            
            
            if(!itemFound){
                //No item found, lets create a new one, the image is loaded by imageQueue later on
                QueueItem newItem;
                newItem.itemId = queueItem["id"].asInt();
                newItem.path = "images/"+queueItem["id"].asString()+".png";
                newItem.username = queueItem["username"].asString();
                newItem.takePhoto = true;
                
                imageQueue->incommingItemsQueue.push_back(newItem);
            }
        }

    } else  {
        cout<<"got unkown message "<<args.json<<endl;
    }

}

//--------------------------------------------------------------
void NodeCommunication::onBroadcast( ofxLibwebsockets::Event& args ){
    cout<<"got broadcast "<<args.message<<endl;
}

void NodeCommunication::setupUI(){
    statusLabel = gui->addLabel("NODE NOT CONNECTED");
    
    gui->addToggle("TAKE PICTURE", &triggerPicture);
}

void NodeCommunication::guiEvent(ofxUIEventArgs &e){
    if(e.getName() == "ENABLE"){
        ofxUIToggle* toggle = e.getToggle();
        enabled = toggle->getValue();
    }

}
