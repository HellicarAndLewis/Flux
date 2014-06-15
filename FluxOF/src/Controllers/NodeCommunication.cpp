//
//  NodeCommunication.cpp
//  FluxOF
//
//  Created by Jonas Jongejan    Firma HalfdanJ on 15/06/14.
//
//

#include "NodeCommunication.h"


void NodeCommunication::setup(){
    client.connect("localhost",8080);
    client.addListener(this);

}


//--------------------------------------------------------------
void NodeCommunication::onConnect( ofxLibwebsockets::Event& args ){
    cout<<"on connected"<<endl;
}

//--------------------------------------------------------------
void NodeCommunication::onOpen( ofxLibwebsockets::Event& args ){
    cout<<"on open"<<endl;
}

//--------------------------------------------------------------
void NodeCommunication::onClose( ofxLibwebsockets::Event& args ){
    cout<<"on close"<<endl;
}

//--------------------------------------------------------------
void NodeCommunication::onIdle( ofxLibwebsockets::Event& args ){
    cout<<"on idle"<<endl;
}

//--------------------------------------------------------------
void NodeCommunication::onMessage( ofxLibwebsockets::Event& args ){
    cout<<"got message "<<args.message<<endl;
}

//--------------------------------------------------------------
void NodeCommunication::onBroadcast( ofxLibwebsockets::Event& args ){
    cout<<"got broadcast "<<args.message<<endl;
}
