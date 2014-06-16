#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    renderEngine = NULL;
    
    
    //  Initialize the Render
    //
    renderEngine = new RenderRadar();
    
    // Setup the controllers
    //
    imageQueue.renderEngine = renderEngine;
    imageQueue.loadQueueFromFile();

    nodeCommunication.imageQueue = &imageQueue;
    nodeCommunication.setup();
    
    
    //  Load STUFF
    //
    calibration.load();
    renderAssets.load();
    
    loadScene();
    loadAnimation(LASERS);
}

void ofApp::loadScene(){
    
    
}

void ofApp::loadAnimation(ANIMATION_STYLE _animation){
    
    if (renderEngine != NULL){
        renderEngine->stop();
        delete renderEngine;
        renderEngine = NULL;
    }
    
    if (_animation == RADAR){
        renderEngine = new RenderRadar();
    } else if (_animation == LASERS){
        renderEngine = new RenderLasers();
    } else {
        return;
    }
    
    //  Link renderEngine to ImageQueue
    //
    imageQueue.renderEngine = renderEngine;

    //  Loading Resources (this could be pointers)
    //
    renderEngine->assets = &renderAssets;
        
    //  Setup The RenderEngine
    //
	renderEngine->setup();

    renderEngine->addUiClass(&imageQueue);
    renderEngine->addUiClass(&nodeCommunication);
    renderEngine->setCalibration(&calibration);
    
    //  Ready to GO
    //
	renderEngine->play();
    renderEngine->getTimeline()->setDurationInSeconds(5);
//    imageQueue.transitionTo(imageQueue.currentItem);
}

//--------------------------------------------------------------
void ofApp::update(){
    nodeCommunication.update();
    imageQueue.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if(key == OF_KEY_F1){
        loadAnimation(RADAR);
    } else if (key == OF_KEY_F2){
        loadAnimation(LASERS);
    }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
