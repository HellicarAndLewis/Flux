//
//  Prototype01.cpp
//
//  Created by Patricio Gonzalez Vivo on 9/23/13.
//
//
#include "Prototype01.h"

void Prototype01::selfSetup(){
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofEnableSmoothing();

    terrainTransition.load(getDataPath()+"shaders/terrainTrans");
    shoeTransition.load(getDataPath()+"shaders/shoesTrans");
}

void Prototype01::selfSetupGuis(){
    backgroundSet(new UIMapBackground());
    lightAdd("SPOT", OF_LIGHT_SPOT);
    
    guiAdd(terrainTransition);
    guiAdd(shoeTransition);
}

//Some helper functions
//--------------------------------------------------------------
void Prototype01::addFace(ofMesh& mesh, ofVec3f a, ofVec3f b, ofVec3f c) {
	ofVec3f normal = ((b - a).cross(c - a)).normalize();
	mesh.addNormal(normal);
	mesh.addVertex(a);
	mesh.addNormal(normal);
	mesh.addVertex(b);
	mesh.addNormal(normal);
	mesh.addVertex(c);
}

//--------------------------------------------------------------
void Prototype01::addFace(ofMesh& mesh, ofVec3f a, ofVec3f b, ofVec3f c, ofVec3f d) {
	addFace(mesh, a, b, c);
	addFace(mesh, a, c, d);
}

//--------------------------------------------------------------
ofVec3f Prototype01::getVertexFromImg(ofFloatImage& img, int x, int y) {
	return ofVec3f(x, y, 100 * img.getColor(x, y).getBrightness());
}

void Prototype01::selfGuiEvent(ofxUIEventArgs &e){
    
}

void Prototype01::selfSetupSystemGui(){
    sysGui->addLabel("Shoe_Position");
    sysGui->add2DPad("Shoe_Translation", ofPoint(-100,100), ofPoint(-100,100), &shoeTranslation);
    sysGui->addSlider("Shoe_Altitude", 0, 200, &shoeAltitud);
    
    sysGui->addLabel("Shoe_Rotation");
    sysGui->addSlider("Shoe_X_Rot", -1., 1., &shoeRotation.x );
    sysGui->addSlider("Shoe_Y_Rot", -1., 1., &shoeRotation.y );
    sysGui->addSlider("Shoe_Z_Rot", -1., 1., &shoeRotation.z );
    
    sysGui->addSpacer();
    sysGui->addSlider("Shoe_Scale", 0, 0.5, &shoeScale);
}

void Prototype01::guiSystemEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    
}

void Prototype01::selfSetupRenderGui(){
    
}

void Prototype01::guiRenderEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    
}

//---------------------------------------------------

void Prototype01::selfBegin(){
    
    //  TERRAIN
    //
    terrainImg.loadImage(getDataPath()+"nyc-small.exr");
    
    terrainMesh.setMode(OF_PRIMITIVE_TRIANGLES);
	int skip = 1;
	int width = terrainImg.getWidth();
	int height = terrainImg.getHeight();
	for(int y = 0; y < height - skip; y += skip) {
		for(int x = 0; x < width - skip; x += skip) {
			ofVec3f nw = getVertexFromImg(terrainImg, x, y);
			ofVec3f ne = getVertexFromImg(terrainImg, x + skip, y);
			ofVec3f sw = getVertexFromImg(terrainImg, x, y + skip);
			ofVec3f se = getVertexFromImg(terrainImg, x + skip, y + skip);
			
			addFace(terrainMesh, nw, ne, se, sw);
		}
	}

    //  SHOES
    //
    shoeModel.loadModel(getDataPath()+"shoe.dae");
    shoeMesh = shoeModel.getMesh(0);
    
    
    //  FONTS
    //
    font.loadFont(getDataPath()+"Exo2-Light.ttf", 44);
    font.setSpaceSize(0.65);
}

void Prototype01::selfUpdate(){

}

void Prototype01::selfDraw(){
    materials["MATERIAL 1"]->begin();
    
    ofPushMatrix();
    ofScale(1, -1, 1);
    ofTranslate(-terrainImg.getWidth() / 2, -terrainImg.getHeight() / 2, 0);
    ofSetColor(255);
    terrainTransition.begin();
    terrainMesh.draw();
    terrainTransition.end();
    ofPopMatrix();
    
    ofPushMatrix();
    shoeTransition.begin();
    ofTranslate(shoeTranslation.x,shoeTranslation.y, shoeAltitud);
    ofScale(shoeScale,shoeScale,shoeScale);
    
    //  This could be better... way! better
    //
    ofRotate(90, shoeRotation.x, shoeRotation.y, shoeRotation.z);
    
    ofTranslate(-shoeMesh.getCentroid());
    
    shoeMesh.draw();
    
    shoeTransition.end();
    ofPopMatrix();
    
    materials["MATERIAL 1"]->end();
}

void Prototype01::selfDrawOverlay(){

}

void Prototype01::selfEnd(){
    
}

void Prototype01::selfExit(){
    
}

void Prototype01::selfKeyPressed(ofKeyEventArgs & args){

}

void Prototype01::selfKeyReleased(ofKeyEventArgs & args){
	
}

void Prototype01::selfMouseMoved(ofMouseEventArgs& data){
	
}

void Prototype01::selfMousePressed(ofMouseEventArgs& data){

}

void Prototype01::selfMouseDragged(ofMouseEventArgs& data){

}

void Prototype01::selfMouseReleased(ofMouseEventArgs& data){

}