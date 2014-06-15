#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    renderEngine = NULL;
    
    //  Load stuff
    //
    calibration.load();
    imageQueue.load();
    
    loadScene();
    loadAnimation(LASERS);
}

void ofApp::loadScene(){
    
    //  Load modeles
    //
    ofxAssimpModelLoader shoeModel;
    ofxAssimpModelLoader terrainModel;
    shoeModel.loadModel("models/LOCKED_SHOE_ROTATION.obj");
    terrainModel.loadModel("models/LOCKED_TERRAFORM.obj");
    
    //  Calculate Scene Min/Max values
    //
    terrainMesh = terrainModel.getMesh(0);
    shoeMesh = shoeModel.getMesh(0);
    sceneMax.set(0, 0, 0);
    sceneMin.set(100000, 100000, 100000);
	for(unsigned int i = 0; i < terrainMesh.getVertices().size(); i++) {
		if (terrainMesh.getVertices()[i].x < sceneMin.x ){
            sceneMin.x = terrainMesh.getVertices()[i].x;
        }
        
        if (terrainMesh.getVertices()[i].x > sceneMax.x ){
            sceneMax.x = terrainMesh.getVertices()[i].x;
        }
        
        if (terrainMesh.getVertices()[i].y < sceneMin.y ){
            sceneMin.y = terrainMesh.getVertices()[i].y;
        }
        
        if (terrainMesh.getVertices()[i].y > sceneMax.y ){
            sceneMax.y = terrainMesh.getVertices()[i].y;
        }
        
        if (terrainMesh.getVertices()[i].z < sceneMin.z ){
            sceneMin.z = terrainMesh.getVertices()[i].z;
        }
        
        if (terrainMesh.getVertices()[i].z > sceneMax.z ){
            sceneMax.z = terrainMesh.getVertices()[i].z;
        }
	}
    
    for(unsigned int i = 0; i < shoeMesh.getVertices().size(); i++) {
        if (shoeMesh.getVertices()[i].x < sceneMin.x ){
            sceneMin.x = shoeMesh.getVertices()[i].x;
        }
        
        if (shoeMesh.getVertices()[i].x > sceneMax.x ){
            sceneMax.x = shoeMesh.getVertices()[i].x;
        }
        
        if (shoeMesh.getVertices()[i].y < sceneMin.y ){
            sceneMin.y = shoeMesh.getVertices()[i].y;
        }
        
        if (shoeMesh.getVertices()[i].y > sceneMax.y ){
            sceneMax.y = shoeMesh.getVertices()[i].y;
        }
        
        if (shoeMesh.getVertices()[i].z < sceneMin.z ){
            sceneMin.z = shoeMesh.getVertices()[i].z;
        }
        
        if (shoeMesh.getVertices()[i].z > sceneMax.z ){
            sceneMax.z = shoeMesh.getVertices()[i].z;
        }
	}
    
    //  Load Textures
    //
    ofLoadImage(terrainDepthMap, "models/terrainDepthMap.png");
    ofLoadImage(terrainNormalMap, "models/terrainNormalMap.png");
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
    renderEngine->shoeMesh = shoeMesh;
    renderEngine->terrainMesh = terrainMesh;
    
    renderEngine->sceneMin = sceneMin;
    renderEngine->sceneMax = sceneMax;
    
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
