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

#include "ofxRipples.h"
#include "ofxNormals.h"

class RenderRadar : public RenderEngine {
public:
    
    string getSystemName(){ return "RenderRadar";}
    
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
    
    
    //Draw
    void drawTerrain(int viewport);
        
protected:
    
    //  AUDIO
    //
    UIShader        audioTerrain;
    ofxRipples      ripples;
    ofxNormals      ripplesNormals;
    ofFloatColor    ripplesColor;
    float           ripplesColorLerpToRadar;
    float           ripplesColorLerpToWhite;
    
    // TERRAIN TRANSITION
    //
    UIShader        radarShader;
    ofFbo           radarTexture;

    UIShader        terrainNoise;
    ofFbo           terrainNoiseTex;
    
    UIShader        terrainTransition;
    PingPong        terrainTransitionTex;
    
    ofFbo           terrainTex;
    UIShader        terrainShader;
    
    UIShader        terrainMeshShader;
    
    ofFbo           terrainMask[3];
    
    //  SHOES   TRANSITION
    //
    UIShader        shoeTransition;
    UIShader        shoeLaserTransition;
    
    //  RADAR
    //
    ofFloatColor    radarColor;
    ofPoint         radarCenter;
    float           radarPct;
    float           radarHeight;
    
    // TEST
    bool            testPatternEnabled;
    string          testMode;
    ofFbo           testFbo;
};