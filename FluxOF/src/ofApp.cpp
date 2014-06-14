#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    
    //  Load STUFF
    //
    calibration.setup();
    imageQueue.setup();
    
    shoeModel.loadModel("models/LOCKED_SHOE_ROTATION.obj");
    terrainModel.loadModel("models/LOCKED_TERRAFORM.obj");
    
    //  Initialize the Render
    //
    renderEngine = new RenderRadar();
    
    imageQueue.renderEngine = renderEngine;
    
    renderEngine->shoeMesh = shoeModel.getMesh(0);
    renderEngine->terrainMesh = terrainModel.getMesh(0);
    
    ofLoadImage(renderEngine->terrainDepthMap, "models/terrainDepthMap.png");
    renderEngine->terrainResolution = renderEngine->terrainDepthMap.getWidth();
    
	renderEngine->setup();
    renderEngine->setImageQueue(&imageQueue);
    renderEngine->setCalibration(&calibration);
    
    //  Ready to go
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
