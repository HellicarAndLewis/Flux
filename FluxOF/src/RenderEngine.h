//
//  RenderEngine.h
//  FluxOF
//
//  Created by Patricio Gonzalez Vivo on 6/14/14.
//
//

#pragma once

#include "ofxCv.h"
#include "ofxTimeline3DPro.h"

//  INPUTS
//
#include "UIBufferIn.h"
#include "QueueItem.h"
#include "CalibrationLoader.h"
#include "RenderAssets.h"

#include "PingPong.h"
#include "UIShader.h"

#include "ofxMultiGLFWWindow.h"
#include "ofxColorPalette.h"

#include "ofxSyphon.h"

class RenderEngine : public ofxTimeline3DPro {
public:
    
    virtual void addUIClass(UIClass *UIClass);
    virtual void setCalibration(CalibrationLoader *_calibration);
    
    virtual string getSystemName(){ return "RenderEngine-SuperClass";}
    
    virtual void startTransitionTo(QueueItem queueItem);
    virtual bool transitionDone();
        
    virtual void selfSetup();
    virtual void selfSetupRenderGui();

    virtual void selfUpdate();
    
    void startMatrixTranformation(int viewport, bool terrain);
    void endMatrixTranformation(int viewport, bool terrain);
    
    // Render pass fbo's
    ofFbo terrainTexture;
    ofFbo terrainMaskTexture;
    ofFbo shoeBackgroundTexture;
    ofFbo shoeForegroundTexture;
    
    PingPong renderPasses;
    PingPong renderTmpFbo;
    
    // Texture update calls
    virtual void updateTerrainTexture(ofPoint size);
    virtual void updateTerrainMaskTexture(ofPoint size, int viewport);
    virtual void updateShoeBackgroundTexture(ofPoint size);
    virtual void updateShoeForegroundTexture(ofPoint size);
    
    // Drawing calls
    virtual void drawTerrain(int viewport);
    virtual void drawTerrainMask(int viewport);
    virtual void drawShoeBackground(int viewport);
    virtual void drawShoeDetails(int viewport);
    virtual void drawShoeForeground(int viewport);
    virtual void drawShoeMask(int viewport);
    
    //Enable / disable stages
    bool terrainDrawEnabled;
    bool terrainMaskDrawEnabled;
    bool shoeBackgroundDrawEnabled;
    bool shoeDetailsDrawEnabled;
    bool shoeForegroundDrawEnabled;
    bool shoeMaskDrawEnabled;
    
    void draw(ofEventArgs & args);
    void selfPostDraw();
    ofFbo& getRenderTarget(int viewNumber);
    
    UIBufferIn          audioIn;
    
    CalibrationLoader   *calibration;
    RenderAssets        *assets;
    
    PingPong            shoeTex;
    
    vector<ofColor>     srcPalette;
    vector<ofColor>     dstPalette;
    
    ofPoint             textOffset;
    string              text;
    float               textAlpha;
    float               textScale;
    
    bool                simulatorMode;
    
    bool                terrainMaskSyphon;

    ofxMultiGLFWWindow  *glfw;
    
    ofxSyphonClient     syphon;
};