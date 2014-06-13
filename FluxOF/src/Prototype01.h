//
//  Prototype01.h
//
//  Created by Patricio Gonzalez Vivo on 9/23/13.
//
//

#pragma once

#include "UITime3DProject.h"

//  GEOM
//
#include "CalibrationLoader.h"
#include "ofxAssimpModelLoader.h"

//  FX
//
#include "UIShader.h"
#include "ofxBlur.h"
#include "ofxNormals.h"
#include "ofxDisplacePixels.h"

//  IN
//
#include "UIBufferIn.h"
#include "ColorPalette.h"

class Prototype01 : public UITime3DProject {
public:
    
    string getSystemName(){ return "Prototype01";}
    
    void selfSetupGuis();
    void selfGuiEvent(ofxUIEventArgs &e);
    
    void selfSetupSystemGui();
    void guiSystemEvent(ofxUIEventArgs &e);
    
    void selfSetupRenderGui();
    void guiRenderEvent(ofxUIEventArgs &e);
    
    void selfSetup();
    void selfUpdate();
    
    void selfDraw();
    void selfDrawOverlay();
    
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
    
    //Calibration
    //
    CalibrationLoader calibration;

    // TERRAIN
    //
    ofVboMesh       terrainMesh;
    ofTexture       terrainDepthMap;
    float           terrainResolution;
    
    UIShader        radarShader;
    ofFbo           radarTexture;
    
    UIShader        noiseShader;
    ofFbo           noiseTexture;
    
    ofxBlur         blur;
    ofxNormals      normalMap;
    ofxDisplacePixels   displace;
    
    UIShader        terrainTransition;
    PingPong        terrainTex;
 
    vector<ofColor> colorPalette;
    
    //  SHOES
    //
    ofVboMesh       shoeMesh;
    UIShader        shoeTransition;
    ofTexture       shoeTexA,shoeTexB;
    
    bool            simulatorMode;
};