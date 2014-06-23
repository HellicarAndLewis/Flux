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

    void selfDrawOverlay();
    
    
    //Draw override the render rengine
    void drawTerrain(int viewport);
    void drawShoeBackground(int viewport);
    void drawShoeForeground(int viewport);
    
protected:
    
    //  AUDIO
    //
    UIShader        audioTerrain, audioTerrainDebug;
    ofxRipples      ripples;
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
    float           radarRadius;
    float           radarAlpha;
    float           radarRadiusAlpha;
    
    // TEST
    bool            testPatternEnabled;
    string          testMode;
    ofFbo           testFbo;
};