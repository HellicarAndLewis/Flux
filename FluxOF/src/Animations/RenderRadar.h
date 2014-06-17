//
//  RenderRadar.h
//
//  Created by Patricio Gonzalez Vivo on 6/11/14.
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

class RenderRadar : public RenderEngine {
public:
    
    string getSystemName(){ return "RenderRadar";}
    
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
    
protected:
	
    // TERRAIN TRANSITION
    //
    UIShader        radarShader;
    ofFbo           radarTexture;
    
    UIShader        noiseShader;
    ofFbo           noiseTexture;

    UIShader        terrainTransition;
    PingPong        terrainTransitionTex;
    
    ofFbo           terrainTex;
    
    UIShader        terrainMeshTransition;
    ofFbo           terrainMeshTransitionTex;
    ofFbo           terrainMeshTex;
    
    //  SHOES   TRANSITION
    //
    UIShader        shoeTransition;
    
    //  RADAR
    //
    ofFloatColor    radarColor;
    ofPoint         radarCenter;
    float           radarPct;
    float           radarHeight;
};