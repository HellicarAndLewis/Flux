#pragma once
#include "UIClass.h"
#include "ofxLibwebsockets.h"
#include "ImageQueue.h"
#include "RenderEngine.h"

class NodeCommunication : public UIClass {
public:
    void setup();
    void update();
    
    string  getClassName(){return "NODE-COM";};

    ofxLibwebsockets::Client client;
    ImageQueue * imageQueue;
    
    
    bool enabled;
    bool connected;
    float reconnectCounter;
    bool triggerPicture;
    
    ofxUILabel * statusLabel;
    
    
    // websocket methods
    void onConnect( ofxLibwebsockets::Event& args );
    void onOpen( ofxLibwebsockets::Event& args );
    void onClose( ofxLibwebsockets::Event& args );
    void onIdle( ofxLibwebsockets::Event& args );
    void onMessage( ofxLibwebsockets::Event& args );
    void onBroadcast( ofxLibwebsockets::Event& args );
    
protected:
    void    setupUI();
    void    guiEvent(ofxUIEventArgs &e);

};