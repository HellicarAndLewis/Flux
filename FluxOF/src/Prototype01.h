//
//  Prototype01.h
//
//  Created by Patricio Gonzalez Vivo on 9/23/13.
//
//

#pragma once

#include "UITime3DProject.h"
#include "UIShader.h"
#include "UIBufferIn.h"

#include "ofxCv.h"
#include "ofxAssimpModelLoader.h"

class Prototype01 : public UITime3DProject {
public:
    
    string getSystemName(){ return "Prototype01";}
    
    void selfSetupGuis();
    void selfGuiEvent(ofxUIEventArgs &e);
    
    void selfSetupSystemGui();
    void guiSystemEvent(ofxUIEventArgs &e);
    
    void selfSetupRenderGui();
    void guiRenderEvent(ofxUIEventArgs &e);
    
    void selfBegin();

    void selfSetup();
    void selfUpdate();
    
    void selfDraw();
    void selfDrawOverlay();
    
	void selfEnd();
    void selfExit();
    
    void selfKeyPressed(ofKeyEventArgs & args);
    void selfKeyReleased(ofKeyEventArgs & args);
    
    void selfMouseDragged(ofMouseEventArgs& data);
    void selfMouseMoved(ofMouseEventArgs& data);
    void selfMousePressed(ofMouseEventArgs& data);
    void selfMouseReleased(ofMouseEventArgs& data);
    
    void startTransitionTo(string _twitterUser, string _twitterImgPath);
    
protected:
	
    UIBufferIn      audio;
    
    // TEXT
    //
    ofTrueTypeFont  font;
    string          text;

    // TERRAIN
    //
    ofVboMesh       terrainMesh;
    ofTexture       terrainDepthMap;
    UIShader        terrainTransition;
    PingPong        terrainTex;
    vector<ofColor> terrainPalette;
    
    //  SHOES
    //
    ofVboMesh       shoeMesh;
    UIShader        shoeTransition;
    PingPong        shoeTex;
    ofTexture       shoeDestTex;
    
    bool            simulatorMode;
};