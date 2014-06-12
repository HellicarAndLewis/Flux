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
    
    audio.setup(44100, 256);
    audio.start();
}

void Prototype01::selfSetupGuis(){
    backgroundSet(new UIMapBackground());
    lightAdd("SPOT", OF_LIGHT_SPOT);
    
    guiAdd(audio);
    
    guiAdd(terrainTransition);
    guiAdd(shoeTransition);
    
}

void Prototype01::selfGuiEvent(ofxUIEventArgs &e){
    
}

void Prototype01::selfSetupSystemGui(){
    sysGui->addSpacer();
    sysGui->addLabel("Render mode");
    sysGui->addToggle("Simulator", &simulatorMode);
    
    sysGui->addLabel("Shoe_Position");
    sysGui->add2DPad("Shoe_Translation", ofPoint(-10,10), ofPoint(-10,10), &shoeTranslation);
    sysGui->addSlider("Shoe_Altitude", -10, 10, &shoeTranslation.z);
    
    sysGui->addSpacer();
    sysGui->addSlider("Shoe_Scale", 0, 1.0, &shoeScale);
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
    
    //  SHOES
    //
    ofxAssimpModelLoader loader;
    loader.loadModel(getDataPath()+"terrain_and_shoe.dae");
    
    shoeTex.allocate(1500, 1500);
    shoeMesh = loader.getMesh(1);
    
    //  TERRAIN
    //
    terrainTex.allocate(1500, 1500);
    terrainMesh = loader.getMesh(0);
    
    //  FONTS
    //
    font.loadFont(getDataPath()+"Exo2-Light.ttf", 44);
    font.setSpaceSize(0.65);
}

void Prototype01::startTransitionTo(string _twitterUser, ofTexture &_shoeTex, ofTexture &_terrainTex){
    shoeDestTex = _shoeTex;
    terrainDestTex = _terrainTex;
    
    timeline->setPercentComplete(0.0);
    timeline->play();
}

void Prototype01::selfUpdate(){
    if(simulatorMode){
        setupNumViewports(1);
        cameraEnable(true);
    } else {
        setupNumViewports(2);
        cameraEnable(false);
    }

    //  TERRAIN TEXTURE
    //
    terrainTex.dst->begin();
    
    // - TWIITER TEXT
    
    terrainTex.dst->end();
    
    //  SHOE TEXTURE
    //
    shoeTex.dst->begin();
    
    shoeTex.dst->end();
}

void Prototype01::selfDraw(){
    materials["MATERIAL 1"]->begin();
    
    
    ofPushMatrix();
    if(currentViewPort == 1){
        ofTranslate(-100, 0,0);
    }

    ofSetColor(255);
    
    //  TERRAIN
    //

    //  Load Terrain Calib
    
    terrainTransition.begin();
    terrainMesh.draw();
    terrainTransition.end();

    //  SHOE
    //
    
    ofScale(shoeScale, shoeScale,shoeScale);
    ofTranslate(shoeTranslation);
    
    //  Load Shoe Calib
    
    shoeTransition.begin();
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