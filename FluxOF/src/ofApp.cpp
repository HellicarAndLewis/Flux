#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    renderEngine = NULL;
    
    //  Load STUFF
    //
    calibration.setup();
    imageQueue.setup();
    
    shoeModel.loadModel("models/LOCKED_SHOE_ROTATION.obj");
    terrainModel.loadModel("models/LOCKED_TERRAFORM.obj");
    
    ofLoadImage(terrainDepthMap, "models/terrainDepthMap.png");
    ofLoadImage(terrainNormalMap, "models/terrainNormalMap.png");

    loadAnimation(RADAR);
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
    renderEngine->shoeMesh = shoeModel.getMesh(0);
    renderEngine->terrainMesh = terrainModel.getMesh(0);
    
    renderEngine->terrainDepthMap = terrainDepthMap;
    renderEngine->terrainNormalMap = terrainNormalMap;
    renderEngine->terrainResolution = terrainDepthMap.getWidth();
    
    //  Setup The RenderEngine
    //
	renderEngine->setup();
    renderEngine->setImageQueue(&imageQueue);
    renderEngine->setCalibration(&calibration);
    
    //  Ready to GO
    //
	renderEngine->play();
    imageQueue.transitionToNextItem();
}

//--------------------------------------------------------------
void ofApp::update(){
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
