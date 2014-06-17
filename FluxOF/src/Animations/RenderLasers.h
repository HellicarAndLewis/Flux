//
//  RenderLasers.h
//
//  Created by Patricio Gonzalez Vivo on 6/15/14.
//
//

#pragma once

#include "RenderEngine.h"

//  GEOM
//
#include "CalibrationLoader.h"
#include "ofxAssimpModelLoader.h"

//  FX
//
#include "UIShader.h"

class RenderLasers : public RenderEngine {
public:
    
    string getSystemName(){ return "RenderLasers";}
    
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
    
    bool transitionDone();
protected:
	
    // TERRAIN TRANSITION
    //
    UIShader        lasersMaskShader;
    ofFbo           lasersMask;
    
    UIShader        terrainTransition;
    PingPong        terrainTransitionTex;
    
    ofFbo           terrainTex[2];
    ofFbo           terrainMapPrev;
    
    //  SHOES   TRANSITION
    //
    UIShader        shoeTransition;

    //  LASER
    //
    ofFloatColor    laserColor;
    ofPoint         laserPosition;
};