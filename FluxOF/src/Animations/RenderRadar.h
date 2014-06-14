//
//  RenderRadar.h
//
//  Created by Patricio Gonzalez Vivo on 9/23/13.
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
#include "ofxBlur.h"
#include "ofxNormals.h"
#include "ofxDisplacePixels.h"

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
    
    ofxBlur         blur;
    ofxNormals      normalMap;
    ofxDisplacePixels   displace;
    
    UIShader        terrainTransition;
    PingPong        terrainTex;
 
    //  SHOES   TRANSITION
    //
    UIShader        shoeTransition;
};