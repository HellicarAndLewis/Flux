#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    
    
    //  Initialize the Render
    //
    renderEngine = new RenderRadar();
    
    // Setup the controllers
    //
    imageQueue.renderEngine = renderEngine;
    imageQueue.setup();

    nodeCommunication.renderEngine = renderEngine;
    nodeCommunication.imageQueue = &imageQueue;
    nodeCommunication.setup();
    
    
    //  Load STUFF
    //
    calibration.setup();
    
    shoeModel.loadModel("models/LOCKED_SHOE_ROTATION.obj");
    terrainModel.loadModel("models/LOCKED_TERRAFORM_002.obj");
    
    
    
    
    renderEngine->shoeMesh = shoeModel.getMesh(0);
    renderEngine->terrainMesh = terrainModel.getMesh(0);
    
    ofLoadImage(renderEngine->terrainDepthMap, "models/terrainDepthMap.png");
    ofLoadImage(renderEngine->terrainNormalMap, "models/terrainNormalMap.png");
    renderEngine->terrainResolution = renderEngine->terrainDepthMap.getWidth();
    
	renderEngine->setup();

    renderEngine->addUiClass(&imageQueue);
    renderEngine->addUiClass(&nodeCommunication);
    renderEngine->setCalibration(&calibration);
    
    //  Ready to go
    //
	renderEngine->play();
    
    normalMap.allocate(renderEngine->terrainResolution,renderEngine->terrainResolution);
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
